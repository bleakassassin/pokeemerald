.set LOCALID_LADYS_MON, 34

LilycoveCity_ContestHall_EventScript_ContestLady::
	lock
	faceplayer
	special Script_BufferContestLadyCategoryAndMonName
	msgbox LilycoveCity_ContestHall_Text_ImTheContestLady, MSGBOX_DEFAULT
	goto_if_eq VAR_CONTEST_LADY_STATE, 4, LilycoveCity_ContestHall_EventScript_AirContestLadyShow
	goto_if_eq VAR_CONTEST_LADY_STATE, 5, LilycoveCity_ContestHall_EventScript_EnterMoreContests
	goto_if_eq VAR_CONTEST_LADY_STATE, 1, LilycoveCity_ContestHall_EventScript_WonContest
	goto_if_eq VAR_CONTEST_LADY_STATE, 2, LilycoveCity_ContestHall_EventScript_LostContest
	goto_if_eq VAR_CONTEST_LADY_STATE, 3, LilycoveCity_ContestHall_EventScript_StartFromScratch
	specialvar VAR_RESULT, HasPlayerGivenContestLadyPokeblock
	goto_if_eq VAR_RESULT, FALSE, LilycoveCity_ContestHall_EventScript_AskForPokeblock
	goto_if_unset FLAG_DAILY_GAVE_CONTEST_LADY_POKEBLOCK, LilycoveCity_ContestHall_EventScript_AskForAnotherPokeblock
	msgbox LilycoveCity_ContestHall_Text_ThankForPokeblock, MSGBOX_DEFAULT
	release
	end

LilycoveCity_ContestHall_EventScript_AskForAnotherPokeblock::
	msgbox LilycoveCity_ContestHall_Text_AskingForAnotherPokeblock, MSGBOX_YESNO
	goto LilycoveCity_ContestHall_EventScript_GiveAnotherPokeblock

LilycoveCity_ContestHall_EventScript_LostContest::
	msgbox LilycoveCity_ContestHall_Text_WeDidntWin, MSGBOX_DEFAULT
	setvar VAR_CONTEST_LADY_STATE, 0
	goto_if_unset FLAG_DAILY_GAVE_CONTEST_LADY_POKEBLOCK, LilycoveCity_ContestHall_EventScript_AskForAnotherPokeblock
	release
	end

LilycoveCity_ContestHall_EventScript_StartFromScratch::
	msgbox LilycoveCity_ContestHall_Text_WeLostBadly, MSGBOX_DEFAULT
	setvar VAR_CONTEST_LADY_STATE, 0
LilycoveCity_ContestHall_EventScript_AskForPokeblock::
	msgbox LilycoveCity_ContestHall_Text_MyFriendDisplaysQuality, MSGBOX_DEFAULT
	checkitem ITEM_POKEBLOCK_CASE
	goto_if_eq VAR_RESULT, FALSE, LilycoveCity_ContestHall_EventScript_NoPokeblockCase
	msgbox LilycoveCity_ContestHall_Text_AskingForOnePokeblock, MSGBOX_YESNO
LilycoveCity_ContestHall_EventScript_GiveAnotherPokeblock::
	goto_if_eq VAR_RESULT, NO, LilycoveCity_ContestHall_EventScript_DeclineGivePokeblock
LilycoveCity_ContestHall_EventScript_ChoosePokeblock::
	fadescreen FADE_TO_BLACK
	special OpenPokeblockCaseForContestLady
	waitstate
	goto_if_ne VAR_RESULT, 0xFFFF, LilycoveCity_ContestHall_EventScript_GivePokeblock
	msgbox LilycoveCity_ContestHall_Text_ICantHaveOnePokeblock, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, LilycoveCity_ContestHall_EventScript_ChoosePokeblock
LilycoveCity_ContestHall_EventScript_DeclineGivePokeblock::
	msgbox LilycoveCity_ContestHall_Text_WhatACheapskate, MSGBOX_DEFAULT
	release
	end

