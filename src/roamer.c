#include "global.h"
#include "event_data.h"
#include "pokemon.h"
#include "random.h"
#include "roamer.h"

// Despite having a variable to track it, the roamer is
// hard-coded to only ever be in map group 0
#define ROAMER_MAP_GROUP 0

enum
{
    MAP_GRP, // map group
    MAP_NUM, // map number
};

#define ROAMER (&gSaveBlock1Ptr->roamer)
EWRAM_DATA static u8 sLocationHistory[TOTAL_ROAMING_POKEMON][3][2] = {0};
EWRAM_DATA static u8 sRoamerLocation[TOTAL_ROAMING_POKEMON][2] = {0};
EWRAM_DATA u8 sSlot = 0;

#define ___ MAP_NUM(MAP_UNDEFINED) // For empty spots in the location table

// Note: There are two potential softlocks that can occur with this table if its maps are
//       changed in particular ways. They can be avoided by ensuring the following:
//       - There must be at least 2 location sets that start with a different map,
//         i.e. every location set cannot start with the same map. This is because of
//         the while loop in RoamerMoveToOtherLocationSet.
//       - Each location set must have at least 3 unique maps. This is because of
//         the while loop in RoamerMove. In this loop the first map in the set is
//         ignored, and an additional map is ignored if the roamer was there recently.
//       - Additionally, while not a softlock, it's worth noting that if for any
//         map in the location table there is not a location set that starts with
//         that map then the roamer will be significantly less likely to move away
//         from that map when it lands there.
static const u8 sRoamerLocations[][6] =
{
    { MAP_NUM(MAP_ROUTE101), MAP_NUM(MAP_ROUTE102), MAP_NUM(MAP_ROUTE103), MAP_NUM(MAP_ROUTE110), ___, ___ },
    { MAP_NUM(MAP_ROUTE102), MAP_NUM(MAP_ROUTE104), MAP_NUM(MAP_ROUTE101), MAP_NUM(MAP_ROUTE103), ___, ___ },
    { MAP_NUM(MAP_ROUTE103), MAP_NUM(MAP_ROUTE110), MAP_NUM(MAP_ROUTE117), MAP_NUM(MAP_ROUTE118), ___, ___ },
    { MAP_NUM(MAP_ROUTE104), MAP_NUM(MAP_ROUTE115), MAP_NUM(MAP_ROUTE116), ___, ___, ___ },
    { MAP_NUM(MAP_ROUTE105), MAP_NUM(MAP_ROUTE104), MAP_NUM(MAP_ROUTE106), ___, ___, ___ },
    { MAP_NUM(MAP_ROUTE106), MAP_NUM(MAP_ROUTE105), MAP_NUM(MAP_ROUTE107), ___, ___, ___ },
    { MAP_NUM(MAP_ROUTE107), MAP_NUM(MAP_ROUTE106), MAP_NUM(MAP_ROUTE108), ___, ___, ___ },
    { MAP_NUM(MAP_ROUTE108), MAP_NUM(MAP_ROUTE107), MAP_NUM(MAP_ROUTE109), ___, ___, ___ },
    { MAP_NUM(MAP_ROUTE109), MAP_NUM(MAP_ROUTE110), MAP_NUM(MAP_ROUTE103), MAP_NUM(MAP_ROUTE134), ___, ___ },
    { MAP_NUM(MAP_ROUTE110), MAP_NUM(MAP_ROUTE111), MAP_NUM(MAP_ROUTE117), MAP_NUM(MAP_ROUTE118), MAP_NUM(MAP_ROUTE134), ___ },
    { MAP_NUM(MAP_ROUTE111), MAP_NUM(MAP_ROUTE110), MAP_NUM(MAP_ROUTE117), MAP_NUM(MAP_ROUTE118), ___, ___ },
    { MAP_NUM(MAP_ROUTE112), MAP_NUM(MAP_ROUTE111), MAP_NUM(MAP_ROUTE117), MAP_NUM(MAP_ROUTE118), MAP_NUM(MAP_ROUTE110), MAP_NUM(MAP_ROUTE113) },
    { MAP_NUM(MAP_ROUTE113), MAP_NUM(MAP_ROUTE111), MAP_NUM(MAP_ROUTE112), MAP_NUM(MAP_ROUTE114), ___, ___ },
    { MAP_NUM(MAP_ROUTE114), MAP_NUM(MAP_ROUTE113), MAP_NUM(MAP_ROUTE111), ___, ___, ___ },
    { MAP_NUM(MAP_ROUTE115), MAP_NUM(MAP_ROUTE116), MAP_NUM(MAP_ROUTE104), ___, ___, ___ },
    { MAP_NUM(MAP_ROUTE116), MAP_NUM(MAP_ROUTE104), MAP_NUM(MAP_ROUTE115), ___, ___, ___ },
    { MAP_NUM(MAP_ROUTE117), MAP_NUM(MAP_ROUTE111), MAP_NUM(MAP_ROUTE110), MAP_NUM(MAP_ROUTE118), ___, ___ },
    { MAP_NUM(MAP_ROUTE118), MAP_NUM(MAP_ROUTE117), MAP_NUM(MAP_ROUTE110), MAP_NUM(MAP_ROUTE111), MAP_NUM(MAP_ROUTE119), ___ },
    { MAP_NUM(MAP_ROUTE119), MAP_NUM(MAP_ROUTE118), MAP_NUM(MAP_ROUTE120), ___, ___, ___ },
    { MAP_NUM(MAP_ROUTE120), MAP_NUM(MAP_ROUTE119), MAP_NUM(MAP_ROUTE121), ___, ___, ___ },
    { MAP_NUM(MAP_ROUTE121), MAP_NUM(MAP_ROUTE120), MAP_NUM(MAP_ROUTE122), MAP_NUM(MAP_ROUTE123), ___, ___ },
    { MAP_NUM(MAP_ROUTE122), MAP_NUM(MAP_ROUTE121), MAP_NUM(MAP_ROUTE123), ___, ___, ___ },
    { MAP_NUM(MAP_ROUTE123), MAP_NUM(MAP_ROUTE122), MAP_NUM(MAP_ROUTE121), ___, ___, ___ },
    { MAP_NUM(MAP_ROUTE124), MAP_NUM(MAP_ROUTE121), MAP_NUM(MAP_ROUTE125), MAP_NUM(MAP_ROUTE126), ___, ___ },
    { MAP_NUM(MAP_ROUTE125), MAP_NUM(MAP_ROUTE124), MAP_NUM(MAP_ROUTE127), ___, ___, ___ },
    { MAP_NUM(MAP_ROUTE126), MAP_NUM(MAP_ROUTE124), MAP_NUM(MAP_ROUTE127), ___, ___, ___ },
    { MAP_NUM(MAP_ROUTE127), MAP_NUM(MAP_ROUTE125), MAP_NUM(MAP_ROUTE126), MAP_NUM(MAP_ROUTE128), ___, ___ },
    { MAP_NUM(MAP_ROUTE128), MAP_NUM(MAP_ROUTE127), MAP_NUM(MAP_ROUTE129), ___, ___, ___ },
    { MAP_NUM(MAP_ROUTE129), MAP_NUM(MAP_ROUTE128), MAP_NUM(MAP_ROUTE130), ___, ___, ___ },
    { MAP_NUM(MAP_ROUTE130), MAP_NUM(MAP_ROUTE129), MAP_NUM(MAP_ROUTE131), ___, ___, ___ },
    { MAP_NUM(MAP_ROUTE131), MAP_NUM(MAP_ROUTE130), MAP_NUM(MAP_ROUTE132), ___, ___, ___ },
    { MAP_NUM(MAP_ROUTE132), MAP_NUM(MAP_ROUTE131), MAP_NUM(MAP_ROUTE130), ___, ___, ___ },
    { MAP_NUM(MAP_ROUTE134), MAP_NUM(MAP_ROUTE109), MAP_NUM(MAP_ROUTE110), ___, ___, ___ },
    { ___, ___, ___, ___, ___, ___ },
};

