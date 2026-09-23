//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include <esp32_idf.h>
#include <nanoPAL.h>
#include <nanoHAL_Time.h>
#include <nanoHAL_Types.h>
#include <target_platform.h>
#include <nanoPAL_Events.h>
#include <nanoPAL_BlockStorage.h>
#include <nanoHAL_ConfigurationManager.h>
#include <nanoHAL_StorageOperation.h>
#include <nanoHAL_Graphics.h>

#if (CONFIG_SUPPORT_GRAPHICSTOUCH == TRUE)
#include "TouchPanel.h"
#include "TouchInterface.h"
#include "TouchDevice.h"
extern TouchPanel g_TouchPanel;
extern TouchInterface g_TouchInterface;
extern TouchDevice g_TouchDevice;
#endif

void Storage_Initialize();
void Storage_Uninitialize();

extern "C" void FixUpHalSystemConfig();
extern "C" void FixUpBlockRegionInfo();

// Test code for BLE, will be removed once we have managed code interface
extern void blehr_start();
extern void ibeacon_start();


static bool rebootinprogress = false;

//
//  Reboot handlers clean up on reboot
//
static ON_SOFT_REBOOT_HANDLER s_rebootHandlers[16] =
    {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

void HAL_AddSoftRebootHandler(ON_SOFT_REBOOT_HANDLER handler)
{
    for (unsigned int i = 0; i < ARRAYSIZE(s_rebootHandlers); i++)
    {
        if (s_rebootHandlers[i] == NULL)
        {
            s_rebootHandlers[i] = handler;
            return;
        }
        else if (s_rebootHandlers[i] == handler)
        {
            return;
        }
    }
}

// because nanoHAL_Initialize/Uninitialize needs to be called in both C and C++ we need a proxy to allow it to be called
// in 'C'
extern "C"
{

    void nanoHAL_Initialize_C()
    {
        nanoHAL_Initialize();
    }

    void nanoHAL_Uninitialize_C(bool isPoweringDown)
    {
        nanoHAL_Uninitialize(isPoweringDown);
    }
}

void nanoHAL_Initialize()
{
    HAL_CONTINUATION::InitializeList();
    HAL_COMPLETION ::InitializeList();

    // Fixup System & Block storage parameters based on Flash chip and partition layout
    FixUpHalSystemConfig();
    FixUpBlockRegionInfo();

    BlockStorageList_Initialize();
    BlockStorage_AddDevices();
    BlockStorageList_InitializeDevices();

    FS_Initialize();
    FileSystemVolumeList::Initialize();
    FS_AddVolumes();
    FileSystemVolumeList::InitializeVolumes();

    // allocate & clear managed heap region
    unsigned char *heapStart = NULL;
    unsigned int heapSize = 0;

    ::HeapLocation(heapStart, heapSize);
    memset(heapStart, 0, heapSize);
    Events_Initialize();
    PalEvent_Initialize();
    Network_Initialize();

#if (CONFIG_SUPPORT_GRAPHICS == TRUE)
    if (!rebootinprogress)
    {
        DisplayInterfaceConfig displayConfig;
        g_GraphicsMemoryHeap.Initialize(6000000);
        g_DisplayInterface.Initialize(displayConfig);
        g_DisplayDriver.Initialize();
        rebootinprogress = true;
    }
#endif

#if (CONFIG_SUPPORT_GRAPHICSTOUCH == TRUE)
 //   g_TouchInterface.Initialize();
 //   g_TouchDevice.Initialize();
 //   g_TouchPanel.Initialize();
#endif
}

void nanoHAL_Uninitialize(bool isPoweringDown)
{
    (void)isPoweringDown;

    // check for s_rebootHandlers
    for (unsigned int i = 0; i < ARRAYSIZE(s_rebootHandlers); i++)
    {
        if (s_rebootHandlers[i] != NULL)
        {
            s_rebootHandlers[i]();
        }
        else
        {
            break;
        }
    }

    Network_Uninitialize();
    FileSystemVolumeList::UninitializeVolumes();
    // required to remove flash partitions memory mapping
    BlockStorageList_UnInitializeDevices();

//    CPU_GPIO_Uninitialize();

    // PalEvent_Uninitialize();

    Events_Uninitialize();

    HAL_CONTINUATION::Uninitialize();
    HAL_COMPLETION ::Uninitialize();
}

