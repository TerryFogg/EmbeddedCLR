#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

if(NOT DEFINED NETWORK)
    set(NETWORK ON)
endif()

list(APPEND Bluetooth_Sources
)

list(APPEND Bluetooth_Includes
)

target_sources(nanoCLR.elf PUBLIC ${Bluetooth_Sources} )
target_include_directories(nanoCLR.elf PUBLIC  ${Bluetooth_Includes} )   
