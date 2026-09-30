//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include <targetHAL.h>
#include <nanoCLR_Application.h>
#include <WireProtocol_ReceiverThread.h>
#include "board.h"

extern void CLRStartupThread(void const *argument);

void receiver_task(void *pvParameter)
{
    (void)pvParameter;
    ReceiverThread(0);
    vTaskDelete(NULL);
}
void main_task(void *pvParameter)
{
    (void)pvParameter;
    CLR_SETTINGS clrSettings = {
        .EnterDebuggerLoopAfterExit = true,
        .MaxContextSwitches = 50,
        .RevertToBooterOnFault = false,
        .WaitForDebugger = false};
    CLRStartupThread(&clrSettings);
    vTaskDelete(NULL);
}
// App_main called from Esp32 IDF start up code
void app_main()
{
    UBaseType_t taskPriority = 5;
    // Switch off logging so as not to interfere with WireProtocol over Uart0
    esp_log_level_set("*", ESP_LOG_NONE);
    ESP_ERROR_CHECK(nvs_flash_init());
    InitializeBoard();
    vTaskPrioritySet(NULL, taskPriority);
    // start receiver task pinned to core 0
    xTaskCreatePinnedToCore(&receiver_task, "ReceiverThread", 3072, NULL, taskPriority, NULL, 0);
    // start the CLR main task pinned to core 1
    xTaskCreatePinnedToCore(&main_task, "main_task", 15000, NULL, taskPriority, NULL, 1);
}
