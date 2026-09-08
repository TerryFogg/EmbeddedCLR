#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

list (APPEND Hashing_Sources
    ${CMAKE_SOURCE_DIR}/src/nanoFramework.System.IO.Hashing/nf_sys_io_hashing.cpp
    ${CMAKE_SOURCE_DIR}/targets/Espressif/common/nanoFramework.System.IO.Hashing/nf_sys_io_hashing_System_IO_Hashing_Crc32.cpp
)

list(APPEND Hashing_Includes
            ${CMAKE_SOURCE_DIR}/src/nanoFramework.System.IO.Hashing
            ${CMAKE_SOURCE_DIR}/targets/Espressif/common/nanoFramework.System.IO.Hashing
)
target_sources(nanoCLR PUBLIC ${Hashing_Sources} )
target_include_directories(nanoCLR PUBLIC  ${Hashing_Includes} )   

