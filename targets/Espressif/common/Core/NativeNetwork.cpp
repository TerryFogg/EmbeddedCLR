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

#pragma region WiFi

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

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
bool WiFi::StartScan()
{
    esp_err_t result = esp_wifi_scan_start(NULL, true);
    return (result == ESP_OK);
}
bool WiFi::GetScanResults()
{
    uint16_t apCount = 0;
    esp_wifi_scan_stop();
    esp_wifi_scan_get_ap_num(&apCount);
    wifi_ap_record_t *aps = (wifi_ap_record_t *)platform_malloc(sizeof(wifi_ap_record_t) * apCount);
    esp_wifi_scan_get_ap_records(&apCount, aps);

    //for (int i = 0; i < apCount; i++)
    //{
    //    aps->bssid = ;
    //    aps->ssid = ;
    //    aps->rssi = ;
    //    aps->authmode = ;
    //    aps->pairwise_cipher = ;
    //    aps->group_cipher = ;
    //}
    return true;
}

bool WiFi::GetRSSI()
{
    wifi_ap_record_t ap;
    esp_err_t result = esp_wifi_sta_get_ap_info(&ap);
    //if (result)
    //{
    //    int rssi = ap.rssi;
    //}
    return (result == ESP_OK);
}

#pragma endregion
