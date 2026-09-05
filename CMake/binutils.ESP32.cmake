#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

include(binutils.common)

macro(nf_find_esp32_files_at_location files locations)
    foreach(SRC_FILE ${files})
        set(IDF_SRC_FILE SRC_FILE -NOTFOUND)
        find_file(IDF_SRC_FILE ${SRC_FILE}
            PATHS 
            ${locations}
            CMAKE_FIND_ROOT_PATH_BOTH
        )
        list(APPEND ESP32_IDF_SOURCES ${IDF_SRC_FILE})
    endforeach()
endmacro()

function(nf_set_optimization_options target) 
    target_compile_options(${target} PRIVATE
        $<$<CONFIG:Debug>:-Og -g>
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:MinSizeRel>:-Os>
        $<$<CONFIG:RelWithDebInfo>:-Os -g>
    )
endfunction()
function(nf_set_linker_file target linker_file_name)
endfunction()
macro(nf_set_compile_definitions)
    cmake_parse_arguments(NFSCD "" "TARGET" "EXTRA_COMPILE_DEFINITIONS;BUILD_TARGET" ${ARGN})
    if(NOT NFSCD_TARGET OR "${NFSCD_TARGET}" STREQUAL "")
        message(FATAL_ERROR "Need to set TARGET argument when calling nf_set_compile_definitions()")
    endif()
     target_compile_definitions(nanoCLR.elf PUBLIC ${NFSCD_EXTRA_COMPILE_DEFINITIONS})
endmacro()

macro(nf_add_platform_packages)
    cmake_parse_arguments(NFAPP "" "TARGET" "" ${ARGN})
    find_package(ESP32_IDF REQUIRED QUIET)
    if("${NFAPP_TARGET}" STREQUAL "${NANOCLR_PROJECT_NAME}")
        if(USE_NETWORKING_OPTION)
            find_package(NF_Network REQUIRED QUIET)
        endif()
    endif()
endmacro()
macro(nf_add_platform_dependencies target)
    configure_file(${BASE_PATH_FOR_CLASS_LIBRARIES_MODULES}/target_platform.h.in
                   ${CMAKE_BINARY_DIR}/targets/ESP32/ESP32_P4/target_platform.h @ONLY)

    nf_add_lib_coreclr(
        EXTRA_INCLUDES
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}/${target}
            ${CMAKE_CURRENT_SOURCE_DIR}/${target}
            ${CMAKE_CURRENT_SOURCE_DIR}/Include
            ${CMAKE_CURRENT_SOURCE_DIR}/Network
            ${CMAKE_BINARY_DIR}/targets/ESP32
            ${ESP32_IDF_INCLUDE_DIRS}
            ${TARGET_ESP32_IDF_INCLUDES})

    add_dependencies(${target}.elf nano::NF_CoreCLR)

    
    if(NF_FEATURE_DEBUGGER)
        add_wireprotocol()
        add_debugger()
    endif()

    nf_add_lib_native_assemblies(
        EXTRA_INCLUDES
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_BINARY_DIR}/targets/ESP32
            ${ESP32_IDF_INCLUDE_DIRS}
            ${TARGET_ESP32_IDF_INCLUDES})
    
    add_dependencies(${target}.elf nano::NF_NativeAssemblies)
  

endmacro()

macro(nf_add_platform_include_directories target)

    FetchContent_GetProperties(esp32_idf)

    target_include_directories(${target}.elf PUBLIC

        ${TARGET_ESP32_IDF_COMMON_INCLUDE_DIRS}
        ${CMAKE_BINARY_DIR}/targets/ESP32
        ${ESP32_IDF_INCLUDE_DIRS}
        ${NF_NativeAssemblies_INCLUDE_DIRS}
        ${NF_CoreCLR_INCLUDE_DIRS}
    )
    if(${target} STREQUAL ${NANOCLR_PROJECT_NAME})

        target_include_directories(${target}.elf PUBLIC

            ${TARGET_ESP32_IDF_NANOCLR_INCLUDE_DIRS}
            ${TARGET_ESP32_IDF_NETWORK_INCLUDE_DIRS}
        )

    endif()

endmacro()

macro(nf_add_platform_sources target)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/target_common.h.in
                   ${CMAKE_CURRENT_BINARY_DIR}/target_common.h @ONLY)
    configure_file(${CMAKE_SOURCE_DIR}/CMake/ESP32_target_os.h.in
                       ${CMAKE_BINARY_DIR}/targets/ESP32/target_os.h @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/nanoCLR/target_board.h.in
                       ${CMAKE_CURRENT_BINARY_DIR}/nanoCLR/target_board.h @ONLY)

    target_sources(nanoCLR.elf PUBLIC
                   ${TARGET_ESP32_IDF_COMMON_SOURCES}
                   ${TARGET_ESP32_IDF_NANOCLR_SOURCES}
                   ${ESP32_IDF_SOURCES}
    )
 if(USE_SECURITY_MBEDTLS_OPTION)
        target_link_libraries(${target}.elf
                             mbedtls
                  )

 add_dependencies(NF_Network mbedtls)
    endif()

