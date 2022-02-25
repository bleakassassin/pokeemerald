FieryPath_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FieryPath_OnTransition
	.byte 0

FieryPath_OnTransition:
	call_if_unset FLAG_LANDMARK_FIERY_PATH, FieryPath_EventScript_MoveScottToFallarbor
	setflag FLAG_LANDMARK_FIERY_PATH
	call_if_set FLAG_ITEM_FIERY_PATH_FIRE_STONE, FieryPath_EventScript_SetBoulders_FireStone
	call_if_set FLAG_ITEM_FIERY_PATH_TM06, FieryPath_EventScript_SetBoulders_TM06
	end

FieryPath_EventScript_SetBoulders_FireStone:
	setobjectxyperm 3, 10, 15
	setobjectxyperm 4, 12, 11
	setobjectxyperm 5, 6, 22
	setobjectxyperm 6, 7, 23
	setobjectxyperm 7, 5, 21

FieryPath_EventScript_SetBoulders_TM06:
	setobjectxyperm 2, 10, 14
	return	

FieryPath_EventScript_MoveScottToFallarbor::
	setflag FLAG_HIDE_VERDANTURF_TOWN_SCOTT
	clearflag FLAG_HIDE_FALLARBOR_TOWN_BATTLE_TENT_SCOTT
	return