#undef ___
#define NUM_LOCATION_SETS (ARRAY_COUNT(sRoamerLocations) - 1)
#define NUM_LOCATIONS_PER_SET (ARRAY_COUNT(sRoamerLocations[0]))

void ClearRoamerData(void)
{
    memset(gSaveBlock1Ptr->roamerTrio, 0, sizeof(gSaveBlock1Ptr->roamerTrio));
    memset(ROAMER, 0, sizeof(*ROAMER));
    ROAMER->species = SPECIES_LATIAS;
}

void ClearRoamerLocationData(void)
{
    u8 i;

    for (sSlot = 0; sSlot < TOTAL_ROAMING_POKEMON; sSlot++)
    {
        for (i = 0; i < ARRAY_COUNT(sLocationHistory); i++)
        {
            sLocationHistory[sSlot][i][MAP_GRP] = 0;
            sLocationHistory[sSlot][i][MAP_NUM] = 0;
        }
        sRoamerLocation[sSlot][MAP_GRP] = 0;
        sRoamerLocation[sSlot][MAP_NUM] = 0;
    }
}

static void CreateInitialRoamerMon(u8 createRoamer)
{
    if (createRoamer == ROAMING_LATI)
    {
        ROAMER->species = VarGet(VAR_ROAMER_POKEMON) + SPECIES_LATIAS;
        ROAMER->level = 40;
        CreateMon(&gEnemyParty[0], ROAMER->species, ROAMER->level, USE_RANDOM_IVS, FALSE, 0, OT_ID_PLAYER_ID, 0);
        ROAMER->status = 0;
        ROAMER->active = TRUE;
        ROAMER->ivs = GetMonData(&gEnemyParty[0], MON_DATA_IVS);
        ROAMER->personality = GetMonData(&gEnemyParty[0], MON_DATA_PERSONALITY);
        ROAMER->hp = GetMonData(&gEnemyParty[0], MON_DATA_MAX_HP);
        ROAMER->cool = GetMonData(&gEnemyParty[0], MON_DATA_COOL);
        ROAMER->beauty = GetMonData(&gEnemyParty[0], MON_DATA_BEAUTY);
        ROAMER->cute = GetMonData(&gEnemyParty[0], MON_DATA_CUTE);
        ROAMER->smart = GetMonData(&gEnemyParty[0], MON_DATA_SMART);
        ROAMER->tough = GetMonData(&gEnemyParty[0], MON_DATA_TOUGH);
    }
    else
    {
        CreateMon(&gEnemyParty[0], createRoamer + SPECIES_RAIKOU, ROAMING_BEAST_LEVEL, USE_RANDOM_IVS, FALSE, 0, OT_ID_PLAYER_ID, 0);
        gSaveBlock1Ptr->roamerTrio[createRoamer].status = 0;
        gSaveBlock1Ptr->roamerTrio[createRoamer].active = TRUE;
        gSaveBlock1Ptr->roamerTrio[createRoamer].ivs = GetMonData(&gEnemyParty[0], MON_DATA_IVS);
        gSaveBlock1Ptr->roamerTrio[createRoamer].personality = GetMonData(&gEnemyParty[0], MON_DATA_PERSONALITY);
        gSaveBlock1Ptr->roamerTrio[createRoamer].hp = GetMonData(&gEnemyParty[0], MON_DATA_MAX_HP);
    }
    sRoamerLocation[createRoamer][MAP_GRP] = ROAMER_MAP_GROUP;
    sRoamerLocation[createRoamer][MAP_NUM] = sRoamerLocations[Random() % NUM_LOCATION_SETS][0];
}

