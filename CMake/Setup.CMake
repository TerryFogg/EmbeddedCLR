
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(NANOCLR_PROJECT_NAME "nanoCLR" CACHE INTERNAL "nanoCLR project name")
set(TARGET_BOARD "ESP32_P4" CACHE INTERNAL "Target board name")

set(TARGET_FAMILY "Esp32" CACHE INTERNAL "Target family")
set(TARGET_CONFIGURATION_PATH  ${CMAKE_SOURCE_DIR}/targets/ESP32/ESP32_P4/Configuration CACHE INTERNAL "Target configuration path")

set(RTOS_ESP32_CHECK TRUE CACHE INTERNAL "Check if RTOS is enabled for ESP32")
set(ESP32_CPU_TYPE "riscv" CACHE INTERNAL "Setting CPU type")
set(BUILD_VERSION 0.0.0.0)

set(NF_TRACE_TO_STDIO FALSE CACHE INTERNAL "Trace to stdio")
set(NF_FEATURE_SUPPORT_REFLECTION TRUE CACHE INTERNAL "Support reflection feature")
set(NF_FEATURE_BINARY_SERIALIZATION TRUE CACHE INTERNAL "Binary serialization feature")
set(NF_FEATURE_HAS_CONFIG_BLOCK FALSE CACHE INTERNAL "Has config block feature")

set(Collections TRUE CACHE INTERNAL "Collections feature")
set(Cryptography FALSE CACHE INTERNAL "Cryptography feature")
set(Esp32Hardware FALSE CACHE INTERNAL "ESP32 hardware feature")
set(Hashing TRUE CACHE INTERNAL "Hashing feature")
set(FileSystem FALSE CACHE INTERNAL "File system feature")
set(Graphics TRUE CACHE INTERNAL "Graphics feature")
set(Mathematics TRUE CACHE INTERNAL "Mathematics feature")
set(Network FALSE CACHE INTERNAL "Network feature")
set(OneWire FALSE CACHE INTERNAL "1-Wire feature")
set(ResourceManager TRUE CACHE INTERNAL "Resource manager feature")
set(Text TRUE CACHE INTERNAL "Text feature")
set(UsbStream FALSE CACHE INTERNAL "USB stream feature")
set(WireProtocol TRUE CACHE INTERNAL "Wire protocol feature")

set(NF_SECURITY_MBEDTLS ON CACHE INTERNAL "MbedTLS must be ON for IDF build")  ##???????????
