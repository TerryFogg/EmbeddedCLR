// Copyright (c) .NET Foundation and Contributors
// Portions Copyright (c) Microsoft Corporation.  All rights reserved.

#include "TouchDevice.h"
#include "TouchInterface.h"
#include "DevicePin.h"
#include "Device.IO.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"

struct TouchDevice g_TouchDevice;
//extern TouchInterface g_TouchInterface;
//static int m_TouchInterruptPin;
//static int m_TouchWidth;
//static int m_TouchHeight;
//static int m_TouchInvertX;
//static int m_TouchInvertY;

// GT911 registers
#define ESP_LCD_TOUCH_IO_I2C_GT911_ADDRESS (0x5D)
#define ESP_LCD_TOUCH_GT911_READ_KEY_REG   (0x8093)
#define ESP_LCD_TOUCH_GT911_READ_XY_REG    (0x814E)
#define ESP_LCD_TOUCH_GT911_CONFIG_REG     (0x8047)
#define ESP_LCD_TOUCH_GT911_PRODUCT_ID_REG (0x8140)
#define ESP_LCD_TOUCH_GT911_ENTER_SLEEP    (0x8040)

#define ESP_GT911_TOUCH_MAX_BUTTONS (4)

#define ESP_LCD_TOUCH_IO_I2C_GT911_CONFIG()                                                                            \
    {                                                                                                                  \
        .scl_speed_hz = 100000, .dev_addr = ESP_LCD_TOUCH_IO_I2C_GT911_ADDRESS, .control_phase_bytes = 1,              \
        .dc_bit_offset = 0, .lcd_cmd_bits = 16, .flags = {                                                             \
            .disable_control_phase = 1,                                                                                \
        }                                                                                                              \
    }

static GPIO_INTERRUPT_SERVICE_ROUTINE touchInterrupt = NULL;
void gpio_callback(uint32_t gpio, uint32_t events)
{
    // The interrupt edge (from pinState) can be used for the Touch Down(falling edge) and Touch Up(Rising edge)
    bool pinState = (events == 4) ? true : false;
    if (touchInterrupt != NULL)
    {
        touchInterrupt(gpio, pinState, NULL);
    }
}
bool TouchDevice::Initialize(int TouchInterruptPin, int TouchWidth, int TouchHeight, int TouchInvertX, int TouchInvertY)
{
    //m_TouchInterruptPin = TouchInterruptPin;
    //m_TouchWidth = TouchWidth;
    //m_TouchHeight = TouchHeight;
    //m_TouchInvertX = TouchInvertX;
    //m_TouchInvertY = TouchInvertY;

    // Time of starting to report point after resetting minimum time ?? milliseconds
    // // ??????????????????????????
    // PLATFORM_DELAY(310);
    // // ??????????????????????????

    // Check device type correct

        //CLR_UINT8 registerCommand = FT6X06_CMD::FOCALTECH_ID;
        //CLR_UINT8 *id = g_TouchInterface.Write_Read(&registerCommand, 1, 1);

        //if (*id == ID_VALUE)
        //{
        //    // Configured to interrupt on touch down and up but not each controller sampling period
        //    uint8_t set_interrupt_mode[2]{FT6X06_CMD::G_MODE, FT6X06_VALUES::G_MODE_INTERRUPT_POLLING};
        //    g_TouchInterface.Write_Read(set_interrupt_mode, 2, 0);
        //}
        //else
        //{
        //    return false;
        //}
        //DevicePin::ReservePin((PinNameValue)m_TouchInterruptPin);
        //GpioIO::InitializePin((PinNameValue)m_TouchInterruptPin);
        //DevicePin::RegisterPinMode((PinNameValue)m_TouchInterruptPin, PinMode_Input);
        //GpioIO::SetMode((PinNameValue)m_TouchInterruptPin, PinMode_Input);

        //GpioCallbackParameter *newGpioParameter =
        //    (GpioCallbackParameter *)platform_malloc(sizeof(GpioCallbackParameter));
        //memset(newGpioParameter, 0, sizeof(GpioCallbackParameter));
        //DevicePin::AddPinCallbackParameter((PinNameValue)m_TouchInterruptPin, newGpioParameter);
        //newGpioParameter->callBack = true;
        //newGpioParameter->edgeTrigger = GPIO_INT_EDGE_HIGH;

        return true;
}
bool TouchDevice::Enable(GPIO_INTERRUPT_SERVICE_ROUTINE touchIsrProc)
{
    //touchInterrupt = touchIsrProc;
    //GpioIO::InterruptEnable((PinNameValue)m_TouchInterruptPin, GPIO_INT_EDGE_BOTH, (void *)gpio_callback);
    return TRUE;
}
bool TouchDevice::Disable()
{
    //GpioIO::InterruptDisable((PinNameValue)m_TouchInterruptPin);
    return true;
}
TouchPointDevice TouchDevice::GetPoint()
{
    // The FT6x06 touch controller does its own processing, averaging etc
    //CLR_UINT8 registerCommand = FT6X06_CMD::DEV_MODE;
    //CLR_UINT8 *touchValues = g_TouchInterface.Write_Read(&registerCommand, 1, 7);

    //CLR_INT16 touchx1 = ((touchValues[3] & 0x0F) << 8) | touchValues[4];
    //CLR_INT16 touchy1 = ((touchValues[5] & 0x0F) << 8) | (touchValues[6]);

    TouchPointDevice tp;
    //if (m_TouchInvertX == true)
    //{
    //    tp.x = m_TouchWidth - touchx1;
    //}
    //else
    //{
    //    tp.x = touchx1;
    //}
    //if (m_TouchInvertY == true)
    //{
    //    tp.y = m_TouchHeight - touchy1;
    //}
    //else
    //{
    //    tp.y = touchy1;
    //}
    tp.x = 0;
    tp.y = 0;
    return tp;
}
