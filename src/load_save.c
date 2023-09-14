#include "global.h"
#include "malloc.h"
#include "berry_powder.h"
#include "event_data.h"
#include "item.h"
#include "item_menu.h"
#include "lilycove_lady.h"
#include "load_save.h"
#include "main.h"
#include "overworld.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "random.h"
#include "roamer.h"
#include "save_location.h"
#include "trainer_hill.h"
#include "gba/flash_internal.h"
#include "decoration_inventory.h"
#include "agb_flash.h"
#include "constants/heal_locations.h"
#include "constants/items.h"

static void UpdateVanillaSave(void);
static void UpdateOldHackSave(void);
static void UpdateGiftRibbons(void);
static void CheckProgressFlags(void);
static void ApplyNewEncryptionKeyToAllEncryptedData(u32 encryptionKey);

#define SAVEBLOCK_MOVE_RANGE    128
#define TM_FLAGS 41

struct LoadedSaveData
{
 /*0x0000*/ struct ItemSlot items[BAG_ITEMS_COUNT];
 /*0x012C*/ struct ItemSlot keyItems[BAG_KEYITEMS_COUNT];
 /*0x01A4*/ struct ItemSlot pokeBalls[BAG_POKEBALLS_COUNT];
 /*0x01E4*/ struct ItemSlot TMsHMs[BAG_TMHM_COUNT];
 /*0x02E4*/ struct ItemSlot berries[BAG_BERRIES_COUNT];
 /*0x039C*/ struct ItemSlot medicine[BAG_MEDICINE_COUNT];
 /*0x0414*/ struct ItemSlot battleItems[BAG_BATTLEITEMS_COUNT];
 /*0x0440*/ struct ItemSlot treasures[BAG_TREASURES_COUNT];
 /*0x048C*/ struct ItemSlot bagMail[BAG_MAIL_COUNT];
 /*0x04BC*/ struct Mail mail[MAIL_COUNT];
};

// EWRAM DATA
EWRAM_DATA struct SaveBlock2ASLR gSaveblock2 = {0};
EWRAM_DATA struct SaveBlock1ASLR gSaveblock1 = {0};
EWRAM_DATA struct PokemonStorageASLR gPokemonStorage = {0};
EWRAM_DATA struct SaveBlock3 gSaveBlock3 = {0};

EWRAM_DATA struct LoadedSaveData gLoadedSaveData = {0};
EWRAM_DATA u32 gLastEncryptionKey = 0;

// IWRAM common
bool32 gFlashMemoryPresent;
struct SaveBlock1 *gSaveBlock1Ptr;
struct SaveBlock2 *gSaveBlock2Ptr;
struct PokemonStorage *gPokemonStoragePtr;
struct SaveBlock3 *gSaveBlock3Ptr;

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

// code
void CheckForFlashMemory(void)
{
    if (!IdentifyFlash())
    {
        gFlashMemoryPresent = TRUE;
        InitFlashTimer();
    }
    else
    {
        gFlashMemoryPresent = FALSE;
    }
}

void ClearSav2(void)
{
    CpuFill16(0, &gSaveblock2, sizeof(struct SaveBlock2ASLR));
}

void ClearSav1(void)
{
    CpuFill16(0, &gSaveblock1, sizeof(struct SaveBlock1ASLR));
}

// Offset is the sum of the trainer id bytes
void SetSaveBlocksPointers(u16 offset)
{
    struct SaveBlock1** sav1_LocalVar = &gSaveBlock1Ptr;

    offset = (offset + Random()) & (SAVEBLOCK_MOVE_RANGE - 4);

    gSaveBlock2Ptr = (void *)(&gSaveblock2) + offset;
    *sav1_LocalVar = (void *)(&gSaveblock1) + offset;
    gPokemonStoragePtr = (void *)(&gPokemonStorage) + offset;
    gSaveBlock3Ptr = &gSaveBlock3;

    SetBagItemsPointers();
    SetDecorationInventoriesPointers();
}

