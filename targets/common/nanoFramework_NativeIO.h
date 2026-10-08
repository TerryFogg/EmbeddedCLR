//-----------------------------------------------------------------------------
//
//                   ** WARNING! **
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------

#ifndef NANOFRAMEWORK_NATIVEIO_H
#define NANOFRAMEWORK_NATIVEIO_H

#include <nanoCLR_Interop.h>
#include <nanoCLR_Runtime.h>
#include <nanoPackStruct.h>
#include <corlib_native.h>

typedef enum __nfpack BusSpeed
{
    BusSpeed_Standard = 0,
    BusSpeed_Fast = 1,
    BusSpeed_FastPlus = 2,
} BusSpeed;

typedef enum __nfpack GpioBias
{
    GpioBias_NoBias = 0,
    GpioBias_PullUp = 1,
    GpioBias_PullDown = 2,
} GpioBias;

typedef enum __nfpack GpioInterruptMode
{
    GpioInterruptMode_EdgeLow = 0,
    GpioInterruptMode_EdgeHigh = 1,
    GpioInterruptMode_EdgeBoth = 2,
} GpioInterruptMode;

typedef enum __nfpack GpioPinLevel
{
    GpioPinLevel_Low = 0,
    GpioPinLevel_High = 1,
} GpioPinLevel;

typedef enum __nfpack GpioPinMode
{
    GpioPinMode_None = 0,
    GpioPinMode_Input = 1,
    GpioPinMode_Output = 2,
    GpioPinMode_OutputOpenDrain = 3,
} GpioPinMode;

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

struct Library_nanoFramework_NativeIO_nanoFramework_NativeIO_NativeAdc
{
    NANOCLR_NATIVE_DECLARE(Read___STATIC__I4__I4);

    //--//
};

struct Library_nanoFramework_NativeIO_nanoFramework_NativeIO_NativeDac
{
    NANOCLR_NATIVE_DECLARE(Write___STATIC__VOID__I4__I4);

    //--//
};

struct Library_nanoFramework_NativeIO_nanoFramework_NativeIO_NativeGpio
{
    NANOCLR_NATIVE_DECLARE(
        ConfigurePin___STATIC__VOID__I4__nanoFrameworkNativeIOGpioPinMode__nanoFrameworkNativeIOGpioBias);
    NANOCLR_NATIVE_DECLARE(Read___STATIC__nanoFrameworkNativeIOGpioPinLevel__I4);
    NANOCLR_NATIVE_DECLARE(Write___STATIC__VOID__I4__nanoFrameworkNativeIOGpioPinLevel);
    NANOCLR_NATIVE_DECLARE(AddInterrupt___STATIC__VOID__I4__nanoFrameworkNativeIOGpioInterruptMode);
    NANOCLR_NATIVE_DECLARE(EnableInterrupt___STATIC__VOID__I4);
    NANOCLR_NATIVE_DECLARE(DisableInterrupt___STATIC__VOID__I4);
    NANOCLR_NATIVE_DECLARE(RemoveInterrupt___STATIC__VOID__I4);

    //--//
};

struct Library_nanoFramework_NativeIO_nanoFramework_NativeIO_NativeI2C
{
    NANOCLR_NATIVE_DECLARE(Open___STATIC__BOOLEAN__I4__I4__I4);
    NANOCLR_NATIVE_DECLARE(AddSlaveDevice___STATIC__BOOLEAN__I4__nanoFrameworkNativeIOBusSpeed__U2);
    NANOCLR_NATIVE_DECLARE(Probe___STATIC__BOOLEAN__I4__U2__I4);
    NANOCLR_NATIVE_DECLARE(Write___STATIC__BOOLEAN__I4__U2__SZARRAY_U1__I4);
    NANOCLR_NATIVE_DECLARE(Read___STATIC__BOOLEAN__I4__U2__SZARRAY_U1__I4);
    NANOCLR_NATIVE_DECLARE(WriteRead___STATIC__BOOLEAN__I4__U2__SZARRAY_U1__I4__SZARRAY_U1__I4);

    //--//
};

struct Library_nanoFramework_NativeIO_nanoFramework_NativeIO_NativePwm
{
    NANOCLR_NATIVE_DECLARE(Initialize___STATIC__VOID__I4);
    NANOCLR_NATIVE_DECLARE(ConfigurePin___STATIC__VOID__I4__I4__I4);
    NANOCLR_NATIVE_DECLARE(SetDutyCycle___STATIC__VOID__I4__I4);
    NANOCLR_NATIVE_DECLARE(SetFrequency___STATIC__VOID__I4__I4);
    NANOCLR_NATIVE_DECLARE(Start___STATIC__VOID__I4__I4);
    NANOCLR_NATIVE_DECLARE(Stop___STATIC__VOID__I4__BOOLEAN);

    //--//
};

struct Library_nanoFramework_NativeIO_nanoFramework_NativeIO_NativeTouch
{
    NANOCLR_NATIVE_DECLARE(Read___STATIC__I4__I4);

    //--//
};

extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_NativeIO;

#endif // NANOFRAMEWORK_NATIVEIO_H
