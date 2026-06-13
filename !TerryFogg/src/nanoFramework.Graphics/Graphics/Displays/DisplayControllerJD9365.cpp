//
// Copyright (c) .NET Foundation and Contributors
// Portions Copyright (c) Microsoft Corporation.  All rights reserved.
// See LICENSE file in the project root for full license information.
//

// The JD9365 is a single‑chip driver IC for TFT LCD panels used in embedded displays.
// It combines source drivers, gate drivers, and control logic into one chip.
// Its main job is to convert digital image data into signals that physically drive the LCD pixels.
// It supports high‑resolution panels (e.g., 800×1280 WXGA) with millions of colors.
// The controller manages timing, scanning, and addressing of pixel rows and columns.
// It typically interfaces with the host via MIPI‑DSI (or similar display interfaces).
// It generates the analog voltages required to control liquid crystal cells.
// Built‑in power management circuits (DC/DC converters) provide panel drive voltages.
// It includes initialization and configuration command handling for panel setup.
// The chip ensures correct color rendering and refresh timing of the display.
// It handles low‑power operation modes to reduce energy usage in portable devices.

// NOTE:
// It does not store a full framebuffer, relying on the host to stream pixel data.
// Overall, it acts as the bridge between a microcontroller/SoC and the LCD glass, making the screen usable

#include "Display.h"
#include "DisplayInterface.h"
#include "DisplayControllerJD9365.h"
#include "DisplayInterfaceMipiDsi.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_lcd_mipi_dsi.h"
#include "esp_lcd_panel_commands.h"
#include "esp_lcd_panel_interface.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_ldo_regulator.h"
#include "esp_lcd_types.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "sd_pwr_ctrl_by_on_chip_ldo.h"
#include "sdkconfig.h"

#include "JD9365_data_waveshare_10_1.inc"

#define JD9365_DSI_2_LANE   (0x01)
#define JD9365_CMD_PAGE     (0xE0)
#define JD9365_PAGE_USER    (0x00)
#define JD9365_CMD_DSI_INT0 (0x80)

#define BSP_LCD_BACKLIGHT              (GPIO_NUM_26)
#define BSP_LCD_RST                    (GPIO_NUM_27)
#define LCD_LEDC_CH                    (1)
#define CONFIG_BSP_LCD_DPI_BUFFER_NUMS (3)

static esp_err_t panel_jd9365_del(esp_lcd_panel_t *panel);
static esp_err_t panel_jd9365_init(esp_lcd_panel_t *panel);
static esp_err_t panel_jd9365_reset(esp_lcd_panel_t *panel);
static esp_err_t panel_jd9365_invert_color(esp_lcd_panel_t *panel, bool invert_color_data);
static esp_err_t panel_jd9365_mirror(esp_lcd_panel_t *panel, bool mirror_x, bool mirror_y);
static esp_err_t panel_jd9365_disp_on_off(esp_lcd_panel_t *panel, bool on_off);

bsp_lcd_handles_t handles;
esp_lcd_panel_handle_t disp_panel = NULL;

struct DisplayDriver g_DisplayDriver;
extern DisplayInterface g_DisplayInterface;
extern DisplayInterfaceConfig g_DisplayInterfaceConfig;
extern esp_lcd_dsi_bus_handle_t mipi_dsi_bus;
extern esp_lcd_panel_io_handle_t io_handle;

SemaphoreHandle_t refresh_finish;

IRAM_ATTR static bool test_notify_refresh_ready(
    esp_lcd_panel_handle_t panel,
    esp_lcd_dpi_panel_event_data_t *edata,
    void *user_ctx)
{
    refresh_finish = (SemaphoreHandle_t)user_ctx;
    BaseType_t need_yield = pdFALSE;

    xSemaphoreGiveFromISR(refresh_finish, &need_yield);

    return (need_yield == pdTRUE);
}

