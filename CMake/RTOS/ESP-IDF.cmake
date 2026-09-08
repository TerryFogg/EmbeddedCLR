#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

include(binutils.common)
include(FetchContent)

macro(nf_install_idf_component_from_registry component_name object_id)
    set(downloadUrl https://components.espressif.com/api/downloads/?object_type=component&object_id=${object_id})
    set(archiveName ${CMAKE_BINARY_DIR}/downloads/${component_name}_${object_id}.zip)
    set(destinationPath ${IDF_PATH_CMAKED}/components/${component_name})
    set(extractPath ${IDF_PATH_CMAKED}/components)
    if(NOT EXISTS ${destinationPath})
        file(DOWNLOAD ${downloadUrl} ${archiveName})
        message(STATUS "Component archive '" ${component_name} "' downloaded")
        file(ARCHIVE_EXTRACT 
            INPUT ${archiveName} 
            DESTINATION ${extractPath}
        )
        # Remove idf_component.yml file otherwise we will get warning about Component manager not being enabled
        file(REMOVE ${destinationPath}/idf_component.yml)
        message(STATUS "'" ${component_name} "' installed in IDF component directory > " components/${component_name})
    endif()
endmacro()

# macro to add IDF as a library to the build and add the IDF components according to variant and options
macro(nf_add_idf_as_library)
    nf_install_idf_component_from_registry(littlefs 97bf51ce-1daa-4369-81ec-eacbd8102815) 
    nf_install_idf_component_from_registry(esp_wifi_remote c90c182f-b7fc-4a59-a445-96f712e36bb2)
    nf_install_idf_component_from_registry(esp_hosted 2c2bb417-ac4a-415a-8bd8-d2437701bb5e)
    include(${IDF_PATH_CMAKED}/tools/cmake/idf.cmake)
    # if needed, "fix" the reported version so it doesn't show '-dirty'
    # this is because we could be deleting some files and tweaking others in the IDF
    get_property(MY_IDF_VER TARGET __idf_build_target PROPERTY IDF_VER)

    string(FIND ${MY_IDF_VER} "-dirty" MY_IDF_VER_DIRTY)
    if(${MY_IDF_VER_DIRTY} GREATER -1)

        # found '-dirty' in the version string
        string(REPLACE "-dirty" "" MY_IDF_VER_FIXED "${MY_IDF_VER}")
        set_property(TARGET __idf_build_target PROPERTY IDF_VER ${MY_IDF_VER_FIXED})
        set(IDF_VER_FIXED ${MY_IDF_VER_FIXED} CACHE INTERNAL "IDF version as CMake var")

        # for COMPILE DEFINITIONS it's a bit more work
        get_property(IDF_COMPILE_DEFINITIONS TARGET __idf_build_target PROPERTY COMPILE_DEFINITIONS )

        string(REPLACE "-dirty" "" IDF_COMPILE_DEFINITIONS_FIXED "${IDF_COMPILE_DEFINITIONS}")
        set_property(TARGET __idf_build_target PROPERTY COMPILE_DEFINITIONS ${IDF_COMPILE_DEFINITIONS_FIXED})
        
        message(STATUS "Fixed IDF version. Is now: ${MY_IDF_VER_FIXED}")
    endif()

    set(SDKCONFIG_DEFAULTS_FILE ${CMAKE_SOURCE_DIR}/targets/Espressif/ESP32P4/Configuration/sdkconfig.default.rev.less3.esp32p4)

    file(READ
        "${SDKCONFIG_DEFAULTS_FILE}"
        SDKCONFIG_ORIGINAL_CONTENTS
    )

    # Make temporary copy of sdkconfig.defaults.? file into build dir as we are going to make changes
    set(SDKCONFIG_DEFAULTS_TEMP_FILE ${CMAKE_SOURCE_DIR}/build/sdkconfig.default)
    file(WRITE ${SDKCONFIG_DEFAULTS_TEMP_FILE} ${SDKCONFIG_ORIGINAL_CONTENTS})

    # set list with the IDF components to add
    # need to match the list below with the respective libraries
    set(IDF_COMPONENTS_TO_ADD
        lwip
        freertos
        esptool_py
        fatfs
        esp_event
        vfs
        esp_netif
        esp_eth
        esp_psram
        esp_adc
        littlefs
        esp_lcd
        esp_driver_ppa
        nvs_flash
        esp_wifi
        esp_timer
    )

    # set list with the libraries for IDF components added
    # need to match the list above with the IDF components
    set(IDF_LIBRARIES_TO_ADD
        idf::lwip
        idf::freertos
        idf::esptool_py
        idf::fatfs
        idf::esp_event
        idf::vfs
        idf::esp_netif
        idf::esp_eth
        idf::esp_psram
        idf::esp_adc
        idf::littlefs
        idf::esp_lcd
        idf::esp_driver_ppa
        idf::nvs_flash
        idf::esp_wifi
        idf::esp_timer

)


    # Needed for remote Wifi module on P4 boards
    list(APPEND IDF_COMPONENTS_TO_ADD esp_wifi_remote)
    list(APPEND IDF_COMPONENTS_TO_ADD esp_hosted)
    list(APPEND IDF_LIBRARIES_TO_ADD idf::esp_hosted)
    list(APPEND IDF_LIBRARIES_TO_ADD idf::esp_wifi_remote)

    if(HAL_USE_BLE_OPTION)
        list(APPEND IDF_COMPONENTS_TO_ADD bt)
        list(APPEND IDF_LIBRARIES_TO_ADD idf::bt)
    endif()

    if(ESP32_ETHERNET_SUPPORT)
        list(APPEND IDF_COMPONENTS_TO_ADD esp_eth)
        list(APPEND IDF_LIBRARIES_TO_ADD idf::esp_eth)
    endif()

    if(HAL_USE_THREAD_OPTION)
        list(APPEND IDF_COMPONENTS_TO_ADD openthread)
        list(APPEND IDF_LIBRARIES_TO_ADD idf::openthread)
    endif()

    option(HAL_USE_THREAD_OPTION "option to enable OpenThread support")
    option(ESP32_THREAD_DEVICE_TYPE "option to specify OpenThread device type (FTD or MTD")

    if(HAL_USE_THREAD_OPTION)
        message(DEBUG "Reading SDK config from '${SDKCONFIG_DEFAULTS_FILE}' to set Thread options")

        file(READ
            "${SDKCONFIG_DEFAULTS_TEMP_FILE}"
            SDKCONFIG_DEFAULT_CONTENTS)

        # Append config based on options
        string(APPEND SDKCONFIG_DEFAULT_CONTENTS "\nCONFIG_OPENTHREAD_ENABLED=y\n")
        string(APPEND SDKCONFIG_DEFAULT_CONTENTS "\nCONFIG_OPENTHREAD_CLI=y\n")
        string(APPEND SDKCONFIG_DEFAULT_CONTENTS "CONFIG_OPENTHREAD_LOG_LEVEL_DYNAMIC=y\n")
        string(APPEND SDKCONFIG_DEFAULT_CONTENTS "CONFIG_OPENTHREAD_JOINER=y\n")
        
        # make sure these options are enabled for openthread & mbedtls
        string(APPEND SDKCONFIG_DEFAULT_CONTENTS "CONFIG_MBEDTLS_CMAC_C=y\n")
        string(APPEND SDKCONFIG_DEFAULT_CONTENTS "CONFIG_MBEDTLS_SSL_PROTO_DTLS=y\n")
        string(APPEND SDKCONFIG_DEFAULT_CONTENTS "CONFIG_MBEDTLS_KEY_EXCHANGE_ECJPAKE=y\n")
        string(APPEND SDKCONFIG_DEFAULT_CONTENTS "CONFIG_MBEDTLS_ECJPAKE_C=y\n")
        
        # ESP32_THREAD_DEVICE_TYPE
        set(ESP32_THREAD_DEVICE_TYPE_SUPPORTED "FTD" "MTD" CACHE INTERNAL "supported THREAD device types")
        list(FIND ESP32_THREAD_DEVICE_TYPE_SUPPORTED ${ESP32_THREAD_DEVICE_TYPE} ESP32_THREAD_DEVICE_TYPE_INDEX)

        if(ESP32_THREAD_DEVICE_TYPE_INDEX EQUAL -1)
            # Default FTD if not specified
            set(ESP32_THREAD_DEVICE_TYPE_INDEX 0)
        endif()
        
        if (${ESP32_THREAD_DEVICE_TYPE_INDEX} EQUAL 0)
            string(APPEND SDKCONFIG_DEFAULT_CONTENTS "CONFIG_OPENTHREAD_FTD=y\n")
            message(STATUS "OpenThread configured as full thread device (FTD)")
        else()
            string(APPEND SDKCONFIG_DEFAULT_CONTENTS "CONFIG_OPENTHREAD_MTD=y\n")
            message(STATUS "OpenThread configured as a minimal thread device (MTD)")
        endif()

        # need to temporarilly allow changes in source files
        set(CMAKE_DISABLE_SOURCE_CHANGES OFF)

        file(WRITE 
            ${SDKCONFIG_DEFAULTS_TEMP_FILE} 
            ${SDKCONFIG_DEFAULT_CONTENTS})

        set(CMAKE_DISABLE_SOURCE_CHANGES ON)
    endif()

    # Fixed default frequency will be used)

    # create IDF static libraries
    idf_build_process(esp32p4 COMPONENTS 
                      ${IDF_COMPONENTS_TO_ADD}
                      SDKCONFIG_DEFAULTS
                      ${SDKCONFIG_DEFAULTS_TEMP_FILE}
                      PROJECT_NAME "nanoCLR"
                      PROJECT_VER ${BUILD_VERSION}
                      PROJECT_DIR ${CMAKE_SOURCE_DIR}
    )

    # Link the static libraries to the executable
    target_link_libraries(nanoCLR  ${IDF_LIBRARIES_TO_ADD} )
    file(READ  ${CMAKE_SOURCE_DIR}/sdkconfig  SDKCONFIG_DEFAULT_CONTENTS)
    set(SPIRAM_SUPPORT_PRESENT -1)
    string(FIND ${SDKCONFIG_DEFAULT_CONTENTS} "CONFIG_SPIRAM=y" SPIRAM_SUPPORT_PRESENT)
    
    # find out revision info for any target series
    unset(ESP32_REVISION)
    string(TOUPPER CONFIG_esp32p4_REV_MIN_ CONFIG_ESP32X_REV_MIN)
    string(FIND ${SDKCONFIG_DEFAULT_CONTENTS} ${CONFIG_ESP32X_REV_MIN}0=y CONFIG_ESP32X_REV_MIN_0_POS)
    string(FIND ${SDKCONFIG_DEFAULT_CONTENTS} ${CONFIG_ESP32X_REV_MIN}1=y CONFIG_ESP32X_REV_MIN_1_POS)
    string(FIND ${SDKCONFIG_DEFAULT_CONTENTS} ${CONFIG_ESP32X_REV_MIN}2=y CONFIG_ESP32X_REV_MIN_2_POS)
    string(FIND ${SDKCONFIG_DEFAULT_CONTENTS} ${CONFIG_ESP32X_REV_MIN}3=y CONFIG_ESP32X_REV_MIN_3_POS)
    string(FIND ${SDKCONFIG_DEFAULT_CONTENTS} ${CONFIG_ESP32X_REV_MIN}4=y CONFIG_ESP32X_REV_MIN_4_POS)

    # set variable
    if(${CONFIG_ESP32X_REV_MIN_0_POS} GREATER -1)
        set(REVISION_INFO ", chip rev. >= 0")
        message(STATUS "Building for chip revision >= 0")
        set(ESP32_REVISION "0" CACHE STRING "ESP32 revision")
    elseif(${CONFIG_ESP32X_REV_MIN_1_POS} GREATER -1)
        set(REVISION_INFO ", chip rev. >= 1")
        message(STATUS "Building for chip revision >= 1")
        set(ESP32_REVISION "1" CACHE STRING "ESP32 revision")
    elseif(${CONFIG_ESP32X_REV_MIN_2_POS} GREATER -1)
        set(REVISION_INFO ", chip rev. >= 2")
        message(STATUS "Building for chip revision >= 2")
        set(ESP32_REVISION "2" CACHE STRING "ESP32 revision")
    elseif(${CONFIG_ESP32X_REV_MIN_3_POS} GREATER -1)
        set(REVISION_INFO ", chip rev. >= 3")
        message(STATUS "Building for chip revision >= 3")
        set(ESP32_REVISION "3" CACHE STRING "ESP32 revision")
    elseif(${CONFIG_ESP32X_REV_MIN_4_POS} GREATER -1)
        set(REVISION_INFO ", chip rev. 4")
        message(STATUS "Building for chip revision 4")
        set(ESP32_REVISION "4" CACHE STRING "ESP32 revision")
    endif()


endmacro()
