.set LOCALID_SAILOR, 1
.set LOCALID_SS_TIDAL, 2

FarawayIsland_Entrance_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FarawayIsland_Entrance_OnTransition
	.byte 0

FarawayIsland_Entrance_OnTransition:
	setflag FLAG_ARRIVED_ON_FARAWAY_ISLAND
	end

FarawayIsland_Entrance_EventScript_SetCloudsWeather::
	setweather WEATHER_SUNNY_CLOUDS
	doweather
	end

FarawayIsland_Entrance_EventScript_ClearWeather::
	setweather WEATHER_NONE
	doweather
	end

FarawayIsland_Entrance_EventScript_Sailor::
	lock
	faceplayer
	msgbox FarawayIsland_Entrance_Text_SailorReturn, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, FarawayIsland_Entrance_EventScript_AsYouLike
	msgbox EventTicket_Text_SailHome, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	delay 30
	hideobjectat LOCALID_SAILOR, MAP_FARAWAY_ISLAND_ENTRANCE
	setvar VAR_0x8004, LOCALID_SS_TIDAL
	call Common_EventScript_FerryDepartIsland
	warp MAP_LILYCOVE_CITY_HARBOR, 8, 11
	waitstate
	release
	end

FarawayIsland_Entrance_EventScript_AsYouLike::
	msgbox EventTicket_Text_AsYouLike, MSGBOX_DEFAULT
	release
	end

FarawayIsland_Entrance_EventScript_Sign::
	msgbox FarawayIsland_Entrance_Text_Sign, MSGBOX_SIGN
	goto_if_eq VAR_OLD_SEA_MAP_STATE, 6, FarawayIsland_Entrance_EventScript_End
	lockall
	setvar VAR_OLD_SEA_MAP_STATE, 6
	fadeoutbgm 0
	delay 60
	setvar VAR_0x8004, 8  @ num loops
	setvar VAR_0x8005, 35 @ delay
	special LoopWingFlapSE
	delay 60
	playmoncry SPECIES_ARTICUNO, CRY_MODE_ENCOUNTER
	waitmoncry
	playmoncry SPECIES_ZAPDOS, CRY_MODE_ENCOUNTER
	waitmoncry
	playmoncry SPECIES_MOLTRES, CRY_MODE_ENCOUNTER
	waitmoncry
	delay 30
	msgbox FarawayIsland_Entrance_Text_PokemonFlyingAbove, MSGBOX_DEFAULT
	fadeinbgm 1
	releaseall
FarawayIsland_Entrance_EventScript_End::
	end

