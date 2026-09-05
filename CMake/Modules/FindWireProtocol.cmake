#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

macro(add_wireprotocol)

    set(WP_TRACE_MASK 0 CACHE INTERNAL "WP trace mask")
    if(NF_WP_TRACE_ERRORS)
        math(EXPR WP_TRACE_MASK "${WP_TRACE_MASK} + 1")
    endif()
    if(NF_WP_TRACE_HEADERS)
        math(EXPR WP_TRACE_MASK "${WP_TRACE_MASK} + 2")
    endif()
    if(NF_WP_TRACE_STATE)
        math(EXPR WP_TRACE_MASK "${WP_TRACE_MASK} + 4")
    endif()
    if(NF_WP_TRACE_NODATA)
        math(EXPR WP_TRACE_MASK "${WP_TRACE_MASK} + 8")
    endif()
    if(NF_WP_TRACE_VERBOSE)
        math(EXPR WP_TRACE_MASK "${WP_TRACE_MASK} + 16")
    endif()
    if(NF_WP_TRACE_ALL)
        math(EXPR WP_TRACE_MASK "16 + 8 + 4 + 2 + 1")
    endif()

    list(APPEND WireProtocol_INCLUDE_DIRS
                ${CMAKE_SOURCE_DIR}/src/CLR/Include
    )

    list(APPEND WireProtocol_SRCS
        ${CMAKE_SOURCE_DIR}/src/CLR/WireProtocol/WireProtocol_Message.c
        ${CMAKE_SOURCE_DIR}/src/CLR/WireProtocol/WireProtocol_MonitorCommands.c
        ${CMAKE_SOURCE_DIR}/src/CLR/WireProtocol/WireProtocol_App_Interface.c
        ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/WireProtocol_HAL_Interface.c
        ${CMAKE_SOURCE_DIR}/targets/ESP32/_common/nanoSupport_CRC32.c
    )

    target_sources(nanoCLR.elf PUBLIC
                   ${WireProtocol_SRCS}
    )
    target_include_directories(nanoCLR.elf PUBLIC 
                               ${WireProtocol_INCLUDE_DIRS}
    )   
endmacro()
