SootopolisCity_Gym_1F_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, SootopolisCity_Gym_1F_OnFrame
	map_script MAP_SCRIPT_ON_RESUME, SootopolisCity_Gym_1F_OnResume
	map_script MAP_SCRIPT_ON_LOAD, SootopolisCity_Gym_1F_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, SootopolisCity_Gym_1F_OnTransition
	.byte 0

SootopolisCity_Gym_1F_OnTransition:
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_JUAN
	call_if_unset FLAG_READ_STEVENS_LETTER, SootopolisCity_Gym_1F_SetupWallace
	setvar VAR_ICE_STEP_COUNT, 1
	end

SootopolisCity_Gym_1F_SetupWallace::
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_WALLACE
	return

SootopolisCity_Gym_1F_OnResume:
	setstepcallback STEP_CB_SOOTOPOLIS_ICE
	end

SootopolisCity_Gym_1F_OnLoad:
	call SootopolisCity_Gym_1F_EventScript_CheckSetStairMetatiles
	special SetSootopolisGymCrackedIceMetatiles
	end

SootopolisCity_Gym_1F_EventScript_CheckSetStairMetatiles::
	goto_if_lt VAR_ICE_STEP_COUNT, 8, SootopolisCity_Gym_1F_EventScript_StopCheckingStairs  @ All stairs ice
	goto_if_lt VAR_ICE_STEP_COUNT, 28, SootopolisCity_Gym_1F_EventScript_OpenFirstStairs
	goto_if_lt VAR_ICE_STEP_COUNT, 67, SootopolisCity_Gym_1F_EventScript_OpenFirstAndSecondStairs
	setmetatile 8, 4, METATILE_SootopolisGym_Stairs, FALSE
	setmetatile 8, 5, METATILE_SootopolisGym_Stairs, FALSE
SootopolisCity_Gym_1F_EventScript_OpenFirstAndSecondStairs::
	setmetatile 8, 10, METATILE_SootopolisGym_Stairs, FALSE
	setmetatile 8, 11, METATILE_SootopolisGym_Stairs, FALSE
SootopolisCity_Gym_1F_EventScript_OpenFirstStairs::
	setmetatile 8, 15, METATILE_SootopolisGym_Stairs, FALSE
	setmetatile 8, 16, METATILE_SootopolisGym_Stairs, FALSE
SootopolisCity_Gym_1F_EventScript_StopCheckingStairs::
	return

SootopolisCity_Gym_1F_OnFrame:
	map_script_2 VAR_ICE_STEP_COUNT, 8, SootopolisCity_Gym_1F_EventScript_UnlockFirstStairs
	map_script_2 VAR_ICE_STEP_COUNT, 28, SootopolisCity_Gym_1F_EventScript_UnlockSecondStairs
	map_script_2 VAR_ICE_STEP_COUNT, 67, SootopolisCity_Gym_1F_EventScript_UnlockThirdStairs
	map_script_2 VAR_ICE_STEP_COUNT, 0, SootopolisCity_Gym_1F_EventScript_FallThroughIce
	.2byte 0

SootopolisCity_Gym_1F_EventScript_UnlockFirstStairs::
	addvar VAR_ICE_STEP_COUNT, 1
	delay 40
	playse SE_ICE_STAIRS
	call SootopolisCity_Gym_1F_EventScript_CheckSetStairMetatiles
	special DrawWholeMapView
	end

SootopolisCity_Gym_1F_EventScript_UnlockSecondStairs::
	addvar VAR_ICE_STEP_COUNT, 1
	delay 40
	playse SE_ICE_STAIRS
	call SootopolisCity_Gym_1F_EventScript_CheckSetStairMetatiles
	special DrawWholeMapView
	end

SootopolisCity_Gym_1F_EventScript_UnlockThirdStairs::
	addvar VAR_ICE_STEP_COUNT, 1
	delay 40
	playse SE_ICE_STAIRS
	call SootopolisCity_Gym_1F_EventScript_CheckSetStairMetatiles
	special DrawWholeMapView
	end

SootopolisCity_Gym_1F_EventScript_FallThroughIce::
	lockall
	delay 20
	applymovement OBJ_EVENT_ID_PLAYER, SootopolisCity_Gym_1F_Movement_FallThroughIce
	waitmovement 0
	playse SE_FALL
	delay 60
	warphole MAP_SOOTOPOLIS_CITY_GYM_B1F
	waitstate
	end

SootopolisCity_Gym_1F_Movement_FallThroughIce:
	set_invisible
	step_end

