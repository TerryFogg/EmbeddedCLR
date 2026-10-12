#pragma once
//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include <stddef.h>
#include <stdint.h>
#include "board.h"
#include "NativeCalls.h"

class WiFi
{
  private:
  public:
    struct ScanRecord
    {
        uint8_t bssid[6];
        uint8_t ssid[33];
        uint8_t rssi;
        uint8_t authMode;
        uint8_t cypherType;
    };
    static bool Initialize();
    static bool IsAvailable();
    static bool Connect(int channel, char *szSSID, char *szPassword);
    static bool Disconnect();
    static bool IsConnected();
    static bool Enable();
    static bool Disable();
    static int ScanToCache();
    static bool GetScanResults(uint8_t *packedScanReport, int apCount);
    static bool GetRSSI();
};

#pragma once

#include <stdint.h>

class NativeSockets
{
  private:
    static void SetReceiveTimeout(int32_t socket, int32_t timeoutMs);
    static void SetSendTimeout(int32_t socket, int32_t timeoutMs);
    static bool IsValidSocket(int32_t socket);
  public:
    static bool ResolveHost(const char *host, uint32_t *address);
    static int32_t Create(SocketProtocol protocol);
    static bool Close(int32_t socket);
    static bool Bind(int32_t socket, uint16_t port);
    static bool Connect(int32_t socket, const char *host, uint16_t port, int32_t timeoutMs);
    static bool Listen(int32_t socket, uint16_t port, int32_t maxConnections);
    static int32_t Accept(int32_t socket, int32_t timeoutMs);
    static bool IsConnected(int32_t socket);
    static int32_t Send(int32_t socket, const uint8_t *data, int32_t offset, int32_t length);
    static int32_t Receive(int32_t socket, uint8_t *buffer, int32_t offset, int32_t length, int32_t timeoutMs);
    static int32_t SendTo(
        int32_t socket,
        const char *host,
        uint16_t port,
        const uint8_t *data,
        int32_t offset,
        int32_t length);

    static int32_t ReceiveFrom(
        int32_t socket,
        uint8_t *buffer,
        int32_t offset,
        int32_t length,
        int32_t timeoutMs,
        uint8_t *remoteAddress,
        uint8_t *remotePort);
};

class Bluetooth
{
  private:
  public:
    static bool Enable();
    static bool Disable();
    static int32_t GetState();
    static bool StartScan();
    static bool StopScan();
    static int32_t DeviceCount();
    static const char *GetDeviceAddress(int32_t index);
    static const char *GetDeviceName(int32_t index);
    static bool Connect(const char *address, int32_t timeoutMs);
    static bool Disconnect(const char *address);
    static int32_t ReadCharacteristic(
        const char *address,
        const char *characteristicUuid,
        uint8_t *buffer,
        int32_t bufferLength);
    static bool WriteCharacteristic(
        const char *address,
        const char *characteristicUuid,
        const uint8_t *data,
        int32_t length);
};
