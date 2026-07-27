//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include "Device.IO.h"

#pragma region System.Device.Gpio
GpioIO::GpioIO()
{
}
bool GpioIO::InitializePin(PinNameValue pinNameValue)
{
    // Sets pin as input, sets output to low and function as SIO
    int pinNumber = pinNameValue;
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << pinNumber),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
        .hys_ctrl_mode = GPIO_HYS_SOFT_DISABLE};
    gpio_config(&io_conf);
    return true;
}
bool GpioIO::Dispose(PinNameValue pinNameValue)
{
    GpioIO::InterruptRemove(pinNameValue);
    GpioIO::SetLowPower(pinNameValue);
    return true;
}
bool GpioIO::Read(PinNameValue pinNameValue)
{
    gpio_num_t pinNumber = (gpio_num_t)pinNameValue;
    return gpio_get_level(pinNumber);
}
bool GpioIO::Write(PinNameValue pinNameValue, bool pinState)
{
    gpio_num_t pinNumber = (gpio_num_t)pinNameValue;
    gpio_set_level(pinNumber, pinState);
    return true;
}
bool GpioIO::Toggle(PinNameValue pinNameValue)
{
    Write(pinNameValue, !Read(pinNameValue));
    return true;
}
bool GpioIO::SetLowPower(PinNameValue pinNameValue)
{
    gpio_num_t pinNumber = (gpio_num_t)pinNameValue;
    gpio_reset_pin(pinNumber);
    gpio_set_direction(pinNumber, GPIO_MODE_DISABLE);
    return true;
}
bool GpioIO::SetMode(PinNameValue pinNameValue, PinMode pinMode)
{
    bool status = false;
    gpio_num_t pinNumber = (gpio_num_t)pinNameValue;
    gpio_config_t io_conf = {.pin_bit_mask = (uint64_t)pinNumber, .intr_type = GPIO_INTR_DISABLE};
    switch (pinMode)
    {
        case PinMode_Input:
            io_conf.mode = GPIO_MODE_INPUT;
            io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            status = true;
            break;
        case PinMode_InputPullDown:
            io_conf.mode = GPIO_MODE_INPUT;
            io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
            status = true;
            break;
        case PinMode_InputPullUp:
            io_conf.mode = GPIO_MODE_INPUT;
            io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            status = true;
            break;
        case PinMode_Output:
            io_conf.mode = GPIO_MODE_OUTPUT;
            io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            status = true;
            break;
        case PinMode_OutputOpenDrain:
            io_conf.mode = GPIO_MODE_OUTPUT_OD;
            io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            status = true;
            break;
        case PinMode_OutputOpenDrainPullUp:
            io_conf.mode = GPIO_MODE_OUTPUT_OD;
            io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            status = true;
            break;
        case PinMode_OutputOpenSource:
            status = false;
            break;
        case PinMode_OutputOpenSourcePullDown:
            status = false;
            break;
        default:
            status = false;
            break;
    }

    gpio_config(&io_conf);
    return status;
}
bool GpioIO::InterruptEnable(PinNameValue pinNameValue, GPIO_INT_EDGE events, void *interruptRoutine)
{
    bool enable = true;
    gpio_num_t pinNumber = (gpio_num_t)pinNameValue;

    gpio_int_type_t edge_events;
    switch (events)
    {
        case GPIO_INT_NONE:
            edge_events = GPIO_INTR_DISABLE;
            break;
        case GPIO_INT_EDGE_LOW:
            edge_events = GPIO_INTR_NEGEDGE;
        case GPIO_INT_LEVEL_LOW:
            edge_events = GPIO_INTR_LOW_LEVEL;
            break;
        case GPIO_INT_EDGE_HIGH:
            edge_events = GPIO_INTR_POSEDGE;
            break;
        case GPIO_INT_LEVEL_HIGH:
            edge_events = GPIO_INTR_HIGH_LEVEL;
            break;
        case GPIO_INT_EDGE_BOTH:
            edge_events = GPIO_INTR_ANYEDGE;
            break;
    }
    gpio_set_intr_type(pinNumber, edge_events);
    if (interruptRoutine != NULL)
    {
        gpio_isr_handler_add(pinNumber, (gpio_isr_t)interruptRoutine, NULL);
    }
    return enable;
}
bool GpioIO::InterruptDisable(PinNameValue pinNameValue)
{
    gpio_num_t pinNumber = (gpio_num_t)pinNameValue;
    gpio_set_intr_type(pinNumber, GPIO_INTR_DISABLE);
    return false;
}
bool GpioIO::InterruptRemove(PinNameValue pinNameValue)
{
    gpio_num_t pinNumber = (gpio_num_t)pinNameValue;

    GpioIO::InterruptDisable(pinNameValue);
    gpio_isr_handler_remove(pinNumber);
    return true;
}
#pragma endregion
