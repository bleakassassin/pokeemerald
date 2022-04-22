@ Interact with cuttable tree
EventScript_CutTree::
	lockall
	goto_if_unset FLAG_BADGE01_GET, EventScript_CheckTreeCantCut
	checkitem ITEM_HM01, 1
	goto_if_eq VAR_RESULT, FALSE, EventScript_CheckTreeCantCut
	goto_if_set FLAG_TEMP_CUT, EventScript_SkipTree
	msgbox Text_WantToCut, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_CancelCut
	checkpartymove MOVE_CUT
	goto_if_eq VAR_RESULT, PARTY_SIZE, EventScript_CutWildMon
	clearflag FLAG_SYS_WILD_FIELD_MOVE
	setfieldeffectargument 0, VAR_RESULT
	bufferpartymonnick STR_VAR_1, VAR_RESULT
	buffermovename STR_VAR_2, MOVE_CUT
	msgbox Text_MonUsedFieldMove, MSGBOX_DEFAULT
EventScript_InitCut::
	closemessage
	dofieldeffect FLDEFF_USE_CUT_ON_TREE
	waitstate
	setflag FLAG_TEMP_CUT
EventScript_CutTreeDown::
	applymovement VAR_LAST_TALKED, Movement_CutTreeDown
	waitmovement 0
	removeobject VAR_LAST_TALKED
	releaseall
	end

@ Use cut from party menu
EventScript_UseCut::
	lockall
	goto_if_set FLAG_TEMP_CUT, EventScript_SkipTree
	goto EventScript_InitCut

EventScript_CutWildMon::
	setflag FLAG_SYS_WILD_FIELD_MOVE
	setfieldeffectargument 0, SPECIES_ZIGZAGOON
	goto EventScript_InitCut

Movement_CutTreeDown:
	cut_tree
	step_end

EventScript_CheckTreeCantCut::
	msgbox Text_CantCut, MSGBOX_DEFAULT
	releaseall
	end

EventScript_CancelCut::
	closemessage
	releaseall
	end
	
EventScript_SkipTree::
	playse SE_M_CUT
	incrementgamestat GAME_STAT_USED_CUT
	goto EventScript_CutTreeDown

Text_WantToCut:
	.string "This tree looks like it can be\n"
	.string "Cut down!\p"
	.string "Would you like to Cut it?$"

Text_MonUsedFieldMove:
	.string "{STR_VAR_1} used {STR_VAR_2}!$"

Text_CantCut:
	.string "This tree looks like it can be\n"
	.string "Cut down!$"

@ Interact with smashable rock
EventScript_RockSmash::
	lockall
	goto_if_unset FLAG_BADGE03_GET, EventScript_CantSmashRock
	checkitem ITEM_HM06, 1
	goto_if_eq VAR_RESULT, FALSE, EventScript_CantSmashRock
	goto_if_set FLAG_TEMP_ROCK_SMASH, EventScript_SkipRock
	msgbox Text_WantToSmash, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_CancelSmash
	checkpartymove MOVE_ROCK_SMASH
	goto_if_eq VAR_RESULT, PARTY_SIZE, EventScript_RockSmashWildMon
	clearflag FLAG_SYS_WILD_FIELD_MOVE
	setfieldeffectargument 0, VAR_RESULT
	bufferpartymonnick STR_VAR_1, VAR_RESULT
	buffermovename STR_VAR_2, MOVE_ROCK_SMASH
	msgbox Text_MonUsedFieldMove, MSGBOX_DEFAULT
	closemessage
EventScript_InitRockSmash::
	dofieldeffect FLDEFF_USE_ROCK_SMASH
	waitstate
	setflag FLAG_TEMP_ROCK_SMASH
EventScript_SmashRock::
	applymovement VAR_LAST_TALKED, Movement_SmashRock
	waitmovement 0
	removeobject VAR_LAST_TALKED
	specialvar VAR_RESULT, TryUpdateRusturfTunnelState
	goto_if_eq VAR_RESULT, TRUE, EventScript_EndSmash
	special RockSmashWildEncounter
	goto_if_eq VAR_RESULT, FALSE, EventScript_EndSmash
	waitstate
	releaseall
	end

