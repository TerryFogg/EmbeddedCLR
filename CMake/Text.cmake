#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

list(APPEND Text_Includes
            "${CMAKE_SOURCE_DIR}/src/HAL/Include"
            "${CMAKE_SOURCE_DIR}/src/nanoFramework.System.Text"
)
list(APPEND Text_Sources
    ${CMAKE_SOURCE_DIR}/src/nanoFramework.System.Text/nf_system_text_System_Text_UTF8Decoder.cpp
    ${CMAKE_SOURCE_DIR}/src/nanoFramework.System.Text/nf_system_text_System_Text_UTF8Encoding.cpp
    ${CMAKE_SOURCE_DIR}/src/nanoFramework.System.Text/nf_system_text.cpp
)
target_sources(nanoCLR PUBLIC ${Text_Sources} )
target_include_directories(nanoCLR PUBLIC  ${Text_Includes} )   