SootopolisCity_Gym_1F_EventScript_Juan::
	goto_if_unset FLAG_SYS_GAME_CLEAR, SootopolisCity_Gym_1F_EventScript_Wallace
	trainerbattle_single TRAINER_JUAN_1, SootopolisCity_Gym_1F_Text_JuanIntro, SootopolisCity_Gym_1F_Text_JuanDefeat, SootopolisCity_Gym_1F_EventScript_JuanDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, SootopolisCity_Gym_1F_EventScript_JuanRematch
	msgbox SootopolisCity_Gym_1F_Text_JuanPostBattle, MSGBOX_DEFAULT
	release
	end

SootopolisCity_Gym_1F_EventScript_Wallace::
	trainerbattle_single TRAINER_WALLACE_LEADER, SootopolisCity_Gym_1F_Text_WallaceIntro, SootopolisCity_Gym_1F_Text_WallaceDefeat, SootopolisCity_Gym_1F_EventScript_WallaceDefeated, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_WATER_PULSE, SootopolisCity_Gym_1F_EventScript_GiveWaterPulse2
	goto_if_unset FLAG_BADGE06_GET, SootopolisCity_Gym_1F_EventScript_GoGetFortreeBadge
	msgbox SootopolisCity_Gym_1F_Text_JuanPostBattle, MSGBOX_DEFAULT
	release
	end

SootopolisCity_Gym_1F_EventScript_WallaceDefeated::
	message SootopolisCity_Gym_1F_Text_ReceivedRainBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox SootopolisCity_Gym_1F_Text_ExplainRainBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_SOOTOPOLIS_GYM
	setflag FLAG_BADGE08_GET
	setflag FLAG_HIDE_SOOTOPOLIS_CITY_RESIDENTS
	setflag FLAG_HIDE_SOOTOPOLIS_CITY_STEVEN
	setvar VAR_SOOTOPOLIS_CITY_STATE, 6
	clearflag FLAG_HIDE_SOOTOPOLIS_CITY_MAN_1
	setvar VAR_0x8008, 8
	call Common_EventScript_SetGymTrainers
	call SootopolisCity_Gym_1F_EventScript_GiveWaterPulse
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox SootopolisCity_Gym_1F_Text_RegisteredWallace, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_WALLACE_MATCH_CALL
	release
	end

SootopolisCity_Gym_1F_EventScript_JuanDefeated::
	msgbox SootopolisCity_Gym_1F_Text_RewardThisTicket, MSGBOX_DEFAULT
	giveitem ITEM_AURORA_TICKET
	setflag FLAG_ENABLE_SHIP_BIRTH_ISLAND
	setflag FLAG_RECEIVED_AURORA_TICKET
	msgbox SootopolisCity_Gym_1F_EventScript_StrangeThing, MSGBOX_DEFAULT
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox SootopolisCity_Gym_1F_Text_RegisteredJuan, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_JUAN_MATCH_CALL
	release
	end

SootopolisCity_Gym_1F_EventScript_GiveWaterPulse::
	giveitem ITEM_TM_WATER_PULSE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	msgbox SootopolisCity_Gym_1F_Text_ExplainWaterPulse, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_WATER_PULSE
	return

SootopolisCity_Gym_1F_EventScript_GiveWaterPulse2::
	giveitem ITEM_TM_WATER_PULSE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox SootopolisCity_Gym_1F_Text_ExplainWaterPulse, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_WATER_PULSE
	release
	end

SootopolisCity_Gym_1F_EventScript_GoGetFortreeBadge::
	msgbox SootopolisCity_Gym_1F_Text_GoGetFortreeBadge, MSGBOX_DEFAULT
	release
	end

SootopolisCity_Gym_1F_EventScript_JuanRematch::
	trainerbattle_rematch_double TRAINER_JUAN_1, SootopolisCity_Gym_1F_Text_JuanPreRematch, SootopolisCity_Gym_1F_Text_JuanRematchDefeat, SootopolisCity_Gym_1F_Text_JuanRematchNeedTwoMons
	msgbox SootopolisCity_Gym_1F_Text_JuanPostRematch, MSGBOX_AUTOCLOSE
	end

SootopolisCity_Gym_1F_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_SOOTOPOLIS_GYM, SootopolisCity_Gym_1F_EventScript_GymGuidePostVictory
	msgbox SootopolisCity_Gym_1F_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

SootopolisCity_Gym_1F_EventScript_GymGuidePostVictory::
	msgbox SootopolisCity_Gym_1F_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

SootopolisCity_Gym_1F_EventScript_GymStatue::
	lockall
	goto_if_set FLAG_READ_STEVENS_LETTER, SootopolisCity_Gym_1F_EventScript_GymStatueCertifiedPostgame
	goto_if_set FLAG_BADGE08_GET, SootopolisCity_Gym_1F_EventScript_GymStatueCertified
	msgbox SootopolisCity_Gym_1F_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

