.set LOCALID_GRUNT_1, 10
.set LOCALID_GRUNT_2, 11
.set LOCALID_GRUNT_3, 12
.set LOCALID_GRUNT_4, 13
.set LOCALID_MAXIE, 14
.set LOCALID_SCOTT, 16

MossdeepCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MossdeepCity_OnTransition
	.byte 0

MossdeepCity_OnTransition:
	clearflag FLAG_MOSSDEEP_GYM_SWITCH_1
	clearflag FLAG_MOSSDEEP_GYM_SWITCH_2
	clearflag FLAG_MOSSDEEP_GYM_SWITCH_3
	clearflag FLAG_MOSSDEEP_GYM_SWITCH_4
	call_if_set FLAG_SYS_WEATHER_CTRL, Common_EventScript_SetAbnormalWeather
	end

MossdeepCity_EventScript_PokefanF::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_HM_DIVE, MossdeepCity_EventScript_PokefanFMagmaGone
	msgbox MossdeepCity_Text_SpaceCenterReceivedLetter, MSGBOX_DEFAULT
	release
	end

MossdeepCity_EventScript_PokefanFMagmaGone::
	msgbox MossdeepCity_Text_SpaceCenterLaunchingRockets, MSGBOX_DEFAULT
	release
	end

MossdeepCity_EventScript_Sailor::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_HM_DIVE, MossdeepCity_EventScript_SailorMagmaGone
	msgbox MossdeepCity_Text_MossdeepTargetedByMagma, MSGBOX_DEFAULT
	release
	end

MossdeepCity_EventScript_SailorMagmaGone::
	msgbox MossdeepCity_Text_FeelReliefOnLand, MSGBOX_DEFAULT
	release
	end

MossdeepCity_EventScript_NinjaBoy::
	msgbox MossdeepCity_Text_WailmerWatching, MSGBOX_NPC
	end

MossdeepCity_EventScript_ExpertM::
	msgbox MossdeepCity_Text_LifeNeedsSeaToLive, MSGBOX_NPC
	end

MossdeepCity_EventScript_Girl::
	msgbox MossdeepCity_Text_NiceIfWorldCoveredByFlowers, MSGBOX_NPC
	end

MossdeepCity_EventScript_Woman::
	lock
	faceplayer
	goto_if_eq VAR_JIRACHI_WISH_STATE, 4, MossdeepCity_EventScript_WomanKeepMakingWishes
	goto_if_eq VAR_JIRACHI_WISH_STATE, 3, MossdeepCity_EventScript_ReceiveJirachi
	goto_if_eq VAR_JIRACHI_WISH_STATE, 2, MossdeepCity_EventScript_WomanCheckWeek
	setvar VAR_JIRACHI_WISH_STATE, 1
MossdeepCity_EventScript_WomanMakeWish::
	msgbox MossdeepCity_Text_SpecialSpaceCenterRock, MSGBOX_DEFAULT
	release
	end

MossdeepCity_EventScript_ReceiveJirachi::
	msgbox MossdeepCity_Text_ShootingStarFellDown, MSGBOX_DEFAULT
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_JIRACHI
	givemon SPECIES_JIRACHI, 5
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, MossdeepCity_EventScript_ReceiveJirachiParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, MossdeepCity_EventScript_ReceiveJirachiPC
	goto Common_EventScript_NoMoreRoomForPokemon

MossdeepCity_EventScript_ReceiveJirachiParty::
	call MossdeepCity_EventScript_ReceivedJirachiFanfare
MossdeepCity_EventScript_WomanKeepMakingWishes::
	msgbox MossdeepCity_Text_KeepMakingWishes, MSGBOX_DEFAULT
	release
	end

MossdeepCity_EventScript_ReceiveJirachiPC::
	call MossdeepCity_EventScript_ReceivedJirachiFanfare
	call Common_EventScript_TransferredToPC
	goto MossdeepCity_EventScript_WomanKeepMakingWishes

