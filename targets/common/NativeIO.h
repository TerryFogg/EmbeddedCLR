#pragma once
//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include <stddef.h>
#include <stdint.h>
#include <board.h>
#include <hal/ledc_types.h>

#include "nanoFramework_NativeIO.h"

enum PinNameValue
{
    GPIO0 = 0,
    GPIO1 = 1,
    GPIO2 = 2,
    GPIO3 = 3,
    GPIO4 = 4,
    GPIO5 = 5,
    GPIO6 = 6,
    GPIO7 = 7,
    GPIO8 = 8,
    GPIO9 = 9,
    GPIO10 = 10,
    GPIO11 = 11,
    GPIO12 = 12,
    GPIO13 = 13,
    GPIO14 = 14,
    GPIO15 = 15,
    GPIO16 = 16,
    GPIO17 = 17,
    GPIO18 = 18,
    GPIO19 = 19,
    GPIO20 = 20,
    GPIO21 = 21,
    GPIO22 = 22,
    GPIO23 = 23,
    GPIO24 = 24,
    GPIO25 = 25,
    GPIO26 = 26,
    GPIO27 = 27,
    GPIO28 = 28,
    GPIO29 = 29,
    GPIO30 = 30,
    GPIO31 = 31,
    GPIO32 = 32,
    GPIO33 = 33,
    GPIO34 = 34,
    GPIO35 = 35,
    GPIO36 = 36,
    GPIO37 = 37,
    GPIO38 = 38,
    GPIO39 = 39,
    GPIO40 = 40,
    GPIO41 = 41,
    GPIO42 = 42,
    GPIO43 = 43,
    GPIO44 = 44,
    GPIO45 = 45,
    GPIO46 = 46,
    GPIO47 = 47,
    GPIO48 = 48,
    GPIO49 = 49,
    GPIO50 = 50,
    GPIO51 = 51,
    GPIO52 = 52,
    GPIO53 = 53,
    GPIO54 = 54
};
// enum GpioPinLevel
//{
//     LOW,
//     HIGH
// };
// enum GpioPinMode
//{
//     NONE,
//     MODE_INPUT,
//     MODE_OUTPUT
// };
// enum GpioBias
//{
//     NOBIAS,
//     PullUp,
//     PullDown,
//     OpenDrain
// };
// enum GPIO_INTERRUPT_EDGE
//{
//     GPIO_INTERRUPT_NONE = 0,
//     GPIO_INTERRUPT_EDGE_LOW = 1,
//     GPIO_INTERRUPT_EDGE_HIGH = 2,
//     GPIO_INTERRUPT_EDGE_BOTH = 3,
// };

class GpioIO
{
  private:
  public:
    static void Initialize();
    static bool InitializePin(PinNameValue pin, GpioPinMode mode, GpioBias Bias);
    static GpioPinLevel ReadLevel(PinNameValue pinNumber);
    static bool SetDirection(PinNameValue pinNameValue, GpioPinMode pinMode);
    static bool SetLevel(PinNameValue pinNumber, GpioPinLevel pinState);
    static bool EnableInterrupt(
        PinNameValue pinNumber,
        GPIO_INTERRUPT_EDGE events,
        void *alternateInterruptHandler = nullptr);
    static bool DisableInterrupt(PinNameValue pinNumber);
};
class AdcIO
{
  private:
  public:
    static bool Initialize();
    static bool AddChannel(int channelNumber);
    static bool Read(int channelNumber, int *data);
};
class DacIO
{
  private:
  public:
    static bool Initialize();
    static bool Write(PinNameValue PinNumber, int value);
};
class I2cIO
{
  private:
  public:
    static bool Initialize(int i2c_bus, int pinSDA, int pinSCL);
    static bool AddDevice(int I2C_deviceId, int I2C_speed, unsigned short slaveAddress);
    static bool Probe(int i2c_bus, int slaveAddress, int timeout);
    static bool Write(int slaveAddress, unsigned char *writeBuffer, int writeSize);
    static bool Read(int slaveAddress, unsigned char *readBuffer, int maxReadSize);
    static bool WriteRead(
        int slaveAddress,
        unsigned char *writeBuffer,
        int writeSize,
        unsigned char *readBuffer,
        int readSize);
};
class PwmIO
{
  private:
    static uint8_t s_usedChannels;

  public:
    static bool Initialize(int Frequency = 1000);
    static int AllocatePwmChannel(PinNameValue pinNumber);
    static int FindPwmChannel(PinNameValue pinNumber);
    static bool ConfigurePin(PinNameValue pinNumber, int Frequency, int DutyCycle);
    static bool SetDutyCycle(PinNameValue pinNumber, int DutyCycle);
    static bool Start(PinNameValue pinNumber, int timerId);
    static bool SetFrequency(PinNameValue pinNumber, int frequency);
    static bool Stop(PinNameValue pinNumber, bool OutputHigh);
    static bool Start(PinNameValue pinNumber);
};
class SerialIO
{
  private:
  public:
    static bool Initialize(
        int usartDeviceNumber,
        enum PinNameValue TX,
        enum PinNameValue RX,
        int baud,
        int databits,
        int parity,
        int stopbits,
        int flowcontrol);
    static int Write(int usartDeviceNumber, unsigned char *data, int dataLength);
    static int Read(int usartDeviceNumber, unsigned char *data, int maxdataLength, int timeoutInMilliseconds);
};
class SpiIO
{
  private:
  public:
    static bool Initialize(int spiBusNumber, PinNameValue pinMosi, PinNameValue pinMiso, PinNameValue pinSCLK);
    static bool AttachDevice(int spiBusNumber, PinNameValue pinChipSelect);
    static bool Write(int spiInstance, unsigned char *writeData, int writeDataSize);
    static int Read(int spiInstance, unsigned char *readData, int maxReadData);
};