SootopolisCity_Gym_1F_EventScript_GymStatueCertifiedPostgame::
	msgbox SootopolisCity_Gym_1F_Text_GymStatueCertifiedPostgame, MSGBOX_DEFAULT
	releaseall
	end

SootopolisCity_Gym_1F_EventScript_GymStatueCertified::
	msgbox SootopolisCity_Gym_1F_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end


SootopolisCity_Gym_1F_Text_GymGuideAdvice:
	.string "Yo! How's it going, CHAMPION-\n"
	.string "bound {PLAYER}?\p"
	.string "SOOTOPOLIS's GYM LEADER WALLACE is\n"
	.string "a master of WATER-type POKéMON.\p"
	.string "And, to get to WALLACE, an icy floor\n"
	.string "will hamper your progress…\p"
	.string "Listen, I'm sorry, but that's all the\n"
	.string "advice that I have for you.\p"
	.string "The rest of the way, you have to\n"
	.string "go for it yourself!$"

SootopolisCity_Gym_1F_Text_GymGuidePostVictory:
	.string "Yow! You've beaten even WALLACE, who\n"
	.string "was supposedly the best in all HOENN!\p"
	.string "Okay! Check out your TRAINER CARD.\p"
	.string "If you've gotten all the BADGES, you're\n"
	.string "set for the POKéMON LEAGUE challenge!$"
	
SootopolisCity_Gym_1F_Text_WallaceIntro:
	.string "Allow me to formally introduce myself.\p"
	.string "I am WALLACE.\n"
	.string "The GYM LEADER of SOOTOPOLIS CITY.\p"
	.string "That incident…\n"
	.string "That was superb work, putting an end\l"
	.string "to that crisis all by yourself.\p"
	.string "Oops!\p"
	.string "It wouldn't be fair to say that you\n"
	.string "alone ended the crisis.\p"
	.string "You overcame that difficult situation\n"
	.string "by working as one with your POKéMON.\p"
	.string "Now, show me. Show me the power you\n"
	.string "wield with your POKéMON.\p"
	.string "And I, in turn, shall present you with\n"
	.string "a performance of illusions in water\l"
	.string "by me and my POKéMON!$"

SootopolisCity_Gym_1F_Text_JuanIntro:
	.string "Let me ask you.\n"
	.string "Did you know?\l"
	.string "Ah, I should not be so coy.\p"
	.string "It was I, JUAN, who taught WALLACE\n"
	.string "everything there is to know about\l"
	.string "POKéMON.\p"
	.string "Once, I had given up my position as\n"
	.string "the GYM LEADER.\p"
	.string "In my place, I had entrusted WALLACE\n"
	.string "with the GYM.\p"
	.string "However, a compelling reason arose for\n"
	.string "me to make a comeback.\p"
	.string "Ah, but enough chatter.\n"
	.string "Let us begin our match, shall we?\p"
	.string "Please, you shall bear witness to\n"
	.string "our artistry.\p"
	.string "A grand illusion of water sculpted\n"
	.string "by POKéMON and myself!$"
	
SootopolisCity_Gym_1F_Text_WallaceDefeat:
	.string "Bravo.\p"
	.string "I realize now your authenticity and\n"
	.string "magnificence as a POKéMON TRAINER.\p"
	.string "I find much joy in having met you and\n"
	.string "your POKéMON.\p"
	.string "You have proven yourself worthy of\n"
	.string "the RAIN BADGE. Accept it.$"

@ NOTE: This defeat text actually causes a buffer overflow. It's about 50 bytes too long for
@ the gDisplayedStringBattle buffer that it's put into, and it stomps all over the gBattleTextBuffs
@ after, as well as the otherwise unused array after that. One wonders if that's the reason for
@ the existence of that unused array of ints.
SootopolisCity_Gym_1F_Text_JuanDefeat:
	.string "Ahahaha, excellent!\n"
	.string "Very well, you are the winner.\p"
	.string "From you, I sense the brilliant shine\n"
	.string "of skill that will overcome all.\p"
	.string "However, compared with me or even\n"
	.string "WALLACE, you are lacking in elegance.\p"
	.string "Perhaps I should make you a loan\n"
	.string "of my outfit?\p"
	.string "… … … … … …\n"
	.string "… … … … … …\p"
	.string "Hahaha, I merely jest!$"

SootopolisCity_Gym_1F_Text_ReceivedRainBadge:
	.string "{PLAYER} received the RAIN BADGE\n"
	.string "from WALLACE.$"

SootopolisCity_Gym_1F_Text_ExplainRainBadgeTakeThis:
	.string "Having the RAIN BADGE shall assure you\n"
	.string "the full obedience of all your POKéMON\l"
	.string "to your every command.\p"
	.string "The RAIN BADGE also allows the use\n"
	.string "of the HM move WATERFALL to scale\l"
	.string "walls of cascading water.\p"
	.string "And, so that you never forget the\n"
	.string "battle we shared, take this…$"

