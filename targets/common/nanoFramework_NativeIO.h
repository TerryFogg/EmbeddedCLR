#pragma once
//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include <nanoCLR_Interop.h>
#include <nanoCLR_Runtime.h>
#include <nanoPackStruct.h>
#include <corlib_native.h>

typedef enum __nfpack PinCapabilities
{
    PinCapabilities_DigitalInput = 1,
    PinCapabilities_DigitalOutput = 2,
    PinCapabilities_Interrupt = 4,
    PinCapabilities_Adc = 8,
    PinCapabilities_Dac = 16,
    PinCapabilities_Pwm = 32,
    PinCapabilities_I2C = 64,
    PinCapabilities_Spi = 128,
    PinCapabilities_I2S = 256,
    PinCapabilities_Uart = 512,
    PinCapabilities_Can = 1024,
    PinCapabilities_Usb = 2048,
    PinCapabilities_Touch = 4096,
    PinCapabilities_PullUp = 8192,
    PinCapabilities_PullDown = 16384,
    PinCapabilities_OpenDrain = 32768,
    PinCapabilities_Rtc = 65536,
    PinCapabilities_Wakeup = 131072,
    PinCapabilities_Reserved = 262144,
    PinCapabilities_McPwm = 524288,
} PinCapabilities;

struct Library_nanoFramework_NativeIO_NativeAdc
{
    NANOCLR_NATIVE_DECLARE(Read___STATIC__I4__I4);

    //--//
};

struct Library_nanoFramework_NativeIO_NativeDac
{
    NANOCLR_NATIVE_DECLARE(Write___STATIC__VOID__I4__I4);

    //--//
};

struct Library_nanoFramework_NativeIO_NativeGpio
{
    NANOCLR_NATIVE_DECLARE(ConfigureInput___STATIC__VOID__I4);
    NANOCLR_NATIVE_DECLARE(ConfigureOutput___STATIC__VOID__I4);
    NANOCLR_NATIVE_DECLARE(Read___STATIC__BOOLEAN__I4);
    NANOCLR_NATIVE_DECLARE(Write___STATIC__VOID__I4__BOOLEAN);
    NANOCLR_NATIVE_DECLARE(EnableInterrupt___STATIC__VOID__I4);
    NANOCLR_NATIVE_DECLARE(DisableInterrupt___STATIC__VOID__I4);

    //--//
};

struct Library_nanoFramework_NativeIO_NativeI2C
{
    NANOCLR_NATIVE_DECLARE(Open___STATIC__I4__I4__I4__I4);

    //--//
};

struct Library_nanoFramework_NativeIO_NativePwm
{
    NANOCLR_NATIVE_DECLARE(Start___STATIC__VOID__I4__I4__I4);
    NANOCLR_NATIVE_DECLARE(Stop___STATIC__VOID__I4);
    NANOCLR_NATIVE_DECLARE(SetDuty___STATIC__VOID__I4);

    //--//
};

struct Library_nanoFramework_NativeIO_NativeTouch
{
    NANOCLR_NATIVE_DECLARE(Read___STATIC__I4__I4);

    //--//
};

extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_NativeIO;

