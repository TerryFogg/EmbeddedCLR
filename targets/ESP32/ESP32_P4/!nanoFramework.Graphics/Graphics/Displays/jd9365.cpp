//
// Copyright (c) .NET Foundation and Contributors
// Portions Copyright (c) Microsoft Corporation.  All rights reserved.
// See LICENSE file in the project root for full license information.
//

#include "Graphics.h"
#include "DisplayInterface.h"
#include "Display.h"
#include "C:/nftools/espressif/v5.5.4/esp-idf/components/esp_hw_support/ldo/include/esp_ldo_regulator.h"
//#include "esp_lcd_panel_ops.h"
//#include "esp_lcd_mipi_dsi.h"
//#include "esp_lcd_types.h"
//#include "esp_lcd_panel_dev.h"

#include "esp_err.h"
#include "esp_check.h"

//#include "esp_ldo_regulator.h"
#include "sd_pwr_ctrl_by_on_chip_ldo.h"

//#include "bsp/esp_lcd_jd9365_10_1.h"
//#include "bsp/esp32_p4_wifi6_lcd_x.h"
//#include "bsp/display.h"
//#include "bsp_err_check.h"

#include "driver/gpio.h"
#include "driver/ledc.h"
//#include "driver/i2c_types.h"
//#include "driver/i2c_master.h"
//#include "esp_lcd_panel_dev.h"

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


#define BSP_LCD_BACKLIGHT (GPIO_NUM_26)
#define BSP_LCD_RST       (GPIO_NUM_27)

#define CONFIG_BSP_DISPLAY_BRIGHTNESS_LEDC_CH 1
#define LCD_LEDC_CH       CONFIG_BSP_DISPLAY_BRIGHTNESS_LEDC_CH

typedef struct
{
    int cmd;               // The specific LCD command */
    const void *data;      // Buffer that holds the command specific data */
    size_t data_bytes;     // Size of `data` in memory, in bytes */
    unsigned int delay_ms; // Delay in milliseconds after this command */
} jd9365_lcd_init_cmd_t;