SootopolisCity_Gym_1F_Text_ExplainWaterPulse:
	.string "The TECHNICAL MACHINE I handed you\n"
	.string "contains WATER PULSE.\p"
	.string "In use, it will occasionally confuse\n"
	.string "the target with ultrasonic waves.\p"
	.string "… … … … … …$"

SootopolisCity_Gym_1F_Text_RewardThisTicket:
	.string "Rather than my clothes, I shall reward\n"
	.string "you with this, a peculiar ticket!$"

SootopolisCity_Gym_1F_EventScript_StrangeThing:
	.string "Ah, now what a strange thing to give\n"
	.string "you as a reward, is it not?\p"
	.string "Let me tell you something about it.\p"
	.string "WALLACE left this ticket behind when\n"
	.string "he returned control of the GYM to me.\p"
	.string "He's gathered numerous trinkets in his\n"
	.string "travels, and this is no exception.\p"
	.string "But why leave it? Was it a mistake?\n"
	.string "Did he forget to take it with him?\p"
	.string "Or maybe he knew that you would return\n"
	.string "here and wanted you to have it!\p"
	.string "I'm merely speculating, of course.\p"
	.string "Try using it at the ferry at LILYCOVE\n"
	.string "CITY and see where it takes you.\p"
	.string "… … … … … …$"

SootopolisCity_Gym_1F_Text_RegisteredWallace:
	.string "Registered GYM LEADER WALLACE\n"
	.string "in the POKéNAV.$"

SootopolisCity_Gym_1F_Text_RegisteredJuan:
	.string "Registered GYM LEADER JUAN\n"
	.string "in the POKéNAV.$"

SootopolisCity_Gym_1F_Text_JuanPostBattle:
	.string "The TRAINERS who have gathered all\n"
	.string "the GYM BADGES of HOENN should make\l"
	.string "way to the ultimate destination.\p"
	.string "The POKéMON LEAGUE.\p"
	.string "Travel to the easternmost reaches\n"
	.string "of HOENN, to the island EVER GRANDE.\p"
	.string "There, you shall find the POKéMON\n"
	.string "LEAGUE.$"

SootopolisCity_Gym_1F_Text_GoGetFortreeBadge:
	.string "There remains but one BADGE to\n"
	.string "obtain in HOENN.\p"
	.string "If you wish to challenge the POKéMON\n"
	.string "LEAGUE, you must obtain the last\l"
	.string "BADGE from the GYM in FORTREE.$"

SootopolisCity_Gym_1F_Text_GymStatue:
	.string "SOOTOPOLIS CITY POKéMON GYM$"

SootopolisCity_Gym_1F_Text_GymStatueCertified:
	.string "SOOTOPOLIS CITY POKéMON GYM\p"
	.string "WALLACE'S CERTIFIED TRAINERS:\n"
	.string "{PLAYER}$"
	
SootopolisCity_Gym_1F_Text_GymStatueCertifiedPostgame:
	.string "SOOTOPOLIS CITY POKéMON GYM\p"
	.string "CERTIFIED TRAINERS:\n"
	.string "{PLAYER}$"

SootopolisCity_Gym_1F_Text_JuanPreRematch:
	.string "JUAN: Ah, this GYM had returned to its\n"
	.string "usual state of serenity…\p"
	.string "But our young typhoon has returned\n"
	.string "to put us to the test again!\p"
	.string "Well, my friend, most certainly!\p"
	.string "I shall be delighted to dance with you\n"
	.string "as often as you wish!$"

SootopolisCity_Gym_1F_Text_JuanRematchDefeat:
	.string "Ahahaha, you are the winner!\n"
	.string "You have defeated me again!$"

SootopolisCity_Gym_1F_Text_JuanPostRematch:
	.string "JUAN: If I told you to become my\n"
	.string "apprentice, you will refuse, I am sure.\p"
	.string "I would like to make a gift of my coat\n"
	.string "to you.\p"
	.string "But again, you will refuse.\n"
	.string "I imagine that to be so.\p"
	.string "And that, my friend, is a certain sign\n"
	.string "of nobility!$"

SootopolisCity_Gym_1F_Text_JuanRematchNeedTwoMons:
	.string "JUAN: Ah, this GYM had returned to its\n"
	.string "usual state of serenity…\p"
	.string "But our young typhoon has returned\n"
	.string "to put us to the test again!\p"
	.string "Well, my friend, most certainly!\n"
	.string "I shall be delighted to dance with you…\p"
	.string "Ah, no, no, no.\n"
	.string "You have with you but one POKéMON.\p"
	.string "I wish that you would return with\n"
	.string "two, perhaps more, POKéMON, please.$"

