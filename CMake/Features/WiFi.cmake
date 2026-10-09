#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

if(NOT DEFINED NETWORK)
    set(NETWORK ON)
endif()


list(APPEND WiFi_Sources
)

list(APPEND WiFi_Includes
)

target_sources(nanoCLR.elf PUBLIC ${WiFi_Sources} )
target_include_directories(nanoCLR.elf PUBLIC  ${WiFi_Includes} )   
