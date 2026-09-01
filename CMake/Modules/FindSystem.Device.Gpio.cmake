#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

# native code directory
set(BASE_PATH_FOR_THIS_MODULE ${BASE_PATH_FOR_CLASS_LIBRARIES_MODULES}/System.Device.Gpio)


# set include directories
list(APPEND System.Device.Gpio_INCLUDE_DIRS
            ${CMAKE_SOURCE_DIR}/src/CLR/Core
            ${CMAKE_SOURCE_DIR}/src/CLR/Include
            ${CMAKE_SOURCE_DIR}/src/HAL/Include
            ${CMAKE_SOURCE_DIR}/src/PAL/Include
            ${BASE_PATH_FOR_THIS_MODULE}
            ${CMAKE_SOURCE_DIR}/src/System.Device.Gpio
)
set(System.Device.Gpio_SRCS
    sys_dev_gpio_native.cpp
    cpu_gpio.cpp
    sys_dev_gpio_native_System_Device_Gpio_GpioController.cpp
    sys_dev_gpio_native_System_Device_Gpio_GpioPin.cpp
)
foreach(SRC_FILE ${System.Device.Gpio_SRCS})
    set(System.Device.Gpio_SRC_FILE SRC_FILE-NOTFOUND)
    find_file(System.Device.Gpio_SRC_FILE ${SRC_FILE} PATHS
	        ${BASE_PATH_FOR_THIS_MODULE}
            ${TARGET_BASE_LOCATION}
            ${CMAKE_SOURCE_DIR}/src/System.Device.Gpio
	    CMAKE_FIND_ROOT_PATH_BOTH
    )
    list(APPEND System.Device.Gpio_SOURCES ${System.Device.Gpio_SRC_FILE})
endforeach()
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(System.Device.Gpio DEFAULT_MSG System.Device.Gpio_INCLUDE_DIRS System.Device.Gpio_SOURCES)
