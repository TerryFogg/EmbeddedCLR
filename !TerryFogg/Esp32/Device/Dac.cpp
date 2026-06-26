//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include "Device.IO.h"


#pragma region System.Device.Dac
int DacIO::Resolution()
{
    // No DAC
    return 0;
}
int DacIO::ChannelCount()
{
    // No DAC
    return 0;
}
bool DacIO::Initialize(CLR_INT32 dac_channel_number)
{
    // No DAC
    (void)dac_channel_number;
    return false;
}
bool DacIO::Open(CLR_INT32 dac_channel_number)
{
    // No DAC
    (void)dac_channel_number;
    return false;
}
char *DacIO::DeviceSelector(int dac_channel_number)
{
    // No DAC
    (void)dac_channel_number;
    static char DeviceNameDummy[] = "NotSupported\0";
    return DeviceNameDummy;
}
void DacIO::Write(CLR_INT32 dac_channel_number, CLR_INT32 value)
{
    // No DAC
    (void)dac_channel_number;
    (void)value;
}
PinNameValue DacIO::ChannelToPin(CLR_INT32 dac_channel_number)
{
    // No DAC
    (void)dac_channel_number;
    return (PinNameValue)-1;
}
bool DacIO::Dispose(CLR_INT32 dac_channel_number)
{
    // No DAC
    (void)dac_channel_number;
    return true;
}
#pragma endregion
