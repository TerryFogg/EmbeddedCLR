//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include "soc/soc_caps.h"
#include "board.h"
#include "esp_attr.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_system.h"
#include "esp_types.h"
#include "esp_wifi.h"
#include "freertos/event_groups.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "NativeNetwork.h"
#include <nanoHAL_v2.h>
#include "nanoPAL.h"
#include <cstring>
#include <lwip/sockets.h>
#include <lwip/netdb.h>
#include <lwip/inet.h>

#include "nimble/ble.h"
#include "host/ble_hs.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_gap.h"
#include "host/ble_hs_adv.h"
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>

#pragma region WiFi

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

static WiFi::ScanRecord *records;

bool WiFi::Initialize()
{
    wifi_init_config_t defaultConfiguration = {
        .osi_funcs = &g_wifi_osi_funcs,
        .wpa_crypto_funcs = g_wifi_default_wpa_crypto_funcs,
        .static_rx_buf_num = 10,
        .dynamic_rx_buf_num = 32,
        .tx_buf_type = 1,
        .static_tx_buf_num = 0,
        .dynamic_tx_buf_num = 32,
        .rx_mgmt_buf_type = 0,
        .rx_mgmt_buf_num = 5,
        .cache_tx_buf_num = 0,
        .csi_enable = 0,
        .ampdu_rx_enable = 1,
        .ampdu_tx_enable = 1,
        .amsdu_tx_enable = 0,
        .nvs_enable = 1,
        .nano_enable = 0,
        .rx_ba_win = 6,
        .wifi_task_core_id = 0,
        .beacon_max_len = 752,
        .mgmt_sbuf_num = 32,
        .feature_caps = ((1 << 0) | 0 | 0 | 0 | 0 | (1 << 5) | 0 | (1 << 7)),
        .sta_disconnected_pm = true,
        .espnow_max_encrypt_num = 7,
        .tx_hetb_queue_num = 3,
        .dump_hesigb_enable = false,
        .magic = 0x1F2F3F4F};
    esp_wifi_init(&defaultConfiguration);

    return true;
}

bool WiFi::IsAvailable()
{
    wifi_mode_t mode;
    esp_err_t result = esp_wifi_get_mode(&mode);
    return (result == ESP_OK);
}

bool WiFi::Connect(int channel, char *szSSID, char *szPassword)
{
    wifi_config_t config = {
        .sta = {
            .ssid = "",
            .password = "",
            .scan_method = wifi_scan_method_t::WIFI_ALL_CHANNEL_SCAN,
            .bssid_set = false,
            .bssid = "",
            .channel = 0,
            .listen_interval = 0,
            .sort_method = wifi_sort_method_t::WIFI_CONNECT_AP_BY_SIGNAL,
            .threshold = {.rssi = -80, .authmode = WIFI_AUTH_WPA2_PSK, .rssi_5g_adjustment = 0},
            .pmf_cfg = {.capable = false, .required = true},
            .rm_enabled = 1,
            .btm_enabled = 1,
            .mbo_enabled = 1,
            .ft_enabled = 1,
            .owe_enabled = 1,
            .transition_disable = 1,
            .reserved = 0,
            .sae_pwe_h2e = wifi_sae_pwe_method_t::WPA3_SAE_PWE_BOTH,
            .sae_pk_mode = wifi_sae_pk_mode_t::WPA3_SAE_PK_MODE_AUTOMATIC,
            .failure_retry_cnt = 5,
            .he_dcm_set = 1,
            .he_dcm_max_constellation_tx = 3,
            .he_dcm_max_constellation_rx = 3,
            .he_mcs9_enabled = 0,
            .he_su_beamformee_disabled = 0,
            .he_trig_su_bmforming_feedback_disabled = 0,
            .he_trig_mu_bmforming_partial_feedback_disabled = 0,
            .he_trig_cqi_feedback_disabled = 0,
            .he_reserved = 0,
            .sae_h2e_identifier = ""}};
    hal_strcpy_s((char *)config.sta.ssid, sizeof(config.sta.ssid) - 1, szSSID);
    hal_strcpy_s((char *)config.sta.password, sizeof(config.sta.password) - 1, szPassword);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &config);
    esp_wifi_start();
    esp_wifi_connect();

    return true;
}

bool WiFi::Disconnect()
{
    esp_err_t result = esp_wifi_disconnect();
    return (result == ESP_OK);
}

