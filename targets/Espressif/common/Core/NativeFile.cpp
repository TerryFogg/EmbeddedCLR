#include "NativeFile.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "esp_vfs_fat.h"
#include "esp_vfs.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"

// +---------------------------------------------------------------+
// |              Newlib/Picolib (C Standard Library)              |
// |           fopen, fread, fwrite, fclose, fseek, etc.           |
// +---------------------------------------------------------------+
// |                  VFS (Virtual File System)                    |
// |  POSIX API: open, read, write, close, lseek, stat, etc.       |
// +---------------------------------------------------------------+
// |                       FatFs Library                           |
// |       f_mount, f_open, f_read, f_write, f_close, f_mkfs       |
// +---------------------------------------------------------------+
// |                      Disk I/O Layer                           |
// |  ff_diskio_register, disk_read, disk_write, disk_ioctl, etc.  |
// +-------------------------------+-------------------------------+
// |   Wear Leveling  (optional)   |                               |
// |  wl_mount, wl_read, wl_write  |        SD/MMC Driver          |
// +-------------------------------+                               |
// |         esp_partition         |   sdmmc_read/write_sectors    |
// |    esp_partition_read/write   |                               |
// +-------------------------------+-------------------------------+
// |           SPI Flash           |           SD Card             |
// +-------------------------------+-------------------------------+

bool File::FileExists(const char *path)
{
    struct stat info;
    bool result = (stat(path, &info) == 0 && S_ISREG(info.st_mode));
    return result;
}

int File::FileOpen(const char *path, int mode, int access)
{
    BYTE fatMode = 0;
    FIL *file = new FIL();
    switch (mode)
    {
        case 1:
            fatMode = FA_CREATE_NEW;
            break;
        case 2:
            fatMode = FA_CREATE_ALWAYS;
            break;
        case 3:
            fatMode = FA_OPEN_EXISTING;
            break;
        case 4:
            fatMode = FA_OPEN_ALWAYS;
            break;
        case 5:
            fatMode = FA_CREATE_ALWAYS;
            break;
        case 6:
            fatMode = FA_OPEN_APPEND;
            break;
    }
    if (access & 1)
    {
        fatMode |= FA_READ;
    }
    if (access & 2)
    {
        fatMode |= FA_WRITE;
    }
    if (f_open(file, path, fatMode) == FR_OK)
    {
        return (int32_t)(uintptr_t)file;
    }
    else
    {
        delete file;
        return -1;
    }
}

bool File::FileClose(FIL *file)
{
    bool result = (f_close(file) == FR_OK);
    delete file;
    return result;
}

bool File::FileFlush(FIL *file)
{
    bool result = (f_sync(file) == FR_OK);
    return result;
}

bool File::Delete(const char *path)
{
    bool result = (unlink(path) == 0);
    return result;
}

bool File::FileMove(const char *source, const char *destination)
{
    bool result = (rename(source, destination) == 0);
    return result;
}

bool File::FileRename(const char *source, const char *destination)
{
    bool result = (rename(source, destination) == 0);
    return result;
}

bool File::FileCopy(const char *source, const char *destination, bool overwrite)
{
    FIL sourceFile;
    FIL destinationFile;
    FRESULT result;
    BYTE buffer[512];
    UINT bytesRead;
    UINT bytesWritten;
    bool success = true;

    result = f_open(&sourceFile, source, FA_READ);
    if (result != FR_OK)
    {
        return false;
    }
    result = f_open(&destinationFile, destination, FA_CREATE_ALWAYS | FA_WRITE);
    if (result != FR_OK)
    {
        f_close(&sourceFile);
        return false;
    }
    while (true)
    {
        result = f_read(&sourceFile, buffer, sizeof(buffer), &bytesRead);

        if (result != FR_OK)
        {
            success = false;
            break;
        }
        if (bytesRead == 0)
        {
            break;
        }
        result = f_write(&destinationFile, buffer, bytesRead, &bytesWritten);
        if (result != FR_OK || bytesWritten != bytesRead)
        {
            success = false;
            break;
        }
    }
    f_close(&destinationFile);
    f_close(&sourceFile);

    if (!success)
    {
        f_unlink(destination);
    }

    return success;
}

bool File::FileSetLength(FIL *file, int length)
{
    if (!file)
    {
        return false;
    }
    FRESULT result = f_lseek(file, length);
    if (result != FR_OK)
    {
        return false;
    }
    return (f_truncate(file) == FR_OK);
}

int File::FileGetLength(FIL *file)
{
    if (file == nullptr)
    {
        return -1;
    }

    return f_size(file);
}
int File::FileGetLength(const char *path)
{
    FIL file;
    FRESULT result = f_open(&file, path, FA_READ);
    if (result != FR_OK)
    {
        return -1;
    }
    int length = f_size(&file);
    f_close(&file);
    return length;
}   


int File::FileGetPosition(FIL *file)
{
    if (file == nullptr)
    {
        return -1;
    }
    return f_tell(file);
}

int File::FileRead(FIL *file, uint8_t *buffer, int32_t offset, int32_t length)
{
    FRESULT result;
    UINT bytesRead;

    if (!file || !buffer)
    {
        return -1;
    }

    result = f_lseek(file, offset);
    if (result != FR_OK)
    {
        return -1;
    }

    result = f_read(file, buffer, length, &bytesRead);
    if (result != FR_OK)
    {
        return -1;
    }
    return bytesRead;
}

int File::FileWrite(FIL *file, const uint8_t *data, int32_t offset, int32_t length, bool append)
{
    UINT bytesWritten = 0;

    if (file == nullptr || data == nullptr || length <= 0)
    {
        return -1;
    }
    if (append)
    {
        if (f_lseek(file, f_size(file)) != FR_OK)
        {
            return -1;
        }
    }
    else if (offset >= 0)
    {
        if (f_lseek(file, offset) != FR_OK)
        {
            return -1;
        }
    }
    FRESULT result = f_write(file, data, length, &bytesWritten);
    if (result != FR_OK)
    {
        return -1;
    }
    return bytesWritten;
}

