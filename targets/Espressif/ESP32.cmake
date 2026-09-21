#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#
list(APPEND ESP32_Sources
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/core/CoreIO.cpp
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/core/Device_BlockStorage.c
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/core/nanoCRT.cpp
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/core/nanoHAL.cpp
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/core/nanoSupport_CRC32.c
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/core/CLR_Startup_Thread.c
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/core/targetHAL.cpp

            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/core/platform_heap.c
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/core/targetHAL.c
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/core/targetHAL_Rtos.c
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/core/targetHAL_Time.cpp
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/core/targetPAL_Events.cpp
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/core/targetPAL_Timer.cpp
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/core/Target_BlockStorage_ESP32FlashDriver.c

            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/WireProtocol/WireProtocol_HAL_Interface.c
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/WireProtocol/WireProtocol_ReceiverThread.c

            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/${TARGET_SERIES}/Configuration/platform_BlockStorage.c
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/${TARGET_SERIES}/Configuration/target_BlockStorage.c
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/${TARGET_SERIES}/Configuration/target_common.c
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/${TARGET_SERIES}/Configuration/Memory.cpp


            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/${TARGET_SERIES}/targetHAL_Power.c
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/${TARGET_SERIES}/targetRandom.cpp
)
list(APPEND ESP32_Includes
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/Core
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/FileSystem
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/Network
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/WireProtocol
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/common/include
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/${TARGET_SERIES}/Configuration
            ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/${TARGET_SERIES}/Startup

            ${CMAKE_BINARY_DIR}/config

)
list(APPEND IDF_INCLUDES
            ${ESP32_IDF_PATH}/components/bt/include
            ${ESP32_IDF_PATH}/components/bt/common/include
            ${ESP32_IDF_PATH}/components/bt/host/nimble/nimble/include
            ${ESP32_IDF_PATH}/components/bt/host/nimble/nimble/nimble/controller/include
            ${ESP32_IDF_PATH}/components/bt/host/nimble/nimble/nimble/host/include
            ${ESP32_IDF_PATH}/components/bt/host/nimble/port/include
            ${ESP32_IDF_PATH}/components/esp_additions/include
            ${ESP32_IDF_PATH}/components/esp_adc/include
            ${ESP32_IDF_PATH}/components/esp_adc/interface
            ${ESP32_IDF_PATH}/components/esp_common/include
            ${ESP32_IDF_PATH}/components/esp_driver_gpio/include
            ${ESP32_IDF_PATH}/components/esp_hw_support/include

            ${ESP32_IDF_PATH}/components/esp_driver_i2c
            ${ESP32_IDF_PATH}/components/esp_driver_i2c/include
            ${ESP32_IDF_PATH}/components/esp_driver_i2s/include
            ${ESP32_IDF_PATH}/components/esp_driver_ledc/include
            ${ESP32_IDF_PATH}/components/esp_driver_spi/include
            ${ESP32_IDF_PATH}/components/esp_partition/include
            ${ESP32_IDF_PATH}/components/esp_driver_uart/include
            ${ESP32_IDF_PATH}/components/esp_hw_support/include
            ${ESP32_IDF_PATH}/components/esp_hw_support/dma/include
            ${ESP32_IDF_PATH}/components/esp_hw_support/include/soc
            ${ESP32_IDF_PATH}/components/esp_rom/include
            ${ESP32_IDF_PATH}/components/esp_system/include
            ${ESP32_IDF_PATH}/components/freertos/config/include
            ${ESP32_IDF_PATH}/components/freertos/config/riscv/include 
            ${ESP32_IDF_PATH}/components/freertos/esp_additions/include
            ${ESP32_IDF_PATH}/components/freertos/FreeRTOS-Kernel/include
            ${ESP32_IDF_PATH}/components/hal/include
            ${ESP32_IDF_PATH}/components/heap/include
            ${ESP32_IDF_PATH}/components/nvs_flash/include
            ${ESP32_IDF_PATH}/components/partition_table/include
            ${ESP32_IDF_PATH}/components/soc/include
            ${ESP32_IDF_PATH}/components/soc/${TARGET_SERIES}/include
            ${ESP32_IDF_PATH}/components/vfs/include
            ${ESP32_IDF_PATH}/components/hal/${TARGET_SERIES}/include
            ${ESP32_IDF_PATH}/components/soc/${TARGET_SERIES}/include
            ${ESP32_IDF_PATH}/components/esp_driver_i2c//include
)

target_sources(nanoCLR.elf PUBLIC ${ESP32_Sources} )
target_include_directories(nanoCLR.elf PUBLIC
                           ${ESP32_Includes}
                           ${IDF_INCLUDES}
)

set(SDKCONFIG_OVERRIDES_FILE ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/${TARGET_SERIES}/Configuration/sdkconfig.defaults.esp32p4 )

# Create a list of IDF components to add to the build based on the options selected in the CMakeLists.txt file
set( ADDITIONAL_IDF_COMPONENTS
      freertos
      esp_driver_uart
      esp_timer
      esp_event
      nvs_flash
      vfs
      # CoreIO
      esp_hw_support
      esp_adc
      esp_driver_gpio
#      esp_driver_i2c
      esp_driver_ledc
      esp_driver_spi
)

if( CONFIG_SUPPORT_INTERSOUND)
    list(APPEND ADDITIONAL_IDF_COMPONENTS
                esp_driver_i2s
    )
endif()

