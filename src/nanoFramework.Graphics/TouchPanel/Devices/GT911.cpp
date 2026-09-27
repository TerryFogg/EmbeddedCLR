//
// Copyright (c) .NET Foundation and Contributors
// Portions Copyright (c) Microsoft Corporation.  All rights reserved.

#include "TouchDevice.h"
#include "TouchInterface.h"
#include "CoreIO.h"
#include "freertos/task.h"
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
#define ESP_GT911_TOUCH_MAX_BUTTONS (4)

//static PinNameValue TouchResetPin = TOUCH_RESET_PIN; // Shared with LCD reset on ESP32P4-WIFI6 (Waveshare)
static PinNameValue TouchInterruptPin = TOUCH_INTERRUPT_PIN;
static int TouchWidth;
static int TouchHeight;
// static bool TouchMirrorX = false;
// static bool TouchMirrorY = false;
// static bool TouchSwapXY = false;
// static bool TouchRotation = false;
// static int TouchTearAvoidMode = 4; // Triple buffering with partial refresh
int control_phase_bytes = 1;
int lcd_cmd_bits = 16;
int scl_speed_hz = 100000;
int disable_control_phase = 1;

bool TouchDevice::Initialize()
{
    uint8_t RegisterStart[2] = {0x81, 0x46};
    uint8_t buf[4];

    // Check the device is answering to the default primary address
    if (!I2cIO::Probe(INTERNAL_SHARED_I2C_MASTER_BUS, LCD_TOUCH_GT911_ADDRESS, 100))
    {
        return false;
    }
    I2cIO::AddDevice(INTERNAL_SHARED_I2C_MASTER_BUS, 40000, LCD_TOUCH_GT911_ADDRESS);
    I2cIO::WriteRead(INTERNAL_SHARED_I2C_MASTER_BUS, LCD_TOUCH_GT911_ADDRESS, RegisterStart, 2, buf, 9);
    TouchHeight = ((uint16_t)buf[0] << 8) | (uint16_t)buf[1];
    TouchWidth = ((uint16_t)buf[2] << 8) | (uint16_t)buf[3];

    // GT911 INT ==> High->idle INT ==> Low->touch data available
    // The touch controller is run in interrupt mode
    GpioIO::InitializePin(TouchInterruptPin, GpioPinMode::MODE_INPUT, GpioBias::NOBIAS);
    return true;
}
bool TouchDevice::Enable(GPIO_INTERRUPT touchIsrProc)
{
    GpioIO::EnableInterrupt(TouchInterruptPin, GPIO_INTERRUPT_EDGE::GPIO_INTERRUPT_EDGE_LOW, touchIsrProc);
    return TRUE;
}
bool TouchDevice::Disable()
{
    GpioIO::DisableInterrupt(TouchInterruptPin);
    return true;
}
TouchPointDevice TouchDevice::GetPoint()
{
    TouchPointDevice tp = {.x = 0, .y = 0, .touch_down = false};
    uint8_t RegisterStart[2] = {0x81, 0x4E};
    uint8_t buf[9];
    uint8_t status = buf[0] = {};

    I2cIO::WriteRead(INTERNAL_SHARED_I2C_MASTER_BUS, LCD_TOUCH_GT911_ADDRESS, RegisterStart, 2, buf, 9);
    uint8_t touch_count = status & 0x0F;
    tp.x = ((uint16_t)buf[3] << 8) | (uint16_t)buf[2];
    tp.y = ((uint16_t)buf[5] << 8) | (uint16_t)buf[4];
    tp.touch_down = (touch_count > 0);

    // clear MSB/LSB/Status
    uint8_t clear_status[3] = {0x81, 0x4E, 0x00};
    I2cIO::Write(LCD_TOUCH_GT911_ADDRESS, clear_status, 3);
    return tp;
}
