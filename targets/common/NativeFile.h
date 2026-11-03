#pragma once
//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include <stdint.h>
#include "ff.h"

class File
{
  public:
    static int FileOpen(const char *path, int mode, int access);
    static bool FileClose(FIL *file);
    static bool FileFlush(FIL *file);
    static bool FileExists(const char *path);
    static int FileCount(const char *path);
    static bool FileMove(const char *source, const char *destination);
    static bool FileRename(const char *source, const char *destination, bool overwrite);
    static bool FileCopy(const char *source, const char *destination, bool overwrite);
    static bool FileSetLength(FIL *file, int length);
    static int FileGetLength(FIL *file);
    static int FileGetLength(const char *path);
    static int FileGetPosition(FIL *file);
    static int FileRead(FIL *file, uint8_t *buffer, int32_t offset, int32_t length);
    static int FileWrite(FIL *file, const uint8_t *data, int32_t offset, int32_t length, bool append);
    static bool FileSeek(FIL *file, int32_t offset);
    static long FileGetCreationTime(const char *path);
    static long FileGetLastWriteTime(const char *path);
    static long FileSetLastWriteTime(const char *path, long lastWriteTime);
    static int FileGetAttributes(FIL *file);
    static int FileSetAttributes(FIL *file, int attributes);



    static bool Delete(const char *path);
    static bool DirectoryExists(const char *path);
    static bool CreateDirectory(const char *path);
    static bool RemoveDirectory(const char *path);
    static bool DirectoryMove(const char *source, const char *destination);

    static int DirectoryCount(const char *path);
    static bool FileRename(const char *source, const char *destination);
    static bool DirectoryGetFileName(const char *path, int32_t index, char *fileName, int32_t maximumLength);
    static bool DirectoryGetName(const char *path, int32_t index, char *directoryName, int32_t maximumLength);
    static bool IsDirectory(const char *parent, const char *name);
    static long  DriveTotalSize(const char *path);
    static long  DriveFreeSpace(const char *path);
    static bool MountSdCard();
    static void UnmountSdCard();
    static bool IsSdMounted();
};
