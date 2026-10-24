#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#
#
list(APPEND FileSystem_Sources
            ${CMAKE_SOURCE_DIR}/targets/common/NativeCalls_FileSystem.cpp
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/core/NativeFile.cpp
)

list(APPEND FileSystem_Includes
            ${CMAKE_SOURCE_DIR}/targets/common
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/Core
)

target_sources(nanoCLR.elf PUBLIC ${FileSystem_Sources} )
target_include_directories(nanoCLR.elf PUBLIC  ${FileSystem_Includes} )   

