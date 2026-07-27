#
# This file is part of the nanoFramework project (https://www.nanoframework.net)
# Copyright (c) 2017 The nanoFramework project contributors
# See LICENSE file in the project root for full license information.
#
# Create additions and deletions to the build for ESP32_P4 target here.
# This file is included by the the ESP32P4 CMakeLists.txt.

get_property(TARGETS
    DIRECTORY ${CMAKE_SOURCE_DIR}
    PROPERTY BUILDSYSTEM_TARGETS)

get_target_property(SRCS nanoCLR.elf SOURCES)
get_target_property(SRCS_INCLUDES nanoCLR.elf INCLUDE_DIRECTORIES)
list(REMOVE_DUPLICATES SRCS)
list(REMOVE_DUPLICATES SRCS_INCLUDES)

get_target_property(NativeAssemblySources NF_NativeAssemblies SOURCES)
get_target_property(NativeAssemblyIncludes NF_NativeAssemblies INCLUDE_DIRECTORIES)
list(REMOVE_DUPLICATES NativeAssemblySources)
list(REMOVE_DUPLICATES NativeAssemblyIncludes)


list(REMOVE_ITEM SRCS C:/nf-interpreter/targets/ESP32/_nanoCLR/Memory.cpp)
list(REMOVE_ITEM SRCS C:/nf-interpreter/targets/ESP32/_nanoCLR/targetHAL.cpp)

# - Use alternate graphics sources
# OR = original
Set(ORIGINAL_NATIVE_SOURCE "${CMAKE_SOURCE_DIR}/src/nanoFramework.Graphics")
Set(INDEVELOPMENT_SOURCE "${CMAKE_SOURCE_DIR}/!TerryFogg/nanoFramework.Graphics")

# Remove with replacement
list(REMOVE_ITEM NativeAssemblySources ${ORIGINAL_NATIVE_SOURCE}/TouchPanel/Core/Gestures.cpp)
list(REMOVE_ITEM NativeAssemblySources ${ORIGINAL_NATIVE_SOURCE}/TouchPanel/Core/Ink.cpp)
list(REMOVE_ITEM NativeAssemblySources ${ORIGINAL_NATIVE_SOURCE}/Graphics/Native/nanoFramework_Graphics_nanoFramework_UI_Ink.cpp)
list(REMOVE_ITEM NativeAssemblySources ${ORIGINAL_NATIVE_SOURCE}/Graphics/Native/nanoFramework_Graphics_nanoFramework_UI_TouchCollectorConfiguration.cpp)
list(REMOVE_ITEM NativeAssemblySources ${ORIGINAL_NATIVE_SOURCE}/Graphics/Native/nanoFramework_Graphics_nanoFramework_UI_TouchPanel.cpp)
list(REMOVE_ITEM NativeAssemblySources ${ORIGINAL_NATIVE_SOURCE}/Graphics/Displays/ILI9341_240x320_SPI.cpp)
list(REMOVE_ITEM NativeAssemblySources ${ORIGINAL_NATIVE_SOURCE}/TouchPanel/Devices/XPT2046.cpp)
list(REMOVE_ITEM NativeAssemblySources ${ORIGINAL_NATIVE_SOURCE}/Graphics/Displays/Spi_To_Display.cpp)
list(REMOVE_ITEM NativeAssemblySources ${ORIGINAL_NATIVE_SOURCE}/Graphics/Native/nanoFramework_Graphics.cpp)
list(REMOVE_ITEM NativeAssemblySources ${ORIGINAL_NATIVE_SOURCE}/Graphics/Native/nanoFramework_Graphics_nanoFramework_UI_Bitmap.cpp)
list(REMOVE_ITEM NativeAssemblySources ${ORIGINAL_NATIVE_SOURCE}/Graphics/Native/nanoFramework_Graphics_nanoFramework_UI_DisplayControl.cpp)
list(REMOVE_ITEM NativeAssemblySources ${ORIGINAL_NATIVE_SOURCE}/Graphics/Native/nanoFramework_Graphics_nanoFramework_UI_Font.cpp)
list(REMOVE_ITEM NativeAssemblySources ${ORIGINAL_NATIVE_SOURCE}/Graphics/Native/nanoFramework_Graphics_nanoFramework_UI_TouchEventProcessor.cpp)
list(REMOVE_ITEM NativeAssemblySources ${ORIGINAL_NATIVE_SOURCE}/TouchPanel/Core/TouchPanel.cpp)
list(REMOVE_ITEM NativeAssemblySources C:/nf-interpreter/targets/ESP32/ESP32_P4/nanoCLR/nanoFramework.Graphics/Spi_To_TouchPanel.cpp)
list(REMOVE_ITEM NativeAssemblySources C:/nf-interpreter/targets/ESP32/ESP32_P4/nanoCLR/nanoFramework.Graphics/Graphics_Memory.cpp)

