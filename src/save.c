#include "global.h"
#include "malloc.h"
#include "agb_flash.h"
#include "gba/flash_internal.h"
#include "fieldmap.h"
#include "save.h"
#include "task.h"
#include "decompress.h"
#include "load_save.h"
#include "overworld.h"
#include "pokemon_storage_system.h"
#include "main.h"
#include "trainer_hill.h"
#include "link.h"
#include "event_data.h"
#include "item.h"
#include "lilycove_lady.h"
#include "registered_items_menu.h"
#include "roamer.h"
#include "save_versions.h"
#include "constants/game_stat.h"
#include "constants/heal_locations.h"
#include "constants/items.h"

static u16 CalculateChecksum(void *, u16);
static bool8 ReadFlashSector(u8, struct SaveSector *);
static u8 GetSaveValidStatus(const struct SaveSectorLocation *);
static u8 CopySaveSlotData(u16, struct SaveSectorLocation *);
static u8 TryWriteSector(u8, u8 *);
static u8 HandleWriteSector(u16, const struct SaveSectorLocation *);
static u8 HandleReplaceSector(u16, const struct SaveSectorLocation *);
static void UpdateVanillaSave(void);
static void UpdateOldHackSave(void);
static void UpdateGiftRibbons(void);
static void CheckProgressFlags(void);

// Divide save blocks into individual chunks to be written to flash sectors

/*
 * Sector Layout:
 *
 * Sectors 0 - 13:      Save Slot 1
 * Sectors 14 - 27:     Save Slot 2
 * Sectors 28 - 29:     Hall of Fame
 * Sector 30:           Trainer Hill
 * Sector 31:           Recorded Battle
 *
 * There are two save slots for saving the player's game data. We alternate between
 * them each time the game is saved, so that if the current save slot is corrupt,
 * we can load the previous one. We also rotate the sectors in each save slot
 * so that the same data is not always being written to the same sector. This
 * might be done to reduce wear on the flash memory, but I'm not sure, since all
 * 14 sectors get written anyway.
 *
 * See SECTOR_ID_* constants in save.h
 */

#define TM_FLAGS 39
#define SAVEBLOCK_CHUNK(structure, chunkNum)                                   \
{                                                                              \
    chunkNum * SECTOR_DATA_SIZE,                                               \
    sizeof(structure) >= chunkNum * SECTOR_DATA_SIZE ?                         \
    min(sizeof(structure) - chunkNum * SECTOR_DATA_SIZE, SECTOR_DATA_SIZE) : 0 \
}

struct
{
    u16 offset;
    u16 size;
} static const sSaveSlotLayout[NUM_SECTORS_PER_SLOT] =
{
    SAVEBLOCK_CHUNK(struct SaveBlock2, 0), // SECTOR_ID_SAVEBLOCK2

    SAVEBLOCK_CHUNK(struct SaveBlock1, 0), // SECTOR_ID_SAVEBLOCK1_START
    SAVEBLOCK_CHUNK(struct SaveBlock1, 1),
    SAVEBLOCK_CHUNK(struct SaveBlock1, 2),
    SAVEBLOCK_CHUNK(struct SaveBlock1, 3), // SECTOR_ID_SAVEBLOCK1_END

    SAVEBLOCK_CHUNK(struct PokemonStorage, 0), // SECTOR_ID_PKMN_STORAGE_START
    SAVEBLOCK_CHUNK(struct PokemonStorage, 1),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 2),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 3),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 4),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 5),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 6),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 7),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 8), // SECTOR_ID_PKMN_STORAGE_END
};

struct
{
    u16 offset;
    u16 size;
} static const sOldSaveSlotLayout[NUM_SECTORS_PER_SLOT] =
{
    SAVEBLOCK_CHUNK(struct SaveBlock2Old, 0), // SECTOR_ID_SAVEBLOCK2

    SAVEBLOCK_CHUNK(struct SaveBlock1Old, 0), // SECTOR_ID_SAVEBLOCK1_START
    SAVEBLOCK_CHUNK(struct SaveBlock1Old, 1),
    SAVEBLOCK_CHUNK(struct SaveBlock1Old, 2),
    SAVEBLOCK_CHUNK(struct SaveBlock1Old, 3), // SECTOR_ID_SAVEBLOCK1_END

    SAVEBLOCK_CHUNK(struct PokemonStorage, 0), // SECTOR_ID_PKMN_STORAGE_START
    SAVEBLOCK_CHUNK(struct PokemonStorage, 1),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 2),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 3),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 4),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 5),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 6),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 7),
    SAVEBLOCK_CHUNK(struct PokemonStorage, 8), // SECTOR_ID_PKMN_STORAGE_END
};

static const u16 sTMFlagChecks[][2] =
{
    {FLAG_ITEM_ROUTE_115_TM_FOCUS_PUNCH,                         ITEM_TM01},
    {FLAG_ITEM_METEOR_FALLS_B1F_2R_TM_DRAGON_CLAW,               ITEM_TM02},
    {FLAG_RECEIVED_TM_WATER_PULSE,                               ITEM_TM03},
    {FLAG_RECEIVED_TM_CALM_MIND,                                 ITEM_TM04},
    {FLAG_RECEIVED_TM_ROAR,                                      ITEM_TM05},
    {FLAG_ITEM_FIERY_PATH_TM_TOXIC,                              ITEM_TM06},
    {FLAG_ITEM_SHOAL_CAVE_ICE_ROOM_TM_HAIL,                      ITEM_TM07},
    {FLAG_RECEIVED_TM_BULK_UP,                                   ITEM_TM08},
    {FLAG_RECEIVED_TM_BULLET_SEED,                               ITEM_TM09},
    {FLAG_RECEIVED_TM_HIDDEN_POWER,                              ITEM_TM10},
    {FLAG_ITEM_SCORCHED_SLAB_TM_SUNNY_DAY,                       ITEM_TM11},
    {FLAG_ITEM_ABANDONED_SHIP_ROOMS_B1F_TM_ICE_BEAM,             ITEM_TM13},
    {FLAG_ITEM_ABANDONED_SHIP_HIDDEN_FLOOR_ROOM_1_TM_RAIN_DANCE, ITEM_TM18},
    {FLAG_RECEIVED_TM_GIGA_DRAIN,                                ITEM_TM19},
    {FLAG_ITEM_SAFARI_ZONE_NORTH_WEST_TM_SOLAR_BEAM,             ITEM_TM22},
    {FLAG_ITEM_METEOR_FALLS_1F_1R_TM_IRON_TAIL,                  ITEM_TM23},
    {FLAG_GOT_TM_THUNDERBOLT_FROM_WATTSON,                       ITEM_TM24},
    {FLAG_ITEM_SEAFLOOR_CAVERN_ROOM_9_TM_EARTHQUAKE,             ITEM_TM26},
    {FLAG_RECEIVED_TM_DIG,                                       ITEM_TM28},
    {FLAG_ITEM_VICTORY_ROAD_B1F_TM_PSYCHIC,                      ITEM_TM29},
    {FLAG_ITEM_MT_PYRE_6F_TM_SHADOW_BALL,                        ITEM_TM30},
    {FLAG_RECEIVED_TM_BRICK_BREAK,                               ITEM_TM31},
    {FLAG_HIDDEN_ITEM_ROUTE_113_TM_DOUBLE_TEAM,                  ITEM_TM32},
    {FLAG_RECEIVED_TM_SHOCK_WAVE,                                ITEM_TM34},
    {FLAG_RECEIVED_TM_RETURN,                                    ITEM_TM35}, // Cozmo gives the player TM35 (Flamethrower) instead of TM27 (Return) in the hack.
    {FLAG_RECEIVED_TM_SLUDGE_BOMB,                               ITEM_TM36},
    {FLAG_ITEM_ROUTE_111_TM_SANDSTORM,                           ITEM_TM37},
    {FLAG_RECEIVED_TM_ROCK_TOMB,                                 ITEM_TM39},
    {FLAG_RECEIVED_TM_AERIAL_ACE,                                ITEM_TM40},
    {FLAG_RECEIVED_TM_TORMENT,                                   ITEM_TM41},
    {FLAG_RECEIVED_TM_FACADE,                                    ITEM_TM42},
    {FLAG_RECEIVED_SECRET_POWER,                                 ITEM_TM43},
    {FLAG_RECEIVED_TM_REST,                                      ITEM_TM44},
    {FLAG_RECEIVED_TM_ATTRACT,                                   ITEM_TM45},
    {FLAG_RECEIVED_TM_THIEF,                                     ITEM_TM46},
    {FLAG_DELIVERED_STEVEN_LETTER,                               ITEM_TM47},
    {FLAG_ITEM_MT_PYRE_EXTERIOR_TM_SKILL_SWAP,                   ITEM_TM48},
    {FLAG_RECEIVED_TM_SNATCH,                                    ITEM_TM49},
    {FLAG_RECEIVED_TM_OVERHEAT,                                  ITEM_TM50},
};

