



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
    add_compile_definitions(PLATFORM_NO_CLR_TRACE=0)
    add_compile_definitions(NANOCLR_PROFILE_NEW_CALLS=1)
    add_compile_definitions(PROFILE_NEW_ALLOCATIONS=1)
    add_compile_definitions(DTRACE_MEMORY_STATS=1)
endif()

add_compile_definitions(CONFIG_TOUCH_DISPLAY_SUPPORT)
add_compile_definitions(I_AM_NANOCLR)
add_compile_definitions(TRACE_MASK=0)
add_compile_definitions(PLATFORM_ESP32)
add_compile_definitions(ESP_PLATFORM)
add_compile_definitions(TARGET=esp32p4)
add_compile_definitions(USE_FPU=TRUE)

if(CMAKE_BUILD_TYPE STREQUAL "Debug") 
    add_compile_definitions(DNANOCLR_ENABLE_SOURCELEVELDEBUGGING)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(BUILD_RTM)
endif()

if(CONFIG_NF_FEATURE_HAS_CONFIG_BLOCK)
    add_compile_definitions(CONFIG_NF_FEATURE_HAS_CONFIG_BLOCK)
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