void InitRoamer(void)
{
    CreateInitialRoamerMon(gSpecialVar_0x8003);
}

void UpdateLocationHistoryForRoamer(void)
{
    for (sSlot = 0; sSlot < TOTAL_ROAMING_POKEMON; sSlot++)
    {
        sLocationHistory[sSlot][2][MAP_GRP] = sLocationHistory[sSlot][1][MAP_GRP];
        sLocationHistory[sSlot][2][MAP_NUM] = sLocationHistory[sSlot][1][MAP_NUM];

        sLocationHistory[sSlot][1][MAP_GRP] = sLocationHistory[sSlot][0][MAP_GRP];
        sLocationHistory[sSlot][1][MAP_NUM] = sLocationHistory[sSlot][0][MAP_NUM];

        sLocationHistory[sSlot][0][MAP_GRP] = gSaveBlock1Ptr->location.mapGroup;
        sLocationHistory[sSlot][0][MAP_NUM] = gSaveBlock1Ptr->location.mapNum;
    }
}

void RoamerMoveToOtherLocationSet(void)
{
    u8 mapNum = 0;

    for (sSlot = 0; sSlot < TOTAL_ROAMING_POKEMON; sSlot++)
    {
        if ((sSlot < ROAMING_LATI && gSaveBlock1Ptr->roamerTrio[sSlot].active) || (sSlot == ROAMING_LATI && ROAMER->active))
        {
            // Choose a location set that starts with a map
            // different from the roamer's current map
            while (1)
            {
                mapNum = sRoamerLocations[Random() % NUM_LOCATION_SETS][0];
                if (sRoamerLocation[sSlot][MAP_NUM] != mapNum)
                {
                    sRoamerLocation[sSlot][MAP_NUM] = mapNum;
                    break;
                }
            }
        }
    }
}

