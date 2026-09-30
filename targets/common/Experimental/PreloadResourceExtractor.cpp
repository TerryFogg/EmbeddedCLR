// Pre-load resource extractor implementation
// Scans the deployment assembly region and extracts a resource file's raw bytes

#include "stdafx.h"

#include "Core.h"
#include <nanoPAL_BlockStorage.h>
#include <nanoCLR_Types.h>
#include "PreloadResourceExtractor.h"

// externals from StringTableData.cpp
extern const size_t c_CLR_StringTable_Size;
extern const CLR_STRING c_CLR_StringTable_Lookup[];
extern const char c_CLR_StringTable_Data[];

static const char *GetStringFromAssembly(const CLR_RECORD_ASSEMBLY *header, CLR_STRING idx)
{
    static const CLR_STRING iMax = (CLR_STRING)(0xFFFF - c_CLR_StringTable_Size);

    if (idx >= iMax)
    {
        return &c_CLR_StringTable_Data[c_CLR_StringTable_Lookup[(CLR_STRING)0xFFFF - idx]];
    }

    return (const char *)((const uint8_t *)header + header->startOfTables[TBL_Strings] + idx);
}

bool Preload_GetResourceFileData(const char *assemblyName, const char *resourceFileName, unsigned char **outBuf, uint32_t *outSize)
{
    NATIVE_PROFILE_CLR_STARTUP();

    if (!assemblyName || !resourceFileName || !outBuf || !outSize)
        return false;

    *outBuf = NULL;
    *outSize = 0;

    BlockStorageStream stream;
    memset(&stream, 0, sizeof(stream));

    if (!BlockStorageStream_Initialize(&stream, BlockUsage_DEPLOYMENT))
        return false;

    const unsigned int headerInBytes = sizeof(CLR_RECORD_ASSEMBLY);

    unsigned char *headerBuffer = NULL;
    unsigned char *assembliesBuffer = NULL;

    bool isXIP = (stream.Flags & BLOCKSTORAGESTREAM_c_BlockStorageStream__XIP) ||
                 (stream.Flags & BLOCKSTORAGESTREAM_c_BlockStorageStream__MemoryMapped);

    if (!isXIP)
    {
        headerBuffer = (unsigned char *)platform_malloc(headerInBytes);
        if (!headerBuffer)
            return false;
        memset(headerBuffer, 0, headerInBytes);
    }

    bool found = false;

    while (stream.CurrentIndex < stream.Length)
    {
        if ((stream.Length - stream.CurrentIndex) < headerInBytes)
            break;

        if (!BlockStorageStream_Read(&stream, &headerBuffer, headerInBytes))
            break;

        const CLR_RECORD_ASSEMBLY *header = (const CLR_RECORD_ASSEMBLY *)headerBuffer;

        if (!header->GoodHeader())
            continue;

        unsigned int assemblySizeInByte = ROUNDTOMULTIPLE(header->TotalSize(), CLR_UINT32);

        if (!isXIP)
        {
            assembliesBuffer = (unsigned char *)platform_malloc(assemblySizeInByte);
            if (!assembliesBuffer)
            {
                platform_free(headerBuffer);
                return false;
            }
            memset(assembliesBuffer, 0, assemblySizeInByte);
        }

        // rewind to include header and read the whole assembly
        BlockStorageStream_Seek(&stream, -headerInBytes, BlockStorageStream_SeekCurrent);

        if (!BlockStorageStream_Read(&stream, &assembliesBuffer, assemblySizeInByte))
        {
            if (!isXIP)
                platform_free(assembliesBuffer);
            break;
        }

        header = (const CLR_RECORD_ASSEMBLY *)assembliesBuffer;

        if (!header->GoodAssembly())
        {
            if (!isXIP)
                platform_free(assembliesBuffer);
            continue;
        }

        // compare assembly name
        const char *asmName = GetStringFromAssembly(header, header->assemblyName);

        if (strcmp(asmName, assemblyName) != 0)
        {
            if (!isXIP)
                platform_free(assembliesBuffer);
            continue;
        }

        // found matching assembly; search resource files
        const uint32_t resourcesFilesBytes = (uint32_t)header->SizeOfTable(TBL_ResourcesFiles);
        uint32_t numFiles = 0;
        if (resourcesFilesBytes >= sizeof(CLR_RECORD_RESOURCE_FILE))
            numFiles = resourcesFilesBytes / sizeof(CLR_RECORD_RESOURCE_FILE);

        const CLR_RECORD_RESOURCE_FILE *filesBase = (const CLR_RECORD_RESOURCE_FILE *)((const uint8_t *)header + header->startOfTables[TBL_ResourcesFiles]);

        for (uint32_t i = 0; i < numFiles; i++)
        {
            const CLR_RECORD_RESOURCE_FILE *rf = &filesBase[i];

            const char *rfn = GetStringFromAssembly(header, rf->name);

            if (!strcmp(rfn, resourceFileName))
            {
                // compute resource range
                const uint32_t totalResourcesBytes = (uint32_t)header->SizeOfTable(TBL_Resources);
                uint32_t totalResources = 0;
                if (totalResourcesBytes >= sizeof(CLR_RECORD_RESOURCE))
                    totalResources = totalResourcesBytes / sizeof(CLR_RECORD_RESOURCE);

                const CLR_RECORD_RESOURCE *resBase = (const CLR_RECORD_RESOURCE *)((const uint8_t *)header + header->startOfTables[TBL_Resources]);

                uint32_t firstIndex = rf->offset;
                if (firstIndex >= totalResources)
                    continue; // invalid

                const CLR_RECORD_RESOURCE *firstRes = &resBase[firstIndex];

                uint32_t startOffset = firstRes->offset;
                uint32_t endOffset = 0;

                uint32_t afterIndex = firstIndex + rf->numberOfResources;

                if (afterIndex < totalResources)
                {
                    const CLR_RECORD_RESOURCE *nextRes = &resBase[afterIndex];
                    endOffset = nextRes->offset;
                    // subtract padding indicated by nextRes flags
                    endOffset -= (nextRes->flags & CLR_RECORD_RESOURCE::FLAGS_PaddingMask);
                }
                else
                {
                    // use entire ResourcesData table size
                    endOffset = (uint32_t)header->SizeOfTable(TBL_ResourcesData);
                }

                if (endOffset <= startOffset)
                {
                    // nothing to copy
                    if (!isXIP)
                        platform_free(assembliesBuffer);
                    found = false;
                    break;
                }

                uint32_t size = endOffset - startOffset;

                const uint8_t *dataPtr = (const uint8_t *)header + header->startOfTables[TBL_ResourcesData] + startOffset;

                unsigned char *buf = (unsigned char *)platform_malloc(size);
                if (!buf)
                {
                    if (!isXIP)
                        platform_free(assembliesBuffer);
                    found = false;
                    break;
                }

                memcpy(buf, dataPtr, size);

                *outBuf = buf;
                *outSize = size;

                if (!isXIP)
                    platform_free(assembliesBuffer);

                found = true;
                break;
            }
        }

        if (!isXIP && !found)
            platform_free(assembliesBuffer);

        if (found)
            break;
    }

    if (!isXIP && headerBuffer)
        platform_free(headerBuffer);

    return found;
}
