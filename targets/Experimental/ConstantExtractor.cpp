// Helper to extract public const (literal) field values from managed assemblies
// Minimal implementation: supports integer (I4/I8), unsigned (U4/U8), boolean, float (R4) and double (R8).

#include "ConstantExtractor.h"
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>

// Forward declare main extractor used by wrappers defined earlier in this file
bool ExtractPublicConstFromImage(
    const uint8_t* image,
    size_t imageSize,
    const char *szClass,
    const char *szNamespace,
    const char *szField,
    void *outValue,
    int outSize);

// Provide a safe strnlen for platforms where strnlen isn't available
static size_t safe_strnlen(const char* s, size_t maxlen)
{
    size_t i = 0;
    for (; i < maxlen; ++i)
    {
        if (s[i] == '\0') break;
    }
    return i;
}

// Minimal metadata reader for extracting public literal (const) integer and string
// values directly from a PE/CLI image in memory. This implementation is intentionally
// small and only implements the features required by the ESP32 use-case: locate
// the metadata root, read the #~ stream (tables), read #Strings and #Blob heaps,
// find a TypeDef by namespace+name, enumerate its fields and look up the Constant
// table entry for a matching public literal field.

// Helper: read values assuming little-endian target.

static bool read_strings_heap_string(
    const uint8_t *stringsHeap,
    size_t stringsSize,
    size_t strOffset,
    char *outBuf,
    int outBufSize)
{
    if (strOffset >= stringsSize)
    {
        return false;
    }
    const char *src = (const char *)(stringsHeap + strOffset);
    size_t maxLen = safe_strnlen(src, stringsSize - strOffset);
    if (maxLen == stringsSize - strOffset)
    {
        return false;
    }
    if (outBuf && outBufSize > 0)
    {
        int cpy = (int)maxLen < (outBufSize - 1) ? (int)maxLen : (outBufSize - 1);
        memcpy(outBuf, src, cpy);
        outBuf[cpy] = '\0';
    }
    return true;
}
static bool read_u16(const uint8_t *image, size_t imageSize, size_t offset, uint16_t &out)
{
    if (offset + 2 > imageSize)
    {
        return false;
    }
    out = (uint16_t)image[offset] | ((uint16_t)image[offset + 1] << 8);
    return true;
}
static bool read_u32(const uint8_t *image, size_t imageSize, size_t offset, uint32_t &out)
{
    if (offset + 4 > imageSize)
    {
        return false;
    }
    out = (uint32_t)image[offset] | ((uint32_t)image[offset + 1] << 8) | ((uint32_t)image[offset + 2] << 16) |
          ((uint32_t)image[offset + 3] << 24);
    return true;
}
// Global flash image pointer set by CE_InitFlashImage
static const uint8_t *g_flashBase = nullptr;
static size_t g_flashSize = 0;

void CE_InitFlashImage(const void *flashBase, size_t flashSize)
{
    g_flashBase = (const uint8_t *)flashBase;
    g_flashSize = flashSize;
}

// Simple fixed-size cache mapping (namespace,class) -> PE base pointer to avoid
// scanning flash repeatedly for the same type. This avoids using STL and keeps
// memory usage bounded for embedded targets.
struct PECacheEntry
{
    char ns[64];
    char cls[64];
    const void *peBase;
};

static PECacheEntry g_peCache[16];
static int g_peCacheCount = 0;

static const void *lookup_pe_cache(const char *szClass, const char *szNamespace)
{
    if (!szClass || !szNamespace)
    {
        return nullptr;
    }
    for (int i = 0; i < g_peCacheCount; ++i)
    {
        if (strncmp(g_peCache[i].cls, szClass, sizeof(g_peCache[i].cls)) == 0 &&
            strncmp(g_peCache[i].ns, szNamespace, sizeof(g_peCache[i].ns)) == 0)
        {
            return g_peCache[i].peBase;
        }
    }
    return nullptr;
}

static void add_pe_cache(const char *szClass, const char *szNamespace, const void *peBase)
{
    if (!szClass || !szNamespace || !peBase)
    {
        return;
    }
    // If already present, update and return
    for (int i = 0; i < g_peCacheCount; ++i)
    {
        if (strncmp(g_peCache[i].cls, szClass, sizeof(g_peCache[i].cls)) == 0 &&
            strncmp(g_peCache[i].ns, szNamespace, sizeof(g_peCache[i].ns)) == 0)
        {
            g_peCache[i].peBase = peBase;
            return;
        }
    }
    if (g_peCacheCount >= (int)(sizeof(g_peCache) / sizeof(g_peCache[0])))
    {
        // simple eviction: overwrite oldest (index 0) by shifting left
        for (int i = 1; i < g_peCacheCount; ++i)
        {
            g_peCache[i - 1] = g_peCache[i];
        }
        g_peCacheCount -= 1;
    }
    // append new
    int idx = g_peCacheCount;
    memset(g_peCache[idx].ns, 0, sizeof(g_peCache[idx].ns));
    memset(g_peCache[idx].cls, 0, sizeof(g_peCache[idx].cls));
    // copy with truncation
    strncpy(g_peCache[idx].ns, szNamespace, sizeof(g_peCache[idx].ns) - 1);
    strncpy(g_peCache[idx].cls, szClass, sizeof(g_peCache[idx].cls) - 1);
    g_peCache[idx].peBase = peBase;
    g_peCacheCount += 1;
}