// These will produce an error if a save struct is larger than the space
// alloted for it in the flash.
STATIC_ASSERT(sizeof(struct SaveBlock2) <= SECTOR_DATA_SIZE, SaveBlock2FreeSpace);
STATIC_ASSERT(sizeof(struct SaveBlock1) <= SECTOR_DATA_SIZE * (SECTOR_ID_SAVEBLOCK1_END - SECTOR_ID_SAVEBLOCK1_START + 1), SaveBlock1FreeSpace);
STATIC_ASSERT(sizeof(struct PokemonStorage) <= SECTOR_DATA_SIZE * (SECTOR_ID_PKMN_STORAGE_END - SECTOR_ID_PKMN_STORAGE_START + 1), PokemonStorageFreeSpace);

u16 gLastWrittenSector;
u32 gLastSaveCounter;
u16 gLastKnownGoodSector;
u32 gDamagedSaveSectors;
u32 gSaveCounter;
struct SaveSector *gReadWriteSector; // Pointer to a buffer for reading/writing a sector
u16 gIncrementalSectorId;
u16 gSaveUnusedVar;
u16 gSaveFileStatus;
void (*gGameContinueCallback)(void);
struct SaveSectorLocation gRamSaveSectorLocations[NUM_SECTORS_PER_SLOT];
u16 gSaveUnusedVar2;
u16 gSaveAttemptStatus;

EWRAM_DATA struct SaveSector gSaveDataBuffer = {0}; // Buffer used for reading/writing sectors
EWRAM_DATA static u8 sUnusedVar = 0;

void ClearSaveData(void)
{
    u16 i;

    // Clear the full save two sectors at a time
    for (i = 0; i < SECTORS_COUNT / 2; i++)
    {
        EraseFlashSector(i);
        EraseFlashSector(i + SECTORS_COUNT / 2);
    }
}

void Save_ResetSaveCounters(void)
{
    gSaveCounter = 0;
    gLastWrittenSector = 0;
    gDamagedSaveSectors = 0;
}

static bool32 SetDamagedSectorBits(u8 op, u8 sectorId)
{
    bool32 retVal = FALSE;

    switch (op)
    {
    case ENABLE:
        gDamagedSaveSectors |= (1 << sectorId);
        break;
    case DISABLE:
        gDamagedSaveSectors &= ~(1 << sectorId);
        break;
    case CHECK: // unused
        if (gDamagedSaveSectors & (1 << sectorId))
            retVal = TRUE;
        break;
    }

    return retVal;
}

static u8 WriteSaveSectorOrSlot(u16 sectorId, const struct SaveSectorLocation *locations)
{
    u32 status;
    u16 i;

    gReadWriteSector = &gSaveDataBuffer;

    if (sectorId != FULL_SAVE_SLOT)
    {
        // A sector was specified, just write that sector.
        // This is never reached, FULL_SAVE_SLOT is always used instead.
        status = HandleWriteSector(sectorId, locations);
    }
    else
    {
        // No sector was specified, write full save slot.
        gLastKnownGoodSector = gLastWrittenSector; // backup the current written sector before attempting to write.
        gLastSaveCounter = gSaveCounter;
        gLastWrittenSector++;
        gLastWrittenSector = gLastWrittenSector % NUM_SECTORS_PER_SLOT;
        gSaveCounter++;
        status = SAVE_STATUS_OK;

        for (i = 0; i < NUM_SECTORS_PER_SLOT; i++)
            HandleWriteSector(i, locations);

        if (gDamagedSaveSectors)
        {
            // At least one sector save failed
            status = SAVE_STATUS_ERROR;
            gLastWrittenSector = gLastKnownGoodSector;
            gSaveCounter = gLastSaveCounter;
        }
    }

    return status;
}

static u8 HandleWriteSector(u16 sectorId, const struct SaveSectorLocation *locations)
{
    u16 i;
    u16 sector;
    u8 *data;
    u16 size;

    // Adjust sector id for current save slot
    sector = sectorId + gLastWrittenSector;
    sector %= NUM_SECTORS_PER_SLOT;
    sector += NUM_SECTORS_PER_SLOT * (gSaveCounter % NUM_SAVE_SLOTS);

    // Get current save data
    data = locations[sectorId].data;
    size = locations[sectorId].size;

    // Clear temp save sector
    for (i = 0; i < SECTOR_SIZE; i++)
        ((u8 *)gReadWriteSector)[i] = 0;

    // Set footer data
    gReadWriteSector->id = sectorId;
    gReadWriteSector->signature = SECTOR_SIGNATURE;
    gReadWriteSector->counter = gSaveCounter;

    // Copy current data to temp buffer for writing
    for (i = 0; i < size; i++)
        gReadWriteSector->data[i] = data[i];

    gReadWriteSector->checksum = CalculateChecksum(data, size);

    return TryWriteSector(sector, gReadWriteSector->data);
}

static u8 HandleWriteSectorNBytes(u8 sectorId, u8 *data, u16 size)
{
    u16 i;
    struct SaveSector *sector = &gSaveDataBuffer;

    // Clear temp save sector
    for (i = 0; i < SECTOR_SIZE; i++)
        ((u8 *)sector)[i] = 0;

    sector->signature = SECTOR_SIGNATURE;

    // Copy data to temp buffer for writing
    for (i = 0; i < size; i++)
        sector->data[i] = data[i];

    sector->id = CalculateChecksum(data, size); // though this appears to be incorrect, it might be some sector checksum instead of a whole save checksum and only appears to be relevent to HOF data, if used.
    return TryWriteSector(sectorId, sector->data);
}

static u8 TryWriteSector(u8 sector, u8 *data)
{
    if (ProgramFlashSectorAndVerify(sector, data)) // is damaged?
    {
        // Failed
        SetDamagedSectorBits(ENABLE, sector);
        return SAVE_STATUS_ERROR;
    }
    else
    {
        // Succeeded
        SetDamagedSectorBits(DISABLE, sector);
        return SAVE_STATUS_OK;
    }
}

static u32 RestoreSaveBackupVarsAndIncrement(const struct SaveSectorLocation *locations)
{
    gReadWriteSector = &gSaveDataBuffer;
    gLastKnownGoodSector = gLastWrittenSector;
    gLastSaveCounter = gSaveCounter;
    gLastWrittenSector++;
    gLastWrittenSector %= NUM_SECTORS_PER_SLOT;
    gSaveCounter++;
    gIncrementalSectorId = 0;
    gDamagedSaveSectors = 0;
    return 0;
}

static u32 RestoreSaveBackupVars(const struct SaveSectorLocation *locations)
{
    gReadWriteSector = &gSaveDataBuffer;
    gLastKnownGoodSector = gLastWrittenSector;
    gLastSaveCounter = gSaveCounter;
    gIncrementalSectorId = 0;
    gDamagedSaveSectors = 0;
    return 0;
}

