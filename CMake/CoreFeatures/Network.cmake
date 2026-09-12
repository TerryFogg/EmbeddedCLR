#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

list(APPEND Network_Sources

    ${CMAKE_SOURCE_DIR}/src/DeviceInterfaces/System.Net/sys_net_native.cpp  
    ${CMAKE_SOURCE_DIR}/src/DeviceInterfaces/System.Net/sys_net_native_System_Net_Sockets_NativeSocket.cpp
    ${CMAKE_SOURCE_DIR}/src/DeviceInterfaces/System.Net/sys_net_native_System_Net_IPAddress.cpp
    ${CMAKE_SOURCE_DIR}/src/DeviceInterfaces/System.Net/sys_net_native_System_Net_NetworkInformation_NetworkInterface.cpp
    ${CMAKE_SOURCE_DIR}/src/DeviceInterfaces/System.Net/sys_net_native_System_Net_NetworkInformation_IPGlobalProperties.cpp
    ${CMAKE_SOURCE_DIR}/src/DeviceInterfaces/System.Net/sys_net_native_System_Net_NetworkInformation_Wireless80211Configuration.cpp
    ${CMAKE_SOURCE_DIR}/src/DeviceInterfaces/System.Net/sys_net_native_System_Net_NetworkInformation_WirelessAPConfiguration.cpp
    ${CMAKE_SOURCE_DIR}/src/DeviceInterfaces/System.Net/sys_net_native_System_Security_Cryptography_X509Certificates_X509Certificate.cpp
    ${CMAKE_SOURCE_DIR}/src/DeviceInterfaces/System.Net/sys_net_native_System_Security_Cryptography_X509Certificates_X509Certificate2.cpp
    ${CMAKE_SOURCE_DIR}/src/DeviceInterfaces/System.Net/sys_net_native_System_Net_Security_SslNative.cpp
    ${CMAKE_SOURCE_DIR}/src/DeviceInterfaces/System.Net/sys_net_native_System_Net_Security_CertificateManager.cpp


    ${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/sockets_lwip.cpp

    ${CMAKE_SOURCE_DIR}/src/PAL/Lwip/lwIP_Sockets.cpp
    ${CMAKE_SOURCE_DIR}/src/PAL/Lwip/lwIP_Sockets_functions.cpp
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/Target_Network.cpp
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/targetHAL_Network.cpp

)

if(MBEDTLS)
    list(APPEND Network_Sources
                ${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/ssl.cpp
                ${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_accept_internal.cpp
                ${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_add_cert_auth_internal.cpp
                ${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_close_socket_internal.cpp
                ${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_connect_internal.cpp
                ${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_decode_private_key_internal.cpp
                ${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_exit_context_internal.cpp
                ${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_generic.cpp
                ${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_generic_init_internal.cpp
                ${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_initialize_internal.cpp
                ${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_parse_certificate_internal.cpp
                ${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_available_internal.cpp
                ${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_read_internal.cpp
                ${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_uninitialize_internal.cpp
                ${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS/ssl_write_internal.cpp
    )

    list(APPEND Network_Includes
            ${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl/MbedTLS
            ${esp32_idf_SOURCE_DIR}/components/mbedtls/port/include
            ${esp32_idf_SOURCE_DIR}/components/mbedtls/port/include/mbedtls
            ${esp32_idf_SOURCE_DIR}/components/mbedtls/mbedtls/include
            ${mbedtls_SOURCE_DIR}/include
)

endif()



list(APPEND Network_Includes
            ${CMAKE_SOURCE_DIR}/src/DeviceInterfaces/System.Net
            ${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets
            ${CMAKE_SOURCE_DIR}/src/PAL/COM/sockets/ssl
            ${CMAKE_SOURCE_DIR}/src/DeviceInterfaces/Networking.Sntp
            ${mbedtls_SOURCE_DIR}/include
            ${CMAKE_SOURCE_DIR}/src/PAL/Lwip
)


if(SNTP)
    list(APPEND Network_Sources
        ${CMAKE_SOURCE_DIR}/src/DeviceInterfaces/Networking.Sntp/nf_networking_sntp_nanoFramework_Networking_Sntp.cpp
        ${CMAKE_SOURCE_DIR}/src/DeviceInterfaces/Networking.Sntp/nf_networking_sntp.cpp    
    )
    list(APPEND Network_Includes
                ${CMAKE_SOURCE_DIR}/src/DeviceInterfaces/Networking.Sntp
    )
endif()

if(WIFI)
    list(APPEND Network_Sources
                ${CMAKE_SOURCE_DIR}/src/System.Device.Wifi/sys_dev_wifi_native.cpp
                ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/System.Device.Wifi/sys_dev_wifi_native_System_Device_Wifi_WifiAdapter.cpp
    )
    list(APPEND Network_Includes
                ${CMAKE_SOURCE_DIR}/src/System.Device.Wifi
    )
endif()


if(ETHERNET)
   configure_file(${CMAKE_CURRENT_SOURCE_DIR}/esp32_ethernet_options.h.in
                  ${CMAKE_BINARY_DIR}/targets/ESP32/${TARGET_BOARD}/esp32_ethernet_options.h @ONLY)
   list(APPEND Network_Sources
               ${CMAKE_CURRENT_SOURCE_DIR}/NF_ESP32_Network.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/NF_ESP32_Ethernet.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/NF_ESP32_Wireless.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/NF_ESP32_SmartConfig.cpp
)
endif()

if (HAL_USE_THREAD_OPTION)
  list(APPEND Network_Sources
              ${CMAKE_CURRENT_SOURCE_DIR}/NF_ESP32_OpenThread.cpp)
endif()

target_sources(nanoCLR PUBLIC ${Network_Sources} )
target_include_directories(nanoCLR PUBLIC  ${Network_Includes} )   


target_compile_definitions(nanoCLR PRIVATE LWIP_IPV6=0)
target_compile_definitions(nanoCLR PRIVATE PLATFORM_ESP32=0)
target_compile_definitions(nanoCLR PRIVATE CONFIG_SOC_WIFI_SUPPORTED=0)
target_compile_definitions(nanoCLR PRIVATE CONFIG_SOC_WIRELESS_HOST_SUPPORTED=0)






###    if(USE_NETWORKING_OPTION)
###   
###        FetchContent_GetProperties(esp32_idf)
###   
###        # get list of source files for lwIP
###        get_target_property(IDF_LWIP_SOURCES __idf_lwip SOURCES)
###   
###        # remove the ones we'll be replacing
###        list(REMOVE_ITEM 
###            IDF_LWIP_SOURCES
###                ${ESP32_IDF_PATH}/components/lwip/lwip/src/api/api_msg.c
###                ${ESP32_IDF_PATH}/components/lwip/lwip/src/api/sockets.c
###                ${ESP32_IDF_PATH}/components/lwip/port/freertos/sys_arch.c
###        )
###   
###        # add our modified sources
###        list(APPEND 
###            IDF_LWIP_SOURCES
###                ${CMAKE_SOURCE_DIR}/targets/ESP32/_lwIP/nf_api_msg.c
###                ${CMAKE_SOURCE_DIR}/targets/ESP32/_lwIP/nf_sockets.c
###                ${CMAKE_SOURCE_DIR}/targets/ESP32/_lwIP/nf_sys_arch.c
###        )
###   
###        # replace the source list
###        set_property(
###            TARGET __idf_lwip 
###            PROPERTY SOURCES ${IDF_LWIP_SOURCES}
###        )
###        
###        # get list of include directories for lwIP
###        get_target_property(IDF_LWIP_INCLUDE_DIRECTORIES __idf_lwip INCLUDE_DIRECTORIES)
###   
###        # add nanoCLR include path to lwIP so our lwipots are taken instead of the IDF ones
###        list(INSERT 
###            IDF_LWIP_INCLUDE_DIRECTORIES 0
###                ${CMAKE_SOURCE_DIR}/targets/ESP32/_include
###                ${CMAKE_SOURCE_DIR}/targets/ESP32/ESP32_P4
###                ${CMAKE_SOURCE_DIR}/src/DeviceInterfaces/Networking.Sntp
###                ${CMAKE_SOURCE_DIR}/src/CLR/Include
###                ${CMAKE_SOURCE_DIR}/src/HAL/Include
###        )
###   
###        # replace the include directories
###        set_property(
###            TARGET __idf_lwip 
###            PROPERTY
###            INCLUDE_DIRECTORIES
###            ${IDF_LWIP_INCLUDE_DIRECTORIES}
###        )
###   
###        # add nanoCLR compile definitions to lwIP
###        list(APPEND 
###            IDF_LWIP_COMPILE_DEFINITIONS 
###                PLATFORM_ESP32
###                ESP_LWIP_COMPONENT_BUILD
###            )
###   
###        # add the compile definitions
###        set_property(
###            TARGET __idf_lwip 
###            PROPERTY COMPILE_DEFINITIONS ${IDF_LWIP_COMPILE_DEFINITIONS}
###        )
###   
###    endif()
###
###    # need to add include path to find our ffconfig.h and target_platform.h
###    
###    # get list of include directories for FATFS
###    get_target_property(IDF_FATFS_INCLUDE_DIRECTORIES __idf_fatfs INCLUDE_DIRECTORIES)
###
###    # add nanoCLR include path to FATFS so our lwipots are taken instead of the IDF ones
###    list(APPEND
###        IDF_FATFS_INCLUDE_DIRECTORIES
###        ${CMAKE_BINARY_DIR}/targets/ESP32/ESP32_P4/
###    )
###
###    # add nanoCLR include path to FATFS so our lwipots are taken instead of the IDF ones
###    list(APPEND
###        IDF_FATFS_INCLUDE_DIRECTORIES
###            ${CMAKE_SOURCE_DIR}/targets/ESP32/ESP32_P4
###    )
###
###    # replace the include directories
###    set_property( TARGET __idf_fatfs 
###        PROPERTY INCLUDE_DIRECTORIES ${IDF_FATFS_INCLUDE_DIRECTORIES}
###    )
###
