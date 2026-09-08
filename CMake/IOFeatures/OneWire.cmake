#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

list(APPEND OneWire_Sources
            ${CMAKE_SOURCE_DIR}/src/nanoFramework.Device.OneWire/nf_dev_onewire.cpp
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Device.OneWire/nf_dev_onewire_nanoFramework_Device_OneWire_OneWireHost.cpp
)

list(APPEND OneWire_Includes
            ${CMAKE_SOURCE_DIR}/src/nanoFramework.Device.OneWire
            ${CMAKE_SOURCE_DIR}/targets/ESP32/ESP32_P4/
)

target_sources(nanoCLR PUBLIC
               ${OneWire_Sources}
)

target_include_directories(nanoCLR PUBLIC 
                           ${OneWire_Includes}
)   