// Read a compressed unsigned int (ECMA-335 compressed uint) from blob at offset.
// Returns false on bounds error. outLenBytes will receive how many bytes were consumed.
static bool read_compressed_uint(
    const uint8_t *image,
    size_t imageSize,
    size_t offset,
    uint32_t &out,
    size_t &outLenBytes)
{
    if (offset >= imageSize)
    {
        return false;
    }
    uint8_t first = image[offset];
    if ((first & 0x80) == 0)
    {
        out = first;
        outLenBytes = 1;
        return true;
    }
    else if ((first & 0xC0) == 0x80)
    {
        // two bytes
        if (offset + 2 > imageSize)
        {
            return false;
        }
        out = ((first & 0x3F) << 8) | image[offset + 1];
        outLenBytes = 2;
        return true;
    }
    else if ((first & 0xE0) == 0xC0)
    {
        // four bytes
        if (offset + 4 > imageSize)
        {
            return false;
        }
        out = ((first & 0x1F) << 24) | ((uint32_t)image[offset + 1] << 16) | ((uint32_t)image[offset + 2] << 8) |
              image[offset + 3];
        outLenBytes = 4;
        return true;
    }
    return false;
}

// Flash-based wrappers: if the PE image is memory-mapped at a known flash address
// and accessible from the CPU, call these with the base pointer and size.
bool ExtractPublicConstUnsignedFromFlash(
    const void *flashBase,
    size_t flashSize,
    const char *szClass,
    const char *szNamespace,
    const char *szField,
    uint64_t *outVal)
{
    if (!flashBase || !outVal)
    {
        return false;
    }
    // Zero the output and ask the in-memory extractor to fill it.
    memset(outVal, 0, sizeof(uint64_t));
    return ExtractPublicConstFromImage(
        (const uint8_t *)flashBase,
        flashSize,
        szClass,
        szNamespace,
        szField,
        outVal,
        (int)sizeof(uint64_t));
}

bool ExtractPublicConstStringFromFlash(
    const void *flashBase,
    size_t flashSize,
    const char *szClass,
    const char *szNamespace,
    const char *szField,
    char *outBuf,
    int outBufSize)
{
    if (!flashBase || !outBuf || outBufSize <= 0)
    {
        return false;
    }
    // Use the extraction helper to write into the provided buffer directly.
    if (!ExtractPublicConstFromImage(
            (const uint8_t *)flashBase,
            flashSize,
            szClass,
            szNamespace,
            szField,
            outBuf,
            outBufSize))
    {
        return false;
    }
    return true;
}

// Read a null-terminated UTF8 SerString from blob heap location (per spec)
static bool read_serstring(const uint8_t *blobHeap, size_t blobSize, size_t blobOffset, char *outBuf, int outBufSize)
{
    // A serstring is either null (0xFF) or compressed-length-prefixed UTF8 bytes then 0x00.
    if (blobOffset >= blobSize)
    {
        return false;
    }
    if (blobHeap[blobOffset] == 0xFF)
    {
        if (outBuf && outBufSize > 0)
        {
            outBuf[0] = '\0';
        }
        return true;
    }
    uint32_t len = 0;
    size_t lenBytes = 0;
    if (!read_compressed_uint(blobHeap, blobSize, blobOffset, len, lenBytes))
    {
        return false;
    }
    size_t start = blobOffset + lenBytes;
    if (start + len > blobSize)
    {
        return false;
    }
    if (outBuf && outBufSize > 0)
    {
        int cpy = (int)len < (outBufSize - 1) ? (int)len : (outBufSize - 1);
        memcpy(outBuf, blobHeap + start, cpy);
        outBuf[cpy] = '\0';
    }
    return true;
}

