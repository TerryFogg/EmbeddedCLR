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

//  Allocate SPIRAM -> (32MB - 2048000) == (total - rotation buffer)
//  +------------------------------------------------------------------------------------+
//  | Region                      | Size      | Description                              |
//  +-----------------------------+-----------+------------------------------------------+
//  | nanoFramework_Deployment    |  16 MB    | Portable Executable (PE)                 |
//  | Rotation Buffer(1280*800*2) |  2048000  | Equal to Frame Buffer for Landscape Mode |
//  | ESP-IDF allocations         |   500000  | Wifi,Bluetooth etc system allocation     |
//  | Graphics Working Memory     |  ~12 MB   | Remaining Memory for graphics operations |
//  +-----------------------------+--------+---------------------------------------------+

//  SPIRAM reserved for later allocation when the panel is created
//  +----------------------------+-----------+-------------------------------------------+
//  | Frame Buffer(1280*800*2)   |  2048000  | JD9365  panel driver allocates this       |
//  |                            |           | memory for the frame buffer.              |
//  +----------------------------+-----------+-------------------------------------------+

#define TOTAL_SPIRAM                33554430
#define ESP_IDF                     500000
#define nanoFrameworkDeploymentsize 16777215
#define RotationBufferSize          FrameBufferSize
#define GraphicsMemoryReserve                                                                                          \
    (TOTAL_SPIRAM - ESP_IDF - nanoFrameworkDeploymentsize - FrameBufferSize - RotationBufferSize)

#define FrameBufferSize 2048000

uint16_t *graphicsRotationBuffer;

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

    graphicsRotationBuffer = (uint16_t *)heap_caps_malloc(RotationBufferSize, MALLOC_CAP_SPIRAM);

    return true;
}
