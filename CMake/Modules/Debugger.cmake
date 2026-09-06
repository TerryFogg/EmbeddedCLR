#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

# set include directories for nanoFramework Debugger

macro(add_debugger)

    list(APPEND NF_Debugger_SOURCES
        ${CMAKE_SOURCE_DIR}/src/CLR/Debugger/Debugger.cpp
        ${CMAKE_SOURCE_DIR}/src/CLR/Messaging/Messaging.cpp
        ${CMAKE_SOURCE_DIR}/src/CLR/Debugger/Debugger_full.cpp
    )
    list(APPEND NF_Debugger_INCLUDE_DIRS
                ${CMAKE_SOURCE_DIR}/src/CLR/Debugger
                ${CMAKE_SOURCE_DIR}/src/CLR/Messaging
                ${CMAKE_SOURCE_DIR}/src/CLR/WireProtocol
    )
    target_sources(nanoCLR.elf PUBLIC
                   ${NF_Debugger_SOURCES}
    )
    target_include_directories(nanoCLR.elf PUBLIC 
                               ${NF_Debugger_INCLUDE_DIRS}
    )   

endmacro()