MossdeepCity_EventScript_ReceivedJirachiFanfare::
	playfanfare MUS_EVOLVED
	setvar VAR_JIRACHI_WISH_STATE, 4
	message MossdeepCity_EventScript_PlayerReceivedJirachi
	waitmessage
	waitfanfare
	bufferspeciesname STR_VAR_1, SPECIES_JIRACHI
	return

MossdeepCity_EventScript_WomanCheckWeek::
	dotimebasedevents
	specialvar VAR_RESULT, GetWeekCount
	goto_if_gt VAR_RESULT, VAR_WEEK_TOTAL_WHEN_WISH, MossdeepCity_EventScript_WomanAboutToLaunch
	goto MossdeepCity_EventScript_WomanMakeWish

MossdeepCity_EventScript_WomanAboutToLaunch::
	msgbox MossdeepCity_Text_AboutToLaunch, MSGBOX_DEFAULT
	release
	end

MossdeepCity_EventScript_WhiteRock::
	lock
	goto_if_ge VAR_JIRACHI_WISH_STATE, 3, MossdeepCity_EventScript_WhiteRockDent
	goto_if_eq VAR_JIRACHI_WISH_STATE, 2, MossdeepCity_EventScript_RockCheckWeek
	goto_if_eq VAR_JIRACHI_WISH_STATE, 1, MossdeepCity_EventScript_WhiteRockWish
MossdeepCity_EventScript_WhiteRockDefault::
	msgbox MossdeepCity_Text_ItsAWhiteRock, MSGBOX_DEFAULT
MossdeepCity_EventScript_WhiteRockEnd::
	release
	end

MossdeepCity_EventScript_WhiteRockWish::
	msgbox MossdeepCity_Text_MakeAWish, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MossdeepCity_EventScript_WhiteRockEnd
	setvar VAR_JIRACHI_WISH_STATE, 2
	dotimebasedevents
	specialvar VAR_WEEK_TOTAL_WHEN_WISH, GetWeekCount
	msgbox MossdeepCity_Text_SeeIfItComesTrue, MSGBOX_DEFAULT
	release
	end

MossdeepCity_EventScript_RockCheckWeek::
	dotimebasedevents
	specialvar VAR_RESULT, GetWeekCount
	goto_if_gt VAR_RESULT, VAR_WEEK_TOTAL_WHEN_WISH, MossdeepCity_EventScript_WhiteRockTag
	msgbox MossdeepCity_Text_SeeIfItComesTrueShort, MSGBOX_DEFAULT
	release
	end

MossdeepCity_EventScript_WhiteRockTag::
	msgbox MossdeepCity_Text_TagDisappeared, MSGBOX_DEFAULT
	release
	end

MossdeepCity_EventScript_WhiteRockDent::
	msgbox MossdeepCity_Text_RockHasDent, MSGBOX_DEFAULT
	release
	end

MossdeepCity_EventScript_GymSign::
	msgbox MossdeepCity_Text_GymSign, MSGBOX_SIGN
	end

MossdeepCity_EventScript_CitySign::
	msgbox MossdeepCity_Text_CitySign, MSGBOX_SIGN
	end

MossdeepCity_EventScript_SpaceCenterSign::
	msgbox MossdeepCity_Text_SpaceCenterSign, MSGBOX_SIGN
	end

MossdeepCity_EventScript_VisitedMossdeep::
	setflag FLAG_VISITED_MOSSDEEP_CITY
	setvar VAR_TEMP_1, 1
	end

