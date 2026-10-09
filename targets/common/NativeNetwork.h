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
    static void Initialize();
    static bool Enable();
    static bool Disable();
    static bool Scan();
    static bool GetSSID();
    static bool GetRSSI();
    static bool Connect(string ssid, string password, int timeoutMs);
    static bool Disconnect();
    static bool GetState();
    static char *GetIPAddress();
    static int GetSignalStrength();
};
class SocketComms::
{
  private:
  public:
    static bool Create(SocketProtocol protocol);
    static bool Connect(int socket, string host, ushort port, int timeoutMs);
    static bool Send(int socket, byte[] data, int offset, int length);
    static bool Receive(int socket, byte[] buffer, int offset, int length, int timeoutMs);
    static bool Close(int socket);
};
class DnsClient::
{
  private:
  public:
    static bool Initialize();
    static bool ResolveHost(string hostName);
};
class BlueTooth::
{
  private:
  public:
    static bool Enable();
    static bool Disable();
    static bool GetState();
    static bool StartScan();
    static bool StopScan();
    static bool DeviceCount();
    static bool GetDeviceAddress(int index);
    static bool GetDeviceName(int index);
    static bool Connect(string address, int timeoutMs);
    static bool Disconnect(string address);
    static bool ReadCharacteristic(string address, string characteristicUuid, byte[] buffer);
    static bool WriteCharacteristic(string address, string characteristicUuid, byte[] data);
};
