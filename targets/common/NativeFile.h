#pragma once
//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include <stdint.h>

class File
{
  public:
    static bool Exists(const char *path);
    static bool Delete(const char *path);
    static bool CreateDirectory(const char *path);
    static bool DirectoryExists(const char *path);
    static int32_t FileCount(const char *path);
    static int32_t DirectoryCount(const char *path);
    static bool Move(const char *source, const char *destination);
    static int32_t Read(const char *path, uint8_t *buffer, int32_t offset, int32_t length);
    static int32_t Write(const char *path, const uint8_t *data, int32_t offset, int32_t length, bool append);
    static bool GetFileName(const char *path, int32_t index, char *fileName, int32_t maximumLength);
    static bool GetDirectoryName(const char *path, int32_t index, char *directoryName, int32_t maximumLength);
    static int64_t Length(const char *path);
};
