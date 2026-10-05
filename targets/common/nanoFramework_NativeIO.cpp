//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include "nanoFramework_NativeIO.h"

// clang-format off

static const CLR_RT_MethodHandler method_lookup[] =
{
    Library_nanoFramework_NativeIO_NativeAdc::Read___STATIC__I4__I4,
    Library_nanoFramework_NativeIO_NativeDac::Write___STATIC__VOID__I4__I4,
    Library_nanoFramework_NativeIO_NativeGpio::ConfigureInput___STATIC__VOID__I4,
    Library_nanoFramework_NativeIO_NativeGpio::ConfigureOutput___STATIC__VOID__I4,
    Library_nanoFramework_NativeIO_NativeGpio::Read___STATIC__BOOLEAN__I4,
    Library_nanoFramework_NativeIO_NativeGpio::Write___STATIC__VOID__I4__BOOLEAN,
    Library_nanoFramework_NativeIO_NativeGpio::EnableInterrupt___STATIC__VOID__I4,
    Library_nanoFramework_NativeIO_NativeGpio::DisableInterrupt___STATIC__VOID__I4,
    Library_nanoFramework_NativeIO_NativeI2C::Open___STATIC__I4__I4__I4__I4,
    Library_nanoFramework_NativeIO_NativePwm::Start___STATIC__VOID__I4__I4__I4,
    Library_nanoFramework_NativeIO_NativePwm::Stop___STATIC__VOID__I4,
    Library_nanoFramework_NativeIO_NativePwm::SetDuty___STATIC__VOID__I4,
    Library_nanoFramework_NativeIO_NativeTouch::Read___STATIC__I4__I4,
};

const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_NativeIO =
{
    "nanoFramework.NativeIO",
    0x12C22875,
    method_lookup,
    { 100, 1, 0, 6 }
};

// clang-format on
