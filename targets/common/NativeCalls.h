//-----------------------------------------------------------------------------
//
//                   ** WARNING! **
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------

#ifndef NATIVECALLS_H
#define NATIVECALLS_H

#include <nanoCLR_Interop.h>
#include <nanoCLR_Runtime.h>
#include <nanoPackStruct.h>
#include <corlib_native.h>

typedef enum __nfpack BluetoothState
{
    BluetoothState_Disabled = 0,
    BluetoothState_Enabled = 1,
    BluetoothState_Scanning = 2,
    BluetoothState_Connected = 3,
} BluetoothState;

typedef enum __nfpack BusSpeed
{
    BusSpeed_Standard = 0,
    BusSpeed_Fast = 1,
    BusSpeed_FastPlus = 2,
} BusSpeed;

typedef enum __nfpack GpioBias
{
    GpioBias_NoBias = 0,
    GpioBias_PullUp = 1,
    GpioBias_PullDown = 2,
} GpioBias;

typedef enum __nfpack GpioInterruptMode
{
    GpioInterruptMode_EdgeLow = 0,
    GpioInterruptMode_EdgeHigh = 1,
    GpioInterruptMode_EdgeBoth = 2,
} GpioInterruptMode;

typedef enum __nfpack GpioPinLevel
{
    GpioPinLevel_Low = 0,
    GpioPinLevel_High = 1,
} GpioPinLevel;

typedef enum __nfpack GpioPinMode
{
    GpioPinMode_None = 0,
    GpioPinMode_Input = 1,
    GpioPinMode_Output = 2,
    GpioPinMode_OutputOpenDrain = 3,
} GpioPinMode;

typedef enum __nfpack NetworkEventType
{
    NetworkEventType_SocketDataAvailable = 1,
    NetworkEventType_SocketDisconnected = 2,
    NetworkEventType_SocketError = 3,
    NetworkEventType_UdpDatagramReceived = 4,
    NetworkEventType_UdpError = 5,
} NetworkEventType;

typedef enum __nfpack NetworkState
{
    NetworkState_Disabled = 0,
    NetworkState_Disconnected = 1,
    NetworkState_Connecting = 2,
    NetworkState_Connected = 3,
} NetworkState;

typedef enum __nfpack PinCapabilities
{
    PinCapabilities_DigitalInput = 1,
    PinCapabilities_DigitalOutput = 2,
    PinCapabilities_Interrupt = 4,
    PinCapabilities_Adc = 8,
    PinCapabilities_Dac = 16,
    PinCapabilities_Pwm = 32,
    PinCapabilities_I2C = 64,
    PinCapabilities_Spi = 128,
    PinCapabilities_I2S = 256,
    PinCapabilities_Uart = 512,
    PinCapabilities_Can = 1024,
    PinCapabilities_Usb = 2048,
    PinCapabilities_Touch = 4096,
    PinCapabilities_PullUp = 8192,
    PinCapabilities_PullDown = 16384,
    PinCapabilities_OpenDrain = 32768,
    PinCapabilities_Rtc = 65536,
    PinCapabilities_Wakeup = 131072,
    PinCapabilities_Reserved = 262144,
    PinCapabilities_McPwm = 524288,
} PinCapabilities;

typedef enum __nfpack SocketProtocol
{
    SocketProtocol_Tcp = 0,
    SocketProtocol_Udp = 1,
} SocketProtocol;

typedef enum __nfpack Wifi_WiFiEventType
{
    Wifi_WiFiEventType_WifiConnected = 1,
    Wifi_WiFiEventType_WifiDisconnected = 2,
} Wifi_WiFiEventType;

typedef enum __nfpack WifiAuthMode
{
    WifiAuthMode_Open = 0,
    WifiAuthMode_Wep = 1,
    WifiAuthMode_Wpa = 2,
    WifiAuthMode_Wpa2 = 3,
    WifiAuthMode_Wpa3 = 4,
    WifiAuthMode_Wpa2Wpa3 = 5,
} WifiAuthMode;

typedef enum __nfpack WifiCipherType
{
    WifiCipherType_None = 0,
    WifiCipherType_Wep40 = 1,
    WifiCipherType_Wep104 = 2,
    WifiCipherType_Tkip = 3,
    WifiCipherType_Ccmp = 4,
    WifiCipherType_TkipCcmp = 5,
    WifiCipherType_AesCmac128 = 6,
} WifiCipherType;