void MoveSaveBlocks_ResetHeap(void)
{
    void *vblankCB, *hblankCB;
    u32 encryptionKey;
    struct SaveBlock2 *saveBlock2Copy;
    struct SaveBlock1 *saveBlock1Copy;
    struct PokemonStorage *pokemonStorageCopy;

    // save interrupt functions and turn them off
    vblankCB = gMain.vblankCallback;
    hblankCB = gMain.hblankCallback;
    gMain.vblankCallback = NULL;
    gMain.hblankCallback = NULL;
    gTrainerHillVBlankCounter = NULL;

    saveBlock2Copy = (struct SaveBlock2 *)(gHeap);
    saveBlock1Copy = (struct SaveBlock1 *)(gHeap + sizeof(struct SaveBlock2));
    pokemonStorageCopy = (struct PokemonStorage *)(gHeap + sizeof(struct SaveBlock2) + sizeof(struct SaveBlock1));

    // backup the saves.
    *saveBlock2Copy = *gSaveBlock2Ptr;
    *saveBlock1Copy = *gSaveBlock1Ptr;
    *pokemonStorageCopy = *gPokemonStoragePtr;

    // change saveblocks' pointers
    // argument is a sum of the individual trainerId bytes
    SetSaveBlocksPointers(
      saveBlock2Copy->playerTrainerId[0] +
      saveBlock2Copy->playerTrainerId[1] +
      saveBlock2Copy->playerTrainerId[2] +
      saveBlock2Copy->playerTrainerId[3]);

    // restore saveblock data since the pointers changed
    *gSaveBlock2Ptr = *saveBlock2Copy;
    *gSaveBlock1Ptr = *saveBlock1Copy;
    *gPokemonStoragePtr = *pokemonStorageCopy;

    // heap was destroyed in the copying process, so reset it
    InitHeap(gHeap, HEAP_SIZE);

    // restore interrupt functions
    gMain.hblankCallback = hblankCB;
    gMain.vblankCallback = vblankCB;

    // create a new encryption key
    encryptionKey = (Random() << 16) + (Random());
    ApplyNewEncryptionKeyToAllEncryptedData(encryptionKey);
    gSaveBlock2Ptr->encryptionKey = encryptionKey;
}

u32 UseContinueGameWarp(void)
{
    return gSaveBlock2Ptr->specialSaveWarpFlags & CONTINUE_GAME_WARP;
}

void ClearContinueGameWarpStatus(void)
{
    gSaveBlock2Ptr->specialSaveWarpFlags &= ~CONTINUE_GAME_WARP;
}

void SetContinueGameWarpStatus(void)
{
    gSaveBlock2Ptr->specialSaveWarpFlags |= CONTINUE_GAME_WARP;
}

void SetContinueGameWarpStatusToDynamicWarp(void)
{
    SetContinueGameWarpToDynamicWarp(0);
    gSaveBlock2Ptr->specialSaveWarpFlags |= CONTINUE_GAME_WARP;
}

void ClearContinueGameWarpStatus2(void)
{
    gSaveBlock2Ptr->specialSaveWarpFlags &= ~CONTINUE_GAME_WARP;
}

void SavePlayerParty(void)
{
    int i;

    gSaveBlock1Ptr->playerPartyCount = gPlayerPartyCount;

    for (i = 0; i < PARTY_SIZE; i++)
        gSaveBlock1Ptr->playerParty[i] = gPlayerParty[i];
}

void LoadPlayerParty(void)
{
    int i;

    gPlayerPartyCount = gSaveBlock1Ptr->playerPartyCount;

    for (i = 0; i < PARTY_SIZE; i++)
        gPlayerParty[i] = gSaveBlock1Ptr->playerParty[i];
}

void SaveObjectEvents(void)
{
    int i;

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
        gSaveBlock1Ptr->objectEvents[i] = gObjectEvents[i];
}

void LoadObjectEvents(void)
{
    int i;

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
        gObjectEvents[i] = gSaveBlock1Ptr->objectEvents[i];
}

void CopyPartyAndObjectsToSave(void)
{
    SavePlayerParty();
    SaveObjectEvents();
}

