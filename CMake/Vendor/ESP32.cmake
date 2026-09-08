#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

include(FetchContent)


set(ESP32_RESERVE_IRAM_IDF_ALLOCATION_KB "0" CACHE STRING "Setting default value for ESP32_RESERVE_IRAM_IDF_ALLOCATION_KB")
set(ESP32_RESERVE_SPIRAM_IDF_ALLOCATION_BYTES "0" CACHE STRING "Setting default value for ESP32_RESERVE_SPIRAM_IDF_ALLOCATION_BYTES")

set(esp32_idf_SOURCE_DIR ${ESP32_IDF_PATH})
list(APPEND CMAKE_MODULE_PATH ${esp32_idf_SOURCE_DIR}/CMake)

string(REPLACE "\\" "/" IDF_SOURCE_DIR_PATH "$ENV{IDF_PATH}")
string(TOLOWER "${IDF_SOURCE_DIR_PATH}" IDF_SOURCE_DIR_PATH_LOWER )
string(TOLOWER "${esp32_idf_SOURCE_DIR}" esp32_idf_SOURCE_DIR_LOWER )

set(IDF_PATH_CMAKED ${IDF_SOURCE_DIR_PATH} CACHE INTERNAL "CMake formated IDF path")


list(APPEND ESP32_Sources
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/DeviceMapping_common.cpp
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/Device_BlockStorage.c
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/ESP32_P4_DeviceMapping.cpp
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/GenericPort_Write.cpp
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/nanoSupport_CRC32.c
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/platform_BlockStorage.c
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/platform_heap.c
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/targetHAL.c
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/targetHAL_ConfigStorageLittlefs.c
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/targetHAL_ConfigurationManager.cpp
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/targetHAL_FileOperation.cpp
           #${CMAKE_SOURCE_DIR}/targets/ESP32/_common/targetHAL_Network.cpp
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/targetHAL_Rtos.c
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/targetHAL_StorageOperation.cpp
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/targetHAL_Time.cpp
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/Target_BlockStorage_ESP32FlashDriver.c
            #${CMAKE_SOURCE_DIR}/targets/ESP32/_common/Target_System_IO_FileSystem.c
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/WireProtocol_HAL_Interface.c
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/WireProtocol_ReceiverThread.c

            ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/CLR_Startup_Thread.c 
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/Memory.cpp
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoCRT.cpp
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoHAL.cpp
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/targetHAL.cpp
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/targetHAL_Power.c
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/targetHAL_Time.cpp
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/targetPAL.c 
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/targetPAL_Events.cpp
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/targetPAL_I2c.cpp
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/targetPAL_Time.cpp
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/targetRandom.cpp
)
list(APPEND ESP32_Includes
            ${CMAKE_SOURCE_DIR}/targets/ESP32/ESP32_P4/Configuration
)

target_sources(nanoCLR PUBLIC ${ESP32_Sources} )
target_include_directories(nanoCLR PUBLIC ${ESP32_Includes} )

# Add the linker option to ESP32 build to suppress warnings about read/write/execute segments
target_link_options(nanoCLR PUBLIC "-Wl,--no-warn-rwx-segments")

set(TARGET_CONFIGURATION_PATH  ${CMAKE_SOURCE_DIR}/targets/ESP32/ESP32_P4/Configuration CACHE INTERNAL "Target configuration path")
configure_file(${TARGET_CONFIGURATION_PATH}/ESP32_target_os.h.in ${CMAKE_BINARY_DIR}/target_os.h @ONLY)
configure_file(${TARGET_CONFIGURATION_PATH}/target_platform.h.in ${CMAKE_BINARY_DIR}/target_platform.h @ONLY)
configure_file(${TARGET_CONFIGURATION_PATH}/target_common.h.in ${CMAKE_BINARY_DIR}/target_common.h @ONLY)
configure_file(${TARGET_CONFIGURATION_PATH}/target_board.h.in ${CMAKE_BINARY_DIR}/target_board.h @ONLY)

set(ESP32_PARTITION_TABLE_UTILITY ${IDF_PATH_CMAKED}/components/partition_table/gen_esp32part.py )
set(gen_partition_table "python" "${ESP32_PARTITION_TABLE_UTILITY}")

add_custom_target(generate_partition ALL
    COMMAND ${gen_partition_table} 
    --flash-size 4MB 
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_IDF/esp32p4/partitions_nanoclr_4mb.csv
    ${CMAKE_BINARY_DIR}/partitions_4mb.bin
    COMMENT "Generate partition table for 4MB flash" )

add_custom_command( TARGET generate_partition POST_BUILD
    COMMAND ${gen_partition_table} 
    --flash-size 8MB 
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_IDF/esp32p4/partitions_nanoclr_8mb.csv
    ${CMAKE_BINARY_DIR}/partitions_8mb.bin
    COMMENT "Generate partition table for 8MB flash" )

add_custom_command( TARGET generate_partition POST_BUILD
    COMMAND ${gen_partition_table} 
    --flash-size 16MB 
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_IDF/esp32p4/partitions_nanoclr_16mb.csv
    ${CMAKE_BINARY_DIR}/partitions_16mb.bin
    COMMENT "Generate partition table for 16MB flash" )

# 32MB partition table for ESP32_S3
add_custom_command( TARGET generate_partition POST_BUILD
    COMMAND ${gen_partition_table} 
    --flash-size 32MB 
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_IDF/esp32p4/partitions_nanoclr_32mb.csv
    ${CMAKE_BINARY_DIR}/partitions_32mb.bin
    COMMENT "Generate partition table for 32MB flash" )
