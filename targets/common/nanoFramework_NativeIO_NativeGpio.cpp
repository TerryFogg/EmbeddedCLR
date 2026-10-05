//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include "nanoFramework_NativeIO.h"
#include "CoreIO.h"

HRESULT Library_nanoFramework_NativeIO_NativeGpio::ConfigureInput___STATIC__VOID__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        GpioIO::InitializePin(GPIO28, GpioPinMode::MODE_INPUT, GpioBias::NOBIAS);
    }
    NANOCLR_NOCLEANUP_NOLABEL();
}

HRESULT Library_nanoFramework_NativeIO_NativeGpio::ConfigureOutput___STATIC__VOID__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        GpioIO::InitializePin(GPIO28, GpioPinMode::MODE_OUTPUT, GpioBias::NOBIAS);
    }
    NANOCLR_NOCLEANUP_NOLABEL();
}

HRESULT Library_nanoFramework_NativeIO_NativeGpio::Read___STATIC__BOOLEAN__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        GpioIO::InitializePin(GPIO28, GpioPinMode::MODE_INPUT, GpioBias::NOBIAS);
    }
    NANOCLR_NOCLEANUP_NOLABEL();
}

HRESULT Library_nanoFramework_NativeIO_NativeGpio::Write___STATIC__VOID__I4__BOOLEAN(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        GpioIO::InitializePin(GPIO28, GpioPinMode::MODE_INPUT, GpioBias::NOBIAS);
    }
    NANOCLR_NOCLEANUP_NOLABEL();
}

HRESULT Library_nanoFramework_NativeIO_NativeGpio::EnableInterrupt___STATIC__VOID__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        GpioIO::InitializePin(GPIO28, GpioPinMode::MODE_INPUT, GpioBias::NOBIAS);
    }
    NANOCLR_NOCLEANUP_NOLABEL();
}

HRESULT Library_nanoFramework_NativeIO_NativeGpio::DisableInterrupt___STATIC__VOID__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        GpioIO::InitializePin(GPIO28, GpioPinMode::MODE_INPUT, GpioBias::NOBIAS);
    }
    NANOCLR_NOCLEANUP_NOLABEL();
}