static u8 HandleWriteIncrementalSector(u16 numSectors, const struct SaveSectorLocation *locations)
{
    u8 status;

    if (gIncrementalSectorId < numSectors - 1)
    {
        status = SAVE_STATUS_OK;
        HandleWriteSector(gIncrementalSectorId, locations);
        gIncrementalSectorId++;
        if (gDamagedSaveSectors)
        {
            status = SAVE_STATUS_ERROR;
            gLastWrittenSector = gLastKnownGoodSector;
            gSaveCounter = gLastSaveCounter;
        }
    }
    else
    {
        // Exceeded max sector, finished
        status = SAVE_STATUS_ERROR;
    }

    return status;
}

static u8 HandleReplaceSectorAndVerify(u16 sectorId, const struct SaveSectorLocation *locations)
{
    u8 status = SAVE_STATUS_OK;

    HandleReplaceSector(sectorId - 1, locations);

    if (gDamagedSaveSectors)
    {
        status = SAVE_STATUS_ERROR;
        gLastWrittenSector = gLastKnownGoodSector;
        gSaveCounter = gLastSaveCounter;
    }
    return status;
}

// Similar to HandleWriteSector, but fully erases the sector first, and skips writing the first signature byte
static u8 HandleReplaceSector(u16 sectorId, const struct SaveSectorLocation *locations)
{
    u16 i;
    u16 sector;
    u8 *data;
    u16 size;
    u8 status;

    // Adjust sector id for current save slot
    sector = sectorId + gLastWrittenSector;
    sector %= NUM_SECTORS_PER_SLOT;
    sector += NUM_SECTORS_PER_SLOT * (gSaveCounter % NUM_SAVE_SLOTS);

    // Get current save data
    data = locations[sectorId].data;
    size = locations[sectorId].size;

    // Clear temp save sector.
    for (i = 0; i < SECTOR_SIZE; i++)
        ((u8 *)gReadWriteSector)[i] = 0;

    // Set footer data
    gReadWriteSector->id = sectorId;
    gReadWriteSector->signature = SECTOR_SIGNATURE;
    gReadWriteSector->counter = gSaveCounter;

    // Copy current data to temp buffer for writing
    for (i = 0; i < size; i++)
        gReadWriteSector->data[i] = data[i];

    gReadWriteSector->checksum = CalculateChecksum(data, size);

    // Erase old save data
    EraseFlashSector(sector);

    status = SAVE_STATUS_OK;

    // Write new save data up to signature field
    for (i = 0; i < SECTOR_SIGNATURE_OFFSET; i++)
    {
        if (ProgramFlashByte(sector, i, ((u8 *)gReadWriteSector)[i]))
        {
            status = SAVE_STATUS_ERROR;
            break;
        }
    }

    if (status == SAVE_STATUS_ERROR)
    {
        // Writing save data failed
        SetDamagedSectorBits(ENABLE, sector);
        return SAVE_STATUS_ERROR;
    }
    else
    {
        // Writing save data succeeded, write signature and counter
        status = SAVE_STATUS_OK;

        // Write signature (skipping the first byte) and counter fields.
        // The byte of signature that is skipped is instead written by WriteSectorSignatureByte or WriteSectorSignatureByte_NoOffset
        for (i = 0; i < SECTOR_SIZE - (SECTOR_SIGNATURE_OFFSET + 1); i++)
        {
            if (ProgramFlashByte(sector, SECTOR_SIGNATURE_OFFSET + 1 + i, ((u8 *)gReadWriteSector)[SECTOR_SIGNATURE_OFFSET + 1 + i]))
            {
                status = SAVE_STATUS_ERROR;
                break;
            }
        }

        if (status == SAVE_STATUS_ERROR)
        {
            // Writing signature/counter failed
            SetDamagedSectorBits(ENABLE, sector);
            return SAVE_STATUS_ERROR;
        }
        else
        {
            // Succeeded
            SetDamagedSectorBits(DISABLE, sector);
            return SAVE_STATUS_OK;
        }
    }
}

static u8 WriteSectorSignatureByte_NoOffset(u16 sectorId, const struct SaveSectorLocation *locations)
{
    // Adjust sector id for current save slot
    // This first line lacking -1 is the only difference from WriteSectorSignatureByte
    u16 sector = sectorId + gLastWrittenSector;
    sector %= NUM_SECTORS_PER_SLOT;
    sector += NUM_SECTORS_PER_SLOT * (gSaveCounter % NUM_SAVE_SLOTS);

    // Write just the first byte of the signature field, which was skipped by HandleReplaceSector
    if (ProgramFlashByte(sector, SECTOR_SIGNATURE_OFFSET, SECTOR_SIGNATURE & 0xFF))
    {
        // Sector is damaged, so enable the bit in gDamagedSaveSectors and restore the last written sector and save counter.
        SetDamagedSectorBits(ENABLE, sector);
        gLastWrittenSector = gLastKnownGoodSector;
        gSaveCounter = gLastSaveCounter;
        return SAVE_STATUS_ERROR;
    }
    else
    {
        // Succeeded
        SetDamagedSectorBits(DISABLE, sector);
        return SAVE_STATUS_OK;
    }
}

static u8 CopySectorSignatureByte(u16 sectorId, const struct SaveSectorLocation *locations)
{
    // Adjust sector id for current save slot
    u16 sector = sectorId + gLastWrittenSector - 1;
    sector %= NUM_SECTORS_PER_SLOT;
    sector += NUM_SECTORS_PER_SLOT * (gSaveCounter % NUM_SAVE_SLOTS);

    // Copy just the first byte of the signature field from the read/write buffer
    if (ProgramFlashByte(sector, SECTOR_SIGNATURE_OFFSET, ((u8 *)gReadWriteSector)[SECTOR_SIGNATURE_OFFSET]))
    {
        // Sector is damaged, so enable the bit in gDamagedSaveSectors and restore the last written sector and save counter.
        SetDamagedSectorBits(ENABLE, sector);
        gLastWrittenSector = gLastKnownGoodSector;
        gSaveCounter = gLastSaveCounter;
        return SAVE_STATUS_ERROR;
    }
    else
    {
        // Succeeded
        SetDamagedSectorBits(DISABLE, sector);
        return SAVE_STATUS_OK;
    }
}

static u8 WriteSectorSignatureByte(u16 sectorId, const struct SaveSectorLocation *locations)
{
    // Adjust sector id for current save slot
    u16 sector = sectorId + gLastWrittenSector - 1;
    sector %= NUM_SECTORS_PER_SLOT;
    sector += NUM_SECTORS_PER_SLOT * (gSaveCounter % NUM_SAVE_SLOTS);

    // Write just the first byte of the signature field, which was skipped by HandleReplaceSector
    if (ProgramFlashByte(sector, SECTOR_SIGNATURE_OFFSET, SECTOR_SIGNATURE & 0xFF))
    {
        // Sector is damaged, so enable the bit in gDamagedSaveSectors and restore the last written sector and save counter.
        SetDamagedSectorBits(ENABLE, sector);
        gLastWrittenSector = gLastKnownGoodSector;
        gSaveCounter = gLastSaveCounter;
        return SAVE_STATUS_ERROR;
    }
    else
    {
        // Succeeded
        SetDamagedSectorBits(DISABLE, sector);
        return SAVE_STATUS_OK;
    }
}

static u8 TryLoadSaveSlot(u16 sectorId, struct SaveSectorLocation *locations)
{
    u8 status;
    gReadWriteSector = &gSaveDataBuffer;
    if (sectorId != FULL_SAVE_SLOT)
    {
        // This function may not be used with a specific sector id
        status = SAVE_STATUS_ERROR;
    }
    else
    {
        status = GetSaveValidStatus(locations);
        CopySaveSlotData(FULL_SAVE_SLOT, locations);
    }

    return status;
}