// Main simplified reader function. Assumes 'image' points to a valid PE/COFF CLI image in memory.
// Internal: extract or check type. If szField==nullptr, function checks whether
// the given type (szClass+szNamespace) exists in the image and returns true
// if found. If szField != nullptr, it attempts to extract the constant value
// into outValue (size outSize).
bool ExtractPublicConstFromImage(
    const uint8_t *image,
    size_t imageSize,
    const char *szClass,
    const char *szNamespace,
    const char *szField, // nullable: if null, only check for type existence
    void *outValue,
    int outSize)
{
    if (!image || !szClass)
    {
        return false;
    }
    bool onlyCheckType = (szField == nullptr);

    // 1) Parse DOS header e_lfanew
    uint32_t e_lfanew = 0;
    if (!read_u32(image, imageSize, 0x3C, e_lfanew))
    {
        return false;
    }
    if (e_lfanew + 4 > imageSize)
    {
        return false;
    }
    // Check PE signature
    if (image[e_lfanew] != 'P' || image[e_lfanew + 1] != 'E' || image[e_lfanew + 2] != 0 || image[e_lfanew + 3] != 0)
    {
        return false;
    }

    // 2) Locate optional header and data directories
    size_t peHeader = e_lfanew + 4;
    // Skip COFF FileHeader (20 bytes)
    if (peHeader + 20 + 2 > imageSize)
    {
        return false;
    }
    uint16_t machine = 0;
    if (!read_u16(image, imageSize, peHeader, machine))
    {
        return false;
    } // not used
    // NumberOfSections at offset 6
    uint16_t numberOfSections = 0;
    if (!read_u16(image, imageSize, peHeader + 2, numberOfSections))
    {
        return false;
    }
    // SizeOfOptionalHeader at offset 16
    uint16_t sizeOfOptionalHeader = 0;
    if (!read_u16(image, imageSize, peHeader + 16, sizeOfOptionalHeader))
    {
        return false;
    }

    size_t optionalHeader = peHeader + 20;
    if (optionalHeader + sizeOfOptionalHeader > imageSize)
    {
        return false;
    }

    // Determine location of data directories. For PE32 vs PE32+ the offset differs.
    uint16_t magic = 0;
    if (!read_u16(image, imageSize, optionalHeader, magic))
    {
        return false;
    }
    size_t dataDirsOffset = 0;
    if (magic == 0x10b)
    {
        // PE32
        dataDirsOffset = optionalHeader + 96; // offsetof(IMAGE_OPTIONAL_HEADER32, DataDirectory)
    }
    else if (magic == 0x20b)
    {
        // PE32+
        dataDirsOffset = optionalHeader + 112; // offsetof(IMAGE_OPTIONAL_HEADER64, DataDirectory)
    }
    else
    {
        return false;
    }

    // CLI header is IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR = index 14
    size_t cliDirRvaOff = dataDirsOffset + 14 * 8; // each directory is 8 bytes (RVA + Size)
    if (cliDirRvaOff + 8 > imageSize)
    {
        return false;
    }
    uint32_t cliRva = 0;
    uint32_t cliSize = 0;
    if (!read_u32(image, imageSize, cliDirRvaOff, cliRva))
    {
        return false;
    }
    if (!read_u32(image, imageSize, cliDirRvaOff + 4, cliSize))
    {
        return false;
    }
    if (cliRva == 0)
    {
        return false;
    }

    // We need to convert RVA to file offset. Find section headers and locate containing section.
    size_t sectionTable = optionalHeader + sizeOfOptionalHeader;
    size_t sectionEntrySize = 40; // IMAGE_SECTION_HEADER
    size_t sectionFoundOffset = 0;
    bool sectionFound = false;
    for (uint16_t i = 0; i < numberOfSections; ++i)
    {
        size_t ent = sectionTable + i * sectionEntrySize;
        if (ent + sectionEntrySize > imageSize)
        {
            return false;
        }
        (void)0; // suppress unused-variable warning placeholder
        uint32_t virtualAddress = 0;
        uint32_t sizeOfRawData = 0;
        uint32_t pointerToRawData = 0;
        // VirtualAddress at offset 12, SizeOfRawData at 16, PointerToRawData at 20
        if (!read_u32(image, imageSize, ent + 12, virtualAddress))
        {
            return false;
        }
        if (!read_u32(image, imageSize, ent + 16, sizeOfRawData))
        {
            return false;
        }
        if (!read_u32(image, imageSize, ent + 20, pointerToRawData))
        {
            return false;
        }
        if (cliRva >= virtualAddress && cliRva < virtualAddress + sizeOfRawData)
        {
            sectionFound = true;
            sectionFoundOffset = pointerToRawData + (cliRva - virtualAddress);
            break;
        }
    }
    if (!sectionFound)
    {
        return false;
    }

    // Read CLI header to get Metadata RVA
    uint32_t metaRva = 0;
    if (!read_u32(image, imageSize, sectionFoundOffset + 8, metaRva))
    {
        return false;
    } // Metadata RVA at offset 8
    if (metaRva == 0)
    {
        return false;
    }

    // Convert metadata RVA to file offset (same section scan)
    size_t metaOffset = 0;
    bool metaFound = false;
    for (uint16_t i = 0; i < numberOfSections; ++i)
    {
        size_t ent = sectionTable + i * sectionEntrySize;
        if (ent + sectionEntrySize > imageSize)
        {
            return false;
        }
        uint32_t virtualAddress = 0;
        uint32_t sizeOfRawData = 0;
        uint32_t pointerToRawData = 0;
        if (!read_u32(image, imageSize, ent + 12, virtualAddress))
        {
            return false;
        }
        if (!read_u32(image, imageSize, ent + 16, sizeOfRawData))
        {
            return false;
        }
        if (!read_u32(image, imageSize, ent + 20, pointerToRawData))
        {
            return false;
        }
        if (metaRva >= virtualAddress && metaRva < virtualAddress + sizeOfRawData)
        {
            metaFound = true;
            metaOffset = pointerToRawData + (metaRva - virtualAddress);
            break;
        }
    }
    if (!metaFound)
    {
        return false;
    }

    // Metadata Root: signature 'BSJB' (0x424A5342)
    if (metaOffset + 4 > imageSize)
    {
        return false;
    }
    if (image[metaOffset] != 'B' || image[metaOffset + 1] != 'S' || image[metaOffset + 2] != 'J' ||
        image[metaOffset + 3] != 'B')
    {
        return false;
    }

    // Skip to streams: at offset metaOffset + 8 is version length etc. Read stream headers count
    // Metadata root header layout: signature(4) + major(2) + minor(2) + reserved(4) + versionLength(4) +
    // version[versionLength] + flags(2) + streams(2)
    uint32_t versionLength = 0;
    if (!read_u32(image, imageSize, metaOffset + 8, versionLength))
    {
        return false;
    }
    size_t streamsCountOffset = metaOffset + 12 + versionLength;
    if (streamsCountOffset + 4 > imageSize)
    {
        return false;
    }
    uint16_t flags = 0;
    uint16_t streams = 0;
    if (!read_u16(image, imageSize, metaOffset + 12 + versionLength, flags))
    {
        return false;
    }
    if (!read_u16(image, imageSize, metaOffset + 12 + versionLength + 2, streams))
    {
        return false;
    }

    size_t streamHeaderStart = metaOffset + 12 + versionLength + 4;
    if (streamHeaderStart > imageSize)
    {
        return false;
    }

    // Find #~ (tables), #Strings and #Blob stream offsets
    size_t tablesOffset = 0;
    size_t stringsOffset = 0, stringsSize = 0;
    size_t blobOffset = 0, blobSize = 0;

    size_t cur = streamHeaderStart;
    for (uint16_t i = 0; i < streams; ++i)
    {
        if (cur + 8 > imageSize)
        {
            return false;
        }
        uint32_t offset = 0;
        uint32_t size = 0;
        if (!read_u32(image, imageSize, cur, offset))
        {
            return false;
        }
        if (!read_u32(image, imageSize, cur + 4, size))
        {
            return false;
        }
        size_t nameStart = cur + 8;
        if (nameStart >= imageSize)
        {
            return false;
        }
        // name is null-terminated padded to 4-byte boundary
        const char *namePtr = (const char *)(image + nameStart);
        size_t nameLen = safe_strnlen(namePtr, imageSize - nameStart);
        if (nameLen == imageSize - nameStart)
        {
            return false;
        }
        std::string name(namePtr, nameLen);
        size_t nextHeader = nameStart + nameLen + 1;
        // align to 4
        while ((nextHeader - metaOffset) % 4 != 0)
            nextHeader++;
        cur = nextHeader;

        size_t fileOffset = 0;
        // convert RVA (offset) relative to metadata root to file offset: metadata root offset + offset
        fileOffset = metaOffset + offset;

        if (name == "#~" || name == "#-")
        {
            tablesOffset = fileOffset;
            (void)size;
        }
        else if (name == "#Strings")
        {
            stringsOffset = fileOffset;
            stringsSize = size;
        }
        else if (name == "#Blob")
        {
            blobOffset = fileOffset;
            blobSize = size;
        }
    }

    if (tablesOffset == 0 || stringsOffset == 0 || blobOffset == 0)
    {
        return false;
    }

    // Parse Tables stream header
    // reserved(4), major(1), minor(1), heapSizes(1), reserved2(1), valid(8), sorted(8)
    uint32_t tablesReserved = 0;
    if (!read_u32(image, imageSize, tablesOffset, tablesReserved))
    {
        return false;
    }
    uint8_t major = 0, minor = 0, heapSizes = 0, reserved2 = 0;
    if (tablesOffset + 8 > imageSize)
    {
        return false;
    }
    major = image[tablesOffset + 4];
    minor = image[tablesOffset + 5];
    heapSizes = image[tablesOffset + 6];
    reserved2 = image[tablesOffset + 7];
    (void)major;
    (void)minor;
    (void)reserved2;
    uint64_t valid = 0;
    if (tablesOffset + 16 > imageSize)
    {
        return false;
    }
    memcpy(&valid, image + tablesOffset + 8, sizeof(valid));
    uint64_t sorted = 0;
    if (tablesOffset + 24 > imageSize)
    {
        return false;
    }
    memcpy(&sorted, image + tablesOffset + 16, sizeof(sorted));

    // Count rows for each table (0..63)
    uint32_t rows[64] = {0};
    size_t rowCountsOffset = tablesOffset + 24;
    size_t idx = 0;
    for (int t = 0; t < 64; ++t)
    {
        if (valid & ((uint64_t)1 << t))
        {
            uint32_t r = 0;
            if (!read_u32(image, imageSize, rowCountsOffset + idx * 4, r))
            {
                return false;
            }
            rows[t] = r;
            idx++;
        }
    }

    // Helper lambdas to determine index sizes
    bool stringsAre4 = (heapSizes & 0x01) != 0;
    bool blobAre4 = (heapSizes & 0x04) != 0;

    auto coded_index_size = [&](int tagBits, const std::initializer_list<int> &tablesList) -> int {
        uint32_t maxRows = 0;
        for (int t : tablesList)
        {
            if (t >= 0 && t < 64)
            {
                maxRows = std::max(maxRows, rows[t]);
            }
        }
        uint64_t needed = ((uint64_t)maxRows << tagBits);
        return (needed < 0x10000) ? 2 : 4;
    };

    // HasConstant coded index refers to {Field, Param, Property} -> tagBits = 2; tables: Field=4, Param=5, Property=17
    int size_HasConstant = coded_index_size(2, {4, 5, 17});

    // Compute starting offset of first table row (immediately after rowCounts)
    size_t tableDataOffset = rowCountsOffset + idx * 4;
    size_t curTableOffset = tableDataOffset;

    // Pre-calc table offsets by iterating tables in order and saving their start offsets.
    size_t tableOffset[64];
    for (int t = 0; t < 64; ++t)
        tableOffset[t] = 0;
    for (int t = 0; t < 64; ++t)
    {
        if (!(valid & ((uint64_t)1 << t)))
        {
            continue;
        }
        tableOffset[t] = curTableOffset;
        // get size of a row depending on table
        size_t rowSize = 0;
        switch (t)
        {
            case 2: // TypeRef (not used)
                // minimal skip: ResolutionScope(2/4), TypeName(string), TypeNamespace(string)
                rowSize = (2) + (stringsAre4 ? 4 : 2) + (stringsAre4 ? 4 : 2);
                break;
            case 4: // Field
                // Flags (2), Name (String), Signature (Blob)
                rowSize = 2 + (stringsAre4 ? 4 : 2) + (blobAre4 ? 4 : 2);
                break;
            case 6: // MethodDef
                // skip
                rowSize = 8;
                break;
            case 10: // Param
                rowSize = 4;
                break;
            case 12: // InterfaceImpl
                rowSize = 4;
                break;
            case 14: // MemberRef
                rowSize = 4;
                break;
            case 20: // StandAloneSig
                rowSize = (blobAre4 ? 4 : 2);
                break;
            // (no explicit case 0x02 here)
            default:
                // For tables we don't need, approximate size 4 to skip
                rowSize = 4;
                break;
        }
        // Special handling: TypeDef (table 2)
        if (t == 2)
        {
            // TypeDef: Flags(4), TypeName(String), TypeNamespace(String), Extends (TypeDefOrRef), FieldList (index into
            // Field table) Extends is coded index TypeDefOrRef (tagBits=2) referencing TypeDef/TypeRef/TypeSpec. Use 4
            // bytes conservative.
            int size_TypeName = stringsAre4 ? 4 : 2;
            int size_TypeNamespace = stringsAre4 ? 4 : 2;
            int size_Extends = 4;                             // safe
            int size_FieldList = (rows[4] < 0x10000) ? 2 : 4; // Field table rows count
            rowSize = 4 + size_TypeName + size_TypeNamespace + size_Extends + size_FieldList + 0;
        }
        // Constant table (table 14?) Actually Constant table is 12? ECMA table index for Constant is 11 (0x0B)
        if (t == 11)
        {
            // Constant: Type(1), Padding(1), Parent(HasConstant), Value(Blob)
            rowSize = 1 + 1 + size_HasConstant + (blobAre4 ? 4 : 2);
        }

        curTableOffset += rowSize * (size_t)rows[t];
    }

    // Now find TypeDef row matching szClass + szNamespace
    uint32_t typeDefCount = rows[2];
    if (typeDefCount == 0)
    {
        return false;
    }
    // compute sizes used in TypeDef row
    int sizeString = stringsAre4 ? 4 : 2;
    int sizeFieldListIndex = (rows[4] < 0x10000) ? 2 : 4;
    size_t typeDefRowOffset = tableOffset[2];
    for (uint32_t typeIndex = 1; typeIndex <= typeDefCount; ++typeIndex)
    {
        // read Flags (4)
        uint32_t typeFlags = 0;
        if (!read_u32(image, imageSize, typeDefRowOffset, typeFlags))
        {
            return false;
        }
        size_t off = typeDefRowOffset + 4;
        // read TypeName index
        uint32_t nameIdx = 0;
        if (sizeString == 2)
        {
            uint16_t tmp = 0;
            if (!read_u16(image, imageSize, off, tmp))
            {
                return false;
            }
            nameIdx = tmp;
            off += 2;
        }
        else
        {
            if (!read_u32(image, imageSize, off, nameIdx))
            {
                return false;
            }
            off += 4;
        }
        // read TypeNamespace index
        uint32_t nsIdx = 0;
        if (sizeString == 2)
        {
            uint16_t tmp = 0;
            if (!read_u16(image, imageSize, off, tmp))
            {
                return false;
            }
            nsIdx = tmp;
            off += 2;
        }
        else
        {
            if (!read_u32(image, imageSize, off, nsIdx))
            {
                return false;
            }
            off += 4;
        }
        // skip Extends (4)
        off += 4;
        // read FieldList
        uint32_t fieldList = 0;
        if (sizeFieldListIndex == 2)
        {
            uint16_t tmp = 0;
            if (!read_u16(image, imageSize, off, tmp))
            {
                return false;
            }
            fieldList = tmp;
            off += 2;
        }
        else
        {
            if (!read_u32(image, imageSize, off, fieldList))
            {
                return false;
            }
            off += 4;
        }

        // Read type name and namespace strings
        char typeName[256];
        typeName[0] = '\0';
        char typeNs[256];
        typeNs[0] = '\0';
        if (nameIdx != 0)
        {
            // string heap: stringsOffset + nameIdx
            if (!read_strings_heap_string(image + stringsOffset, stringsSize, nameIdx, typeName, sizeof(typeName)))
            {
            }
        }
        if (nsIdx != 0)
        {
            if (!read_strings_heap_string(image + stringsOffset, stringsSize, nsIdx, typeNs, sizeof(typeNs)))
            {
            }
        }

        if (strcmp(typeName, szClass) == 0 && strcmp(typeNs, szNamespace == NULL ? "" : szNamespace) == 0)
        {
            // Found type. If caller only wanted to check type existence, return true.
            if (onlyCheckType)
            {
                return true;
            }

            // Determine range of fields: from FieldList to next TypeDef's FieldList-1 or end of Field table
            uint32_t fieldCountTotal = rows[4];
            uint32_t endField = fieldCountTotal;
            size_t typeDefRowSize = 4 + sizeString + sizeString + 4 + sizeFieldListIndex;
            size_t nextOffset = typeDefRowOffset + typeDefRowSize * (size_t)typeIndex;
            if (typeIndex < typeDefCount)
            {
                uint32_t nf = 0;
                size_t nfOff = nextOffset + 4 + sizeString + sizeString + 4;
                if (sizeString == 2)
                {
                    uint16_t tmp;
                    if (!read_u16(image, imageSize, nfOff, tmp))
                    {
                        return false;
                    }
                    nf = tmp;
                }
                else
                {
                    if (!read_u32(image, imageSize, nfOff, nf))
                    {
                        return false;
                    }
                }
                if (nf != 0)
                {
                    endField = nf - 1;
                }
            }

            if (fieldList == 0)
            {
                return false;
            }
            uint32_t startField = fieldList;
            uint32_t finalField = endField;

            // Field table start offset
            size_t fieldTableOffset = tableOffset[4];
            int fieldRowSize = 2 + sizeString + (blobAre4 ? 4 : 2);

            for (uint32_t fld = startField; fld <= finalField; ++fld)
            {
                size_t fldOff = fieldTableOffset + (size_t)(fld - 1) * fieldRowSize;
                uint16_t fieldFlags = 0;
                if (!read_u16(image, imageSize, fldOff, fieldFlags))
                {
                    return false;
                }
                size_t foff = fldOff + 2;
                uint32_t fieldNameIdx = 0;
                if (sizeString == 2)
                {
                    uint16_t tmp;
                    if (!read_u16(image, imageSize, foff, tmp))
                    {
                        return false;
                    }
                    fieldNameIdx = tmp;
                    foff += 2;
                }
                else
                {
                    if (!read_u32(image, imageSize, foff, fieldNameIdx))
                    {
                        return false;
                    }
                    foff += 4;
                }

                char fieldName[256];
                fieldName[0] = '\0';
                if (fieldNameIdx != 0)
                {
                    read_strings_heap_string(
                        image + stringsOffset,
                        stringsSize,
                        fieldNameIdx,
                        fieldName,
                        sizeof(fieldName));
                }

                // check public: access mask low 3 bits equal 0x6 (Public)
                if ((fieldFlags & 0x0007) != 0x0006)
                {
                    continue;
                }
                // check literal
                if ((fieldFlags & 0x0040) == 0)
                {
                    continue;
                }

                if (strcmp(fieldName, szField) != 0)
                {
                    continue;
                }

                // Find Constant table entry for this field
                uint32_t constCount = rows[11];
                if (constCount == 0)
                {
                    return false;
                }
                size_t constTableOff = tableOffset[11];
                size_t constRowSize = 1 + 1 + size_HasConstant + (blobAre4 ? 4 : 2);
                for (uint32_t ci = 1; ci <= constCount; ++ci)
                {
                    size_t cOff = constTableOff + (size_t)(ci - 1) * constRowSize;
                    if (cOff + 1 > imageSize)
                    {
                        return false;
                    }
                    uint8_t cType = image[cOff];
                    // read Parent coded index
                    size_t parentOff = cOff + 2;
                    uint32_t parentIdx = 0;
                    if (size_HasConstant == 2)
                    {
                        uint16_t tmp = 0;
                        if (!read_u16(image, imageSize, parentOff, tmp))
                        {
                            return false;
                        }
                        parentIdx = tmp;
                        parentOff += 2;
                    }
                    else
                    {
                        if (!read_u32(image, imageSize, parentOff, parentIdx))
                        {
                            return false;
                        }
                        parentOff += 4;
                    }
                    // decode HasConstant: tagBits=2, tag 0=Field
                    uint32_t tag = parentIdx & 0x3;
                    uint32_t index = parentIdx >> 2;
                    if (tag != 0)
                    {
                        continue; // not a field
                    }
                    if (index != fld)
                    {
                        continue;
                    }

                    // read Value blob index
                    uint32_t valueBlobIdx = 0;
                    if (blobAre4)
                    {
                        if (!read_u32(image, imageSize, parentOff, valueBlobIdx))
                        {
                            return false;
                        }
                    }
                    else
                    {
                        uint16_t tmp;
                        if (!read_u16(image, imageSize, parentOff, tmp))
                        {
                            return false;
                        }
                        valueBlobIdx = tmp;
                    }

                    // Read blob content: blob heap at blobOffset + valueBlobIdx is length-prefixed
                    uint32_t blobLen = 0;
                    size_t blobLenBytes = 0;
                    if (!read_compressed_uint(image + blobOffset, blobSize, valueBlobIdx, blobLen, blobLenBytes))
                    {
                        return false;
                    }
                    size_t blobDataStart = blobOffset + valueBlobIdx + blobLenBytes;
                    if (blobDataStart + blobLen > imageSize)
                    {
                        return false;
                    }

                    // Interpret based on cType (CorElementType)
                    switch (cType)
                    {
                        case 0x02: // BOOLEAN
                        case 0x04: // I1
                        case 0x05: // U1
                        case 0x06: // I2
                        case 0x07: // U2
                        case 0x08: // I4
                        case 0x09: // U4
                        case 0x0a: // I8
                        case 0x0b: // U8
                        {
                            // blobs store integer in little endian occupying blobLen bytes
                            if ((int)blobLen > outSize)
                            {
                                return false;
                            }
                            // copy into outValue and zero-extend if needed
                            memset(outValue, 0, outSize);
                            memcpy(outValue, image + blobDataStart, blobLen);
                            return true;
                        }
                        case 0x0e: // STRING
                        {
                            // serstring at blobDataStart
                            if (!read_serstring(image + blobOffset, blobSize, valueBlobIdx, (char *)outValue, outSize))
                            {
                                return false;
                            }
                            return true;
                        }
                        default:
                            return false;
                    }
                }

                return false;
            }

            return false;
        }

        // advance to next TypeDef row
        typeDefRowOffset += 4 + sizeString + sizeString + 4 + sizeFieldListIndex;
    }

    return false;
}