@ VAR_0x8004 here is the return value from GivePokeblockToContestLady
LilycoveCity_ContestHall_EventScript_GivePokeblock::
	msgbox LilycoveCity_ContestHall_Text_IllUseYourPokeblock, MSGBOX_DEFAULT
	setflag FLAG_DAILY_GAVE_CONTEST_LADY_POKEBLOCK
	special SetContestLadyGivenPokeblock
	special GetContestLadyMonSpecies
	applymovement VAR_LAST_TALKED, LilycoveCity_ContestHall_Movement_LadyFaceMon
	waitmovement 0
	delay 60
	applymovement LOCALID_LADYS_MON, LilycoveCity_ContestHall_Movement_MonFaceLady
	waitmovement 0
	delay 60
	waitse
	playmoncry VAR_0x8005, CRY_MODE_NORMAL
	delay 120
	waitmoncry
	goto_if_eq VAR_0x8004, 1, LilycoveCity_ContestHall_EventScript_MonEnjoyPokeblock
	goto LilycoveCity_ContestHall_EventScript_FinishFeedPokeblock

@ VAR_0x8004 here is the return value from GivePokeblockToContestLady
LilycoveCity_ContestHall_EventScript_MonEnjoyPokeblock::
	applymovement LOCALID_LADYS_MON, LilycoveCity_ContestHall_Movement_MonJump
	waitmovement 0
	delay 60
LilycoveCity_ContestHall_EventScript_FinishFeedPokeblock::
	applymovement VAR_LAST_TALKED, Common_Movement_FacePlayer
	waitmovement 0
	delay 60
	goto_if_eq VAR_0x8004, 1, LilycoveCity_ContestHall_EventScript_MonLikedPokeblock
	msgbox LilycoveCity_ContestHall_Text_NoChangeThanks, MSGBOX_DEFAULT
	goto LilycoveCity_ContestHall_EventScript_CheckAirContestLadyShow

LilycoveCity_ContestHall_EventScript_MonLikedPokeblock::
	special Script_BufferContestLadyCategoryAndMonName
	msgbox LilycoveCity_ContestHall_Text_ReallyImprovedThanks, MSGBOX_DEFAULT
LilycoveCity_ContestHall_EventScript_CheckAirContestLadyShow::
	specialvar VAR_RESULT, ShouldContestLadyShowGoOnAir
	goto_if_eq VAR_RESULT, 1, LilycoveCity_ContestHall_EventScript_AirContestLadyShow
	release
	end

LilycoveCity_ContestHall_EventScript_AirContestLadyShow::
	msgbox LilycoveCity_ContestHall_Text_ReadyToEnterContests, MSGBOX_DEFAULT
	special PutLilycoveContestLadyShowOnTheAir
	release
	end

LilycoveCity_ContestHall_EventScript_WonContest::
	msgbox LilycoveCity_ContestHall_Text_WeWonPleaseHaveMon, MSGBOX_DEFAULT
	setvar VAR_TEMP_1, SPECIES_TORCHIC
	call_if_eq VAR_STARTER_MON, 0, LilycoveCity_ContestHall_EventScript_GiveMudkip
	call_if_eq VAR_STARTER_MON, 1, LilycoveCity_ContestHall_EventScript_GiveTreecko
	givemon VAR_TEMP_1, 5
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, LilycoveCity_ContestHall_EventScript_Send3rdStarterParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, LilycoveCity_ContestHall_EventScript_Send3rdStarterPC
	goto Common_EventScript_NoMoreRoomForPokemon
	release
	end

LilycoveCity_ContestHall_EventScript_GiveMudkip::
	setvar VAR_TEMP_1, SPECIES_MUDKIP
	return

LilycoveCity_ContestHall_EventScript_GiveTreecko::
	setvar VAR_TEMP_1, SPECIES_TREECKO
	return

LilycoveCity_ContestHall_EventScript_Send3rdStarterParty::
	call LilycoveCity_ContestHall_EventScript_Received3rdStarter
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, LilycoveCity_ContestHall_EventScript_EnterMoreContests
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto LilycoveCity_ContestHall_EventScript_EnterMoreContests

LilycoveCity_ContestHall_EventScript_Send3rdStarterPC::
	call LilycoveCity_ContestHall_EventScript_Received3rdStarter
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, LilycoveCity_ContestHall_EventScript_3rdStarterTransferredToPC
	call Common_EventScript_NameReceivedBoxMon
LilycoveCity_ContestHall_EventScript_3rdStarterTransferredToPC::
	call Common_EventScript_TransferredToPC
LilycoveCity_ContestHall_EventScript_EnterMoreContests::
	msgbox LilycoveCity_ContestHall_Text_WellEnterMoreContests, MSGBOX_DEFAULT
	release
	end

