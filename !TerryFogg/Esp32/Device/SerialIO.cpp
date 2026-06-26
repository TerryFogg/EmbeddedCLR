//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include "Device.IO.h"

#pragma region System.IO.Ports
static USART_Properties *mcuUSART;
void SerialIO::SetupUsartList(USART_Properties *boardMcuUsart)
{
    mcuUSART = boardMcuUsart;
}
bool SerialIO::Initialize(CLR_INT32 usartDeviceNumber, CLR_INT32 baudrate)
{
    if (!mcuUSART[usartDeviceNumber].USART_Initialized)
    {
        uart_init(uart_get_instance(usartDeviceNumber), baudrate);
        gpio_set_function(mcuUSART[usartDeviceNumber].tx, GPIO_FUNC_UART);
        gpio_set_function(mcuUSART[usartDeviceNumber].rx, GPIO_FUNC_UART);
        mcuUSART[usartDeviceNumber].USART_Initialized = true;
    }
    return true;
}
bool SerialIO::Dispose(CLR_INT32 usartDeviceNumber)
{
    uart_deinit(uart_get_instance(usartDeviceNumber));
    return true;
}
CLR_INT32 SerialIO::BytesAvailable(CLR_INT32 usartDeviceNumber)
{
    uart_inst_t *uart = uart_get_instance(usartDeviceNumber);
    (void)uart;
    return 1;
}
bool SerialIO::ReadBytes(CLR_INT32 usartDeviceNumber, CLR_UINT8 *data, CLR_INT32 length)
{
    (void)length;

    uart_inst_t *uart = uart_get_instance(usartDeviceNumber);
    (void)uart;
    return true;
}
bool SerialIO::ReadLine(CLR_INT32 usartDeviceNumber, char *newLine, CLR_UINT8 length, char *line)
{
    (void)newLine;
    (void)line;
    (void)length;

    uart_inst_t *uart = uart_get_instance(usartDeviceNumber);
    (void)uart;
    return true;
}
bool SerialIO::SetSignalLevels(CLR_INT32 usartDeviceNumber, bool inversion)
{
    (void)inversion;

    uart_inst_t *uart = uart_get_instance(usartDeviceNumber);
    (void)uart;
    return true;
}
bool SerialIO::GetSignalLevels(CLR_INT32 usartDeviceNumber)
{
    // bool result = IsSerialSignalInverted(UsartDeviceNumber);
    return true;
}
bool SerialIO::WriteBytes(CLR_INT32 usartDeviceNumber, CLR_UINT8 *data, CLR_INT32 count)
{
    (void)data;
    (void)count;
    uart_inst_t *uart = uart_get_instance(usartDeviceNumber);
    (void)uart;

    return true;
}
bool SerialIO::SetWatchCharacter(CLR_INT32 usartDeviceNumber, CLR_UINT8 watch_character)
{
    uart_inst_t *uart = uart_get_instance(usartDeviceNumber);
    (void)watch_character;
    return true;
}
bool SerialIO::SetReceiveThreshold(CLR_INT32 usartDeviceNumber, CLR_INT32 threshold)
{
    uart_inst_t *uart = uart_get_instance(usartDeviceNumber);
    (void)threshold;
    return 1;
}
char *SerialIO::GetDevice(CLR_INT32 usartDeviceNumber)
{
    uart_inst_t *uart = uart_get_instance(usartDeviceNumber);
    (void)uart;
    static char DeviceSelector[1];
    DeviceSelector[0] = ' ';
    return DeviceSelector;
}
bool SerialIO::InvertSignalLevels(CLR_INT32 usartDeviceNumber, bool InvertSignal)
{
    (void)InvertSignal;

    uart_inst_t *uart = uart_get_instance(usartDeviceNumber);
    (void)uart;
    return true;
}
bool SerialIO::SetBaudRate(CLR_INT32 usartDeviceNumber, CLR_INT32 baudRate)
{
    int actualBaudRate = uart_set_baudrate(uart_get_instance(usartDeviceNumber), baudRate);
    return true;
}
bool SerialIO::SetConfig(
    CLR_INT32 usartDeviceNumber,
    SerialMode serialMode,
    CLR_INT32 stopBits,
    CLR_INT32 dataBits,
    CLR_INT32 RequestedParity)
{
    uart_parity_t parity = (uart_parity_t)RequestedParity;
    uart_set_format(uart_get_instance(usartDeviceNumber), dataBits, stopBits, parity);
    return true;
}
bool SerialIO::SetHandshake(CLR_INT32 usartDeviceNumber, Handshake handshake)
{
    bool cts = false;
    bool rts = false;
    bool status = false;

    switch (handshake)
    {
        case Handshake::Handshake_None:
            status = true;
            break;
        case Handshake_XOnXOff:
            break;
        case Handshake::Handshake_RequestToSend:
            cts = true;
            rts = true;
            status = true;
            break;
        case Handshake::Handshake_RequestToSendXOnXOff:
            status = true;
            break;
    }

    uart_set_hw_flow(uart_get_instance(usartDeviceNumber), cts, rts);
    return status;
}
HRESULT SerialIO::SetupWriteLine(CLR_RT_StackFrame &stack, char **buffer, uint32_t *length, bool *isNewAllocation)
{
    (void)stack;
    (void)buffer;
    (void)length;
    (void)isNewAllocation;
    return 1;
}
bool SerialIO::SetMode(CLR_INT32 UsartDeviceNumber, CLR_INT32 mode)
{
    return true;
}
#pragma endregion
