AlteringCave_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, AlteringCave_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, AlteringCave_OnResume
	.byte 0

AlteringCave_OnTransition:
	setflag FLAG_LANDMARK_ALTERING_CAVE
	setflag FLAG_HIDE_ARTICUNO
	setflag FLAG_HIDE_ZAPDOS
	setflag FLAG_HIDE_MOLTRES
	call_if_unset FLAG_DAILY_ITEM_ALTERING_CAVE, AlteringCave_EventScript_ResetItemBall
	copyvar VAR_ALTERING_CAVE_WILD_SET, VAR_BIRCH_STATE
	copyvar VAR_0x8000, VAR_ALTERING_CAVE_WILD_SET
	addvar VAR_0x8000, LAYOUT_ALTERING_CAVE
	setmaplayoutindex VAR_0x8000
	goto_if_eq VAR_OLD_SEA_MAP_STATE, 6, AlteringCave_EventScript_SetupLegendaryBirds
	end

AlteringCave_EventScript_ResetItemBall::
	clearflag FLAG_ITEM_ALTERING_CAVE_VITAMIN_BALL
	return

AlteringCave_EventScript_SetupLegendaryBirds::
	goto_if_eq VAR_ALTERING_CAVE_WILD_SET, 1, AlteringCave_EventScript_SetupMoltres
	goto_if_eq VAR_ALTERING_CAVE_WILD_SET, 3, AlteringCave_EventScript_SetupZapdos
	goto_if_eq VAR_ALTERING_CAVE_WILD_SET, 5, AlteringCave_EventScript_SetupArticuno
	end

AlteringCave_EventScript_SetupMoltres::
	call_if_unset FLAG_CAUGHT_MOLTRES, AlteringCave_EventScript_TryShowMoltres
	end

AlteringCave_EventScript_TryShowMoltres::
	goto_if_set FLAG_DEFEATED_MOLTRES, Common_EventScript_NopReturn
	clearflag FLAG_HIDE_MOLTRES
	return

AlteringCave_EventScript_SetupZapdos::
	call_if_unset FLAG_CAUGHT_ZAPDOS, AlteringCave_EventScript_TryShowZapdos
	end

AlteringCave_EventScript_TryShowZapdos::
	goto_if_set FLAG_DEFEATED_ZAPDOS, Common_EventScript_NopReturn
	clearflag FLAG_HIDE_ZAPDOS
	return

AlteringCave_EventScript_SetupArticuno::
	call_if_unset FLAG_CAUGHT_ARTICUNO, AlteringCave_EventScript_TryShowArticuno
	end

AlteringCave_EventScript_TryShowArticuno::
	goto_if_set FLAG_DEFEATED_ARTICUNO, Common_EventScript_NopReturn
	clearflag FLAG_HIDE_ARTICUNO
	return

AlteringCave_OnResume::
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, AlteringCave_EventScript_TryRemoveLegendaryBird
	end

AlteringCave_EventScript_TryRemoveLegendaryBird::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject VAR_LAST_TALKED
	return

AlteringCave_EventScript_Zapdos::
	setvar VAR_TEMP_1, SPECIES_ZAPDOS
	call AlteringCave_EventScript_LegendaryBird
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, AlteringCave_EventScript_DefeatedZapdos
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, AlteringCave_EventScript_RanFromZapdos
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, AlteringCave_EventScript_RanFromZapdos
	setflag FLAG_CAUGHT_ZAPDOS
	release
	end

AlteringCave_EventScript_Moltres::
	setvar VAR_TEMP_1, SPECIES_MOLTRES
	call AlteringCave_EventScript_LegendaryBird
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, AlteringCave_EventScript_DefeatedMoltres
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, AlteringCave_EventScript_RanFromMoltres
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, AlteringCave_EventScript_RanFromMoltres
	setflag FLAG_CAUGHT_MOLTRES
	release
	end

AlteringCave_EventScript_Articuno::
	setvar VAR_TEMP_1, SPECIES_ARTICUNO
	call AlteringCave_EventScript_LegendaryBird
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, AlteringCave_EventScript_DefeatedArticuno
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, AlteringCave_EventScript_RanFromArticuno
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, AlteringCave_EventScript_RanFromArticuno
	setflag FLAG_CAUGHT_ARTICUNO
	release
	end

AlteringCave_EventScript_LegendaryBird::
	lock
	faceplayer
	waitse
	delay 20
	playse SE_THUNDERSTORM_STOP
	setvar VAR_0x8004, 0  @ vertical pan
	setvar VAR_0x8005, 3  @ horizontal pan
	setvar VAR_0x8006, 4  @ num shakes
	setvar VAR_0x8007, 2  @ shake delay
	special ShakeCamera
	delay 30
	playse SE_THUNDERSTORM_STOP
	setvar VAR_0x8004, 0  @ vertical pan
	setvar VAR_0x8005, 3  @ horizontal pan
	setvar VAR_0x8006, 4  @ num shakes
	setvar VAR_0x8007, 2  @ shake delay
	special ShakeCamera
	delay 30
	delay 50
	message AlteringCave_Text_LegendaryBird
	waitse
	playmoncry VAR_TEMP_1, CRY_MODE_ENCOUNTER
	waitmoncry
	delay 20
	closemessage
	copyvar VAR_0x8004, VAR_TEMP_1
	setvar VAR_0x8005, 60
	setvar VAR_0x8006, 0
	special CreateEventLegalEnemyMon
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	special BattleSetup_StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	return

AlteringCave_EventScript_DefeatedZapdos::
	setflag FLAG_DEFEATED_ZAPDOS
AlteringCave_EventScript_RanFromZapdos::
	setvar VAR_0x8004, SPECIES_ZAPDOS
	goto Common_EventScript_LegendaryFlewAway

AlteringCave_EventScript_DefeatedMoltres::
	setflag FLAG_DEFEATED_MOLTRES
AlteringCave_EventScript_RanFromMoltres::
	setvar VAR_0x8004, SPECIES_MOLTRES
	goto Common_EventScript_LegendaryFlewAway

AlteringCave_EventScript_DefeatedArticuno::
	setflag FLAG_DEFEATED_ARTICUNO
AlteringCave_EventScript_RanFromArticuno::
	setvar VAR_0x8004, SPECIES_ARTICUNO
	goto Common_EventScript_LegendaryFlewAway

AlteringCave_EventScript_DailyVitamin::
	setflag FLAG_DAILY_ITEM_ALTERING_CAVE
	switch VAR_ALTERING_CAVE_WILD_SET
	case 0, ItemBall_EventScript_ItemHPUp
	case 1, ItemBall_EventScript_ItemCalcium
	case 2, ItemBall_EventScript_ItemIron
	case 3, ItemBall_EventScript_ItemCarbos
	case 4, ItemBall_EventScript_ItemProtein
	case 5, ItemBall_EventScript_ItemPPUp
	case 6, ItemBall_EventScript_ItemZinc

AlteringCave_Text_LegendaryBird:
	.string "Gyaoo!$"
