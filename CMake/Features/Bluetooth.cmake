#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#


list(APPEND Bluetooth_Sources
            ${CMAKE_SOURCE_DIR}/targets/common/NativeCalls_NativeBluetooth.cpp
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/core/NativeNetwork.cpp
)

list(APPEND Bluetooth_Includes
            ${CMAKE_SOURCE_DIR}/targets/common/core
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/core
)

target_sources(nanoCLR.elf PUBLIC ${Bluetooth_Sources} )
target_include_directories(nanoCLR.elf PUBLIC  ${Bluetooth_Includes} )   


