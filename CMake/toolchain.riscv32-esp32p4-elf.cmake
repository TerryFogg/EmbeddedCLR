#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

include(CMakeForceCompiler)



set(ESP_TOOLCHAIN_DIR
    "C:/Users/Terry/.espressif/tools/riscv32-esp-elf/esp-14.2.0_20260121/riscv32-esp-elf/bin")

set(CMAKE_C_COMPILER   "${ESP_TOOLCHAIN_DIR}/riscv32-esp-elf-gcc.exe")
set(CMAKE_CXX_COMPILER "${ESP_TOOLCHAIN_DIR}/riscv32-esp-elf-g++.exe")
set(CMAKE_ASM_COMPILER "${ESP_TOOLCHAIN_DIR}/riscv32-esp-elf-gcc.exe")
set(CMAKE_OBJCOPY      "${ESP_TOOLCHAIN_DIR}/riscv32-esp-elf-objcopy.exe")
set(CMAKE_OBJDUMP      "${ESP_TOOLCHAIN_DIR}/riscv32-esp-elf-objdump.exe")
set(CMAKE_SIZE         "${ESP_TOOLCHAIN_DIR}/riscv32-esp-elf-size.exe")
set(CMAKE_AR           "${ESP_TOOLCHAIN_DIR}/riscv32-esp-elf-ar.exe")

set(CMAKE_C_STANDARD 17 CACHE INTERNAL "C standard for all targets")
set(CMAKE_CXX_STANDARD 17 CACHE INTERNAL "C++ standard for all targets")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

