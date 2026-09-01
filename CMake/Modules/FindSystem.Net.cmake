#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

list(APPEND System.Net_INCLUDE_DIRS
            "${CMAKE_SOURCE_DIR}/src/HAL/Include"
            "${CMAKE_SOURCE_DIR}/src/DeviceInterfaces/System.Net"
)
set(System.Net_SRCS
    sys_net_native.cpp  
    sys_net_native_System_Net_Sockets_NativeSocket.cpp
    sys_net_native_System_Net_IPAddress.cpp
    sys_net_native_System_Net_NetworkInformation_NetworkInterface.cpp
    sys_net_native_System_Net_NetworkInformation_IPGlobalProperties.cpp
    sys_net_native_System_Net_NetworkInformation_Wireless80211Configuration.cpp
    sys_net_native_System_Net_NetworkInformation_WirelessAPConfiguration.cpp
    sys_net_native_System_Security_Cryptography_X509Certificates_X509Certificate.cpp
    sys_net_native_System_Security_Cryptography_X509Certificates_X509Certificate2.cpp
    sys_net_native_System_Net_Security_SslNative.cpp
    sys_net_native_System_Net_Security_CertificateManager.cpp
)
foreach(SRC_FILE ${System.Net_SRCS})
    set(System.Net_SRC_FILE SRC_FILE-NOTFOUND)
    find_file(System.Net_SRC_FILE ${SRC_FILE} PATHS
              ${CMAKE_SOURCE_DIR}/src/DeviceInterfaces/System.Net
              CMAKE_FIND_ROOT_PATH_BOTH
    )
    list(APPEND System.Net_SOURCES ${System.Net_SRC_FILE})
endforeach()
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(System.Net DEFAULT_MSG System.Net_INCLUDE_DIRS System.Net_SOURCES)
