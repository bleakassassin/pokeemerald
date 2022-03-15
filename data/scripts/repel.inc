EventScript_RepelWoreOff::
	lockall
	checkitem VAR_REPEL_LAST_USED, 1
	goto_if_eq VAR_RESULT, FALSE, EventScript_RepelWoreOff_NoMoreRepels
	msgbox Text_RepelWoreOff_UseAnother, MSGBOX_YESNO
	closemessage
	goto_if_eq VAR_RESULT, 0, EventScript_RepelWoreOff_End
	copyvar VAR_0x8004, VAR_REPEL_LAST_USED
	callnative ItemId_GetHoldEffectParam_Script
	copyvar VAR_REPEL_STEP_COUNT, VAR_RESULT
	bufferitemname 1, VAR_REPEL_LAST_USED
	removeitem VAR_REPEL_LAST_USED, 1
	playse SE_REPEL
	waitse
	msgbox gText_PlayerUsedVar2_ow, MSGBOX_SIGN
	releaseall
	end
	
EventScript_RepelWoreOff_NoMoreRepels:
	msgbox Text_RepelWoreOff, MSGBOX_SIGN
EventScript_RepelWoreOff_End:
	releaseall
	end

Text_RepelWoreOff:
	.string "Repel's effect wore off!$"

Text_RepelWoreOff_UseAnother:
	.string "Repel's effect wore off!\n"
	.string "Use another?$"
