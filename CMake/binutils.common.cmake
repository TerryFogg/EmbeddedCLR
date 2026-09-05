#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#


# Add packages that are common to ALL builds
# To be called from target CMakeList.txt
macro(nf_add_common_packages)

    find_package(WireProtocol REQUIRED QUIET)
    find_package(NF_NativeAssemblies REQUIRED QUIET)
    find_package(NF_CoreCLR REQUIRED QUIET)
    find_package(NF_HALCore REQUIRED QUIET)

    if(NF_FEATURE_DEBUGGER)
        find_package(NF_Debugger REQUIRED QUIET)
        find_package(NF_Diagnostics REQUIRED QUIET)
    endif()

endmacro()

macro(nf_add_common_include_directories target)
    target_include_directories(${target}.elf PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/common
        ${CMAKE_CURRENT_BINARY_DIR}/${target}
        ${CMAKE_CURRENT_SOURCE_DIR}/${target}
        ${NF_HALCore_INCLUDE_DIRS}
    )
    if(${target} STREQUAL ${NANOCLR_PROJECT_NAME})
        target_include_directories(${target}.elf PUBLIC
            ${NF_Diagnostics_INCLUDE_DIRS}
            ${Graphics_Includes}
        )
    endif()
endmacro()

macro(nf_add_common_sources)
    cmake_parse_arguments(
        NFACS 
        "" 
        "TARGET" 
        "EXTRA_LIBRARIES" 
        ${ARGN})
    target_sources(${NFACS_TARGET}.elf PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/target_common.c
        ${CMAKE_CURRENT_SOURCE_DIR}/target_BlockStorage.c
        ${CMAKE_SOURCE_DIR}/src/PAL/BlockStorage/nanoPAL_BlockStorage.c
        ${COMMON_PROJECT_SOURCES}
        ${NF_HALCore_SOURCES}
    )
    if(${NFACS_TARGET} STREQUAL ${NANOCLR_PROJECT_NAME})
        target_link_libraries(${NFACS_TARGET}.elf
            nano::NF_CoreCLR
            nano::NF_NativeAssemblies
            ${NFACS_EXTRA_LIBRARIES}
        )
#        if(NF_FEATURE_DEBUGGER)
#            target_link_libraries(${NFACS_TARGET}.elf
#                nano::NF_Debugger
#            )
#       endif()  
        target_sources(${NFACS_TARGET}.elf PUBLIC
            ${NANOCLR_PROJECT_SOURCES}
            ${Graphics_Sources}
        )
    endif()
endmacro()

function(nf_generate_build_output_files target)

    # need to remove the .elf suffix from target name
    string(FIND ${target} "." TARGET_EXTENSION_DOT_INDEX)
    string(SUBSTRING ${target} 0 ${TARGET_EXTENSION_DOT_INDEX} TARGET_SHORT)

    set(TARGET_HEX_FILE ${CMAKE_BINARY_DIR}/${TARGET_SHORT}.hex)
    set(TARGET_BIN_FILE ${CMAKE_BINARY_DIR}/${TARGET_SHORT}.bin)
    set(TARGET_DUMP_FILE ${CMAKE_BINARY_DIR}/${TARGET_SHORT}.lst)

    if(CMAKE_BUILD_TYPE MATCHES "Release" OR CMAKE_BUILD_TYPE MATCHES "MinSizeRel")
        add_custom_command(TARGET ${TARGET_SHORT}.elf POST_BUILD
            # copy target image to other formats
            COMMAND ${CMAKE_OBJCOPY} $<TARGET_FILE:${TARGET_SHORT}.elf> ${CMAKE_BINARY_DIR}/${TARGET_SHORT}.elf
            COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${TARGET_SHORT}.elf> ${TARGET_HEX_FILE}
            COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${TARGET_SHORT}.elf> ${TARGET_BIN_FILE}
            BYPRODUCTS 
                ${TARGET_HEX_FILE} 
                ${TARGET_BIN_FILE}
            COMMENT "Generate nanoBooter HEX and BIN files for deployment")
    else()

        add_custom_command(TARGET ${TARGET_SHORT}.elf POST_BUILD
                # copy target image to other formats
                COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${TARGET_SHORT}.elf> ${TARGET_HEX_FILE}
                COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${TARGET_SHORT}.elf> ${TARGET_BIN_FILE}

                # copy target file to build folder (this is only useful for debugging in VS Code because of path in launch.json)
                COMMAND ${CMAKE_OBJCOPY} $<TARGET_FILE:${TARGET_SHORT}.elf> ${CMAKE_BINARY_DIR}/${TARGET_SHORT}.elf

                # dump target image as source code listing 
                # ONLY when DEBUG info is available, this is on 'Debug' and 'RelWithDebInfo'
                COMMAND ${CMAKE_OBJDUMP} -d -EL -S $<TARGET_FILE:${TARGET_SHORT}.elf> > ${TARGET_DUMP_FILE}

                COMMENT "Generate nanoBooter HEX and BIN files for deployment, LST file for debug")

    endif()
        
    # add this to print the size of the output targets
    nf_print_target_size(${target})

