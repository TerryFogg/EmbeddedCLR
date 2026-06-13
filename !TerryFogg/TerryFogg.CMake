
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
get_target_property(SRCS_GRAPHICS NF_NativeAssemblies SOURCES)
get_target_property(INCLUDES_GRAPHICS NF_NativeAssemblies INCLUDE_DIRECTORIES)

list(REMOVE_DUPLICATES SRCS)
list(REMOVE_ITEM SRCS C:/nf-interpreter/targets/ESP32/_nanoCLR/Memory.cpp)


list(REMOVE_ITEM SRCS_GRAPHICS C:/nf-interpreter/targets/ESP32/ESP32_P4/nanoCLR/nanoFramework.Graphics/Graphics_Memory.cpp)
set_target_properties(nanoCLR.elf PROPERTIES SOURCES "${SRCS}")


# Changes to graphic sources, remove official and replace with mine.
# Graphics Sources

Set(MY_GRAPHICS "${CMAKE_SOURCE_DIR}/!TerryFogg/src/nanoFramework.Graphics")


list(REMOVE_ITEM SRCS_GRAPHICS "${CMAKE_SOURCE_DIR}/src/nanoFramework.Graphics/TouchPanel/Core/Gestures.cpp")
list(REMOVE_ITEM SRCS_GRAPHICS "${CMAKE_SOURCE_DIR}/src/nanoFramework.Graphics/TouchPanel/Core/Ink.cpp")
list(REMOVE_ITEM SRCS_GRAPHICS "${CMAKE_SOURCE_DIR}/src/nanoFramework.Graphics/TouchPanel/Core/TouchPanel.cpp")

list(REMOVE_ITEM SRCS_GRAPHICS "${CMAKE_SOURCE_DIR}/src/nanoFramework.Graphics/Graphics/Native/nanoFramework_Graphics.cpp")
list(REMOVE_ITEM SRCS_GRAPHICS "${CMAKE_SOURCE_DIR}/src/nanoFramework.Graphics/Graphics/Native/nanoFramework_Graphics_nanoFramework_UI_Bitmap.cpp")
list(REMOVE_ITEM SRCS_GRAPHICS "${CMAKE_SOURCE_DIR}/src/nanoFramework.Graphics/Graphics/Native/nanoFramework_Graphics_nanoFramework_UI_DisplayControl.cpp")
list(REMOVE_ITEM SRCS_GRAPHICS "${CMAKE_SOURCE_DIR}/src/nanoFramework.Graphics/Graphics/Native/nanoFramework_Graphics_nanoFramework_UI_Font.cpp")
list(REMOVE_ITEM SRCS_GRAPHICS "${CMAKE_SOURCE_DIR}/src/nanoFramework.Graphics/Graphics/Native/nanoFramework_Graphics_nanoFramework_UI_Ink.cpp")
list(REMOVE_ITEM SRCS_GRAPHICS "${CMAKE_SOURCE_DIR}/src/nanoFramework.Graphics/Graphics/Native/nanoFramework_Graphics_nanoFramework_UI_TouchCollectorConfiguration.cpp")
list(REMOVE_ITEM SRCS_GRAPHICS "${CMAKE_SOURCE_DIR}/src/nanoFramework.Graphics/Graphics/Native/nanoFramework_Graphics_nanoFramework_UI_TouchEventProcessor.cpp")
list(REMOVE_ITEM SRCS_GRAPHICS "${CMAKE_SOURCE_DIR}/src/nanoFramework.Graphics/Graphics/Native/nanoFramework_Graphics_nanoFramework_UI_TouchPanel.cpp")

list(APPEND SRCS_GRAPHICS "${MY_GRAPHICS}/Memory.cpp")
list(APPEND SRCS_GRAPHICS "${MY_GRAPHICS}/Graphics/Displays/DisplayControllerJD9365.cpp")
list(APPEND SRCS_GRAPHICS "${MY_GRAPHICS}/Graphics/Interfaces/DisplayInterfaceMipiDsi.cpp")
list(APPEND SRCS_GRAPHICS "${MY_GRAPHICS}/TouchPanel/Core/TouchPanel.cpp")
list(APPEND SRCS_GRAPHICS "${MY_GRAPHICS}/TouchPanel/Interfaces/I2c_To_TouchPanel.cpp")

list(APPEND SRCS_GRAPHICS "${MY_GRAPHICS}/Graphics/Native/nanoFramework_Graphics.cpp")
list(APPEND SRCS_GRAPHICS "${MY_GRAPHICS}/Graphics/Native/nanoFramework_Graphics_nanoFramework_UI_Bitmap.cpp")
list(APPEND SRCS_GRAPHICS "${MY_GRAPHICS}/Graphics/Native/nanoFramework_Graphics_nanoFramework_UI_DisplayControl.cpp")
list(APPEND SRCS_GRAPHICS "${MY_GRAPHICS}/Graphics/Native/nanoFramework_Graphics_nanoFramework_UI_Font.cpp")
list(APPEND SRCS_GRAPHICS "${MY_GRAPHICS}/Graphics/Native/nanoFramework_Graphics_nanoFramework_UI_TouchEventProcessor.cpp")


set_target_properties(NF_NativeAssemblies PROPERTIES SOURCES "${SRCS_GRAPHICS}")

# Graphics Includes
# -----------------
list(REMOVE_ITEM INCLUDES_GRAPHICS "${CMAKE_SOURCE_DIR}/src/nanoFramework.Graphics/Graphics/Graphics/Displays")
list(REMOVE_ITEM INCLUDES_GRAPHICS "${CMAKE_SOURCE_DIR}/src/nanoFramework.Graphics/Graphics/Graphics/Interfaces")
list(REMOVE_ITEM INCLUDES_GRAPHICS "${CMAKE_SOURCE_DIR}/src/nanoFramework.Graphics/Graphics/Graphics/Native")
list(REMOVE_ITEM INCLUDES_GRAPHICS "${CMAKE_SOURCE_DIR}/src/nanoFramework.Graphics/TouchPanel/Core")
list(REMOVE_ITEM INCLUDES_GRAPHICS "${CMAKE_SOURCE_DIR}/src/nanoFramework.Graphics/TouchPanel/Devices")
list(REMOVE_ITEM INCLUDES_GRAPHICS "${CMAKE_SOURCE_DIR}/src/nanoFramework.Graphics/TouchPanel/Interfaces")

list(APPEND INCLUDES_GRAPHICS "${MY_GRAPHICS}/Graphics/Displays")
list(APPEND INCLUDES_GRAPHICS "${MY_GRAPHICS}/Graphics/Interfaces")
list(APPEND INCLUDES_GRAPHICS "${MY_GRAPHICS}/Graphics/Native")
list(APPEND INCLUDES_GRAPHICS "${MY_GRAPHICS}/TouchPanel/Core")
list(APPEND INCLUDES_GRAPHICS "${MY_GRAPHICS}/TouchPanel/Devices")
list(APPEND INCLUDES_GRAPHICS "${MY_GRAPHICS}/TouchPanel/Interfaces")
set_target_properties(NF_NativeAssemblies PROPERTIES INCLUDE_DIRECTORIES "${INCLUDES_GRAPHICS}")
