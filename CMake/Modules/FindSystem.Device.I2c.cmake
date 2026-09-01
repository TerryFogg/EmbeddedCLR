#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

# native code directory
set(BASE_PATH_FOR_THIS_MODULE ${BASE_PATH_FOR_CLASS_LIBRARIES_MODULES}/System.Device.I2c)


# set include directories
list(APPEND System.Device.I2c_INCLUDE_DIRS
            ${CMAKE_SOURCE_DIR}/src/CLR/Core
            ${CMAKE_SOURCE_DIR}/src/CLR/Include
            ${CMAKE_SOURCE_DIR}/src/HAL/Include
            ${CMAKE_SOURCE_DIR}/src/PAL/Include
            ${TARGET_BASE_LOCATION}
            ${BASE_PATH_FOR_THIS_MODULE}
            ${CMAKE_SOURCE_DIR}/src/System.Device.I2c
)
set(System.Device.I2c_SRCS
    sys_dev_i2c_native.cpp
    sys_dev_i2c_native_System_Device_I2c_I2cDevice.cpp
    target_system_device_i2c_config.cpp
)
foreach(SRC_FILE ${System.Device.I2c_SRCS})
    set(System.Device.I2c_SRC_FILE SRC_FILE-NOTFOUND)
    find_file(System.Device.I2c_SRC_FILE ${SRC_FILE}  PATHS
	        ${TARGET_BASE_LOCATION}
	        ${BASE_PATH_FOR_THIS_MODULE}
            ${CMAKE_SOURCE_DIR}/src/System.Device.I2c
	    CMAKE_FIND_ROOT_PATH_BOTH
    )
    list(APPEND System.Device.I2c_SOURCES ${System.Device.I2c_SRC_FILE})
endforeach()
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(System.Device.I2c DEFAULT_MSG System.Device.I2c_INCLUDE_DIRS System.Device.I2c_SOURCES)
