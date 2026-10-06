//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include "nanoFramework_NativeIO.h"
#include "NativeIO.h"

void Gpio_Interrupt_ISR(GPIO_PIN pinNumber, bool pinState)
{
    PostManagedEvent(EVENT_GPIO, 0, (uint16_t)pinNumber, (uint32_t)pinState);
}


HRESULT Library_nanoFramework_NativeIO_nanoFramework_NativeIO_NativeGpio::
    ConfigurePin___STATIC__VOID__I4__nanoFrameworkNativeIOGpioPinMode__nanoFrameworkNativeIOGpioBias(
        CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        CLR_INT32 pinNumber = stack.Arg0().NumericByRef().s4;
        GpioPinMode pinMode = (GpioPinMode)stack.Arg1().NumericByRef().s4;
        GpioBias gpioBias = (GpioBias)stack.Arg2().NumericByRef().s4;
        GpioIO::InitializePin((PinNameValue)pinNumber, pinMode, gpioBias);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_nanoFramework_NativeIO_nanoFramework_NativeIO_NativeGpio::
    Read___STATIC__nanoFrameworkNativeIOGpioPinLevel__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        CLR_INT32 pinNumber = stack.Arg0().NumericByRef().s4;
        GpioPinLevel value = GpioIO::ReadLevel((PinNameValue)pinNumber);
        stack.SetResult_I4(value);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_nanoFramework_NativeIO_nanoFramework_NativeIO_NativeGpio::
    Write___STATIC__VOID__I4__nanoFrameworkNativeIOGpioPinLevel(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        CLR_INT32 pinNumber = stack.Arg0().NumericByRef().s4;
        GpioPinLevel level = (GpioPinLevel)stack.Arg1().NumericByRef().s4;
        GpioIO::SetLevel((PinNameValue)pinNumber, level);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_nanoFramework_NativeIO_nanoFramework_NativeIO_NativeGpio::EnableInterrupt___STATIC__VOID__I4(
    CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        CLR_INT32 pinNumber = stack.Arg0().NumericByRef().s4;
        GpioIO::EnableInterrupt(
            (PinNameValue)pinNumber,
            GPIO_INTERRUPT_EDGE_GPIO_INTERRUPT_EDGE_BOTH);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_nanoFramework_NativeIO_nanoFramework_NativeIO_NativeGpio::DisableInterrupt___STATIC__VOID__I4(
    CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        CLR_INT32 pinNumber = stack.Arg0().NumericByRef().s4;
        GpioIO::DisableInterrupt((PinNameValue)pinNumber);
    }
    NANOCLR_NOCLEANUP();
}
