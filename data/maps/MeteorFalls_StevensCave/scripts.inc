.set LOCALID_STEVEN, 1

MeteorFalls_StevensCave_MapScripts::
	.byte 0

MeteorFalls_StevensCave_EventScript_Steven::
	lock
	goto_if_set FLAG_DAILY_WON_STEVEN_REMATCH, MeteorFalls_StevensCave_EventScript_Defeated
	goto_if_set FLAG_DEFEATED_METEOR_FALLS_STEVEN, MeteorFalls_StevensCave_EventScript_RematchText
	waitse
	playse SE_PIN
	applymovement LOCALID_STEVEN, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_STEVEN, Common_Movement_Delay48
	waitmovement 0
	applymovement LOCALID_STEVEN, Common_Movement_FacePlayer
	waitmovement 0
	msgbox MeteorFalls_StevensCave_Text_ShouldKnowHowGoodIAmExpectWorst, MSGBOX_DEFAULT
MeteorFalls_StevensCave_EventScript_StevenBattle::
	trainerbattle_no_intro TRAINER_STEVEN, MeteorFalls_StevensCave_Text_StevenDefeat
	msgbox MeteorFalls_StevensCave_Text_MyPredictionCameTrue, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_METEOR_FALLS_STEVEN
	setflag FLAG_DAILY_WON_STEVEN_REMATCH
	release
	end

MeteorFalls_StevensCave_EventScript_Defeated::
	applymovement LOCALID_STEVEN, Common_Movement_FacePlayer
	waitmovement 0
	msgbox MeteorFalls_StevensCave_Text_MyPredictionCameTrue, MSGBOX_DEFAULT
	release
	end

MeteorFalls_StevensCave_EventScript_RematchText::
	faceplayer
	msgbox MeteorFalls_StevensCave_Text_TestSkillsAgainstYou, MSGBOX_DEFAULT
	goto MeteorFalls_StevensCave_EventScript_StevenBattle

MeteorFalls_StevensCave_Text_ShouldKnowHowGoodIAmExpectWorst:
	.string "STEVEN: Oh, wow, {PLAYER}{KUN}.\n"
	.string "I'm amazed you knew where to find me.\p"
	.string "Many, uh…maybe think of me as\n"
	.string "just a rock maniac.\p"
	.string "But you know there's more to me.\p"
	.string "You should have a very good idea about\n"
	.string "how good I am as a TRAINER.\p"
	.string "My POKéMON and I have polished every\n"
	.string "surface of our game since our last\l"
	.string "match with you and your team.\p"
	.string "Okay, {PLAYER}{KUN}, if you're going to mount\n"
	.string "a serious challenge, expect the worst!$"

MeteorFalls_StevensCave_Text_TestSkillsAgainstYou:
	.string "STEVEN: So you've returned, {PLAYER}{KUN}.\p"
	.string "Let me test my skills against you\n"
	.string "once again!$"

MeteorFalls_StevensCave_Text_StevenDefeat:
	.string "You…\n"
	.string "I had no idea you'd become so strong…$"

MeteorFalls_StevensCave_Text_MyPredictionCameTrue:
	.string "STEVEN: Come to think of it, ever since\n"
	.string "our paths first crossed in GRANITE\l"
	.string "CAVE in DEWFORD, I had this feeling.\p"
	.string "I thought that you would eventually\n"
	.string "become the CHAMPION.\p"
	.string "My predictions usually come true.\p"
	.string "And where will you go from here?\p"
	.string "… … … … … …\n"
	.string "… … … … … …\p"
	.string "Fufufu, even I couldn't tell you that.$"