static void test_draw_color_bar(esp_lcd_panel_handle_t panel_handle, uint16_t h_res, uint16_t v_res)
{
    refresh_finish = xSemaphoreCreateBinary();
    esp_lcd_dpi_panel_event_callbacks_t cbs = {
        .on_color_trans_done = test_notify_refresh_ready,
        .on_refresh_done = NULL};
    esp_lcd_dpi_panel_register_event_callbacks(disp_panel, &cbs, refresh_finish);

    uint8_t byte_per_pixel = (16 + 7) / 8;
    uint16_t row_line = v_res / byte_per_pixel / 8;
    uint8_t *color = (uint8_t *)heap_caps_calloc(1, row_line * h_res * byte_per_pixel, MALLOC_CAP_DMA);

    for (int j = 0; j < byte_per_pixel * 8; j++)
    {
        for (int i = 0; i < row_line * h_res; i++)
        {
            for (int k = 0; k < byte_per_pixel; k++)
            {
                color[i * byte_per_pixel + k] = (BIT(j) >> (k * 8)) & 0xff;
            }
        }
        esp_lcd_panel_draw_bitmap(panel_handle, 0, j * row_line, h_res, (j + 1) * row_line, color);
        xSemaphoreTake(refresh_finish, portMAX_DELAY);
    }

    uint16_t color_line = row_line * byte_per_pixel * 8;
    uint16_t res_line = v_res - color_line;
    if (res_line)
    {
        for (int i = 0; i < res_line * h_res; i++)
        {
            for (int k = 0; k < byte_per_pixel; k++)
            {
                color[i * byte_per_pixel + k] = 0xff;
            }
        }
        esp_lcd_panel_draw_bitmap(panel_handle, 0, color_line, h_res, v_res, color);
        xSemaphoreTake(refresh_finish, portMAX_DELAY);
    }
}

bool DisplayDriver::Initialize()
{
    SetupDisplayAttributes();
    SetDefaultOrientation();

    // Initialize the backlight control using LEDC
    {
        const ledc_channel_config_t LCD_backlight_channel = {
            .gpio_num = BSP_LCD_BACKLIGHT,
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .channel = LEDC_CHANNEL_1,
            .intr_type = LEDC_INTR_DISABLE,
            .timer_sel = LEDC_TIMER_1,
            .duty = 0,
            .hpoint = 0,
            .sleep_mode = LEDC_SLEEP_MODE_NO_ALIVE_NO_PD,
            .flags = {.output_invert = 0}};
        const ledc_timer_config_t LCD_backlight_timer = {
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .duty_resolution = LEDC_TIMER_10_BIT,
            .timer_num = LEDC_TIMER_1,
            .freq_hz = 5000,
            .clk_cfg = LEDC_AUTO_CLK,
            .deconfigure = false};
        ledc_timer_config(&LCD_backlight_timer);
        ledc_channel_config(&LCD_backlight_channel);
    }

    DisplayBrightness(100);

    // Create the Panel
    {
        esp_lcd_dpi_panel_config_t dpi_config = {
            .virtual_channel = 0,
            .dpi_clk_src = MIPI_DSI_DPI_CLK_SRC_DEFAULT,
            .dpi_clock_freq_mhz = 80,
            .pixel_format = LCD_COLOR_PIXEL_FORMAT_RGB565,
            .in_color_format = LCD_COLOR_FMT_RGB565,
            .out_color_format = LCD_COLOR_FMT_RGB565,
            .num_fbs = 1,
            .video_timing =
                {
                    .h_size = 800,
                    .v_size = 1280,
                    .hsync_pulse_width = 20,
                    .hsync_back_porch = 20,
                    .hsync_front_porch = 40,
                    .vsync_pulse_width = 4,
                    .vsync_back_porch = 12,
                    .vsync_front_porch = 30,
                },
            .flags =
                {
                    .use_dma2d = false,
                    .disable_lp = false,
                },
        };

        dpi_config.num_fbs = CONFIG_BSP_LCD_DPI_BUFFER_NUMS;

        jd9365_vendor_config_t vendor_config = {
            .init_cmds = jd9365_vendor_specific_init_default,
            .init_cmds_size =
                sizeof(jd9365_vendor_specific_init_default) / sizeof(jd9365_vendor_specific_init_default[0]),
            .mipi_config = {
                .dsi_bus = mipi_dsi_bus,
                .dpi_config = &dpi_config,
                .lane_num = 2,
            }};
        esp_lcd_panel_dev_config_t lcd_dev_config = {
            .reset_gpio_num = BSP_LCD_RST,
            .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
            .data_endian = LCD_RGB_DATA_ENDIAN_BIG,
            .bits_per_pixel = 16,
            .flags = {.reset_active_high = 0},
            .vendor_config = &vendor_config,
        };
        esp_lcd_new_panel_jd9365(io_handle, &lcd_dev_config, &disp_panel);
    }

    esp_lcd_panel_reset(disp_panel);
    esp_lcd_panel_init(disp_panel);
    esp_lcd_panel_disp_on_off(disp_panel, true);

    // Save the handles for later use
    handles.io = io_handle;
    handles.mipi_dsi_bus = mipi_dsi_bus;
    handles.panel = disp_panel;
    handles.control = NULL;

    esp_lcd_dpi_panel_set_pattern(disp_panel, MIPI_DSI_PATTERN_BAR_VERTICAL);

    vTaskDelay(pdMS_TO_TICKS(3000));
    test_draw_color_bar(disp_panel, 800, 1200);

    return true;
}