@ Use rock smash from party menu
EventScript_UseRockSmash::
	lockall
	goto_if_set FLAG_TEMP_ROCK_SMASH, EventScript_SkipRock
	goto EventScript_InitRockSmash

EventScript_RockSmashWildMon::
	closemessage
	setflag FLAG_SYS_WILD_FIELD_MOVE
	setfieldeffectargument 0, SPECIES_MAKUHITA
	goto EventScript_InitRockSmash

EventScript_EndSmash::
	specialvar VAR_RESULT RockSmashItem
	goto_if_gt VAR_RESULT, ITEM_NONE, EventScript_RockSmashItem
	releaseall
	end

Movement_SmashRock:
	rock_smash_break
	step_end

EventScript_CantSmashRock::
	msgbox Text_CantSmash, MSGBOX_DEFAULT
	releaseall
	end

EventScript_CancelSmash::
	closemessage
	releaseall
	end

EventScript_SkipRock::
	playse SE_M_ROCK_THROW
	incrementgamestat GAME_STAT_USED_ROCK_SMASH
	goto EventScript_SmashRock

EventScript_RockSmashItem::
	giveitem VAR_RESULT
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	releaseall
	end

Text_WantToSmash:
	.string "This rock appears to be breakable.\n"
	.string "Would you like to use Rock Smash?$"

Text_CantSmash:
	.string "It's a rugged rock, but a Pokémon\n"
	.string "may be able to smash it.$"

EventScript_StrengthBoulder::
	lockall
	goto_if_unset FLAG_BADGE04_GET, EventScript_CantStrength
	checkitem ITEM_HM04, 1
	goto_if_eq VAR_RESULT, FALSE, EventScript_CantStrength
	goto_if_set FLAG_SYS_USE_STRENGTH, EventScript_CheckActivatedBoulder
	msgbox Text_WantToStrength, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_CancelStrength
	closemessage
	checkpartymove MOVE_STRENGTH
	goto_if_eq VAR_RESULT, PARTY_SIZE, EventScript_StrengthWildMon
	clearflag FLAG_SYS_WILD_FIELD_MOVE
	setfieldeffectargument 0, VAR_RESULT
EventScript_ActivateStrength::
	dofieldeffect FLDEFF_USE_STRENGTH
	waitstate
	setflag FLAG_SYS_USE_STRENGTH
	msgbox Text_MonUsedStrength, MSGBOX_DEFAULT
	releaseall
	end

EventScript_UseStrength::
	lockall
	goto EventScript_ActivateStrength
	end

EventScript_StrengthWildMon::
	setflag FLAG_SYS_WILD_FIELD_MOVE
	setfieldeffectargument 0, SPECIES_MAKUHITA
	dofieldeffect FLDEFF_USE_STRENGTH
	waitstate
	setflag FLAG_SYS_USE_STRENGTH
EventScript_CheckActivatedBoulder::
	msgbox Text_StrengthActivated, MSGBOX_DEFAULT
	releaseall
	end

EventScript_CantStrength::
	msgbox Text_CantStrength, MSGBOX_DEFAULT
	releaseall
	end

EventScript_CancelStrength::
	closemessage
	releaseall
	end

Text_WantToStrength:
	.string "It's a big boulder, but a Pokémon\n"
	.string "may be able to push it aside.\p"
	.string "Would you like to use Strength?$"

Text_MonUsedStrength:
	.string "{STR_VAR_1} used Strength!\p"
	.string "{STR_VAR_1}'s Strength made it\n"
	.string "possible to move boulders around!$"

Text_CantStrength:
	.string "It's a big boulder, but a Pokémon\n"
	.string "may be able to push it aside.$"

Text_StrengthActivated:
	.string "Strength made it possible to move\n"
	.string "boulders around.$"

EventScript_UseWaterfall::
	lockall
	msgbox Text_WantToWaterfall, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_EndWaterfall
	checkpartymove MOVE_WATERFALL
	goto_if_eq VAR_RESULT, PARTY_SIZE, EventScript_WaterfallWildMon
	clearflag FLAG_SYS_WILD_FIELD_MOVE
	bufferpartymonnick STR_VAR_1, VAR_RESULT
	setfieldeffectargument 0, VAR_RESULT
	msgbox Text_MonUsedWaterfall, MSGBOX_DEFAULT
