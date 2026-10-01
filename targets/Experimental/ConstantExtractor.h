#pragma once

#include <cstddef>
#include <cstdint>

// Initialize the extractor with the base address in flash where PE images are
// stored and the size (bytes) of the region to search. The memory must be
// readable (memory-mapped flash).
void CE_InitFlashImage(const void* flashBase, size_t flashSize);

// Find the PE image (within the initialized flash region) that contains the
// given type (namespace + class). Returns the pointer to the PE image base in
// flash or nullptr if not found.
const void* CE_FindPEByType(const char* szClass, const char* szNamespace);

// Extract a public literal integer value from a PE image at the given base
// address in flash. The caller provides the PE base pointer and the PE size
// (bytes) available at that address. The value is returned zero-extended in
// outVal. Returns true on success.
bool CE_ExtractPublicConstUnsignedAt(const void* peBase, const char* szClass, const char* szNamespace, const char* szField, uint64_t* outVal);

// Extract a public literal string constant. The string will be written into
// outBuf (null-terminated) up to outBufSize bytes. Returns true on success.
bool CE_ExtractPublicConstStringAt(const void* peBase, const char* szClass, const char* szNamespace, const char* szField, char* outBuf, int outBufSize);

// Flash-based wrappers: pass a pointer to the PE image in flash and its size.
bool ExtractPublicConstUnsignedFromFlash(const void* flashBase, size_t flashSize, const char* szClass, const char* szNamespace, const char* szField, uint64_t* outVal);
// Extract string constant into caller-supplied buffer (no std::string dependency).
bool ExtractPublicConstStringFromFlash(const void* flashBase, size_t flashSize, const char* szClass, const char* szNamespace, const char* szField, char* outBuf, int outBufSize);
