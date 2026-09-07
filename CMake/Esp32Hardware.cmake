#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

# set include directories
list(APPEND Hardware_Includes
            ${CMAKE_SOURCE_DIR}/src/CLR/Core
            ${CMAKE_SOURCE_DIR}/src/CLR/Include
            ${CMAKE_SOURCE_DIR}/src/HAL/Include
            ${CMAKE_SOURCE_DIR}/src/PAL/Include
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Hardware.ESP32

            ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Hardware.Esp32.Rmt
            ${CMAKE_SOURCE_DIR}/src/nanoFramework.Hardware.Esp32.Rmt
)
# source files
list(APPEND Hardware_Sources
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Hardware.ESP32/nanoFramework_hardware_esp32_native.cpp
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Hardware.ESP32/nanoFramework_hardware_esp32_native_Hardware_Esp32_Sleep.cpp
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Hardware.ESP32/nanoFramework_hardware_esp32_native_Hardware_Esp32_Logging.cpp
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Hardware.ESP32/nanoFramework_hardware_esp32_native_Hardware_Esp32_HighResTimer.cpp
	${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Hardware.ESP32/nanoFramework_hardware_esp32_native_Hardware_Esp32_Configuration.cpp
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Hardware.ESP32/nanoFramework_hardware_esp32_native_Hardware_Esp32_NativeMemory.cpp
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Hardware.ESP32/nanoFramework_hardware_esp32_native_nanoFramework_Hardware_Esp32_Touch_TouchPad.cpp
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Hardware.ESP32/nanoFramework_hardware_esp32_native_System_Device_Gpio_GpioPulseCounter.cpp

    ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Hardware.Esp32.Rmt/nanoFramework_hardware_esp32_rmt_native.cpp
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Hardware.Esp32.Rmt/nanoFramework_hardware_esp32_rmt_native_nanoFramework_Hardware_Esp32_Rmt_RmtChannel.cpp
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Hardware.Esp32.Rmt/nanoFramework_hardware_esp32_rmt_native_nanoFramework_Hardware_Esp32_Rmt_TransmitterChannel.cpp
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Hardware.Esp32.Rmt/nanoFramework_hardware_esp32_rmt_native_nanoFramework_Hardware_Esp32_Rmt_ReceiverChannel.cpp
)

target_sources(nanoCLR PUBLIC
               ${Hardware_Sources}
)
target_include_directories(nanoCLR PUBLIC 
                           ${Hardware_Includes}
)   