endmacro()
macro(nf_setup_target_build)
    cmake_parse_arguments(
        NFSTBC 
        "" 
        "CLR_LINKER_FILE;CLR_EXTRA_LINKMAP_PROPERTIES;CLR_EXTRA_COMPILE_DEFINITIONS;CLR_EXTRA_COMPILE_OPTIONS;CLR_EXTRA_LINK_FLAGS" 
        "CLR_EXTRA_SOURCE_FILES;CLR_EXTRA_LIBRARIES" 
        ${ARGN})
    nf_add_common_packages()
    nf_add_platform_packages()
    target_sources( nanoCLR.elf PUBLIC
        ${NFSTBC_CLR_EXTRA_SOURCE_FILES}
    )

    #nf_add_platform_packages(TARGET nanoCLR)
    nf_add_platform_dependencies(nanoCLR)
    target_sources(nanoCLR.elf PUBLIC
                   ${CMAKE_CURRENT_SOURCE_DIR}/target_common.c
                   ${CMAKE_CURRENT_SOURCE_DIR}/target_BlockStorage.c
                   ${CMAKE_SOURCE_DIR}/src/PAL/BlockStorage/nanoPAL_BlockStorage.c
                   ${COMMON_PROJECT_SOURCES}
                   ${NF_HALCore_SOURCES}
    )
    target_link_libraries(nanoCLR.elf
                          NF_CoreCLR
                          NF_NativeAssemblies
                          ${NFACS_EXTRA_LIBRARIES}
    )

    target_sources(nanoCLR.elf PUBLIC
        ${NANOCLR_PROJECT_SOURCES}
        ${Graphics_Sources}
    )
    nf_add_platform_sources(nanoCLR)
    nf_add_common_include_directories(nanoCLR)
    nf_add_platform_include_directories(nanoCLR)
    nf_set_compile_options(TARGET nanoCLR.elf EXTRA_COMPILE_OPTIONS ${NFSTBC_BOOTER_EXTRA_COMPILE_OPTIONS})
    nf_set_compile_definitions(TARGET nanoCLR.elf EXTRA_COMPILE_DEFINITIONS ${NFSTBC_CLR_EXTRA_COMPILE_DEFINITIONS} BUILD_TARGET ${NANOCLR_PROJECT_NAME} )
    nf_set_link_options(TARGET nanoCLR.elf EXTRA_LINK_FLAGS ${NFSTBC_CLR_EXTRA_LINK_FLAGS})
    idf_build_executable(nanoCLR.elf)
endmacro()

# macro that setups the calls to the partition tool to generate the various partitions
macro(nf_setup_partition_tables_generator)

    # create partition tables for other memory sizes
    set(ESP32_PARTITION_TABLE_UTILITY ${IDF_PATH_CMAKED}/components/partition_table/gen_esp32part.py )

    # create command line for partition table generator
    set(gen_partition_table "python" "${ESP32_PARTITION_TABLE_UTILITY}")


        add_custom_command( TARGET ${NANOCLR_PROJECT_NAME}.elf POST_BUILD
            COMMAND ${gen_partition_table} 
            --flash-size 4MB 
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_IDF/esp32p4/partitions_nanoclr_4mb.csv
            ${CMAKE_BINARY_DIR}/partitions_4mb.bin
            COMMENT "Generate partition table for 4MB flash" )

        add_custom_command( TARGET ${NANOCLR_PROJECT_NAME}.elf POST_BUILD
            COMMAND ${gen_partition_table} 
            --flash-size 8MB 
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_IDF/esp32p4/partitions_nanoclr_8mb.csv
            ${CMAKE_BINARY_DIR}/partitions_8mb.bin
            COMMENT "Generate partition table for 8MB flash" )

        add_custom_command( TARGET ${NANOCLR_PROJECT_NAME}.elf POST_BUILD
            COMMAND ${gen_partition_table} 
            --flash-size 16MB 
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_IDF/esp32p4/partitions_nanoclr_16mb.csv
            ${CMAKE_BINARY_DIR}/partitions_16mb.bin
            COMMENT "Generate partition table for 16MB flash" )



        # 32MB partition table for ESP32_S3
        add_custom_command( TARGET ${NANOCLR_PROJECT_NAME}.elf POST_BUILD
            COMMAND ${gen_partition_table} 
            --flash-size 32MB 
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_IDF/esp32p4/partitions_nanoclr_32mb.csv
            ${CMAKE_BINARY_DIR}/partitions_32mb.bin
            COMMENT "Generate partition table for 32MB flash" )



