EventScript_RepelWoreOff::
	lock
	checkitem VAR_REPEL_LAST_USED, 1
	compare VAR_RESULT, FALSE
	goto_if_eq EventScript_RepelWoreOff_NoMoreRepels
	msgbox Text_RepelWoreOff_UseAnother, MSGBOX_YESNO
	closemessage
	compare VAR_RESULT, 0
	goto_if_eq EventScript_RepelWoreOff_End
	copyvar VAR_0x8004, VAR_REPEL_LAST_USED
	callnative ItemId_GetHoldEffectParam_Script
	copyvar VAR_REPEL_STEP_COUNT, VAR_RESULT
	bufferitemname 1, VAR_REPEL_LAST_USED
	removeitem VAR_REPEL_LAST_USED, 1
	playse SE_REPEL
	waitse
	msgbox gText_PlayerUsedVar2_ow, MSGBOX_SIGN
	end
	
EventScript_RepelWoreOff_NoMoreRepels:
	msgbox Text_RepelWoreOff, MSGBOX_SIGN
EventScript_RepelWoreOff_End:	
	end

Text_RepelWoreOff:
	.string "REPEL's effect wore off!$"

Text_RepelWoreOff_UseAnother:
	.string "REPEL's effect wore off!\n"
	.string "Use another?$"
