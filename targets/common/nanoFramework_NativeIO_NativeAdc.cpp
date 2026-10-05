//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include "nanoFramework_NativeIO.h"
#include "CoreIO.h"


HRESULT Library_nanoFramework_NativeIO_NativeAdc::Read___STATIC__I4__I4( CLR_RT_StackFrame &stack )
{
    NANOCLR_HEADER();
    {
        int channelNumber = stack.Arg0().NumericByRef().s4;
        int adcValue = 0;
        AdcIO::Read(channelNumber, &adcValue);
        stack.SetResult_I4(adcValue);
    }
    NANOCLR_NOCLEANUP_NOLABEL();
}