MossdeepCity_EventScript_TeamMagmaEnterSpaceCenter::
	lockall
	applymovement LOCALID_MAXIE, MossdeepCity_Movement_MaxieGestureToSpaceCenter
	waitmovement 0
	applymovement LOCALID_GRUNT_1, MossdeepCity_Movement_GruntFaceSpaceCenter
	applymovement LOCALID_GRUNT_2, MossdeepCity_Movement_GruntFaceSpaceCenter
	applymovement LOCALID_GRUNT_3, MossdeepCity_Movement_GruntFaceSpaceCenter
	applymovement LOCALID_GRUNT_4, MossdeepCity_Movement_GruntFaceSpaceCenter
	waitmovement 0
	applymovement LOCALID_MAXIE, MossdeepCity_Movement_MaxieEnterSpaceCenter
	applymovement LOCALID_GRUNT_1, MossdeepCity_Movement_Grunt1EnterSpaceCenter
	applymovement LOCALID_GRUNT_2, MossdeepCity_Movement_Grunt2EnterSpaceCenter
	applymovement LOCALID_GRUNT_3, MossdeepCity_Movement_Grunt3EnterSpaceCenter
	applymovement LOCALID_GRUNT_4, MossdeepCity_Movement_Grunt4EnterSpaceCenter
	waitmovement 0
	removeobject LOCALID_MAXIE
	removeobject LOCALID_GRUNT_1
	removeobject LOCALID_GRUNT_2
	removeobject LOCALID_GRUNT_3
	removeobject LOCALID_GRUNT_4
	delay 30
	setvar VAR_MOSSDEEP_CITY_STATE, 2
	setflag FLAG_HIDE_MOSSDEEP_CITY_TEAM_MAGMA
	releaseall
	end

MossdeepCity_Movement_MaxieGestureToSpaceCenter:
	delay_16
	face_right
	delay_16
	delay_16
	delay_16
	delay_16
	face_left
	delay_16
	delay_16
	step_end

MossdeepCity_Movement_GruntFaceSpaceCenter:
	face_right
	delay_16
	delay_16
	delay_16
	step_end

MossdeepCity_Movement_MaxieEnterSpaceCenter:
	walk_down
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

MossdeepCity_Movement_Grunt1EnterSpaceCenter:
	delay_16
	delay_8
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

MossdeepCity_Movement_Grunt2EnterSpaceCenter:
	delay_16
	delay_8
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

MossdeepCity_Movement_Grunt3EnterSpaceCenter:
	delay_16
	delay_8
	walk_down
	walk_right
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

MossdeepCity_Movement_Grunt4EnterSpaceCenter:
	delay_16
	delay_8
	walk_right
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

MossdeepCity_EventScript_Man::
	lock
	faceplayer
	msgbox MossdeepCity_Text_SurfExhilarating, MSGBOX_DEFAULT
	applymovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

MossdeepCity_EventScript_KingsRockBoy::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_KINGS_ROCK, MossdeepCity_EventScript_ReceivedKingsRock
	msgbox MossdeepCity_Text_WantKingsRockStevenGaveMe, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MossdeepCity_EventScript_DeclineKingsRock
	msgbox MossdeepCity_Text_YouCanKeepIt, MSGBOX_DEFAULT
	giveitem ITEM_KINGS_ROCK
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_KINGS_ROCK
	release
	end

MossdeepCity_EventScript_ReceivedKingsRock::
	msgbox MossdeepCity_Text_StevensHouseOverThere, MSGBOX_DEFAULT
	release
	end

MossdeepCity_EventScript_DeclineKingsRock::
	msgbox MossdeepCity_Text_WhatToDoWithWeirdRock, MSGBOX_DEFAULT
	release
	end

MossdeepCity_EventScript_BlackBelt::
	goto_if_set FLAG_READ_STEVENS_LETTER, MossdeepCity_EventScript_SootopolisNewGymLeader
	msgbox MossdeepCity_Text_WavesWashOver, MSGBOX_NPC
	end

MossdeepCity_EventScript_SootopolisNewGymLeader::
	msgbox MossdeepCity_Text_SootopolisNewGymLeader, MSGBOX_NPC
	end

MossdeepCity_EventScript_Scott::
	lock
	faceplayer
	msgbox MossdeepCity_Text_ScottSomethingWrongWithTown, MSGBOX_DEFAULT
	closemessage
	call_if_eq VAR_FACING, DIR_NORTH, MossdeepCity_EventScript_ScottExitNorth
	call_if_eq VAR_FACING, DIR_EAST, MossdeepCity_EventScript_ScottExitEast
	addvar VAR_SCOTT_STATE, 1
	removeobject LOCALID_SCOTT
	release
	end

