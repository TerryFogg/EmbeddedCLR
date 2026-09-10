#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#
include(FetchContent)

# Add the linker option to ESP32 build to suppress warnings about read/write/execute segments
target_link_options(nanoCLR PUBLIC "-Wl,--no-warn-rwx-segments")

configure_file(${CMAKE_SOURCE_DIR}/targets/Espressif/ESP32P4/Configuration/target_os.h.in       ${CMAKE_BINARY_DIR}/target_os.h @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/targets/Espressif/ESP32P4/Configuration/target_platform.h.in ${CMAKE_BINARY_DIR}/target_platform.h @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/targets/Espressif/ESP32P4/Configuration/target_common.h.in   ${CMAKE_BINARY_DIR}/target_common.h @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/targets/Espressif/ESP32P4/Configuration/target_board.h.in    ${CMAKE_BINARY_DIR}/target_board.h @ONLY)

set(gen_partition_table "python" "${ESP32_IDF_PATH}/components/partition_table/gen_esp32part.py")
add_custom_target(generate_partition ALL
    COMMAND ${gen_partition_table} 
    --flash-size ${PARTITION_SIZE} 
    "${CMAKE_SOURCE_DIR}/targets/Espressif/ESP32P4/Configuration/partitions_nanoclr_${PARTITION_SIZE}.csv"
    "${CMAKE_BINARY_DIR}/partitions_${PARTITION_SIZE}.bin"
    COMMENT "Generate selected partition table size of  flash" )
