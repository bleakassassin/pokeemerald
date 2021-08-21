SeafloorCavern_Room5_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SeafloorCavern_Room5_OnTransition
	.byte 0

SeafloorCavern_Room5_OnTransition::
	call_if_set FLAG_HIDE_SEAFLOOR_CAVERN_AQUA_GRUNTS, SeafloorCavern_Room5_EventScript_SetBoulders

SeafloorCavern_Room5_EventScript_SetBoulders::
	setobjectxyperm 2, 4, 13
	setobjectxyperm 3, 13, 7
	setobjectxyperm 6, 12, 9
	return
