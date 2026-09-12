#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#
list(APPEND Mathematics_Sources
    ${CMAKE_SOURCE_DIR}/src/CLR/System.Math/nf_native_system_math.cpp
    ${CMAKE_SOURCE_DIR}/src/CLR/System.Math/nf_native_system_math_System_Math.cpp
)

list(APPEND Mathematics_Includes
            ${CMAKE_SOURCE_DIR}/src/CLR/System.Math
)

target_sources(nanoCLR PUBLIC ${Mathematics_Sources} )
target_include_directories(nanoCLR PUBLIC  ${Mathematics_Includes} )   

if(CONFIG_SUPPORT_MATHEMATICS_EMULATION) 
  target_compile_definitions(nanoCLR PRIVATE NANOCLR_EMULATED_FLOATINGPOINT)
  else()
  target_compile_definitions(nanoCLR PRIVATE CONFIG_NF_ENABLE_DOUBLE_PRECISION_FP)
endif()