// sectorId arg is ignored, this always reads the full save slot
static u8 CopySaveSlotData(u16 sectorId, struct SaveSectorLocation *locations)
{
    u16 i;
    u16 checksum;
    u16 slotOffset = NUM_SECTORS_PER_SLOT * (gSaveCounter % NUM_SAVE_SLOTS);
    u16 id;

    for (i = 0; i < NUM_SECTORS_PER_SLOT; i++)
    {
        ReadFlashSector(i + slotOffset, gReadWriteSector);

        id = gReadWriteSector->id;
        if (id == 0)
            gLastWrittenSector = i;

        checksum = CalculateChecksum(gReadWriteSector->data, SECTOR_DATA_SIZE);

        // Only copy data for sectors whose signature and checksum fields are correct
        if (gReadWriteSector->signature == SECTOR_SIGNATURE && gReadWriteSector->checksum == checksum)
        {
            u16 j;
            for (j = 0; j < locations[id].size; j++)
                ((u8 *)locations[id].data)[j] = gReadWriteSector->data[j];
        }
    }

    return SAVE_STATUS_OK;
}

static u8 GetSaveValidStatus(const struct SaveSectorLocation *locations)
{
    u16 i;
    u16 checksum;
    u32 saveSlot1Counter = 0;
    u32 saveSlot2Counter = 0;
    u32 validSectorFlags = 0;
    bool8 signatureValid = FALSE;
    u8 saveSlot1Status;
    u8 saveSlot2Status;

    // Check save slot 1
    for (i = 0; i < NUM_SECTORS_PER_SLOT; i++)
    {
        ReadFlashSector(i, gReadWriteSector);
        if (gReadWriteSector->signature == SECTOR_SIGNATURE)
        {
            signatureValid = TRUE;
            checksum = CalculateChecksum(gReadWriteSector->data, SECTOR_DATA_SIZE);
            if (gReadWriteSector->checksum == checksum)
            {
                saveSlot1Counter = gReadWriteSector->counter;
                validSectorFlags |= 1 << gReadWriteSector->id;
            }
        }
    }

    if (signatureValid)
    {
        if (validSectorFlags == (1 << NUM_SECTORS_PER_SLOT) - 1)
            saveSlot1Status = SAVE_STATUS_OK;
        else
            saveSlot1Status = SAVE_STATUS_ERROR;
    }
    else
    {
        // No sectors in slot 1 have the correct signature, treat it as empty
        saveSlot1Status = SAVE_STATUS_EMPTY;
    }

    validSectorFlags = 0;
    signatureValid = FALSE;

    // Check save slot 2
    for (i = 0; i < NUM_SECTORS_PER_SLOT; i++)
    {
        ReadFlashSector(i + NUM_SECTORS_PER_SLOT, gReadWriteSector);
        if (gReadWriteSector->signature == SECTOR_SIGNATURE)
        {
            signatureValid = TRUE;
            checksum = CalculateChecksum(gReadWriteSector->data, SECTOR_DATA_SIZE);
            if (gReadWriteSector->checksum == checksum)
            {
                saveSlot2Counter = gReadWriteSector->counter;
                validSectorFlags |= 1 << gReadWriteSector->id;
            }
        }
    }

    if (signatureValid)
    {
        if (validSectorFlags == (1 << NUM_SECTORS_PER_SLOT) - 1)
            saveSlot2Status = SAVE_STATUS_OK;
        else
            saveSlot2Status = SAVE_STATUS_ERROR;
    }
    else
    {
        // No sectors in slot 2 have the correct signature, treat it as empty.
        saveSlot2Status = SAVE_STATUS_EMPTY;
    }

    if (saveSlot1Status == SAVE_STATUS_OK && saveSlot2Status == SAVE_STATUS_OK)
    {
        if ((saveSlot1Counter == -1 && saveSlot2Counter ==  0)
         || (saveSlot1Counter ==  0 && saveSlot2Counter == -1))
        {
            if ((unsigned)(saveSlot1Counter + 1) < (unsigned)(saveSlot2Counter + 1))
                gSaveCounter = saveSlot2Counter;
            else
                gSaveCounter = saveSlot1Counter;
        }
        else
        {
            if (saveSlot1Counter < saveSlot2Counter)
                gSaveCounter = saveSlot2Counter;
            else
                gSaveCounter = saveSlot1Counter;
        }
        return SAVE_STATUS_OK;
    }

    // One or both save slots are not OK

    if (saveSlot1Status == SAVE_STATUS_OK)
    {
        gSaveCounter = saveSlot1Counter;
        if (saveSlot2Status == SAVE_STATUS_ERROR)
            return SAVE_STATUS_ERROR; // Slot 2 errored
        return SAVE_STATUS_OK; // Slot 1 is OK, slot 2 is empty
    }

    if (saveSlot2Status == SAVE_STATUS_OK)
    {
        gSaveCounter = saveSlot2Counter;
        if (saveSlot1Status == SAVE_STATUS_ERROR)
            return SAVE_STATUS_ERROR; // Slot 1 errored
        return SAVE_STATUS_OK; // Slot 2 is OK, slot 1 is empty
    }

    // Neither slot is OK, check if both are empty
    if (saveSlot1Status == SAVE_STATUS_EMPTY
     && saveSlot2Status == SAVE_STATUS_EMPTY)
    {
        gSaveCounter = 0;
        gLastWrittenSector = 0;
        return SAVE_STATUS_EMPTY;
    }

    // Both slots errored
    gSaveCounter = 0;
    gLastWrittenSector = 0;
    return SAVE_STATUS_CORRUPT;
}

static u8 TryLoadSaveSector(u8 sectorId, u8 *data, u16 size)
{
    u16 i;
    struct SaveSector *sector = &gSaveDataBuffer;
    ReadFlashSector(sectorId, sector);
    if (sector->signature == SECTOR_SIGNATURE)
    {
        u16 checksum = CalculateChecksum(sector->data, SECTOR_DATA_SIZE);
        if (sector->id == checksum)
        {
            // Signature and checksum are correct, copy data
            for (i = 0; i < size; i++)
                data[i] = sector->data[i];
            return SAVE_STATUS_OK;
        }
        else
        {
            // Incorrect checksum
            return SAVE_STATUS_CORRUPT;
        }
    }
    else
    {
        // Incorrect signature value
        return SAVE_STATUS_EMPTY;
    }
}

// Return value always ignored
static bool8 ReadFlashSector(u8 sectorId, struct SaveSector *sector)
{
    ReadFlash(sectorId, 0, sector->data, SECTOR_SIZE);
    return TRUE;
}

static u16 CalculateChecksum(void *data, u16 size)
{
    u16 i;
    u32 checksum = 0;

    for (i = 0; i < (size / 4); i++)
    {
        checksum += *((u32 *)data);
        data += sizeof(u32);
    }

    return ((checksum >> 16) + checksum);
}

static void UpdateSaveAddresses(void)
{
    int i = SECTOR_ID_SAVEBLOCK2;
    gRamSaveSectorLocations[i].data = (void *)(gSaveBlock2Ptr) + sSaveSlotLayout[i].offset;
    gRamSaveSectorLocations[i].size = sSaveSlotLayout[i].size;

    for (i = SECTOR_ID_SAVEBLOCK1_START; i <= SECTOR_ID_SAVEBLOCK1_END; i++)
    {
        gRamSaveSectorLocations[i].data = (void *)(gSaveBlock1Ptr) + sSaveSlotLayout[i].offset;
        gRamSaveSectorLocations[i].size = sSaveSlotLayout[i].size;
    }

    for (; i <= SECTOR_ID_PKMN_STORAGE_END; i++) //setting i to SECTOR_ID_PKMN_STORAGE_START does not match
    {
        gRamSaveSectorLocations[i].data = (void *)(gPokemonStoragePtr) + sSaveSlotLayout[i].offset;
        gRamSaveSectorLocations[i].size = sSaveSlotLayout[i].size;
    }
}

