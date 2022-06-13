.set LOCALID_WALLACE, 1
.set LOCALID_MEW, 2

CaveOfOrigin_B4F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CaveOfOrigin_B4F_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, CaveOfOrigin_B4F_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, CaveOfOrigin_B4F_OnFrame
	.byte 0

CaveOfOrigin_B4F_OnTransition:
	call_if_eq VAR_SOOTOPOLIS_CITY_STATE, 12, CaveOfOrigin_B4F_EventScript_SetupMewtwo
	call_if_eq VAR_SOOTOPOLIS_CITY_STATE, 8, CaveOfOrigin_B4F_EventScript_SetupLava
	call_if_eq VAR_SOOTOPOLIS_CITY_STATE, 9, CaveOfOrigin_B4F_EventScript_SetupWater
	call_if_eq VAR_SOOTOPOLIS_CITY_STATE, 10, CaveOfOrigin_B4F_EventScript_SetupBattle
	end

CaveOfOrigin_B4F_EventScript_SetupMewtwo::
	goto_if_set FLAG_CAUGHT_MEWTWO, Common_EventScript_NopReturn
	goto_if_set FLAG_DEFEATED_MEWTWO, Common_EventScript_NopReturn
	setvar VAR_SOOTOPOLIS_CITY_STATE, 7
	end

CaveOfOrigin_B4F_EventScript_SetupLava::
	setflag FLAG_HIDE_MAP_NAME_POPUP
	setmaplayoutindex LAYOUT_CAVE_OF_ORIGIN_B4F_LAVA
	setobjectxyperm LOCALID_MEW, 12, 9
	setobjectmovementtype LOCALID_MEW, MOVEMENT_TYPE_FACE_RIGHT
	return

CaveOfOrigin_B4F_EventScript_SetupWater::
	setmaplayoutindex LAYOUT_CAVE_OF_ORIGIN_B4F_WATER
	setobjectxyperm LOCALID_MEW, 6, 9
	setobjectmovementtype LOCALID_MEW, MOVEMENT_TYPE_FACE_LEFT
	return

CaveOfOrigin_B4F_EventScript_SetupBattle::
	setflashlevel 8
	setmaplayoutindex LAYOUT_CAVE_OF_ORIGIN_B4F
	setobjectxyperm LOCALID_MEW, 9, 9
	setobjectmovementtype LOCALID_MEW, MOVEMENT_TYPE_FACE_DOWN
	return

CaveOfOrigin_B4F_OnWarp:
	map_script_2 VAR_SOOTOPOLIS_CITY_STATE, 8, CaveOfOrigin_B4F_EventScript_SetupWarp
	map_script_2 VAR_SOOTOPOLIS_CITY_STATE, 9, CaveOfOrigin_B4F_EventScript_SetupWarp
	map_script_2 VAR_SOOTOPOLIS_CITY_STATE, 10, CaveOfOrigin_B4F_EventScript_SetupWarp
	map_script_2 VAR_SOOTOPOLIS_CITY_STATE, 11, CaveOfOrigin_B4F_EventScript_SetupWarpPostBattle
	.2byte 0

CaveOfOrigin_B4F_EventScript_SetupWarpPostBattle::
	setvar VAR_SOOTOPOLIS_CITY_STATE, 12
CaveOfOrigin_B4F_EventScript_SetupWarp::
	setobjectxy OBJ_EVENT_ID_PLAYER, 9, 13
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	end

CaveOfOrigin_B4F_OnFrame:
	map_script_2 VAR_SOOTOPOLIS_CITY_STATE, 8, CaveOfOrigin_B4F_EventScript_MewtwoLava
	map_script_2 VAR_SOOTOPOLIS_CITY_STATE, 9, CaveOfOrigin_B4F_EventScript_MewtwoWater
	map_script_2 VAR_SOOTOPOLIS_CITY_STATE, 10, CaveOfOrigin_B4F_EventScript_MewtwoBattle
	.2byte 0

CaveOfOrigin_B4F_EventScript_Wallace::
	lock
	faceplayer
	msgbox CaveOfOrigin_B4F_Text_WallaceStory, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_WALLACE, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	delay 60
	playse SE_PIN
	applymovement LOCALID_WALLACE, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_WALLACE, Common_Movement_Delay48
	waitmovement 0
	delay 30
	applymovement LOCALID_WALLACE, Common_Movement_FacePlayer
	waitmovement 0
	message CaveOfOrigin_B4F_Text_WhereIsRayquaza
	waitmessage
	goto CaveOfOrigin_B4F_EventScript_WheresRayquaza

CaveOfOrigin_B4F_EventScript_WheresRayquaza::
	multichoice 0, 0, MULTI_WHERES_RAYQUAZA, FALSE
	switch VAR_RESULT
	case 0, CaveOfOrigin_B4F_EventScript_AtCaveOfOrigin
	case 1, CaveOfOrigin_B4F_EventScript_AtMtPyre
	case 2, CaveOfOrigin_B4F_EventScript_AtSkyPillar
	case 3, CaveOfOrigin_B4F_EventScript_DontRemember
	goto CaveOfOrigin_B4F_EventScript_DontRemember
	end

