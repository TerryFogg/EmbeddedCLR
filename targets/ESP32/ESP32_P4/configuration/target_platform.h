#pragma once

//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#define ESP32_RESERVE_SPIRAM_IDF_ALLOCATION_BYTES 262144
#define ESP32_RESERVE_IRAM_IDF_ALLOCATION_KB      0

#define NANOCLR_GRAPHICS   TRUE
#define HAL_USE_SPI        FALSE
#define HAL_USE_UART       FALSE
#define HAL_USE_SDC        FALSE
#define HAL_USE_BLE        FALSE
#define SDC_MAX_OPEN_FILES 5
