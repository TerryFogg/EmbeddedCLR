//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include "Device.IO.h"


static ADC_Properties *mcuADC;
void AdcIO::SetupAdcList(ADC_Properties *boardAdcDefinitions)
{
    mcuADC = boardAdcDefinitions;
}
int AdcIO::MaximumValue()
{
    return 4095;
}
int AdcIO::MinimumValue()
{
    return 0;
}
int AdcIO::Resolution()
{
    // 12 bit(8.7 ENOB)
    return 12;
}
int AdcIO::ChannelCount()
{
    return 3;
}
bool AdcIO::Initialize()
{
    adc_init();
    return true;
}
bool AdcIO::Open(CLR_INT32 adc_channel_number)
{
    if (!mcuADC[adc_channel_number].ADC_Initialized)
    {
        adc_gpio_init(mcuADC[adc_channel_number].adc);
        mcuADC[adc_channel_number].ADC_Initialized = true;
    }
    return true;
}
bool AdcIO::Dispose(CLR_INT32 adc_channel_number)
{
    if (mcuADC[adc_channel_number].ADC_Initialized)
    {
        GpioIO::SetLowPower(mcuADC[adc_channel_number].adc);
    }
    return true;
}
CLR_UINT16 AdcIO::Read(CLR_INT32 adc_channel_number)
{
    adc_select_input(adc_channel_number);
    return adc_read();
}
CLR_UINT16 AdcIO::IsModeSupported(AdcChannelMode requestedMode)
{
    bool modeSupported = false;
    if (requestedMode == AdcChannelMode::AdcChannelMode_SingleEnded)
    {
        modeSupported = true;
    }
    if (requestedMode == AdcChannelMode::AdcChannelMode_Differential)
    {
        modeSupported = false;
    }
    return modeSupported;
}
