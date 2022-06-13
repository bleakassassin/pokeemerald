CaveOfOrigin_B3F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CaveOfOrigin_B3F_OnTransition
	.byte 0

CaveOfOrigin_B3F_OnTransition:
	call_if_eq VAR_SOOTOPOLIS_CITY_STATE, 12, CaveOfOrigin_EventScript_DisableTriggers
	end