EventScript_InitWaterfall::
	closemessage
	dofieldeffect FLDEFF_USE_WATERFALL
EventScript_EndWaterfall::
	releaseall
	end

EventScript_CannotUseWaterfall::
	lockall
	msgbox Text_CantWaterfall, MSGBOX_DEFAULT
	releaseall
	end

EventScript_WaterfallWildMon::
	setflag FLAG_SYS_WILD_FIELD_MOVE
	setfieldeffectargument 0, SPECIES_WAILMER
	goto EventScript_InitWaterfall

Text_CantWaterfall:
	.string "A wall of water is crashing down with\n"
	.string "a mighty roar.$"

Text_WantToWaterfall:
	.string "It's a large waterfall.\n"
	.string "Would you like to use Waterfall?$"

Text_MonUsedWaterfall:
	.string "{STR_VAR_1} used Waterfall.$"

EventScript_UseDive::
	lockall
	checkitem ITEM_HM08, 1
	goto_if_eq VAR_RESULT, FALSE, EventScript_CantDive
	msgbox Text_WantToDive, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_EndDive
	checkpartymove MOVE_DIVE
	goto_if_eq VAR_RESULT, PARTY_SIZE, EventScript_DiveWildMon
	clearflag FLAG_SYS_WILD_FIELD_MOVE
	bufferpartymonnick STR_VAR_1, VAR_RESULT
	setfieldeffectargument 0, VAR_RESULT
	msgbox Text_MonUsedDive, MSGBOX_DEFAULT
EventScript_InitDive::	
	closemessage
	setfieldeffectargument 1, 1
	dofieldeffect FLDEFF_USE_DIVE
	waitstate
EventScript_EndDive::
	releaseall
	end

EventScript_CantDive::
	msgbox Text_CantDive, MSGBOX_DEFAULT
	releaseall
	end

EventScript_DiveWildMon::
	setflag FLAG_SYS_WILD_FIELD_MOVE
	setfieldeffectargument 0, SPECIES_WAILMER
	goto EventScript_InitDive

EventScript_UseDiveUnderwater::
	lockall
	msgbox Text_WantToSurface, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_EndSurface
	checkpartymove MOVE_DIVE
	goto_if_eq VAR_RESULT, PARTY_SIZE, EventScript_SurfaceWildMon
	clearflag FLAG_SYS_WILD_FIELD_MOVE
	bufferpartymonnick STR_VAR_1, VAR_RESULT
	setfieldeffectargument 0, VAR_RESULT
	msgbox Text_MonUsedDive, MSGBOX_DEFAULT
EventScript_InitSurface::	
	closemessage
	setfieldeffectargument 1, 1
	dofieldeffect FLDEFF_USE_DIVE
	waitstate
EventScript_EndSurface::
	releaseall
	end

EventScript_CantSurface::
	lockall
	msgbox Text_CantSurface, MSGBOX_DEFAULT
	goto EventScript_EndSurface
	end

EventScript_SurfaceWildMon::
	setflag FLAG_SYS_WILD_FIELD_MOVE
	setfieldeffectargument 0, SPECIES_WAILMER
	goto EventScript_InitSurface

Text_CantDive:
	.string "The sea is deep here. A Pokémon\n"
	.string "may be able to go underwater.$"

Text_WantToDive:
	.string "The sea is deep here.\n"
	.string "Would you like to use Dive?$"

Text_MonUsedDive:
	.string "{STR_VAR_1} used Dive.$"

Text_CantSurface:
	.string "Light is filtering down from above.\n"
	.string "A Pokémon may be able to surface.$"

Text_WantToSurface:
	.string "Light is filtering down from above.\n"
	.string "Would you like to use Dive?$"

EventScript_FailSweetScent::
	msgbox Text_FailSweetScent, MSGBOX_SIGN
	end

Text_FailSweetScent:
	.string "Looks like there's nothing here…$"

EventScript_ChangeAutoRun::
	release
	end