bool WiFi::IsConnected()
{
    wifi_ap_record_t ap;
    return (esp_wifi_sta_get_ap_info(&ap) == ESP_OK);
}

bool WiFi::Enable()
{
    esp_err_t result = esp_wifi_start();
    return (result == ESP_OK);
}

bool WiFi::Disable()
{
    esp_err_t result = esp_wifi_stop();
    return (result == ESP_OK);
}

int WiFi::ScanToCache()
{
    uint16_t apCount = 0;
    esp_wifi_scan_start(NULL, true);
    esp_wifi_scan_get_ap_num(&apCount);
    wifi_ap_record_t *aps = nullptr;
    esp_wifi_scan_get_ap_records(&apCount, aps);
    records = (ScanRecord *)platform_malloc(sizeof(ScanRecord) * apCount);
    return apCount;
}

bool WiFi::GetScanResults(uint8_t *packedScanReport, int apCount)
{
    for (int i = 0; i < apCount; i++)
    {
        memcpy(packedScanReport, records[i].bssid, 6);
        packedScanReport += 6;
        memcpy(packedScanReport, records[i].ssid, 33);
        packedScanReport += 33;
        *packedScanReport++ = records[i].rssi;
        *packedScanReport++ = records[i].authMode;
        *packedScanReport++ = records[i].cypherType;
    }
    platform_free(records);

    return true;
}

bool WiFi::GetRSSI()
{
    wifi_ap_record_t ap;
    esp_err_t result = esp_wifi_sta_get_ap_info(&ap);
    // if (result)
    //{
    //     int rssi = ap.rssi;
    // }
    return (result == ESP_OK);
}

#pragma endregion

#pragma region Bluetooth

#define MAX_BT_DEVICES 32
struct BTDevice
{
    uint8_t bda[6];
    char addrStr[18];
    char name[64];
};
static BTDevice s_btDevices[MAX_BT_DEVICES];
static bool s_initialized = false;
static bool s_connected = false;
static bool s_scanning = false;
static bool s_enabled = false;
static int s_DeviceCount = 0;

static void bt_addr_to_str(const uint8_t *bda, char *out, size_t outLen)
{
    snprintf(out, outLen, "%02X:%02X:%02X:%02X:%02X:%02X", bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]);
}

static void ble_on_sync(void)
{
    // Called when the NimBLE host and controller are in sync. Minimal
    // implementation: nothing special to do for now.
}

static void ble_host_task(void *param)
{
    // Runs the NimBLE host thread loop. When nimble_port_freertos_init is
    // used this function is provided to the port and executed in a FreeRTOS
    // task.
    (void)param;
    nimble_port_run();
    nimble_port_freertos_deinit();
}

static int ble_gap_event_cb(struct ble_gap_event *event, void *arg)
{
    struct ble_hs_adv_fields fields;

    if (event->type == BLE_GAP_EVENT_DISC)
    {
        const struct ble_gap_disc_desc *disc = &event->disc;
        for (int i = 0; i < s_DeviceCount; i++)
        {
            if (memcmp(s_btDevices[i].bda, disc->addr.val, 6) == 0)
            {
                return 0;
            }
        }
        if (s_DeviceCount >= MAX_BT_DEVICES)
        {
            return 0;
        }
        int idx = s_DeviceCount++;
        memcpy(s_btDevices[idx].bda, disc->addr.val, 6);
        bt_addr_to_str(disc->addr.val, s_btDevices[idx].addrStr, sizeof(s_btDevices[idx].addrStr));
        memset(&fields, 0, sizeof(fields));
        int rc = ble_hs_adv_parse_fields(&fields, disc->data, disc->length_data);
        if (rc == 0 && fields.name_len > 0 && fields.name != NULL)
        {
            int copy_len = fields.name_len;
            if (copy_len >= (int)sizeof(s_btDevices[idx].name))
                copy_len = sizeof(s_btDevices[idx].name) - 1;
            memcpy(s_btDevices[idx].name, fields.name, copy_len);
            s_btDevices[idx].name[copy_len] = '\0';
        }
        else
        {
            s_btDevices[idx].name[0] = '\0';
        }
    }

    return 0;
}
bool Bluetooth::Enable()
{
    if (s_initialized)
    {
        return true;
    }
    nimble_port_init();

    /* Set the sync callback so we know when the host is ready. */
    ble_hs_cfg.sync_cb = ble_on_sync;

    /* Start the host task. */
    nimble_port_freertos_init(ble_host_task);
    s_initialized = true;
    s_DeviceCount = 0;
    return true;
}

