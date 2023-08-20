.set LOCALID_RIVAL, 1

LittlerootTown_MaysHouse_2F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, LittlerootTown_MaysHouse_2F_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, LittlerootTown_MaysHouse_2F_OnWarp
	.byte 0

LittlerootTown_MaysHouse_2F_OnTransition:
	call_if_lt VAR_LITTLEROOT_RIVAL_STATE, 2, LittlerootTown_MaysHouse_2F_EventScript_SetReadyToMeetMay
	call_if_eq VAR_LITTLEROOT_INTRO_STATE, 4, PlayersHouse_2F_EventScript_BlockStairsUntilClockIsSet
	call SecretBase_EventScript_SetDecorationFlags
	setvar VAR_SECRET_BASE_INITIALIZED, 0
	end

LittlerootTown_MaysHouse_2F_EventScript_SetReadyToMeetMay::
	setobjectxyperm LOCALID_RIVAL, 7, 2
	setobjectmovementtype LOCALID_RIVAL, MOVEMENT_TYPE_FACE_UP
	return

LittlerootTown_MaysHouse_2F_OnWarp:
	map_script_2 VAR_SECRET_BASE_INITIALIZED, 0, LittlerootTown_MaysHouse_2F_EventScript_CheckInitDecor
	.2byte 0

LittlerootTown_MaysHouse_2F_EventScript_CheckInitDecor::
	checkplayergender
	goto_if_eq VAR_RESULT, FEMALE, SecretBase_EventScript_InitDecorations
	end

LittlerootTown_MaysHouse_2F_EventScript_RivalsPokeBall::
	lockall
	msgbox RivalsHouse_2F_Text_ItsRivalsPokeBall, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RivalsHouse_2F_EventScript_LeaveRivalStarter
RivalsHouse_2F_EventScript_TakeRivalStarter::
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_TREECKO
	call_if_eq VAR_STARTER_MON, 0, RivalsHouse_2F_EventScript_GiveTorchic
	call_if_eq VAR_STARTER_MON, 1, RivalsHouse_2F_EventScript_GiveMudkip
	givemon VAR_TEMP_TRANSFERRED_SPECIES, 5
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, RivalsHouse_2F_EventScript_SendRivalStarterParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, RivalsHouse_2F_EventScript_SendRivalStarterPC
	goto Common_EventScript_NoMoreRoomForPokemon

RivalsHouse_2F_EventScript_GiveTorchic::
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_TORCHIC
	return

RivalsHouse_2F_EventScript_GiveMudkip::
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_MUDKIP
	return

RivalsHouse_2F_EventScript_SendRivalStarterParty::
	call RivalsHouse_2F_EventScript_ReceivedRivalStarter
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RivalsHouse_2F_EventScript_AddedRivalStarter
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto RivalsHouse_2F_EventScript_AddedRivalStarter

RivalsHouse_2F_EventScript_SendRivalStarterPC::
	call RivalsHouse_2F_EventScript_ReceivedRivalStarter
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RivalsHouse_2F_EventScript_RivalStarterTransferredToPC
	call Common_EventScript_NameReceivedBoxMon
RivalsHouse_2F_EventScript_RivalStarterTransferredToPC::
	call Common_EventScript_TransferredToPC
RivalsHouse_2F_EventScript_AddedRivalStarter::
	releaseall
	end

RivalsHouse_2F_EventScript_ReceivedRivalStarter::
	bufferspeciesname STR_VAR_1, VAR_TEMP_TRANSFERRED_SPECIES
	removeobject VAR_LAST_TALKED
	playfanfare MUS_EVOLVED
	message gText_ReceivedPokemon
	waitmessage
	waitfanfare
	return

RivalsHouse_2F_EventScript_LeaveRivalStarter::
	msgbox RivalsHouse_2F_Text_LeaveRightWhereItIs, MSGBOX_DEFAULT
	releaseall
	end

RivalsHouse_2F_EventScript_Rival::
	lockall
	goto_if_set FLAG_MET_RIVAL_LILYCOVE, RivalsHouse_2F_EventScript_RivalPostLilycove
	checkplayergender
	call_if_eq VAR_RESULT, MALE, RivalsHouse_2F_EventScript_May
	call_if_eq VAR_RESULT, FEMALE, RivalsHouse_2F_EventScript_Brendan
	playse SE_EXIT
	removeobject VAR_LAST_TALKED
	setvar VAR_LITTLEROOT_RIVAL_STATE, 3
	setvar VAR_LITTLEROOT_TOWN_STATE, 1
	savebgm MUS_DUMMY
	fadedefaultbgm
	releaseall
	end