const void *CE_FindPEByType(const char *szClass, const char *szNamespace)
{
    if (!szClass || !szNamespace)
    {
        return nullptr;
    }

    // check cache first
    const void *cached = lookup_pe_cache(szClass, szNamespace);
    if (cached)
    {
        return cached;
    }

    if (!g_flashBase || g_flashSize == 0)
    {
        return nullptr;
    }

    // Scan the configured flash region for PE images (MZ header). For each
    // candidate PE found, compute a conservative PE size from section headers
    // and test whether it contains the requested type. Return the base of the
    // first matching PE or nullptr if none found.

    const uint8_t *base = g_flashBase;
    size_t regionSize = g_flashSize;
    // scan with 4-byte step to be efficient
    for (size_t off = 0; off + 0x40 < regionSize; off += 4)
    {
        if (base[off] != 'M' || base[off + 1] != 'Z')
        {
            continue;
        }

        const uint8_t *img = base + off;
        size_t imgSize = regionSize - off;

        // Read e_lfanew
        uint32_t e_lfanew = 0;
        if (!read_u32(img, imgSize, 0x3C, e_lfanew))
        {
            continue;
        }
        if (e_lfanew + 4 > imgSize)
        {
            continue;
        }
        if (img[e_lfanew] != 'P' || img[e_lfanew + 1] != 'E' || img[e_lfanew + 2] != 0 || img[e_lfanew + 3] != 0)
        {
            continue;
        }

        size_t peHeader = e_lfanew + 4;
        // read NumberOfSections and SizeOfOptionalHeader
        uint16_t numberOfSections = 0;
        uint16_t sizeOfOptionalHeader = 0;
        if (!read_u16(img, imgSize, peHeader + 2, numberOfSections))
        {
            continue;
        }
        if (!read_u16(img, imgSize, peHeader + 16, sizeOfOptionalHeader))
        {
            continue;
        }

        size_t optionalHeader = peHeader + 20;
        if (optionalHeader + sizeOfOptionalHeader > imgSize)
        {
            continue;
        }

        size_t sectionTable = optionalHeader + sizeOfOptionalHeader;
        size_t sectionEntrySize = 40;
        if (sectionTable + numberOfSections * sectionEntrySize > imgSize)
        {
            // possibly truncated, but continue with available data
        }

        // compute conservative PE size from sections
        size_t maxEnd = 0;
        for (uint16_t i = 0; i < numberOfSections; ++i)
        {
            size_t ent = sectionTable + i * sectionEntrySize;
            if (ent + 24 > imgSize)
            {
                break;
            }
            uint32_t sizeOfRawData = 0;
            uint32_t pointerToRawData = 0;
            if (!read_u32(img, imgSize, ent + 16, sizeOfRawData))
            {
                break;
            }
            if (!read_u32(img, imgSize, ent + 20, pointerToRawData))
            {
                break;
            }
            size_t end = (size_t)pointerToRawData + (size_t)sizeOfRawData;
            if (end > maxEnd)
            {
                maxEnd = end;
            }
        }

        size_t peSize = 0;
        if (maxEnd > 0 && maxEnd <= imgSize)
        {
            peSize = maxEnd;
        }
        else
        {
            peSize = imgSize; // fallback: use remaining region size
        }

        // Test whether this PE contains the requested type
        if (ExtractPublicConstFromImage(img, peSize, szClass, szNamespace, nullptr, nullptr, 0))
        {
            add_pe_cache(szClass, szNamespace, (const void *)img);
            return (const void *)img;
        }
    }

    return nullptr;
}

