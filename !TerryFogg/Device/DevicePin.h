#pragma once
//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include <nanoCLR_Types.h>

#include "PinNames.h"


// Note: prefix 'dp' added as name conflict with other code
//       where names are not fully defined
enum DevicePinFunction
{
    dpNONE,
    dpLOW_POWER,
    dpGPIO,
    dpADC,
    dpDAC,
    dpSPI,
    dpPWM,
    dpI2C,
    dpUSART,
    dpCAN,
    dpCOUNTER,
    dpTIMER,
    dpI2S,
    dpWAKEUP,
    dpSD,
    dpLCD,
    dpLED,
    dpBUTTON,
    dpMEMORY_INTERFACE,
    dpWIFI_INTERFACE,
    dpUSB,
    dpCAMERA
};

typedef struct GpioCallbackParameter
{
    bool callBack;
    uint8_t edgeTrigger;
    void *param;
    void* debounceTimer;
    bool expectedState;
    uint32_t debounceMs;
    bool waitingForDebounceToExpire;
} GpioCallbackParameter;

typedef struct DeviceGpioPin
{
    PinNameValues pinNameValue;
    bool Reserved;
    GpioCallbackParameter *GpioCallbackParameters;
    PinMode Mode;
    DevicePinFunction Function;
    void *FunctionCode;
} DeviceGpioPin;

typedef enum I2c_Bus_Type
{
    Master,
    Slave
} I2c_Bus_Type;

typedef struct AdcPin
{
    void *controllerId;
    int controllerNumber;
    int channelNumber;
} AdcPin;
typedef struct DacPin
{
    void *controllerId;
    int controllerNumber;
    int channelNumber;
} DacPin;
typedef struct I2cPin
{
    void *controllerId;
    int controllerNumber;
    int channelNumber;
} I2cPin;
typedef struct PwmPin
{
    void *controllerId;
    int controllerNumber;
    int channelNumber;
} PwmPin;
typedef struct SpiPin
{
    void *controllerId;
    int controllerNumber;
    int channelNumber;
} SpiPin;
typedef struct SerialPin
{
    void *controllerId;
    int controllerNumber;
    int channelNumber;
} SerialPin;
typedef struct SerialChannel
{
    void *controllerId;
    char newLine;
} SerialChannel;

#ifdef __cplusplus
extern "C"
{
#endif

    class DevicePin
    {
      public:
        static void CreatePinList(DeviceGpioPin *GpioPins, int numberOfPins);
        static int NumberOfPins();
        static bool ReservePin(PinNameValue pinNameValue);
        static bool IsPinReserved(PinNameValue pinNameValue);
        static bool IsValidPin(PinNameValue pinNameValue);
        static void ReleasePin(PinNameValue pinNameValue);

        static bool AddPinCallbackParameter(PinNameValue pinNameValue, void *newParameters);
        static bool RemovePinCallbackParameters(PinNameValue pinNameValue);

        static bool RegisterPinMode(PinNameValue pinNameValue, PinMode pinMode);
        static bool RegisterPinFunction(PinNameValue pinNameValue, DevicePinFunction function);

        static int FindPin(PinNameValue pinNameValue);
        static DeviceGpioPin GetPin(PinNameValue pinNameValue);
        static bool IsValidOutputDriveMode(PinMode driveMode);
        static bool IsValidInputDriveMode(PinMode driveMode);
        static bool IsValidOutputPin(PinNameValue pinNameValue);
        static bool IsValidInputPin(PinNameValue pinNameValue);

    };

#ifdef __cplusplus
}
#endif
