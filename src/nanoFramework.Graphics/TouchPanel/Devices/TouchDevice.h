#pragma once
// Copyright (c) .NET Foundation and Contributors
// Portions Copyright (c) Microsoft Corporation.  All rights reserved.
// See LICENSE file in the project root for full license information.

#include "nanoCLR_Types.h"

struct TouchPointDevice
{
    int x;
    int y;
};
struct TouchDevice
{
    bool Initialize();
    TouchPointDevice GetPoint();
    bool Enable(GPIO_INTERRUPT_SERVICE_ROUTINE touchIsrProc);
    bool Disable();
};