bool File::FileSeek(FIL *file, int32_t offset)
{
    FRESULT result = f_lseek(file, offset);
    if (result == FR_OK)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int File::FileCount(const char *path)
{
    int count = 0;
    struct dirent *entry;

    DIR *dir = opendir(path);

    if (!dir)
    {
        return -1;
    }

    while ((entry = readdir(dir)) != nullptr)
    {
        if (entry->d_type == DT_REG)
        {
            ++count;
        }
    }

    closedir(dir);

    return count;
}

long File::FileGetLastWriteTime(const char *path)
{
    FILINFO info;
    if (f_stat(path, &info) == FR_OK)
    {
        WORD date = info.fdate;
        WORD time = info.ftime;
        // Convert date and time to a single long value (example implementation - adjust as needed)
        return (long)date << 16 | time;
    }
    else
    {
        return -1;
    }
}

int File::FileGetAttributes(FIL *file)
{
    if (file == nullptr)
    {
        return -1;
    }
    return file->obj.attr;
}
int File::FileSetAttributes(FIL *file, int attributes)
{
    if (file == nullptr)
    {
        return -1;
    }
    file->obj.attr = attributes;
    return 0;
}

bool File::CreateDirectory(const char *path)
{
    bool result = (mkdir(path, 0777) == 0);
    return result;
}

bool File::RemoveDirectory(const char *path)
{
    bool result = (rmdir(path) == 0);
    return result;
}

bool File::DirectoryExists(const char *path)
{
    struct stat info;
    bool result = (stat(path, &info) == 0 && S_ISDIR(info.st_mode));
    return result;
}

bool File::DirectoryMove(const char *source, const char *destination)
{
    bool result = (rename(source, destination) == FR_OK);
    return result;
}

bool File::DirectoryGetFileName(const char *path, int32_t index, char *fileName, int32_t maximumLength)
{
    if (path == nullptr || fileName == nullptr || maximumLength <= 0)
    {
        return false;
    }

    DIR *dir = opendir(path);

    if (dir == nullptr)
    {
        return false;
    }

    struct dirent *entry;
    int32_t current = 0;

    while ((entry = readdir(dir)) != nullptr)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        if (entry->d_type == DT_REG)
        {
            if (current == index)
            {
                strncpy(fileName, entry->d_name, maximumLength - 1);

                fileName[maximumLength - 1] = '\0';

                closedir(dir);
                return true;
            }

            current++;
        }
    }

    closedir(dir);

    return false;
}

bool File::DirectoryGetName(const char *path, int32_t index, char *directoryName, int32_t maximumLength)
{
    if (path == nullptr || directoryName == nullptr || maximumLength <= 0)
    {
        return false;
    }

    DIR *dir = opendir(path);

    if (dir == nullptr)
    {
        return false;
    }

    struct dirent *entry;
    int32_t current = 0;

    while ((entry = readdir(dir)) != nullptr)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        if (entry->d_type == DT_DIR)
        {
            if (current == index)
            {
                strncpy(directoryName, entry->d_name, maximumLength - 1);

                directoryName[maximumLength - 1] = '\0';

                closedir(dir);
                return true;
            }

            current++;
        }
    }

    closedir(dir);

    return false;
}

int File::DirectoryCount(const char *path)
{
    int32_t count = 0;
    struct dirent *entry;

    DIR *dir = opendir(path);
    if (!dir)
    {
        return -1;
    }
    while ((entry = readdir(dir)) != nullptr)
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            ++count;
        }
    }
    closedir(dir);
    return count;
}

long File::DriveFreeSpace(const char *path)
{
    DWORD freeClusters;
    FATFS *fileSystem;

    if (f_getfree(path, &freeClusters, &fileSystem) != FR_OK)
    {
        return -1;
    }
    long freeSize = freeClusters * fileSystem->csize * FF_MAX_SS;
    return freeSize;
}

long File::DriveTotalSize(const char *path)
{
    DWORD freeClusters;
    FATFS *fileSystem;

    if (f_getfree(path, &freeClusters, &fileSystem) != FR_OK)
    {
        return -1;
    }
    long totalsize = (fileSystem->n_fatent - 2) * fileSystem->csize * FF_MAX_SS;
    return totalsize;
}
bool File::IsDirectory(const char *parent, const char *name)
{
    char fullPath[256];
    struct stat st;

    snprintf(fullPath, sizeof(fullPath), "%s/%s", parent, name);
    return stat(fullPath, &st) == 0 && S_ISDIR(st.st_mode);
}

sdmmc_card_t *card = nullptr;

bool File::MountSdCard()
{
    esp_vfs_fat_sdmmc_mount_config_t mountConfig = {
        .format_if_mount_failed = false,
        .max_files = 16,
        .allocation_unit_size = 32 * 1024,
        .disk_status_check_enable = true,
        .use_one_fat = true};

    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    sdmmc_slot_config_t slotConfig = SDMMC_SLOT_CONFIG_DEFAULT();
    esp_err_t err = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slotConfig, &mountConfig, &card);
    if (err != ESP_OK)
    {
        return false;
    }
    sdmmc_card_print_info(stdout, card);
    return (err == ESP_OK);
}

void File::UnmountSdCard()
{
    esp_vfs_fat_sdcard_unmount("/sdcard", card);
    card = nullptr;
}

bool File::IsSdMounted()
{
    struct stat st;
    bool result = (stat("/sdcard", &st) == 0);
    return result;
}

