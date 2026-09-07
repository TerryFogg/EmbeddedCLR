#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#



macro(nf_add_common_sources)
    target_sources(${NFACS_TARGET}.elf PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/target_common.c
        ${CMAKE_CURRENT_SOURCE_DIR}/target_BlockStorage.c
        ${CMAKE_SOURCE_DIR}/src/PAL/BlockStorage/nanoPAL_BlockStorage.c
        ${COMMON_PROJECT_SOURCES}
        ${NF_HALCore_SOURCES}
    )
endmacro()

function(nf_generate_build_output_files target)
    target_link_options(nanoCLR PUBLIC "-Wl,-Map=${CMAKE_BINARY_DIR}/nanoCLR.map,--cref")
    set(PythonCommand "python" ${IDF_PATH_CMAKED}/tools/idf_size.py)
    add_custom_command(TARGET nanoCLR POST_BUILD
      COMMAND ${CMAKE_OBJCOPY}           $<TARGET_FILE:nanoCLR>   ${CMAKE_BINARY_DIR}/nanoCLR.elf
      COMMAND ${CMAKE_OBJCOPY} -Oihex    $<TARGET_FILE:nanoCLR>   ${CMAKE_BINARY_DIR}/nanoCLR.hex
      COMMAND ${CMAKE_OBJCOPY} -Obinary  $<TARGET_FILE:nanoCLR>   ${CMAKE_BINARY_DIR}/nanoCLR.bin
      COMMAND ${CMAKE_OBJDUMP} -d -EL -S $<TARGET_FILE:nanoCLR> > ${CMAKE_BINARY_DIR}/nanoCLR.lst
  #    COMMAND ${PythonCommand} --archives ${CMAKE_BINARY_DIR}/nanoCLR.map COMMENT "Output IDF size summary"
    )

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

