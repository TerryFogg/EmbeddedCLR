//
// Copyright (c) .NET Foundation and Contributors
// Portions Copyright (c) Microsoft Corporation.  All rights reserved.
// See LICENSE file in the project root for full license information.
//

#include "Graphics.h"
#include "nanoFramework_Graphics.h"
#include "TouchPanel.h"
extern TouchPanel g_TouchPanel;

HRESULT Library_nanoFramework_Graphics_nanoFramework_UI_Touch::Enable___STATIC__VOID(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        g_TouchPanel.Enable();
    }
    NANOCLR_NOCLEANUP_NOLABEL();
}

HRESULT Library_nanoFramework_Graphics_nanoFramework_UI_Touch::Disable___STATIC__VOID(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        g_TouchPanel.Disable();
    }
    NANOCLR_NOCLEANUP_NOLABEL();
}
