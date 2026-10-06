//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include "TouchDevice.h"
#include "TouchInterface.h"
#include "TouchPanel.h"

#include "NativeIO.h"
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

#include "ConstantExtractor.h"

extern GraphicsMemoryHeap g_GraphicsMemoryHeap;
extern DisplayInterface g_DisplayInterface;
extern DisplayDriver g_DisplayDriver;
extern TouchPanel g_TouchPanel;
extern TouchInterface g_TouchInterface;
extern TouchDevice g_TouchDevice;

uint32_t GetConfiguredIPAddress();

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

  //  GetConfiguredIPAddress();
    return;
}
void InitializeGpio()
{
    GpioIO::Initialize();
    GpioIO::InitializePin(GPIO28, GpioPinMode_INPUT, GpioBias_NoBias);
    GpioIO::InitializePin(GPIO29, GpioPinMode_INPUT, GpioBias_NoBias);
    GpioIO::InitializePin(GPIO37, GpioPinMode_INPUT, GpioBias_NoBias);
    GpioIO::InitializePin(GPIO38, GpioPinMode_INPUT, GpioBias_NoBias);
    return;
}
void InitializeADC()
{
    // ESP32-P4 has 2 ADC units and multiple analog-capable pins, but the GPIO-to-channel mapping comes from the chip
    AdcIO::Initialize();
    AdcIO::AddChannel(ADC_CHANNEL_0); // GPIO2
    AdcIO::AddChannel(ADC_CHANNEL_1); // GPIO3
    AdcIO::AddChannel(ADC_CHANNEL_2); // GPIO4
    AdcIO::AddChannel(ADC_CHANNEL_3); // GPIO5
    return;
}
void InitializeI2C()
{
    // Internal Touch / Codec / ADC_PA
    I2cIO::Initialize(INTERNAL_SHARED_I2C_MASTER_BUS, GPIO7, GPIO8);

    // Accessible on external jumper J8, back panel 40 pin connector
    I2cIO::Initialize(i2c_port_t::I2C_NUM_0, GPIO21, GPIO22);
    return;
}
void InitializePWM()
{
    PwmIO::Initialize(1000);
    return;
}
void InitializeSpi()
{
    SpiIO::Initialize(spi_host_device_t::SPI1_HOST, GPIO30, GPIO31, GPIO32);
    SpiIO::AttachDevice(spi_host_device_t::SPI1_HOST, GPIO32);
    return;
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
    return;
}

void InitializeGraphics()
{
    g_GraphicsMemoryHeap.Initialize(6000000);
    g_DisplayInterface.Initialize();
    g_DisplayDriver.Initialize();
    return;
}
void InitializeLcdTouchPanel()
{
    g_TouchInterface.Initialize(INTERNAL_SHARED_I2C_MASTER_BUS, 0x5D);
    g_TouchDevice.Initialize();
    //    g_TouchPanel.Initialize();
    return;
}


#include "ConstantExtractor.h"
#include <cstdint>
#include <cstdio>

uint32_t GetConfiguredIPAddress()
{
    // Set these to your real mapped flash region
    const void *flashBase = (const void *)0x1B0000;
    size_t flashSize = 0x1A0000;

    CE_InitFlashImage(flashBase, flashSize);

    const char *cls = "network";
    const char *ns = "nanoFramework.Configuration";
    const char *field = "IPAddress";

    // Find PE that contains the type (uses internal cache after first find)
    const void *peBase = CE_FindPEByType(cls, ns);
    if (!peBase)
    {
        // not found
        return 0;
    }

    uint64_t value = 0;
    if (!CE_ExtractPublicConstUnsignedAt(peBase, cls, ns, field, &value))
    {
        // field missing or wrong type
        return 0;
    }

    // constant is 0xC0A80004u (fits in 32 bits)
    uint32_t ip = (uint32_t)value;
    // optional: print dotted decimal
    uint8_t a = (ip >> 24) & 0xFF;
    uint8_t b = (ip >> 16) & 0xFF;
    uint8_t c = (ip >> 8) & 0xFF;
    uint8_t d = ip & 0xFF;
    printf("Found IP: 0x%08X (%u.%u.%u.%u)\n", ip, a, b, c, d);

    return ip;
}
