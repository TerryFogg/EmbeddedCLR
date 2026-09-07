#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

list(APPEND Core_Sources
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/Cache.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/Checks.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_DblLinkedList.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_HeapBlock.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_HeapBlock_Array.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_HeapBlock_ArrayList.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_HeapBlock_BinaryBlob.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_HeapBlock_Delegate.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_HeapBlock_Delegate_List.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_HeapBlock_Finalizer.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_HeapBlock_Lock.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_HeapBlock_LockRequest.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_HeapBlock_Node.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_HeapBlock_Queue.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_HeapBlock_Stack.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_HeapBlock_String.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_HeapBlock_Timer.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_HeapBlock_WaitForObject.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_HeapCluster.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_Interop.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_Memory.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_ObjectToEvent_Destination.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_ObjectToEvent_Source.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_RuntimeMemory.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_StackFrame.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_SystemAssembliesTable.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/CLR_RT_UnicodeHelper.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/Core.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/Execution.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/GarbageCollector.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/GarbageCollector_Compaction.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/GarbageCollector_ComputeReachabilityGraph.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/GarbageCollector_Info.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/Interpreter.cpp
#           ${CMAKE_SOURCE_DIR}/src/CLR/Core/nanoSupport_CRC32.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/Random.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/Streams.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/StringTable.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/StringTableData.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/Thread.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/TypeSystem.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/TypeSystemLookup.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/Various.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/Hardware/Hardware.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/InterruptHandler/InterruptHandler.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/NativeEventDispatcher/NativeEventDispatcher.cpp

            ${CMAKE_SOURCE_DIR}/src/CLR/Debugger/Debugger.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Messaging/Messaging.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Debugger/Debugger_full.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Diagnostics/Info.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Diagnostics/Profile.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Diagnostics/Profiler.cpp

           #${CMAKE_SOURCE_DIR}/src/CLR/Core/RPC/CLR_RT_HeapBlock_EndPoint.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Core/RPC/RPC_stub.cpp

            ${CMAKE_SOURCE_DIR}/src/CLR/Core/Serialization/BinaryFormatter.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_AppDomain.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Array.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Attribute.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_BitConverter.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Collections_ArrayList.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Convert.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_DateTime.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Delegate.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Diagnostics_Debug.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Diagnostics_Debugger.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Double.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Enum.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Exception.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_GC.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Globalization_CultureInfo.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Globalization_DateTimeFormat.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Guid.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_MarshalByRefObject.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_MathInternal.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_MulticastDelegate.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Number.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Object.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Random.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Reflection_Assembly.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Reflection_Binder.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Reflection_ConstructorInfo.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Reflection_FieldInfo.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Reflection_MemberInfo.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Reflection_MethodBase.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Reflection_PropertyInfo.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Reflection_RuntimeFieldInfo.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Reflection_RuntimeMethodInfo.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Runtime_CompilerServices_RuntimeHelpers.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Runtime_Remoting_RemotingServices.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_RuntimeType.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_String.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Threading_AutoResetEvent.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Threading_Interlocked.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Threading_ManualResetEvent.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Threading_Monitor.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Threading_SpinWait.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Threading_Thread.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Threading_Timer.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Threading_WaitHandle.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_TimeSpan.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Type.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_ValueType.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_WeakReference.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Debugger/Debugger_stub.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Diagnostics/Info_Safeprintf.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Helpers/nanoprintf/nanoprintf.c
            ${CMAKE_SOURCE_DIR}/src/CLR/Helpers/NanoRingBuffer/nanoRingBuffer.c
            ${CMAKE_SOURCE_DIR}/src/CLR/Messaging/Messaging_stub.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/Startup/CLRStartup.cpp
            ${CMAKE_SOURCE_DIR}/src/HAL/nanoHAL_Boot.c
            ${CMAKE_SOURCE_DIR}/src/HAL/nanoHAL_Capabilites.c
            ${CMAKE_SOURCE_DIR}/src/HAL/nanoHAL_SystemEvents.c
            ${CMAKE_SOURCE_DIR}/src/HAL/nanoHAL_SystemInformation.cpp
            ${CMAKE_SOURCE_DIR}/src/HAL/nanoHAL_Time.cpp
            ${CMAKE_SOURCE_DIR}/src/HAL/nanoHAL_Watchdog.c
            ${CMAKE_SOURCE_DIR}/src/nanoFramework.Runtime.Native/nf_rt_native.cpp
            ${CMAKE_SOURCE_DIR}/src/nanoFramework.Runtime.Native/nf_rt_native_nanoFramework_Runtime_Hardware_SystemInfo.cpp
            ${CMAKE_SOURCE_DIR}/src/nanoFramework.Runtime.Native/nf_rt_native_nanoFramework_Runtime_Native_ExecutionConstraint.cpp
            ${CMAKE_SOURCE_DIR}/src/nanoFramework.Runtime.Native/nf_rt_native_nanoFramework_Runtime_Native_GC.cpp
            ${CMAKE_SOURCE_DIR}/src/nanoFramework.Runtime.Native/nf_rt_native_nanoFramework_Runtime_Native_Power.cpp
            ${CMAKE_SOURCE_DIR}/src/nanoFramework.Runtime.Native/nf_rt_native_nanoFramework_Runtime_Native_Rtc_stubs.cpp
            ${CMAKE_SOURCE_DIR}/src/nanoFramework.Runtime.Native/nf_rt_native_System_Environment.cpp
            ${CMAKE_SOURCE_DIR}/src/PAL/AsyncProcCall/AsyncCompletions.cpp
            ${CMAKE_SOURCE_DIR}/src/PAL/BlockStorage/nanoPAL_BlockStorage.c
            ${CMAKE_SOURCE_DIR}/src/PAL/COM/COM_stubs.c
            ${CMAKE_SOURCE_DIR}/src/PAL/COM/GenericPort_stubs.c
            ${CMAKE_SOURCE_DIR}/src/PAL/Double/nanoPAL_NativeDouble.cpp
            ${CMAKE_SOURCE_DIR}/src/PAL/Events/nanoPAL_Events.cpp
            ${CMAKE_SOURCE_DIR}/src/PAL/FileSystem/nanoPAL_FileSystem_stubs.cpp
            ${CMAKE_SOURCE_DIR}/src/PAL/nanoPAL_Network_stubs.cpp
            ${CMAKE_SOURCE_DIR}/src/PAL/Profiler/nanoPAL_PerformanceCounters_stubs.cpp
            ${CMAKE_SOURCE_DIR}/targets/ESP32/ESP32_P4/target_BlockStorage.c
            ${CMAKE_SOURCE_DIR}/targets/ESP32/ESP32_P4/target_common.c
    )