if(CONFIG_SUPPORT_BLUETOOTH)
# Bluetooth currently not building with hosted/remote config
    file(APPEND ${SDKCONFIG_OVERRIDES_FILE} "CONFIG_BT_ENABLED=y\n")
    file(APPEND ${SDKCONFIG_OVERRIDES_FILE} "CONFIG_BT_NIMBLE_ENABLED=y\n")
    file(APPEND ${SDKCONFIG_OVERRIDES_FILE} "CONFIG_BT_NIMBLE_SVC_GAP_DEVICE_NAME=\"nanoBLE\"\n")
    file(APPEND ${SDKCONFIG_OVERRIDES_FILE} "CONFIG_BT_NIMBLE_TRANSPORT_ACL_FROM_LL_COUNT=10\n")
    file(APPEND ${SDKCONFIG_OVERRIDES_FILE} "CONFIG_BT_NIMBLE_TRANSPORT_EVT_COUNT=20\n")

    list(APPEND ADDITIONAL_IDF_COMPONENTS
                bt
    )
endif()

if(CONFIG_SUPPORT_GRAPHICS)
    list(APPEND ADDITIONAL_IDF_COMPONENTS
                esp_lcd
                esp_driver_ppa
    )
endif()

if(CONFIG_SUPPORT_THREAD)
    list(APPEND ADDITIONAL_IDF_COMPONENTS
                openthread
    )
endif()

if(CONFIG_SUPPORT_FILESYSTEM)
    list(APPEND ADDITIONAL_IDF_COMPONENTS
                littlefs
                fatfs
    )
endif()

if(CONFIG_SUPPORT_USBSTREAM)
    list(APPEND ADDITIONAL_IDF_COMPONENTS
                tinyusb
                esp_tinyusb
    )
endif()

if(CONFIG_SUPPORT_WIFI)
    list(APPEND ADDITIONAL_IDF_COMPONENTS
                esp_hosted
                esp_wifi_remote
                esp_wifi
                esp_netif
                esp_event
                lwip
    )
endif()

# Let the IDF build system handle the build of the IDF core and components used by nanoCLR
include(${ESP32_IDF_PATH}/tools/cmake/idf.cmake)
idf_build_process(esp32p4
                  COMPONENTS 
                     ${ADDITIONAL_IDF_COMPONENTS}
                  SDKCONFIG_DEFAULTS
                    ${SDKCONFIG_OVERRIDES_FILE}
                  PROJECT_NAME
                     "nanoCLR.elf"
                  PROJECT_VER
                       ${BUILD_VERSION}
                  PROJECT_DIR
                       ${CMAKE_SOURCE_DIR}

)
foreach(component ${ADDITIONAL_IDF_COMPONENTS})
     list(APPEND IDF_LIBRARIES_TO_ADD
                 idf::${component}
     )
endforeach()
target_link_libraries(nanoCLR.elf ${IDF_LIBRARIES_TO_ADD})
target_link_options(nanoCLR.elf PUBLIC "-Wl,--no-warn-rwx-segments")
idf_build_executable(nanoCLR.elf)

# Configuration
configure_file(${CMAKE_SOURCE_DIR}/targets/${VENDOR}/${TARGET_SERIES}/Configuration/target_os.h.in       ${CMAKE_BINARY_DIR}/target_os.h @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/targets/${VENDOR}/${TARGET_SERIES}/Configuration/target_platform.h.in ${CMAKE_BINARY_DIR}/target_platform.h @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/targets/${VENDOR}/${TARGET_SERIES}/Configuration/target_common.h.in   ${CMAKE_BINARY_DIR}/target_common.h @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/targets/${VENDOR}/${TARGET_SERIES}/Configuration/target_board.h.in    ${CMAKE_BINARY_DIR}/target_board.h @ONLY)

add_custom_target(generate_partition ALL
                  COMMAND
                  "python"
                  ${ESP32_IDF_PATH}/components/partition_table/gen_esp32part.py 
                  --flash-size  ${FLASH_SIZE} 
                  ${CMAKE_SOURCE_DIR}/targets/${VENDOR}/${TARGET_SERIES}/Configuration/partition_${FLASH_SIZE}.csv
                  ${CMAKE_BINARY_DIR}/partitions_${FLASH_SIZE}.bin
                  COMMENT
                  "Generate selected partition table size of  flash"
)

#   # if needed, "fix" the reported version so it doesn't show '-dirty'
#   # this is because we could be deleting some files and tweaking others in the IDF
#
#   get_property(MY_IDF_VER TARGET __idf_build_target PROPERTY IDF_VER)
#   string(FIND ${MY_IDF_VER} "-dirty" MY_IDF_VER_DIRTY)
#   if(${MY_IDF_VER_DIRTY} GREATER -1)
#
#       # found '-dirty' in the version string
#       string(REPLACE "-dirty" "" MY_IDF_VER_FIXED "${MY_IDF_VER}")
#       set_property(TARGET __idf_build_target PROPERTY IDF_VER ${MY_IDF_VER_FIXED})
#       set(IDF_VER_FIXED ${MY_IDF_VER_FIXED} CACHE INTERNAL "IDF version as CMake var")
#
#       # for COMPILE DEFINITIONS it's a bit more work
#       get_property(IDF_COMPILE_DEFINITIONS TARGET __idf_build_target PROPERTY COMPILE_DEFINITIONS )
#
#       string(REPLACE "-dirty" "" IDF_COMPILE_DEFINITIONS_FIXED "${IDF_COMPILE_DEFINITIONS}")
#       set_property(TARGET __idf_build_target PROPERTY COMPILE_DEFINITIONS ${IDF_COMPILE_DEFINITIONS_FIXED})
#       
#       message(STATUS "Fixed IDF version. Is now: ${MY_IDF_VER_FIXED}")
#   endif()
