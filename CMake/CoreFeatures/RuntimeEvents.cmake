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
