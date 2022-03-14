MysteryGiftScript_MysticTicket::
	setvaddress MysteryGiftScript_MysticTicket
	lock
	faceplayer
	vgoto_if_set FLAG_RECEIVED_MYSTIC_TICKET, MysticTicket_Obtained
	removeitem ITEM_MYSTIC_TICKET
	vmessage MysteryGift_Text_TheresATicketForYouShort
	waitmessage
	waitbuttonpress
	checkitemspace ITEM_MYSTIC_TICKET
	vgoto_if_eq VAR_RESULT, FALSE, MysticTicket_NoBagSpace
	goto_if_set FLAG_EXCHANGED_SCANNER, MysticTicket_Give
	setvar VAR_TEMP_1, 1
MysticTicket_Give:
	setvar VAR_TEMP_0, 1
	clearflag FLAG_EXCHANGED_SCANNER
	giveitem ITEM_MYSTIC_TICKET
	setflag FLAG_RECEIVED_MYSTIC_TICKET
MysticTicket_Obtained:
	vmessage MysteryGift_Text_TryUsingItAtLilycovePort
	waitmessage
	waitbuttonpress
	release
	end

MysticTicket_NoBagSpace:
	vmessage MysteryGift_Text_BagFull
	waitmessage
	waitbuttonpress
	release
	end
