//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include "NativeCalls.h"
#include "NativeIO.h"

// Setup interrupts to be one shot, must be re-enabled after each interrupt.
// This is to avoid flooding the event queue with interrupts if the pin is bouncing.
void Gpio_Interrupt_ISR(GPIO_PIN pinNumber, int pinLevel)
{
    GpioIO::DisableInterrupt((PinNameValue)pinNumber);
    PostManagedEvent(EVENT_GPIO, 0, (uint16_t)pinNumber, pinLevel);
}

HRESULT Library_NativeCalls_Gpio::ConfigurePin___STATIC__VOID__I4__GpioPinMode__GpioBias(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        CLR_INT32 pinNumber = stack.Arg0().NumericByRef().s4;
        GpioPinMode pinMode = (GpioPinMode)stack.Arg1().NumericByRef().s4;
        GpioBias gpioBias = (GpioBias)stack.Arg2().NumericByRef().s4;
        GpioIO::InitializePin((PinNameValue)pinNumber, pinMode, gpioBias);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Gpio::Read___STATIC__GpioPinLevel__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        CLR_INT32 pinNumber = stack.Arg0().NumericByRef().s4;
        GpioPinLevel value = GpioIO::ReadLevel((PinNameValue)pinNumber);
        stack.SetResult_I4(value);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Gpio::Write___STATIC__VOID__I4__GpioPinLevel(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        CLR_INT32 pinNumber = stack.Arg0().NumericByRef().s4;
        GpioPinLevel level = (GpioPinLevel)stack.Arg1().NumericByRef().s4;
        GpioIO::SetLevel((PinNameValue)pinNumber, level);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Gpio::AddInterrupt___STATIC__VOID__I4__GpioInterruptMode(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        CLR_INT32 pinNumber = stack.Arg0().NumericByRef().s4;
        GpioInterruptMode interruptMode = (GpioInterruptMode)stack.Arg1().NumericByRef().s4;
        GpioIO::AddInterrupt((PinNameValue)pinNumber, interruptMode);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Gpio::EnableInterrupt___STATIC__VOID__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        CLR_INT32 pinNumber = stack.Arg0().NumericByRef().s4;
        GpioIO::EnableInterrupt((PinNameValue)pinNumber);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Gpio::DisableInterrupt___STATIC__VOID__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        CLR_INT32 pinNumber = stack.Arg0().NumericByRef().s4;
        GpioIO::DisableInterrupt((PinNameValue)pinNumber);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Gpio::RemoveInterrupt___STATIC__VOID__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        CLR_INT32 pinNumber = stack.Arg0().NumericByRef().s4;
        GpioIO::RemoveInterrupt((PinNameValue)pinNumber);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}
