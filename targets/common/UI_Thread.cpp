//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "TouchPanel.h"
#include "UI_Thread.h"

extern TouchPanel g_TouchPanel;
TaskHandle_t g_uiTaskHandle = nullptr;

void UITask(void *arg)
{
    for (;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        g_TouchPanel.PollTouchPoint();
    }
}

void StartUITask()
{
    // UI task pinned to core
    xTaskCreatePinnedToCore(&UITask, "UITask", 4096, NULL, 4, &g_uiTaskHandle, 0);
}

