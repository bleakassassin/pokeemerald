FortreeCity_House1_MapScripts::
	.byte 0

FortreeCity_House1_EventScript_Trader::
	lock
	faceplayer
	goto_if_set FLAG_FORTREE_NPC_TRADE_COMPLETED, FortreeCity_House1_EventScript_TradeCompleted
	setvar VAR_0x8008, INGAME_TRADE_SKITTY
	copyvar VAR_0x8004, VAR_0x8008
	specialvar VAR_RESULT, GetInGameTradeSpeciesInfo
	copyvar VAR_0x8009, VAR_RESULT
	msgbox FortreeCity_House1_Text_YouWillTradeWontYou, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, FortreeCity_House1_EventScript_DeclineTrade
	special ChoosePartyMon
	waitstate
	copyvar VAR_0x800A, VAR_0x8004
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, FortreeCity_House1_EventScript_DeclineTrade
	copyvar VAR_0x8005, VAR_0x800A
	specialvar VAR_RESULT, GetTradeSpecies
	copyvar VAR_0x800B, VAR_RESULT
	goto_if_ne VAR_RESULT, VAR_0x8009, FortreeCity_House1_EventScript_NotRequestedMon
	copyvar VAR_0x8004, VAR_0x8008
	copyvar VAR_0x8005, VAR_0x800A
	special CreateInGameTradePokemon
	special DoInGameTradeScene
	waitstate
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox FortreeCity_House1_Text_MonYouTakeCare, MSGBOX_DEFAULT
	setflag FLAG_FORTREE_NPC_TRADE_COMPLETED
	release
	end

FortreeCity_House1_EventScript_DeclineTrade::
	msgbox FortreeCity_House1_Text_YouWontTradeMe, MSGBOX_DEFAULT
	release
	end

FortreeCity_House1_EventScript_NotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox FortreeCity_House1_Text_ThisIsntAMon, MSGBOX_DEFAULT
	release
	end

FortreeCity_House1_EventScript_TradeCompleted::
	msgbox FortreeCity_House1_Text_GoingToMakeVolbeatStrong, MSGBOX_DEFAULT
	release
	end

FortreeCity_House1_EventScript_ExpertF::
	msgbox FortreeCity_House1_Text_TradingMemoriesWithOthers, MSGBOX_NPC
	end

FortreeCity_House1_EventScript_Zigzagoon::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_ZIGZAGOON, CRY_MODE_NORMAL
	msgbox FortreeCity_House1_Text_Zigzagoon, MSGBOX_DEFAULT
	waitmoncry
	release
	end

FortreeCity_House1_Text_YouWillTradeWontYou:
	.string "Hi, there's this POKéMON that I'm\n"
	.string "looking for.\p"
	.string "I want a {STR_VAR_1}.\n"
	.string "Do you have one?\p"
	.string "Would you trade it for my {STR_VAR_2}?$"

FortreeCity_House1_Text_MonYouTakeCare:
	.string "Thank you!\p"
	.string "I'll be good to your {STR_VAR_1},\n"
	.string "so please be nice to my {STR_VAR_2}!$"

FortreeCity_House1_Text_ThisIsntAMon:
	.string "Wait!\n"
	.string "That POKéMON isn't a {STR_VAR_1}.\p"
	.string "I guess you don't want to trade\n"
	.string "with me.$"

FortreeCity_House1_Text_YouWontTradeMe:
	.string "Oh, okay. If you change your mind,\n"
	.string "please come see me.\p"
	.string "I'll be waiting!$"

FortreeCity_House1_Text_GoingToMakeVolbeatStrong:
	.string "Thank you for trading with me!\p"
	.string "I really wanted this POKéMON, so\n"
	.string "I promise to take good care of it!$"

FortreeCity_House1_Text_TradingMemoriesWithOthers:
	.string "Trading POKéMON with others…\p"
	.string "It's as if you're trading your own\n"
	.string "memories with other people.$"

FortreeCity_House1_Text_Zigzagoon:
	.string "ZIGZAGOON: Gumomoh?$"

