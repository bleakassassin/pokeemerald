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

EWRAM_DATA static u8 sLocationHistory[TOTAL_ROAMING_POKEMON][3][2] = {0};
EWRAM_DATA static u8 sRoamerLocation[TOTAL_ROAMING_POKEMON][2] = {0};
EWRAM_DATA u8 sSlot = 0;

#define ___ MAP_NUM(UNDEFINED) // For empty spots in the location table

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
    { MAP_NUM(ROUTE101), MAP_NUM(ROUTE102), MAP_NUM(ROUTE103), MAP_NUM(ROUTE110), ___, ___ },
    { MAP_NUM(ROUTE102), MAP_NUM(ROUTE104), MAP_NUM(ROUTE101), MAP_NUM(ROUTE103), ___, ___ },
    { MAP_NUM(ROUTE103), MAP_NUM(ROUTE110), MAP_NUM(ROUTE117), MAP_NUM(ROUTE118), ___, ___ },
    { MAP_NUM(ROUTE104), MAP_NUM(ROUTE115), MAP_NUM(ROUTE116), MAP_NUM(ROUTE102), ___, ___ },
    { MAP_NUM(ROUTE105), MAP_NUM(ROUTE104), MAP_NUM(ROUTE106), ___, ___, ___ },
    { MAP_NUM(ROUTE106), MAP_NUM(ROUTE105), MAP_NUM(ROUTE107), ___, ___, ___ },
    { MAP_NUM(ROUTE107), MAP_NUM(ROUTE106), MAP_NUM(ROUTE108), ___, ___, ___ },
    { MAP_NUM(ROUTE108), MAP_NUM(ROUTE107), MAP_NUM(ROUTE109), ___, ___, ___ },
    { MAP_NUM(ROUTE109), MAP_NUM(ROUTE110), MAP_NUM(ROUTE103), MAP_NUM(ROUTE134), ___, ___ },
    { MAP_NUM(ROUTE110), MAP_NUM(ROUTE111), MAP_NUM(ROUTE117), MAP_NUM(ROUTE118), MAP_NUM(ROUTE134), ___ },
    { MAP_NUM(ROUTE111), MAP_NUM(ROUTE110), MAP_NUM(ROUTE117), MAP_NUM(ROUTE118), ___, ___ },
    { MAP_NUM(ROUTE112), MAP_NUM(ROUTE111), MAP_NUM(ROUTE117), MAP_NUM(ROUTE118), MAP_NUM(ROUTE110), MAP_NUM(ROUTE113) },
    { MAP_NUM(ROUTE113), MAP_NUM(ROUTE111), MAP_NUM(ROUTE112), MAP_NUM(ROUTE114), ___, ___ },
    { MAP_NUM(ROUTE114), MAP_NUM(ROUTE113), MAP_NUM(ROUTE115), ___, ___, ___ },
    { MAP_NUM(ROUTE115), MAP_NUM(ROUTE116), MAP_NUM(ROUTE104), ___, ___, ___ },
    { MAP_NUM(ROUTE116), MAP_NUM(ROUTE117), MAP_NUM(ROUTE104), MAP_NUM(ROUTE115), ___, ___ },
    { MAP_NUM(ROUTE117), MAP_NUM(ROUTE111), MAP_NUM(ROUTE110), MAP_NUM(ROUTE118), ___, ___ },
    { MAP_NUM(ROUTE118), MAP_NUM(ROUTE117), MAP_NUM(ROUTE110), MAP_NUM(ROUTE111), MAP_NUM(ROUTE119), MAP_NUM(ROUTE123) },
    { MAP_NUM(ROUTE119), MAP_NUM(ROUTE118), MAP_NUM(ROUTE120), ___, ___, ___ },
    { MAP_NUM(ROUTE120), MAP_NUM(ROUTE119), MAP_NUM(ROUTE121), ___, ___, ___ },
    { MAP_NUM(ROUTE121), MAP_NUM(ROUTE120), MAP_NUM(ROUTE122), MAP_NUM(ROUTE123), ___, ___ },
    { MAP_NUM(ROUTE122), MAP_NUM(ROUTE121), MAP_NUM(ROUTE123), ___, ___, ___ },
    { MAP_NUM(ROUTE123), MAP_NUM(ROUTE122), MAP_NUM(ROUTE118), ___, ___, ___ },
    { MAP_NUM(ROUTE124), MAP_NUM(ROUTE121), MAP_NUM(ROUTE125), MAP_NUM(ROUTE126), ___, ___ },
    { MAP_NUM(ROUTE125), MAP_NUM(ROUTE124), MAP_NUM(ROUTE127), ___, ___, ___ },
    { MAP_NUM(ROUTE126), MAP_NUM(ROUTE124), MAP_NUM(ROUTE127), ___, ___, ___ },
    { MAP_NUM(ROUTE127), MAP_NUM(ROUTE125), MAP_NUM(ROUTE126), MAP_NUM(ROUTE128), ___, ___ },
    { MAP_NUM(ROUTE128), MAP_NUM(ROUTE127), MAP_NUM(ROUTE129), ___, ___, ___ },
    { MAP_NUM(ROUTE129), MAP_NUM(ROUTE128), MAP_NUM(ROUTE130), ___, ___, ___ },
    { MAP_NUM(ROUTE130), MAP_NUM(ROUTE129), MAP_NUM(ROUTE131), ___, ___, ___ },
    { MAP_NUM(ROUTE131), MAP_NUM(ROUTE130), MAP_NUM(ROUTE132), ___, ___, ___ },
    { MAP_NUM(ROUTE132), MAP_NUM(ROUTE131), MAP_NUM(ROUTE133), ___, ___, ___ },
    { MAP_NUM(ROUTE133), MAP_NUM(ROUTE132), MAP_NUM(ROUTE134), ___, ___, ___ },
    { MAP_NUM(ROUTE134), MAP_NUM(ROUTE133), MAP_NUM(ROUTE110), ___, ___, ___ },
    { ___, ___, ___, ___, ___, ___ },
};

