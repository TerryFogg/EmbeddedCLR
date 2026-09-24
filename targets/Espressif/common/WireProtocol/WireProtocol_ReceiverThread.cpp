//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include <esp32_idf.h>
#include <WireProtocol_HAL_Interface.h>
#include <WireProtocol_Message.h>
#include <WireProtocol_ReceiverThread.h>

void WP_Message_Process();
void WP_Message_PrepareReception();

void ReceiverThread(void const *argument)
{
    (void)argument;

    WP_Message_PrepareReception();

    // loop forever
    while (1)
    {
        WP_Message_Process();
        vTaskDelay(1);
    }
}
void WP_Message_PrepareReception_Platform()
{
    // empty on purpose, nothing to configure
}
