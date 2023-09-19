#ifndef GUARD_SAVE_VERSIONS_H
#define GUARD_SAVE_VERSIONS_H

#include "global.h"

struct BattleFrontierOld
{
    /*0x64C*/ struct EmeraldBattleTowerRecord towerPlayer;
    /*0x738*/ struct EmeraldBattleTowerRecord towerRecords[BATTLE_TOWER_RECORD_COUNT]; // From record mixing.
    /*0xBEB*/ struct BattleTowerInterview towerInterview;
    /*0xBEC*/ struct BattleTowerEReaderTrainer ereaderTrainer;
    /*0xCA8*/ u8 challengeStatus;
    /*0xCA9*/ u8 lvlMode:2;
    /*0xCA9*/ u8 challengePaused:1;
    /*0xCA9*/ u8 disableRecordBattle:1;
    /*0xCAA*/ u16 selectedPartyMons[MAX_FRONTIER_PARTY_SIZE];
    /*0xCB2*/ u16 curChallengeBattleNum; // Battle number / room number (Pike) / floor number (Pyramid)
    /*0xCB4*/ u16 trainerIds[20];
    /*0xCDC*/ u32 winStreakActiveFlags;
    /*0xCE0*/ u16 towerWinStreaks[4][FRONTIER_LVL_MODE_COUNT];
    /*0xCF0*/ u16 towerRecordWinStreaks[4][FRONTIER_LVL_MODE_COUNT];
    /*0xD00*/ u16 battledBrainFlags;
    /*0xD02*/ u16 towerSinglesStreak; // Never read
    /*0xD04*/ u16 towerNumWins; // Increments to MAX_STREAK but never read otherwise
    /*0xD06*/ u8 towerBattleOutcome;
    /*0xD07*/ u8 towerLvlMode;
    /*0xD08*/ u8 domeAttemptedSingles50:1;
    /*0xD08*/ u8 domeAttemptedSinglesOpen:1;
    /*0xD08*/ u8 domeHasWonSingles50:1;
    /*0xD08*/ u8 domeHasWonSinglesOpen:1;
    /*0xD08*/ u8 domeAttemptedDoubles50:1;
    /*0xD08*/ u8 domeAttemptedDoublesOpen:1;
    /*0xD08*/ u8 domeHasWonDoubles50:1;
    /*0xD08*/ u8 domeHasWonDoublesOpen:1;
    /*0xD09*/ u8 domeLastOutfit;
    /*0xD0A*/ u8 domeLvlMode;
    /*0xD0B*/ u8 domeBattleMode;
    /*0xD0C*/ u16 domeWinStreaks[2][FRONTIER_LVL_MODE_COUNT];
    /*0xD14*/ u16 domeRecordWinStreaks[2][FRONTIER_LVL_MODE_COUNT];
    /*0xD1C*/ u16 domeTotalChampionships[2][FRONTIER_LVL_MODE_COUNT];
    /*0xD24*/ struct BattleDomeTrainer domeTrainers[DOME_TOURNAMENT_TRAINERS_COUNT];
    /*0xD64*/ u16 domeMonIds[DOME_TOURNAMENT_TRAINERS_COUNT][FRONTIER_PARTY_SIZE];
    /*0xDC4*/ u16 unused_DC4;
    /*0xDC6*/ u16 palacePrize;
    /*0xDC8*/ u16 palaceWinStreaks[2][FRONTIER_LVL_MODE_COUNT];
    /*0xDD0*/ u16 palaceRecordWinStreaks[2][FRONTIER_LVL_MODE_COUNT];
    /*0xDD8*/ u16 arenaPrize;
    /*0xDDA*/ u16 arenaWinStreaks[FRONTIER_LVL_MODE_COUNT];
    /*0xDDE*/ u16 arenaRecordStreaks[FRONTIER_LVL_MODE_COUNT];
    /*0xDE2*/ u16 factoryWinStreaks[2][FRONTIER_LVL_MODE_COUNT];
    /*0xDEA*/ u16 factoryRecordWinStreaks[2][FRONTIER_LVL_MODE_COUNT];
    /*0xDF6*/ u16 factoryRentsCount[2][FRONTIER_LVL_MODE_COUNT];
    /*0xDFA*/ u16 factoryRecordRentsCount[2][FRONTIER_LVL_MODE_COUNT];
    /*0xE02*/ u16 pikePrize;
    /*0xE04*/ u16 pikeWinStreaks[FRONTIER_LVL_MODE_COUNT];
    /*0xE08*/ u16 pikeRecordStreaks[FRONTIER_LVL_MODE_COUNT];
    /*0xE0C*/ u16 pikeTotalStreaks[FRONTIER_LVL_MODE_COUNT];
    /*0xE10*/ u8 pikeHintedRoomIndex:3;
    /*0xE10*/ u8 pikeHintedRoomType:4;
    /*0xE10*/ u8 pikeHealingRoomsDisabled:1;
    /*0xE12*/ u16 pikeHeldItemsBackup[FRONTIER_PARTY_SIZE];
    /*0xE18*/ u16 pyramidPrize;
    /*0xE1A*/ u16 pyramidWinStreaks[FRONTIER_LVL_MODE_COUNT];
    /*0xE1E*/ u16 pyramidRecordStreaks[FRONTIER_LVL_MODE_COUNT];
    /*0xE22*/ u16 pyramidRandoms[4];
    /*0xE2A*/ u8 pyramidTrainerFlags; // 1 bit for each trainer (MAX_PYRAMID_TRAINERS)
    /*0xE2C*/ struct PyramidBag pyramidBag;
    /*0xE68*/ u8 pyramidLightRadius;
    /*0xE6A*/ u16 verdanturfTentPrize;
    /*0xE6C*/ u16 fallarborTentPrize;
    /*0xE6E*/ u16 slateportTentPrize;
    /*0xE70*/ struct RentalMon rentalMons[FRONTIER_PARTY_SIZE * 2];
    /*0xEB8*/ u16 battlePoints;
    /*0xEBA*/ u16 cardBattlePoints;
    /*0xEBC*/ u32 battlesCount;
    /*0xEC0*/ u16 domeWinningMoves[DOME_TOURNAMENT_TRAINERS_COUNT];
    /*0xEE0*/ u8 trainerFlags;
    /*0xEE1*/ u8 opponentNames[FRONTIER_LVL_MODE_COUNT][PLAYER_NAME_LENGTH + 1];
    /*0xEF1*/ u8 opponentTrainerIds[FRONTIER_LVL_MODE_COUNT][TRAINER_ID_LENGTH];
    /*0xEF9*/ u8 unk_EF9:7; // Never read
    /*0xEF9*/ u8 savedGame:1;
    /*0xEFA*/ u8 unused_EFA;
    /*0xEFB*/ u8 unused_EFB;
    /*0xEFC*/ struct DomeMonData domePlayerPartyData[FRONTIER_PARTY_SIZE];
};

