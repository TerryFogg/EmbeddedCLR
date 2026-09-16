//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#ifndef ESP32_IDF_H
#define ESP32_IDF_H

#include <nanoCLR_Headers.h>
#include <target_platform.h>

#include <sys/time.h>
#include <time.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>
#include <freertos/event_groups.h>
#include <esp_system.h>
#include <esp_attr.h>
#include <nvs_flash.h>
#include <sdkconfig.h>
 
#include <esp_timer.h>
#include <esp_sleep.h>

// need this hack here, otherwise can't call ESP_LOGxx from cpp code
#ifndef CONFIG_LOG_TIMESTAMP_SOURCE_RTOS
#define CONFIG_LOG_TIMESTAMP_SOURCE_RTOS 1
#endif

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
#endif

#include <esp_log.h>

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
#endif

#include <soc/i2s_reg.h>
#include <driver/uart.h>
#include <soc/uart_channel.h>
#include <esp_rom_crc.h>
#include <esp_rom_caps.h>

#if HAL_USE_THREAD == TRUE
#include "esp_openthread.h"
#include "esp_openthread_netif_glue.h"
#include "esp_vfs_eventfd.h"
#endif


#ifdef __cplusplus
extern "C"
{
#endif

    int ets_printf(const char *fmt, ...);

#ifdef __cplusplus
}

#endif

#endif // ESP32_IDF_H