endfunction()


#######################################################################################################################################
# this function sets the linker options AND a specific linker file (full path and name, including extension)
function(nf_set_linker_options_and_file target linker_file_name)
    get_target_property(TARGET_LD_FLAGS ${target} LINK_FLAGS)
    if(TARGET_LD_FLAGS)
        set(TARGET_LD_FLAGS "-T${linker_file_name} ${TARGET_LD_FLAGS}")
    else()
        set(TARGET_LD_FLAGS "-T${linker_file_name}")
    endif()
    set_target_properties(${target} PROPERTIES LINK_FLAGS ${TARGET_LD_FLAGS})
endfunction()

macro(nf_set_link_map)
    cmake_parse_arguments(NFSLM "" "TARGET;EXTRA_LINKMAP_PROPERTIES" "" ${ARGN})
    if(NOT NFSLM_TARGET OR "${NFSLM_TARGET}" STREQUAL "")
        message(FATAL_ERROR "Need to set TARGET argument when calling nf_set_link_map()")
    endif()
    string(FIND ${NFSLM_TARGET} "." TARGET_EXTENSION_DOT_INDEX)
    string(SUBSTRING ${NFSLM_TARGET} 0 ${TARGET_EXTENSION_DOT_INDEX} TARGET_SHORT)
    set_property(TARGET ${TARGET_SHORT}.elf APPEND_STRING PROPERTY LINK_FLAGS " -Wl,-Map=${CMAKE_BINARY_DIR}/${TARGET_SHORT}.map${NFSLM_EXTRA_LINKMAP_PROPERTIES}")
endmacro()


macro(nf_include_libraries_in_build target)
    string(FIND ${target} ${NANOCLR_PROJECT_NAME} CLR_INDEX)
    set_property(TARGET ${target} APPEND_STRING PROPERTY LINK_FLAGS " -Wl,--whole-archive -L${CMAKE_CURRENT_BINARY_DIR} -lNF_CoreCLR -Wl,--no-whole-archive ")
    set_property(TARGET ${target} APPEND_STRING PROPERTY LINK_FLAGS " -Wl,--whole-archive -L${CMAKE_CURRENT_BINARY_DIR} -lNF_NativeAssemblies -Wl,--no-whole-archive ")
    if(USE_NETWORKING_OPTION)
        set_property(TARGET ${target} APPEND_STRING PROPERTY LINK_FLAGS " -Wl,--whole-archive -L${CMAKE_CURRENT_BINARY_DIR} -lNF_Network -Wl,--no-whole-archive ")
    endif()
    #if(NF_FEATURE_DEBUGGER)
    #    set_property(TARGET ${target} APPEND_STRING PROPERTY LINK_FLAGS " -Wl,--whole-archive -L${CMAKE_CURRENT_BINARY_DIR} -lNF_Debugger -Wl,--no-whole-archive ")
    #endif()
    set_property(TARGET ${target} APPEND_STRING PROPERTY LINK_FLAGS " -Wl,--whole-archive -lgcc -Wl,--no-whole-archive ")
endmacro()


# function to check the path limit in Windows
function(nf_check_path_limits)

    # only need to check in Windows
    if (WIN32)
        set(FILESYSTEM_REG_PATH "HKLM\\SYSTEM\\CurrentControlSet\\Control\\FileSystem")
        cmake_host_system_information(RESULT WIN_LONG_PATH_OPTION QUERY WINDOWS_REGISTRY ${FILESYSTEM_REG_PATH} VALUE LongPathsEnabled)
        if(${WIN_LONG_PATH_OPTION} EQUAL 0)
            message(STATUS "******* WARNING ******\n\nWindows path limit is too short.\nPlease enable long paths in Windows registry.\nSee https://docs.microsoft.com/en-us/windows/win32/fileio/maximum-file-path-limitation?tabs=cmd#enable-long-paths-in-windows-10-version-1607-and-later\n\n")
            set(CMAKE_OBJECT_PATH_MAX 260)
            set(CMAKE_OBJECT_NAME_MAX 255)
        endif()
    endif()
endfunction()