struct SaveBlock2Old
{
    /*0x00*/ u8 playerName[PLAYER_NAME_LENGTH + 1];
    /*0x08*/ u8 playerGender; // MALE, FEMALE
    /*0x09*/ u8 specialSaveWarpFlags;
    /*0x0A*/ u8 playerTrainerId[TRAINER_ID_LENGTH];
    /*0x0E*/ u16 playTimeHours;
    /*0x10*/ u8 playTimeMinutes;
    /*0x11*/ u8 playTimeSeconds;
    /*0x12*/ u8 playTimeVBlanks;
    /*0x13*/ u8 optionsButtonMode;  // OPTIONS_BUTTON_MODE_[NORMAL/LR/L_EQUALS_A]
    /*0x14*/ u16 optionsTextSpeed:3; // OPTIONS_TEXT_SPEED_[SLOW/MID/FAST]
             u16 optionsWindowFrameType:5; // Specifies one of the 20 decorative borders for text boxes
             u16 optionsSound:1; // OPTIONS_SOUND_[MONO/STEREO]
             u16 optionsDifficulty:2; // OPTIONS_DIFFICULTY_[EASY/NORMAL/HARD], formerly OPTIONS_BATTLE_STYLE_[SHIFT/SET]
             u16 optionsBattleSceneOff:1; // whether battle animations are disabled
             u16 regionMapZoom:1; // whether the map is zoomed in
             u16 optionsAttackStyle:1; // OPTIONS_ATTACK_STYLE_[TYPE/CATEGORY]
             u16 optionsDisableMatchCall:1; // OPTIONS_MATCH_CALL [ON/OFF]
             u16 optionsUnitSystem:1; // OPTIONS_UNIT_SYSTEM [IMPERIAL/METRIC]
             u16 optionsCurrentFont:3; // Specifies one of multiple fonts to use (Emerald, FR/LG, R/S Europe, R/S U.S., HG/SS)
    /*0x18*/ struct Pokedex pokedex;
    /*0x90*/ u8 outfitId:7;
    /*0x90*/ u8 autoRun:1;
    /*0x91*/ u8 filler_91[0x7];
    /*0x98*/ struct Time localTimeOffset;
    /*0xA0*/ struct Time lastBerryTreeUpdate;
    /*0xA8*/ u32 gcnLinkFlags; // Read by Pokemon Colosseum/XD
    /*0xAC*/ u32 encryptionKey;
    /*0xB0*/ struct PlayersApprentice playerApprentice;
    /*0xDC*/ struct Apprentice apprentices[APPRENTICE_COUNT];
    /*0x1EC*/ struct BerryCrush berryCrush;
    /*0x1FC*/ struct PokemonJumpRecords pokeJump;
    /*0x20C*/ struct BerryPickingResults berryPick;
    /*0x21C*/ struct RankingHall1P hallRecords1P[HALL_FACILITIES_COUNT][FRONTIER_LVL_MODE_COUNT][HALL_RECORDS_COUNT]; // From record mixing.
    /*0x57C*/ struct RankingHall2P hallRecords2P[FRONTIER_LVL_MODE_COUNT][HALL_RECORDS_COUNT]; // From record mixing.
    /*0x624*/ u16 contestLinkResults[CONTEST_CATEGORIES_COUNT][CONTESTANT_COUNT];
    /*0x64C*/ struct BattleFrontierOld frontier;
}; // sizeof=0xF2C

