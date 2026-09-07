#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

list(APPEND UsbStream_Sources
    ${CMAKE_SOURCE_DIR}/src/System.Device.UsbStream/sys_dev_usbstream_native.cpp
    ${CMAKE_SOURCE_DIR}/src/System.Device.UsbStream/sys_dev_usbstream_native_System_Device_Usb_UsbStream_stubs.cpp
)

list(APPEND UsbStream_Includes
            ${CMAKE_SOURCE_DIR}/src/System.Device.UsbStream
)

target_sources(nanoCLR PUBLIC ${UsbStream_Sources} )
target_include_directories(nanoCLR PUBLIC  ${UsbStream_Includes} )   
