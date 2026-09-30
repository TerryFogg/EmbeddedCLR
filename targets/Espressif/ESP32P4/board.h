#pragma once
//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include "driver/gpio.h"
#include "hal/uart_types.h"
#include "hal/i2c_types.h"

//- 4 ADC inputs
//- 1 dedicated user I²C bus
//- 1 dedicated user SPI bus
//- 6 PWM outputs
//- 2 interrupt-capable spare GPIOs
//- 1 status LED pin



#define GPIO_INTERRUPT gpio_isr_t

#define WIRE_PROTOCOL_UART ((uart_port_t)UART_NUM_0)

#define MAXIMUM_I2C_BUSES 2

// Status LED
#define PIN_STATUS_LED GPIO_NUM_52

#define INTERNAL_SHARED_I2C_MASTER_BUS i2c_port_t::I2C_NUM_1
#define INTERNAL_I2C_SDA               GPIO7
#define INTERNAL_I2C_SCL               GPIO8

#define TOUCH_HEIGHT                   800
#define TOUCH_WIDTH                    1280
#define TOUCH_RESET_PIN                GPIO23 // Shared with LCD reset on ESP32P4-WIFI6 (Waveshare)
#define TOUCH_INTERRUPT_PIN            GPIO33
#define LCD_TOUCH_GT911_ADDRESS        (0x5D)
#define LCD_TOUCH_GT911_ADDRESS_BACKUP (0x14)

#define LCD_RESET      (0x27)
#define LCD_BACK_LIGHT (0x26)

#ifdef __cplusplus
extern "C"
{
#endif

    void InitializeBoard();

#ifdef __cplusplus
}
#endif
