#pragma once
//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include <stddef.h>
#include <stdint.h>
#include <board.h>


class WiFi
{
  private:
  public:
    static bool Initialize();
    static bool IsAvailable();
    static bool Connect(int channel, char *szSSID, char *szPassword);
    static bool Disconnect();
    static bool IsConnected();
    static bool Enable();
    static bool Disable();
    static bool StartScan();
    static bool GetScanResults();
    static bool GetRSSI();
};
//class SocketComms::
//{
//  private:
//  public:
//    static bool Create();
//    static bool Connect();
//    static bool Send();
//    static bool Receive();
//    static bool Close(int socket);
//};
//class DnsClient::
//{
//  private:
//  public:
//    static bool Initialize();
//    static bool ResolveHost();
//};
//class BlueTooth::
//{
//  private:
//  public:
//    static bool Enable();
//    static bool Disable();
//    static bool GetState();
//    static bool StartScan();
//    static bool StopScan();
//    static bool DeviceCount();
//    static bool GetDeviceAddress(int index);
//    static bool GetDeviceName(int index);
//    static bool Connect();
//    static bool Disconnect();
//    static bool ReadCharacteristic();
//    static bool WriteCharacteristic();
//};