u8 HandleSavingData(u8 saveType)
{
    u8 i;
    u32 *backupVar = gTrainerHillVBlankCounter;
    u8 *tempAddr;

    gTrainerHillVBlankCounter = NULL;
    UpdateSaveAddresses();
    switch (saveType)
    {
    case SAVE_HALL_OF_FAME_ERASE_BEFORE:
        // Unused. Erases the special save sectors (HOF, Trainer Hill, Recorded Battle)
        // before overwriting HOF.
        for (i = SECTOR_ID_HOF_1; i < SECTORS_COUNT; i++)
            EraseFlashSector(i);
        // fallthrough
    case SAVE_HALL_OF_FAME:
        if (GetGameStat(GAME_STAT_ENTERED_HOF) < 999)
            IncrementGameStat(GAME_STAT_ENTERED_HOF);

        // Write the full save slot first
        CopyPartyAndObjectsToSave();
        WriteSaveSectorOrSlot(FULL_SAVE_SLOT, gRamSaveSectorLocations);

        // Save the Hall of Fame
        tempAddr = gDecompressionBuffer;
        HandleWriteSectorNBytes(SECTOR_ID_HOF_1, tempAddr, SECTOR_DATA_SIZE);
        HandleWriteSectorNBytes(SECTOR_ID_HOF_2, tempAddr + SECTOR_DATA_SIZE, SECTOR_DATA_SIZE);
        break;
    case SAVE_NORMAL:
    default:
        CopyPartyAndObjectsToSave();
        WriteSaveSectorOrSlot(FULL_SAVE_SLOT, gRamSaveSectorLocations);
        break;
    case SAVE_LINK:
    case SAVE_EREADER: // Dummied, now duplicate of SAVE_LINK
        // Used by link / Battle Frontier
        // Write only SaveBlocks 1 and 2 (skips the PC)
        CopyPartyAndObjectsToSave();
        for(i = SECTOR_ID_SAVEBLOCK2; i <= SECTOR_ID_SAVEBLOCK1_END; i++)
            HandleReplaceSector(i, gRamSaveSectorLocations);
        for(i = SECTOR_ID_SAVEBLOCK2; i <= SECTOR_ID_SAVEBLOCK1_END; i++)
            WriteSectorSignatureByte_NoOffset(i, gRamSaveSectorLocations);
        break;
    case SAVE_OVERWRITE_DIFFERENT_FILE:
        // Erase Hall of Fame
        for (i = SECTOR_ID_HOF_1; i < SECTORS_COUNT; i++)
            EraseFlashSector(i);

        // Overwrite save slot
        CopyPartyAndObjectsToSave();
        WriteSaveSectorOrSlot(FULL_SAVE_SLOT, gRamSaveSectorLocations);
        break;
    }
    gTrainerHillVBlankCounter = backupVar;
    return 0;
}

u8 TrySavingData(u8 saveType)
{
    if (gFlashMemoryPresent != TRUE)
    {
        gSaveAttemptStatus = SAVE_STATUS_ERROR;
        return SAVE_STATUS_ERROR;
    }

    HandleSavingData(saveType);
    if (!gDamagedSaveSectors)
    {
        gSaveAttemptStatus = SAVE_STATUS_OK;
        return SAVE_STATUS_OK;
    }
    else
    {
        DoSaveFailedScreen(saveType);
        gSaveAttemptStatus = SAVE_STATUS_ERROR;
        return SAVE_STATUS_ERROR;
    }
}

bool8 LinkFullSave_Init(void)
{
    if (gFlashMemoryPresent != TRUE)
        return TRUE;
    UpdateSaveAddresses();
    CopyPartyAndObjectsToSave();
    RestoreSaveBackupVarsAndIncrement(gRamSaveSectorLocations);
    return FALSE;
}

bool8 LinkFullSave_WriteSector(void)
{
    u8 status = HandleWriteIncrementalSector(NUM_SECTORS_PER_SLOT, gRamSaveSectorLocations);
    if (gDamagedSaveSectors)
        DoSaveFailedScreen(SAVE_NORMAL);

    // In this case "error" either means that an actual error was encountered
    // or that the given max sector has been reached (meaning it has finished successfully).
    // If there was an actual error the save failed screen above will also be shown.
    if (status == SAVE_STATUS_ERROR)
        return TRUE;
    else
        return FALSE;
}

bool8 LinkFullSave_ReplaceLastSector(void)
{
    HandleReplaceSectorAndVerify(NUM_SECTORS_PER_SLOT, gRamSaveSectorLocations);
    if (gDamagedSaveSectors)
        DoSaveFailedScreen(SAVE_NORMAL);
    return FALSE;
}

bool8 LinkFullSave_SetLastSectorSignature(void)
{
    CopySectorSignatureByte(NUM_SECTORS_PER_SLOT, gRamSaveSectorLocations);
    if (gDamagedSaveSectors)
        DoSaveFailedScreen(SAVE_NORMAL);
    return FALSE;
}

u8 WriteSaveBlock2(void)
{
    if (gFlashMemoryPresent != TRUE)
        return TRUE;

    UpdateSaveAddresses();
    CopyPartyAndObjectsToSave();
    RestoreSaveBackupVars(gRamSaveSectorLocations);

    // Because RestoreSaveBackupVars is called immediately prior, gIncrementalSectorId will always be 0 below,
    // so this function only saves the first sector (SECTOR_ID_SAVEBLOCK2)
    HandleReplaceSectorAndVerify(gIncrementalSectorId + 1, gRamSaveSectorLocations);
    return FALSE;
}

// Used in conjunction with WriteSaveBlock2 to write both for certain link saves.
// This will be called repeatedly in a task, writing each sector of SaveBlock1 incrementally.
// It returns TRUE when finished.
bool8 WriteSaveBlock1Sector(void)
{
    u8 finished = FALSE;
    u16 sectorId = ++gIncrementalSectorId; // Because WriteSaveBlock2 will have been called prior, this will be SECTOR_ID_SAVEBLOCK1_START
    if (sectorId <= SECTOR_ID_SAVEBLOCK1_END)
    {
        // Write a single sector of SaveBlock1
        HandleReplaceSectorAndVerify(gIncrementalSectorId + 1, gRamSaveSectorLocations);
        WriteSectorSignatureByte(sectorId, gRamSaveSectorLocations);
    }
    else
    {
        // Beyond SaveBlock1, don't write the sector.
        // Does write 1 byte of the next sector's signature field, but as these
        // are the same for all valid sectors it doesn't matter.
        WriteSectorSignatureByte(sectorId, gRamSaveSectorLocations);
        finished = TRUE;
    }

    if (gDamagedSaveSectors)
        DoSaveFailedScreen(SAVE_LINK);

    return finished;
}

u8 LoadGameSave(u8 saveType)
{
    u8 status;

    if (gFlashMemoryPresent != TRUE)
    {
        gSaveFileStatus = SAVE_STATUS_NO_FLASH;
        return SAVE_STATUS_ERROR;
    }

    UpdateSaveAddresses();
    switch (saveType)
    {
    case SAVE_NORMAL:
    default:
        status = TryLoadSaveSlot(FULL_SAVE_SLOT, gRamSaveSectorLocations);
        TryReadSpecialSaveSector(SECTOR_ID_TRAINER_HILL, (u8 *)gSaveBlock3Ptr);
        CopyPartyAndObjectsFromSave();
        gSaveFileStatus = status;
        gGameContinueCallback = 0;
        break;
    case SAVE_HALL_OF_FAME:
        status = TryLoadSaveSector(SECTOR_ID_HOF_1, gDecompressionBuffer, SECTOR_DATA_SIZE);
        if (status == SAVE_STATUS_OK)
            status = TryLoadSaveSector(SECTOR_ID_HOF_2, &gDecompressionBuffer[SECTOR_DATA_SIZE], SECTOR_DATA_SIZE);
        break;
    }

    return status;
}

