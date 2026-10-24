

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

bool File::DirectoryExists(const char *path)
{
    struct stat info;
    return stat(path, &info) == 0 && S_ISDIR(info.st_mode);
}

int32_t File::FileCount(const char *path)
{
    DIR *dir = opendir(path);
    if (!dir)
    {
        return -1;
    }
    int32_t count = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        if (entry->d_type == DT_REG)
        {
            count++;
        }
    }
    closedir(dir);
    return count;
}

int32_t File::DirectoryCount(const char *path)
{
    DIR *dir = opendir(path);

    if (!dir)
    {
        return -1;
    }

    int32_t count = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        if (entry->d_type == DT_DIR)
        {
            count++;
        }
    }
    closedir(dir);
    return count;
}

bool File::Move(const char *source, const char *destination)
{
    return rename(source, destination) == 0;
}

int32_t File::Read(const char *path, uint8_t *buffer, int32_t offset, int32_t length)
{
    FILE *fp = fopen(path, "rb");
    if (!fp)
    {
        return -1;
    }
    fseek(fp, offset, SEEK_SET);
    int32_t bytesRead = (int32_t)fread(buffer, 1, length, fp);
    fclose(fp);
    return bytesRead;
}

int32_t File::Write(const char *path, const uint8_t *data, int32_t offset, int32_t length, bool append)
{
    FILE *fp = fopen(path, append ? "ab" : "r+b");
    if (!fp && !append)
    {
        fp = fopen(path, "wb+");
    }
    if (!fp)
    {
        return -1;
    }
    if (!append)
    {
        fseek(fp, offset, SEEK_SET);
    }
    int32_t written = (int32_t)fwrite(data, 1, length, fp);
    fclose(fp);
    return written;
}

bool File::GetFileName(const char *path, int32_t index, char *fileName, int32_t maximumLength)
{
    DIR *dir = opendir(path);
    struct dirent *entry;
    int32_t current = 0;

    if (!dir)
    {
        return false;
    }

    while ((entry = readdir(dir)) != nullptr)
    {
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

bool File::GetDirectoryName(const char *path, int32_t index, char *directoryName, int32_t maximumLength)
{
    DIR *dir = opendir(path);

    if (!dir)
    {
        return false;
    }

    struct dirent *entry;
    int32_t current = 0;

    while ((entry = readdir(dir)) != nullptr)
    {
        if (entry->d_type == DT_DIR)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }
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

int64_t File::Length(const char *path)
{
    struct stat st;

    if (stat(path, &st) != 0)
    {
        return -1;
    }
    return st.st_size;
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
        .use_one_fat = true,
        .rootdir_entries = 0
    };

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
    return stat("/sdcard", &st) == 0;
}

uint64_t File::GetFreeBytes()
{
    FATFS *fs;
    DWORD freeClusters;
    f_getfree("0:", &freeClusters, &fs);
    uint64_t freeSectors = freeClusters * fs->csize;
    return freeSectors * 512ULL;
}
