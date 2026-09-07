#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#
list(APPEND Cryptography_Includes
            ${CMAKE_SOURCE_DIR}/src/nanoFramework.System.Security.Cryptography
)
list(APPEND Cryptography_Sources
    ${CMAKE_SOURCE_DIR}/src/nanoFramework.System.Security.Cryptography/nf_sys_sec_cryptography.cpp
    ${CMAKE_SOURCE_DIR}/src/nanoFramework.System.Security.Cryptography/nf_sys_sec_cryptography_System_Security_Cryptography_Aes.cpp
    ${CMAKE_SOURCE_DIR}/src/nanoFramework.System.Security.Cryptography/nf_sys_sec_cryptography_System_Security_Cryptography_HMACSHA256.cpp
    ${CMAKE_SOURCE_DIR}/src/nanoFramework.System.Security.Cryptography/nf_sys_sec_cryptography_System_Security_Cryptography_HMACSHA512.cpp
)
target_sources(nanoCLR PUBLIC ${Cryptography_Sources} )
target_include_directories(nanoCLR PUBLIC  ${Cryptography_Includes} )   
