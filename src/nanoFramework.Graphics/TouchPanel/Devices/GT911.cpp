//
// Copyright (c) .NET Foundation and Contributors
// Portions Copyright (c) Microsoft Corporation.  All rights reserved.
//

#include "TouchDevice.h"
#include "TouchInterface.h"
#include "NativeIO.h"
#include "board.h"

struct TouchDevice g_TouchDevice;

#pragma pack(push, 1)
typedef struct
{
    uint8_t Status;
    uint8_t TrackId;
    uint16_t X;
    uint16_t Y;
    uint16_t Size;
} GT911TouchRecord;
#pragma pack(pop)

extern TaskHandle_t g_uiTaskHandle;

static void local_touch_gpio_callback(void *arg)
{
    BaseType_t taskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(g_uiTaskHandle, &taskWoken);
    portYIELD_FROM_ISR(taskWoken);
}

bool TouchDevice::Initialize()
{
    // The default slave address is the primary, but force it by placing a zero voltage
    // on the interrupt pin of the GT911/GT9271 which is read at reset (forcing primary slave address 0x5D)
    // Set it back to an input afterwards
    // ----------------------------------
    GpioIO::InitializePin(TOUCH_INTERRUPT_PIN, GpioPinMode_Output, GpioBias_PullDown);

    // Reset the GT911 controller
    // --------------------------
    GpioIO::InitializePin(TOUCH_RESET_PIN, GpioPinMode_Output, GpioBias_NoBias);
    GpioIO::SetLevel(TOUCH_RESET_PIN, GpioPinLevel_Low);
    PLATFORM_DELAY(5);
    GpioIO::SetLevel(TOUCH_RESET_PIN, GpioPinLevel_High);
    PLATFORM_DELAY(50);

    // Check the device is answering to the default primary address
    // ------------------------------------------------------------
    bool found_0x5D = I2cIO::Probe(INTERNAL_SHARED_I2C_MASTER_BUS, LCD_TOUCH_GT911_ADDRESS, 100);
    if (!found_0x5D)
    {
        return false;
    }

    I2cIO::AddDevice(INTERNAL_SHARED_I2C_MASTER_BUS, 100000, LCD_TOUCH_GT911_ADDRESS);

    // Set interrupt input pin to pullup during normal operation
    // ---------------------------------------------------------
    GpioIO::InitializePin(TOUCH_INTERRUPT_PIN, GpioPinMode_Input, GpioBias_NoBias);

    return true;
}
bool TouchDevice::Enable()
{
    GpioIO::AddInterrupt(TOUCH_INTERRUPT_PIN, GpioInterruptMode_EdgeLow, (void *)local_touch_gpio_callback);
    GpioIO::EnableInterrupt(TOUCH_INTERRUPT_PIN);
    return true;
}
bool TouchDevice::Disable()
{
    GpioIO::DisableInterrupt(TOUCH_INTERRUPT_PIN);
    return true;
}
TouchPointDevice TouchDevice::GetPoint()
{
    // Many touch controllers provide explicit events like: DOWN/MOVE/UP,
    // the GT911 does not, it has a data changed interrupt

    // GT911/GT9271 Register commands
    // ------------------------------
    uint8_t ClearStatusAndCoordinates[3] = {0x81, 0x4E, 0x00};
    uint8_t ReadXYCoordinates[2] = {0x81, 0x4E};
    TouchPointDevice tp = {.x = 0, .y = 0, .touchStatus = TouchStatus::NoChange};
    GT911TouchRecord touch = {.Status = 0, .TrackId = 0, .X = 0, .Y = 0, .Size = 0};

    I2cIO::WriteRead(LCD_TOUCH_GT911_ADDRESS, ReadXYCoordinates, 2, (uint8_t *)&touch, sizeof(touch), 10);
    I2cIO::Write(LCD_TOUCH_GT911_ADDRESS, ClearStatusAndCoordinates, sizeof(ClearStatusAndCoordinates), 10);

    bool NewDataAvailable = (touch.Status & 0x80) != 0;
    uint8_t TouchCount = touch.Status & 0x0F;
    if (NewDataAvailable)
    {
        // Switch x and y in landscape mode
        tp.y = touch.X;
        tp.x = TOUCH_WIDTH - touch.Y;

        if (TouchCount > 0)
        {
            tp.touchStatus = TouchStatus::TouchDown;
        }
        else
        {
            tp.touchStatus = TouchStatus::TouchUp;
        }
    }
    else
    {
        tp.touchStatus = TouchStatus::NoChange;
    }
    return tp;
}
