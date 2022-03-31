NavelRock_Fork_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, NavelRock_Fork_OnTransition
	.byte 0

NavelRock_Fork_OnTransition:
	call_if_set FLAG_LEGENDARY_BEASTS_ROAMING, NavelRock_Fork_EventScript_StopScript
	end

NavelRock_Fork_EventScript_StopScript::
	setvar VAR_TEMP_1, 1
	return

NavelRock_Fork_EventScript_LegendaryBeasts::
	lockall
	setvar VAR_TEMP_1, 1
	setflag FLAG_LEGENDARY_BEASTS_ROAMING
	fadeoutbgm 0
	delay 60
	special DoSealedChamberShakingEffect_Long
	playse SE_M_EARTHQUAKE
	waitstate
	delay 40
	call_if_unset FLAG_SYS_LEGENDARY_BEASTS_FIRST_TRIGGER, NavelRock_Fork_EventScript_LegendaryBeastsFirstTrigger
	call_if_set FLAG_DEFEATED_ROAMING_RAIKOU, NavelRock_Fork_EventScript_Raikou
	call_if_set FLAG_DEFEATED_ROAMING_ENTEI, NavelRock_Fork_EventScript_Entei
	call_if_set FLAG_DEFEATED_ROAMING_SUICUNE, NavelRock_Fork_EventScript_Suicune
	delay 30
	msgbox NavelRock_Fork_Text_EscapedFromCave, MSGBOX_DEFAULT
	fadeinbgm 1
	releaseall
	end

NavelRock_Fork_EventScript_LegendaryBeastsFirstTrigger::
	call NavelRock_Fork_EventScript_Raikou
	call NavelRock_Fork_EventScript_Entei
	call NavelRock_Fork_EventScript_Suicune
	setflag FLAG_SYS_LEGENDARY_BEASTS_FIRST_TRIGGER
	return

NavelRock_Fork_EventScript_Raikou::
	clearflag FLAG_DEFEATED_ROAMING_RAIKOU
	setvar VAR_0x8003, 0
	special InitRoamer
	call NavelRock_Fork_EventScript_Shake
	playmoncry SPECIES_RAIKOU, CRY_MODE_ENCOUNTER
	waitmoncry
	return

NavelRock_Fork_EventScript_Entei::
	clearflag FLAG_DEFEATED_ROAMING_ENTEI
	setvar VAR_0x8003, 1
	special InitRoamer
	call NavelRock_Fork_EventScript_Shake
	playmoncry SPECIES_ENTEI, CRY_MODE_ENCOUNTER
	waitmoncry
	return

NavelRock_Fork_EventScript_Suicune::
	clearflag FLAG_DEFEATED_ROAMING_SUICUNE
	setvar VAR_0x8003, 2
	special InitRoamer
	call NavelRock_Fork_EventScript_Shake
	playmoncry SPECIES_SUICUNE, CRY_MODE_ENCOUNTER
	waitmoncry
	return

NavelRock_Fork_EventScript_Shake::
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 1  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	waitstate
	return

NavelRock_Fork_Text_EscapedFromCave:
	.string "It sounds like something escaped from\n"
	.string "the cave!$"
