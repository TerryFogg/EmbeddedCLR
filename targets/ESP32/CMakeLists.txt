#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

include(FetchContent)
include(binutils.ESP32)

set(ESP32_RESERVE_IRAM_IDF_ALLOCATION_KB "0" CACHE STRING "Setting default value for ESP32_RESERVE_IRAM_IDF_ALLOCATION_KB")
set(ESP32_RESERVE_SPIRAM_IDF_ALLOCATION_BYTES "0" CACHE STRING "Setting default value for ESP32_RESERVE_SPIRAM_IDF_ALLOCATION_BYTES")
set(BASE_PATH_FOR_PLATFORM ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
set(NO_ESP32_IDF_PATH FALSE)
set(esp32_idf_SOURCE_DIR ${ESP32_IDF_PATH})

list(APPEND CMAKE_MODULE_PATH ${esp32_idf_SOURCE_DIR}/CMake)

string(REPLACE "\\" "/" IDF_SOURCE_DIR_PATH "$ENV{IDF_PATH}")
string(TOLOWER "${IDF_SOURCE_DIR_PATH}" IDF_SOURCE_DIR_PATH_LOWER )
string(TOLOWER "${esp32_idf_SOURCE_DIR}" esp32_idf_SOURCE_DIR_LOWER )
set(IDF_PATH_CMAKED ${IDF_SOURCE_DIR_PATH} CACHE INTERNAL "CMake formated IDF path")


if(NF_FEATURE_HAS_SDCARD)
    if(EXISTS ${esp32_idf_SOURCE_DIR}/components/fatfs/src/ffconf.h)
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy ${esp32_idf_SOURCE_DIR}/components/fatfs/src/ffconf.h ${esp32_idf_SOURCE_DIR}/components/fatfs/src/ffconf.h.sav
        )
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E remove ${esp32_idf_SOURCE_DIR}/components/fatfs/src/ffconf.h
        )
    endif()
else()
    # if not using FatFS need to check if IDF ffconf.h exists
    if(EXISTS ${esp32_idf_SOURCE_DIR}/components/fatfs/src/ffconf.h)
        # all good, ffconf.h still there
    else()
        # ffconf.h doesn't exist so restore it
        if(EXISTS ${esp32_idf_SOURCE_DIR}/components/fatfs/src/ffconf.h.sav)
            execute_process(
                COMMAND ${CMAKE_COMMAND} -E copy ${esp32_idf_SOURCE_DIR}/components/fatfs/src/ffconf.h.sav ${esp32_idf_SOURCE_DIR}/components/fatfs/src/ffconf.h
            )
        else()
            message(FATAL_ERROR "\r\n*******************************************************************\r\n'${esp32_idf_SOURCE_DIR}/components/fatfs/src/ffconf.h' file is missing from IDF\r\nIf this was deleted on a previous build using SDCard, just revert the change and restore the file.\r\n*******************************************************************\r\n")
        endif()
    endif()
endif()

set(NF_SECURITY_MBEDTLS ON CACHE INTERNAL "MbedTLS must be ON for IDF build")
add_subdirectory(_common)
add_subdirectory(_include)
add_subdirectory(_nanoCLR)
if(USE_NETWORKING_OPTION)
    add_subdirectory(_Network)
endif()
