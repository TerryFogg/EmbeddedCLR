#pragma once

#include "driver/gpio.h"
#include "bsp/display.h"
#include "esp_err.h"
#include "driver/i2c_master.h"


/**************************************************************************************************
 *  BSP Capabilities
 **************************************************************************************************/

#define BSP_CAPS_DISPLAY       1
#define BSP_CAPS_TOUCH         1
#define BSP_CAPS_BUTTONS       0
#define BSP_CAPS_AUDIO         1
#define BSP_CAPS_AUDIO_SPEAKER 1
#define BSP_CAPS_AUDIO_MIC     1
#define BSP_CAPS_SDCARD        1
#define BSP_CAPS_IMU           0

/**************************************************************************************************
 *  ESP-BOX pinout
 **************************************************************************************************/
/* I2C */
#define BSP_I2C_SCL (GPIO_NUM_8)
#define BSP_I2C_SDA (GPIO_NUM_7)

/* Audio */
#define BSP_I2S_SCLK     (GPIO_NUM_12)
#define BSP_I2S_MCLK     (GPIO_NUM_13)
#define BSP_I2S_LCLK     (GPIO_NUM_10)
#define BSP_I2S_DOUT     (GPIO_NUM_9)
#define BSP_I2S_DSIN     (GPIO_NUM_11)
#define BSP_POWER_AMP_IO (GPIO_NUM_53)

#define BSP_LCD_BACKLIGHT (GPIO_NUM_26)
#define BSP_LCD_RST       (GPIO_NUM_27)
#define BSP_LCD_TOUCH_RST (GPIO_NUM_NC)
#define BSP_LCD_TOUCH_INT (GPIO_NUM_NC)

/* uSD card */
#define BSP_SD_D0  (GPIO_NUM_39)
#define BSP_SD_D1  (GPIO_NUM_40)
#define BSP_SD_D2  (GPIO_NUM_41)
#define BSP_SD_D3  (GPIO_NUM_42)
#define BSP_SD_CMD (GPIO_NUM_44)
#define BSP_SD_CLK (GPIO_NUM_43)

#ifdef __cplusplus
extern "C"
{
#endif

#define BSP_I2C_NUM 1
    esp_err_t bsp_i2c_init(void);
    esp_err_t bsp_i2c_deinit(void);
    i2c_master_bus_handle_t bsp_i2c_get_handle(void);

#ifdef __cplusplus
}
#endif
