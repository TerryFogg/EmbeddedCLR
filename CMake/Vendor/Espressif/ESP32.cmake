#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#
include(binutils.common)
include(FetchContent)


list(APPEND ESP32_Sources
            ${CMAKE_SOURCE_DIR}/targets/Espressif/common/core/Device_BlockStorage.c
            ${CMAKE_SOURCE_DIR}/targets/Espressif/common/core/nanoCRT.cpp
            ${CMAKE_SOURCE_DIR}/targets/Espressif/common/core/nanoHAL.cpp
            ${CMAKE_SOURCE_DIR}/targets/Espressif/common/core/nanoSupport_CRC32.c

            ${CMAKE_SOURCE_DIR}/targets/Espressif/common/core/platform_heap.c
            ${CMAKE_SOURCE_DIR}/targets/Espressif/common/core/targetHAL.c
            ${CMAKE_SOURCE_DIR}/targets/Espressif/common/core/targetHAL_Rtos.c
            ${CMAKE_SOURCE_DIR}/targets/Espressif/common/core/targetHAL_StorageOperation.cpp
            ${CMAKE_SOURCE_DIR}/targets/Espressif/common/core/targetHAL_Time.cpp
            ${CMAKE_SOURCE_DIR}/targets/Espressif/common/core/targetPAL.c 
            ${CMAKE_SOURCE_DIR}/targets/Espressif/common/core/targetPAL_Events.cpp
            ${CMAKE_SOURCE_DIR}/targets/Espressif/common/core/targetPAL_Time.cpp
            ${CMAKE_SOURCE_DIR}/targets/Espressif/common/core/Target_BlockStorage_ESP32FlashDriver.c

            ${CMAKE_SOURCE_DIR}/targets/Espressif/common/WireProtocol/WireProtocol_HAL_Interface.c
            ${CMAKE_SOURCE_DIR}/targets/Espressif/common/WireProtocol/WireProtocol_ReceiverThread.c

            ${CMAKE_SOURCE_DIR}/targets/Espressif/${TARGET_SERIES}/Configuration/platform_BlockStorage.c
            ${CMAKE_SOURCE_DIR}/targets/Espressif/${TARGET_SERIES}/Configuration/target_BlockStorage.c
            ${CMAKE_SOURCE_DIR}/targets/Espressif/${TARGET_SERIES}/Configuration/target_common.c
            ${CMAKE_SOURCE_DIR}/targets/Espressif/${TARGET_SERIES}/Configuration/Memory.cpp

            ${CMAKE_SOURCE_DIR}/targets/Espressif/${TARGET_SERIES}/Startup/CLR_Startup_Thread.c
            ${CMAKE_SOURCE_DIR}/targets/Espressif/${TARGET_SERIES}/Startup/targetHAL.cpp

            ${CMAKE_SOURCE_DIR}/targets/Espressif/${TARGET_SERIES}/targetHAL_Power.c
            ${CMAKE_SOURCE_DIR}/targets/Espressif/${TARGET_SERIES}/targetRandom.cpp

)
list(APPEND ESP32_Includes
            ${CMAKE_BINARY_DIR}/targets/ESP32/${TARGET_SERIES}
            ${CMAKE_BINARY_DIR}/targets/ESP32/${TARGET_SERIES}/nanoCLR
            ${CMAKE_SOURCE_DIR}/targets/Espressif/${TARGET_SERIES}/Configuration
            ${CMAKE_SOURCE_DIR}/targets/Espressif/Adaption
            ${CMAKE_SOURCE_DIR}/targets/Espressif/common/core
            ${CMAKE_SOURCE_DIR}/targets/Espressif/FileSystem
            ${CMAKE_SOURCE_DIR}/targets/Espressif/Network
            ${CMAKE_SOURCE_DIR}/targets/Espressif/common/include
            ${CMAKE_BINARY_DIR}/targets/ESP32/

)

target_sources(nanoCLR PUBLIC ${ESP32_Sources} )
target_include_directories(nanoCLR PUBLIC ${ESP32_Includes} )



