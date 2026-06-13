//
// Copyright (c) .NET Foundation and Contributors
// Portions Copyright (c) Microsoft Corporation.  All rights reserved.
// See LICENSE file in the project root for full license information.
//
#include <esp32_idf.h>
#include <nanoHAL.h>
#include <nanoPAL.h>
#include "nanoCLR_Types.h"
#include "GraphicsMemoryHeap.h"

//  Allocate SPIRAM
//  +-------------------------------------------------------------------+
//  | Region                     | Size   | Start Address | End Address |
//  +----------------------------+--------+---------------+-------------+
//  | nanoFramework_Deployment   |  16 MB | 0x48000000    | 0x48FFFFFF  |
//  | Graphics Working Memory    | ~12 MB | 0x49000000    | 0x49BFFFFF  |
//  | Frame Buffer 1(1280*800*2) | ~2 MB  | 0x49C00000    | 0x49DFFFFF  |
//  | Frame Buffer 2(1280*800*2) | ~2 MB  | 0x49E00000    | 0x49FFFFFF  |
//  +----------------------------+--------+---------------+-------------+
#define nanoFrameworkDeploymentsize 12048000
#define FrameBufferSize1            2048000
#define FrameBufferSize2            2048000
#define GraphicsMemoryReserve       (nanoFrameworkDeploymentsize - FrameBufferSize1 - FrameBufferSize2)

unsigned char *managedHeapAddress = NULL;
size_t managedHeapSize = 0;

void HeapLocation(unsigned char *&baseAddress, unsigned int &sizeInBytes)
{
    // This is called by targetHAL.cpp and CLRStartup( reset memory)
    // Not sure why CLRStartup needs to reset memory but it does so we need to make sure we return the same memory
    // address and size for all calls
    if (managedHeapAddress == NULL)
    {
        managedHeapAddress = (unsigned char *)heap_caps_malloc(nanoFrameworkDeploymentsize, MALLOC_CAP_SPIRAM);
        HalSystemConfig.RAM1.Size = nanoFrameworkDeploymentsize;
        HalSystemConfig.RAM1.Base = (unsigned int)managedHeapAddress;
    }
    baseAddress = managedHeapAddress;
    sizeInBytes = nanoFrameworkDeploymentsize;
}

bool GraphicsMemory::GraphicsHeapLocation(
    CLR_UINT32 requested,
    CLR_UINT8 *&graphicsStartingAddress,
    CLR_UINT8 *&graphicsEndingAddress)
{
    (void)requested;
    graphicsStartingAddress = (unsigned char *)heap_caps_malloc(GraphicsMemoryReserve, MALLOC_CAP_SPIRAM);
    graphicsEndingAddress = graphicsStartingAddress + GraphicsMemoryReserve;
    return true;
}
