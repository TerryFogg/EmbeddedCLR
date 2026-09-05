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

set(NANOCLR_PROJECT_NAME "nanoCLR")
set(TARGET_BOARD "ESP32_P4")
set(RTOS_ESP32_CHECK TRUE)
set(ESP32_CPU_TYPE "riscv" CACHE INTERNAL "Setting CPU type")
set(BUILD_VERSION 0.0.0.0)
add_compile_definitions(CONFIG_TOUCH_DISPLAY_SUPPORT)

project(nanoFramework VERSION ${BUILD_VERSION} LANGUAGES C CXX ASM)

#include(ClrProfilerOptions)



set(BASE_PATH_FOR_PLATFORM ${CMAKE_CURRENT_SOURCE_DIR}/targets/ESP32)
set(BASE_PATH_FOR_CLASS_LIBRARIES_MODULES ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR)

add_subdirectory(${CMAKE_SOURCE_DIR}/targets/ESP32)
nf_add_idf_as_library()
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/targets/ESP32/ESP32_P4)
nf_clear_output_files_nanoclr()