bool CE_ExtractPublicConstUnsignedAt(
    const void *peBase,
    const char *szClass,
    const char *szNamespace,
    const char *szField,
    uint64_t *outVal)
{
    if (!peBase || !szClass || !szField || !outVal)
    {
        return false;
    }
    // compute available size from flash base if possible
    const uint8_t *p = (const uint8_t *)peBase;
    size_t avail = 0;
    if (g_flashBase && p >= g_flashBase)
    {
        avail = g_flashSize - (size_t)(p - g_flashBase);
    }
    else
    {
        avail = 0;
    }
    uint8_t buf[8];
    memset(buf, 0, sizeof(buf));
    if (!ExtractPublicConstFromImage(
            (const uint8_t *)peBase,
            avail,
            szClass,
            szNamespace,
            szField,
            buf,
            (int)sizeof(buf)))
    {
        return false;
    }
    uint64_t v = 0;
    for (int i = 0; i < 8; ++i)
    {
        v |= ((uint64_t)buf[i]) << (8 * i);
    }
    *outVal = v;
    return true;
}

bool CE_ExtractPublicConstStringAt(
    const void *peBase,
    const char *szClass,
    const char *szNamespace,
    const char *szField,
    char *outBuf,
    int outBufSize)
{
    if (!peBase || !szClass || !szField || !outBuf || outBufSize <= 0)
    {
        return false;
    }
    const uint8_t *p = (const uint8_t *)peBase;
    size_t avail = 0;
    if (g_flashBase && p >= g_flashBase)
    {
        avail = g_flashSize - (size_t)(p - g_flashBase);
    }
    else
    {
        avail = 0;
    }
    if (!ExtractPublicConstFromImage((const uint8_t *)peBase, avail, szClass, szNamespace, szField, outBuf, outBufSize))
    {
        return false;
    }
    return true;
}
