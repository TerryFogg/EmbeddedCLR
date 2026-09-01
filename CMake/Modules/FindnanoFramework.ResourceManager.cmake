#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#
list(APPEND nanoFramework.ResourceManager_INCLUDE_DIRS
            "${CMAKE_SOURCE_DIR}/src/HAL/Include"
            "${CMAKE_SOURCE_DIR}/src/nanoFramework.ResourceManager"
            "${CMAKE_SOURCE_DIR}/src/nanoFramework.Graphics/Graphics/Core"
            "${CMAKE_SOURCE_DIR}/src/nanoFramework.Graphics/Graphics/Displays"
)
set(nanoFramework.ResourceManager_SRCS
    nf_system_resourcemanager_nanoFramework_Runtime_Native_ResourceUtility.cpp
    nf_system_resourcemanager_System_Resources_ResourceManager.cpp
    nf_system_resourcemanager.cpp
)
foreach(SRC_FILE ${nanoFramework.ResourceManager_SRCS})
    set(nanoFramework.ResourceManager_SRC_FILE SRC_FILE-NOTFOUND)
    find_file(nanoFramework.ResourceManager_SRC_FILE ${SRC_FILE} PATHS
             ${CMAKE_SOURCE_DIR}/src/nanoFramework.ResourceManager
            CMAKE_FIND_ROOT_PATH_BOTH
    )
    list(APPEND nanoFramework.ResourceManager_SOURCES ${nanoFramework.ResourceManager_SRC_FILE})
endforeach()
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(nanoFramework.ResourceManager DEFAULT_MSG nanoFramework.ResourceManager_INCLUDE_DIRS nanoFramework.ResourceManager_SOURCES)
