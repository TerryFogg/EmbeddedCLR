//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include "nanoFramework_NativeCalls.h"
#include "NativeIO.h"

// Setup interrupts to be one shot, must be re-enabled after each interrupt.
// This is to avoid flooding the event queue with interrupts if the pin is bouncing.
void Gpio_Interrupt_ISR(GPIO_PIN pinNumber, int pinLevel)
{
    GpioIO::DisableInterrupt((PinNameValue)pinNumber);
    PostManagedEvent(EVENT_GPIO, 0, (uint16_t)pinNumber, pinLevel);
}

HRESULT Library_nanoFramework_NativeCalls_NativeCalls_NativeGpio::
    ConfigurePin___STATIC__VOID__I4__NativeCallsGpioPinMode__NativeCallsGpioBias(
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

HRESULT Library_nanoFramework_NativeCalls_NativeCalls_NativeGpio::Read___STATIC__NativeCallsGpioPinLevel__I4(
    CLR_RT_StackFrame &stack)
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

HRESULT Library_nanoFramework_NativeCalls_NativeCalls_NativeGpio::Write___STATIC__VOID__I4__NativeCallsGpioPinLevel(
    CLR_RT_StackFrame &stack)
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

HRESULT Library_nanoFramework_NativeCalls_NativeCalls_NativeGpio::
    AddInterrupt___STATIC__VOID__I4__NativeCallsGpioInterruptMode(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        CLR_INT32 pinNumber = stack.Arg0().NumericByRef().s4;
        GpioInterruptMode interruptMode = (GpioInterruptMode)stack.Arg1().NumericByRef().s4;
        GpioIO::AddInterrupt((PinNameValue)pinNumber, interruptMode);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_nanoFramework_NativeCalls_NativeCalls_NativeGpio::EnableInterrupt___STATIC__VOID__I4(
    CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        CLR_INT32 pinNumber = stack.Arg0().NumericByRef().s4;
        GpioIO::EnableInterrupt((PinNameValue)pinNumber);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_nanoFramework_NativeCalls_NativeCalls_NativeGpio::DisableInterrupt___STATIC__VOID__I4(
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

HRESULT Library_nanoFramework_NativeCalls_NativeCalls_NativeGpio::RemoveInterrupt___STATIC__VOID__I4(
    CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        CLR_INT32 pinNumber = stack.Arg0().NumericByRef().s4;
        GpioIO::RemoveInterrupt((PinNameValue)pinNumber);
    }
    NANOCLR_NOCLEANUP();
}