u16 GetSaveBlocksPointersBaseOffset(void)
{
    u16 i, slotOffset;
    struct SaveSector* sector;

    sector = gReadWriteSector = &gSaveDataBuffer;
    if (gFlashMemoryPresent != TRUE)
        return 0;
    UpdateSaveAddresses();
    GetSaveValidStatus(gRamSaveSectorLocations);
    slotOffset = NUM_SECTORS_PER_SLOT * (gSaveCounter % NUM_SAVE_SLOTS);
    for (i = 0; i < NUM_SECTORS_PER_SLOT; i++)
    {
        ReadFlashSector(i + slotOffset, gReadWriteSector);

        // Base offset for SaveBlock2 is calculated using the trainer id
        if (gReadWriteSector->id == SECTOR_ID_SAVEBLOCK2)
            return sector->data[offsetof(struct SaveBlock2, playerTrainerId[0])] +
                   sector->data[offsetof(struct SaveBlock2, playerTrainerId[1])] +
                   sector->data[offsetof(struct SaveBlock2, playerTrainerId[2])] +
                   sector->data[offsetof(struct SaveBlock2, playerTrainerId[3])];
    }
    return 0;
}

u32 TryReadSpecialSaveSector(u8 sector, u8 *dst)
{
    s32 i;
    s32 size;
    u8 *savData;

    if (sector != SECTOR_ID_TRAINER_HILL && sector != SECTOR_ID_RECORDED_BATTLE)
        return SAVE_STATUS_ERROR;

    ReadFlash(sector, 0, (u8 *)&gSaveDataBuffer, SECTOR_SIZE);
    if (*(u32 *)(&gSaveDataBuffer.data[0]) != SPECIAL_SECTOR_SENTINEL)
        return SAVE_STATUS_ERROR;

    // Copies whole save sector except u32 counter
    i = 0;
    size = SECTOR_COUNTER_OFFSET - 1;
    savData = &gSaveDataBuffer.data[4]; // data[4] to skip past SPECIAL_SECTOR_SENTINEL
    for (; i <= size; i++)
        dst[i] = savData[i];
    return SAVE_STATUS_OK;
}

u32 TryWriteSpecialSaveSector(u8 sector, u8 *src)
{
    s32 i;
    s32 size;
    u8 *savData;
    void *savDataBuffer;

    if (sector != SECTOR_ID_TRAINER_HILL && sector != SECTOR_ID_RECORDED_BATTLE)
        return SAVE_STATUS_ERROR;

    savDataBuffer = &gSaveDataBuffer;
    *(u32 *)(savDataBuffer) = SPECIAL_SECTOR_SENTINEL;

    // Copies whole save sector except u32 counter
    i = 0;
    size = SECTOR_COUNTER_OFFSET - 1;
    savData = &gSaveDataBuffer.data[4]; // data[4] to skip past SPECIAL_SECTOR_SENTINEL
    for (; i <= size; i++)
        savData[i] = src[i];
    if (ProgramFlashSectorAndVerify(sector, savDataBuffer) != 0)
        return SAVE_STATUS_ERROR;
    return SAVE_STATUS_OK;
}

void UpdateSaveVersion(void)
{
    u16 version;

    version = VarGet(VAR_SAVE_COMPATIBILITY);
    if (version == VANILLA_SAVE)
        UpdateVanillaSave();
    else if (version <= VERSION_RIBBON_DESCRIPTIONS)
    {
        UpdateOldHackSave();
        UpdateSaveAddresses();
    }
    else if (version == VERSION_SAVE_REFACTOR)
        gSaveBlock2Ptr->encryptionKeyHack = gSaveBlock2Ptr->encryptionKey;
    VarSet(VAR_SAVE_COMPATIBILITY, VERSION_LATEST);
}

static void UpdateVanillaSave(void)
{
    FlagClear(FLAG_REMATCH_SIDNEY);
    FlagClear(FLAG_REMATCH_PHOEBE);
    FlagClear(FLAG_REMATCH_GLACIA);
    FlagClear(FLAG_REMATCH_DRAKE);
    FlagClear(FLAG_REMATCH_WALLACE);
    FlagSet(FLAG_HIDE_MEW_CAVE_OF_ORIGIN);

    gSaveBlock1Ptr->registeredItems[0] = MapRegisteredItem(gSaveBlock1Ptr->registeredItem);
    gSaveBlock1Ptr->registeredItem = ITEM_NONE;
    gSaveBlock2Ptr->optionsDifficulty = OPTIONS_DIFFICULTY_NORMAL;
    gSaveBlock2Ptr->encryptionKeyHack = gSaveBlock2Ptr->encryptionKey;
    InitLilycoveLady();
    UpdateGiftRibbons();

    ClearItemSlots(gSaveBlock2Ptr->frontier.bagPocket_BattleItems, BAG_BATTLEITEMS_COUNT);
    ClearItemSlots(gSaveBlock2Ptr->frontier.bagPocket_Treasures, BAG_TREASURES_COUNT);

    if (gSaveBlock1Ptr->mapLayoutId == 420)
    {
        SetContinueGameWarpStatus();
        SetContinueGameWarpToHealLocation(HEAL_LOCATION_SLATEPORT_CITY);
    }
    if (gSaveBlock2Ptr->optionsButtonMode >= OPTIONS_BUTTON_MODE_L_EQUALS_A)
        gSaveBlock2Ptr->optionsButtonMode--;
    if (FlagGet(FLAG_RECEIVED_OLD_SEA_MAP) == TRUE)
        VarSet(VAR_OLD_SEA_MAP_STATE, 4);
    if (VarGet(VAR_DEX_UPGRADE_JOHTO_STARTER_STATE) >= 3)
    {
        VarSet(VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 2);
        FlagClear(FLAG_HIDE_LITTLEROOT_TOWN_BIRCHS_LAB_POKEBALL_CYNDAQUIL);
        FlagClear(FLAG_HIDE_LITTLEROOT_TOWN_BIRCHS_LAB_POKEBALL_TOTODILE);
        FlagClear(FLAG_HIDE_LITTLEROOT_TOWN_BIRCHS_LAB_POKEBALL_CHIKORITA);
    }
    if (FlagGet(FLAG_SYS_GAME_CLEAR) == FALSE)
    {
        FlagSet(FLAG_HIDE_OCEANIC_MUSEUM_REPORTER);
        FlagSet(FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_2F_POKE_BALL);
        FlagSet(FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F_POKE_BALL);

        if (FlagGet(FLAG_BADGE08_GET) == TRUE)
        {
            FlagSet(FLAG_ENABLE_WALLACE_MATCH_CALL);
            FlagClear(FLAG_ENABLE_JUAN_MATCH_CALL);
            FlagClear(TRAINER_FLAGS_START + TRAINER_JUAN_1);
        }
    }
    else
    {
        FlagClear(FLAG_HIDE_OCEANIC_MUSEUM_REPORTER);
        FlagClear(FLAG_HIDE_LITTLEROOT_TOWN_BIRCHS_LAB_RIVAL);

        if (gSaveBlock2Ptr->playerGender == MALE)
            FlagClear(FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_2F_POKE_BALL);
        else
            FlagClear(FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F_POKE_BALL);
    }
    CheckProgressFlags();
}