struct NewRoamer
{
    /*0x00*/ u32 ivs;
    /*0x04*/ u32 personality;
    /*0x08*/ u16 species;
    /*0x0A*/ u16 hp;
    /*0x0C*/ u8 level;
    /*0x0D*/ u8 status;
    /*0x0E*/ bool8 active;
    /*0x0F*/ u8 filler;
};

struct SaveBlock1Old
{
    /*0x00*/ struct Coords16 pos;
    /*0x04*/ struct WarpData location;
    /*0x0C*/ struct WarpData continueGameWarp;
    /*0x14*/ struct WarpData dynamicWarp;
    /*0x1C*/ struct WarpData lastHealLocation; // used by white-out and teleport
    /*0x24*/ struct WarpData escapeWarp; // used by Dig and Escape Rope
    /*0x2C*/ u16 savedMusic;
    /*0x2E*/ u8 weather;
    /*0x2F*/ u8 weatherCycleStage;
    /*0x30*/ u8 flashLevel;
    /*0x31*/ //u8 padding1;
    /*0x32*/ u16 mapLayoutId;
    /*0x34*/ u16 mapView[0x100];
    /*0x234*/ u8 playerPartyCount;
    /*0x235*/ //u8 padding2[3];
    /*0x238*/ struct Pokemon playerParty[PARTY_SIZE];
    /*0x490*/ u32 money;
    /*0x494*/ u16 coins;
    /*0x496*/ u16 registeredItem; // registered for use with SELECT button
    /*0x498*/ struct ItemSlot pcItems[PC_ITEMS_COUNT];
    /*0x560*/ struct ItemSlot bagPocket_Medicine[BAG_MEDICINE_COUNT];
    /*0x5D8*/ struct ItemSlot bagPocket_KeyItems[BAG_KEYITEMS_COUNT];
    /*0x650*/ struct ItemSlot bagPocket_PokeBalls[BAG_POKEBALLS_COUNT];
    /*0x690*/ struct ItemSlot bagPocket_TMHM[BAG_TMHM_COUNT];
    /*0x790*/ struct ItemSlot bagPocket_Berries[BAG_BERRIES_COUNT];
    /*0x848*/ struct Pokeblock pokeblocks[POKEBLOCKS_COUNT];
    /*0x988*/ struct ItemSlot bagPocket_BattleItems[BAG_BATTLEITEMS_COUNT];
    /*0x9B4*/ u8 unused_9B4[8];
    /*0x9BC*/ u16 berryBlenderRecords[3];
    /*0x9C2*/ u8 registeredItemLastSelected:4; //max 16 items
              u8 registeredItemListCount:4;
    /*0x9C3*/ u8 unused_9C3[5];
    /*0x9C8*/ u16 trainerRematchStepCounter;
    /*0x9CA*/ u8 trainerRematches[MAX_REMATCH_ENTRIES];
    /*0xA2E*/ //u8 padding3[2];
    /*0xA30*/ struct ObjectEvent objectEvents[OBJECT_EVENTS_COUNT];
    /*0xC70*/ struct ObjectEventTemplate objectEventTemplates[OBJECT_EVENT_TEMPLATES_COUNT];
    /*0x1270*/ u8 flags[NUM_FLAG_BYTES];
    /*0x139C*/ u16 vars[VARS_COUNT];
    /*0x159C*/ u32 gameStats[NUM_GAME_STATS];
    /*0x169C*/ struct BerryTree berryTrees[BERRY_TREES_COUNT];
    /*0x1A9C*/ struct SecretBase secretBases[SECRET_BASES_COUNT];
    /*0x271C*/ u8 playerRoomDecorations[DECOR_MAX_PLAYERS_HOUSE];
    /*0x2728*/ u8 playerRoomDecorationPositions[DECOR_MAX_PLAYERS_HOUSE];
    /*0x2734*/ u8 decorationDesks[10];
    /*0x273E*/ u8 decorationChairs[10];
    /*0x2748*/ u8 decorationPlants[10];
    /*0x2752*/ u8 decorationOrnaments[30];
    /*0x2770*/ u8 decorationMats[30];
    /*0x278E*/ u8 decorationPosters[10];
    /*0x2798*/ u8 decorationDolls[40];
    /*0x27C0*/ u8 decorationCushions[10];
    /*0x27CA*/ //u8 padding4[2];
    /*0x27CC*/ TVShow tvShows[TV_SHOWS_COUNT];
    /*0x2B50*/ PokeNews pokeNews[POKE_NEWS_COUNT];
    /*0x2B90*/ u16 outbreakPokemonSpecies;
    /*0x2B92*/ u8 outbreakLocationMapNum;
    /*0x2B93*/ u8 outbreakLocationMapGroup;
    /*0x2B94*/ u8 outbreakPokemonLevel;
    /*0x2B95*/ u8 outbreakUnused1;
    /*0x2B96*/ u16 outbreakUnused2;
    /*0x2B98*/ u16 outbreakPokemonMoves[MAX_MON_MOVES];
    /*0x2BA0*/ u8 outbreakUnused3;
    /*0x2BA1*/ u8 outbreakPokemonProbability;
    /*0x2BA2*/ u16 outbreakDaysLeft;
    /*0x2BA4*/ struct GabbyAndTyData gabbyAndTyData;
    /*0x2BB0*/ u16 easyChatProfile[EASY_CHAT_BATTLE_WORDS_COUNT];
    /*0x2BBC*/ u16 easyChatBattleStart[EASY_CHAT_BATTLE_WORDS_COUNT];
    /*0x2BC8*/ u16 easyChatBattleWon[EASY_CHAT_BATTLE_WORDS_COUNT];
    /*0x2BD4*/ u16 easyChatBattleLost[EASY_CHAT_BATTLE_WORDS_COUNT];
    /*0x2BE0*/ struct Mail mail[MAIL_COUNT];
    /*0x2E20*/ u8 unlockedTrendySayings[NUM_TRENDY_SAYING_BYTES]; // Bitfield for unlockable Easy Chat words in EC_GROUP_TRENDY_SAYING
    /*0x2E25*/ //u8 padding5[3];
    /*0x2E28*/ OldMan oldMan;
    /*0x2e64*/ struct DewfordTrend dewfordTrends[SAVED_TRENDS_COUNT];
    /*0x2e90*/ struct ContestWinner contestWinners[NUM_CONTEST_WINNERS]; // see CONTEST_WINNER_*
    /*0x3030*/ struct DayCare daycare;
    /*0x3150*/ struct LinkBattleRecords linkBattleRecords;
    /*0x31A8*/ u8 giftRibbons[GIFT_RIBBONS_COUNT];
    /*0x31B3*/ struct ExternalEventData externalEventData;
    /*0x31C7*/ struct ExternalEventFlags externalEventFlags;
    /*0x31DC*/ struct Roamer roamer;
    /*0x31F8*/ struct EnigmaBerry enigmaBerry;
    /*0x322C*/ struct MysteryGiftSave mysteryGift;
    /*0x3598*/ struct NewRoamer roam[TOTAL_ROAMING_POKEMON];;
    /*0x35D8*/ u16 registeredItems[REGISTERED_ITEMS_LIST_COUNT];
    /*0x35EC*/ struct ItemSlot bagPocket_Items[BAG_ITEMS_COUNT];
    /*0x3718*/ u32 trainerHillTimes[NUM_TRAINER_HILL_MODES];
    /*0x3728*/ struct RamScript ramScript;
    /*0x3B14*/ struct RecordMixingGift recordMixingGift;
    /*0x3B24*/ struct ItemSlot bagPocket_Mail[BAG_MAIL_COUNT];
    /*0x3B54*/ struct ContestLady contestLady;
    /*0x3B58*/ struct LilycoveLady lilycoveLady;
    /*0x3B98*/ struct TrainerNameRecord trainerNameRecords[20];
    /*0x3C88*/ u8 registeredTexts[UNION_ROOM_KB_ROW_COUNT][21];
    /*0x3D5A*/ u8 unused_3D5A[10];
    /*0x3D64*/ struct TrainerHillSave trainerHill;
    /*0x3D70*/ struct WaldaPhrase waldaPhrase;
    /*0x3D88*/ struct ItemSlot bagPocket_Treasures[BAG_TREASURES_COUNT];
    // sizeof: 0x3DD4
};

#endif // GUARD_GLOBAL_H
