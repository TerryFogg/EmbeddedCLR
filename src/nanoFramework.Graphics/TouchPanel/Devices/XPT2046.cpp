//
// Copyright (c) .NET Foundation and Contributors
// Portions Copyright (c) Microsoft Corporation.  All rights reserved.
// See LICENSE file in the project root for full license information.
//

#ifndef XPT2046_H
#define XPT2046_H

#include "TouchDevice.h"
#include "TouchInterface.h"

struct TouchDevice g_TouchDevice;
extern TouchInterface g_TouchInterface;

bool TouchDevice::Initialize()
{
    return true;
}

bool TouchDevice::Enable(GPIO_INTERRUPT_SERVICE_ROUTINE touchIsrProc)
{
    if (touchIsrProc == NULL)
    {
    };
    return TRUE;
}

bool TouchDevice::Disable()
{
    return true;
}

TouchPointDevice TouchDevice::GetPoint()
{

    // stub
    TouchPointDevice TouchValue;

    TouchValue.x = 0;
    TouchValue.y = 0;

    return TouchValue;
}
#endif // XPT2046_H