MossdeepCity_EventScript_ScottExitNorth::
	applymovement OBJ_EVENT_ID_PLAYER, MossdeepCity_Movement_PlayerWatchScottExit
	applymovement LOCALID_SCOTT, MossdeepCity_Movement_ScottExitNorth
	waitmovement 0
	return

MossdeepCity_EventScript_ScottExitEast::
	applymovement OBJ_EVENT_ID_PLAYER, MossdeepCity_Movement_PlayerWatchScottExit
	applymovement LOCALID_SCOTT, MossdeepCity_Movement_ScottExitEast
	waitmovement 0
	return

MossdeepCity_Movement_PlayerWatchScottExit:
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

MossdeepCity_Movement_ScottExitNorth:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MossdeepCity_Movement_ScottExitEast:
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MossdeepCity_Text_WantKingsRockStevenGaveMe:
	.string "I got this from Steven, but I don't\n"
	.string "know what it's good for.\p"
	.string "I think it's called King's Rock.\n"
	.string "Do you want it?$"

MossdeepCity_Text_YouCanKeepIt:
	.string "Why would you want it?\n"
	.string "You're weird.\p"
	.string "You can keep it, but keep it a secret\n"
	.string "from Steven.$"

MossdeepCity_Text_StevensHouseOverThere:
	.string "Steven's house is right over there!$"

MossdeepCity_Text_WhatToDoWithWeirdRock:
	.string "Yeah, you think so, too! What are you\n"
	.string "supposed to do with some weird rock?$"

MossdeepCity_Text_WailmerWatching:
	.string "Around Mossdeep, you can see wild\n"
	.string "Wailmer.\p"
	.string "It's called, uh…\n"
	.string "What was it now…\p"
	.string "Wai, Wai, Wai…\p"
	.string "Wailmer watching!$"

MossdeepCity_Text_SpaceCenterReceivedLetter:
	.string "The island's Space Center has been\n"
	.string "launching huge rockets.\p"
	.string "There's been some kind of an uproar\n"
	.string "over a letter they received recently.$"

MossdeepCity_Text_SpaceCenterLaunchingRockets:
	.string "The island's Space Center has been\n"
	.string "launching huge rockets.$"

MossdeepCity_Text_MossdeepTargetedByMagma:
	.string "I heard from a Sailor buddy that\n"
	.string "Team Aqua set up shop in Lilycove.\p"
	.string "I also heard that someone came along\n"
	.string "and wiped the floor with them!\p"
	.string "But Mossdeep here's been targeted\n"
	.string "by that Team Magma.\p"
	.string "If you want to know what they're up to,\n"
	.string "go visit the Space Center.$"

MossdeepCity_Text_FeelReliefOnLand:
	.string "I'm a Sailor, so the sea's obviously\n"
	.string "more important to me.\p"
	.string "But you know? When I get back on land\n"
	.string "after a long voyage, I do feel relief!$"

MossdeepCity_Text_NiceIfWorldCoveredByFlowers:
	.string "Wouldn't it be nice?\p"
	.string "If the whole world was covered in\n"
	.string "plants and flowers like this island?$"

MossdeepCity_Text_LifeNeedsSeaToLive:
	.string "All life needs the sea to live, even\n"
	.string "though it makes its home on the land.\p"
	.string "Life, having run its course, becomes\n"
	.string "soil and returns to the land.\p"
	.string "And the sea is always connected to\n"
	.string "the land.\p"
	.string "Yes, like the very shoreline here.$"

MossdeepCity_Text_SurfExhilarating:
	.string "A voyage on a ship is fine.\p"
	.string "But crossing the sea with Pokémon\n"
	.string "using Surf…\p"
	.string "Now that's an exhilarating trip!\n"
	.string "Wouldn't you agree, youngster?$"

