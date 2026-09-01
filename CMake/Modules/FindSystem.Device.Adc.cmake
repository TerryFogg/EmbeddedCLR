#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

# native code directory
set(BASE_PATH_FOR_THIS_MODULE ${BASE_PATH_FOR_CLASS_LIBRARIES_MODULES}/System.Device.Adc)


# set include directories
list(APPEND System.Device.Adc_INCLUDE_DIRS
            ${CMAKE_SOURCE_DIR}/src/CLR/Core
            ${CMAKE_SOURCE_DIR}/src/CLR/Include
            ${CMAKE_SOURCE_DIR}/src/HAL/Include
            ${CMAKE_SOURCE_DIR}/src/PAL/Include
            ${TARGET_BASE_LOCATION}
            ${BASE_PATH_FOR_THIS_MODULE}
            ${CMAKE_SOURCE_DIR}/src/System.Device.Adc
)
set(System.Device.Adc_SRCS
    sys_dev_adc_native.cpp
    sys_dev_adc_native_System_Device_Adc_AdcChannel.cpp
    sys_dev_adc_native_System_Device_Adc_AdcController.cpp
    target_system_device_adc_config.cpp
)
foreach(SRC_FILE ${System.Device.Adc_SRCS})
    set(System.Device.Adc_SRC_FILE SRC_FILE-NOTFOUND)
    find_file(System.Device.Adc_SRC_FILE ${SRC_FILE}  PATHS
	        ${TARGET_BASE_LOCATION}
	        ${BASE_PATH_FOR_THIS_MODULE}
            ${CMAKE_SOURCE_DIR}/src/System.Device.Adc
	    CMAKE_FIND_ROOT_PATH_BOTH
    )
    list(APPEND System.Device.Adc_SOURCES ${System.Device.Adc_SRC_FILE})
endforeach()
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(System.Device.Adc DEFAULT_MSG System.Device.Adc_INCLUDE_DIRS System.Device.Adc_SOURCES)
