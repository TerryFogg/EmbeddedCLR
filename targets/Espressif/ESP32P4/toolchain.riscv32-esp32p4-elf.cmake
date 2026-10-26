#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

include(CMakeForceCompiler)

# the name of the operating system for which CMake is to build
set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_C_FLAGS  "-march=rv32imafc_zicsr_zifencei_zaamo_zalrsc_xesploop_xespv -mabi=ilp32f -mtune=esp-base -Wno-frame-address" CACHE STRING "C Compiler Base Flags")
set(CMAKE_CXX_FLAGS "-march=rv32imafc_zicsr_zifencei_zaamo_zalrsc_xesploop_xespv -mabi=ilp32f  -mtune=esp-base -Wno-frame-address -Wno-literal-suffix -fno-rtti" CACHE STRING "C++ Compiler Base Flags")
set(CMAKE_ASM_FLAGS "-march=rv32imafc_zicsr_zifencei_zaamo_zalrsc_xesploop_xespv -mabi=ilp32f  -mtune=esp-base  ${CMAKE_ASM_FLAGS}" CACHE STRING "Asm Compiler Base Flags" FORCE)
#
#
set(CMAKE_C_COMPILER   "${ESP_TOOLCHAIN_DIR}/riscv32-esp-elf-gcc.exe")
set(CMAKE_CXX_COMPILER "${ESP_TOOLCHAIN_DIR}/riscv32-esp-elf-g++.exe")
set(CMAKE_ASM_COMPILER "${ESP_TOOLCHAIN_DIR}/riscv32-esp-elf-gcc.exe")
set(CMAKE_OBJCOPY      "${ESP_TOOLCHAIN_DIR}/riscv32-esp-elf-objcopy.exe")
set(CMAKE_OBJDUMP      "${ESP_TOOLCHAIN_DIR}/riscv32-esp-elf-objdump.exe")
set(CMAKE_SIZE         "${ESP_TOOLCHAIN_DIR}/riscv32-esp-elf-size.exe")
set(CMAKE_AR           "${ESP_TOOLCHAIN_DIR}/riscv32-esp-elf-ar.exe")

## root paths to search on the filesystem for cross-compiling
#get_filename_component(CMAKE_FIND_ROOT_PATH ${CMAKE_C_COMPILER} DIRECTORY CACHE)
#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
#
set(CMAKE_C_STANDARD 23 CACHE INTERNAL "C standard for all targets")
set(CMAKE_CXX_STANDARD 23 CACHE INTERNAL "C++ standard for all targets")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
#

#"-specs=C:/esp/v6.1/esp-idf/examples/get-started/hello_world/build/specs/picolibc.specs"
