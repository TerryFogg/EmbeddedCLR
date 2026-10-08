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

static void local_gpio_callback(void *arg)
{
    touchInterruptServiceRoutine(NULL);
}

bool TouchDevice::Initialize()
{
    uint8_t RegisterStart[2] = {0x81, 0x46};
    uint8_t ClearStatusAndCoordinates[3] = {0x81, 0x4E, 0x00};

    uint8_t buf[4];

    // Reset the controller
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

    I2cIO::AddDevice(INTERNAL_SHARED_I2C_MASTER_BUS, 40000, LCD_TOUCH_GT911_ADDRESS);
    I2cIO::WriteRead(LCD_TOUCH_GT911_ADDRESS, RegisterStart, 2, buf, 4);
    TouchWidth = buf[0] | (buf[1] << 8);
    TouchHeight = buf[2] | (buf[3] << 8);

    I2cIO::Write(LCD_TOUCH_GT911_ADDRESS, ClearStatusAndCoordinates, sizeof(ClearStatusAndCoordinates));
    GpioIO::InitializePin(TOUCH_INTERRUPT_PIN, GpioPinMode_Input, GpioBias_PullUp);
    return true;
}
bool TouchDevice::Enable(GPIO_INTERRUPT touchIsrProc)
{
    touchInterruptServiceRoutine = touchIsrProc;
    GpioIO::AddInterrupt(TOUCH_INTERRUPT_PIN, GpioInterruptMode_EdgeLow , (void *)local_gpio_callback);
    return TRUE;
}
bool TouchDevice::Disable()
{
    GpioIO::DisableInterrupt(TOUCH_INTERRUPT_PIN);
    return true;
}
TouchPointDevice TouchDevice::GetPoint()
{
    // Many touch controllers provide explicit events like: DOWN/MOVE/UP, btu the GT911 does not!
    // Maybe because of the multi-touch capability.
    // For simple touch down/up, will need to infer it.

    uint8_t ClearStatusAndCoordinates[3] = {0x81, 0x4E, 0x00};
    TouchPointDevice tp = {.x = 0, .y = 0, .touchStatus = TouchStatus::NoChange};
    uint8_t ReadXYCoordinates[2] = {0x81, 0x4E};
    GT911TouchRecord touch = {.Status = 0, .TrackId = 0, .X = 0, .Y = 0, .Size = 0};

    I2cIO::WriteRead(LCD_TOUCH_GT911_ADDRESS, ReadXYCoordinates, 2, (uint8_t *)&touch, sizeof(touch));

    bool NewDataAvailable = (touch.Status & 0x80) != 0;
    uint8_t TouchCount = touch.Status & 0x0F;
    if (NewDataAvailable)
    {
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
        // clear MSB/LSB/Status
        I2cIO::Write(LCD_TOUCH_GT911_ADDRESS, ClearStatusAndCoordinates, sizeof(ClearStatusAndCoordinates));
    }
    else
    {
        tp.touchStatus = TouchStatus::NoChange;
    }

    return tp;
}
