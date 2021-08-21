SeafloorCavern_Room2_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SeafloorCavern_Room2_OnTransition
	.byte 0

SeafloorCavern_Room2_OnTransition::
	call_if_set FLAG_HIDE_SEAFLOOR_CAVERN_AQUA_GRUNTS, SeafloorCavern_Room2_EventScript_SetBoulders
	end

SeafloorCavern_Room2_EventScript_SetBoulders::
	setobjectxyperm 2, 12, 3
	setobjectxyperm 4, 4, 2
	setobjectxyperm 8, 6, 10
	return
