#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

list(APPEND Runtime_Serialization_Sources
    ${CMAKE_SOURCE_DIR}/src/System.Runtime.Serialization/nf_system_runtime_serialization.cpp
    ${CMAKE_SOURCE_DIR}/src/System.Runtime.Serialization/nf_system_runtime_serialization_System_Runtime_Serialization_Formatters_Binary_BinaryFormatter.cpp
)

list(APPEND Runtime_Serialization_Includes
            ${CMAKE_SOURCE_DIR}/src/System.Runtime.Serialization
)


target_sources(nanoCLR PUBLIC ${Runtime_Serialization_Sources} )
target_include_directories(nanoCLR PUBLIC  ${Runtime_Serialization_Includes} )   
