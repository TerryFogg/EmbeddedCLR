#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#
list (APPEND Collections_Sources
             ${CMAKE_SOURCE_DIR}/src/nanoFramework.System.Collections/nf_system_collections_System_Collections_Hashtable.cpp
             ${CMAKE_SOURCE_DIR}/src/nanoFramework.System.Collections/nf_system_collections_System_Collections_Hashtable__HashtableEnumerator.cpp
             ${CMAKE_SOURCE_DIR}/src/nanoFramework.System.Collections/nf_system_collections_System_Collections_Queue.cpp
             ${CMAKE_SOURCE_DIR}/src/nanoFramework.System.Collections/nf_system_collections_System_Collections_Stack.cpp
             ${CMAKE_SOURCE_DIR}/src/nanoFramework.System.Collections/nf_system_collections.cpp
)

list(APPEND Collections_Includes
            "${CMAKE_SOURCE_DIR}/src/HAL/Include"
            "${CMAKE_SOURCE_DIR}/src/nanoFramework.System.Collections"
)

target_sources(nanoCLR PUBLIC ${Collections_Sources} )
target_include_directories(nanoCLR PUBLIC  ${Collections_Includes} )   

    target_compile_definitions(nanoCLR PRIVATE "CONFIG_API_NANOFRAMEWORK_SYSTEM_COLLECTIONS=1" )
