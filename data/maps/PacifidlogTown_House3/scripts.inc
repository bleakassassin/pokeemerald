PacifidlogTown_House3_MapScripts::
	.byte 0

PacifidlogTown_House3_EventScript_Trader::
	lock
	faceplayer
	goto_if_set FLAG_PACIFIDLOG_NPC_TRADE_COMPLETED, PacifidlogTown_House3_EventScript_TradeCompleted
	setvar VAR_0x8008, INGAME_TRADE_CORSOLA
	copyvar VAR_0x8004, VAR_0x8008
	specialvar VAR_RESULT, GetInGameTradeSpeciesInfo
	copyvar VAR_0x8009, VAR_RESULT
	msgbox PacifidlogTown_House3_Text_WillingToTradeIt, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, PacifidlogTown_House3_EventScript_DeclineTrade
	special ChoosePartyMon
	waitstate
	copyvar VAR_0x800A, VAR_0x8004
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, PacifidlogTown_House3_EventScript_DeclineTrade
	copyvar VAR_0x8005, VAR_0x800A
	specialvar VAR_RESULT, GetTradeSpecies
	copyvar VAR_0x800B, VAR_RESULT
	goto_if_ne VAR_RESULT, VAR_0x8009, PacifidlogTown_House3_EventScript_NotRequestedMon
	copyvar VAR_0x8004, VAR_0x8008
	copyvar VAR_0x8005, VAR_0x800A
	special CreateInGameTradePokemon
	special DoInGameTradeScene
	waitstate
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox PacifidlogTown_House3_Text_ItsSubtlyDifferentThankYou, MSGBOX_DEFAULT
	setflag FLAG_PACIFIDLOG_NPC_TRADE_COMPLETED
	release
	end

PacifidlogTown_House3_EventScript_DeclineTrade::
	msgbox PacifidlogTown_House3_Text_NotDesperateOrAnything, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House3_EventScript_NotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox PacifidlogTown_House3_Text_WontAcceptAnyLessThanRealMon, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House3_EventScript_TradeCompleted::
	msgbox PacifidlogTown_House3_Text_ReallyWantedToGetBagon, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House3_EventScript_Girl::
	msgbox PacifidlogTown_House3_Text_IsThatAPokedex, MSGBOX_NPC
	end

PacifidlogTown_House3_Text_WillingToTradeIt:
	.string "Hello, TRAINER. Would you do me a favor\n"
	.string "and trade POKéMON with me?\p"
	.string "Living in PACIFIDLOG, I can only catch\n"
	.string "certain kinds of POKéMON.\p"
	.string "{STR_VAR_1}? Do you have one?\n"
	.string "I'll trade you my {STR_VAR_2} for it.$"

PacifidlogTown_House3_Text_ItsSubtlyDifferentThankYou:
	.string "Wow!\n"
	.string "This {STR_VAR_1} is so fluffy!$"

PacifidlogTown_House3_Text_WontAcceptAnyLessThanRealMon:
	.string "That's no {STR_VAR_1}.$"

PacifidlogTown_House3_Text_NotDesperateOrAnything:
	.string "I'm a little disappointed...$"

PacifidlogTown_House3_Text_ReallyWantedToGetBagon:
	.string "Since we traded, you and me,\n"
	.string "we're friends, aren't we?$"

PacifidlogTown_House3_Text_IsThatAPokedex:
	.string "Is that a POKéDEX?\p"
	.string "Did you get to meet a lot of different\n"
	.string "POKéMON?\p"
	.string "I wish I was like you.$"

