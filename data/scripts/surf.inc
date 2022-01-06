EventScript_UseSurf::
	lockall
	msgbox gText_WantToUseSurf, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_ReleaseUseSurf
	checkpartymove MOVE_SURF
	goto_if_eq VAR_RESULT, PARTY_SIZE, EventScript_SurfWildMon
	clearflag FLAG_TEMP_5
	bufferpartymonnick 0, VAR_RESULT
	setfieldeffectargument 0, VAR_RESULT
	msgbox gText_PlayerUsedSurf, MSGBOX_DEFAULT
EventScript_InitSurf::
	closemessage
	dofieldeffect FLDEFF_USE_SURF
	waitstate
EventScript_ReleaseUseSurf::
	releaseall
	end

EventScript_SurfWildMon::
	setflag FLAG_TEMP_5
	setfieldeffectargument 0, SPECIES_WAILMER
	goto EventScript_InitSurf