#undef ___
#define NUM_LOCATION_SETS (ARRAY_COUNT(sRoamerLocations) - 1)
#define NUM_LOCATIONS_PER_SET (ARRAY_COUNT(sRoamerLocations[0]))

void ClearRoamerData(void)
{
    for (sSlot = 0; sSlot < TOTAL_ROAMING_POKEMON; sSlot++)
    {
        memset(&gSaveBlock1Ptr->roam[sSlot], 0, sizeof(*&gSaveBlock1Ptr->roam[sSlot]));
    }
    gSaveBlock1Ptr->roam[sSlot].species = SPECIES_LATIAS;
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
    if (createRoamer == 3)
    {
        gSaveBlock1Ptr->roam[createRoamer].species = VarGet(VAR_ROAMER_POKEMON) + SPECIES_LATIAS;
        gSaveBlock1Ptr->roam[createRoamer].level = 40;
    }
    else
    {
        gSaveBlock1Ptr->roam[createRoamer].species = createRoamer + SPECIES_RAIKOU;
        gSaveBlock1Ptr->roam[createRoamer].level = 50;
    }

    CreateMon(&gEnemyParty[0], gSaveBlock1Ptr->roam[createRoamer].species, 40, USE_RANDOM_IVS, FALSE, 0, OT_ID_PLAYER_ID, 0);
    gSaveBlock1Ptr->roam[createRoamer].status = 0;
    gSaveBlock1Ptr->roam[createRoamer].active = TRUE;
    gSaveBlock1Ptr->roam[createRoamer].ivs = GetMonData(&gEnemyParty[0], MON_DATA_IVS);
    gSaveBlock1Ptr->roam[createRoamer].personality = GetMonData(&gEnemyParty[0], MON_DATA_PERSONALITY);
    gSaveBlock1Ptr->roam[createRoamer].hp = GetMonData(&gEnemyParty[0], MON_DATA_MAX_HP);
    sRoamerLocation[createRoamer][MAP_GRP] = ROAMER_MAP_GROUP;
    sRoamerLocation[createRoamer][MAP_NUM] = sRoamerLocations[Random() % NUM_LOCATION_SETS][0];
}

void InitRoamer(void)
{
    if (gSaveBlock1Ptr->roam[gSpecialVar_0x8003].active == FALSE)
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
        if (gSaveBlock1Ptr->roam[sSlot].active)
        {
            sRoamerLocation[sSlot][MAP_GRP] = ROAMER_MAP_GROUP;

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
        if (gSaveBlock1Ptr->roam[sSlot].active)
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
                           && mapNum != MAP_NUM(UNDEFINED))
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
    if (gSaveBlock1Ptr->roam[sSlot].active && mapGroup == sRoamerLocation[sSlot][MAP_GRP] && mapNum == sRoamerLocation[sSlot][MAP_NUM])
        return TRUE;
    else
        return FALSE;
}

void CreateRoamerMonInstance(u8 id)
{
    u32 status;
    struct Pokemon *mon = &gEnemyParty[0];
    gSpecialVar_0x8003 = id;

    ZeroEnemyPartyMons();
    CreateMonWithIVsPersonality(mon, gSaveBlock1Ptr->roam[id].species, gSaveBlock1Ptr->roam[id].level, gSaveBlock1Ptr->roam[id].ivs, gSaveBlock1Ptr->roam[id].personality);
// The roamer's status field is u8, but SetMonData expects status to be u32, so will set the roamer's status
// using the status field and the following 3 bytes (cool, beauty, and cute).
#ifdef BUGFIX
    status = gSaveBlock1Ptr->roam[id].status;
    SetMonData(mon, MON_DATA_STATUS, &status);
#else
    SetMonData(mon, MON_DATA_STATUS, &gSaveBlock1Ptr->roam[id].status);
#endif
    SetMonData(mon, MON_DATA_HP, &gSaveBlock1Ptr->roam[id].hp);
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
    gSaveBlock1Ptr->roam[gSpecialVar_0x8003].hp = GetMonData(mon, MON_DATA_HP);
    gSaveBlock1Ptr->roam[gSpecialVar_0x8003].status = GetMonData(mon, MON_DATA_STATUS);

    RoamerMoveToOtherLocationSet();
}

void SetRoamerInactive(void)
{
    gSaveBlock1Ptr->roam[gSpecialVar_0x8003].active = FALSE;
}

void GetRoamerLocation(u8 id, u8 *mapGroup, u8 *mapNum)
{
    *mapGroup = sRoamerLocation[id][MAP_GRP];
    *mapNum = sRoamerLocation[id][MAP_NUM];
}
