

target_compile_options( nanoCLR PUBLIC
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
  #  -mabi=ilp32f
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(nanoCLR PRIVATE
                           -Og
                           -g3
                           -fno-omit-frame-pointer
                           -fno-inline
                           -fno-optimize-sibling-calls
    )
    target_compile_definitions( nanoCLR PRIVATE
                                NANOCLR_ENABLE_SOURCELEVELDEBUGGING
                                DEBUG
    )
endif()

target_compile_definitions( nanoCLR PRIVATE
                            ESP_PLATFORM
                            F_GETPATH=20
                            I_AM_NANOCLR
                            NANOCLR_ENABLE_SOURCELEVELDEBUGGING
                            PLATFORM_ESP32
                            SOC_MMU_PAGE_SIZE=CONFIG_MMU_PAGE_SIZE
                            SOC_XTAL_FREQ_MHZ=CONFIG_XTAL_FREQ
                            TARGET=esp32p4
                            TRACE_MASK=0
                            USE_FPU=TRUE
)




# Defines used in the c/c++ code in rare situations to debug and profile
if(NANOCLR_APPDOMAINS)
    add_compile_definitions(NANOCLR_APPDOMAINS=1)
endif()

if(SUPPORT_ANY_BASE_CONVERSION)
    add_compile_definitions(SUPPORT_ANY_BASE_CONVERSION=1)
endif()

if(NANOCLR_NO_IL_INLINE)
add_compile_definitions(NANOCLR_NO_IL_INLINE=0)
endif()

if(ADVANCED_PROFILING)
     target_compile_definitions(nanoCLR PRIVATE
                                PLATFORM_NO_CLR_TRACE=0
                                NANOCLR_PROFILE_NEW_CALLS=1
                                PROFILE_NEW_ALLOCATIONS=1
                                DTRACE_MEMORY_STATS=1
     )
endif()


if(CMAKE_BUILD_TYPE STREQUAL "Release")
   target_compile_definitions(nanoCLR PRIVATE BUILD_RTM)
endif()

if(CONFIG_NF_FEATURE_HAS_CONFIG_BLOCK)
      target_compile_definitions(nanoCLR PRIVATE CONFIG_NF_FEATURE_HAS_CONFIG_BLOCK)
endif()


 target_compile_options(nanoCLR PRIVATE
                        $<$<CONFIG:Debug>:-Og -g>
                        $<$<CONFIG:Release>:-O3>
                        $<$<CONFIG:MinSizeRel>:-Os>
                        $<$<CONFIG:RelWithDebInfo>:-Os -g>
 )

 set(NANOCLR_PROFILE_NEW_CALLS FALSE CACHE INTERNAL "option to support profilling new function calls")
set(NANOCLR_PROFILE_NEW_ALLOCATIONS FALSE CACHE INTERNAL "option to support profilling new object allocations")
set(NANOCLR_TRACE_MEMORY_STATS FALSE CACHE INTERNAL "option to enable trace of memory stats")



set_source_files_properties(
    ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_Interop.cpp
    PROPERTIES
    COMPILE_OPTIONS "-Wno-error=cast-user-defined"
)