void DisplayDriver::SetupDisplayAttributes()
{
    // Define the LCD/TFT resolution
    Attributes.LongerSide = g_DisplayInterfaceConfig.Screen.width;
    Attributes.ShorterSide = g_DisplayInterfaceConfig.Screen.height;
    Attributes.PowerSave = PowerSaveState::NORMAL;
    Attributes.BitsPerPixel = 16;
    g_DisplayInterface.GetTransferBuffer(Attributes.TransferBuffer, Attributes.TransferBufferSize);
    return;
}

bool DisplayDriver::ChangeOrientation(DisplayOrientation orientation)
{
    switch (orientation)
    {
        case DisplayOrientation::DisplayOrientation_Portrait:
            Attributes.Height = Attributes.ShorterSide;
            Attributes.Width = Attributes.LongerSide;
            break;

        case DisplayOrientation::DisplayOrientation_Portrait180:
            Attributes.Height = Attributes.ShorterSide;
            Attributes.Width = Attributes.LongerSide;
            break;

        case DisplayOrientation::DisplayOrientation_Landscape:
            Attributes.Height = Attributes.ShorterSide;
            Attributes.Width = Attributes.LongerSide;
            break;

        case DisplayOrientation::DisplayOrientation_Landscape180:
            Attributes.Height = Attributes.ShorterSide;
            Attributes.Width = Attributes.LongerSide;
            break;
    }
    return true;
}

void DisplayDriver::SetDefaultOrientation()
{
    ChangeOrientation(DisplayOrientation::DisplayOrientation_Landscape);
}

bool DisplayDriver::Uninitialize()
{
    // Unsupported, but return true to avoid error
    return TRUE;
}

void DisplayDriver::PowerSave(PowerSaveState powerState)
{
    switch (powerState)
    {
        case PowerSaveState::NORMAL:
            break;
        case PowerSaveState::SLEEP:
            break;
        default:
            break;
    }
    return;
}

void DisplayDriver::Clear()
{
    memset(Attributes.TransferBuffer, 0, Attributes.TransferBufferSize);
    return;
}

void DisplayDriver::DisplayBrightness(CLR_INT16 brightness_percent)
{
    brightness_percent = (brightness_percent > 100) ? 100 : (brightness_percent < 0) ? 0 : brightness_percent;
    uint32_t duty_cycle = (1023 * brightness_percent) / 100;
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, duty_cycle);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
    return;
}

bool DisplayDriver::SetWindow(CLR_INT16 x1, CLR_INT16 y1, CLR_INT16 x2, CLR_INT16 y2)
{
    // Not supported for MIPI DSI panels, as the window is set by the host when sending pixel data
    // eg.
    // esp_lcd_panel_draw_bitmap(disp_panel, x1, y1, x2, y2, data);
    return true;
}

void DisplayDriver::BitBlt(
    int srcX,
    int srcY,
    int width,
    int height,
    int stride,
    int screenX,
    int screenY,
    CLR_UINT32 data[])
{
}

CLR_UINT32 DisplayDriver::PixelsPerWord()
{
    return (32 / Attributes.BitsPerPixel);
}

CLR_UINT32 DisplayDriver::WidthInWords()
{
    return ((Attributes.Width + (PixelsPerWord() - 1)) / PixelsPerWord());
}

CLR_UINT32 DisplayDriver::SizeInWords()
{
    return (WidthInWords() * Attributes.Height);
}