#pragma region jd9365 initialization data
static const jd9365_lcd_init_cmd_t jd9365_vendor_specific_init_default[] = {
    {0xE0, (uint8_t[]){0x00}, 1, 0},
    {0xE1, (uint8_t[]){0x93}, 1, 0},
    {0xE2, (uint8_t[]){0x65}, 1, 0},
    {0xE3, (uint8_t[]){0xF8}, 1, 0},
    {0x80, (uint8_t[]){0x01}, 1, 0},

    {0xE0, (uint8_t[]){0x01}, 1, 0},
    {0x00, (uint8_t[]){0x00}, 1, 0},
    {0x01, (uint8_t[]){0x38}, 1, 0},
    {0x03, (uint8_t[]){0x10}, 1, 0},
    {0x04, (uint8_t[]){0x38}, 1, 0},

    {0x0C, (uint8_t[]){0x74}, 1, 0},

    {0x17, (uint8_t[]){0x00}, 1, 0},
    {0x18, (uint8_t[]){0xAF}, 1, 0},
    {0x19, (uint8_t[]){0x00}, 1, 0},
    {0x1A, (uint8_t[]){0x00}, 1, 0},
    {0x1B, (uint8_t[]){0xAF}, 1, 0},
    {0x1C, (uint8_t[]){0x00}, 1, 0},

    {0x35, (uint8_t[]){0x26}, 1, 0},

    {0x37, (uint8_t[]){0x09}, 1, 0},

    {0x38, (uint8_t[]){0x04}, 1, 0},
    {0x39, (uint8_t[]){0x00}, 1, 0},
    {0x3A, (uint8_t[]){0x01}, 1, 0},
    {0x3C, (uint8_t[]){0x78}, 1, 0},
    {0x3D, (uint8_t[]){0xFF}, 1, 0},
    {0x3E, (uint8_t[]){0xFF}, 1, 0},
    {0x3F, (uint8_t[]){0x7F}, 1, 0},

    {0x40, (uint8_t[]){0x06}, 1, 0},
    {0x41, (uint8_t[]){0xA0}, 1, 0},
    {0x42, (uint8_t[]){0x81}, 1, 0},
    {0x43, (uint8_t[]){0x1E}, 1, 0},
    {0x44, (uint8_t[]){0x0D}, 1, 0},
    {0x45, (uint8_t[]){0x28}, 1, 0},
    //{0x4A, (uint8_t[]){0x35}, 1, 0},//bist

    {0x55, (uint8_t[]){0x02}, 1, 0},
    {0x57, (uint8_t[]){0x69}, 1, 0},
    {0x59, (uint8_t[]){0x0A}, 1, 0},
    {0x5A, (uint8_t[]){0x2A}, 1, 0},
    {0x5B, (uint8_t[]){0x17}, 1, 0},

    {0x5D, (uint8_t[]){0x7F}, 1, 0},
    {0x5E, (uint8_t[]){0x6A}, 1, 0},
    {0x5F, (uint8_t[]){0x5B}, 1, 0},
    {0x60, (uint8_t[]){0x4F}, 1, 0},
    {0x61, (uint8_t[]){0x4A}, 1, 0},
    {0x62, (uint8_t[]){0x3D}, 1, 0},
    {0x63, (uint8_t[]){0x41}, 1, 0},
    {0x64, (uint8_t[]){0x2A}, 1, 0},
    {0x65, (uint8_t[]){0x44}, 1, 0},
    {0x66, (uint8_t[]){0x43}, 1, 0},
    {0x67, (uint8_t[]){0x44}, 1, 0},
    {0x68, (uint8_t[]){0x62}, 1, 0},
    {0x69, (uint8_t[]){0x52}, 1, 0},
    {0x6A, (uint8_t[]){0x59}, 1, 0},
    {0x6B, (uint8_t[]){0x4C}, 1, 0},
    {0x6C, (uint8_t[]){0x48}, 1, 0},
    {0x6D, (uint8_t[]){0x3A}, 1, 0},
    {0x6E, (uint8_t[]){0x26}, 1, 0},
    {0x6F, (uint8_t[]){0x00}, 1, 0},
    {0x70, (uint8_t[]){0x7F}, 1, 0},
    {0x71, (uint8_t[]){0x6A}, 1, 0},
    {0x72, (uint8_t[]){0x5B}, 1, 0},
    {0x73, (uint8_t[]){0x4F}, 1, 0},
    {0x74, (uint8_t[]){0x4A}, 1, 0},
    {0x75, (uint8_t[]){0x3D}, 1, 0},
    {0x76, (uint8_t[]){0x41}, 1, 0},
    {0x77, (uint8_t[]){0x2A}, 1, 0},
    {0x78, (uint8_t[]){0x44}, 1, 0},
    {0x79, (uint8_t[]){0x43}, 1, 0},
    {0x7A, (uint8_t[]){0x44}, 1, 0},
    {0x7B, (uint8_t[]){0x62}, 1, 0},
    {0x7C, (uint8_t[]){0x52}, 1, 0},
    {0x7D, (uint8_t[]){0x59}, 1, 0},
    {0x7E, (uint8_t[]){0x4C}, 1, 0},
    {0x7F, (uint8_t[]){0x48}, 1, 0},
    {0x80, (uint8_t[]){0x3A}, 1, 0},
    {0x81, (uint8_t[]){0x26}, 1, 0},
    {0x82, (uint8_t[]){0x00}, 1, 0},

    {0xE0, (uint8_t[]){0x02}, 1, 0},
    {0x00, (uint8_t[]){0x42}, 1, 0},
    {0x01, (uint8_t[]){0x42}, 1, 0},
    {0x02, (uint8_t[]){0x40}, 1, 0},
    {0x03, (uint8_t[]){0x40}, 1, 0},
    {0x04, (uint8_t[]){0x5E}, 1, 0},
    {0x05, (uint8_t[]){0x5E}, 1, 0},
    {0x06, (uint8_t[]){0x5F}, 1, 0},
    {0x07, (uint8_t[]){0x5F}, 1, 0},
    {0x08, (uint8_t[]){0x5F}, 1, 0},
    {0x09, (uint8_t[]){0x57}, 1, 0},
    {0x0A, (uint8_t[]){0x57}, 1, 0},
    {0x0B, (uint8_t[]){0x77}, 1, 0},
    {0x0C, (uint8_t[]){0x77}, 1, 0},
    {0x0D, (uint8_t[]){0x47}, 1, 0},
    {0x0E, (uint8_t[]){0x47}, 1, 0},
    {0x0F, (uint8_t[]){0x45}, 1, 0},
    {0x10, (uint8_t[]){0x45}, 1, 0},
    {0x11, (uint8_t[]){0x4B}, 1, 0},
    {0x12, (uint8_t[]){0x4B}, 1, 0},
    {0x13, (uint8_t[]){0x49}, 1, 0},
    {0x14, (uint8_t[]){0x49}, 1, 0},
    {0x15, (uint8_t[]){0x5F}, 1, 0},

    {0x16, (uint8_t[]){0x41}, 1, 0},
    {0x17, (uint8_t[]){0x41}, 1, 0},
    {0x18, (uint8_t[]){0x40}, 1, 0},
    {0x19, (uint8_t[]){0x40}, 1, 0},
    {0x1A, (uint8_t[]){0x5E}, 1, 0},
    {0x1B, (uint8_t[]){0x5E}, 1, 0},
    {0x1C, (uint8_t[]){0x5F}, 1, 0},
    {0x1D, (uint8_t[]){0x5F}, 1, 0},
    {0x1E, (uint8_t[]){0x5F}, 1, 0},
    {0x1F, (uint8_t[]){0x57}, 1, 0},
    {0x20, (uint8_t[]){0x57}, 1, 0},
    {0x21, (uint8_t[]){0x77}, 1, 0},
    {0x22, (uint8_t[]){0x77}, 1, 0},
    {0x23, (uint8_t[]){0x46}, 1, 0},
    {0x24, (uint8_t[]){0x46}, 1, 0},
    {0x25, (uint8_t[]){0x44}, 1, 0},
    {0x26, (uint8_t[]){0x44}, 1, 0},
    {0x27, (uint8_t[]){0x4A}, 1, 0},
    {0x28, (uint8_t[]){0x4A}, 1, 0},
    {0x29, (uint8_t[]){0x48}, 1, 0},
    {0x2A, (uint8_t[]){0x48}, 1, 0},
    {0x2B, (uint8_t[]){0x5F}, 1, 0},

    {0x2C, (uint8_t[]){0x01}, 1, 0},
    {0x2D, (uint8_t[]){0x01}, 1, 0},
    {0x2E, (uint8_t[]){0x00}, 1, 0},
    {0x2F, (uint8_t[]){0x00}, 1, 0},
    {0x30, (uint8_t[]){0x1F}, 1, 0},
    {0x31, (uint8_t[]){0x1F}, 1, 0},
    {0x32, (uint8_t[]){0x1E}, 1, 0},
    {0x33, (uint8_t[]){0x1E}, 1, 0},
    {0x34, (uint8_t[]){0x1F}, 1, 0},
    {0x35, (uint8_t[]){0x17}, 1, 0},
    {0x36, (uint8_t[]){0x17}, 1, 0},
    {0x37, (uint8_t[]){0x37}, 1, 0},
    {0x38, (uint8_t[]){0x37}, 1, 0},
    {0x39, (uint8_t[]){0x08}, 1, 0},
    {0x3A, (uint8_t[]){0x08}, 1, 0},
    {0x3B, (uint8_t[]){0x0A}, 1, 0},
    {0x3C, (uint8_t[]){0x0A}, 1, 0},
    {0x3D, (uint8_t[]){0x04}, 1, 0},
    {0x3E, (uint8_t[]){0x04}, 1, 0},
    {0x3F, (uint8_t[]){0x06}, 1, 0},
    {0x40, (uint8_t[]){0x06}, 1, 0},
    {0x41, (uint8_t[]){0x1F}, 1, 0},

    {0x42, (uint8_t[]){0x02}, 1, 0},
    {0x43, (uint8_t[]){0x02}, 1, 0},
    {0x44, (uint8_t[]){0x00}, 1, 0},
    {0x45, (uint8_t[]){0x00}, 1, 0},
    {0x46, (uint8_t[]){0x1F}, 1, 0},
    {0x47, (uint8_t[]){0x1F}, 1, 0},
    {0x48, (uint8_t[]){0x1E}, 1, 0},
    {0x49, (uint8_t[]){0x1E}, 1, 0},
    {0x4A, (uint8_t[]){0x1F}, 1, 0},
    {0x4B, (uint8_t[]){0x17}, 1, 0},
    {0x4C, (uint8_t[]){0x17}, 1, 0},
    {0x4D, (uint8_t[]){0x37}, 1, 0},
    {0x4E, (uint8_t[]){0x37}, 1, 0},
    {0x4F, (uint8_t[]){0x09}, 1, 0},
    {0x50, (uint8_t[]){0x09}, 1, 0},
    {0x51, (uint8_t[]){0x0B}, 1, 0},
    {0x52, (uint8_t[]){0x0B}, 1, 0},
    {0x53, (uint8_t[]){0x05}, 1, 0},
    {0x54, (uint8_t[]){0x05}, 1, 0},
    {0x55, (uint8_t[]){0x07}, 1, 0},
    {0x56, (uint8_t[]){0x07}, 1, 0},
    {0x57, (uint8_t[]){0x1F}, 1, 0},

    {0x58, (uint8_t[]){0x40}, 1, 0},
    {0x5B, (uint8_t[]){0x30}, 1, 0},
    {0x5C, (uint8_t[]){0x00}, 1, 0},
    {0x5D, (uint8_t[]){0x34}, 1, 0},
    {0x5E, (uint8_t[]){0x05}, 1, 0},
    {0x5F, (uint8_t[]){0x02}, 1, 0},
    {0x63, (uint8_t[]){0x00}, 1, 0},
    {0x64, (uint8_t[]){0x6A}, 1, 0},
    {0x67, (uint8_t[]){0x73}, 1, 0},
    {0x68, (uint8_t[]){0x07}, 1, 0},
    {0x69, (uint8_t[]){0x08}, 1, 0},
    {0x6A, (uint8_t[]){0x6A}, 1, 0},
    {0x6B, (uint8_t[]){0x08}, 1, 0},

    {0x6C, (uint8_t[]){0x00}, 1, 0},
    {0x6D, (uint8_t[]){0x00}, 1, 0},
    {0x6E, (uint8_t[]){0x00}, 1, 0},
    {0x6F, (uint8_t[]){0x88}, 1, 0},

    {0x75, (uint8_t[]){0xFF}, 1, 0},
    {0x77, (uint8_t[]){0xDD}, 1, 0},
    {0x78, (uint8_t[]){0x2C}, 1, 0},
    {0x79, (uint8_t[]){0x15}, 1, 0},
    {0x7A, (uint8_t[]){0x17}, 1, 0},
    {0x7D, (uint8_t[]){0x14}, 1, 0},
    {0x7E, (uint8_t[]){0x82}, 1, 0},

    {0xE0, (uint8_t[]){0x04}, 1, 0},
    {0x00, (uint8_t[]){0x0E}, 1, 0},
    {0x02, (uint8_t[]){0xB3}, 1, 0},
    {0x09, (uint8_t[]){0x61}, 1, 0},
    {0x0E, (uint8_t[]){0x48}, 1, 0},
    {0x37, (uint8_t[]){0x58}, 1, 0}, // 全志
    {0x2B, (uint8_t[]){0x0F}, 1, 0}, // 全志

    {0xE0, (uint8_t[]){0x00}, 1, 0},

    {0xE6, (uint8_t[]){0x02}, 1, 0},
    {0xE7, (uint8_t[]){0x0C}, 1, 0},

    {0x11, (uint8_t[]){0x00}, 1, 120},

    {0x29, (uint8_t[]){0x00}, 1, 20},
};
#pragma endregion