LilycoveCity_ContestHall_EventScript_Received3rdStarter::
	bufferspeciesname STR_VAR_1, VAR_TEMP_1
	setvar VAR_CONTEST_LADY_STATE, 5
	playfanfare MUS_EVOLVED
	message LilycoveCity_ContestHall_Text_Received3rdStarter
	waitmessage
	waitfanfare
	return

LilycoveCity_ContestHall_EventScript_NoPokeblockCase::
	msgbox LilycoveCity_ContestHall_Text_DontHaveAPokeblockCase, MSGBOX_DEFAULT
	release
	end

LilycoveCity_ContestHall_Movement_LadyFaceMon:
	face_up
	delay_8
	step_end

LilycoveCity_ContestHall_Movement_MonFaceLady:
	face_down
	delay_8
	step_end

LilycoveCity_ContestHall_Movement_MonJump:
	disable_jump_landing_ground_effect
	jump_in_place_down
	disable_jump_landing_ground_effect
	jump_in_place_down
	step_end

LilycoveCity_ContestHall_EventScript_ContestLadyMon::
	lock
	faceplayer
	specialvar VAR_RESULT, GetContestLadyCategory
	special Script_BufferContestLadyCategoryAndMonName
	special GetContestLadyMonSpecies
	waitse
	playmoncry VAR_0x8005, CRY_MODE_NORMAL
	call_if_eq VAR_RESULT, CONTEST_CATEGORY_COOL, LilycoveCity_ContestHall_EventScript_Zigzagoon
	call_if_eq VAR_RESULT, CONTEST_CATEGORY_BEAUTY, LilycoveCity_ContestHall_EventScript_Skitty
	call_if_eq VAR_RESULT, CONTEST_CATEGORY_CUTE, LilycoveCity_ContestHall_EventScript_Poochyena
	call_if_eq VAR_RESULT, CONTEST_CATEGORY_SMART, LilycoveCity_ContestHall_EventScript_Kecleon
	call_if_eq VAR_RESULT, CONTEST_CATEGORY_TOUGH, LilycoveCity_ContestHall_EventScript_Pikachu
	waitmoncry
	release
	end

LilycoveCity_ContestHall_EventScript_Zigzagoon::
	msgbox LilycoveCity_ContestHall_Text_Zigzagoon, MSGBOX_DEFAULT
	return

LilycoveCity_ContestHall_EventScript_Skitty::
	msgbox LilycoveCity_ContestHall_Text_Skitty, MSGBOX_DEFAULT
	return

LilycoveCity_ContestHall_EventScript_Poochyena::
	msgbox LilycoveCity_ContestHall_Text_Poochyena, MSGBOX_DEFAULT
	return

LilycoveCity_ContestHall_EventScript_Kecleon::
	msgbox LilycoveCity_ContestHall_Text_Kecleon, MSGBOX_DEFAULT
	return

LilycoveCity_ContestHall_EventScript_Pikachu::
	msgbox LilycoveCity_ContestHall_Text_Pikachu, MSGBOX_DEFAULT
	return

LilycoveCity_ContestHall_Text_ImTheContestLady:
	.string "I'm the CONTEST LADY!\n"
	.string "I sure do love CONTESTS!$"

LilycoveCity_ContestHall_Text_ThankForPokeblock:
	.string "Thanks for your {POKEBLOCK} before!$"

LilycoveCity_ContestHall_Text_MyFriendDisplaysQuality:
	.string "This is my friend {STR_VAR_1}!\n"
	.string "It's the epitome of {STR_VAR_2}!\p"
	.string "But I think that it will display\n"
	.string "even more {STR_VAR_2}!$"

LilycoveCity_ContestHall_Text_DontHaveAPokeblockCase:
	.string "So, I need your help!\p"
	.string "Please, may I have one {POKEBLOCK}?\n"
	.string "All I'm asking for is one!\p"
	.string "…Oh, but…\n"
	.string "Don't you have a {POKEBLOCK} CASE?\l"
	.string "That's no good. Next time, then!$"

LilycoveCity_ContestHall_Text_AskingForOnePokeblock:
	.string "So, I need your help!\p"
	.string "Please, may I have one {POKEBLOCK}?\n"
	.string "All I'm asking for is one!$"

