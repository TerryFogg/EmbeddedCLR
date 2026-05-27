//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include "DisplayInterface.h"
#include "sys_dev_spi_native.h"
#include <nanoPAL.h>
#include <target_platform.h>

struct DisplayInterface g_DisplayInterface;
DisplayInterfaceConfig g_DisplayInterfaceConfig;

// Display Interface
void DisplayInterface::Initialize(DisplayInterfaceConfig &config)
{
(void)config;
return;
}
void DisplayInterface::GetTransferBuffer(CLR_UINT8 *&TransferBuffer, CLR_UINT32 &TransferBufferSize)
{
}
void DisplayInterface::ClearFrameBuffer()
{
// Set screen to black
}
void DisplayInterface::WriteToFrameBuffer(
CLR_UINT8 command,
CLR_UINT8 data[],
CLR_UINT32 dataCount,
CLR_UINT32 frameOffset)
{
(void)frameOffset;
return;
}
void DisplayInterface::SendCommand(CLR_UINT8 arg_count, ...)
{
}
void DisplayInterface::DisplayBacklight(bool on) // true = on
{
}
void SendCommandBytes(CLR_UINT8 *data, CLR_UINT32 length)
{
}
void SendDataBytes(CLR_UINT8 *data, CLR_UINT32 length)
{
}

