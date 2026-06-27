//
// Copyright (c) 2017 The nanoFramework project contributors
// See LICENSE file in the project root for full license information.
//

#include "TouchInterface.h"
#include "Device.IO.h"
#include <driver/i2c_master.h>

#include <esp_lcd_touch.h>
#include "esp_lcd_touch_gt911.h"

TouchInterface g_TouchInterface;

#define BSP_I2C_SCL                               (GPIO_NUM_8)
#define BSP_I2C_SDA                               (GPIO_NUM_7)
#define CONFIG_BSP_I2C_NUM                        1
#define BSP_I2C_NUM                               CONFIG_BSP_I2C_NUM
#define BSP_LCD_H_RES                             (800)
#define BSP_LCD_V_RES                             (1280)
#define BSP_LCD_TOUCH_RST                         (GPIO_NUM_NC)
#define BSP_LCD_TOUCH_INT                         (GPIO_NUM_NC)
#define ESP_LCD_TOUCH_IO_I2C_GT911_ADDRESS        (0x5D)
#define ESP_LCD_TOUCH_IO_I2C_GT911_ADDRESS_BACKUP (0x14)
#define CONFIG_BSP_I2C_CLK_SPEED_HZ               400000

typedef struct
{
    struct
    {
        unsigned int swap_xy;  /*!< Swap X and Y after read coordinates */
        unsigned int mirror_x; /*!< Mirror X after read coordinates */
        unsigned int mirror_y; /*!< Mirror Y after read coordinates */
    } touch_flags;
} bsp_display_cfg_t;



static bool i2c_initialized = false;
static i2c_master_bus_handle_t i2c_handle = NULL; // I2C Handle
static esp_lcd_touch_handle_t tp = NULL;

static esp_err_t bsp_i2c_device_probe(uint8_t addr)
{
    return i2c_master_probe(i2c_handle, addr, 100);
}

esp_err_t bsp_i2c_init(void)
{
    /* I2C was initialized before */
    if (i2c_initialized)
    {
        return ESP_OK;
    }

    i2c_master_bus_config_t i2c_bus_conf = {
        .i2c_port = BSP_I2C_NUM,
        .sda_io_num = BSP_I2C_SDA,
        .scl_io_num = BSP_I2C_SCL,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 0,
        .intr_priority = 0,
        .trans_queue_depth = 0,
        .flags{.enable_internal_pullup = 0, .allow_pd = 0}};
    esp_err_t result = i2c_new_master_bus(&i2c_bus_conf, &i2c_handle);

    i2c_initialized = true;

    return result;
}
esp_err_t bsp_touch_new(esp_lcd_touch_handle_t *ret_touch)
{

    /* Initilize I2C */
    esp_err_t result = bsp_i2c_init();

    /* Initialize touch */
    const esp_lcd_touch_config_t tp_cfg = {
        .x_max = BSP_LCD_H_RES,
        .y_max = BSP_LCD_V_RES,
        .rst_gpio_num = BSP_LCD_TOUCH_RST, // Shared with LCD reset
        .int_gpio_num = BSP_LCD_TOUCH_INT,
        .levels =
            {
                .reset = 0,
                .interrupt = 0,
            },
        .flags =
            {
                .swap_xy = 0,
                .mirror_x = 0,
                .mirror_y = 0,
            },
        .process_coordinates = NULL,
        .interrupt_callback = NULL,
        .user_data = NULL,
        .driver_data = NULL

    };
    esp_lcd_panel_io_handle_t tp_io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t tp_io_config;
    // Touch 0x5d found;
    esp_lcd_panel_io_i2c_config_t config = {
        .dev_addr = ESP_LCD_TOUCH_IO_I2C_GT911_ADDRESS,
        .on_color_trans_done = NULL,
        .user_ctx = NULL,
        .control_phase_bytes = 1,
        .dc_bit_offset = 0,
        .lcd_cmd_bits = 16,
        .lcd_param_bits = 0,
        .flags =
            {
                .dc_low_on_data = 0,
                .disable_control_phase = 1,
            },
        .scl_speed_hz = 100000,
    };

    result = bsp_i2c_device_probe(ESP_LCD_TOUCH_IO_I2C_GT911_ADDRESS);
    if (result == ESP_OK)
    {
        memcpy(&tp_io_config, &config, sizeof(config));
    }
    else if (ESP_OK == bsp_i2c_device_probe(ESP_LCD_TOUCH_IO_I2C_GT911_ADDRESS_BACKUP))
    {
        // Touch 0x14 found;
        config.dev_addr = ESP_LCD_TOUCH_IO_I2C_GT911_ADDRESS_BACKUP;
        memcpy(&tp_io_config, &config, sizeof(config));
    }
    else
    {
        // Touch not found;
        return ESP_ERR_NOT_FOUND;
    }
    tp_io_config.scl_speed_hz = CONFIG_BSP_I2C_CLK_SPEED_HZ;

    // Replace
//    {
//        result = esp_lcd_new_panel_io_i2c(i2c_handle, &tp_io_config, &tp_io_handle);
        result = esp_lcd_touch_new_i2c_gt911(tp_io_handle, &tp_cfg, ret_touch);
//    }
    return ESP_OK;
}


// static CLR_UINT8 I2C_READ_BUFFER[32];
// static int m_TouchI2cBus;
// static int m_TouchI2cSlaveAddress;
//
bsp_display_cfg_t *cfg;

 
bool TouchInterface::Initialize()
{
    // m_TouchI2cBus = TouchI2cBus;
    // m_TouchI2cSlaveAddress = TouchI2cSlaveAddress;
    // I2cIO::Initialize(TouchI2cBus, I2cBusSpeed_StandardMode, I2C_CONTROL_TYPE::MASTER, TouchI2cSlaveAddress);

    bsp_touch_new(&tp);

    return true;
}
CLR_UINT8 *TouchInterface::Write_Read(CLR_UINT8 *writeBuffer, CLR_UINT16 writeSize, CLR_UINT16 readSize)
{
    // I2cTransferStatus writeStatus = I2cTransferStatus_UnknownError;
    // I2cTransferStatus readStatus = I2cTransferStatus_UnknownError;

    // writeStatus = I2cIO::Write(m_TouchI2cBus, m_TouchI2cSlaveAddress, writeBuffer, writeSize,
    // I2C_CONTROL_TYPE::MASTER); if (writeStatus != I2cTransferStatus_FullTransfer)
    //{
    //     return NULL;
    // }
    // if (readSize > 0)
    //{
    //     I2cIO::Read(m_TouchI2cBus, m_TouchI2cSlaveAddress, I2C_READ_BUFFER, readSize);
    // }
    // if (readStatus != I2cTransferStatus_FullTransfer)
    //{
    //     return I2C_READ_BUFFER;
    // }
    // else
    //{
    return NULL;
    //}
}


