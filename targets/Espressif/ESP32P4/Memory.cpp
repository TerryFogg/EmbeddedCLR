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

//
// +----------------------------------+
//| ESP32-P4 SoC - memory layout
// +----------------------------------+
//

// +------+
// INTERNAL
// +------+
// ROM
// ├─ 128 KB HP ROM
//  ├  First-stage bootloader
//  ├  Chip startup
//  ├  Flash initialization
//  ├  ROM library functions
//  ├  Security functions
//  └─ Recovery paths
// ├─ 16 KB LP ROM
//  ├  Low-power startup
//  ├  Sleep
//  ├  Sleep/wake support
//  └─  LP processor runtime code
//
// RAM
// ├─ 768 KB HP L2MEM
//  ├  .data
//  ├  .bss
//  ├  Stacks
//  ├  FreeRTOS
//  ├  native IDF API's allocations
//  ├  DMA buffers
//  └─ Interrupt data
// ├─ 32 KB LP SRAM
//  ├  LP core code
//  ├  LP variables
//  ├  Sleep - mode operation
//  └─ Always - on functions
// └─ 8 KB SPM(Scratchpad Memory)
//  ├  DSP routines
//  ├  Critical interrupt code
//  └─ Real-time control loops

// +------+
// EXTERNAL
// +------+
// ├─ 32MB PSRAM
//  ├  CLR Managed Memory
//  ├  Graphics Memory
//  └─ Frame Buffers
// └─ 32MB Flash
//  ├  Managed Code
//  ├  Non volatile store (nvs)
//  └─ Internal Flash Disk (small)
// +----------------------------------+

// +----------------------------------+
// 32MB PSRAM Detailed allocation
// +----------------------------------+
//  ├  nanoFramework working data        : 16MB
//  ├  Frame Buffer allocated by JD9365  : 2048000  ( 1280 * 800 *2) pixels
//  ├  Frame Buffer (Rotation)           : 2048000  ( 1280 * 800 *2) pixels
//  └─ Graphics Working Memory           : (32MB - 16MB - 2048000 - 2048000)
//                                         Remaining Memory for graphics operations
//
// └─ 32MB Flash
//  ├  0x0000   : Bootloader 
//  ├  ...
//  ├  ...
//  ├  0x8000   : Partition Table
//  ├  0x9000   : NVS
//  ├  ...
//  ├  ...
//  ├  0xF000   : PHY Init
//  ├  0x10000  : factory app ( IDF /native nanoClr)
//  ├  ...
//  ├  ...
//  └─ Internal Flash Disk (small)
// +----------------------------------+

#define TOTAL_SPIRAM 33554430
// +------------------------------------------------+
#define ESP_IDF                     500000
#define nanoFrameworkManagedHeap    16777215
#define FrameBufferSize             2048000
#define RotationBufferSize          FrameBufferSize
#define GraphicsMemoryReserve                                                                                          \
    (TOTAL_SPIRAM - ESP_IDF - nanoFrameworkManagedHeap - FrameBufferSize - RotationBufferSize)

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
        managedHeapAddress = (unsigned char *)heap_caps_malloc(nanoFrameworkManagedHeap, MALLOC_CAP_SPIRAM);
        HalSystemConfig.RAM1.Size = nanoFrameworkManagedHeap;
        HalSystemConfig.RAM1.Base = (unsigned int)managedHeapAddress;
    }
    baseAddress = managedHeapAddress;
    sizeInBytes = nanoFrameworkManagedHeap;
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