void CopyPartyAndObjectsFromSave(void)
{
    LoadPlayerParty();
    LoadObjectEvents();
}

void LoadPlayerBag(void)
{
    int i;

    // load player items.
    for (i = 0; i < BAG_ITEMS_COUNT; i++)
        gLoadedSaveData.items[i] = gSaveBlock1Ptr->bagPocket_Items[i];

    // load player key items.
    for (i = 0; i < BAG_KEYITEMS_COUNT; i++)
        gLoadedSaveData.keyItems[i] = gSaveBlock1Ptr->bagPocket_KeyItems[i];

    // load player pokeballs.
    for (i = 0; i < BAG_POKEBALLS_COUNT; i++)
        gLoadedSaveData.pokeBalls[i] = gSaveBlock1Ptr->bagPocket_PokeBalls[i];

    // load player TMs and HMs.
    for (i = 0; i < BAG_TMHM_COUNT; i++)
        gLoadedSaveData.TMsHMs[i] = gSaveBlock1Ptr->bagPocket_TMHM[i];

    // load player berries.
    for (i = 0; i < BAG_BERRIES_COUNT; i++)
        gLoadedSaveData.berries[i] = gSaveBlock1Ptr->bagPocket_Berries[i];

    // load player medicine.
    for (i = 0; i < BAG_MEDICINE_COUNT; i++)
        gLoadedSaveData.medicine[i] = gSaveBlock1Ptr->bagPocket_Medicine[i];

    // load player battle items.
    for (i = 0; i < BAG_BATTLEITEMS_COUNT; i++)
        gLoadedSaveData.battleItems[i] = gSaveBlock2Ptr->frontier.bagPocket_BattleItems[i];

    // load player treasures.
    for (i = 0; i < BAG_TREASURES_COUNT; i++)
        gLoadedSaveData.treasures[i] = gSaveBlock2Ptr->frontier.bagPocket_Treasures[i];

    // load player mail in bag.
    for (i = 0; i < BAG_MAIL_COUNT; i++)
        gLoadedSaveData.bagMail[i] = gSaveBlock1Ptr->bagPocket_Mail[i];

    // load mail.
    for (i = 0; i < MAIL_COUNT; i++)
        gLoadedSaveData.mail[i] = gSaveBlock1Ptr->mail[i];

    gLastEncryptionKey = gSaveBlock2Ptr->encryptionKey;
}

void SavePlayerBag(void)
{
    int i;
    u32 encryptionKeyBackup;

    // save player items.
    for (i = 0; i < BAG_ITEMS_COUNT; i++)
        gSaveBlock1Ptr->bagPocket_Items[i] = gLoadedSaveData.items[i];

    // save player key items.
    for (i = 0; i < BAG_KEYITEMS_COUNT; i++)
        gSaveBlock1Ptr->bagPocket_KeyItems[i] = gLoadedSaveData.keyItems[i];

    // save player pokeballs.
    for (i = 0; i < BAG_POKEBALLS_COUNT; i++)
        gSaveBlock1Ptr->bagPocket_PokeBalls[i] = gLoadedSaveData.pokeBalls[i];

    // save player TMs and HMs.
    for (i = 0; i < BAG_TMHM_COUNT; i++)
        gSaveBlock1Ptr->bagPocket_TMHM[i] = gLoadedSaveData.TMsHMs[i];

    // save player berries.
    for (i = 0; i < BAG_BERRIES_COUNT; i++)
        gSaveBlock1Ptr->bagPocket_Berries[i] = gLoadedSaveData.berries[i];

    // save player medicine.
    for (i = 0; i < BAG_MEDICINE_COUNT; i++)
        gSaveBlock1Ptr->bagPocket_Medicine[i] = gLoadedSaveData.medicine[i];

    // save player battle items.
    for (i = 0; i < BAG_BATTLEITEMS_COUNT; i++)
        gSaveBlock2Ptr->frontier.bagPocket_BattleItems[i] = gLoadedSaveData.battleItems[i];

    // save player treasures.
    for (i = 0; i < BAG_TREASURES_COUNT; i++)
        gSaveBlock2Ptr->frontier.bagPocket_Treasures[i] = gLoadedSaveData.treasures[i];

    // save player mail in bag.
    for (i = 0; i < BAG_MAIL_COUNT; i++)
        gSaveBlock1Ptr->bagPocket_Mail[i] = gLoadedSaveData.bagMail[i];

    // save mail.
    for (i = 0; i < MAIL_COUNT; i++)
        gSaveBlock1Ptr->mail[i] = gLoadedSaveData.mail[i];

    encryptionKeyBackup = gSaveBlock2Ptr->encryptionKey;
    gSaveBlock2Ptr->encryptionKey = gLastEncryptionKey;
    ApplyNewEncryptionKeyToBagItems(encryptionKeyBackup);
    gSaveBlock2Ptr->encryptionKey = encryptionKeyBackup; // updated twice?
}