MossdeepCity_Text_SpecialSpaceCenterRock:
	.string "This rock has a special meaning to\n"
	.string "the people at the Space Center.\p"
	.string "They put it here as their wish for\n"
	.string "their rockets to fly safely.\p"
	.string "When you make a wish, what do you use?\n"
	.string "Do you wish upon a star?\p"
	.string "I use a wish tag to make it happen.\n"
	.string "That's what I do.$"

MossdeepCity_Text_AboutToLaunch:
	.string "The Space Center is about to launch\n"
	.string "another rocket into space.\p"
	.string "I'm going to make another wish when\n"
	.string "it takes flight.$"

MossdeepCity_Text_ShootingStarFellDown:
	.string "A shooting star fell down and struck\n"
	.string "the wishing rock!\p"
	.string "I'm OK, I'm just worried for the star.\n"
	.string "I think it's alive!\p"
	.string "It has your wish tag, too. It looks\n"
	.string "like you literally wished for a star…\p"
	.string "This is your wish come true, then.\n"
	.string "Why don't you take it with you?$"

MossdeepCity_Text_KeepMakingWishes:
	.string "So that star was a Pokémon?\n"
	.string "That's pretty incredible.\p"
	.string "I'm going to keep making wishes to see\n"
	.string "if one of mine will come true.$"

MossdeepCity_EventScript_PlayerReceivedJirachi:
	.string "{PLAYER} received Jirachi!$"

MossdeepCity_Text_ItsAWhiteRock:
	.string "It's a white rock.$"
	
MossdeepCity_Text_MakeAWish:
	.string "It's the Space Center's wishing rock.\n"
	.string "Do you want to make a wish?$"
	
MossdeepCity_Text_SeeIfItComesTrue:
	.string "You use a spare wish tag to wish upon\n"
	.string "a star…\p"
	.string "… … … … … …\n"
	.string "… … … … … …\p"
MossdeepCity_Text_SeeIfItComesTrueShort:
	.string "Let's wait a bit and see if your wish\n"
	.string "comes true!$"

MossdeepCity_Text_TagDisappeared:
	.string "Your wish tag isn't on the rock\n"
	.string "anymore…$"

MossdeepCity_Text_RockHasDent:
	.string "The rock has a dent the size of a\n"
	.string "meteorite.$"

MossdeepCity_Text_GymSign:
	.string "Mossdeep City Pokémon Gym\n"
	.string "Leaders: Liza & Tate\p"
	.string "“The mystic combination!”$"

MossdeepCity_Text_CitySign:
	.string "Mossdeep City\n"
	.string "“Our slogan: Cherish Pokémon!”$"

MossdeepCity_Text_SpaceCenterSign:
	.string "Mossdeep Space Center\n"
	.string "“The closest place to space.”$"

MossdeepCity_Text_ScottSomethingWrongWithTown:
	.string "Scott: {PLAYER}{KUN}, feeling good?\n"
	.string "I'm doing great!\p"
	.string "I'd heard Mossdeep's Gym Leader is\n"
	.string "pretty strong, so I decided to come\l"
	.string "take a look-see for myself.\p"
	.string "But there's something wrong about\n"
	.string "this town.\p"
	.string "People are going on about a warning\n"
	.string "letter and the Space Center…\p"
	.string "I don't think it concerns me in any\n"
	.string "way, though.$"

MossdeepCity_Text_WavesWashOver:
	.string "Ahh… It feels great letting the waves\n"
	.string "wash over my feet…$"

MossdeepCity_Text_SootopolisNewGymLeader:
	.string "Ahh… It feels great letting the waves\n"
	.string "wash over my feet…\p"
	.string "Speaking of the waves, you know that\n"
	.string "island city Sootopolis?\p"
	.string "I think the Gym there had a new\n"
	.string "Leader come in.\p"
	.string "People tell me the new Leader once\n"
	.string "mentored Wallace.$"