bool Bluetooth::Disable()
{
    if (!s_initialized)
    {
        return true;
    }

    if (s_scanning)
    {
        ble_gap_disc_cancel();
        s_scanning = false;
    }

    nimble_port_stop();
    nimble_port_deinit();

    s_initialized = false;
    s_DeviceCount = 0;
    return true;
}

int32_t Bluetooth::GetState()
{
    if (!s_enabled)
    {
        return (int32_t)BluetoothState::BluetoothState_Disabled;
    }

    if (s_connected)
    {
        return (int32_t)BluetoothState::BluetoothState_Connected;
    }

    if (s_scanning)
    {
        return (int32_t)BluetoothState::BluetoothState_Scanning;
    }

    return (int32_t)BluetoothState::BluetoothState_Enabled;
}

bool Bluetooth::StartScan()
{
    struct ble_gap_disc_params disc_params;

    if (!s_initialized)
    {
        return false;
    }

    if (s_scanning)
    {
        return true;
    }
    memset(&disc_params, 0, sizeof(disc_params));
    disc_params.itvl = 0x0010;
    disc_params.window = 0x0010;
    disc_params.filter_policy = 0;
    disc_params.passive = 0;

    int rc = ble_gap_disc(BLE_OWN_ADDR_PUBLIC, BLE_HS_FOREVER, &disc_params, ble_gap_event_cb, NULL);
    if (rc != 0)
    {
        return false;
    }
    s_scanning = true;
    s_DeviceCount = 0;
    return true;
}

bool Bluetooth::StopScan()
{
    if (!s_initialized || !s_scanning)
    {
        return true;
    }
    ble_gap_disc_cancel();
    s_scanning = false;
    return true;
}

int32_t Bluetooth::DeviceCount()
{
    return s_DeviceCount;
}

const char *Bluetooth::GetDeviceAddress(int32_t index)
{
    if (index < 0 || index >= s_DeviceCount)
    {
        return nullptr;
    }
    return s_btDevices[index].addrStr;
}

const char *Bluetooth::GetDeviceName(int32_t index)
{
    if (index < 0 || index >= s_DeviceCount)
    {
        return nullptr;
    }
    if (s_btDevices[index].name[0] == '\0')
    {
        return nullptr;
    }
    return s_btDevices[index].name;
}

int Bluetooth::GetDeviceCount()
{
    return 1;
}

bool Bluetooth::Connect(const char *address, int32_t timeoutMs)
{
    // TODO: implement device connect
    (void)address;
    (void)timeoutMs;
    return false;
}

bool Bluetooth::Disconnect(const char *address)
{
    // TODO: implement device disconnect
    (void)address;
    return false;
}

int32_t Bluetooth::ReadCharacteristic(
    const char *address,
    const char *characteristicUuid,
    uint8_t *buffer,
    int32_t bufferLength)
{
    // TODO: read characteristic into buffer and return number of bytes read
    (void)address;
    (void)characteristicUuid;
    (void)buffer;
    (void)bufferLength;
    return -1;
}

bool Bluetooth::WriteCharacteristic(
    const char *address,
    const char *characteristicUuid,
    const uint8_t *data,
    int32_t length)
{
    // TODO: write characteristic data
    (void)address;
    (void)characteristicUuid;
    (void)data;
    (void)length;
    return false;
}

#pragma endregion

#pragma region Sockets

void NativeSockets::SetReceiveTimeout(int32_t socket, int32_t timeoutMs)
{
    timeval timeout;
    timeout.tv_sec = timeoutMs / 1000;
    timeout.tv_usec = (timeoutMs % 1000) * 1000;
    setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
}
void NativeSockets::SetSendTimeout(int32_t socket, int32_t timeoutMs)
{
    timeval timeout;
    timeout.tv_sec = timeoutMs / 1000;
    timeout.tv_usec = (timeoutMs % 1000) * 1000;
    setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
}
bool NativeSockets::IsValidSocket(int32_t socket)
{
    return socket > 0;
}
bool NativeSockets::ResolveHost(const char *host, uint32_t *address)
{
    if (inet_pton(AF_INET, host, address) == 1)
    {
        return true;
    }

    hostent *entry = gethostbyname(host);

    if (entry == nullptr)
    {
        return false;
    }
    memcpy(address, entry->h_addr, entry->h_length);

    return true;
}

