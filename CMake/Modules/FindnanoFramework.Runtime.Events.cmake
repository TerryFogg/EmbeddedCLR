#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

set(BASE_PATH_FOR_THIS_MODULE "${CMAKE_SOURCE_DIR}/src/nanoFramework.Runtime.Events")
list(APPEND nanoFramework.Runtime.Events_INCLUDE_DIRS
            "${CMAKE_SOURCE_DIR}/src/CLR/Core"
            "${CMAKE_SOURCE_DIR}/src/CLR/Include"
            "${CMAKE_SOURCE_DIR}/src/HAL/Include"
            "${CMAKE_SOURCE_DIR}/src/PAL/Include"
            "${BASE_PATH_FOR_THIS_MODULE}"
)
set(nanoFramework.Runtime.Events_SRCS
    nf_rt_events_native_nanoFramework_Runtime_Events_EventSink.cpp
    nf_rt_events_native_nanoFramework_Runtime_Events_NativeEventDispatcher.cpp
    nf_rt_events_native_nanoFramework_Runtime_Events_WeakDelegate.cpp    
    nf_rt_events_native.cpp
    AsyncContinuations.cpp
    nanoPAL_Events_functions.cpp
)
foreach(SRC_FILE ${nanoFramework.Runtime.Events_SRCS})
    set(nanoFramework.Runtime.Events_SRC_FILE SRC_FILE-NOTFOUND)
    find_file(nanoFramework.Runtime.Events_SRC_FILE ${SRC_FILE}   PATHS
            ${BASE_PATH_FOR_THIS_MODULE}
            ${CMAKE_SOURCE_DIR}/src/PAL/AsyncProcCall
            ${CMAKE_SOURCE_DIR}/src/PAL/Events
        CMAKE_FIND_ROOT_PATH_BOTH
    )
    list(APPEND nanoFramework.Runtime.Events_SOURCES ${nanoFramework.Runtime.Events_SRC_FILE})
endforeach()
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(nanoFramework.Runtime.Events DEFAULT_MSG nanoFramework.Runtime.Events_INCLUDE_DIRS nanoFramework.Runtime.Events_SOURCES)