static void UpdateOldHackSave(void)
{
    u16 version, var;
    int i = SECTOR_ID_SAVEBLOCK2;
    u8 *sOldSaveBlock, *ptr1, *ptr2, *ptr3;
    const struct SaveBlock2Old *sOldSaveBlock2Ptr;
    const struct SaveBlock1Old *sOldSaveBlock1Ptr;
    const struct PokemonStorage *sOldPokemonStoragePtr;

    version = VarGet(VAR_SAVE_COMPATIBILITY);
    var = VarGet(VAR_OLD_SEA_MAP_STATE);
    sOldSaveBlock = AllocZeroed(SECTOR_DATA_SIZE * NUM_SECTORS_PER_SLOT);

    // Assign locations to load the old save block into the heap
    ptr1 = sOldSaveBlock; //pretend this is gSaveBlock2Ptr
    ptr2 = sOldSaveBlock; //pretend this is gSaveBlock1Ptr
    ptr3 = sOldSaveBlock; //pretend this is gPokemonStoragePtr

    gRamSaveSectorLocations[i].data = (void *)(ptr1) + sOldSaveSlotLayout[i].offset;
    gRamSaveSectorLocations[i].size = sOldSaveSlotLayout[i].size;
    ptr3 = ptr2 = ptr1 + sOldSaveSlotLayout[i].size;

    for (i = SECTOR_ID_SAVEBLOCK1_START; i <= SECTOR_ID_SAVEBLOCK1_END; i++)
    {
        gRamSaveSectorLocations[i].data = (void *)(ptr2) + sOldSaveSlotLayout[i].offset;
        gRamSaveSectorLocations[i].size = sOldSaveSlotLayout[i].size;
        ptr3 += sOldSaveSlotLayout[i].size;
    }

    for (i = SECTOR_ID_PKMN_STORAGE_START; i <= SECTOR_ID_PKMN_STORAGE_END; i++)
    {
        gRamSaveSectorLocations[i].data = (void *)(ptr3) + sOldSaveSlotLayout[i].offset;
        gRamSaveSectorLocations[i].size = sOldSaveSlotLayout[i].size;
    }
    // Load the save from FLASH and onto the heap
    CopySaveSlotData(FULL_SAVE_SLOT, gRamSaveSectorLocations);

    sOldSaveBlock2Ptr = (struct SaveBlock2Old*)(gRamSaveSectorLocations[SECTOR_ID_SAVEBLOCK2].data);
    sOldSaveBlock1Ptr = (struct SaveBlock1Old*)(gRamSaveSectorLocations[SECTOR_ID_SAVEBLOCK1_START].data);
    sOldPokemonStoragePtr = (struct PokemonStorage*)(gRamSaveSectorLocations[SECTOR_ID_PKMN_STORAGE_START].data);

    CpuCopy16(&sOldSaveBlock2Ptr->frontier.ereaderTrainer, &gSaveBlock3Ptr->ereaderTrainer, sizeof(gSaveBlock3Ptr->ereaderTrainer));
    CpuCopy16(&sOldSaveBlock1Ptr->enigmaBerry, &gSaveBlock3Ptr->enigmaBerry, sizeof(gSaveBlock3Ptr->enigmaBerry));
    CpuCopy16(sOldSaveBlock1Ptr->bagPocket_BattleItems, gSaveBlock2Ptr->frontier.bagPocket_BattleItems, sizeof(gSaveBlock2Ptr->frontier.bagPocket_BattleItems));
    CpuCopy16(sOldSaveBlock1Ptr->bagPocket_Treasures, gSaveBlock2Ptr->frontier.bagPocket_Treasures, sizeof(gSaveBlock2Ptr->frontier.bagPocket_Treasures));
    CpuCopy16(sOldSaveBlock1Ptr->bagPocket_Mail, gSaveBlock1Ptr->bagPocket_Mail, sizeof(gSaveBlock1Ptr->bagPocket_Mail));
    CpuCopy16(&sOldSaveBlock1Ptr->contestLady, &gSaveBlock1Ptr->contestLady, sizeof(gSaveBlock1Ptr->contestLady));
    CpuCopy16(gSaveBlock2Ptr->pokedex.seen, gSaveBlock1Ptr->seen1, sizeof(gSaveBlock2Ptr->pokedex.seen));
    CpuCopy16(gSaveBlock2Ptr->pokedex.seen, gSaveBlock1Ptr->seen2, sizeof(gSaveBlock2Ptr->pokedex.seen));

    gSaveBlock2Ptr->encryptionKeyHack = gSaveBlock2Ptr->encryptionKey;

    gSaveBlock2Ptr->optionsBattleStyle = (sOldSaveBlock2Ptr->optionsDifficulty == OPTIONS_DIFFICULTY_HARD) ? OPTIONS_BATTLE_STYLE_SET : OPTIONS_BATTLE_STYLE_SHIFT;
    gSaveBlock2Ptr->optionsDifficulty = sOldSaveBlock2Ptr->optionsDifficulty;
    gSaveBlock2Ptr->optionsBattleSceneOff = sOldSaveBlock2Ptr->optionsBattleSceneOff;
    gSaveBlock2Ptr->regionMapZoom = sOldSaveBlock2Ptr->regionMapZoom;
    gSaveBlock2Ptr->optionsAttackStyle = sOldSaveBlock2Ptr->optionsAttackStyle;
    gSaveBlock2Ptr->optionsDisableMatchCall = sOldSaveBlock2Ptr->optionsDisableMatchCall;
    gSaveBlock2Ptr->optionsUnitSystem = sOldSaveBlock2Ptr->optionsUnitSystem;
    gSaveBlock2Ptr->optionsCurrentFont = sOldSaveBlock2Ptr->optionsCurrentFont;

    for (i = 0; i < REGISTERED_ITEMS_LIST_COUNT; i++)
    {
        if (sOldSaveBlock1Ptr->registeredItems[i] == ITEM_BICYCLE)
            gSaveBlock1Ptr->registeredItems[i] = MapRegisteredItem(ITEM_MACH_BIKE);
        else
            gSaveBlock1Ptr->registeredItems[i] = MapRegisteredItem(sOldSaveBlock1Ptr->registeredItems[i]);
    }

    if (var >= 1)
    {
        FlagSet(FLAG_OCEANIC_MUSEUM_MET_REPORTER);
        VarSet(VAR_OLD_SEA_MAP_STATE, var - 1); // decrement var to account for restored vanilla reporter check, ignore if never spoken to
    }
    if (FlagGet(FLAG_IS_CHAMPION) == TRUE) // flag previously used for FLAG_WON_LEAGUE_REMATCHES
        FlagSet(FLAG_WON_LEAGUE_REMATCHES);
    if (FlagGet(FLAG_SYS_TV_HOME) == TRUE) // flag previously used for FLAG_SYS_LEGENDARY_BEASTS_FIRST_TRIGGER
        FlagSet(FLAG_SYS_LEGENDARY_BEASTS_FIRST_TRIGGER);
    if (CheckBagHasItem(ITEM_BICYCLE, 1))
    {
        RemoveBagItem(ITEM_BICYCLE, 1);
        AddBagItem(ITEM_MACH_BIKE, 1); // return a vanilla bike to saves to prevent breaking game if save is brought back to vanilla
    }
    if (CheckBagHasItem(ITEM_15B, 1))
    {
        FlagSet(FLAG_RECEIVED_OVAL_CHARM); // enable Oval Charm functionality
        RemoveBagItem(ITEM_15B, 1);
    }
    if (CheckBagHasItem(ITEM_15C, 1))
    {
        FlagSet(FLAG_RECEIVED_SHINY_CHARM); // enable Shiny Charm functionality
        RemoveBagItem(ITEM_15C, 1);
    }
    if (VarGet(VAR_LITTLEROOT_HOUSES_STATE_MAY) >= 4) // var set to 4 after triggering roaming Lati
        FlagSet(FLAG_DEFEATED_ROAMING_LATI);
    if (VarGet(VAR_LITTLEROOT_INTRO_STATE) >= 7) // var set to 7 after watching TV at beginning of game in vanilla
        FlagSet(FLAG_SYS_TV_HOME);
    if (VarGet(VAR_LITTLEROOT_TOWN_STATE) >= 4) // var set to 4 after getting Running Shoes in vanilla
        FlagSet(FLAG_SYS_B_DASH);
    if (FlagGet(FLAG_SYS_LEGENDARY_BEASTS_FIRST_TRIGGER) == TRUE)
    {
        ClearRoamerData();
        FlagSet(FLAG_DEFEATED_ROAMING_RAIKOU);
        FlagSet(FLAG_DEFEATED_ROAMING_ENTEI);
        FlagSet(FLAG_DEFEATED_ROAMING_SUICUNE);
    }
    if (FlagGet(FLAG_RECEIVED_MYSTIC_TICKET) == TRUE)
        FlagSet(FLAG_ENABLE_SHIP_NAVEL_ROCK);
    if (FlagGet(FLAG_RECEIVED_AURORA_TICKET) == TRUE)
        FlagSet(FLAG_ENABLE_SHIP_BIRTH_ISLAND);
    if (FlagGet(FLAG_RECEIVED_OLD_SEA_MAP) == TRUE)
        FlagSet(FLAG_ENABLE_SHIP_FARAWAY_ISLAND);
    CheckProgressFlags();    

    gSaveBlock1Ptr->roamer.ivs = sOldSaveBlock1Ptr->roam[ROAMING_LATI].ivs;
    gSaveBlock1Ptr->roamer.personality = sOldSaveBlock1Ptr->roam[ROAMING_LATI].personality;
    gSaveBlock1Ptr->roamer.species = sOldSaveBlock1Ptr->roam[ROAMING_LATI].species;
    gSaveBlock1Ptr->roamer.hp = sOldSaveBlock1Ptr->roam[ROAMING_LATI].hp;
    gSaveBlock1Ptr->roamer.level = sOldSaveBlock1Ptr->roam[ROAMING_LATI].level;
    gSaveBlock1Ptr->roamer.status = sOldSaveBlock1Ptr->roam[ROAMING_LATI].status;
    gSaveBlock1Ptr->roamer.active = sOldSaveBlock1Ptr->roam[ROAMING_LATI].active;

    if (version <= VERSION_CATCH_EXP_EVOLVE_FIX)
        UpdateGiftRibbons();

    if (version == VERSION_LAUNCH)
        AddBagItem(ITEM_HEART_SCALE, 1); // Courtesy gift for players affected by catch exp. evolution glitch

    Free(sOldSaveBlock);
}