int32_t NativeSockets::Create(SocketProtocol protocol)
{
    int32_t type;
    switch (protocol)
    {
        case SocketProtocol::SocketProtocol_Tcp:
            type = SOCK_STREAM;
            break;
        case SocketProtocol::SocketProtocol_Udp:
            type = SOCK_DGRAM;
            break;
        default:
            return -1;
    }
    return socket(AF_INET, type, IPPROTO_IP);
}

bool NativeSockets::Close(int32_t socket)
{
    return close(socket) == 0;
}

bool NativeSockets::Bind(int32_t socket, uint16_t port)
{
    sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);
    int result = bind(socket, reinterpret_cast<sockaddr *>(&address), sizeof(address));
    return result == 0;
}

bool NativeSockets::Connect(int32_t socket, const char *host, uint16_t port, int32_t timeoutMs)
{
    sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    if (!ResolveHost(host, &address.sin_addr.s_addr))
    {
        return false;
    }
    SetSendTimeout(socket, timeoutMs);
    return connect(socket, (sockaddr *)&address, sizeof(address)) == 0;
}
bool NativeSockets::Listen(int32_t socket, uint16_t port, int32_t maxConnections)
{
    if (!Bind(socket, port))
    {
        return false;
    }
    return listen(socket, maxConnections) == 0;
}

int32_t NativeSockets::Accept(int32_t socket, int32_t timeoutMs)
{
    fd_set readSet;

    FD_ZERO(&readSet);
    FD_SET(socket, &readSet);

    timeval timeout;

    timeout.tv_sec = timeoutMs / 1000;
    timeout.tv_usec = (timeoutMs % 1000) * 1000;

    int ready = select(socket + 1, &readSet, nullptr, nullptr, &timeout);

    if (ready <= 0)
    {
        return -1;
    }

    sockaddr_in clientAddress;
    socklen_t clientLength = sizeof(clientAddress);

    return accept(socket, reinterpret_cast<sockaddr *>(&clientAddress), &clientLength);
}

bool NativeSockets::IsConnected(int32_t socket)
{
    if (!IsValidSocket(socket))
    {
        return false;
    }
    // Remote peek to check if still connected
    char value;
    int result = recv(socket, &value, 1, MSG_PEEK | MSG_DONTWAIT);
    if (result == 0)
    {
        return false;
    }
    if (result < 0)
    {
        if ((errno == EWOULDBLOCK) || (errno == EAGAIN))
        {
            return true;
        }
        return false;
    }
    return true;
}

int32_t NativeSockets::Send(int32_t socket, const uint8_t *data, int32_t offset, int32_t length)
{
    return send(socket, data + offset, length, 0);
}

int32_t NativeSockets::Receive(int32_t socket, uint8_t *buffer, int32_t offset, int32_t length, int32_t timeoutMs)
{
    SetReceiveTimeout(socket, timeoutMs);
    return recv(socket, buffer + offset, length, 0);
}

int32_t NativeSockets::SendTo(
    int32_t socket,
    const char *host,
    uint16_t port,
    const uint8_t *data,
    int32_t offset,
    int32_t length)
{
    sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    if (!ResolveHost(host, &address.sin_addr.s_addr))
    {
        return false;
    }
    return sendto(socket, data + offset, length, 0, (sockaddr *)&address, sizeof(address));
}

int32_t NativeSockets::ReceiveFrom(
    int32_t socket,
    uint8_t *buffer,
    int32_t offset,
    int32_t length,
    int32_t timeoutMs,
    uint8_t *remoteAddress,
    uint8_t *remotePort)
{
    SetReceiveTimeout(socket, timeoutMs);
    sockaddr_in sender;
    socklen_t senderLength = sizeof(sender);
    int32_t received = recvfrom(socket, buffer + offset, length, 0, (sockaddr *)&sender, &senderLength);
    if (received <= 0)
    {
        return received;
    }
    uint32_t address = ntohl(sender.sin_addr.s_addr);
    remoteAddress[0] = (address >> 24) & 0xFF;
    remoteAddress[1] = (address >> 16) & 0xFF;
    remoteAddress[2] = (address >> 8) & 0xFF;
    remoteAddress[3] = address & 0xFF;
    uint16_t port = ntohs(sender.sin_port);
    remotePort[0] = (port >> 8) & 0xFF;
    remotePort[1] = port & 0xFF;
    return received;
}

#pragma endregion