void UpdateSaveVersion(void)
{
    u16 version;

    version = VarGet(VAR_SAVE_COMPATIBILITY);
    if (version == VANILLA_SAVE)
        UpdateVanillaSave();
    else if (version <= VERSION_RIBBON_DESCRIPTIONS)
        UpdateOldHackSave();
    //VarSet(VAR_SAVE_COMPATIBILITY, VERSION_LATEST);
}

static void UpdateVanillaSave(void)
{
    FlagClear(FLAG_REMATCH_SIDNEY);
    FlagClear(FLAG_REMATCH_PHOEBE);
    FlagClear(FLAG_REMATCH_GLACIA);
    FlagClear(FLAG_REMATCH_DRAKE);
    FlagClear(FLAG_REMATCH_WALLACE);
    FlagSet(FLAG_HIDE_MEW_CAVE_OF_ORIGIN);

    gSaveBlock1Ptr->registeredItem = ITEM_NONE;
    gSaveBlock2Ptr->optionsDifficulty = OPTIONS_DIFFICULTY_NORMAL;
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

    version = VarGet(VAR_SAVE_COMPATIBILITY);

    InitLilycoveLady();
    memcpy(gSaveBlock2Ptr->frontier.bagPocket_BattleItems, gSaveBlock1Ptr->seen1, sizeof(gSaveBlock2Ptr->frontier.bagPocket_BattleItems));
    memcpy(gSaveBlock1Ptr->bagPocket_Mail, gSaveBlock1Ptr->seen2, sizeof(gSaveBlock1Ptr->bagPocket_Mail));
    memcpy(gSaveBlock1Ptr->seen1, gSaveBlock2Ptr->pokedex.seen, sizeof(gSaveBlock2Ptr->pokedex.seen));
    memcpy(gSaveBlock1Ptr->seen2, gSaveBlock2Ptr->pokedex.seen, sizeof(gSaveBlock2Ptr->pokedex.seen));

    var = VarGet(VAR_OLD_SEA_MAP_STATE);

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
        ClearRoamerLocationData();
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

    if (version <= VERSION_CATCH_EXP_EVOLVE_FIX)
        UpdateGiftRibbons();

    if (version == VERSION_LAUNCH)
        AddBagItem(ITEM_HEART_SCALE, 1); // Courtesy gift for players affected by catch exp. evolution glitch
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

void ApplyNewEncryptionKeyToHword(u16 *hWord, u32 newKey)
{
    *hWord ^= gSaveBlock2Ptr->encryptionKey;
    *hWord ^= newKey;
}

void ApplyNewEncryptionKeyToWord(u32 *word, u32 newKey)
{
    *word ^= gSaveBlock2Ptr->encryptionKey;
    *word ^= newKey;
}

static void ApplyNewEncryptionKeyToAllEncryptedData(u32 encryptionKey)
{
    ApplyNewEncryptionKeyToGameStats(encryptionKey);
    ApplyNewEncryptionKeyToBagItems(encryptionKey);
    ApplyNewEncryptionKeyToBerryPowder(encryptionKey);
    ApplyNewEncryptionKeyToWord(&gSaveBlock1Ptr->money, encryptionKey);
    ApplyNewEncryptionKeyToHword(&gSaveBlock1Ptr->coins, encryptionKey);
}
