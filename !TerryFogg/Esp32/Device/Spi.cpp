//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include "Device.IO.h"


static SPI_Properties *mcuSPI;
void SpiIO::SetupSpiList(SPI_Properties *boardMcuSPI)
{
    mcuSPI = boardMcuSPI;
}
CLR_INT32 SpiIO::MaximumClockFrequencyHz(CLR_INT32 controllerID)
{
    // Todo for ESP32-P4
    return 999999999999;
}
CLR_INT32 SpiIO::MinimumClockFrequencyHz(CLR_INT32 controllerID)
{
    // Todo for ESP32-P4
    return 100;
}
bool SpiIO::Initialize()
{
    // Nothing to do,leave initialization until "Open"
    return true;
}
bool SpiIO::Dispose(CLR_INT32 spi_deviceID)
{
    spi_deinit((spi_inst_t *)mcuSPI[spi_deviceID].SPI_instance);
    return true;
}
bool SpiIO::WriteRead(
    CLR_INT32 deviceId,
    SPI_WRITE_READ_SETTINGS rws,
    CLR_UINT8 *writeData,
    CLR_UINT16 writeSize,
    CLR_UINT8 *readData,
    CLR_UINT16 readSize)
{
    return false;
}
bool SpiIO::Open(SPI_DEVICE_CONFIGURATION spiConfig, CLR_UINT32 handle)
{
    if (spiConfig.Clock_RateHz >= SpiIO::MinimumClockFrequencyHz(spiConfig.BusConfiguration) &&
        spiConfig.Clock_RateHz <= SpiIO::MaximumClockFrequencyHz(spiConfig.BusConfiguration))
    {
        spi_init((spi_inst_t *)mcuSPI[spiConfig.BusConfiguration].SPI_instance, spiConfig.Clock_RateHz);
        gpio_set_function(mcuSPI[spiConfig.BusConfiguration].sck, GPIO_FUNC_SPI);
        gpio_set_function(mcuSPI[spiConfig.BusConfiguration].tx, GPIO_FUNC_SPI);
        gpio_set_function(mcuSPI[spiConfig.BusConfiguration].rx, GPIO_FUNC_SPI);
    }
    else
    {
        return false;
    }
    return true;
}
CLR_INT32 SpiIO::ByteTime()
{
    return 1;
}
SPI_OP_STATUS SpiIO::Completed(CLR_INT32 deviceId)
{
    (void)deviceId;
    return SPI_OP_STATUS::SPI_OP_COMPLETE;
}
