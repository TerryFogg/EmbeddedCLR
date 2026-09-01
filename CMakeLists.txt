#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

cmake_minimum_required(VERSION 3.30)

include(CMakeToolsHelpers OPTIONAL)
include(ExternalProject)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)
include(binutils.common)
include(binutils.ESP32)
nf_check_path_limits()
set(CMAKE_BUILD_TYPE "Debug")
add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")
string(TIMESTAMP BUILD_TIMESTAMP UTC)

if(DEFINED NF_TARGET_DEFCONFIG)
    include(NF_Kconfig)
    nf_load_kconfig()
endif()

set(TARGET_BOARD "ESP32_P4")
set(RTOS_ESP32_CHECK TRUE)
set(TARGET_SERIES_SHORT "esp32p4" CACHE INTERNAL "ESP32 target series lower case, short version")
set(ESP32_CPU_TYPE "riscv" CACHE INTERNAL "Setting CPU type")
set(BUILD_VERSION 0.0.0.0)

project(nanoFramework VERSION ${BUILD_VERSION} LANGUAGES C CXX ASM)

set(NANOCLR_PROJECT_NAME "nanoCLR")
include(ClrProfilerOptions)
set(NF_FEATURE_USE_LITTLEFS_OPTION FALSE CACHE INTERNAL "Set nF littlefs feature FALSE")
set(HAL_USE_UART_OPTION TRUE CACHE INTERNAL "HAL OneWire for nanoFramework.Device.OneWire")

set(BASE_PATH_FOR_PLATFORM ${CMAKE_CURRENT_SOURCE_DIR}/targets/${RTOS})
option(ESP32_USB_CDC "option to enable USB Mass Storage")
nf_set_base_path_for_libraries_modules(${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR)
set(HAL_USE_BLE_OPTION FALSE CACHE INTERNAL "No Bluetooth Support")
set(ESP32_ETHERNET_SUPPORT FALSE CACHE INTERNAL "")
set(HAL_USE_THREAD_OPTION FALSE CACHE INTERNAL "No OpenThread Support")
set(TARGET_BASE_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/targets/ESP32/${TARGET_BOARD})



add_subdirectory(${CMAKE_SOURCE_DIR}/targets/ESP32)
nf_add_idf_as_library()
add_subdirectory(${TARGET_BASE_LOCATION})
nf_clear_output_files_nanoclr()
