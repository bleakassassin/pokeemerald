SeafloorCavern_Room8_MapScripts:: @ 8234DC7
	map_script MAP_SCRIPT_ON_TRANSITION, SeafloorCavern_Room8_OnTransition
	.byte 0

SeafloorCavern_Room8_OnTransition:
	call_if_set FLAG_HIDE_SEAFLOOR_CAVERN_AQUA_GRUNTS, SeafloorCavern_Room8_EventScript_SetBoulders
	end
	
SeafloorCavern_Room8_EventScript_SetBoulders:
	setobjectxyperm 1, 3, 7
	setobjectxyperm 2, 6, 4
	setobjectxyperm 3, 7, 7
	setobjectxyperm 4, 3, 6
	setobjectxyperm 5, 7, 6
	setobjectxyperm 6, 3, 5
	setobjectxyperm 7, 4, 4
	setobjectxyperm 8, 7, 5
	setobjectxyperm 9, 3, 4
	setobjectxyperm 10, 4, 3
	setobjectxyperm 11, 6, 3
	setobjectxyperm 12, 7, 4
	return	