struct DisplayDriver g_DisplayDriver;
extern DisplayInterface g_DisplayInterface;
extern DisplayInterfaceConfig g_DisplayInterfaceConfig;

bool DisplayDriver::Initialize()
{
    SetupDisplayAttributes();
    // g_DisplayInterface.SendCommand(INITIALIZE);
    SetDefaultOrientation();
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
            // g_DisplayInterface.SendCommand( PORTRAIT )
            break;

        case DisplayOrientation::DisplayOrientation_Portrait180:
            Attributes.Height = Attributes.ShorterSide;
            Attributes.Width = Attributes.LongerSide;
            // g_DisplayInterface.SendCommand( PORTRAIT180 )
            break;

        case DisplayOrientation::DisplayOrientation_Landscape:
            Attributes.Height = Attributes.ShorterSide;
            Attributes.Width = Attributes.LongerSide;
            // g_DisplayInterface.SendCommand( LANDSCAPE )
            break;

        case DisplayOrientation::DisplayOrientation_Landscape180:
            Attributes.Height = Attributes.ShorterSide;
            Attributes.Width = Attributes.LongerSide;
            // g_DisplayInterface.SendCommand( LANDSCAPE180 )
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
    Clear();

    // Anything else to Uninitialize?
    return TRUE;
}

