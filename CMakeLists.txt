#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#
cmake_minimum_required(VERSION 3.30)

Set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)

include(binutils.common)
include(ExternalProject)
include(CMakeToolsHelpers OPTIONAL)

project(nanoFramework VERSION ${BUILD_VERSION} LANGUAGES C CXX ASM)
add_executable(nanoCLR)

include(Setup)

include(Esp32_1)
include(${TARGET_FAMILY})
include(nf-Interpreter)

nf_check_path_limits()
set(CMAKE_BUILD_TYPE "Debug")
add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")

string(TIMESTAMP BUILD_TIMESTAMP UTC)

if(DEFINED NF_TARGET_DEFCONFIG)
    include(NF_Kconfig)
    nf_load_kconfig()
endif()



# NF Core

include(CoreIO)
include(RuntimeEvents)
include(RuntimeSerialization)
if(Collections)
    include(Collections)
endif()
if(Cryptography)
   include(Cryptography)
endif()
if(Esp32Hardware)
   include(Esp32Hardware)
endif()
if(Hashing)
    include(Hashing)
endif()
if(FileSystem)
   include(FileSystem)
endif()
if(Graphics)
    include(Graphics)
endif()
if(Mathematics)
   include(Mathematics)
endif()
if(Network)
   include(Network)
endif()
if(OneWire)
   include(OneWire)
endif()
if(ResourceManager)
    include(ResourceManager)
endif()
if(Text)
    include(Text)
endif()
if(UsbStream)
    include(UsbStream)
endif()
if(WireProtocol)
    include(WireProtocol)
endif()

add_compile_definitions(CONFIG_TOUCH_DISPLAY_SUPPORT)
add_compile_definitions(NANOCLR_ENABLE_SOURCELEVELDEBUGGING)
add_compile_definitions(I_AM_NANOCLR)
add_compile_definitions(PLATFORM_NO_CLR_TRACE=0)
add_compile_definitions(NANOCLR_NO_IL_INLINE=0)
add_compile_definitions(TRACE_MASK=0)
add_compile_definitions(PLATFORM_ESP32)
add_compile_definitions(ESP_PLATFORM)
add_compile_definitions(TARGET=esp32p4)
add_compile_definitions(USE_FPU=TRUE)

#add_compile_definitions(Wall
#                        Wextra
#                        Werror
#                        Wno-sign-compare
#                        Wno-unused-parameter
#                        Wshadow
#                        Wimplicit-fallthrough
#                        fshort-wchar
#                        fno-builtin
#                        fno-common
#                        fno-exceptions
#                        fcheck-new )


nf_add_idf_as_library()


target_include_directories(nanoCLR PUBLIC ${CMAKE_SOURCE_DIR}/targets/ESP32/ESP32_P4)

target_sources(nanoCLR PUBLIC
               ${CMAKE_SOURCE_DIR}/targets/ESP32/_IDF/esp32p4/app_main.c
               ${CMAKE_SOURCE_DIR}/targets/ESP32/ESP32_P4/configuration/CLR_RT_InteropAssembliesTable.cpp
)

# Add the linker option to suppress warnings about read/write/execute segments
target_link_options(nanoCLR PUBLIC "-Wl,--no-warn-rwx-segments")

 idf_build_executable(nanoCLR)


nf_generate_build_output_files(nanoCLR)
