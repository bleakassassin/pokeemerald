@ Interact with cuttable tree
EventScript_CutTree:: @ 82906BB
	lockall
	goto_if_unset FLAG_BADGE01_GET, EventScript_CheckTreeCantCut
	checkpartymove MOVE_CUT
	compare VAR_RESULT, PARTY_SIZE
	goto_if_eq EventScript_CheckTreeCantCut
	setfieldeffectargument 0, VAR_RESULT
	bufferpartymonnick 0, VAR_RESULT
	buffermovename 1, MOVE_CUT
	msgbox Text_WantToCut, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq EventScript_CancelCut
	msgbox Text_MonUsedFieldMove, MSGBOX_DEFAULT
	closemessage
	dofieldeffect FLDEFF_USE_CUT_ON_TREE
	waitstate
	goto EventScript_CutTreeDown
	end

@ Use cut from party menu
EventScript_UseCut:: @ 8290705
	lockall
	dofieldeffect FLDEFF_USE_CUT_ON_TREE
	waitstate
	goto EventScript_CutTreeDown
	end

EventScript_CutTreeDown:: @ 8290710
	applymovement VAR_LAST_TALKED, Movement_CutTreeDown
	waitmovement 0
	removeobject VAR_LAST_TALKED
	releaseall
	end

Movement_CutTreeDown: @ 829071F
	cut_tree
	step_end

EventScript_CheckTreeCantCut:: @ 8290721
	msgbox Text_CantCut, MSGBOX_DEFAULT
	releaseall
	end

EventScript_CancelCut:: @ 829072B
	closemessage
	releaseall
	end

Text_WantToCut: @ 829072E
	.string "This tree looks like it can be\n"
	.string "CUT down!\p"
	.string "Would you like to CUT it?$"

Text_MonUsedFieldMove: @ 8290771
	.string "{STR_VAR_1} used {STR_VAR_2}!$"

Text_CantCut: @ 829077D
	.string "This tree looks like it can be\n"
	.string "CUT down!$"

@ Interact with smashable rock
EventScript_RockSmash::
	call EventScript_RockSmash_Main
	call EventScript_RockItemTable
	call EventScript_RockItemTable_Main
	goto EventScript_ItemUnderRubble

EventScript_RockSmash_SeafloorCavern::
	call EventScript_RockSmash_Main
	call EventScript_RockItemTable
	call EventScript_RockItemTable_SeafloorCavern
	goto EventScript_ItemUnderRubble

EventScript_RockSmash_GraniteCave::
	call EventScript_RockSmash_Main
	call EventScript_RockItemTable
	call EventScript_RockItemTable_GraniteCave
	goto EventScript_ItemUnderRubble

EventScript_RockSmash_SafariZone::
	call EventScript_RockSmash_Main
	call EventScript_RockItemTable
	call EventScript_RockItemTable_SafariZone
	goto EventScript_ItemUnderRubble
	
EventScript_RockSmash_NoItem::
	call EventScript_RockSmash_Main
	goto EventScript_EndSmash
	
EventScript_RockSmash_Main::
	lockall
	goto_if_unset FLAG_BADGE03_GET, EventScript_CantSmashRock
	checkpartymove MOVE_ROCK_SMASH
	compare VAR_RESULT, PARTY_SIZE
	goto_if_eq EventScript_CantSmashRock
	setfieldeffectargument 0, VAR_RESULT
	bufferpartymonnick 0, VAR_RESULT
	buffermovename 1, MOVE_ROCK_SMASH
	msgbox Text_WantToSmash, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq EventScript_CancelSmash
	msgbox Text_MonUsedFieldMove, MSGBOX_DEFAULT
	closemessage
	dofieldeffect FLDEFF_USE_ROCK_SMASH
	waitstate
	goto EventScript_SmashRock
	end

@ Use rock smash from party menu
EventScript_UseRockSmash:: @ 82907F0
	lockall
	dofieldeffect FLDEFF_USE_ROCK_SMASH
	waitstate

