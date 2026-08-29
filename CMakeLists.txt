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
string(TOUPPER ${RTOS} RTOS_UPPERCASE)
set(RTOS_${RTOS_UPPERCASE}_CHECK TRUE)
set(BUILD_VERSION 0.0.0.0)
project(nanoFramework VERSION ${BUILD_VERSION} LANGUAGES C CXX ASM)

set(NANOBOOTER_PROJECT_NAME "nanoBooter")
set(NANOCLR_PROJECT_NAME "nanoCLR")
include(ClrProfilerOptions)
set(NF_FEATURE_USE_LITTLEFS_OPTION FALSE CACHE INTERNAL "Set nF littlefs feature FALSE")
set(HAL_USE_UART_OPTION TRUE CACHE INTERNAL "HAL OneWire for nanoFramework.Device.OneWire")





set(USE_NETWORKING_OPTION FALSE CACHE INTERNAL "")
set(API_Windows.Devices.Spi FALSE CACHE INTERNAL "")
set(API_System.Device.Spi FALSE CACHE INTERNAL "")
set(USE_ENC28J60_DRIVER_OPTION FALSE CACHE INTERNAL "NF ENC28J60 network driver")
set(NF_NETWORKING_SNTP FALSE CACHE INTERNAL "SNTP is off as there is no network support")

set(HAL_USE_ADC_OPTION FALSE CACHE INTERNAL "HAL ADC for System.Device.Adc")
set(HAL_USE_DAC_OPTION FALSE CACHE INTERNAL "HAL DAC for System.Device.Dac")
set(HAL_USE_GPIO_OPTION TRUE CACHE INTERNAL "HAL GPIO for System.Device.Gpio")
set(HAL_USE_I2C_OPTION FALSE CACHE INTERNAL "HAL I2C for System.Device.I2c")
set(HAL_USE_I2S_OPTION FALSE CACHE INTERNAL "HAL I2S for System.Device.I2s")
set(HAL_USE_PWM_OPTION FALSE CACHE INTERNAL "HAL PWM for System.DevicesPwm")
set(HAL_USE_SPI_OPTION FALSE CACHE INTERNAL "HAL SPI for System.Device.Spi")
set(HAL_USE_CAN_OPTION FALSE CACHE INTERNAL "HAL CAN for nanoFramework.Device.Can")
set(HAL_USE_ONEWIRE_OPTION FALSE CACHE INTERNAL "HAL ONEWIRE for nanoFramework.Device.OneWire")

set(NANOCLR_GRAPHICS TRUE CACHE INTERNAL "Graphics Support")
set(HAL_USE_SDC_OPTION FALSE CACHE INTERNAL "HAL SDC for NF_FEATURE_HAS_SDCARD")
set(HAL_USBH_USE_MSD_OPTION FALSE CACHE INTERNAL "HAL USBH_USE_MSD for NF_FEATURE_HAS_USB_MSD")
set(HAL_USE_MAC_OPTION FALSE CACHE INTERNAL "HAL MAC disabled for WiFi targets")
set(USE_FILESYSTEM_OPTION FALSE CACHE INTERNAL "NF feature FILESYSTEM")
set(HAL_USE_RTC_OPTION FALSE CACHE INTERNAL "NF feature RTC")
set(HAL_USE_WDG_OPTION TRUE CACHE INTERNAL "NF feature watchdog")


if(TARGET_NAME)
    message(STATUS "Target name set to ${TARGET_NAME}.")
else()
    # use target board as the name
    set(TARGET_NAME ${TARGET_BOARD})
endif()

# Define PLATFORM base path
set(BASE_PATH_FOR_PLATFORM ${CMAKE_CURRENT_SOURCE_DIR}/targets/${RTOS})


    # enables embedded USB CDC 
    # (default is OFF so embedded USB CDC is NOT enabled)
    option(ESP32_USB_CDC "option to enable USB Mass Storage")

    # Define base path for the class libraries
    nf_set_base_path_for_libraries_modules(${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR)

        set(HAL_USE_BLE_OPTION FALSE CACHE INTERNAL "No Bluetooth Support")


        set(ESP32_ETHERNET_SUPPORT FALSE CACHE INTERNAL "")



        set(HAL_USE_THREAD_OPTION FALSE CACHE INTERNAL "No OpenThread Support")

    if(NOT NF_FEATURE_WATCHDOG)
        message(FATAL_ERROR "\n\nESP32 watchdog is enabled by default in so you can't have the NF_FEATURE_WATCHDOG option set to OFF.\n\n")
    endif()

    # now add the subdirectory for the board
    # try to find board in the targets folder
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/targets/ESP32/${TARGET_BOARD})

        # set target base location
        set(TARGET_BASE_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/targets/ESP32/${TARGET_BOARD})

        # board found
        message(STATUS "Support for target board '${TARGET_BOARD}' found")
        message(STATUS "${TARGET_BASE_LOCATION}")

    else()

        # try to find board in the Community targets folder
        if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/targets-community/ESP32/${TARGET_BOARD})

            # set target base location
            set(TARGET_BASE_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/targets-community/ESP32/${TARGET_BOARD})
        
            # board found
            message(STATUS "Support for target board '${TARGET_BOARD}' found in Community targets")
            message(STATUS "${TARGET_BASE_LOCATION}")

        else()
            # board NOT found in targets folder
            message(FATAL_ERROR "\n\nSorry but support for ${TARGET_BOARD} target is not available...\n\nYou can wait for that to be added or you might want to contribute and start working on a PR for that.\n\n")
        endif()
    endif()

    add_subdirectory(${CMAKE_SOURCE_DIR}/targets/ESP32)
    nf_add_idf_as_library()
    add_subdirectory(${TARGET_BASE_LOCATION})
    nf_clear_output_files_nanoclr()

