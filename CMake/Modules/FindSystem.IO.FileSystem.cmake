#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#
set(BASE_PATH_FOR_THIS_MODULE ${BASE_PATH_FOR_CLASS_LIBRARIES_MODULES}/System.IO.FileSystem)
set(PROJECT_COMMON_PATH ${PROJECT_SOURCE_DIR}/targets/ESP32/_common)
list(APPEND System.IO.FileSystem_INCLUDE_DIRS ${BASE_PATH_FOR_THIS_MODULE})
list(APPEND System.IO.FileSystem_INCLUDE_DIRS ${TARGET_BASE_LOCATION}/Include)
list(APPEND System.IO.FileSystem_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/src/System.IO.FileSystem)

set(System.IO.FileSystem_SRCS
    nf_sys_io_filesystem.cpp
    nf_sys_io_filesystem_nanoFramework_System_IO_FileSystem_SDCard_stubs.cpp
    nf_sys_io_filesystem_nanoFramework_System_IO_FileSystem_SDCard.cpp
    nf_sys_io_filesystem_System_IO_Directory.cpp
    nf_sys_io_filesystem_System_IO_DriveInfo.cpp
    nf_sys_io_filesystem_System_IO_NativeFileStream.cpp
    nf_sys_io_filesystem_System_IO_NativeFindFile.cpp
    nf_sys_io_filesystem_System_IO_NativeIO.cpp
    nanoPAL_FileSystem.cpp
    Target_System_IO_FileSystem.c
    target_FileSystem.cpp
)
