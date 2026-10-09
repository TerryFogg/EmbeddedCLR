//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include <Core.h>

extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_mscorlib;
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_Runtime_Native;
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_NativeCalls;
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_Runtime_Events;
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_Runtime_Events_EventSink_DriverProcs;

#ifdef CONFIG_SUPPORT_RESOURCEMANAGER
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_ResourceManager;
#endif
#if CONFIG_SUPPORT_COLLECTIONS
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_System_Collections;
#endif
#if CONFIG_SUPPORT_TEXT
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_System_Text;
#endif
#if CONFIG_SUPPORT_CRYPTOGRAPHY
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_System_Security_Cryptography;
#endif
#if CONFIG_SUPPORT_MATHEMATICS
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_System_Math;
#endif
#if CONFIG_SUPPORT_RUNTIMESERIALIZATION
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_System_Runtime_Serialization;
#endif
#if CONFIG_SUPPORT_GRAPHICS
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_Graphics;
#endif

const CLR_RT_NativeAssemblyData *g_CLR_InteropAssembliesNativeData[] = {
    &g_CLR_AssemblyNative_mscorlib,
    &g_CLR_AssemblyNative_nanoFramework_Runtime_Native,
    &g_CLR_AssemblyNative_nanoFramework_NativeCalls,
    &g_CLR_AssemblyNative_nanoFramework_Runtime_Events,
    &g_CLR_AssemblyNative_nanoFramework_Runtime_Events_EventSink_DriverProcs,
#if CONFIG_SUPPORT_RESOURCEMANAGER
    &g_CLR_AssemblyNative_nanoFramework_ResourceManager,
#endif
#if CONFIG_SUPPORT_COLLECTIONS
    &g_CLR_AssemblyNative_nanoFramework_System_Collections,
#endif
#if CONFIG_SUPPORT_TEXT
    &g_CLR_AssemblyNative_nanoFramework_System_Text,
#endif
#if CONFIG_SUPPORT_CRYPTOGRAPHY
    &g_CLR_AssemblyNative_nanoFramework_System_Security_Cryptography,
#endif
#if CONFIG_SUPPORT_MATHEMATICS
    &g_CLR_AssemblyNative_System_Math,
#endif
#if CONFIG_SUPPORT_RUNTIMESERIALIZATION
    &g_CLR_AssemblyNative_System_Runtime_Serialization,
#endif
#if CONFIG_SUPPORT_GRAPHICS
    &g_CLR_AssemblyNative_nanoFramework_Graphics,
#endif
    NULL};

const uint16_t g_CLR_InteropAssembliesCount = (sizeof(g_CLR_InteropAssembliesNativeData) / sizeof(g_CLR_InteropAssembliesNativeData[0])) - 1;
