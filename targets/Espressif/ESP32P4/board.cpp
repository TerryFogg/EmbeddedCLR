//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include "soc/uart_channel.h"
#include "hal/uart_types.h"
#include "CoreIO.h"
#include "board.h"
#include "driver/uart.h"
#include <nanoHAL_v2.h>
#include <WireProtocol_HAL_Interface.h>

void InitializeWireProtocol();


void InitializeBoard()
{
    InitializeWireProtocol();
}


// ---------------------------------------------
// Setup Serial port for wire protocol debugging
// ---------------------------------------------
void InitializeWireProtocol()
{
    SerialIOPort WireProtocolSetup = {
        .usartDeviceNumber = uart_port_t::UART_NUM_0,
        .pinTX = PinNameValue::GPIO37,
        .pinRX = PinNameValue::GPIO38,
        .baudrate = 921600,
        .dataBits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stopBits = UART_STOP_BITS_1,
        .flowControl = UART_HW_FLOWCTRL_DISABLE};
    SerialIO::Initialize(WireProtocolSetup);
}
