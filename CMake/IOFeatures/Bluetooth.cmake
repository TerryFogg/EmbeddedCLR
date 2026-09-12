#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

SET(

list(APPEND Bluetooth_Sources
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Device.Bluetoothsys_dev_ble_native.cpp

    # Server
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Device.Bluetooth/sys_dev_ble_native_nanoFramework_Device_Bluetooth_GenericAttributeProfile_GattServiceProvider.cpp
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Device.Bluetooth/sys_dev_ble_native_nanoFramework_Device_Bluetooth_GenericAttributeProfile_GattReadRequest.cpp
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Device.Bluetooth/sys_dev_ble_native_nanoFramework_Device_Bluetooth_GenericAttributeProfile_GattWriteRequest.cpp
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Device.Bluetooth/sys_dev_ble_native_nanoFramework_Device_Bluetooth_GenericAttributeProfile_GattLocalCharacteristic.cpp
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Device.Bluetooth/sys_dev_ble_native_nanoFramework_Device_Bluetooth_BluetoothLEAdvertisementWatcher.cpp
 
    # Client / Central
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Device.Bluetooth/sys_dev_ble_native_nanoFramework_Device_Bluetooth_BluetoothLEDevice.cpp
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Device.Bluetooth/sys_dev_ble_native_nanoFramework_Device_Bluetooth_BluetoothNanoDevice.cpp

    # Others
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Device.Bluetooth/sys_dev_ble_native_nanoFramework_Device_Bluetooth_Security.cpp
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Device.Bluetooth/esp32_nimble.cpp
    ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Device.Bluetooth/nimble_utils.cpp
)

list(APPEND Bluetooth_Includes
            ${CMAKE_SOURCE_DIR}/src/CLR/Core)
            ${CMAKE_SOURCE_DIR}/src/CLR/Include
            ${CMAKE_SOURCE_DIR}/src/HAL/Include
            ${CMAKE_SOURCE_DIR}/src/PAL/Include
            ${BASE_PATH_FOR_THIS_MODULE}
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Device.Bluetooth
            ${NIMBLE_COMPONENT_PATH}/port/include
            ${NIMBLE_COMPONENT_PATH}/nimble/nimble/include
            ${NIMBLE_COMPONENT_PATH}/esp-hci/include
            ${NIMBLE_COMPONENT_PATH}/nimble/porting/nimble/include
            ${NIMBLE_COMPONENT_PATH}/nimble/porting/npl/freertos/include
            ${NIMBLE_COMPONENT_PATH}/nimble/nimble/host/include
            ${NIMBLE_COMPONENT_PATH}/nimble/nimble/host/util/include
            ${NIMBLE_COMPONENT_PATH}/nimble/nimble/host/services/gap/include
            ${NIMBLE_COMPONENT_PATH}/nimble/nimble/host/services/gatt/include
            ${NIMBLE_COMPONENT_PATH}/nimble/nimble/transport/include
)

target_sources(nanoCLR PUBLIC ${Bluetooth_Sources} )
target_include_directories(nanoCLR PUBLIC  ${Bluetooth_Includes} )   
