AlteringCave_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, AlteringCave_OnTransition
	.byte 0

AlteringCave_OnTransition:
	setflag FLAG_LANDMARK_ALTERING_CAVE
	call_if_unset FLAG_DAILY_ITEM_ALTERING_CAVE, AlteringCave_EventScript_ResetItemBall
	copyvar VAR_ALTERING_CAVE_WILD_SET, VAR_BIRCH_STATE
	copyvar VAR_0x8000, VAR_ALTERING_CAVE_WILD_SET
	addvar VAR_0x8000, LAYOUT_ALTERING_CAVE
	setmaplayoutindex VAR_0x8000
	end

AlteringCave_EventScript_ResetItemBall::
	clearflag FLAG_ITEM_ALTERING_CAVE_VITAMIN_BALL
	return

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