list(REMOVE_ITEM SRCS_INCLUDES          ${ORIGINAL_NATIVE_SOURCE}/Graphics/Displays)
list(REMOVE_ITEM SRCS_INCLUDES          ${ORIGINAL_NATIVE_SOURCE}/Graphics/Interfaces)
list(REMOVE_ITEM SRCS_INCLUDES          ${ORIGINAL_NATIVE_SOURCE}/Graphics/Native)
list(REMOVE_ITEM NativeAssemblyIncludes ${ORIGINAL_NATIVE_SOURCE}/Graphics/Displays)
list(REMOVE_ITEM NativeAssemblyIncludes ${ORIGINAL_NATIVE_SOURCE}/Graphics/Interfaces)
list(REMOVE_ITEM NativeAssemblyIncludes ${ORIGINAL_NATIVE_SOURCE}/Graphics/Native)

# Use development versions
list(APPEND SRCS C:/nf-interpreter/!TerryFogg/ESP32/targetHAL.cpp)
list(APPEND SRCS C:/nf-interpreter/!TerryFogg/ESP32/ESP32_P4/Memory.cpp)
list(APPEND SRCS C:/nf-interpreter/!TerryFogg/ESP32/esp_lcd_touch_gt911.c)

list(APPEND NativeAssemblySources ${INDEVELOPMENT_SOURCE}/Graphics/Native/nanoFramework_Graphics.cpp)
list(APPEND NativeAssemblySources ${INDEVELOPMENT_SOURCE}/Graphics/Native/nanoFramework_Graphics_nanoFramework_UI_Bitmap.cpp)
list(APPEND NativeAssemblySources ${INDEVELOPMENT_SOURCE}/Graphics/Native/nanoFramework_Graphics_nanoFramework_UI_DisplayControl.cpp)
list(APPEND NativeAssemblySources ${INDEVELOPMENT_SOURCE}/Graphics/Native/nanoFramework_Graphics_nanoFramework_UI_Font.cpp)
list(APPEND NativeAssemblySources ${INDEVELOPMENT_SOURCE}/Graphics/Native/nanoFramework_Graphics_nanoFramework_UI_TouchEventProcessor.cpp)
list(APPEND NativeAssemblySources ${INDEVELOPMENT_SOURCE}/TouchPanel/Core/TouchPanel.cpp)
list(APPEND NativeAssemblySources ${INDEVELOPMENT_SOURCE}/Graphics/Displays/DisplayControllerJD9365.cpp)
list(APPEND NativeAssemblySources ${INDEVELOPMENT_SOURCE}/Graphics/Interfaces/DisplayInterfaceMipiDsi.cpp)
list(APPEND NativeAssemblySources ${INDEVELOPMENT_SOURCE}/TouchPanel/Interfaces/I2c_To_TouchPanel.cpp)
list(APPEND NativeAssemblySources ${INDEVELOPMENT_SOURCE}/TouchPanel/Devices/GT911.cpp)

list(APPEND SRCS_INCLUDES C:/nf-interpreter/!TerryFogg/Device)
list(APPEND SRCS_INCLUDES C:/nf-interpreter/!TerryFogg/ESP32)
list(APPEND SRCS_INCLUDES C:/nf-interpreter/!TerryFogg/ESP32/ESP32_P4)

list(APPEND NativeAssemblyIncludes ${INDEVELOPMENT_SOURCE}/Graphics/Displays)
list(APPEND NativeAssemblyIncludes ${INDEVELOPMENT_SOURCE}/Graphics/Interfaces)
list(APPEND NativeAssemblyIncludes ${INDEVELOPMENT_SOURCE}/Graphics/Native)

list(APPEND NativeAssemblyIncludes C:/nf-interpreter/!TerryFogg/Device)
list(APPEND NativeAssemblyIncludes C:/nf-interpreter/!TerryFogg/ESP32)
list(APPEND NativeAssemblyIncludes C:/nf-interpreter/!TerryFogg/ESP32/ESP32_P4)
list(APPEND NativeAssemblyIncludes C:/nf-interpreter/src/System.Device.Dac)

# Development reworking on I/O
list(APPEND NativeAssemblySources C:/nf-interpreter/!TerryFogg/Device/DevicePin.cpp)
list(APPEND NativeAssemblySources C:/nf-interpreter/!TerryFogg/Esp32/Device/Gpio.cpp)
list(APPEND NativeAssemblySources C:/nf-interpreter/!TerryFogg/Esp32/Device/I2c.cpp)
list(APPEND SRCS C:/nf-interpreter/!TerryFogg/ESP32/ESP32_P4/board.cpp)

set_target_properties(nanoCLR.elf PROPERTIES SOURCES "${SRCS}")
set_target_properties(nanoCLR.elf PROPERTIES INCLUDE_DIRECTORIES "${SRCS_INCLUDES}")

set_target_properties(NF_NativeAssemblies PROPERTIES SOURCES "${NativeAssemblySources}")
set_target_properties(NF_NativeAssemblies PROPERTIES INCLUDE_DIRECTORIES "${NativeAssemblyIncludes}")
