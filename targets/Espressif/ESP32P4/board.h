#pragma once
//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include "hal/uart_types.h"

#define WIRE_PROTOCOL_UART ((uart_port_t)UART_NUM_0)


#ifdef __cplusplus
extern "C"
{
#endif

    void InitializeBoard();

#ifdef __cplusplus
}
#endif
