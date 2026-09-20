//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include <nanoHAL_Types.h>
#include <nanoPAL_BlockStorage.h>
#include <esp32_idf.h>
#include <esp_partition.h>
#include <Target_BlockStorage_Esp32FlashDriver.h>

BlockStorageDevice Device_BlockStorage;
const BlockRange BlockRange1[] = {{.RangeType = BlockRange_BLOCKTYPE_CODE, .StartBlock = 0, .EndBlock = 0}};
const BlockRange BlockRange2[] = {{.RangeType = BlockRange_BLOCKTYPE_DEPLOYMENT, .StartBlock = 0, .EndBlock = 0}};
const BlockRange BlockRange3[] = {{.RangeType = BlockRange_BLOCKTYPE_CONFIG, .StartBlock = 0, .EndBlock = 0}};
BlockRegionInfo BlockRegions[] = {
    {.Attributes = 0,
     .Start = 0,
     .NumBlocks = 1,
     .BytesPerBlock = 0,
     .NumBlockRanges = ARRAYSIZE_CONST_EXPR(BlockRange1),
     .BlockRanges = BlockRange1},
    {.Attributes = BlockRegionAttribute_MemoryMapped,
     .Start = 0,
     .NumBlocks = 1,
     .BytesPerBlock = 0,
     .NumBlockRanges = ARRAYSIZE_CONST_EXPR(BlockRange2),
     .BlockRanges = BlockRange2},
    {.Attributes = 0,
     .Start = 0,
     .NumBlocks = 1,
     .BytesPerBlock = 0,
     .NumBlockRanges = ARRAYSIZE_CONST_EXPR(BlockRange3),
     .BlockRanges = BlockRange3}};

const DeviceBlockInfo Device_BlockInfo =
    {.Attribute = 0, .BytesPerSector = 0, .NumRegions = ARRAYSIZE_CONST_EXPR(BlockRegions), .Regions = BlockRegions};

MEMORY_MAPPED_NOR_BLOCK_CONFIG
Device_BlockStorageConfig = {
    .BlockConfig = {.BlockDeviceInformation = (DeviceBlockInfo *)&Device_BlockInfo, .WriteProtectionPin = {.Pin = 0, .ActiveState = false}},
    .Memory =
        {.ChipSelect = 0,
         .ReadOnly = true,
         .WaitStates = 0,
         .ReleaseCounts = 0,
         .BitWidth = 16,
         .BaseAddress = 0x08000000,
         .SizeInBytes = 0x00200000,
         .XREADYEnable = 0,
         .ByteSignalsForRead = 0,
         .ExternalBufferEnable = 0},
    .ChipProtection = 0,
    .DeviceCode = 0,
    .ManufacturerCode = 0};

//
// Align nanoCLR block regions with regions defined by the esp32 series partition file
//
void FixUpBlockRegionInfo()
{
    const esp_partition_t *part_nanoClr;
    const esp_partition_t *part_deploy;
    const esp_partition_t *part_config;

    part_nanoClr = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_FACTORY, 0);
    part_deploy = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_NANOCLR, 0);
    part_config = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_LITTLEFS, 0);

    BlockRegions[0].Start = part_nanoClr->address;
    BlockRegions[0].BytesPerBlock = part_nanoClr->size;
    BlockRegions[1].Start = part_deploy->address;
    BlockRegions[1].BytesPerBlock = part_deploy->size;
    BlockRegions[2].Start = part_config->address;
    BlockRegions[2].BytesPerBlock = part_config->size;
}
