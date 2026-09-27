//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include "TouchDevice.h"
#include "TouchInterface.h"
#include "TouchPanel.h"

#include "CoreIO.h"
#include "WireProtocol_HAL_Interface.h"
#include "board.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/spi_master.h"
#include "driver/uart.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_err.h"
#include "hal/uart_types.h"
#include "nanoHAL_v2.h"
#include "soc/uart_channel.h"

#include "Display.h"
#include "DisplayInterface.h"
#include "GraphicsMemoryHeap.h"
#include "Graphics.h"

extern GraphicsMemoryHeap g_GraphicsMemoryHeap;

extern DisplayInterface g_DisplayInterface;
extern DisplayDriver g_DisplayDriver;

extern TouchPanel g_TouchPanel;
extern TouchInterface g_TouchInterface;
extern TouchDevice g_TouchDevice;

void InitializeWireProtocol();
void InitializeADC();
void InitializeGpio();
void InitializeI2C();
void InitializePWM();
void InitializeSpi();
void InitializeGraphics();
void InitializeLcdTouchPanel();

void InitializeBoard()
{
    InitializeGpio();
    InitializeADC();
    InitializeI2C();
    InitializePWM();
    InitializeSpi();
    InitializeWireProtocol();

    InitializeGraphics();
    InitializeLcdTouchPanel();
}
void InitializeGpio()
{
    GpioIO::InitializePin(GPIO28, GpioPinMode::MODE_INPUT, GpioBias::NOBIAS);
    GpioIO::InitializePin(GPIO29, GpioPinMode::MODE_INPUT, GpioBias::NOBIAS);
    GpioIO::InitializePin(GPIO37, GpioPinMode::MODE_INPUT, GpioBias::NOBIAS);
    GpioIO::InitializePin(GPIO38, GpioPinMode::MODE_INPUT, GpioBias::NOBIAS);
}
void InitializeADC()
{
    // ESP32-P4 has 2 ADC units and multiple analog-capable pins, but the GPIO-to-channel mapping comes from the chip
    AdcIO::Initialize();
    AdcIO::AddChannel(ADC_CHANNEL_0); // GPIO2
    AdcIO::AddChannel(ADC_CHANNEL_1); // GPIO3
    AdcIO::AddChannel(ADC_CHANNEL_2); // GPIO4
    AdcIO::AddChannel(ADC_CHANNEL_3); // GPIO5
}
void InitializeI2C()
{
    // Internal Touch / Codec / ADC_PA
    I2cIO::Initialize(INTERNAL_SHARED_I2C_MASTER_BUS, GPIO7, GPIO8);

    // Accessible on external jumper J*, back panel 40 pin connector
    I2cIO::Initialize(i2c_port_t::I2C_NUM_0, GPIO21, GPIO22);
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
    SpiIO::Initialize(spi_host_device_t::SPI1_HOST, GPIO30, GPIO31, GPIO32);
    SpiIO::AttachDevice(spi_host_device_t::SPI1_HOST, GPIO32);
}
void InitializeWireProtocol()
{
    SerialIO::Initialize(
        UART_NUM_0,
        GPIO37,
        GPIO38,
        921600,
        UART_DATA_8_BITS,
        UART_PARITY_DISABLE,
        UART_STOP_BITS_1,
        UART_HW_FLOWCTRL_DISABLE);
}

void InitializeGraphics()
{
    g_GraphicsMemoryHeap.Initialize(6000000);
    g_DisplayInterface.Initialize();
    g_DisplayDriver.Initialize();
}

void InitializeLcdTouchPanel()
{
    g_TouchInterface.Initialize(INTERNAL_SHARED_I2C_MASTER_BUS, 0x5D);
    g_TouchDevice.Initialize();
    g_TouchPanel.Initialize();
}
