.set LOCALID_MOVE_RELEARNER, 1

FallarborTown_MoveRelearnersHouse_MapScripts::
	.byte 0

FallarborTown_MoveRelearnersHouse_EventScript_MoveRelearner::
	lockall
	applymovement LOCALID_MOVE_RELEARNER, Common_Movement_FacePlayer
	waitmovement 0
	goto_if_eq VAR_HEART_SCALES_GIVEN_TO_RELEARNER, 10, FallarborTown_MoveRelearnersHouse_EventScript_AskTeachMoveForFree
	goto_if_set FLAG_TEMP_1, FallarborTown_MoveRelearnersHouse_EventScript_AskTeachMove
	msgbox FallarborTown_MoveRelearnersHouse_Text_ImTheMoveTutor, MSGBOX_DEFAULT
	setflag FLAG_TEMP_1
FallarborTown_MoveRelearnersHouse_EventScript_AskTeachMove::
	checkitem ITEM_HEART_SCALE
	goto_if_eq VAR_RESULT, FALSE, FallarborTown_MoveRelearnersHouse_EventScript_ComeBackWithHeartScale
	msgbox FallarborTown_MoveRelearnersHouse_Text_ThatsAHeartScaleWantMeToTeachMove, MSGBOX_YESNO
	switch VAR_RESULT
	case NO, FallarborTown_MoveRelearnersHouse_EventScript_ComeBackWithHeartScale
FallarborTown_MoveRelearnersHouse_EventScript_ChooseMon::
	msgbox FallarborTown_MoveRelearnersHouse_Text_TutorWhichMon, MSGBOX_DEFAULT
	special ChooseMonForMoveRelearner
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, FallarborTown_MoveRelearnersHouse_EventScript_ComeBackWithHeartScale
	special IsSelectedMonEgg
	goto_if_eq VAR_RESULT, TRUE, FallarborTown_MoveRelearnersHouse_EventScript_CantTeachEgg
	goto_if_eq VAR_0x8005, 0, FallarborTown_MoveRelearnersHouse_EventScript_NoMoveToTeachMon
	msgbox FallarborTown_MoveRelearnersHouse_Text_TeachWhichMove, MSGBOX_DEFAULT
	special TeachMoveRelearnerMove
	waitstate
	goto_if_eq VAR_0x8004, 0, FallarborTown_MoveRelearnersHouse_EventScript_ChooseMon
	call_if_lt VAR_HEART_SCALES_GIVEN_TO_RELEARNER, 10, FallarborTown_MoveRelearnersHouse_EventScript_GiveHeartScale
FallarborTown_MoveRelearnersHouse_EventScript_ComeBackWithHeartScale::
	goto_if_eq VAR_HEART_SCALES_GIVEN_TO_RELEARNER, 10, FallarborTown_MoveRelearnersHouse_EventScript_ComeBackToTeachMove
	msgbox FallarborTown_MoveRelearnersHouse_Text_ComeBackWithHeartScale, MSGBOX_DEFAULT
	releaseall
	end

FallarborTown_MoveRelearnersHouse_EventScript_ComeBackToTeachMove::
	msgbox FallarborTown_MoveRelearnersHouse_Text_ComeBackToTeachMove, MSGBOX_DEFAULT
	releaseall
	end

FallarborTown_MoveRelearnersHouse_EventScript_NoMoveToTeachMon::
	msgbox FallarborTown_MoveRelearnersHouse_Text_DontHaveMoveToTeachPokemon, MSGBOX_DEFAULT
	goto FallarborTown_MoveRelearnersHouse_EventScript_ChooseMon
	end

FallarborTown_MoveRelearnersHouse_EventScript_CantTeachEgg::
	msgbox FallarborTown_MoveRelearnersHouse_Text_CantTeachEgg, MSGBOX_DEFAULT
	goto FallarborTown_MoveRelearnersHouse_EventScript_ChooseMon
	end

FallarborTown_MoveRelearnersHouse_EventScript_GiveHeartScale::
	msgbox FallarborTown_MoveRelearnersHouse_Text_HandedOverHeartScale, MSGBOX_DEFAULT
	removeitem ITEM_HEART_SCALE
	addvar VAR_HEART_SCALES_GIVEN_TO_RELEARNER, 1
	goto_if_eq VAR_HEART_SCALES_GIVEN_TO_RELEARNER, 10, FallarborTown_MoveRelearnersHouse_EventScript_TeachMovesForFree
	return

FallarborTown_MoveRelearnersHouse_EventScript_TeachMovesForFree::
	msgbox FallarborTown_MoveRelearnersHouse_Text_TeachMovesForFree, MSGBOX_DEFAULT
	releaseall
	end

FallarborTown_MoveRelearnersHouse_EventScript_AskTeachMoveForFree::
	msgbox FallarborTown_MoveRelearnersHouse_Text_WantMeToTeachMove, MSGBOX_YESNO
	switch VAR_RESULT
	case NO, FallarborTown_MoveRelearnersHouse_Text_ComeBackToTeachMove
	goto FallarborTown_MoveRelearnersHouse_EventScript_ChooseMon

FallarborTown_MoveRelearnersHouse_Text_ImTheMoveTutor:
	.string "I'm the POKéMON MOVE MANIAC.\p"
	.string "I know all the moves that POKéMON\n"
	.string "learn by leveling up, and I can\l"
	.string "teach POKéMON those moves.\p"
	.string "I can teach a move to a POKéMON\n"
	.string "of yours if you'd like.\p"
	.string "I'll do it for a HEART SCALE.\n"
	.string "I'm collecting those now.$"

FallarborTown_MoveRelearnersHouse_Text_ThatsAHeartScaleWantMeToTeachMove:
	.string "Oh! That's it! That's an honest to\n"
	.string "goodness HEART SCALE!\p"
	.string "Let me guess, you want me to teach\n"
	.string "a move?$"

FallarborTown_MoveRelearnersHouse_Text_TutorWhichMon:
	.string "Which POKéMON needs tutoring?$"

FallarborTown_MoveRelearnersHouse_Text_TeachWhichMove:
	.string "Which move should I teach?$"

FallarborTown_MoveRelearnersHouse_Text_DontHaveMoveToTeachPokemon:
	.string "Sorry…\p"
	.string "It doesn't appear as if I have any move\n"
	.string "I can teach that POKéMON.$"

FallarborTown_MoveRelearnersHouse_Text_HandedOverHeartScale:
	.string "{PLAYER} handed over one HEART SCALE\n"
	.string "in exchange.$"

FallarborTown_MoveRelearnersHouse_Text_ComeBackWithHeartScale:
	.string "If your POKéMON need to learn a move,\n"
	.string "come back with a HEART SCALE.$"

FallarborTown_MoveRelearnersHouse_Text_CantTeachEgg:
	.string "Hunh? There isn't a single move that\n"
	.string "I can teach an EGG.$"

FallarborTown_MoveRelearnersHouse_Text_TeachMovesForFree:
	.string "Thank you very much for giving me so\n"
	.string "many HEART SCALES!\p"
	.string "To show my gratitude, I'll start teaching\n"
	.string "moves to your POKéMON for free.\l"
	.string "No HEART SCALES required!$"

FallarborTown_MoveRelearnersHouse_Text_WantMeToTeachMove:
	.string "Do you want me to teach your POKéMON\n"
	.string "a move?$"

FallarborTown_MoveRelearnersHouse_Text_ComeBackToTeachMove:
	.string "Come back when you want me to teach a\n"
	.string "move to your POKéMON.$"
