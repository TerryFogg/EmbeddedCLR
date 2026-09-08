#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

list(APPEND ResourceManager_Sources
    ${CMAKE_SOURCE_DIR}/src/nanoFramework.ResourceManager/nf_system_resourcemanager_nanoFramework_Runtime_Native_ResourceUtility.cpp
    ${CMAKE_SOURCE_DIR}/src/nanoFramework.ResourceManager/nf_system_resourcemanager_System_Resources_ResourceManager.cpp
    ${CMAKE_SOURCE_DIR}/src/nanoFramework.ResourceManager/nf_system_resourcemanager.cpp
)

list(APPEND ResourceManager_Includes
            ${CMAKE_SOURCE_DIR}/src/nanoFramework.ResourceManager
)

target_sources(nanoCLR PUBLIC ${ResourceManager_Sources} )
target_include_directories(nanoCLR PUBLIC  ${ResourceManager_Includes} )   
