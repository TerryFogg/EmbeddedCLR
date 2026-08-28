#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#
cmake_minimum_required(VERSION 3.30)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules;${CMAKE_SOURCE_DIR}/CMake)
set(TARGET_BASE_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/targets/ESP32/ESP32_P4)
set(BUILD_VERSION 0.0.0.0)
set(NANOBOOTER_PROJECT_NAME "nanoBooter")
set(NANOCLR_PROJECT_NAME "nanoCLR")

include(CMakeToolsHelpers OPTIONAL)
include(ExternalProject)
include(binutils.common)
include(binutils.ESP32)
include(NF_Kconfig)

add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>"
                    "-Wno-error=cast-user-defined")

nf_load_kconfig()
############################
#set(TARGET_SERIES_SHORT "esp32p4")
#set(TARGET_SERIES "ESP32_P4")
#set(RTOS_ESP32_CHECK TRUE)  # switch for different network



project(nanoFramework VERSION ${BUILD_VERSION} LANGUAGES C CXX ASM)

include(ClrProfilerOptions)

 set(HAL_USE_UART_OPTION TRUE CACHE INTERNAL "HAL OneWire for nanoFramework.Device.OneWire")
 set(NF_FEATURE_USE_LITTLEFS_OPTION FALSE CACHE INTERNAL "Set nF littlefs feature FALSE")
 set(USE_NETWORKING_OPTION TRUE CACHE INTERNAL "NF feature NETWORKING")
 set(USE_SECURITY_MBEDTLS_OPTION TRUE CACHE INTERNAL "NF security MBEDTLS")
 set(NF_NETWORKING_SNTP FALSE CACHE INTERNAL "SNTP is off  or on")
 set(HAL_USE_ADC_OPTION FALSE CACHE INTERNAL "HAL ADC for System.Device.Adc")
 set(HAL_USE_DAC_OPTION FALSE CACHE INTERNAL "HAL DAC for System.Device.Dac")
 set(HAL_USE_GPIO_OPTION FALSE CACHE INTERNAL "HAL GPIO for System.Device.Gpio")
 set(HAL_USE_I2C_OPTION FALSE CACHE INTERNAL "HAL I2C for System.Device.I2c")
 set(HAL_USE_I2S_OPTION FALSE CACHE INTERNAL "HAL I2S for System.Device.I2s")
 set(HAL_USE_PWM_OPTION FALSE CACHE INTERNAL "HAL PWM for System.DevicesPwm")
 set(HAL_USE_SPI_OPTION FALSE CACHE INTERNAL "HAL SPI for System.Device.Spi")
 set(HAL_USE_CAN_OPTION FALSE CACHE INTERNAL "HAL CAN for nanoFramework.Device.Can")
 set(HAL_USE_STM32_ONEWIRE_OPTION FALSE CACHE INTERNAL "HAL STM32_ONEWIRE for nanoFramework.Device.OneWire")
 set(HAL_USE_ONEWIRE_OPTION FALSE CACHE INTERNAL "HAL ONEWIRE for nanoFramework.Device.OneWire")
 set(NANOCLR_GRAPHICS TRUE CACHE INTERNAL "Graphics Support")
 set(HAL_USE_SDC_OPTION FALSE CACHE INTERNAL "HAL SDC for NF_FEATURE_HAS_SDCARD")
 set(HAL_USBH_USE_MSD_OPTION FALSE CACHE INTERNAL "HAL USBH_USE_MSD for NF_FEATURE_HAS_USB_MSD")
 set(HAL_USE_MAC_OPTION FALSE CACHE INTERNAL "HAL MAC for USE_NETWORKING_OPTION")
 set(USE_FILESYSTEM_OPTION FALSE CACHE INTERNAL "NF feature FILESYSTEM")
 set(HAL_USE_RTC_OPTION FALSE CACHE INTERNAL "NF feature RTC")
 set(HAL_USE_WDG_OPTION TRUE CACHE INTERNAL "NF feature watchdog")
 set(HEX2DFU_TOOL_AVAILABLE FALSE CACHE INTERNAL "hex2dfu tool available")
 set(SRECORD_TOOL_AVAILABLE FALSE CACHE INTERNAL "srec_cat tool available")
 set(ESP32_USB_CDC FALSE CACHE INTERNAL "option to enable USB Mass Storage")

 nf_set_base_path_for_libraries_modules(${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR)

 set(HAL_USE_BLE_OPTION FALSE CACHE INTERNAL "No Bluetooth Support")
 set(HAL_USE_THREAD_OPTION FALSE CACHE INTERNAL "OpenThread Support")

 add_subdirectory(${CMAKE_SOURCE_DIR}/targets/ESP32)
 nf_add_idf_as_library()
 add_subdirectory(${TARGET_BASE_LOCATION})
 nf_clear_output_files_nanoclr()

#NF_ENABLE_DOUBLE_PRECISION_FP
#NF_FEATURE_LIGHT_MATH
#NF_SUPPORT_ANY_BASE_CONVERSION
#NF_FEATURE_USE_RNG
#NF_FEATURE_DEBUGGER
#NF_TRACE_TO_STDIO
#NF_FEATURE_USE_APPDOMAINS
#NF_CLR_NO_TRACE
#NF_CLR_NO_IL_INLINE
#NF_FEATURE_HAS_CONFIG_BLOCK
#NF_FEATURE_HAS_ACCESSIBLE_STORAGE
#NF_FEATURE_HAS_SDCARD
#NF_FEATURE_HAS_USB_MSD -supported only on ChibiOS
#NF_FEATURE_USE_LITTLEFS- this feature currently is supported only on ChibiOS
#NF_TARGET_HAS_NANOBOOTER) this DOES NOT implement nanoBooter"
#NF_WP_ENABLE_CRC32
#NF_FEATURE_SUPPORT_REFLECTION

#API_nanoFramework.System.Collections
#API_nanoFramework.System.Text
#API_System.Net