RivalsHouse_2F_EventScript_May::
	msgbox RivalsHouse_2F_Text_MayGettingReady, MSGBOX_DEFAULT
	playbgm MUS_ENCOUNTER_MAY, TRUE
	applymovement VAR_LAST_TALKED, Common_Movement_FacePlayer
	waitmovement 0
	applymovement VAR_LAST_TALKED, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement VAR_LAST_TALKED, Common_Movement_Delay48
	waitmovement 0
	msgbox RivalsHouse_2F_Text_MayWhoAreYou, MSGBOX_DEFAULT
	closemessage
	goto_if_eq VAR_FACING, DIR_EAST, RivalsHouse_2F_EventScript_MeetMayEast
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceLeft
	applymovement VAR_LAST_TALKED, RivalsHouse_2F_Movement_MayGoDownstairs
	waitmovement 0
	return

RivalsHouse_2F_EventScript_MeetMayEast::
	applymovement OBJ_EVENT_ID_PLAYER, RivalsHouse_2F_Movement_PlayerWatchMayEast
	applymovement VAR_LAST_TALKED, RivalsHouse_2F_Movement_MayGoDownstairsEast
	waitmovement 0
	return

RivalsHouse_2F_Movement_MayGoDownstairsEast:
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	delay_8
	step_end

RivalsHouse_2F_Movement_MayGoDownstairs:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	delay_8
	step_end

RivalsHouse_2F_Movement_PlayerWatchMayEast:
	delay_16
	delay_16
	face_left
	step_end

RivalsHouse_2F_EventScript_Brendan::
	msgbox RivalsHouse_2F_Text_BrendanGettingReady, MSGBOX_DEFAULT
	playbgm MUS_ENCOUNTER_BRENDAN, TRUE
	applymovement VAR_LAST_TALKED, Common_Movement_FacePlayer
	waitmovement 0
	applymovement VAR_LAST_TALKED, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement VAR_LAST_TALKED, Common_Movement_Delay48
	waitmovement 0
	msgbox RivalsHouse_2F_Text_BrendanWhoAreYou, MSGBOX_DEFAULT
	closemessage
	goto_if_eq VAR_FACING, DIR_WEST, RivalsHouse_2F_EventScript_MeetBrendanWest
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceRight
	applymovement VAR_LAST_TALKED, RivalsHouse_2F_Movement_BrendanGoDownstairs
	waitmovement 0
	return

RivalsHouse_2F_EventScript_MeetBrendanWest::
	applymovement OBJ_EVENT_ID_PLAYER, RivalsHouse_2F_Movement_PlayerWatchBrendanWest
	applymovement VAR_LAST_TALKED, RivalsHouse_2F_Movement_BrendanGoDownstairsWest
	waitmovement 0
	return

RivalsHouse_2F_Movement_BrendanGoDownstairsWest:
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_up
	walk_up
	delay_8
	step_end

RivalsHouse_2F_Movement_BrendanGoDownstairs:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_up
	delay_8
	step_end

RivalsHouse_2F_Movement_PlayerWatchBrendanWest:
	delay_16
	delay_16
	face_right
	step_end

RivalsHouse_2F_EventScript_RivalPostLilycove::
	applymovement VAR_LAST_TALKED, Common_Movement_FacePlayer
	waitmovement 0
	checkplayergender
	call_if_eq VAR_RESULT, MALE, RivalsHouse_2F_EventScript_MayPostLilycove
	call_if_eq VAR_RESULT, FEMALE, RivalsHouse_2F_EventScript_BrendanPostLilycove
	setflag FLAG_MET_RIVAL_IN_HOUSE_AFTER_LILYCOVE
	releaseall
	end

RivalsHouse_2F_EventScript_MayPostLilycove::
	goto_if_set FLAG_MET_RIVAL_IN_HOUSE_AFTER_LILYCOVE, RivalsHouse_2F_EventScript_MayWhereShouldIGoNext
	msgbox RivalsHouse_2F_Text_MayJustCheckingMyPokedex, MSGBOX_DEFAULT
	return

RivalsHouse_2F_EventScript_BrendanPostLilycove::
	goto_if_set FLAG_MET_RIVAL_IN_HOUSE_AFTER_LILYCOVE, RivalsHouse_2F_EventScript_BrendanWhereShouldIGoNext
	msgbox RivalsHouse_2F_Text_BrendanJustCheckingMyPokedex, MSGBOX_DEFAULT
	return

RivalsHouse_2F_EventScript_MayWhereShouldIGoNext::
	msgbox RivalsHouse_2F_Text_MayWhereShouldIGoNext, MSGBOX_DEFAULT
	return

RivalsHouse_2F_EventScript_BrendanWhereShouldIGoNext::
	msgbox RivalsHouse_2F_Text_BrendanWhereShouldIGoNext, MSGBOX_DEFAULT
	return

LittlerootTown_MaysHouse_2F_EventScript_PC::
	lockall
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, LittlerootTown_MaysHouse_2F_EventScript_CheckRivalsPC
	goto_if_eq VAR_RESULT, FEMALE, LittlerootTown_MaysHouse_2F_EventScript_CheckPlayersPC
	end

LittlerootTown_MaysHouse_2F_EventScript_CheckRivalsPC::
	msgbox gText_PokemonTrainerSchoolEmail, MSGBOX_DEFAULT
	releaseall
	end

