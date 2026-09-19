// Preload resource extractor API - extract raw resource file bytes from deployment assemblies
#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Extract the raw concatenated resource payload for the named resource file
// in the specified assembly. The function allocates a buffer via platform_malloc
// which the caller must free with platform_free().
// Returns true on success, outBuf and outSize are set.
bool Preload_GetResourceFileData(const char *assemblyName, const char *resourceFileName, unsigned char **outBuf, uint32_t *outSize);

#ifdef __cplusplus
}
#endif