EventScript_SmashRock::
	applymovement VAR_LAST_TALKED, Movement_SmashRock
	waitmovement 0
	removeobject VAR_LAST_TALKED
	specialvar VAR_RESULT, TryUpdateRusturfTunnelState
	compare VAR_RESULT, TRUE
	goto_if_eq EventScript_EndSmash
	special RockSmashWildEncounter
	compare VAR_RESULT, TRUE
	goto_if_eq EventScript_EndSmash_WildEncounter
	return
	
EventScript_EndSmash_WildEncounter::
	waitstate
	releaseall
	end
	
EventScript_RockItemTable::
	random 24
	compare VAR_RESULT, 0
	call_if_eq EventScript_RockItem1
	compare VAR_RESULT, 1
	call_if_eq EventScript_RockItem2
	compare VAR_RESULT, 2
	call_if_eq EventScript_RockItem3
	compare VAR_RESULT, 3
	call_if_eq EventScript_RockItem4
	compare VAR_RESULT, 4
	call_if_eq EventScript_RockItem5
	compare VAR_RESULT, 5
	call_if_eq EventScript_RockItem6
	return

EventScript_RockItemTable_Main::
	compare VAR_RESULT, 6
	call_if_eq EventScript_RockItem7
	compare VAR_RESULT, 7
	call_if_eq EventScript_RockItem8
	compare VAR_RESULT, 8
	call_if_eq EventScript_RockItem9
	compare VAR_RESULT, 9
	call_if_eq EventScript_RockItem10
	return

EventScript_RockItemTable_SeafloorCavern::
	compare VAR_RESULT, 6
	call_if_eq EventScript_RockItem7_SeafloorCavern
	compare VAR_RESULT, 7
	call_if_eq EventScript_RockItem8_SeafloorCavern
	compare VAR_RESULT, 8
	call_if_eq EventScript_RockItem9_SeafloorCavern
	compare VAR_RESULT, 9
	call_if_eq EventScript_RockItem10_SeafloorCavern
	return

EventScript_RockItemTable_GraniteCave::
	compare VAR_RESULT, 6
	call_if_eq EventScript_RockItem7
	compare VAR_RESULT, 7
	call_if_eq EventScript_RockItem8
	compare VAR_RESULT, 8
	call_if_eq EventScript_RockItem9_GraniteCave
	compare VAR_RESULT, 9
	call_if_eq EventScript_RockItem10_GraniteCave
	return

EventScript_RockItemTable_SafariZone::
	compare VAR_RESULT, 6
	call_if_eq EventScript_RockItem7
	compare VAR_RESULT, 7
	call_if_eq EventScript_RockItem8_SafariZone
	compare VAR_RESULT, 8
	call_if_eq EventScript_RockItem9_SafariZone
	compare VAR_RESULT, 9
	call_if_eq EventScript_RockItem10_SafariZone
	return
	
EventScript_ItemUnderRubble::	
	compare VAR_RESULT, 10
	goto_if_ge EventScript_EndSmash
	giveitem VAR_TEMP_0
	compare VAR_RESULT, FALSE
	goto_if_eq Common_EventScript_ShowBagIsFull

EventScript_EndSmash::
	releaseall
	end

EventScript_RockItem1::
	setvar VAR_TEMP_0, ITEM_REVIVE
	return

EventScript_RockItem2::
	setvar VAR_TEMP_0, ITEM_MAX_REVIVE
	return

EventScript_RockItem3::
	setvar VAR_TEMP_0, ITEM_ETHER
	return

EventScript_RockItem4::
	setvar VAR_TEMP_0, ITEM_MAX_ETHER
	return

EventScript_RockItem5::
	setvar VAR_TEMP_0, ITEM_SOFT_SAND
	return

EventScript_RockItem6::
	setvar VAR_TEMP_0, ITEM_HARD_STONE
	return

EventScript_RockItem7::
	setvar VAR_TEMP_0, ITEM_HEART_SCALE
	return

EventScript_RockItem8::
	setvar VAR_TEMP_0, ITEM_STAR_PIECE
	return

EventScript_RockItem9::
	setvar VAR_TEMP_0, ITEM_PEARL
	return

EventScript_RockItem10::
	setvar VAR_TEMP_0, ITEM_BIG_PEARL
	return

EventScript_RockItem7_SeafloorCavern::
	setvar VAR_TEMP_0, ITEM_RED_SHARD
	return

