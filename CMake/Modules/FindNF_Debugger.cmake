#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

# set include directories for nanoFramework Debugger
list(APPEND NF_Debugger_INCLUDE_DIRS
            ${CMAKE_SOURCE_DIR}/src/CLR/Debugger
            ${CMAKE_SOURCE_DIR}/src/CLR/Messaging
            ${CMAKE_SOURCE_DIR}/src/CLR/WireProtocol
)
set(NF_Debugger_SRCS
    Debugger.cpp
    Messaging.cpp
)
    list(APPEND NF_Debugger_SRCS ${NF_Debugger_SRCS} Debugger_full.cpp)
foreach(SRC_FILE ${NF_Debugger_SRCS})
    set(NF_Debugger_SRC_FILE SRC_FILE-NOTFOUND)
    find_file(NF_Debugger_SRC_FILE ${SRC_FILE}  PATHS 
            ${CMAKE_SOURCE_DIR}/src/CLR/Debugger
            ${CMAKE_SOURCE_DIR}/src/CLR/Messaging
            ${CMAKE_SOURCE_DIR}/src/CLR/WireProtocol
            ${CMAKE_SOURCE_DIR}/src/CLR/Core
        CMAKE_FIND_ROOT_PATH_BOTH
    )
    list(APPEND NF_Debugger_SOURCES ${NF_Debugger_SRC_FILE})
endforeach()
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(NF_Debugger DEFAULT_MSG NF_Debugger_INCLUDE_DIRS NF_Debugger_SOURCES)

macro(nf_add_lib_debugger)
    cmake_parse_arguments(NFALD "" "" "EXTRA_INCLUDES;EXTRA_COMPILE_DEFINITIONS" ${ARGN})
    set(LIB_NAME NF_Debugger)
    add_library(
        ${LIB_NAME} STATIC 
            ${NF_Debugger_SOURCES})   
    target_include_directories(
        ${LIB_NAME} 
        PUBLIC 
            ${NF_Debugger_INCLUDE_DIRS}
            ${NF_CoreCLR_INCLUDE_DIRS}
            ${NFALD_EXTRA_INCLUDES})   
        nf_common_compiler_definitions(TARGET ${LIB_NAME} BUILD_TARGET ${NANOCLR_PROJECT_NAME})

        # this is the only one different
        target_compile_definitions(
            ${LIB_NAME} PUBLIC
            -DPLATFORM_ESP32
            ${NFALD_EXTRA_COMPILER_DEFINITIONS}
        )
    add_library("nano::${LIB_NAME}" ALIAS ${LIB_NAME})
    add_dependencies(${LIB_NAME} nano::WireProtocol)
endmacro()
