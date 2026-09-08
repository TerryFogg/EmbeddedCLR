#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

list(APPEND CoreIO_Sources
#     ${CMAKE_SOURCE_DIR}/src/System.Device.Adc/sys_dev_adc_native.cpp
#     ${CMAKE_SOURCE_DIR}/targets/ESP32/ESP32_P4/_nanoCLR/System.Device.Adc/sys_dev_adc_native_System_Device_Adc_AdcChannel.cpp
#     ${CMAKE_SOURCE_DIR}/targets/ESP32/ESP32_P4/_nanoCLR/System.Device.Adc/sys_dev_adc_native_System_Device_Adc_AdcController.cpp
#     ${CMAKE_SOURCE_DIR}/targets/ESP32/ESP32_P4/target_system_device_adc_config.cpp
#
#     ${CMAKE_SOURCE_DIR}/src/System.Device.Dac/sys_dev_dac_native.cpp
#     ${CMAKE_SOURCE_DIR}/targets/ESP32/ESP32_P4/_nanoCLR/System.Device.Dac/sys_dev_dac_native_System_Device_Dac_DacController.cpp
#     ${CMAKE_SOURCE_DIR}/targets/ESP32/ESP32_P4/_nanoCLR/System.Device.Dac/sys_dev_dac_native_System_Device_Dac_DacChannel.cpp
#     ${CMAKE_SOURCE_DIR}/targets/ESP32/ESP32_P4/target_system_device_dac_config.cpp
     
#     ${CMAKE_SOURCE_DIR}/src/System.Device.Gpio/sys_dev_gpio_native.cpp
#     ${CMAKE_SOURCE_DIR}/src/System.Device.Gpio/sys_dev_gpio_native_System_Device_Gpio_GpioController.cpp
#     ${CMAKE_SOURCE_DIR}/src/System.Device.Gpio/sys_dev_gpio_native_System_Device_Gpio_GpioPin.cpp
#     ${CMAKE_SOURCE_DIR}/targets/ESP32/ESP32_P4/_nanoCLR/System.Device.Gpio/cpu_gpio.cpp
     
#     ${CMAKE_SOURCE_DIR}/src/System.Device.I2c/sys_dev_i2c_native.cpp
#     ${CMAKE_SOURCE_DIR}/targets/ESP32/ESP32_P4/_nanoCLR/System.Device.I2c/sys_dev_i2c_native_System_Device_I2c_I2cDevice.cpp
#     
#     ${CMAKE_SOURCE_DIR}/src/System.Device.I2c.Slave/sys_dev_i2c_slave_native.cpp
#     ${CMAKE_SOURCE_DIR}/targets/ESP32/ESP32_P4/_nanoCLR/System.Device.I2c.Slave/sys_dev_i2c_slave_native_System_Device_I2c_I2cSlaveDevice.cpp
#     ${CMAKE_SOURCE_DIR}/src/System.Device.I2s.Slave/sys_dev_i2s_native.cpp
#     ${CMAKE_SOURCE_DIR}/targets/ESP32/ESP32_P4/_nanoCLR/System.Device.I2s.Slave/sys_dev_i2s_native_System_Device_I2s_I2sDevice.cpp
#     
#     ${CMAKE_SOURCE_DIR}/src/System.Device.Pwm/sys_dev_pwm_native.cpp
#     ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/System.Device.Pwm/sys_dev_pwm_native_System_Device_Pwm_PwmChannel.cpp
     
#     ${CMAKE_SOURCE_DIR}/src/System.Device.Spi/nanoHAL_Spi.cpp
#     ${CMAKE_SOURCE_DIR}/src/System.Device.Spi/sys_dev_spi_native.cpp
#     ${CMAKE_SOURCE_DIR}/src/System.Device.Spi/sys_dev_spi_native_System_Device_Spi_SpiBusInfo.cpp
#     ${CMAKE_SOURCE_DIR}/src/System.Device.Spi/sys_dev_spi_native_System_Device_Spi_SpiDevice.cpp
#     ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/System.Device.Spi/cpu_spi.cpp
     
#     ${CMAKE_SOURCE_DIR}/src/System.IO.Portssys_io_ser_native.cpp
#     ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/System.IO.Ports/sys_io_ser_native_System_IO_Ports_SerialPort.cpp
#     ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/System.IO.Ports/sys_io_ser_native_System_IO_Ports_SerialPort__.cpp
)

list(APPEND CoreIO_Includes
            ${CMAKE_SOURCE_DIR}/src/System.Device.Adc
            ${CMAKE_SOURCE_DIR}/src/System.Device.Dac
            ${CMAKE_SOURCE_DIR}/src/System.Device.Gpio
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/System.Device.Gpio
            ${CMAKE_SOURCE_DIR}/src/System.Device.I2c
            ${CMAKE_SOURCE_DIR}/src/System.Device.I2c.Slave
            ${CMAKE_SOURCE_DIR}/src/System.Device.I2s
            ${CMAKE_SOURCE_DIR}/src/System.Device.Pwm
            ${CMAKE_SOURCE_DIR}/src/System.Device.Spi
            ${CMAKE_SOURCE_DIR}/src/System.IO.Ports

            ${CMAKE_SOURCE_DIR}/targets/ESP32/ESP32_P4
)

target_sources(nanoCLR PUBLIC ${I2C_Sources} )
target_include_directories(nanoCLR PUBLIC  ${I2C_Includes} )   
