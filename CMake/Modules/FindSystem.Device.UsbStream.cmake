#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#
set(BASE_PATH_FOR_THIS_MODULE ${BASE_PATH_FOR_CLASS_LIBRARIES_MODULES}/System.Device.UsbStream)

list(APPEND System.Device.UsbStream_INCLUDE_DIRS
            ${PROJECT_SOURCE_DIR}/src/CLR/Core
            ${PROJECT_SOURCE_DIR}/src/CLR/Include)
            ${PROJECT_SOURCE_DIR}/src/HAL/Include)
            ${PROJECT_SOURCE_DIR}/src/PAL/Include)
            ${BASE_PATH_FOR_THIS_MODULE})
            ${PROJECT_SOURCE_DIR}/src/System.Device.UsbStream
)
set(System.Device.UsbStream_SRCS
    sys_dev_usbstream_native.cpp
    sys_dev_usbstream_native_System_Device_Usb_UsbStream.cpp
)
foreach(SRC_FILE ${System.Device.UsbStream_SRCS})
    set(System.Device.UsbStream_SRC_FILE SRC_FILE-NOTFOUND)
    find_file(System.Device.UsbStream_SRC_FILE ${SRC_FILE} PATHS
	        ${BASE_PATH_FOR_THIS_MODULE}
	        ${TARGET_BASE_LOCATION}
            ${PROJECT_SOURCE_DIR}/src/System.Device.UsbStream
	    CMAKE_FIND_ROOT_PATH_BOTH
    )
    list(APPEND System.Device.UsbStream_SOURCES ${System.Device.UsbStream_SRC_FILE})
endforeach()
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(System.Device.UsbStream DEFAULT_MSG System.Device.UsbStream_INCLUDE_DIRS System.Device.UsbStream_SOURCES)