static void CheckProgressFlags(void)
{
    FlagSet(FLAG_IS_CHAMPION);
    if (VarGet(VAR_MOSSDEEP_CITY_STATE) <= 2)
        FlagSet(FLAG_HIDE_MOSSDEEP_WISH_ROCK_GIRL);
    if (FlagGet(FLAG_RECEIVED_BELDUM) == TRUE)
        FlagSet(FLAG_READ_STEVENS_LETTER);
    if (FlagGet(FLAG_SYS_GAME_CLEAR) == FALSE)
        FlagSet(FLAG_HIDE_ROUTE_103_SNORLAX);
}

static void UpdateGiftRibbons(void)
{
    gSaveBlock1Ptr->giftRibbons[COUNTRY_RIBBON - FIRST_GIFT_RIBBON] = GENERIC_TOURNAMENT_RIBBON;
    gSaveBlock1Ptr->giftRibbons[NATIONAL_RIBBON - FIRST_GIFT_RIBBON] = DIFFICULTY_CLEARING_RIBBON;
    gSaveBlock1Ptr->giftRibbons[EARTH_RIBBON - FIRST_GIFT_RIBBON] = HUNDRED_STRAIGHT_WINS_RIBBON;
    gSaveBlock1Ptr->giftRibbons[WORLD_RIBBON - FIRST_GIFT_RIBBON] = GENERIC_TOURNAMENT_RIBBON;
}

void MoveItemsToCorrectPocket(void)
{
    u32 i;
    struct BagPocket *medicine = &gBagPockets[MEDICINE_POCKET];
    struct BagPocket *keyitems = &gBagPockets[KEYITEMS_POCKET];

    ApplyLastHackEncryptionKeyToNewPockets(gSaveBlock2Ptr->encryptionKeyHack);
    for (i = 0; i < TM_FLAGS; i++)
    {
        if (FlagGet(sTMFlagChecks[i][0]) && CheckBagHasItem(sTMFlagChecks[i][1], 1) == FALSE)
            AddBagItem(sTMFlagChecks[i][1], 1);
    }
    if (VarGet(VAR_TRICK_HOUSE_LEVEL) > 5 && CheckBagHasItem(ITEM_TM12, 1) == FALSE)
        AddBagItem(ITEM_TM12, 1);

    for (i = 0; i < BAG_MEDICINE_COUNT; i++) // BAG_MEDICINE_COUNT is the same as BAG_KEYITEMS_COUNT (30)
    {
        if (ItemId_GetPocket(medicine->itemSlots[i].itemId) != POCKET_MEDICINE) // Check for items moved to new pockets
        {
            AddBagItem(medicine->itemSlots[i].itemId, medicine->itemSlots[i].quantity ^ gSaveBlock2Ptr->encryptionKey);
            medicine->itemSlots[i].itemId =  ITEM_NONE;
            medicine->itemSlots[i].quantity =  0 ^ gSaveBlock2Ptr->encryptionKey;
        }

        if (ItemId_GetPocket(keyitems->itemSlots[i].itemId) != POCKET_KEY_ITEMS) // Check for fossils
        {
            AddBagItem(keyitems->itemSlots[i].itemId, keyitems->itemSlots[i].quantity ^ gSaveBlock2Ptr->encryptionKey);
            keyitems->itemSlots[i].itemId =  ITEM_NONE;
            keyitems->itemSlots[i].quantity =  0 ^ gSaveBlock2Ptr->encryptionKey;
        }
    }
    if (gSaveBlock1Ptr->pcItems[0].itemId != ITEM_NONE && FlagGet(FLAG_SYS_NATIVE_SAVE) == TRUE) // Move PC items to bag if not imported save
    {
        for (i = 0; i < PC_ITEMS_COUNT; i++)
        {
            AddBagItem(gSaveBlock1Ptr->pcItems[i].itemId, gSaveBlock1Ptr->pcItems[i].quantity);
            gSaveBlock1Ptr->pcItems[i].itemId = ITEM_NONE;
            gSaveBlock1Ptr->pcItems[i].quantity = 0;
        }
    }
}

#define tState         data[0]
#define tTimer         data[1]
#define tInBattleTower data[2]

// Note that this is very different from TrySavingData(SAVE_LINK).
// Most notably it does save the PC data.
void Task_LinkFullSave(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    switch (tState)
    {
    case 0:
        gSoftResetDisabled = TRUE;
        tState = 1;
        break;
    case 1:
        SetLinkStandbyCallback();
        tState = 2;
        break;
    case 2:
        if (IsLinkTaskFinished())
        {
            if (!tInBattleTower)
                SaveMapView();
            tState = 3;
        }
        break;
    case 3:
        if (!tInBattleTower)
            SetContinueGameWarpStatusToDynamicWarp();
        LinkFullSave_Init();
        tState = 4;
        break;
    case 4:
        if (++tTimer == 5)
        {
            tTimer = 0;
            tState = 5;
        }
        break;
    case 5:
        if (LinkFullSave_WriteSector())
            tState = 6;
        else
            tState = 4; // Not finished, delay again
        break;
    case 6:
        LinkFullSave_ReplaceLastSector();
        tState = 7;
        break;
    case 7:
        if (!tInBattleTower)
            ClearContinueGameWarpStatus2();
        SetLinkStandbyCallback();
        tState = 8;
        break;
    case 8:
        if (IsLinkTaskFinished())
        {
            LinkFullSave_SetLastSectorSignature();
            tState = 9;
        }
        break;
    case 9:
        SetLinkStandbyCallback();
        tState = 10;
        break;
    case 10:
        if (IsLinkTaskFinished())
            tState++;
        break;
    case 11:
        if (++tTimer > 5)
        {
            gSoftResetDisabled = FALSE;
            DestroyTask(taskId);
        }
        break;
    }
}
