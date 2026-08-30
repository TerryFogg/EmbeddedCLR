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

