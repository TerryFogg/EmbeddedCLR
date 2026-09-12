#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#
cmake_minimum_required(VERSION 3.30)

string(TIMESTAMP BUILD_TIMESTAMP UTC)
project(nanoFramework VERSION ${BUILD_VERSION} LANGUAGES C CXX ASM)
add_executable(nanoCLR)

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/CMake"
    "${CMAKE_SOURCE_DIR}/CMake/IOfeatures"
    "${CMAKE_SOURCE_DIR}/CMake/CoreFeatures"
    "${CMAKE_SOURCE_DIR}/CMake/RTOS"
    "${CMAKE_SOURCE_DIR}/CMake/Vendor/${VENDOR}"
)
include(binutils.common)
include(ExternalProject)
include(CMakeToolsHelpers OPTIONAL)
nf_check_path_limits()

include(${TARGET_FAMILY})
include(nf-Interpreter)

foreach(FEATURE IN LISTS FEATURE_DEFINES)
    string(REPLACE "=" ";" RESULT "${FEATURE}")
    list(GET RESULT 0 name)
    list(GET RESULT 1 enabled)
    string(TOUPPER ${name} NAME_UPPER)
    set(CONFIG_SUPPORT_${NAME_UPPER} ${enabled})
    target_compile_definitions(nanoCLR PRIVATE CONFIG_SUPPORT_${NAME_UPPER}=$<BOOL:${enabled}> )
endforeach()

foreach(FEATURE IN LISTS CORE_FEATURES  IO_FEATURES)
    string(REPLACE "=" ";" RESULT "${FEATURE}")
    list(GET RESULT 0 name)
    list(GET RESULT 1 enabled)
    string(TOUPPER ${name} NAME_UPPER)
    set(CONFIG_SUPPORT_${NAME_UPPER} ${enabled})
    if( CONFIG_SUPPORT_${NAME_UPPER} )
        include(${name})
    endif()
endforeach()

include(CommonBuild)
target_sources(nanoCLR PUBLIC
               ${CMAKE_SOURCE_DIR}/targets/Espressif/${TARGET_SERIES}/Startup/app_main.c
               ${CMAKE_SOURCE_DIR}/targets/Espressif/${TARGET_SERIES}/Configuration/CLR_RT_InteropAssembliesTable.cpp
)
nf_generate_build_output_files(nanoCLR)


                option("NF_SECURITY_MBEDTLS": true)


# Strange define for the check of an include file
target_compile_definitions(nanoCLR PRIVATE "CONFIG_RTOS=1" )

    

#nanoCLR_Runtime.h
# NANOCLR_APPDOMAINS
# NANOCLR_VALIDATE_APPDOMAIN_ISOLATION
# NANOCLR_PROFILE_HANDLER
# NOCLR_PROFILE_NEW_CALLS
# NANANOCLR_EMULATED_FLOATINGPOINT
# VIRTUAL_DEVICE
# NANOCLR_ENABLE_SOURCELEVELDEBUGGING
# PLATFORM_WINDOWS_EMULATOR
# HIGH_SURROGATE_START
# NANOCLR_OPCODE_NAMES
# NANOCLR_OPCODE_STACKCHANGES
# NANOCLR_OPCODE_PARSER
# NANOCLR_NO_IL_INLINE
# ENABLE_NATIVE_PROFILER
# NANOCLR_GC_VERBOSE
# NANOCLR_USE_AVLTREE_FOR_METHODLOOKUP
# BUILD_RTM
#NANOCLR_NO_ASSEMBLY_STRINGS
#NANOCLR_FILL_MEMORY_WITH_DIRTY_PATTERN
#NANOCLR_DELEGATE_PRESERVE_STACK

