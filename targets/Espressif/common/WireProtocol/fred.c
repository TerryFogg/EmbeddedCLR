//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include <soc/uart_channel.h>
#include "driver/uart.h"
#include "hal/uart_types.h"
#include <nanoHAL_v2.h>
#include <WireProtocol_HAL_Interface.h>

void WP_ReceiveBytes(uint8_t **ptr, uint32_t *size)
{
    uint32_t requestedSize = *size;
    if (*size)
    {
        size_t read = uart_read_bytes(0, *ptr, (uint32_t)requestedSize, pdMS_TO_TICKS(250));
        *ptr += read;
        *size -= read;
    }
}
uint8_t WP_TransmitMessage(WP_Message *message)
{
    if (uart_write_bytes(0, (const char *)&message->m_header, sizeof(message->m_header)) != sizeof(message->m_header))
    {
        return false;
    }
    if (message->m_header.m_size && message->m_payload)
    {
        if (uart_write_bytes(UART_NUM_0, (const char *)message->m_payload, message->m_header.m_size) != (int)message->m_header.m_size)
        {
            return false;
        }
    }
    return true;
}
