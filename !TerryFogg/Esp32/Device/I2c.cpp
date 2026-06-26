//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include "Device.IO.h"


#pragma region System.Device.I2c
// Reserve 256 byte memory for the I2C slave.
// To write a series of bytes, the master first writes the memory address, followed by the data.
// The address is automatically incremented for each byte transferred, looping back to 0 upon reaching the end.
// Reading is done sequentially from the current memory address.
static struct
{
    uint8_t mem[256];
    uint8_t mem_address;
    bool mem_address_written;
} context;

static I2C_Properties *mcuI2C;
static int gNumberI2CDevices;
void I2cIO::SetupI2CList(I2C_Properties *boardMcuI2C, int NumberI2CDevices)
{
    mcuI2C = boardMcuI2C;
    gNumberI2CDevices = NumberI2CDevices;
}
uint32_t I2cIO::GetByteTime(uint32_t I2C_deviceId)
{
    return mcuI2C[I2C_deviceId].ByteTime;
}
// The handler is called from the I2C ISR, so it must complete quickly.
static void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event)
{
    switch (event)
    {
        case I2C_SLAVE_RECEIVE:
            if (!context.mem_address_written)
            {
                context.mem_address = i2c_read_byte_raw(i2c);
                context.mem_address_written = true;
            }
            else
            {
                context.mem[context.mem_address] = i2c_read_byte_raw(i2c);
                context.mem_address++;
            }
            break;
        case I2C_SLAVE_REQUEST:
            i2c_write_byte_raw(i2c, context.mem[context.mem_address]);
            context.mem_address++;
            break;
        case I2C_SLAVE_FINISH:
            context.mem_address_written = false;
            break;
        default:
            break;
    }
}

bool I2cIO::Initialize(
    CLR_INT32 I2C_deviceId,
    I2cBusSpeed I2C_speed,
    I2C_CONTROL_TYPE I2C_control_type,
    CLR_INT32 deviceAddress)
{
    if (!mcuI2C[I2C_deviceId].I2C_Initialized)
    {
        CLR_INT32 baud;
        switch (I2C_speed)
        {
            case I2cBusSpeed::I2cBusSpeed_StandardMode:
                baud = 100000;
                mcuI2C[I2C_deviceId].ByteTime = 0.1;
            case I2cBusSpeed::I2cBusSpeed_FastMode:
                baud = 400000;
                mcuI2C[I2C_deviceId].ByteTime = 0.02;
                break;
            case I2cBusSpeed::I2cBusSpeed_FastModePlus:
                baud = 1000000;
                mcuI2C[I2C_deviceId].ByteTime = 0.009;
                break;
        }

        gpio_set_function(mcuI2C[I2C_deviceId].sda, GPIO_FUNC_I2C);
        gpio_pull_up(mcuI2C[I2C_deviceId].sda);
        gpio_set_function(mcuI2C[I2C_deviceId].scl, GPIO_FUNC_I2C);
        gpio_pull_up(mcuI2C[I2C_deviceId].scl);

        switch (I2C_control_type)
        {
            case I2C_CONTROL_TYPE::MASTER:
                i2c_master_bus_config_t i2c_bus_conf = {
                    .clk_source = I2C_CLK_SRC_DEFAULT,
                    .sda_io_num = BSP_I2C_SDA,
                    .scl_io_num = BSP_I2C_SCL,
                    .i2c_port = BSP_I2C_NUM,
                };
                esp_err_t result = i2c_new_master_bus(&i2c_bus_conf, &i2c_handle);
                break;
            case I2C_CONTROL_TYPE::SLAVE:
                i2c_slave_init((i2c_inst *)(mcuI2C[I2C_deviceId].i2c_instance), deviceAddress, &i2c_slave_handler);
                break;
        }
        mcuI2C[I2C_deviceId].I2C_Initialized = true;
    }
    return true;
}
bool I2cIO::Dispose(CLR_INT32 I2C_deviceId)
{
    if (!mcuI2C[I2C_deviceId].I2C_Initialized)
    {
        i2c_deinit((i2c_inst *)(mcuI2C[I2C_deviceId].i2c_instance));
    }
    return true;
}
I2cTransferStatus I2cIO::Write(
    CLR_INT32 I2C_deviceId,
    CLR_INT32 slaveAddress,
    CLR_UINT8 *writeBuffer,
    CLR_INT32 writeSize,
    I2C_CONTROL_TYPE busType)
{
    I2cTransferStatus return_status = I2cTransferStatus_UnknownError;
    bool is_master = (busType == I2C_CONTROL_TYPE::MASTER) ? true : false;

    int returnValue = i2c_write_blocking(
        (i2c_inst *)(mcuI2C[I2C_deviceId].i2c_instance),
        slaveAddress,
        writeBuffer,
        writeSize,
        is_master); // true to keep master control of bus

    if (returnValue == PICO_ERROR_TIMEOUT)
    {
        return_status = I2cTransferStatus_ClockStretchTimeout;
    }
    else if (returnValue == PICO_ERROR_GENERIC)
    {
        return_status = I2cTransferStatus_UnknownError;
    }
    else
    {
        if (returnValue != writeSize)
        {
            return_status = I2cTransferStatus_PartialTransfer;
        }
        else if (returnValue = writeSize)
        {
            return_status = I2cTransferStatus_FullTransfer;
        }
    }
    return return_status;
}
I2cTransferStatus I2cIO::Read(CLR_INT32 I2C_deviceId, CLR_INT32 slaveAddress, CLR_UINT8 *readBuffer, CLR_INT32 readSize)
{
    I2cTransferStatus return_status = I2cTransferStatus_UnknownError;
    int numberBytesRead =
        i2c_read_blocking((i2c_inst *)(mcuI2C[I2C_deviceId].i2c_instance), slaveAddress, readBuffer, readSize, false);

    if (numberBytesRead == PICO_ERROR_TIMEOUT)
    {
        return_status = I2cTransferStatus_ClockStretchTimeout;
    }
    else if (numberBytesRead == PICO_ERROR_GENERIC)
    {
        return_status = I2cTransferStatus_UnknownError;
    }
    else
    {
        if (numberBytesRead != readSize)
        {
            return_status = I2cTransferStatus_PartialTransfer;
        }
        else if (numberBytesRead = readSize)
        {
            return_status = I2cTransferStatus_FullTransfer;
        }
    }
    return return_status;
}

//void I2cIO::Execute(I2c_Transaction *pI2CTransaction)
//{
//    pI2CTransaction->status = I2cTransferStatus::I2cTransferStatus_FullTransfer;
//
//    if (pI2CTransaction->IsWrite)
//    {
//        pI2CTransaction->status = Write(
//            pI2CTransaction->busId,
//            pI2CTransaction->slaveAddress,
//            pI2CTransaction->writeBuffer,
//            pI2CTransaction->writeSize,
//            I2C_CONTROL_TYPE::MASTER);
//    }
//    if (pI2CTransaction->IsRead && (pI2CTransaction->status == I2cTransferStatus::I2cTransferStatus_FullTransfer))
//    {
//        pI2CTransaction->status = Read(
//            pI2CTransaction->busId,
//            pI2CTransaction->slaveAddress,
//            pI2CTransaction->readBuffer,
//            pI2CTransaction->readSize);
//    }
//}

#pragma endregion
