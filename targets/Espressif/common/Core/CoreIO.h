#pragma once
//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include <stddef.h>
#include <stdint.h>

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
enum PinMode
{
    MODE_INPUT,
    MODE_OUTPUT,
};
enum GpioBias
{
    None,
    PullUp,
    PullDown
};
enum GPIO_INTERRUPT_EDGE
{
    GPIO_INTERRUPT_NONE = 0,
    GPIO_INTERRUPT_EDGE_LOW = 1,
    GPIO_INTERRUPT_EDGE_HIGH = 2,
    GPIO_INTERRUPT_EDGE_BOTH = 3,
};

struct SerialIOPort
{
    int usartDeviceNumber;
    enum PinNameValue pinTX;
    enum PinNameValue pinRX;
    int baudrate;
    int dataBits;
    int parity;
    int stopBits;
    int flowControl;
};

struct SpiBus
{
    int spi_bus_number;
    enum PinNameValue pinMosi;
    enum PinNameValue pinMiso;
    enum PinNameValue pinChipSelect;
};

class GpioIO
{
  private:
  public:
    static bool InitializePin(PinNameValue pin, PinMode mode, GpioBias Bias);
    static bool Read(PinNameValue pinNumber);
    static bool Write(PinNameValue pinNumber, bool pinState);
    static bool InterruptAdd(PinNameValue pinNumber, GPIO_INTERRUPT_EDGE events, void *interruptRoutine = NULL);
    static bool InterruptEnable(PinNameValue pinNumber);
    static bool InterruptDisable(PinNameValue pinNumber);
    static bool InterruptRemove(PinNameValue pinNumber);
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
    static int AddSlave(int I2C_deviceId, int I2C_speed, int slaveAddress);
    static bool Write(int I2C_deviceId, int slaveAddress, unsigned char *writeBuffer, int writeSize);
    static bool Read(int I2C_deviceId, int slaveAddress, unsigned char *readBuffer, int maxReadSize);
    static bool WriteRead(
        int I2C_deviceId,
        int slaveAddress,
        unsigned char *writeBuffer,
        int writeSize,
        unsigned char *readBuffer,
        int readSize);
};
class PwmIO
{
  private:
  public:
    static bool Initialize(int PwmChannel, PinNameValue pinNumber, int Frequency);
    static bool SetDutyCycle(int PwmChannel, float percent);
    static bool Start(PinNameValue pinNumber, int timerId);
    static bool SetFrequency(int PwmChannel, int desiredFrequency);
    static bool Stop(int PwmChannel, PinNameValue pinNumber, bool OutputHigh);
    static bool Start(int PwmChannel, PinNameValue pinNumber);
};
class SerialIO
{
  private:
  public:
    static bool Initialize(SerialIOPort serialSetup);
    static int Write(int usartDeviceNumber, unsigned char *data, int dataLength);
    static int Read(int usartDeviceNumber, unsigned char *data, int maxdataLength, int timeoutInMilliseconds);
};
class SpiIO
{
  private:
  public:
    static int Initialize(SpiBus spiSetup);
    static bool Write(int spiInstance, unsigned char *writeData, int writeDataSize);
    static int Read(int spiInstance, unsigned char *readData, int maxReadData);
};
