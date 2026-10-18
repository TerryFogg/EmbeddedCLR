//
// Copyright (c) .NET Foundation and Contributors
// Portions Copyright (c) Microsoft Corporation.  All rights reserved.

#include "TouchDevice.h"
#include "TouchInterface.h"
#include "NativeIO.h"
#include "esp_err.h"
#include "board.h"

// +------------------------------------+
// | Timing for Write Operation to GT911|
// +------------------------------------+
//  After setting the starting register address for Write operation, it is allowed to write one or more bytes at a time.
//  GT911 will automatically increase the address of register and store the data bytes in sequence.

//  +-----+---------+---+----------+---+----------+---+------+---+------------+---+--------------+
//  |Start|Address_W|ACK|Register_H|ACK|Register_L|ACK|Data_1|ACK|..... Data_n|ACK|Stop Condition|
//  +-----+---------+---+----------+---+----------+---+------+---+------------+---+--------------+

struct TouchDevice g_TouchDevice;

enum GT911 : uint16_t
{
    LCD_TOUCH_GT911_READ_KEY_REG = 0x8093,
    LCD_TOUCH_GT911_READ_XY_REG = 0x814E,
    LCD_TOUCH_GT911_CONFIG_REG = 0x8047,
    LCD_TOUCH_GT911_PRODUCT_ID_REG = 0x8140,
    LCD_TOUCH_GT911_ENTER_SLEEP = 0x8040,
    GT911_TOUCH_MAX_BUTTONS = 4
};

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

// static PinNameValue TouchResetPin = TOUCH_RESET_PIN; // Shared with LCD reset on ESP32P4-WIFI6 (Waveshare)
static int TouchWidth;
static int TouchHeight;
int control_phase_bytes = 1;
int lcd_cmd_bits = 16;
int scl_speed_hz = 100000;
int disable_control_phase = 1;

GPIO_INTERRUPT touchInterruptServiceRoutine;
extern TaskHandle_t g_uiTaskHandle;

//int interruptCounter = 0;
static void local_touch_gpio_callback(void *arg)
{
    BaseType_t taskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(g_uiTaskHandle, &taskWoken);
    portYIELD_FROM_ISR(taskWoken);
}

