//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include "Device.IO.h"

static PWM_Properties *mcuPWM;
void PwmIO::SetupPwmList(PWM_Properties *boardPWMDefinitions)
{
    mcuPWM = boardPWMDefinitions;
}

bool PwmIO::Initialize(
    CLR_INT32 sliceNumber,
    CLR_INT32 timerId,
    CLR_INT32 pinNameValue,
    CLR_INT32 polarity,
    CLR_INT32 desiredFrequency,
    CLR_INT32 dutyCycle)
{
    if (!mcuPWM[sliceNumber].PWM_Initialized)
    {
        gpio_set_function(mcuPWM[sliceNumber].channelA, GPIO_FUNC_PWM);
        gpio_set_function(mcuPWM[sliceNumber].channelB, GPIO_FUNC_PWM);
    }
    if (polarity == 0)
    {
        pwm_set_output_polarity(sliceNumber, true, true);
    }
    else
    {
        pwm_set_output_polarity(sliceNumber, false, false);
    }
    CLR_INT32 slice_num = pwm_gpio_to_slice_num(mcuPWM[sliceNumber].channelA);

    // Set period of 4 cycles (0 to 3 inclusive)
    int wrapNumber = dutyCycle; // ??????????????????????
    pwm_set_wrap(slice_num, wrapNumber);
    pwm_set_enabled(slice_num, true);

    return true;
}
bool PwmIO::Dispose(CLR_INT32 sliceNumber)
{
    if (!mcuPWM[sliceNumber].PWM_Initialized)
    {
        gpio_set_function(mcuPWM[sliceNumber].channelA, GPIO_FUNC_PWM);
        gpio_set_function(mcuPWM[sliceNumber].channelB, GPIO_FUNC_PWM);
        mcuPWM[sliceNumber].PWM_Initialized = false;
    }
    // TODO: Deactive PWM, set pins to low power
    // Get each pin and set to low power function
    return true;
}
CLR_UINT32 PwmIO::SetDutyCycle(CLR_INT32 pinNameValue, CLR_INT32 desiredFrequency)
{
    // TODO: calculate duty cycle
    return 1;
}
CLR_UINT32 PwmIO::Start(CLR_INT32 pinNameValue, CLR_INT32 timerId)
{
    CLR_INT32 slice_num = timerId;
    pwm_set_enabled(slice_num, true);
    return 1;
}
CLR_UINT32 PwmIO::Stop(CLR_INT32 pinNameValue, CLR_INT32 timerId)
{
    CLR_INT32 slice_num = timerId;
    pwm_set_enabled(slice_num, false);
    return 1;
}
CLR_UINT32 PwmIO::DesiredFrequency(CLR_INT32 timerId, CLR_INT32 desiredFrequency)
{
    int wrapNumber = desiredFrequency; // ??????????????????????
    int slice_num = timerId;
    pwm_set_wrap(slice_num, wrapNumber);
    return 1;
}
CLR_UINT32 PwmIO::GetChannel(CLR_INT32 timerId, CLR_INT32 pin_number)
{
    return 1;
}