set(ESP32_RESERVE_IRAM_IDF_ALLOCATION_KB "0" CACHE STRING "Setting default value for ESP32_RESERVE_IRAM_IDF_ALLOCATION_KB")
set(ESP32_RESERVE_SPIRAM_IDF_ALLOCATION_BYTES "0" CACHE STRING "Setting default value for ESP32_RESERVE_SPIRAM_IDF_ALLOCATION_BYTES")



macro(nf_install_idf_component_from_registry component_name object_id)
    set(downloadUrl https://components.espressif.com/api/downloads/?object_type=component&object_id=${object_id})
    set(archiveName ${CMAKE_BINARY_DIR}/downloads/${component_name}_${object_id}.zip)
    set(destinationPath ${ESP32_IDF_PATH}/components/${component_name})
    set(extractPath ${ESP32_IDF_PATH}/components)
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
#macro(nf_add_idf_as_library)
    nf_install_idf_component_from_registry(littlefs 97bf51ce-1daa-4369-81ec-eacbd8102815) 
    nf_install_idf_component_from_registry(esp_wifi_remote c90c182f-b7fc-4a59-a445-96f712e36bb2)
    nf_install_idf_component_from_registry(esp_hosted 2c2bb417-ac4a-415a-8bd8-d2437701bb5e)
    include(${ESP32_IDF_PATH}/tools/cmake/idf.cmake)
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

    set(SDKCONFIG_DEFAULTS_FILE ${CMAKE_SOURCE_DIR}/targets/Espressif/${TARGET_SERIES}/Configuration/sdkconfig.default.rev.less3.${TARGET_SERIES})

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
        esp_event
        vfs
        esp_psram
        esp_adc
        esp_lcd
        esp_driver_ppa
        nvs_flash
        esp_timer
    )

    # set list with the libraries for IDF components added
    # need to match the list above with the IDF components
    set(IDF_LIBRARIES_TO_ADD
        idf::lwip
        idf::freertos
        idf::esptool_py
        idf::esp_event
        idf::vfs
        idf::esp_psram
        idf::esp_adc
        idf::esp_lcd
        idf::esp_driver_ppa
        idf::nvs_flash
        idf::esp_timer

)

    if(HAL_USE_BLE_OPTION)
        list(APPEND IDF_COMPONENTS_TO_ADD bt)
        list(APPEND IDF_LIBRARIES_TO_ADD idf::bt)
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


#endmacro()

    set(PythonCommand "python" ${ESP32_IDF_PATH}/tools/idf_size.py)
      #COMMAND ${PythonCommand} --archives ${CMAKE_BINARY_DIR}/nanoCLR.map COMMENT "Output IDF size summary"










# Add the linker option to ESP32 build to suppress warnings about read/write/execute segments
target_link_options(nanoCLR PUBLIC "-Wl,--no-warn-rwx-segments")

configure_file(${CMAKE_SOURCE_DIR}/targets/Espressif/${TARGET_SERIES}/Configuration/target_os.h.in       ${CMAKE_BINARY_DIR}/target_os.h @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/targets/Espressif/${TARGET_SERIES}/Configuration/target_platform.h.in ${CMAKE_BINARY_DIR}/target_platform.h @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/targets/Espressif/${TARGET_SERIES}/Configuration/target_common.h.in   ${CMAKE_BINARY_DIR}/target_common.h @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/targets/Espressif/${TARGET_SERIES}/Configuration/target_board.h.in    ${CMAKE_BINARY_DIR}/target_board.h @ONLY)

set(gen_partition_table "python" "${ESP32_IDF_PATH}/components/partition_table/gen_esp32part.py")
add_custom_target(generate_partition ALL
    COMMAND ${gen_partition_table} 
    --flash-size ${PARTITION_SIZE} 
    "${CMAKE_SOURCE_DIR}/targets/Espressif/${TARGET_SERIES}/Configuration/partitions_nanoclr_${PARTITION_SIZE}.csv"
    "${CMAKE_BINARY_DIR}/partitions_${PARTITION_SIZE}.bin"
    COMMENT "Generate selected partition table size of  flash" )


    idf_build_executable(nanoCLR)