list(APPEND Core_Includes
            ${CMAKE_SOURCE_DIR}/src/CLR/Core
            ${CMAKE_SOURCE_DIR}/src/CLR/Include
            ${CMAKE_SOURCE_DIR}/src/HAL/Include
            ${CMAKE_SOURCE_DIR}/src/PAL/Include
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib
            ${CMAKE_SOURCE_DIR}/src/CLR/Startup
            ${CMAKE_SOURCE_DIR}/src/CLR/Diagnostics
            ${CMAKE_SOURCE_DIR}/src/CLR/Debugger
            ${CMAKE_SOURCE_DIR}/src/CLR/Helpers/NanoRingBuffer
            ${CMAKE_SOURCE_DIR}/src/CLR/Helpers/nanoprintf
            ${CMAKE_SOURCE_DIR}/src/CLR/Helpers/Base64
            ${CMAKE_SOURCE_DIR}/src/nanoFramework.Runtime.Events
            ${CMAKE_SOURCE_DIR}/src/nanoFramework.Runtime.Native
            ${CMAKE_SOURCE_DIR}/src/nanoFramework.System.Collections
            ${CMAKE_SOURCE_DIR}/src/DeviceInterfaces/Networking.Sntp

            ${CMAKE_BINARY_DIR}/targets/ESP32/
            ${CMAKE_BINARY_DIR}/targets/ESP32/ESP32_P4
            ${CMAKE_BINARY_DIR}/targets/ESP32/ESP32_P4/nanoCLR

            ${CMAKE_SOURCE_DIR}/src/CLR/Messaging
            ${CMAKE_SOURCE_DIR}/src/CLR/WireProtocol

            ${TARGET_BASE_LOCATION}
            ${TARGET_BASE_LOCATION}/nanoCLR
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_include

)

if(NF_TRACE_TO_STDIO)
    list(APPEND Core_Sources
                ${CMAKE_SOURCE_DIR}/src/PAL/COM/GenericPort_stdio.c
    )
endif()

if(NOT USE_SECURITY_MBEDTLS_OPTION)
    list(APPEND Core_Sources
                ${CMAKE_SOURCE_DIR}/src/CLR/Helpers/Base64/base64.c
    )
endif()

if(NF_FEATURE_HAS_CONFIG_BLOCK)
    list(APPEND Core_Sources
                ${CMAKE_SOURCE_DIR}/src/HAL/nanoHAL_ConfigurationManager.c
    )
else()
    list(APPEND Core_Sources
                ${CMAKE_SOURCE_DIR}/src/HAL/nanoHAL_ConfigurationManager_stubs.c
    )
endif()

list(APPEND Core_Sources
            ${CMAKE_SOURCE_DIR}/targets/ESP32/_nanoCLR/nanoFramework.Runtime.Native/nf_rt_native_nanoFramework_Runtime_Native_Rtc.cpp
)

target_sources(nanoCLR PUBLIC ${Core_Sources} )
target_include_directories(nanoCLR PUBLIC  ${Core_Includes} )   