CLR_UINT32 DisplayDriver::SizeInBytes()
{
    return (SizeInWords() * sizeof(CLR_UINT32));
}

bool bsp_display_brightness_init(void)
{
    const ledc_channel_config_t LCD_backlight_channel = {
        .gpio_num = BSP_LCD_BACKLIGHT,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_1,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_1,
        .duty = 0,
        .hpoint = 0,
        .sleep_mode = LEDC_SLEEP_MODE_NO_ALIVE_NO_PD,
        .flags = {.output_invert = 0}};
    const ledc_timer_config_t LCD_backlight_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .timer_num = LEDC_TIMER_1,
        .freq_hz = 5000,
        .clk_cfg = LEDC_AUTO_CLK,
        .deconfigure = false};

    ledc_timer_config(&LCD_backlight_timer);
    ledc_channel_config(&LCD_backlight_channel);
    return true;
}

typedef struct
{
    esp_lcd_panel_io_handle_t io;
    gpio_num_t reset_gpio_num;
    uint8_t madctl_val;
    uint8_t colmod_val;
    const jd9365_lcd_init_cmd_t *init_cmds;
    uint16_t init_cmds_size;
    uint8_t lane_num;
    struct
    {
        unsigned int reset_level : 1;
    } flags;
    // To save the original functions of MIPI DPI panel
    esp_err_t (*del)(esp_lcd_panel_t *panel);
    esp_err_t (*init)(esp_lcd_panel_t *panel);
} jd9365_panel_t;

esp_err_t esp_lcd_new_panel_jd9365(
    const esp_lcd_panel_io_handle_t io,
    const esp_lcd_panel_dev_config_t *panel_dev_config,
    esp_lcd_panel_handle_t *ret_panel)
{
    jd9365_vendor_config_t *vendor_config = (jd9365_vendor_config_t *)panel_dev_config->vendor_config;

    jd9365_panel_t *jd9365 = (jd9365_panel_t *)calloc(1, sizeof(jd9365_panel_t));

    if (panel_dev_config->reset_gpio_num >= 0)
    {
        gpio_config_t io_conf = {
            .pin_bit_mask = 1ULL << panel_dev_config->reset_gpio_num,
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
            .hys_ctrl_mode = GPIO_HYS_SOFT_DISABLE};
        gpio_config(&io_conf);
    }

    jd9365->madctl_val = 0;
    jd9365->colmod_val = 0x55;
    jd9365->io = io;
    jd9365->init_cmds = vendor_config->init_cmds;
    jd9365->init_cmds_size = vendor_config->init_cmds_size;
    jd9365->lane_num = vendor_config->mipi_config.lane_num;
    jd9365->reset_gpio_num = (gpio_num_t)panel_dev_config->reset_gpio_num;
    jd9365->flags.reset_level = panel_dev_config->flags.reset_active_high;

    // Create MIPI DPI panel
    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_new_panel_dpi(vendor_config->mipi_config.dsi_bus, vendor_config->mipi_config.dpi_config, &panel_handle);

    // Save the original functions of MIPI DPI panel
    jd9365->del = panel_handle->del;
    jd9365->init = panel_handle->init;
    // Overwrite the functions of MIPI DPI panel
    panel_handle->del = panel_jd9365_del;
    panel_handle->init = panel_jd9365_init;
    panel_handle->reset = panel_jd9365_reset;
    panel_handle->mirror = panel_jd9365_mirror;
    panel_handle->invert_color = panel_jd9365_invert_color;
    panel_handle->disp_on_off = panel_jd9365_disp_on_off;
    panel_handle->user_data = jd9365;
    *ret_panel = panel_handle;

    return ESP_OK;
}

