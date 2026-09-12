#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

set(Runtime_Sources
    ${CMAKE_SOURCE_DIR}/src/nanoFramework.Runtime.Events/nf_rt_events_native_nanoFramework_Runtime_Events_EventSink.cpp
    ${CMAKE_SOURCE_DIR}/src/nanoFramework.Runtime.Events/nf_rt_events_native_nanoFramework_Runtime_Events_NativeEventDispatcher.cpp
    ${CMAKE_SOURCE_DIR}/src/nanoFramework.Runtime.Events/nf_rt_events_native_nanoFramework_Runtime_Events_WeakDelegate.cpp    
    ${CMAKE_SOURCE_DIR}/src/nanoFramework.Runtime.Events/nf_rt_events_native.cpp
    ${CMAKE_SOURCE_DIR}/src/PAL/AsyncProcCall/AsyncContinuations.cpp
    ${CMAKE_SOURCE_DIR}/src/PAL/Events/nanoPAL_Events_functions.cpp
)

list(APPEND Runtime_Includes
            ${CMAKE_SOURCE_DIR}/src/CLR/Core
            ${CMAKE_SOURCE_DIR}/src/CLR/Include
            ${CMAKE_SOURCE_DIR}/src/HAL/Include
            ${CMAKE_SOURCE_DIR}/src/PAL/Include
            ${CMAKE_SOURCE_DIR}/src/nanoFramework.Runtime.Events
)

target_sources(nanoCLR PUBLIC
               ${Runtime_Sources}
)
target_include_directories(nanoCLR PUBLIC 
                           ${Runtime_Includes}
)   


# Default #define PLATFORM_DEPENDENT_ENTRY_SIZE ENTRY_SIZE__medium
# Other Options:
  # ENTRY_SIZE__small,
  # ENTRY_SIZE__large

# Default #define PLATFORM_DEPENDENT_HASH_TABLE_SIZE HASH_TABLE_ENTRY__medium
# Other Options:
  # HASH_TABLE_ENTRY__small,
  # HASH_TABLE_ENTRY__large

  #define PLATFORM_DEPENDENT_INTERRUPT_RECORDS INTERRUPT_RECORDS__medium
  # Other Options: NONE

  #define PLATFORM_DEPENDENT_INLINE_BUFFER_SIZE INLINE_BUFFER__medium
    # Other Options: NONE

