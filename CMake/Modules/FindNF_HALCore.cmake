#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

# set include directories for nanoHAL Core

# include directories for nanoHAL Core
list(APPEND NF_HALCore_INCLUDE_DIRS
            ${CMAKE_SOURCE_DIR}/src/CLR/Include
            ${CMAKE_SOURCE_DIR}/src/HAL/Include
            ${BASE_PATH_FOR_PLATFORM}/Include
            ${CMAKE_BINARY_DIR}/targets/ESP32/ESP32_P4
)
set(NF_HALCore_SRCS
    nanoHAL_Capabilites.c
    nanoHAL_Boot.c
)
foreach(SRC_FILE ${NF_HALCore_SRCS})
    set(NF_HALCore_SRC_FILE SRC_FILE-NOTFOUND)
    find_file(NF_HALCore_SRC_FILE ${SRC_FILE}  PATHS 
            ${CMAKE_SOURCE_DIR}/src/HAL
            ${BASE_PATH_FOR_PLATFORM}/common
            ${TARGET_BASE_LOCATION}
        CMAKE_FIND_ROOT_PATH_BOTH
    )
    list(APPEND NF_HALCore_SOURCES ${NF_HALCore_SRC_FILE})
endforeach()
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(NF_HALCore DEFAULT_MSG NF_HALCore_INCLUDE_DIRS NF_HALCore_SOURCES)