CaveOfOrigin_B4F_EventScript_AtCaveOfOrigin::
	message CaveOfOrigin_B4F_Text_ButWereInCaveOfOrigin
	waitmessage
	goto CaveOfOrigin_B4F_EventScript_WheresRayquaza

CaveOfOrigin_B4F_EventScript_AtMtPyre::
	message CaveOfOrigin_B4F_Text_OldLadyDidntMentionThat
	waitmessage
	goto CaveOfOrigin_B4F_EventScript_WheresRayquaza

CaveOfOrigin_B4F_EventScript_DontRemember::
	message CaveOfOrigin_B4F_Text_CantYouRememberSomehow
	waitmessage
	goto CaveOfOrigin_B4F_EventScript_WheresRayquaza

CaveOfOrigin_B4F_EventScript_AtSkyPillar::
	msgbox CaveOfOrigin_B4F_Text_WellHeadToSkyPillar, MSGBOX_DEFAULT
	closemessage
	playse SE_EXIT
	fadescreenspeed FADE_TO_BLACK, 4
	setflag FLAG_WALLACE_GOES_TO_SKY_PILLAR
	setvar VAR_SOOTOPOLIS_CITY_STATE, 3
	removeobject LOCALID_WALLACE
	clearflag FLAG_HIDE_SKY_PILLAR_WALLACE
	fadescreen FADE_FROM_BLACK
	release
	end

CaveOfOrigin_B4F_EventScript_Mewtwo::
	lock
	specialvar VAR_RESULT CheckPartyForMew
	goto_if_eq VAR_RESULT, FALSE, CaveOfOrigin_B4F_EventScript_End
	delay 20
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	delay 20
	clearflag FLAG_HIDE_MEW_CAVE_OF_ORIGIN
	addobject LOCALID_MEW
	applymovement LOCALID_MEW, CaveOfOrigin_B4F_Movement_MewAppearsNearPlayer
	waitmovement 0
	delay 60
	special SpawnCameraObject
	applymovement LOCALID_MEW, CaveOfOrigin_B4F_Movement_MewFloatsToRoomCenter
	applymovement OBJ_EVENT_ID_CAMERA, CaveOfOrigin_B4F_Movement_PanToRoomCenter
	waitmovement 0
	delay 20
	setvar VAR_SOOTOPOLIS_CITY_STATE, 8
	applymovement LOCALID_MEW, CaveOfOrigin_B4F_Movement_MewFloatsToRight
	waitmovement 0
	delay 40
	playmoncry SPECIES_MEW, CRY_MODE_NORMAL
	waitmoncry
	playse SE_M_TELEPORT
	warpwhitefade MAP_CAVE_OF_ORIGIN_B4F, 9, 9
	waitstate
	end

CaveOfOrigin_B4F_EventScript_MewtwoLava::
	special SpawnCameraObject
	lock
	delay 60
	applymovement LOCALID_MEW, CaveOfOrigin_B4F_Movement_MewFloatsToLeft
	waitmovement 0
	delay 40
	setvar VAR_SOOTOPOLIS_CITY_STATE, 9
	playmoncry SPECIES_MEW, CRY_MODE_NORMAL
	waitmoncry
	playse SE_M_TELEPORT
	warpwhitefade MAP_CAVE_OF_ORIGIN_B4F, 9, 9
	waitstate
	end

CaveOfOrigin_B4F_EventScript_MewtwoWater::
	special SpawnCameraObject
	lock
	delay 60
	applymovement LOCALID_MEW, CaveOfOrigin_B4F_Movement_MewFloatsToRight
	waitmovement 0
	delay 60
	applymovement LOCALID_MEW, CaveOfOrigin_B4F_Movement_MewLookAround
	waitmovement 0
	setvar VAR_SOOTOPOLIS_CITY_STATE, 10
	playmoncry SPECIES_MEW, CRY_MODE_NORMAL
	waitmoncry
	playse SE_THUNDER
	warpsilent MAP_CAVE_OF_ORIGIN_B4F, 9, 13
	waitstate
	end

CaveOfOrigin_B4F_EventScript_MewtwoBattle::
	lock
	playse SE_M_ROCK_THROW
	waitse
	delay 20
	playse SE_M_DETECT
	applymovement LOCALID_MEW, CaveOfOrigin_B4F_Movement_MewHide
	waitmovement 0
	delay 40
	playmoncry SPECIES_MEWTWO, CRY_MODE_ENCOUNTER
	setflag FLAG_HIDE_MEW_CAVE_OF_ORIGIN
	waitmoncry
	special RemoveCameraObject
	setwildbattle SPECIES_MEWTWO, 70
	special BattleSetup_StartLegendaryBattle
	waitstate
	setvar VAR_SOOTOPOLIS_CITY_STATE, 11
	setmaplayoutindex LAYOUT_CAVE_OF_ORIGIN_B4F
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, CaveOfOrigin_B4F_EventScript_DefeatedMewtwo
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, CaveOfOrigin_B4F_EventScript_RanFromMewtwo
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, CaveOfOrigin_B4F_EventScript_RanFromMewtwo
	setflag FLAG_CAUGHT_MEWTWO