LilycoveCity_ContestHall_Text_AskingForAnotherPokeblock:
	.string "The {POKEBLOCK} I gave {STR_VAR_1} the other\n"
	.string "day did all that it could.\p"
	.string "We can do better, though!\n"
	.string "I want {STR_VAR_1} to show off as much\l"
	.string "of its {STR_VAR_2} as possible!\p"
	.string "I hope this isn't too much to ask, but\n"
	.string "can I please have another one?$"

LilycoveCity_ContestHall_Text_ICantHaveOnePokeblock:
	.string "Awww!\n"
	.string "I can't have one {POKEBLOCK}?!$"

LilycoveCity_ContestHall_Text_WhatACheapskate:
	.string "Sheesh!\n"
	.string "What a cheapskate!$"

LilycoveCity_ContestHall_Text_IllUseYourPokeblock:
	.string "Yay!\n"
	.string "Thank you!\p"
	.string "I'll feed my POKéMON your {POKEBLOCK}\n"
	.string "right away.$"

LilycoveCity_ContestHall_Text_NoChangeThanks:
	.string "…It doesn't seem to have changed\n"
	.string "in any way at all…\p"
	.string "Hmm…\p"
	.string "Oh, well!\n"
	.string "Thank you very much!$"

LilycoveCity_ContestHall_Text_ReallyImprovedThanks:
	.string "Oh, yay!\n"
	.string "It's really delighted!\p"
	.string "I think it really improved {STR_VAR_1}'s\n"
	.string "{STR_VAR_2} quality, too.\p"
	.string "Thank you so much!$"

LilycoveCity_ContestHall_Text_ReadyToEnterContests:
	.string "Hmm…\p"
	.string "I think we may be ready to enter\n"
	.string "some CONTESTS.\p"
	.string "If you see us in one somewhere,\n"
	.string "I hope you'll cheer for us.$"

LilycoveCity_ContestHall_Text_WeWonPleaseHaveMon:
	.string "{PLAYER}! Guess what?\p"
	.string "We did it!\n"
	.string "We won our CONTEST!\p"
	.string "I hope you could see us. We dazzled\n"
	.string "the crowd and impressed the JUDGE!\p"
	.string "If it wasn't for your help, I don't know\n"
	.string "where we'd be.\p"
	.string "Please have this as my thank-you gift!\n"
	.string "I've already settled on a full team for\l"
	.string "future CONTESTS.\p"
	.string "I figured you'd like it!$"

LilycoveCity_ContestHall_Text_Received3rdStarter:
	.string "{PLAYER} received a {STR_VAR_1} from\n"
	.string "the CONTEST LADY!$"

LilycoveCity_ContestHall_Text_WellEnterMoreContests:
	.string "I'm gonna fully commit to my buddies\n"
	.string "and enter more CONTESTS with them!\p"
	.string "I believe that we can become CONTEST\n"
	.string "masters together!$"

LilycoveCity_ContestHall_Text_WeDidntWin:
	.string "Did you see our CONTEST?\n"
	.string "We did our best, but it wasn't enough…\p"
	.string "It was rough.\n"
	.string "I hope we didn't disappoint you…\p"
	.string "We can bounce back, though.\n"
	.string "{STR_VAR_1} and I have what it takes!$"

LilycoveCity_ContestHall_Text_WeLostBadly:
	.string "I hope you didn't see our CONTEST.\n"
	.string "We lost… badly.\p"
	.string "It was probably the worst feeling\n"
	.string "I've ever had in my life…\p"
	.string "… … … … … …\p"
	.string "I decided it was best to start over\n"
	.string "and build back up from square one.\p"
	.string "I think I've still got what it takes\n"
	.string "to be a master coordinator!$"

LilycoveCity_ContestHall_Text_Zigzagoon:
	.string "{STR_VAR_1}: Guguuh!$"

LilycoveCity_ContestHall_Text_Kecleon:
	.string "{STR_VAR_1}: Igigigiiih!$"

LilycoveCity_ContestHall_Text_Poochyena:
	.string "{STR_VAR_1}: Baaarun…$"

LilycoveCity_ContestHall_Text_Pikachu:
	.string "{STR_VAR_1}: Pikka!$"

LilycoveCity_ContestHall_Text_Skitty:
	.string "{STR_VAR_1}: Umyaaaan!$"
