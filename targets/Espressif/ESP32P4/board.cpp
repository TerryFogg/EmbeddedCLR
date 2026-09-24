//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include "soc/uart_channel.h"
#include "hal/uart_types.h"
#include "CoreIO.h"
#include "board.h"

void InitializeBoard()
{

    // ---------------------------------------------
    // Setup Serial port for wire protocol debugging
    // ---------------------------------------------
    SerialIOPort WireProtocolSetup = {
        .usartDeviceNumber = UART_NUM_0,
        .pinTX = (PinNameValue)UART_NUM_0_TXD_DIRECT_GPIO_NUM,
        .pinRX = (PinNameValue)UART_NUM_0_RXD_DIRECT_GPIO_NUM,
        .baudrate = 921600,
        .dataBits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stopBits = UART_STOP_BITS_1,
        .flowControl = UART_HW_FLOWCTRL_DISABLE};
    SerialIO::Initialize(WireProtocolSetup);





}
