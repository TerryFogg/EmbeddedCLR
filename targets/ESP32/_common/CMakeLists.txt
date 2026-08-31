#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

list(APPEND TARGET_ESP32_IDF_COMMON_SOURCES
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/Device_BlockStorage.c
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/GenericPort_Write.cpp
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/platform_BlockStorage.c
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/platform_heap.c
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/Target_BlockStorage_ESP32FlashDriver.c
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/targetHAL.c
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/targetHAL_Time.cpp
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/targetHAL_Rtos.c
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/WireProtocol_HAL_Interface.c
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/WireProtocol_ReceiverThread.c
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/targetHAL_ConfigStorageLittlefs.c
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/targetHAL_ConfigurationManager.cpp
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/targetHAL_StorageOperation.cpp
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/targetHAL_FileOperation.cpp
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/nanoSupport_CRC32.c
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/DeviceMapping_common.cpp
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/ESP32_P4_DeviceMapping.cpp
)
set(TARGET_ESP32_IDF_COMMON_SOURCES ${TARGET_ESP32_IDF_COMMON_SOURCES} CACHE INTERNAL "make global")
