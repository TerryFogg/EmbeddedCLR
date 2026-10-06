//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include "nanoFramework_NativeIO.h"
#include "NativeIO.h"

HRESULT Library_nanoFramework_NativeIO_nanoFramework_NativeIO_NativeDac::Write___STATIC__VOID__I4__I4(
    CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        CLR_INT32 pinNumber = stack.Arg0().NumericByRef().s4;
        CLR_INT32 dacValue = stack.Arg1().NumericByRef().s4;
        DacIO().Write((PinNameValue)pinNumber, dacValue);
    }
    NANOCLR_NOCLEANUP();
}

