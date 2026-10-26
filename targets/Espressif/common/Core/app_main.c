//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

//
// Task/Core allocation
//
// Core 0
// ------
// UITask
//     - LCD panel initialization
//     - Touch device initialization
//     - Touch interrupt handling
//     - Touch coordinate processing
//     - Touch event generation
//
// Core 1
// ------
// CLRStartupThread
//     - C# interpreter
//     - Managed application execution
//
// Design notes:
//     - UI and touch processing run independently of the CLR.
//     - LCD and Touch device initialization occur in parallel with CLR startup.
//     - Heavy touch activity cannot directly consume CLR CPU time.
//     - Touch ISR only signals UITask.
//     - All touch I2C access is performed by UITask in task context.
//     - UITask blocks while idle and consumes essentially no CPU.
//

#include <targetHAL.h>
#include <nanoCLR_Application.h>
#include <WireProtocol_ReceiverThread.h>
#include "board.h"
#include "UI_Thread.h"
#include "freertos/idf_additions.h"
#include "soc/interrupt_reg.h"

#include <stdbool.h>

extern void CLRStartupThread(void const *argument);
static void receiver_task(void *pvParameter);
static void main_task(void *pvParameter);
extern void StartUITask();

// Entry call from Esp32 IDF start up code
void app_main()
{
    UBaseType_t taskPriority = 5;

    InitializeBoard();

    bool WaitForDebugger = WaitForDebug();

    StartUITask();

    // Receiver task pinned to core 0
    xTaskCreatePinnedToCore(&receiver_task, "ReceiverThread", 3072, NULL, taskPriority, NULL, 0);

    // CLR main task pinned to core 1
    xTaskCreatePinnedToCore(&main_task, "main_task", 15000, (void *)&WaitForDebugger, taskPriority,NULL , 1);
}

static void main_task(void *pvParameter)
{
    bool RequestWaitForDebugger = *(bool *)pvParameter;

    CLR_SETTINGS clrSettings = {
        .EnterDebuggerLoopAfterExit = true,
        .MaxContextSwitches = 50,
        .RevertToBooterOnFault = false,
        .WaitForDebugger = RequestWaitForDebugger};
    CLRStartupThread(&clrSettings);
    vTaskDelete(NULL);
}

static void receiver_task(void *pvParameter)
{
    (void)pvParameter;
    ReceiverThread(0);
    vTaskDelete(NULL);
}
