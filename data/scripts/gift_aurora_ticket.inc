MysteryGiftScript_AuroraTicket::
	setvaddress MysteryGiftScript_AuroraTicket
	lock
	faceplayer
	vgoto_if_set FLAG_RECEIVED_AURORA_TICKET, AuroraTicket_Obtained
	removeitem ITEM_AURORA_TICKET
	vmessage MysteryGift_Text_TheresATicketForYouShort
	waitmessage
	waitbuttonpress
	checkitemspace ITEM_AURORA_TICKET
	vgoto_if_eq VAR_RESULT, FALSE, AuroraTicket_NoBagSpace
	goto_if_set FLAG_ENABLE_JUAN_MATCH_CALL, AuroraTicket_Give
	setvar VAR_TEMP_1, 1
AuroraTicket_Give:
	setvar VAR_TEMP_0, 1
	giveitem ITEM_AURORA_TICKET
	setflag FLAG_RECEIVED_AURORA_TICKET
AuroraTicket_Obtained:
	vmessage MysteryGift_Text_TryUsingItAtLilycovePort
	waitmessage
	waitbuttonpress
	release
	end

AuroraTicket_NoBagSpace:
	vmessage MysteryGift_Text_BagFull
	waitmessage
	waitbuttonpress
	release
	end
