#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

# append target Esp32 nanoCLR source files
set( TARGET_ESP32_IDF_NANOCLR_SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/CLR_Startup_Thread.c 
            ${CMAKE_CURRENT_SOURCE_DIR}/targetPAL.c 
            ${CMAKE_CURRENT_SOURCE_DIR}/nanoHAL.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/targetHAL_Power.c
            ${CMAKE_CURRENT_SOURCE_DIR}/targetPAL_Events.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/targetPAL_Time.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/Memory.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/nanoCRT.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/targetHAL.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/targetHAL_Time.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/targetRandom.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/targetPAL_I2c.cpp
CACHE INTERNAL "make global")


set(TARGET_ESP32_IDF_NANOCLR_INCLUDE_DIRS
           ${CMAKE_CURRENT_SOURCE_DIR}
           ${CMAKE_CURRENT_BINARY_DIR}
           ${CMAKE_SOURCE_DIR}/src/CLR/Core
           ${CMAKE_SOURCE_DIR}/src/PAL/Include
           ${CMAKE_SOURCE_DIR}/src/CLR/Runtime.Native
     CACHE INTERNAL "make global"
)
