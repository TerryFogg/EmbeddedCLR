#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

list(APPEND Reflection_Sources
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Reflection_Assembly.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Reflection_Binder.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Reflection_ConstructorInfo.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Reflection_FieldInfo.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Reflection_MemberInfo.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Reflection_MethodBase.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Reflection_PropertyInfo.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Reflection_RuntimeFieldInfo.cpp
            ${CMAKE_SOURCE_DIR}/src/CLR/CorLib/corlib_native_System_Reflection_RuntimeMethodInfo.cpp
    )

target_sources(nanoCLR PUBLIC ${Reflection_Sources} )


target_compile_definitions(nanoCLR PRIVATE CONFIG_NF_FEATURE_SUPPORT_REFLECTION=1)

