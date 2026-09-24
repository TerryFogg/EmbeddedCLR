//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include <target_platform.h>
#include "CoreIO.h"
#include "board.h"
#include <nanoHAL_v2.h>
#include <WireProtocol.h>
#include <WireProtocol_Message.h>
#include <WireProtocol_HAL_Interface.h>

// -------------------------------------------------------------------------
// NOTE: The Wire protocol hardware is setup during the board initialization
// -------------------------------------------------------------------------

void WP_ReceiveBytes(uint8_t **ptr, uint32_t *size)
{
    uint32_t requestedSize = *size;

    // Its possible to have zero sized packets, so check for request with 0 size
    if (*size)
    {
        // non blocking read from serial port with 100ms timeout
        size_t read = SerialIO::Read(WIRE_PROTOCOL_UART, *ptr, (uint32_t)requestedSize,100);
        *ptr += read;
        *size -= read;
    }
}
uint8_t WP_TransmitMessage(WP_Message *message)
{
    if (SerialIO::Write( WIRE_PROTOCOL_UART, (unsigned char *)&message->m_header, sizeof(message->m_header)) != sizeof(message->m_header))
    {
        return false;
    }
    // if there is anything on the payload send it to the output stream
    if (message->m_header.m_size && message->m_payload)
    {
          if (SerialIO::Write(WIRE_PROTOCOL_UART, (unsigned char *)&message->m_payload, sizeof(message->m_payload)) !=  (int)message->m_header.m_size)
        {
            return false;
        }
    }
    return true;
}