CaveOfOrigin_B4F_EventScript_End::
	warpsilent MAP_CAVE_OF_ORIGIN_B4F, 9, 13
	release
	end

CaveOfOrigin_B4F_EventScript_DefeatedMewtwo::
	setflag FLAG_DEFEATED_MEWTWO
CaveOfOrigin_B4F_EventScript_RanFromMewtwo::
	setvar VAR_0x8004, SPECIES_MEWTWO
	bufferspeciesname STR_VAR_1, VAR_0x8004
	msgbox gText_LegendaryFlewAway, MSGBOX_DEFAULT
	goto CaveOfOrigin_B4F_EventScript_End

CaveOfOrigin_B4F_Movement_MewAppearsNearPlayer:
	walk_right
	face_up
	step_end

CaveOfOrigin_B4F_Movement_MewFloatsToRoomCenter:
	walk_up
	walk_up
	walk_up
	walk_left
	walk_up
	step_end

CaveOfOrigin_B4F_Movement_MewFloatsToRight:
	walk_right
	walk_right
	walk_right
	step_end

CaveOfOrigin_B4F_Movement_MewFloatsToLeft:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

CaveOfOrigin_B4F_Movement_MewLookAround:
	face_down
	delay_16
	delay_16
	face_up
	delay_16
	delay_16
	delay_16
	face_down
	delay_16
	delay_16
	step_end

CaveOfOrigin_B4F_Movement_MewHide:
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	set_invisible
	step_end

CaveOfOrigin_B4F_Movement_PanToRoomCenter:
	walk_slow_up
	walk_slow_up
	walk_slow_up
	walk_slow_up
	step_end

CaveOfOrigin_B4F_Text_WallaceStory:
	.string "Ah, so you are {PLAYER}{KUN}?\n"
	.string "I've heard tales of your exploits.\p"
	.string "My name is WALLACE.\p"
	.string "I'm this town's GYM LEADER.\p"
	.string "I've also been entrusted with the\n"
	.string "protection of the CAVE OF ORIGIN.\p"
	.string "… … … … … …\n"
	.string "… … … … … …\p"
	.string "GROUDON and KYOGRE, the two POKéMON\n"
	.string "wreaking havoc here, are considered\l"
	.string "to be super-ancient POKéMON.\p"
	.string "But there aren't just two super-\n"
	.string "ancient POKéMON.\p"
	.string "There is one more somewhere.\p"
	.string "Somewhere, there is a super-\n"
	.string "ancient POKéMON named RAYQUAZA.\p"
	.string "It's said that it was RAYQUAZA that\n"
	.string "becalmed the two combatants in\l"
	.string "the distant past.\p"
	.string "But even I have no clue as to\n"
	.string "RAYQUAZA's whereabouts…$"

CaveOfOrigin_B4F_Text_WhereIsRayquaza:
	.string "WALLACE: {PLAYER}{KUN}, do you perhaps\n"
	.string "know where RAYQUAZA is now?\p"
	.string "If you do, please tell me.$"

CaveOfOrigin_B4F_Text_ButWereInCaveOfOrigin:
	.string "WALLACE: The CAVE OF ORIGIN?\p"
	.string "But that's right here!\n"
	.string "I need you to do better than that!\p"
	.string "Please, I need you to think about\n"
	.string "where RAYQUAZA might be right now.$"

CaveOfOrigin_B4F_Text_OldLadyDidntMentionThat:
	.string "WALLACE: MT. PYRE?\p"
	.string "But when I met the old lady there\n"
	.string "earlier, she made no mention of it.\p"
	.string "I very much doubt that the old lady\n"
	.string "would try to hide something from me…\p"
	.string "{PLAYER}{KUN}, could you think about this\n"
	.string "more carefully for me?$"

CaveOfOrigin_B4F_Text_CantYouRememberSomehow:
	.string "WALLACE: Huh? You don't remember?\n"
	.string "Hmm… That's a problem…\p"
	.string "Can't you remember somehow?$"

CaveOfOrigin_B4F_Text_WellHeadToSkyPillar:
	.string "WALLACE: The SKY PILLAR?\p"
	.string "That's it!\n"
	.string "It must be the SKY PILLAR!\p"
	.string "{PLAYER}{KUN}, there's not a moment to lose!\n"
	.string "We'll head to the SKY PILLAR right away!$"

