//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include "nanoFramework_NativeIO.h"
#include "CoreIO.h"

HRESULT Library_nanoFramework_NativeIO_NativeDac::Write___STATIC__VOID__I4__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        DacIO().Write((PinNameValue)stack.Arg0().NumericByRef().s4, stack.Arg1().NumericByRef().s4);
    }
    NANOCLR_NOCLEANUP_NOLABEL();
}
