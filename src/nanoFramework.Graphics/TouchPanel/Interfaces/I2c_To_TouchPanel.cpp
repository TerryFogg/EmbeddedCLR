//
// Copyright (c) 2017 The nanoFramework project contributors
// See LICENSE file in the project root for full license information.
//

#include "TouchInterface.h"
#include "CoreIO.h"

TouchInterface g_TouchInterface;

static int m_TouchI2cSlaveAddress;
static int m_TouchI2cBus;
static uint8_t I2C_READ_BUFFER[32];

bool TouchInterface::Initialize(int i2c_bus_number, int slaveAddress)
{
    m_TouchI2cBus = i2c_bus_number;
    m_TouchI2cSlaveAddress = slaveAddress;
    return true;
}
CLR_UINT8 *TouchInterface::Write_Read(uint8_t *writeBuffer, uint16_t writeSize, CLR_UINT16 readSize)
{
    I2cIO::WriteRead( m_TouchI2cSlaveAddress, writeBuffer, writeSize, I2C_READ_BUFFER, readSize);
    return I2C_READ_BUFFER;
}
