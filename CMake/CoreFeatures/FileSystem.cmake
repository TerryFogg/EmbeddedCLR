#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

list(APPEND FileSystem_Sources
    ${CMAKE_SOURCE_DIR}/src/CLR/Core/FileStream/FileStream.cpp
    ${CMAKE_SOURCE_DIR}/src/System.IO.FileSystem/nf_sys_io_filesystem.cpp
    ${CMAKE_SOURCE_DIR}/src/System.IO.FileSystem/nf_sys_io_filesystem_System_IO_Directory.cpp
    ${CMAKE_SOURCE_DIR}/src/System.IO.FileSystem/nf_sys_io_filesystem_System_IO_DriveInfo.cpp
    ${CMAKE_SOURCE_DIR}/src/System.IO.FileSystem/nf_sys_io_filesystem_System_IO_NativeFileStream.cpp
    ${CMAKE_SOURCE_DIR}/src/System.IO.FileSystem/nf_sys_io_filesystem_System_IO_NativeFindFile.cpp
    ${CMAKE_SOURCE_DIR}/src/System.IO.FileSystem/nf_sys_io_filesystem_System_IO_NativeIO.cpp
    ${CMAKE_SOURCE_DIR}/src/PAL/FileSystem/nanoPAL_FileSystem.cpp


    ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/System.IO.FileSystem/nf_sys_io_filesystem_nanoFramework_System_IO_FileSystem_SDCard.cpp
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/Target_System_IO_FileSystem.c
    ${CMAKE_SOURCE_DIR}/targets/ESP32/ESP32_P4/target_FileSystem.cpp
)

list(APPEND FileSystem_Includes
            ${CMAKE_SOURCE_DIR}/src/System.IO.FileSystem)

target_sources(nanoCLR PUBLIC ${FileSystem_Sources} )
target_include_directories(nanoCLR PUBLIC  ${FileSystem_Includes} )   

#define SDC_MAX_OPEN_FILES 5