EventScript_RockItem8_SeafloorCavern::
	setvar VAR_TEMP_0, ITEM_BLUE_SHARD
	return

EventScript_RockItem9_SeafloorCavern::
	setvar VAR_TEMP_0, ITEM_YELLOW_SHARD
	return

EventScript_RockItem10_SeafloorCavern::
	setvar VAR_TEMP_0, ITEM_GREEN_SHARD
	return

EventScript_RockItem9_GraniteCave::
	goto_if_unset FLAG_HIDE_DESERT_UNDERPASS_FOSSIL, EventScript_EndSmash
	checkitem ITEM_ROOT_FOSSIL, 1
	goto_if_ge EventScript_EndSmash
	setvar VAR_TEMP_0, ITEM_ROOT_FOSSIL
	return

EventScript_RockItem10_GraniteCave::
	goto_if_unset FLAG_HIDE_DESERT_UNDERPASS_FOSSIL, EventScript_EndSmash
	checkitem ITEM_CLAW_FOSSIL, 1
	goto_if_ge EventScript_EndSmash
	setvar VAR_TEMP_0, ITEM_CLAW_FOSSIL
	return

EventScript_RockItem8_SafariZone::
	checkitem ITEM_HELIX_FOSSIL, 1
	goto_if_ge EventScript_EndSmash
	setvar VAR_TEMP_0, ITEM_HELIX_FOSSIL
	return

EventScript_RockItem9_SafariZone::
	checkitem ITEM_DOME_FOSSIL, 1
	goto_if_ge EventScript_EndSmash
	setvar VAR_TEMP_0, ITEM_DOME_FOSSIL
	return

EventScript_RockItem10_SafariZone::
	checkitem ITEM_OLD_AMBER, 1
	goto_if_ge EventScript_EndSmash
	setvar VAR_TEMP_0, ITEM_OLD_AMBER
	return

Movement_SmashRock: @ 829082B
	rock_smash_break
	step_end

EventScript_CantSmashRock:: @ 829082D
	msgbox Text_CantSmash, MSGBOX_DEFAULT
	releaseall
	end

EventScript_CancelSmash:: @ 8290837
	closemessage
	releaseall
	end

Text_WantToSmash: @ 829083A
	.string "This rock appears to be breakable.\n"
	.string "Would you like to use ROCK SMASH?$"

Text_CantSmash: @ 829087F
	.string "It's a rugged rock, but a POKéMON\n"
	.string "may be able to smash it.$"

EventScript_StrengthBoulder:: @ 82908BA
	lockall
	goto_if_unset FLAG_BADGE04_GET, EventScript_CantStrength
	goto_if_set FLAG_SYS_USE_STRENGTH, EventScript_CheckActivatedBoulder
	checkpartymove MOVE_STRENGTH
	compare VAR_RESULT, PARTY_SIZE
	goto_if_eq EventScript_CantStrength
	setfieldeffectargument 0, VAR_RESULT
	msgbox Text_WantToStrength, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq EventScript_CancelStrength
	closemessage
	dofieldeffect FLDEFF_USE_STRENGTH
	waitstate
	goto EventScript_ActivateStrength
	end

EventScript_UseStrength:: @ 82908FD
	lockall
	dofieldeffect FLDEFF_USE_STRENGTH
	waitstate
	goto EventScript_ActivateStrength
	end

EventScript_ActivateStrength:: @ 8290908
	setflag FLAG_SYS_USE_STRENGTH
	msgbox Text_MonUsedStrength, MSGBOX_DEFAULT
	releaseall
	end

EventScript_CantStrength:: @ 8290915
	msgbox Text_CantStrength, MSGBOX_DEFAULT
	releaseall
	end

EventScript_CheckActivatedBoulder:: @ 829091F
	msgbox Text_StrengthActivated, MSGBOX_DEFAULT
	releaseall
	end

EventScript_CancelStrength:: @ 8290929
	closemessage
	releaseall
	end

Text_WantToStrength: @ 829092C
	.string "It's a big boulder, but a POKéMON\n"
	.string "may be able to push it aside.\p"
	.string "Would you like to use STRENGTH?$"

Text_MonUsedStrength: @ 829098C
	.string "{STR_VAR_1} used STRENGTH!\p"
	.string "{STR_VAR_1}'s STRENGTH made it\n"
	.string "possible to move boulders around!$"

