#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#
set(BASE_PATH_FOR_THIS_MODULE ${BASE_PATH_FOR_CLASS_LIBRARIES_MODULES}/System.IO.Ports)
list(APPEND System.IO.Ports_INCLUDE_DIRS
            ${CMAKE_SOURCE_DIR}/src/CLR/Core
            ${CMAKE_SOURCE_DIR}/src/CLR/Include
            ${CMAKE_SOURCE_DIR}/src/HAL/Include
            ${CMAKE_SOURCE_DIR}/src/PAL/Include
            ${BASE_PATH_FOR_THIS_MODULE}
            ${CMAKE_SOURCE_DIR}/src/System.IO.Ports
)
set(System.IO.Ports_SRCS
    sys_io_ser_native.cpp
    sys_io_ser_native_System_IO_Ports_SerialPort.cpp
    sys_io_ser_native_System_IO_Ports_SerialPort__.cpp
    target_system_io_ports_config.cpp
)
foreach(SRC_FILE ${System.IO.Ports_SRCS})
    set(System.IO.Ports_SRC_FILE SRC_FILE-NOTFOUND)
    find_file(System.IO.Ports_SRC_FILE ${SRC_FILE}  PATHS
            ${BASE_PATH_FOR_THIS_MODULE}
            ${TARGET_BASE_LOCATION}
            ${CMAKE_SOURCE_DIR}/src/System.IO.Ports
        CMAKE_FIND_ROOT_PATH_BOTH
    )
    list(APPEND System.IO.Ports_SOURCES ${System.IO.Ports_SRC_FILE})
endforeach()
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(System.IO.Ports DEFAULT_MSG System.IO.Ports_INCLUDE_DIRS System.IO.Ports_SOURCES)
