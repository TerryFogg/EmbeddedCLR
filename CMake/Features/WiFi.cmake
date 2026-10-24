#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#


list(APPEND WiFi_Sources
            ${CMAKE_SOURCE_DIR}/targets/common/NativeCalls_NativeWifi.cpp
)

list(APPEND WiFi_Includes
            ${CMAKE_SOURCE_DIR}/targets/common
)

target_sources(nanoCLR.elf PUBLIC ${WiFi_Sources} )
target_include_directories(nanoCLR.elf PUBLIC  ${WiFi_Includes} )   
