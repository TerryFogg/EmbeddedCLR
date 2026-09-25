//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include "CoreIO.h"
#include "WireProtocol_HAL_Interface.h"
#include "board.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/spi_master.h"
#include "driver/uart.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_err.h"
#include "hal/i2c_types.h"
#include "hal/uart_types.h"
#include "nanoHAL_v2.h"
#include "soc/uart_channel.h"

void InitializeWireProtocol();

// Initialize Core IO
void InitializeADC();
void InitializeGpio();
void InitializeI2C();
void InitializePWM();
void InitializeSpi();

void InitializeBoard()
{
    InitializeGpio();
    InitializeADC();
    InitializeI2C();
    InitializePWM();
    InitializeSpi();
    InitializeWireProtocol();
}
void InitializeADC()
{
    // The ESP32-P4 ADC GPIO mapping is fixed in hardware and is different from earlier ESP32 chips.
    // ESP32-P4 has 2 ADC units and multiple analog-capable pins, but the GPIO-to-channel mapping comes from the chip
    // The following uses Unit 1 and the first 4 channels
    // ADC_UNIT_1 ==>  ADC_CHANNEL_0 GPIO2 / ADC_CHANNEL_1 GPIO3 / ADC_CHANNEL_2 GPIO4 / ADC_CHANNEL_3 GPIO5

    AdcIO::Initialize();
    AdcIO::AddChannel(ADC_CHANNEL_0);
    AdcIO::AddChannel(ADC_CHANNEL_1);
    AdcIO::AddChannel(ADC_CHANNEL_2);
    AdcIO::AddChannel(ADC_CHANNEL_3);
}

void InitializeGpio()
{
    // Set the initial state of the GPIO's as input
    GpioIO::InitializePin(GPIO28, PinMode::MODE_INPUT, GpioBias::NOBIAS);
    GpioIO::InitializePin(GPIO29, PinMode::MODE_INPUT, GpioBias::NOBIAS);
    GpioIO::InitializePin(GPIO37, PinMode::MODE_INPUT, GpioBias::NOBIAS);
    GpioIO::InitializePin(GPIO38, PinMode::MODE_INPUT, GpioBias::NOBIAS);
}
void InitializeI2C()
{
    // Internal Touch / Codec / ADC_PA
    I2cIO::Initialize(i2c_port_t::I2C_NUM_0, GPIO7, GPIO8);
    I2cIO::Initialize(i2c_port_t::I2C_NUM_1, GPIO21, GPIO22);
}
void InitializePWM()
{
    PwmIO::Initialize(1000);
    PwmIO::AttachGpio(GPIO46, ledc_timer_t::LEDC_TIMER_0, 1000);
    PwmIO::AttachGpio(GPIO47, ledc_timer_t::LEDC_TIMER_0, 1000);
    PwmIO::AttachGpio(GPIO48, ledc_timer_t::LEDC_TIMER_0, 1000);
    PwmIO::AttachGpio(GPIO49, ledc_timer_t::LEDC_TIMER_0, 1000);
    PwmIO::AttachGpio(GPIO50, ledc_timer_t::LEDC_TIMER_0, 1000);
    PwmIO::AttachGpio(GPIO51, ledc_timer_t::LEDC_TIMER_0, 1000);
}
void InitializeSpi()
{
    SpiIO::Initialize(spi_host_device_t::SPI1_HOST, GPIO30, GPIO31,GPIO32);
    SpiIO::AttachDevice(spi_host_device_t::SPI1_HOST, GPIO32);
}

// ---------------------------------------------
// Setup Serial port for wire protocol debugging
// ---------------------------------------------
void InitializeWireProtocol()
{
    SerialIOPort WireProtocolSetup = {
        .usartDeviceNumber = uart_port_t::UART_NUM_0,
        .pinTX = PinNameValue::GPIO37,
        .pinRX = PinNameValue::GPIO38,
        .baudrate = 921600,
        .dataBits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stopBits = UART_STOP_BITS_1,
        .flowControl = UART_HW_FLOWCTRL_DISABLE};
    SerialIO::Initialize(WireProtocolSetup);
}