struct Library_NativeCalls_Adc
{
    NANOCLR_NATIVE_DECLARE(Read___STATIC__I4__I4);

    //--//
};

struct Library_NativeCalls_Bluetooth
{
    NANOCLR_NATIVE_DECLARE(Enable___STATIC__BOOLEAN);
    NANOCLR_NATIVE_DECLARE(Disable___STATIC__BOOLEAN);
    NANOCLR_NATIVE_DECLARE(GetState___STATIC__BluetoothState);
    NANOCLR_NATIVE_DECLARE(StartScan___STATIC__BOOLEAN);
    NANOCLR_NATIVE_DECLARE(StopScan___STATIC__BOOLEAN);
    NANOCLR_NATIVE_DECLARE(DeviceCount___STATIC__I4);
    NANOCLR_NATIVE_DECLARE(GetDeviceAddress___STATIC__STRING__I4);
    NANOCLR_NATIVE_DECLARE(GetDeviceName___STATIC__STRING__I4);
    NANOCLR_NATIVE_DECLARE(Connect___STATIC__BOOLEAN__STRING__I4);
    NANOCLR_NATIVE_DECLARE(Disconnect___STATIC__BOOLEAN__STRING);
    NANOCLR_NATIVE_DECLARE(ReadCharacteristic___STATIC__I4__STRING__STRING__SZARRAY_U1);
    NANOCLR_NATIVE_DECLARE(WriteCharacteristic___STATIC__BOOLEAN__STRING__STRING__SZARRAY_U1);

    //--//
};

struct Library_NativeCalls_Dac
{
    NANOCLR_NATIVE_DECLARE(Write___STATIC__VOID__I4__I4);

    //--//
};

struct Library_NativeCalls_Dns
{
    NANOCLR_NATIVE_DECLARE(ResolveHost___STATIC__I4__STRING);

    //--//
};

struct Library_NativeCalls_FileSystem
{
    NANOCLR_NATIVE_DECLARE(Exists___STATIC__BOOLEAN__STRING);
    NANOCLR_NATIVE_DECLARE(Delete___STATIC__BOOLEAN__STRING);
    NANOCLR_NATIVE_DECLARE(Move___STATIC__BOOLEAN__STRING__STRING);
    NANOCLR_NATIVE_DECLARE(Length___STATIC__I8__STRING);
    NANOCLR_NATIVE_DECLARE(Read___STATIC__I4__STRING__SZARRAY_U1__I4__I4);
    NANOCLR_NATIVE_DECLARE(Write___STATIC__I4__STRING__SZARRAY_U1__I4__I4__BOOLEAN);
    NANOCLR_NATIVE_DECLARE(CreateDirectory___STATIC__BOOLEAN__STRING);
    NANOCLR_NATIVE_DECLARE(DeleteDirectory___STATIC__BOOLEAN__STRING);
    NANOCLR_NATIVE_DECLARE(DirectoryExists___STATIC__BOOLEAN__STRING);
    NANOCLR_NATIVE_DECLARE(FileCount___STATIC__I4__STRING);
    NANOCLR_NATIVE_DECLARE(GetFileName___STATIC__STRING__STRING__I4);
    NANOCLR_NATIVE_DECLARE(DirectoryCount___STATIC__I4__STRING);
    NANOCLR_NATIVE_DECLARE(GetDirectoryName___STATIC__STRING__STRING__I4);

    //--//
};

struct Library_NativeCalls_Gpio
{
    NANOCLR_NATIVE_DECLARE(ConfigurePin___STATIC__VOID__I4__GpioPinMode__GpioBias);
    NANOCLR_NATIVE_DECLARE(Read___STATIC__GpioPinLevel__I4);
    NANOCLR_NATIVE_DECLARE(Write___STATIC__VOID__I4__GpioPinLevel);
    NANOCLR_NATIVE_DECLARE(AddInterrupt___STATIC__VOID__I4__GpioInterruptMode);
    NANOCLR_NATIVE_DECLARE(EnableInterrupt___STATIC__VOID__I4);
    NANOCLR_NATIVE_DECLARE(DisableInterrupt___STATIC__VOID__I4);
    NANOCLR_NATIVE_DECLARE(RemoveInterrupt___STATIC__VOID__I4);