bool TouchDevice::Initialize()
{
    uint8_t RegistersWidthHeight[2] = {0x81, 0x46};
    uint8_t buf[4];

    // THe default slave address is the primary, but force it by placing a zero voltage
    // on the interrupt pin of the GT911/GT9271 which is read at reset (forcing primary slave address 0x5D)
    // Set it back to an input afterwards
    GpioIO::InitializePin(TOUCH_INTERRUPT_PIN, GpioPinMode_Output, GpioBias_PullDown);

    // Reset the GT911 controller
    GpioIO::InitializePin(TOUCH_RESET_PIN, GpioPinMode_Output, GpioBias_NoBias);
    GpioIO::SetLevel(TOUCH_RESET_PIN, GpioPinLevel_Low);
    PLATFORM_DELAY(5);
    GpioIO::SetLevel(TOUCH_RESET_PIN, GpioPinLevel_High);
    PLATFORM_DELAY(50);

    // Check the device is answering to the default primary address
    bool found_0x5D = I2cIO::Probe(INTERNAL_SHARED_I2C_MASTER_BUS, LCD_TOUCH_GT911_ADDRESS, 100);
    if (!found_0x5D)
    {
        return false;
    }

    I2cIO::AddDevice(INTERNAL_SHARED_I2C_MASTER_BUS, 100000, LCD_TOUCH_GT911_ADDRESS);

    // Read back width and height
    I2cIO::WriteRead(LCD_TOUCH_GT911_ADDRESS, RegistersWidthHeight, 2, buf, 4, 10);
    TouchWidth = buf[0] | (buf[1] << 8);
    TouchHeight = buf[2] | (buf[3] << 8);

    // Set interrupt input pin to pullup during normal operation
    GpioIO::InitializePin(TOUCH_INTERRUPT_PIN, GpioPinMode_Input, GpioBias_NoBias);

    return true;
}
bool TouchDevice::Enable()
{
    GpioIO::AddInterrupt(TOUCH_INTERRUPT_PIN, GpioInterruptMode_EdgeLow, (void *)local_touch_gpio_callback);
    GpioIO::EnableInterrupt(TOUCH_INTERRUPT_PIN);
    return TRUE;
}
bool TouchDevice::Disable()
{
    GpioIO::DisableInterrupt(TOUCH_INTERRUPT_PIN);
    return true;
}
TouchPointDevice TouchDevice::GetPoint()
{
    // Many touch controllers provide explicit events like: DOWN/MOVE/UP, the GT911 does not it has a data changed interrupt
    // Maybe because of the multi-touch capability.
    // For simple touch down/up, will need to infer it.

    // GT911 Register commands
    uint8_t ClearStatusAndCoordinates[3] = {0x81, 0x4E, 0x00};
    uint8_t ReadXYCoordinates[2] = {0x81, 0x4E};

    TouchPointDevice tp = {.x = 0, .y = 0, .touchStatus = TouchStatus::NoChange};
    GT911TouchRecord touch = {.Status = 0, .TrackId = 0, .X = 0, .Y = 0, .Size = 0};
    I2cIO::WriteRead(LCD_TOUCH_GT911_ADDRESS, ReadXYCoordinates, 2, (uint8_t *)&touch, sizeof(touch), 10);

    bool NewDataAvailable = (touch.Status & 0x80) != 0;
    uint8_t TouchCount = touch.Status & 0x0F;
    if (NewDataAvailable)
    {
        // clear MSB/LSB/Status
        I2cIO::Write(LCD_TOUCH_GT911_ADDRESS, ClearStatusAndCoordinates, sizeof(ClearStatusAndCoordinates), 10);
        if (TouchCount > 0)
        {
            tp.x = touch.X;
            tp.y = TOUCH_WIDTH - touch.Y;
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


//TouchPointDevice TouchDevice::GetPoint()
//{
//    // Many touch controllers provide explicit events like:
//    // DOWN / MOVE / UP.
//    // The GT911 instead provides a "data changed" interrupt and
//    // a touch count, so we infer the events ourselves.
//
//    constexpr int TOUCH_MOVE_THRESHOLD = 5;
//
//    static bool s_touchCurrentlyDown = false;
//    static uint16_t s_lastX = 0;
//    static uint16_t s_lastY = 0;
//
//    // GT911 Register commands
//    uint8_t ClearStatusAndCoordinates[3] = {0x81, 0x4E, 0x00};
//    uint8_t ReadXYCoordinates[2] = {0x81, 0x4E};
//
//    TouchPointDevice tp = {.x = 0, .y = 0, .touchStatus = TouchStatus::NoChange};
//
//    GT911TouchRecord touch = {.Status = 0, .TrackId = 0, .X = 0, .Y = 0, .Size = 0};
//
//    I2cIO::WriteRead(
//        LCD_TOUCH_GT911_ADDRESS,
//        ReadXYCoordinates,
//        sizeof(ReadXYCoordinates),
//        (uint8_t *)&touch,
//        sizeof(touch),
//        10);
//
//    bool NewDataAvailable = (touch.Status & 0x80) != 0;
//    uint8_t TouchCount = touch.Status & 0x0F;
//
//    if (NewDataAvailable)
//    {
//        // Tell the GT911 that we've consumed the data
//        I2cIO::Write(LCD_TOUCH_GT911_ADDRESS, ClearStatusAndCoordinates, sizeof(ClearStatusAndCoordinates), 10);
//
//        if (TouchCount > 0)
//        {
//            tp.x = touch.X;
//            tp.y = TOUCH_WIDTH - touch.Y;
//
//            if (!s_touchCurrentlyDown)
//            {
//                // First touch detected
//                tp.touchStatus = TouchStatus::TouchDown;
//                s_touchCurrentlyDown = true;
//            }
//            else
//            {
//                int dx = abs((int)tp.x - (int)s_lastX);
//                int dy = abs((int)tp.y - (int)s_lastY);
//
//                if ((dx >= TOUCH_MOVE_THRESHOLD) || (dy >= TOUCH_MOVE_THRESHOLD))
//                {
//                    tp.touchStatus = TouchStatus::TouchMove;
//                }
//                else
//                {
//                    tp.touchStatus = TouchStatus::NoChange;
//                }
//            }
//
//            s_lastX = tp.x;
//            s_lastY = tp.y;
//        }
//        else
//        {
//            // Touch released
//            tp.touchStatus = TouchStatus::TouchUp;
//
//            s_touchCurrentlyDown = false;
//            s_lastX = 0;
//            s_lastY = 0;
//        }
//    }
//    else
//    {
//        tp.touchStatus = TouchStatus::NoChange;
//    }
//
//    return tp;
//}
