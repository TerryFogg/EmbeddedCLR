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
set(TARGET_SERIES_SHORT "esp32p4" CACHE INTERNAL "ESP32 target series lower case, short version")
set(ESP32_CPU_TYPE "riscv" CACHE INTERNAL "Setting CPU type")
set(BUILD_VERSION 0.0.0.0)

project(nanoFramework VERSION ${BUILD_VERSION} LANGUAGES C CXX ASM)

include(ClrProfilerOptions)

set(TARGET_BASE_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/targets/ESP32/ESP32_P4)
set(BASE_PATH_FOR_PLATFORM ${CMAKE_CURRENT_SOURCE_DIR}/targets/ESP32)
set(BASE_PATH_FOR_CLASS_LIBRARIES_MODULES ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR)

add_subdirectory(${CMAKE_SOURCE_DIR}/targets/ESP32)
nf_add_idf_as_library()
add_subdirectory(${TARGET_BASE_LOCATION})
nf_clear_output_files_nanoclr()

SET(GRAPHICS_SUPPORT TRUE)
set(CONFIG_TOUCH_DISPLAY_SUPPORT TRUE)
add_compile_definitions("CONFIG_TOUCH_DISPLAY_SUPPORT")
SET( DISPLAY_CONTROLLER "DisplayControllerJD9365.cpp")
SET( DISPLAY_INTERFACE_CONTROLLER "DisplayInterfaceMipiDsi.cpp")
SET(TOUCH_CONTROLLER  "GT911.cpp")
SET(TOUCH_INTERFACE_CONTROLLER  "I2c_To_TouchPanel.cpp")
include( CMake/Sources/Options/Graphics.cmake)

#get_target_property(NativeAssemblySources NF_NativeAssemblies SOURCES)
#list(APPEND NativeAssemblySources ${GRAPHIC_SOURCES})
#list(APPEND NativeAssemblySources ${TOUCH_SOURCES})
#set_target_properties(NF_NativeAssemblies PROPERTIES SOURCES "${NativeAssemblySources}")
#
#get_target_property(NativeAssemblyIncludes NF_NativeAssemblies INCLUDE_DIRECTORIES)
#list(APPEND NativeAssemblyIncludes ${GRAPHICS_INCLUDES})
#list(APPEND NativeAssemblyIncludes ${TOUCH_INCLUDES})
#set_target_properties(NF_NativeAssemblies PROPERTIES INCLUDE_DIRECTORIES "${NativeAssemblyIncludes}")
#
get_target_property(SRCS_INCLUDES nanoCLR.elf INCLUDE_DIRECTORIES)
list(APPEND SRCS_INCLUDES ${GRAPHICS_INCLUDES})
list(APPEND SRCS_INCLUDES ${TOUCH_INCLUDES})
set_target_properties(nanoCLR.elf PROPERTIES INCLUDE_DIRECTORIES "${SRCS_INCLUDES}")


    target_sources(nanoCLR.elf PUBLIC
                   ${GRAPHICS_SOURCES}
                   ${TOUCH_SOURCES}
    )


#
