#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

list(APPEND Network_Sources
            ${CMAKE_SOURCE_DIR}/targets/common/NativeCalls_NativeSocket.cpp
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/core/NativeNetwork.cpp
)

list(APPEND Network_Includes
            ${CMAKE_SOURCE_DIR}/targets/common
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/core
)

target_sources(nanoCLR.elf PUBLIC ${Network_Sources} )
target_include_directories(nanoCLR.elf PUBLIC  ${Network_Includes} )   
