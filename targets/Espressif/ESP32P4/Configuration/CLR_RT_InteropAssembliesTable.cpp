//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include <Core.h>

extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_mscorlib;
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_Runtime_Native;

#if CONFIG_SUPPORT_RUNTIMEEVENTS
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_Runtime_Events;
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_Runtime_Events_EventSink_DriverProcs;
#endif

#if CONFIG_SUPPORT_RESOURCEMANAGER
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_ResourceManager;
#endif
#if CONFIG_SUPPORT_COLLECTIONS
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_System_Collections;
#endif
#if CONFIG_SUPPORT_TEXT
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_System_Text;
#endif
#if CONFIG_SUPPORT_MATHEMATICS
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_System_Math;
#endif
#if CONFIG_SUPPORT_RUNTIMESERIALIZATION
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_System_Runtime_Serialization;
#endif
#if CONFIG_SUPPORT_GPIO
#endif
#if CONFIG_SUPPORT_SERIALPORTS
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_System_IO_Ports;
#endif
#if CONFIG_SUPPORT_CAN
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_Device_Can;
#endif
#if CONFIG_SUPPORT_ONEWIRE
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_Device_OneWire;
#endif
#if CONFIG_SUPPORT_HASHING
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_System_IO_Hashing;
#endif
#if CONFIG_SUPPORT_CRYPTOGRAPHY
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_System_Security_Cryptography;
#endif
#if CONFIG_SUPPORT_FILESYSTEM
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_System_IO_FileSystem;
#endif
#if CONFIG_SUPPORT_GRAPHICS
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_Graphics;
#endif
#if (CONFIG_SUPPORT_NETWORK)
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_System_Net;
#endif
#if (CONFIG_SUPPORT_WIFI)
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_System_Device_Wifi;
#endif
//  extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_Networking_Sntp;
#if CONFIG_SUPPORT_USBSTREAM
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_System_Device_UsbStream;
#endif
#if CONFIG_SUPPORT_BLUETOOTH
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_Device_Bluetooth;
#endif

const CLR_RT_NativeAssemblyData *g_CLR_InteropAssembliesNativeData[] = {
    &g_CLR_AssemblyNative_mscorlib,
    &g_CLR_AssemblyNative_nanoFramework_Runtime_Native,
#if CONFIG_SUPPORT_RUNTIMEEVENTS
    &g_CLR_AssemblyNative_nanoFramework_Runtime_Events,
    &g_CLR_AssemblyNative_nanoFramework_Runtime_Events_EventSink_DriverProcs,
#endif
#if CONFIG_SUPPORT_MATHEMATICS
    &g_CLR_AssemblyNative_System_Math,
#endif
#if CONFIG_SUPPORT_RUNTIMESERIALIZATION
    &g_CLR_AssemblyNative_System_Runtime_Serialization,
#endif
#if CONFIG_SUPPORT_RESOURCEMANAGER
    &g_CLR_AssemblyNative_nanoFramework_ResourceManager,
#endif
#if CONFIG_SUPPORT_COLLECTIONS
    &g_CLR_AssemblyNative_nanoFramework_System_Collections,
#endif
#if CONFIG_SUPPORT_TEXT
    &g_CLR_AssemblyNative_nanoFramework_System_Text,
#endif

#if CONFIG_SUPPORT_GPIO
#endif
#if CONFIG_SUPPORT_SERIALPORTS
    &g_CLR_AssemblyNative_System_IO_Ports,
#endif
#if CONFIG_SUPPORT_CAN
    &g_CLR_AssemblyNative_nanoFramework_Device_Can,
#endif
#if CONFIG_SUPPORT_ONEWIRE
    &g_CLR_AssemblyNative_nanoFramework_Device_OneWire,
#endif
#if CONFIG_SUPPORT_HASHING
    &g_CLR_AssemblyNative_nanoFramework_System_IO_Hashing,
#endif
#if CONFIG_SUPPORT_CRYPTOGRAPHY
        &g_CLR_AssemblyNative_nanoFramework_System_Security_Cryptography,
#endif
#if CONFIG_SUPPORT_FILESYSTEM
    &g_CLR_AssemblyNative_System_IO_FileSystem,
#endif
#if CONFIG_SUPPORT_GRAPHICS
    &g_CLR_AssemblyNative_nanoFramework_Graphics,
#endif
#if CONFIG_SUPPORT_NETWORK
    &g_CLR_AssemblyNative_System_Net,
#endif
#if CONFIG_SUPPORT_WIFI
    &g_CLR_AssemblyNative_System_Device_Wifi,
#endif
  //  &g_CLR_AssemblyNative_nanoFramework_Networking_Sntp,
#if CONFIG_SUPPORT_USBSTREAM
    &g_CLR_AssemblyNative_System_Device_UsbStream,
#endif
#if CONFIG_SUPPORT_BLUETOOTH
    &g_CLR_AssemblyNative_nanoFramework_Device_Bluetooth,
#endif
    NULL};

const uint16_t g_CLR_InteropAssembliesCount = (sizeof(g_CLR_InteropAssembliesNativeData) / sizeof(g_CLR_InteropAssembliesNativeData[0])) - 1;