void RoamerMove(void)
{
    for (sSlot = 0; sSlot < TOTAL_ROAMING_POKEMON; sSlot++)
    {        
        if ((sSlot < ROAMING_LATI && gSaveBlock1Ptr->roamerTrio[sSlot].active) || (sSlot == ROAMING_LATI && ROAMER->active))
        {
            u8 locSet = 0;
            while (locSet < NUM_LOCATION_SETS)
            {
                // Find the location set that starts with the roamer's current map
                if (sRoamerLocation[sSlot][MAP_NUM] == sRoamerLocations[locSet][0])
                {
                    u8 mapNum;
                    while (1)
                    {
                        // Choose a new map (excluding the first) within this set
                        // Also exclude a map if the roamer was there 2 moves ago
                        mapNum = sRoamerLocations[locSet][(Random() % (NUM_LOCATIONS_PER_SET - 1)) + 1];
                        if (!(sLocationHistory[sSlot][2][MAP_GRP] == ROAMER_MAP_GROUP
                           && sLocationHistory[sSlot][2][MAP_NUM] == mapNum)
                           && mapNum != MAP_NUM(MAP_UNDEFINED))
                            break;
                    }
                    sRoamerLocation[sSlot][MAP_NUM] = mapNum;
                    break;
                }
                locSet++;
            }
        }
    }
}

bool8 IsRoamerAt(u8 sSlot, u8 mapGroup, u8 mapNum)
{
    if (mapGroup == sRoamerLocation[sSlot][MAP_GRP] && mapNum == sRoamerLocation[sSlot][MAP_NUM])
    {
        if (sSlot == ROAMING_LATI && ROAMER->active)
            return TRUE;
        else if (gSaveBlock1Ptr->roamerTrio[sSlot].active)
            return TRUE;
    }
    return FALSE;
}

void CreateRoamerMonInstance(u8 id)
{
    u32 status;
    struct Pokemon *mon = &gEnemyParty[0];
    gSpecialVar_0x8003 = id;

    ZeroEnemyPartyMons();
    if (id == ROAMING_LATI)
    {
        CreateMonWithIVsPersonality(mon, ROAMER->species, ROAMER->level, ROAMER->ivs, ROAMER->personality);
        SetMonData(mon, MON_DATA_HP, &ROAMER->hp);
        SetMonData(mon, MON_DATA_COOL, &ROAMER->cool);
        SetMonData(mon, MON_DATA_BEAUTY, &ROAMER->beauty);
        SetMonData(mon, MON_DATA_CUTE, &ROAMER->cute);
        SetMonData(mon, MON_DATA_SMART, &ROAMER->smart);
        SetMonData(mon, MON_DATA_TOUGH, &ROAMER->tough);
        status = ROAMER->status;
    }
    else
    {
        CreateMonWithIVsPersonality(mon, id + SPECIES_RAIKOU, ROAMING_BEAST_LEVEL, gSaveBlock1Ptr->roamerTrio[id].ivs, gSaveBlock1Ptr->roamerTrio[id].personality);
        SetMonData(mon, MON_DATA_HP, &gSaveBlock1Ptr->roamerTrio[id].hp);
        status = gSaveBlock1Ptr->roamerTrio[id].status;
    }
    SetMonData(mon, MON_DATA_STATUS, &status);
}

bool8 TryStartRoamerEncounter(void)
{
    for (sSlot = 0; sSlot < TOTAL_ROAMING_POKEMON; sSlot++)
    {
        if (IsRoamerAt(sSlot, gSaveBlock1Ptr->location.mapGroup, gSaveBlock1Ptr->location.mapNum) == TRUE && (Random() % 4) == 0)
        {
            CreateRoamerMonInstance(sSlot);
            return TRUE;
        }
    }
    return FALSE;
}

void UpdateRoamerHPStatus(struct Pokemon *mon)
{
    if (gSpecialVar_0x8003 == ROAMING_LATI)
    {
        ROAMER->hp = GetMonData(mon, MON_DATA_HP);
        ROAMER->status = GetMonData(mon, MON_DATA_STATUS);
    }
    else
    {
        gSaveBlock1Ptr->roamerTrio[gSpecialVar_0x8003].hp = GetMonData(mon, MON_DATA_HP);
        gSaveBlock1Ptr->roamerTrio[gSpecialVar_0x8003].status = GetMonData(mon, MON_DATA_STATUS);
    }
    RoamerMoveToOtherLocationSet();
}

void SetRoamerInactive(void)
{
    if (gSpecialVar_0x8003 == ROAMING_LATI)
        ROAMER->active = FALSE;
    else
        gSaveBlock1Ptr->roamerTrio[gSpecialVar_0x8003].active = FALSE;
}

void GetRoamerLocation(u8 id, u8 *mapGroup, u8 *mapNum)
{
    *mapGroup = sRoamerLocation[id][MAP_GRP];
    *mapNum = sRoamerLocation[id][MAP_NUM];
}
