//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include "NativeCalls.h"
#include "NativeIO.h"


HRESULT Library_NativeCalls_Adc::Read___STATIC__I4__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        CLR_INT32 pinNumber = stack.Arg0().NumericByRef().s4;
        int adcValue = 0;
        AdcIO::Read(pinNumber, &adcValue);
        stack.SetResult_I4(adcValue);
    }
    NANOCLR_NOCLEANUP();
}