Text_CantStrength: @ 82909D6
	.string "It's a big boulder, but a POKéMON\n"
	.string "may be able to push it aside.$"

Text_StrengthActivated: @ 8290A16
	.string "STRENGTH made it possible to move\n"
	.string "boulders around.$"

EventScript_UseWaterfall:: @ 8290A49
	lockall
	checkpartymove MOVE_WATERFALL
	compare VAR_RESULT, PARTY_SIZE
	goto_if_eq EventScript_CantWaterfall
	bufferpartymonnick 0, VAR_RESULT
	setfieldeffectargument 0, VAR_RESULT
	msgbox Text_WantToWaterfall, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq EventScript_EndWaterfall
	msgbox Text_MonUsedWaterfall, MSGBOX_DEFAULT
	dofieldeffect FLDEFF_USE_WATERFALL
	goto EventScript_EndWaterfall

EventScript_CannotUseWaterfall:: @ 8290A83
	lockall

EventScript_CantWaterfall:: @ 8290A84
	msgbox Text_CantWaterfall, MSGBOX_DEFAULT

EventScript_EndWaterfall:: @ 8290A8C
	releaseall
	end

Text_CantWaterfall: @ 8290A8E
	.string "A wall of water is crashing down with\n"
	.string "a mighty roar.$"

Text_WantToWaterfall: @ 8290AC3
	.string "It's a large waterfall.\n"
	.string "Would you like to use WATERFALL?$"

Text_MonUsedWaterfall: @ 8290AFC
	.string "{STR_VAR_1} used WATERFALL.$"

EventScript_UseDive:: @ 8290B0F
	lockall
	checkpartymove MOVE_DIVE
	compare VAR_RESULT, PARTY_SIZE
	goto_if_eq EventScript_CantDive
	bufferpartymonnick 0, VAR_RESULT
	setfieldeffectargument 0, VAR_RESULT
	setfieldeffectargument 1, 1
	msgbox Text_WantToDive, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq EventScript_EndDive
	msgbox Text_MonUsedDive, MSGBOX_DEFAULT
	dofieldeffect FLDEFF_USE_DIVE
	goto EventScript_EndDive
	end

EventScript_CantDive:: @ 8290B4E
	msgbox Text_CantDive, MSGBOX_DEFAULT
	releaseall
	end

EventScript_EndDive:: @ 8290B58
	releaseall
	end

EventScript_UseDiveUnderwater:: @ 8290B5A
	lockall
	checkpartymove MOVE_DIVE
	compare VAR_RESULT, PARTY_SIZE
	goto_if_eq EventScript_CantSurface
	bufferpartymonnick 0, VAR_RESULT
	setfieldeffectargument 0, VAR_RESULT
	setfieldeffectargument 1, 1
	msgbox Text_WantToSurface, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq EventScript_EndSurface
	msgbox Text_MonUsedDive, MSGBOX_DEFAULT
	dofieldeffect FLDEFF_USE_DIVE
	goto EventScript_EndSurface
	end

EventScript_CantSurface:: @ 8290B99
	lockall
	msgbox Text_CantSurface, MSGBOX_DEFAULT
	goto EventScript_EndSurface
	end

EventScript_EndSurface:: @ 8290BA8
	releaseall
	end

Text_CantDive: @ 8290BAA
	.string "The sea is deep here. A POKéMON\n"
	.string "may be able to go underwater.$"

Text_WantToDive: @ 8290BE8
	.string "The sea is deep here.\n"
	.string "Would you like to use DIVE?$"

Text_MonUsedDive: @ 8290C1A
	.string "{STR_VAR_1} used DIVE.$"

Text_CantSurface: @ 8290C28
	.string "Light is filtering down from above.\n"
	.string "A POKéMON may be able to surface.$"

Text_WantToSurface: @ 8290C6E
	.string "Light is filtering down from above.\n"
	.string "Would you like to use DIVE?$"

EventScript_FailSweetScent:: @ 8290CAE
	msgbox Text_FailSweetScent, MSGBOX_SIGN
	end

Text_FailSweetScent: @ 8290CB7
	.string "Looks like there's nothing here…$"
