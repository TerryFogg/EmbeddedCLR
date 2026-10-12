

#include "NativeFile.h"

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

bool File::Exists(const char *path)
{
    struct stat info;
    return stat(path, &info) == 0 && S_ISREG(info.st_mode);
}

bool File::Delete(const char *path)
{
    return unlink(path) == 0;
}

bool File::CreateDirectory(const char *path)
{
    return mkdir(path, 0777) == 0;
}

bool File::Move(const char *source, const char *destination)
{
    return rename(source, destination) == 0;
}

bool File::DirectoryExists(const char *path)
{
    struct stat info;
    return stat(path, &info) == 0 && S_ISDIR(info.st_mode);
}

int32_t File::Read(const char *path, uint8_t *buffer, int32_t offset, int32_t length)
{
    FILE *file = fopen(path, "rb");
    if (file == nullptr)
    {
        return -1;
    }
    int32_t bytesRead = fread(buffer + offset, 1, length, file);
    fclose(file);
    return bytesRead;
}

int32_t File::Write(const char *path, const uint8_t *data, int32_t offset, int32_t length, bool append)
{
    FILE *file = fopen(path, append ? "ab" : "wb");
    if (file == nullptr)
    {
        return -1;
    }
    int32_t bytesWritten = fwrite(data + offset, 1, length, file);
    fclose(file);
    return bytesWritten;
}

int64_t File::Length(const char *path)
{
    struct stat info;
    if (stat(path, &info) != 0)
    {
        return -1;
    }
    return info.st_size;
}

int32_t File::FileCount(const char *path)
{
    struct stat info;
    char fullPath[256];
    int32_t count = 0;
    dirent *entry;
    DIR *directory = opendir(path);

    while ((entry = readdir(directory)) != nullptr)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        if (stat(fullPath, &info) == 0)
        {
            if (S_ISREG(info.st_mode))
            {
                count++;
            }
        }
    }
    closedir(directory);
    return count;
}

int32_t File::DirectoryCount(const char *path)
{
    struct stat info;
    char fullPath[256];
    int32_t count = 0;
    dirent *entry;
    DIR *directory = opendir(path);

    while ((entry = readdir(directory)) != nullptr)
    {
        if (strcmp(entry->d_name, ".") == 0)
        {
            continue;
        }
        if (strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        if (stat(fullPath, &info) == 0)
        {
            if (S_ISDIR(info.st_mode))
            {
                count++;
            }
        }
    }
    closedir(directory);
    return count;
}
bool File::GetFileName(const char *path, int32_t index, char *fileName, int32_t maximumLength)
{
    DIR *directory = opendir(path);
    if (directory == nullptr)
    {
        return false;
    }
    int32_t currentIndex = 0;
    dirent *entry;
    while ((entry = readdir(directory)) != nullptr)
    {
        if (strcmp(entry->d_name, ".") == 0)
        {
            continue;
        }
        if (strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        if (currentIndex == index)
        {
            strncpy(fileName, entry->d_name, maximumLength - 1);
            fileName[maximumLength - 1] = '\0';
            closedir(directory);
            return true;
        }
        currentIndex++;
    }
    closedir(directory);
    return false;
}
#include <sys/stat.h>

bool File::GetDirectoryName(const char *path, int32_t index, char *directoryName, int32_t maximumLength)
{
    int32_t currentIndex = 0;
    struct stat info;
    char fullPath[256];
    dirent *entry;
    DIR *directory = opendir(path);

    while ((entry = readdir(directory)) != nullptr)
    {
        if (strcmp(entry->d_name, ".") == 0)
        {
            continue;
        }
        if (strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        if (stat(fullPath, &info) != 0)
        {
            continue;
        }
        if (!S_ISDIR(info.st_mode))
        {
            continue;
        }
        if (currentIndex == index)
        {
            strncpy(directoryName, entry->d_name, maximumLength - 1);
            directoryName[maximumLength - 1] = '\0';
            closedir(directory);
            return true;
        }
        currentIndex++;
    }
    closedir(directory);
    return false;
}
