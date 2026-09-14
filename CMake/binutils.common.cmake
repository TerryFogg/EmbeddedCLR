#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#



# function to check the path limit in Windows
function(nf_check_path_limits)

    # only need to check in Windows
    if (WIN32)
        set(FILESYSTEM_REG_PATH "HKLM\\SYSTEM\\CurrentControlSet\\Control\\FileSystem")
        cmake_host_system_information(RESULT WIN_LONG_PATH_OPTION QUERY WINDOWS_REGISTRY ${FILESYSTEM_REG_PATH} VALUE LongPathsEnabled)
        if(${WIN_LONG_PATH_OPTION} EQUAL 0)
            message(STATUS "******* WARNING ******\n\nWindows path limit is too short.\nPlease enable long paths in Windows registry.\nSee https://docs.microsoft.com/en-us/windows/win32/fileio/maximum-file-path-limitation?tabs=cmd#enable-long-paths-in-windows-10-version-1607-and-later\n\n")
            set(CMAKE_OBJECT_PATH_MAX 260)
            set(CMAKE_OBJECT_NAME_MAX 255)
        endif()
    endif()
endfunction()

