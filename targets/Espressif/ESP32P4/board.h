#pragma once
//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include "hal/uart_types.h"
#include "driver/gpio.h"

//- 4 ADC inputs
//- 1 dedicated user I²C bus
//- 1 dedicated user SPI bus
//- 6 PWM outputs
//- 2 interrupt-capable spare GPIOs
//- 1 status LED pin

#define WIRE_PROTOCOL_UART ((uart_port_t)UART_NUM_0)






// Status LED
#define PIN_STATUS_LED GPIO_NUM_52

#define ADC0_PIN GPIO_NUM_2
#define ADC1_PIN GPIO_NUM_3
#define ADC2_PIN GPIO_NUM_4
#define ADC3_PIN GPIO_NUM_5








#ifdef __cplusplus
extern "C"
{
#endif

    void InitializeBoard();

#ifdef __cplusplus
}
#endif
