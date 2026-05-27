//
// Copyright (c) 2017 The nanoFramework project contributors
// See LICENSE file in the project root for full license information.
//

#include "TouchInterface.h"

TouchInterface g_TouchInterface;


bool TouchInterface::Initialize(int TouchI2cBus, int TouchI2cSlaveAddress)
{
    return true;
}
CLR_UINT8 *TouchInterface::Write_Read(CLR_UINT8 *writeBuffer, CLR_UINT16 writeSize, CLR_UINT16 readSize)
{
    return (CLR_UINT8 *)1;
    
}
