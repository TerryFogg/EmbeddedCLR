#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#




list(APPEND ConfigBlock_Sources
            ${CMAKE_SOURCE_DIR}/src/HAL/nanoHAL_ConfigurationManager.c
)

target_sources(nanoCLR PUBLIC ${ConfigBlock_Sources} )
target_compile_definitions(nanoCLR PRIVATE CONFIG_SUPPORT_CONFIGBLOCK=1)

# ${CMAKE_SOURCE_DIR}/src/HAL/nanoHAL_ConfigurationManager_stubs.c