void DisplayDriver::PowerSave(PowerSaveState powerState)
{
    switch (powerState)
    {
        default:
            // Illegal fall through to Power on
        case PowerSaveState::NORMAL:
            // g_DisplayInterface.SendCommand(NORMAL)
            break;
        case PowerSaveState::SLEEP:
            // g_DisplayInterface.SendCommand(SLEEP)
            break;
    }
    return;
}

void DisplayDriver::Clear()
{
    SetWindow(0, 0, Attributes.Width - 1, Attributes.Height - 1);
    //  g_DisplayInterface.SendCommand(CLEAR);
}

void DisplayDriver::DisplayBrightness(CLR_INT16 brightness_percent)
{
    if (brightness_percent > 100)
    {
        brightness_percent = 100;
    }
    else if (brightness_percent < 0)
    {
        brightness_percent = 0;
    }
    // Bit number used to represent command and parameter

    // Setup LEDC peripheral for PWM backlight control
    const ledc_channel_config_t LCD_backlight_channel = {
        .gpio_num = BSP_LCD_BACKLIGHT,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = (ledc_channel_t)LCD_LEDC_CH,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = (ledc_timer_t)1,
        .duty = 0,
        .hpoint = 0,
        .sleep_mode = LEDC_SLEEP_MODE_NO_ALIVE_NO_PD,
        .flags = 0};
    const ledc_timer_config_t LCD_backlight_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .timer_num = (ledc_timer_t)1,
        .freq_hz = 5000,
        .clk_cfg = LEDC_AUTO_CLK,
        .deconfigure = false};

    ledc_timer_config(&LCD_backlight_timer);
    ledc_channel_config(&LCD_backlight_channel);

    return;
}

bool DisplayDriver::SetWindow(CLR_INT16 x1, CLR_INT16 y1, CLR_INT16 x2, CLR_INT16 y2)
{
    //   g_DisplayInterfaceConfig.Screen.x
    //   g_DisplayInterfaceConfig.Screen.y
    //   g_DisplayInterface.SendCommand(WINDOW);
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
    // 16 bit colour  RRRRRGGGGGGBBBBB mode 565

    ASSERT((screenX >= 0) && ((screenX + width) <= Attributes.Width));
    ASSERT((screenY >= 0) && ((screenY + height) <= Attributes.Height));

    SetWindow(screenX, screenY, (screenX + width - 1), (screenY + height - 1));

    // g_DisplayInterface.SendCommand(COPY MEMORY);


    return;
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