LittlerootTown_MaysHouse_2F_EventScript_CheckPlayersPC::
	setvar VAR_0x8004, PC_LOCATION_MAYS_HOUSE
	special DoPCTurnOnEffect
	playse SE_PC_ON
	msgbox gText_PlayerHouseBootPC, MSGBOX_DEFAULT
	special BedroomPC
	waitstate
	releaseall
	end

LittlerootTown_MaysHouse_2F_EventScript_TurnOffPlayerPC::
	setvar VAR_0x8004, PC_LOCATION_MAYS_HOUSE
	playse SE_PC_OFF
	special DoPCTurnOffEffect
	releaseall
	end

RivalsHouse_2F_Text_MayWhoAreYou:
	.string "Huh?\n"
	.string "Who… Who are you?\p"
	.string "… … … … … … … …\n"
	.string "… … … … … … … …\p"
	.string "Oh, you're {PLAYER}{KUN}.\n"
	.string "So your move was today.\p"
	.string "Um… I'm MAY.\n"
	.string "Glad to meet you!\p"
	.string "I…\n"
	.string "I have this dream of becoming friends\l"
	.string "with POKéMON all over the world.\p"
	.string "I… I heard about you, {PLAYER}{KUN}, from\n"
	.string "my dad, PROF. BIRCH.\p"
	.string "I was hoping that you would be nice,\n"
	.string "{PLAYER}{KUN}, and that we could be friends.\p"
	.string "Oh, this is silly, isn't it?\n"
	.string "I… I've just met you, {PLAYER}{KUN}.\p"
	.string "Eheheh…\p"
	.string "Oh, no! I forgot!\p"
	.string "I was supposed to go help Dad catch\n"
	.string "some wild POKéMON!\p"
	.string "{PLAYER}{KUN}, I'll catch you later!$"

RivalsHouse_2F_Text_MayGettingReady:
	.string "POKéMON fully restored!\n"
	.string "Items ready, and…$"

RivalsHouse_2F_Text_BrendanWhoAreYou:
	.string "Hey!\n"
	.string "You…\p"
	.string "Who are you?\p"
	.string "Oh, you're {PLAYER}, aren't you?\n"
	.string "Moved in next door, right?\p"
	.string "I didn't know that you're a girl.\p"
	.string "Dad, PROF. BIRCH, said that our new\n"
	.string "next-door neighbor is a GYM LEADER's\l"
	.string "kid, so I assumed you'd be a guy.\p"
	.string "My name's BRENDAN.\n"
	.string "So, hi, neighbor!\p"
	.string "Huh? Hey, {PLAYER}, don't you have\n"
	.string "a POKéMON?\p"
	.string "Do you want me to go catch you one?\p"
	.string "Aw, darn, I forgot…\p"
	.string "I'm supposed to go help my dad catch\n"
	.string "some wild POKéMON.\p"
	.string "Some other time, okay?$"

RivalsHouse_2F_Text_BrendanGettingReady:
	.string "POKéMON fully restored…\n"
	.string "Items all packed, and…$"

RivalsHouse_2F_Text_ItsRivalsPokeBall:
	.string "It's {RIVAL}'s POKé BALL!\p"
	.string "Do you want to take it?$"
	
RivalsHouse_2F_Text_LeaveRightWhereItIs:
	.string "Better leave it right where it is.$"

RivalsHouse_2F_Text_MayJustCheckingMyPokedex:
	.string "MAY: {PLAYER}{KUN}!\p"
	.string "I was just checking my POKéDEX.\p"
	.string "There's still a lot of POKéMON that\n"
	.string "I've seen but haven't caught.\p"
	.string "And there are many others that\n"
	.string "I think will evolve.\p"
	.string "I wonder where I should go catch\n"
	.string "some POKéMON next?$"

RivalsHouse_2F_Text_MayWhereShouldIGoNext:
	.string "MAY: I wonder where I should go catch\n"
	.string "some POKéMON next?\p"
	.string "Wouldn't it be funny if we ran into each\n"
	.string "other, {PLAYER}{KUN}?$"

RivalsHouse_2F_Text_BrendanJustCheckingMyPokedex:
	.string "BRENDAN: Hey, it's {PLAYER}.\p"
	.string "I was just checking out my POKéDEX.\p"
	.string "There are still many POKéMON that\n"
	.string "I need to catch, I know, but this\l"
	.string "is looking pretty good.\p"
	.string "Checking this POKéDEX out gives me\n"
	.string "the urge to hit the road again.$"

RivalsHouse_2F_Text_BrendanWhereShouldIGoNext:
	.string "BRENDAN: I'm having a hard time deciding\n"
	.string "where I should catch POKéMON next.\p"
	.string "Hey, {PLAYER}, if I see you while I'm out\n"
	.string "catching POKéMON, I can pitch you a\l"
	.string "battle if you'd like.$"
