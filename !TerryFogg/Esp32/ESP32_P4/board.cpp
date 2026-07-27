//
// Copyright (c) .NET Foundation and Contributors
// Portions Copyright (c) 2021 STMicroelectronics.  All rights reserved.
// See LICENSE file in the project root for full license information.
//
#include "PinNames.h"
#include "Device.IO.h"
#include "DevicePin.h"

#define ARRAY_LEN(x) (sizeof(x) / sizeof((x)[0]))

#define NUMBER_I2C_BUSES 2
void *i2c0;
void *i2c1;
void *spi0;
void *spi1;
void *uart0;
void *uart1;
void *device_handle;

static DeviceGpioPin mcuPins[28];
static ADC_Properties mcuADC[] = {{false, 0, GPIO0}, {false, 1, GPIO0}, {false, 2, GPIO0}};
static I2C_Properties mcuI2C[] = {
    {false, i2c0, device_handle, GPIO0, GPIO0},
    {false, i2c1, device_handle, GPIO0, GPIO0}};
static PWM_Properties mcuPWM[] = {
    {false, 0, GPIO0, GPIO0},
    {false, 1, GPIO0, GPIO0},
    {false, 2, GPIO0, GPIO0},
    {false, 3, GPIO0, GPIO0},
    {false, 4, GPIO0, GPIO0}};
static SPI_Properties mcuSPI[] = {{false, spi0, GPIO0, GPIO0, GPIO0, GPIO0}, {false, spi1, GPIO0, GPIO0}};
static USART_Properties mcuUSART[] = {{false, uart0, GPIO0, GPIO0}, {false, uart1, GPIO0, GPIO0}};

void SetupPinList()
{
    mcuPins[0] = {GPIO1, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[1] = {GPIO2, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[2] = {GPIO3, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[3] = {GPIO4, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[4] = {GPIO5, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[5] = {GPIO6, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[6] = {GPIO7, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[7] = {GPIO8, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[8] = {GPIO9, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[9] = {GPIO10, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[10] = {GPIO11, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[11] = {GPIO12, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[12] = {GPIO13, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[13] = {GPIO14, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[14] = {GPIO15, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[15] = {GPIO16, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[16] = {GPIO17, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[17] = {GPIO18, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[18] = {GPIO19, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[19] = {GPIO20, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[20] = {GPIO21, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[21] = {GPIO22, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[22] = {GPIO23, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[23] = {GPIO24, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[24] = {GPIO25, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[25] = {GPIO26, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[26] = {GPIO27, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[27] = {GPIO28, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[28] = {GPIO29, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[29] = {GPIO30, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[30] = {GPIO31, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[31] = {GPIO32, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[32] = {GPIO33, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[33] = {GPIO34, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[34] = {GPIO35, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[35] = {GPIO36, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[36] = {GPIO37, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[37] = {GPIO38, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[38] = {GPIO39, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[39] = {GPIO40, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[40] = {GPIO41, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[41] = {GPIO42, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[42] = {GPIO43, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[43] = {GPIO44, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[44] = {GPIO45, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[45] = {GPIO46, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[46] = {GPIO47, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[47] = {GPIO48, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[48] = {GPIO49, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[49] = {GPIO50, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[50] = {GPIO51, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[51] = {GPIO52, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[52] = {GPIO53, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};
    mcuPins[53] = {GPIO54, false, NULL, PinMode::PinMode_Input, DevicePinFunction::dpLOW_POWER};

    DevicePin::CreatePinList(&mcuPins[0], ARRAY_LEN(mcuPins));

    AdcIO::SetupAdcList(mcuADC);
    I2cIO::SetupI2CList(mcuI2C, NUMBER_I2C_BUSES);
    PwmIO::SetupPwmList(mcuPWM);
    SpiIO::SetupSpiList(mcuSPI);
    SerialIO::SetupUsartList(mcuUSART);
};
void Initialize_Board()
{
#ifdef PSRAM
    // Enable XIP ( execute in place)
    // Forums](https://forums.raspberrypi.com/viewtopic.php?t=375109)
    gpio_set_function(47, GPIO_FUNC_XIP_CS1); // Set GPIO47 as CS for PSRAM
    xip_ctrl_hw->ctrl |= XIP_CTRL_WRITABLE_M1_BITS;
#endif

}
uint64_t ReadMicrosecondCounter()
{
    // The RP2040/RP2350 has a dedicated hardware timer peripheral that starts counting from zero at boot.
    // This timer is clocked by the reference clock(clk_ref), typically running at 1 MHz giving 1 µs resolution
    return 1;
    //return time_us_64();
}
