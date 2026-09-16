#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

target_compile_options( nanoCLR.elf PUBLIC
                        -Wall
                        -Wextra
                        -Werror
                        -Wno-sign-compare
                        -Wno-unused-parameter
                        -Wshadow
                        -Wimplicit-fallthrough
                        -fshort-wchar
                        -fno-builtin
                        -fno-common
                        -fno-exceptions
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(nanoCLR.elf PUBLIC
                           -Og
                           -g3
                           -fno-omit-frame-pointer
                           -fno-inline
                           -fno-optimize-sibling-calls
    )
    target_compile_definitions( nanoCLR.elf PUBLIC
                                NANOCLR_ENABLE_SOURCELEVELDEBUGGING
                                DEBUG
    )
endif()

target_compile_definitions( nanoCLR.elf PUBLIC
                            ESP_PLATFORM
                            F_GETPATH=20
                            I_AM_NANOCLR
                            PLATFORM_ESP32
                            SOC_MMU_PAGE_SIZE=CONFIG_MMU_PAGE_SIZE
                            SOC_XTAL_FREQ_MHZ=CONFIG_XTAL_FREQ
                            TARGET=esp32p4
                            TRACE_MASK=0
                            USE_FPU=TRUE
                            CONFIG_RTOS=1     #Strange define for the check of an include file
)

# Defines used in the c/c++ code in rare situations to debug and profile
if(NANOCLR_APPDOMAINS)
    target_compile_definitions(nanoCLR.elf PUBLIC NANOCLR_APPDOMAINS=1)
endif()

if(SUPPORT_ANY_BASE_CONVERSION)
    target_compile_definitions(nanoCLR.elf PUBLIC SUPPORT_ANY_BASE_CONVERSION=1)
endif()

if(NANOCLR_NO_IL_INLINE)
target_compile_definitions(nanoCLR.elf PUBLIC NANOCLR_NO_IL_INLINE=0)
endif()

if(ADVANCED_PROFILING)
     target_compile_definitions(nanoCLR.elf PUBLIC
                                PLATFORM_NO_CLR_TRACE=0
                                NANOCLR_PROFILE_NEW_CALLS=1
                                PROFILE_NEW_ALLOCATIONS=1
                                DTRACE_MEMORY_STATS=1

     )
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
   target_compile_definitions(nanoCLR.elf PUBLIC BUILD_RTM)
endif()

if(CONFIG_NF_FEATURE_HAS_CONFIG_BLOCK)
      target_compile_definitions(nanoCLR.elf PUBLIC CONFIG_NF_FEATURE_HAS_CONFIG_BLOCK)
endif()

target_compile_options(nanoCLR.elf PUBLIC
                       $<$<CONFIG:Debug>:-Og -g>
                       $<$<CONFIG:Release>:-O3>
                       $<$<CONFIG:MinSizeRel>:-Os>
                       $<$<CONFIG:RelWithDebInfo>:-Os -g>
)

set_source_files_properties(
    ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_Interop.cpp
    PROPERTIES
    COMPILE_OPTIONS "-Wno-error=cast-user-defined"
)

add_custom_command(TARGET nanoCLR.elf POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -Oihex    $<TARGET_FILE:nanoCLR.elf>   ${CMAKE_BINARY_DIR}/nanoCLR.hex
  COMMAND ${CMAKE_OBJCOPY} -Obinary  $<TARGET_FILE:nanoCLR.elf>   ${CMAKE_BINARY_DIR}/nanoCLR.bin
  COMMAND ${CMAKE_OBJDUMP} -d -EL -S $<TARGET_FILE:nanoCLR.elf> > ${CMAKE_BINARY_DIR}/nanoCLR.lst
)

## Tracing Options

if(CONFIGURE_SUPPORT_TRACE)
      target_compile_definitions(nanoCLR.elf PUBLIC 
                                 NANOCLR_GC_VERBOSE
                                 NANOCLR_TRACE_CALLS
                                 NANOCLR_TRACE_DEFAULT
                                 NANOCLR_TRACE_EARLYCOLLECTION
                                 NANOCLR_TRACE_ERRORS
                                 NANOCLR_TRACE_EXCEPTIONS
                                 NANOCLR_TRACE_HRESULT
                                 NANOCLR_TRACE_INSTRUCTIONS
                                 NANOCLR_TRACE_MALLOC
                                 NANOCLR_TRACE_MEMORY_STATS
                                 NANOCLR_TRACE_MEMORY_STATS_EXTRA_SIZE
                                 NANOCLR_TRACE_PERSISTENCE
                                 NANOCLR_TRACE_PROFILER_MESSAGES
                                 NANOCLR_TRACE_STACK
                                 NANOCLR_TRACE_STACK_HEAVY
                                 NANOCLR_TRACE_SYSTEMEVENTWAIT)
endif()


##        {
##            "name": "nanoCLR_Runtime.h #ifdef",
##            "hidden": true,
##            "cacheVariables": {
##                "ENABLE_NATIVE_PROFILER" :"ON",
##                "HIGH_SURROGATE_START" :"ON",
##                "NANANOCLR_EMULATED_FLOATINGPOINT" :"ON",
##                "NANOCLR_APPDOMAINS" :"ON",
##                "NANOCLR_DELEGATE_PRESERVE_STACK" :"ON",
##                "NANOCLR_ENABLE_SOURCELEVELDEBUGGING" :"ON",
##                "NANOCLR_FILL_MEMORY_WITH_DIRTY_PATTERN" :"ON",
##                "NANOCLR_NO_ASSEMBLY_STRINGS" :"ON",
##                "NANOCLR_NO_IL_INLINE" :"ON",
##                "NANOCLR_OPCODE_NAMES" :"ON",
##                "NANOCLR_OPCODE_PARSER" :"ON",
##                "NANOCLR_OPCODE_STACKCHANGES" :"ON",
##                "NANOCLR_PROFILE_HANDLER" :"ON",
##                "NANOCLR_USE_AVLTREE_FOR_METHODLOOKUP" :"ON",
##                "NANOCLR_VALIDATE_APPDOMAIN_ISOLATION" :"ON",
##                "NOCLR_PROFILE_NEW_CALLS" :"ON",
##                "PLATFORM_WINDOWS_EMULATOR" :"ON"
##            }
##        },
##
