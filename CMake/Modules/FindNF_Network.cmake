#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#
include(FetchContent)
FetchContent_GetProperties(mbedtls)
FetchContent_GetProperties(esp32_idf)

list(APPEND NF_Network_INCLUDE_DIRS
            ${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets
            ${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl
            ${CMAKE_SOURCE_DIR}/src/PAL/Lwip
            ${CMAKE_SOURCE_DIR}/src/PAL
            ${CMAKE_SOURCE_DIR}/src/DeviceInterfaces/Networking.Sntp
            ${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS
            ${mbedtls_SOURCE_DIR}/include
            ${esp32_idf_SOURCE_DIR}/components/mbedtls/port/include
            ${esp32_idf_SOURCE_DIR}/components/mbedtls/port/include/mbedtls
            ${esp32_idf_SOURCE_DIR}/components/mbedtls/mbedtls/include
)
set(NF_Network_SRCS
    sockets_lwip.cpp
    lwIP_Sockets.cpp
    lwIP_Sockets_functions.cpp 
    Target_Network.cpp
    targetHAL_Network.cpp
)
set(NF_Network_Security_SRCS
    ssl.cpp
    ssl_accept_internal.cpp
    ssl_add_cert_auth_internal.cpp
    ssl_close_socket_internal.cpp
    ssl_connect_internal.cpp
    ssl_decode_private_key_internal.cpp
    ssl_exit_context_internal.cpp
    ssl_generic.cpp
    ssl_generic_init_internal.cpp
    ssl_initialize_internal.cpp
    ssl_parse_certificate_internal.cpp
    ssl_available_internal.cpp
    ssl_read_internal.cpp
    ssl_uninitialize_internal.cpp
    ssl_write_internal.cpp
)

foreach(SRC_FILE ${NF_Network_SRCS})
    set(NF_Network_SRC_FILE SRC_FILE-NOTFOUND)
    find_file(NF_Network_SRC_FILE ${SRC_FILE} PATHS
            ${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets
            ${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl
            ${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS
            ${CMAKE_SOURCE_DIR}/src/PAL/Lwip
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_common
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_Network
            ${BASE_PATH_FOR_CLASS_LIBRARIES_MODULES}
            CMAKE_FIND_ROOT_PATH_BOTH
    )
    list(APPEND NF_Network_SOURCES ${NF_Network_SRC_FILE})
endforeach()

set(NF_Security_Search_Path "${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS")

# 2nd pass: security files if option is selected 
foreach(SRC_FILE ${NF_Network_Security_SRCS})
    set(NF_Network_SRC_FILE SRC_FILE-NOTFOUND)
    find_file(NF_Network_SRC_FILE ${SRC_FILE}  PATHS 
            ${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl
            ${NF_Security_Search_Path}
            CMAKE_FIND_ROOT_PATH_BOTH
    )
    list(APPEND NF_Network_SOURCES ${NF_Network_SRC_FILE})
endforeach()

# unset this warning as error, which is required for these source files
# OK to remove after this issue is fixed upstream https://github.com/Mbed-TLS/mbedtls/issues/9425
set_source_files_properties(${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_accept_internal.cpp PROPERTIES COMPILE_FLAGS -Wno-undef)
set_source_files_properties(${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_add_cert_auth_internal.cpp PROPERTIES COMPILE_FLAGS -Wno-undef)
set_source_files_properties(${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_close_socket_internal.cpp PROPERTIES COMPILE_FLAGS -Wno-undef)
set_source_files_properties(${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_connect_internal.cpp PROPERTIES COMPILE_FLAGS -Wno-undef)
set_source_files_properties(${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_decode_private_key_internal.cpp PROPERTIES COMPILE_FLAGS -Wno-undef)
set_source_files_properties(${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_exit_context_internal.cpp PROPERTIES COMPILE_FLAGS -Wno-undef)
set_source_files_properties(${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_generic.cpp PROPERTIES COMPILE_FLAGS -Wno-undef)
set_source_files_properties(${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_generic_init_internal.cpp PROPERTIES COMPILE_FLAGS -Wno-undef)
set_source_files_properties(${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_initialize_internal.cpp PROPERTIES COMPILE_FLAGS -Wno-undef)
set_source_files_properties(${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_parse_certificate_internal.cpp PROPERTIES COMPILE_FLAGS -Wno-undef)
set_source_files_properties(${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_available_internal.cpp PROPERTIES COMPILE_FLAGS -Wno-undef)
set_source_files_properties(${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_read_internal.cpp PROPERTIES COMPILE_FLAGS -Wno-undef)
set_source_files_properties(${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_uninitialize_internal.cpp PROPERTIES COMPILE_FLAGS -Wno-undef)
set_source_files_properties(${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_write_internal.cpp PROPERTIES COMPILE_FLAGS -Wno-undef)


if(Use_Networking_Extra_Driver)
    foreach(SRC_FILE ${NF_Network_Driver_Srcs})
        set(NF_Network_SRC_FILE SRC_FILE-NOTFOUND)
        find_file(NF_Network_SRC_FILE ${SRC_FILE}
            PATHS 
                ${NF_Network_Driver_Path}
    
            CMAKE_FIND_ROOT_PATH_BOTH
        )
        list(APPEND NF_Network_SOURCES ${NF_Network_SRC_FILE})
    endforeach()
endif()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(NF_Network DEFAULT_MSG NF_Network_INCLUDE_DIRS NF_Network_SOURCES)

# macro to be called from binutils to add network library
# BUILD_TARGET parameter to set the target it's building
# optional EXTRA_SOURCES with source files to be added to the library
# optional EXTRA_INCLUDES with include paths to be added to the library
# optional EXTRA_COMPILE_DEFINITIONS with compiler definitions to be added to the library
# optional EXTRA_COMPILE_OPTIONS with compile options to be added to the library
macro(nf_add_lib_network)
    cmake_parse_arguments(NFALN "" "BUILD_TARGET;EXTRA_COMPILE_OPTIONS" "EXTRA_SOURCES;EXTRA_INCLUDES;EXTRA_COMPILE_DEFINITIONS" ${ARGN})
    set(LIB_NAME NF_Network)
    add_library( ${LIB_NAME} STATIC 
               ${NF_Network_SOURCES}
               ${NFALN_EXTRA_SOURCES})
    target_include_directories( ${LIB_NAME} PUBLIC 
                                ${NF_Network_INCLUDE_DIRS}
                                ${NF_CoreCLR_INCLUDE_DIRS}
                                ${NFALN_EXTRA_INCLUDES})
                    
    target_compile_definitions( ${LIB_NAME} PUBLIC
                                -DPLATFORM_ESP32
                                ${NFALN_EXTRA_COMPILE_DEFINITIONS}
    )
    add_library("nano::${LIB_NAME}" ALIAS ${LIB_NAME})
endmacro()
