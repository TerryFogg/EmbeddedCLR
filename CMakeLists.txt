#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#
cmake_minimum_required(VERSION 3.30)

string(TIMESTAMP BUILD_TIMESTAMP UTC)

project(nanoFramework VERSION ${BUILD_VERSION} LANGUAGES C CXX ASM)
add_executable(nanoCLR.elf 
               ${CMAKE_SOURCE_DIR}/targets/Espressif/common/core/app_main.c
               ${CMAKE_SOURCE_DIR}/targets/Espressif/${TARGET_SERIES}/CLR_RT_InteropAssembliesTable.cpp
 )

list(APPEND CMAKE_MODULE_PATH 
    "${CMAKE_SOURCE_DIR}/CMake"
    "${CMAKE_SOURCE_DIR}/CMake/IOfeatures"
    "${CMAKE_SOURCE_DIR}/CMake/CoreFeatures"
)
include(Common)
include(ExternalProject)
include(CMakeToolsHelpers OPTIONAL)

# Include feature defines, these can be used in the following core and io features for additonal options
foreach(FEATURE IN LISTS FEATURE_DEFINES ADVANCED_DEBUGGING)
    string(REPLACE "=" ";" RESULT "${FEATURE}")
    list(GET RESULT 0 name)
    list(GET RESULT 1 enabled)
    string(TOUPPER ${name} NAME_UPPER)
    set(CONFIG_SUPPORT_${NAME_UPPER} ${enabled})
    target_compile_definitions(nanoCLR.elf PRIVATE CONFIG_SUPPORT_${NAME_UPPER}=$<BOOL:${enabled}> )
endforeach()

foreach(FEATURE IN LISTS CORE_FEATURES  IO_FEATURES)
    string(REPLACE "=" ";" RESULT "${FEATURE}")
    list(GET RESULT 0 name)
    list(GET RESULT 1 enabled)
    string(TOUPPER ${name} NAME_UPPER)
    set(CONFIG_SUPPORT_${NAME_UPPER} ${enabled})
    target_compile_definitions(nanoCLR.elf PRIVATE CONFIG_SUPPORT_${NAME_UPPER}=$<BOOL:${enabled}> )
    if(CONFIG_SUPPORT_${NAME_UPPER}) 
     include(${name})
    endif()
endforeach()

include(nf-Interpreter)
include(${TARGET_FAMILY})
include(CommonBuild)
