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
	case 0, Route111_EventScript_ItemHPUp
	case 1, Route123_EventScript_ItemCalcium
	case 2, Route105_EventScript_ItemIron
	case 3, Route121_EventScript_ItemCarbos
	case 4, Route106_EventScript_ItemProtein
	case 5, Route103_EventScript_ItemPPUp
	case 6, Route119_EventScript_ItemZinc