    //--//
};

struct Library_NativeCalls_I2C
{
    NANOCLR_NATIVE_DECLARE(Open___STATIC__BOOLEAN__I4__I4__I4);
    NANOCLR_NATIVE_DECLARE(AddSlaveDevice___STATIC__BOOLEAN__I4__BusSpeed__U2);
    NANOCLR_NATIVE_DECLARE(Probe___STATIC__BOOLEAN__I4__U2__I4);
    NANOCLR_NATIVE_DECLARE(Write___STATIC__BOOLEAN__I4__U2__SZARRAY_U1__I4);
    NANOCLR_NATIVE_DECLARE(Read___STATIC__BOOLEAN__I4__U2__SZARRAY_U1__I4);
    NANOCLR_NATIVE_DECLARE(WriteRead___STATIC__BOOLEAN__I4__U2__SZARRAY_U1__I4__SZARRAY_U1__I4);

    //--//
};

struct Library_NativeCalls_Pwm
{
    NANOCLR_NATIVE_DECLARE(Initialize___STATIC__VOID__I4);
    NANOCLR_NATIVE_DECLARE(ConfigurePin___STATIC__VOID__I4__I4__I4);
    NANOCLR_NATIVE_DECLARE(SetDutyCycle___STATIC__VOID__I4__I4);
    NANOCLR_NATIVE_DECLARE(SetFrequency___STATIC__VOID__I4__I4);
    NANOCLR_NATIVE_DECLARE(Start___STATIC__VOID__I4__I4);
    NANOCLR_NATIVE_DECLARE(Stop___STATIC__VOID__I4__BOOLEAN);

    //--//
};

struct Library_NativeCalls_Socket
{
    NANOCLR_NATIVE_DECLARE(Create___STATIC__I4__SocketProtocol);
    NANOCLR_NATIVE_DECLARE(Bind___STATIC__BOOLEAN__I4__U2);
    NANOCLR_NATIVE_DECLARE(Connect___STATIC__BOOLEAN__I4__STRING__U2__I4);
    NANOCLR_NATIVE_DECLARE(Listen___STATIC__BOOLEAN__I4__U2__I4);
    NANOCLR_NATIVE_DECLARE(Accept___STATIC__I4__I4__I4);
    NANOCLR_NATIVE_DECLARE(Send___STATIC__I4__I4__SZARRAY_U1__I4__I4);
    NANOCLR_NATIVE_DECLARE(SendTo___STATIC__I4__I4__STRING__U2__SZARRAY_U1__I4__I4);
    NANOCLR_NATIVE_DECLARE(Receive___STATIC__I4__I4__SZARRAY_U1__I4__I4__I4);
    NANOCLR_NATIVE_DECLARE(ReceiveFrom___STATIC__I4__I4__SZARRAY_U1__I4__I4__I4__SZARRAY_U1__SZARRAY_U1);
    NANOCLR_NATIVE_DECLARE(IsConnected___STATIC__BOOLEAN__I4);
    NANOCLR_NATIVE_DECLARE(Close___STATIC__BOOLEAN__I4);

    //--//
};

struct Library_NativeCalls_Touch
{
    NANOCLR_NATIVE_DECLARE(Read___STATIC__I4__I4);

    //--//
};

struct Library_NativeCalls_Wifi
{
    NANOCLR_NATIVE_DECLARE(Initialize___STATIC__BOOLEAN);
    NANOCLR_NATIVE_DECLARE(IsAvailable___STATIC__BOOLEAN);
    NANOCLR_NATIVE_DECLARE(Connect___STATIC__BOOLEAN__STRING__STRING__I4);
    NANOCLR_NATIVE_DECLARE(Disconnect___STATIC__BOOLEAN);
    NANOCLR_NATIVE_DECLARE(IsConnected___STATIC__BOOLEAN);
    NANOCLR_NATIVE_DECLARE(Enable___STATIC__BOOLEAN);
    NANOCLR_NATIVE_DECLARE(Disable___STATIC__BOOLEAN);
    NANOCLR_NATIVE_DECLARE(GetScanResults___STATIC__SZARRAY_U1);
    NANOCLR_NATIVE_DECLARE(GetRSSI___STATIC__I4__I4);

    //--//
};

extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_NativeCalls;

#endif // NATIVECALLS_H