static esp_err_t panel_jd9365_del(esp_lcd_panel_t *panel)
{
    return ESP_OK;
}
static esp_err_t panel_jd9365_init(esp_lcd_panel_t *panel)
{
    jd9365_panel_t *jd9365 = (jd9365_panel_t *)panel->user_data;
    esp_lcd_panel_io_handle_t io = jd9365->io;
    const jd9365_lcd_init_cmd_t *init_cmds = NULL;
    uint16_t init_cmds_size = 0;
    uint8_t lane_command = JD9365_DSI_2_LANE;
    bool is_user_set = true;
    bool is_cmd_overwritten = false;
    uint8_t ID[3];

    esp_lcd_panel_io_rx_param(io, 0x04, ID, 3);
    esp_lcd_panel_io_tx_param(io, JD9365_CMD_PAGE, (uint8_t[]){JD9365_PAGE_USER}, 1);
    esp_lcd_panel_io_tx_param(
        io,
        LCD_CMD_MADCTL,
        (uint8_t[]){
            jd9365->madctl_val,
        },
        1);
    esp_lcd_panel_io_tx_param(
        io,
        LCD_CMD_COLMOD,
        (uint8_t[]){
            jd9365->colmod_val,
        },
        1);
    esp_lcd_panel_io_tx_param(
        io,
        JD9365_CMD_DSI_INT0,
        (uint8_t[]){
            lane_command,
        },
        1);

    init_cmds = jd9365->init_cmds;
    init_cmds_size = jd9365->init_cmds_size;

    for (int i = 0; i < init_cmds_size; i++)
    {
        // Check if the command has been used or conflicts with the internal
        if (is_user_set && (init_cmds[i].data_bytes > 0))
        {
            switch (init_cmds[i].cmd)
            {
                case LCD_CMD_MADCTL:
                    is_cmd_overwritten = true;
                    jd9365->madctl_val = ((uint8_t *)init_cmds[i].data)[0];
                    break;
                case LCD_CMD_COLMOD:
                    is_cmd_overwritten = true;
                    jd9365->colmod_val = ((uint8_t *)init_cmds[i].data)[0];
                    break;
                default:
                    is_cmd_overwritten = false;
                    break;
            }

            if (is_cmd_overwritten)
            {
                is_cmd_overwritten = false;
            }
        }

        // Send command
        esp_lcd_panel_io_tx_param(io, init_cmds[i].cmd, init_cmds[i].data, init_cmds[i].data_bytes);
        vTaskDelay(pdMS_TO_TICKS(init_cmds[i].delay_ms));

        // Check if the current cmd is the "page set" cmd
        if ((init_cmds[i].cmd == JD9365_CMD_PAGE) && (init_cmds[i].data_bytes > 0))
        {
            is_user_set = (((uint8_t *)init_cmds[i].data)[0] == JD9365_PAGE_USER);
        }
    }
    jd9365->init(panel);
    return ESP_OK;
}

static esp_err_t panel_jd9365_reset(esp_lcd_panel_t *panel)
{
    jd9365_panel_t *jd9365 = (jd9365_panel_t *)panel->user_data;
    esp_lcd_panel_io_handle_t io = jd9365->io;

    // Perform hardware reset
    if (jd9365->reset_gpio_num >= 0)
    {
        gpio_set_level(jd9365->reset_gpio_num, !jd9365->flags.reset_level);
        vTaskDelay(pdMS_TO_TICKS(5));
        gpio_set_level(jd9365->reset_gpio_num, jd9365->flags.reset_level);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level(jd9365->reset_gpio_num, !jd9365->flags.reset_level);
        vTaskDelay(pdMS_TO_TICKS(120));
    }
    else if (io)
    { // Perform software reset
        esp_lcd_panel_io_tx_param(io, LCD_CMD_SWRESET, NULL, 0);
        vTaskDelay(pdMS_TO_TICKS(120));
    }
    return ESP_OK;
}

static esp_err_t panel_jd9365_invert_color(esp_lcd_panel_t *panel, bool invert_color_data)
{
    return ESP_OK;
}

static esp_err_t panel_jd9365_mirror(esp_lcd_panel_t *panel, bool mirror_x, bool mirror_y)
{
    return ESP_OK;
}

static esp_err_t panel_jd9365_disp_on_off(esp_lcd_panel_t *panel, bool on_off)
{
    jd9365_panel_t *jd9365 = (jd9365_panel_t *)panel->user_data;
    esp_lcd_panel_io_handle_t io = jd9365->io;
    if (on_off)
    {
        esp_lcd_panel_io_tx_param(io, LCD_CMD_DISPON, NULL, 0);
    }
    else
    {
        esp_lcd_panel_io_tx_param(io, LCD_CMD_DISPOFF, NULL, 0);
    }
    return ESP_OK;
}