endmacro()

# macro to add the tinyusb component which has been downloaded from component registry to IDF components directory
# As the Component Manager is not available for IDF as Library projects we need to set up environment manually for the 
# esp_tinyusb to tinyusb dependency.
macro(nf_add_tinyusb_component)

    # get the esp_tinyusb target library name
    idf_component_get_property(etusb_lib esp_tinyusb COMPONENT_LIB)
    # add the tinyusb src directory as include path to esp_tinyusb library project
    target_include_directories(${etusb_lib} PRIVATE ${IDF_PATH_CMAKED}/components/tinyusb/src)

    # also add the freertos directory as include path
    idf_component_get_property(freertos_include freertos ORIG_INCLUDE_PATH)
    target_include_directories(${etusb_lib} PRIVATE ${freertos_include})

    # Set the CFG_TUSB_MCU compile option for the target MCU
    # for esp_tinyusb lib and main project
    set(tusb_mcu "OPT_MCU_ESP32P4")

    set(compile_options
        "-DCFG_TUSB_MCU=${tusb_mcu}"
    )

    target_compile_options(${etusb_lib} PUBLIC ${compile_options})
    target_compile_options(${NANOCLR_PROJECT_NAME}.elf PUBLIC ${compile_options})

endmacro()

#macro to install component from component registry into IDF component directory
macro(nf_install_idf_component_from_registry component_name object_id)

    message(STATUS "Checking if component '" ${component_name} "' needs to be installed")
    
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

    # sanity check
    if(${MY_IDF_VER} STREQUAL "")
        message(FATAL_ERROR "Couldn't get IDF version from target __idf_build_target")
    endif()

    message(STATUS "ESP_IDF_VERSION: $ENV{ESP_IDF_VERSION}")
    message(STATUS "Current IDF version is: ${MY_IDF_VER}")

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

    set(SDKCONFIG_DEFAULTS_FILE ${CMAKE_SOURCE_DIR}/targets/ESP32/_IDF/${SDK_CONFIG_FILE})

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
    idf_build_process(esp32p4
        COMPONENTS 
            ${IDF_COMPONENTS_TO_ADD}
        SDKCONFIG_DEFAULTS
            ${SDKCONFIG_DEFAULTS_TEMP_FILE}
        PROJECT_NAME "nanoCLR"
        PROJECT_VER ${BUILD_VERSION}
        PROJECT_DIR ${CMAKE_SOURCE_DIR}
    )

    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    # add IDF app_main
    add_executable(
        nanoCLR.elf
        ${CMAKE_SOURCE_DIR}/targets/ESP32/_IDF/esp32p4/app_main.c
        ${CMAKE_SOURCE_DIR}/targets/ESP32/_IDF/project_elf_src_esp32p4.c
    )

    if(USE_NETWORKING_OPTION)

        FetchContent_GetProperties(esp32_idf)

        # get list of source files for lwIP
        get_target_property(IDF_LWIP_SOURCES __idf_lwip SOURCES)

        # remove the ones we'll be replacing
        list(REMOVE_ITEM 
            IDF_LWIP_SOURCES
                ${IDF_PATH_CMAKED}/components/lwip/lwip/src/api/api_msg.c
                ${IDF_PATH_CMAKED}/components/lwip/lwip/src/api/sockets.c
                ${IDF_PATH_CMAKED}/components/lwip/port/freertos/sys_arch.c
        )

        # add our modified sources
        list(APPEND 
            IDF_LWIP_SOURCES
                ${CMAKE_SOURCE_DIR}/targets/ESP32/_lwIP/nf_api_msg.c
                ${CMAKE_SOURCE_DIR}/targets/ESP32/_lwIP/nf_sockets.c
                ${CMAKE_SOURCE_DIR}/targets/ESP32/_lwIP/nf_sys_arch.c
        )

        # replace the source list
        set_property(
            TARGET __idf_lwip 
            PROPERTY SOURCES ${IDF_LWIP_SOURCES}
        )
        
        # get list of include directories for lwIP
        get_target_property(IDF_LWIP_INCLUDE_DIRECTORIES __idf_lwip INCLUDE_DIRECTORIES)

        # add nanoCLR include path to lwIP so our lwipots are taken instead of the IDF ones
        list(INSERT 
            IDF_LWIP_INCLUDE_DIRECTORIES 0
                ${CMAKE_SOURCE_DIR}/targets/ESP32/_include
                ${CMAKE_SOURCE_DIR}/targets/ESP32/ESP32_P4
                ${CMAKE_SOURCE_DIR}/src/DeviceInterfaces/Networking.Sntp
                ${CMAKE_SOURCE_DIR}/src/CLR/Include
                ${CMAKE_SOURCE_DIR}/src/HAL/Include
        )

        # replace the include directories
        set_property(
            TARGET __idf_lwip 
            PROPERTY INCLUDE_DIRECTORIES ${IDF_LWIP_INCLUDE_DIRECTORIES}
        )

        # add nanoCLR compile definitions to lwIP
        list(APPEND 
            IDF_LWIP_COMPILE_DEFINITIONS 
                PLATFORM_ESP32
                ESP_LWIP_COMPONENT_BUILD
            )

        # add the compile definitions
        set_property(
            TARGET __idf_lwip 
            PROPERTY COMPILE_DEFINITIONS ${IDF_LWIP_COMPILE_DEFINITIONS}
        )

    endif()

    # need to add include path to find our ffconfig.h and target_platform.h
    
    # get list of include directories for FATFS
    get_target_property(IDF_FATFS_INCLUDE_DIRECTORIES __idf_fatfs INCLUDE_DIRECTORIES)

    # add nanoCLR include path to FATFS so our lwipots are taken instead of the IDF ones
    list(APPEND
        IDF_FATFS_INCLUDE_DIRECTORIES
        ${CMAKE_BINARY_DIR}/targets/ESP32/ESP32_P4/
    )

    # add nanoCLR include path to FATFS so our lwipots are taken instead of the IDF ones
    list(APPEND
        IDF_FATFS_INCLUDE_DIRECTORIES
            ${CMAKE_SOURCE_DIR}/targets/ESP32/ESP32_P4
    )

    # replace the include directories
    set_property(
        TARGET __idf_fatfs 
        PROPERTY INCLUDE_DIRECTORIES ${IDF_FATFS_INCLUDE_DIRECTORIES}
    )

    # Link the static libraries to the executable
    target_link_libraries(${NANOCLR_PROJECT_NAME}.elf 
        ${IDF_LIBRARIES_TO_ADD}
    )

    # add nano libraries to the link dependencies of IDF build
    idf_build_set_property(__LINK_DEPENDS "NF_CoreCLR;NF_NativeAssemblies" APPEND)
    nf_setup_partition_tables_generator()
    # need to read the supplied SDK CONFIG file        
    file(READ
        ${CMAKE_SOURCE_DIR}/sdkconfig
        SDKCONFIG_DEFAULT_CONTENTS)

    # find out if there is support for PSRAM
    set(SPIRAM_SUPPORT_PRESENT -1)
    string(FIND ${SDKCONFIG_DEFAULT_CONTENTS} "CONFIG_SPIRAM=y" SPIRAM_SUPPORT_PRESENT)
    
    # set variable
    if(${SPIRAM_SUPPORT_PRESENT} GREATER -1)
        set(PSRAM_INFO ", support for PSRAM")
        message(STATUS "Support for PSRAM included")
    else()
        set(PSRAM_INFO ", without support for PSRAM")
        message(STATUS "Support for PSRAM **IS NOT** included")
    endif()

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


    # find out if there is support for BLE
    string(FIND ${SDKCONFIG_DEFAULT_CONTENTS} "CONFIG_BT_ENABLED=y" CONFIG_BT_ENABLED_POS)

    # set variable
    if(${CONFIG_BT_ENABLED_POS} GREATER -1)
        set(BLE_INFO ", support for BLE")
    endif()    

    # Disable warning on link
    target_link_libraries(${NANOCLR_PROJECT_NAME}.elf "-Wl,--no-warn-rwx-segments")

    ############################################################
    # output component size summary for the nanoCLR executable #
    # more on this here: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/performance/size.html#size-summary-idf-py-size
    
    # set the map file with the components
    set(nanoCLRMapfile "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.map")
    target_link_libraries(${NANOCLR_PROJECT_NAME}.elf "-Wl,--cref" "-Wl,--Map=\"${nanoCLRMapfile}\"")

    # setup the call to the python script to generate the size summary
    set(ESP32_IDF_SIZE_UTILITY ${IDF_PATH_CMAKED}/tools/idf_size.py)
    set(output_idf_size "python" "${ESP32_IDF_SIZE_UTILITY}")

    add_custom_command(
        TARGET ${NANOCLR_PROJECT_NAME}.elf POST_BUILD
        COMMAND ${output_idf_size}
        --archives ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.map
        COMMENT "Output IDF size summary")
endmacro()
