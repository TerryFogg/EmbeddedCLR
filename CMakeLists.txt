#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#
cmake_minimum_required(VERSION 3.30)

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/CMake"
    "${CMAKE_SOURCE_DIR}/CMake/IOfeatures"
    "${CMAKE_SOURCE_DIR}/CMake/CoreFeatures"
    "${CMAKE_SOURCE_DIR}/CMake/RTOS"
    "${CMAKE_SOURCE_DIR}/CMake/Vendor"
)

include(binutils.common)
include(ExternalProject)
include(CMakeToolsHelpers OPTIONAL)
nf_check_path_limits()

project(nanoFramework VERSION ${BUILD_VERSION} LANGUAGES C CXX ASM)
add_executable(nanoCLR)

if(DEFINED NF_TARGET_DEFCONFIG)
    include(NF_Kconfig)
    nf_load_kconfig()
endif()

string(TIMESTAMP BUILD_TIMESTAMP UTC)

include(${TARGET_FAMILY})
include(${TARGET_RTOS})
include(nf-Interpreter)

foreach(FEATURE IN LISTS ENABLED_CORE_FEATURES)
        include(${FEATURE})
endforeach()

foreach(FEATURE IN LISTS ENABLED_IO_FEATURES)
        include(${FEATURE})
endforeach()

include(CommonBuild)
target_sources(nanoCLR PUBLIC
               ${CMAKE_SOURCE_DIR}/targets/ESP32/_IDF/esp32p4/app_main.c
               ${CMAKE_SOURCE_DIR}/targets/ESP32/ESP32_P4/configuration/CLR_RT_InteropAssembliesTable.cpp
)
nf_add_idf_as_library()
idf_build_executable(nanoCLR)
nf_generate_build_output_files(nanoCLR)
