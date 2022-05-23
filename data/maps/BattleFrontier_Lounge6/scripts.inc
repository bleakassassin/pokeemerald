BattleFrontier_Lounge6_MapScripts::
	.byte 0

BattleFrontier_Lounge6_EventScript_Trader::
	lock
	faceplayer
	goto_if_set FLAG_BATTLE_FRONTIER_TRADE_DONE, BattleFrontier_Lounge6_EventScript_TradeCompleted
	setvar VAR_0x8008, INGAME_TRADE_EEVEE
	call BattleFrontier_Lounge6_EventScript_Trader_Part1
	msgbox BattleFrontier_Lounge6_Text_WouldYouLikeToTrade, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_Lounge6_EventScript_DeclineTrade
	call BattleFrontier_Lounge6_EventScript_Trader_Part2
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, BattleFrontier_Lounge6_EventScript_DeclineTrade
	call BattleFrontier_Lounge6_EventScript_Trader_Part3
	goto_if_ne VAR_RESULT, VAR_0x8009, BattleFrontier_Lounge6_EventScript_NotRequestedMon
	call BattleFrontier_Lounge6_EventScript_Trader_Part4
	msgbox BattleFrontier_Lounge6_Text_PromiseIllBeGoodToIt, MSGBOX_DEFAULT
	setflag FLAG_BATTLE_FRONTIER_TRADE_DONE
	release
	end

BattleFrontier_Lounge6_EventScript_Trader_Part1::
	copyvar VAR_0x8004, VAR_0x8008
	specialvar VAR_RESULT, GetInGameTradeSpeciesInfo
	copyvar VAR_0x8009, VAR_RESULT
	return

BattleFrontier_Lounge6_EventScript_Trader_Part2::
	special ChoosePartyMon
	waitstate
	copyvar VAR_0x800A, VAR_0x8004
	return

BattleFrontier_Lounge6_EventScript_Trader_Part3::
	copyvar VAR_0x8005, VAR_0x800A
	specialvar VAR_RESULT, GetTradeSpecies
	copyvar VAR_0x800B, VAR_RESULT
	return

BattleFrontier_Lounge6_EventScript_Trader_Part4::
	copyvar VAR_0x8004, VAR_0x8008
	copyvar VAR_0x8005, VAR_0x800A
	special CreateInGameTradePokemon
	special DoInGameTradeScene
	waitstate
	bufferspeciesname STR_VAR_1, VAR_0x8009
	return

BattleFrontier_Lounge6_EventScript_DeclineTrade::
	msgbox BattleFrontier_Lounge6_Text_WellThatsFineToo, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge6_EventScript_NotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox BattleFrontier_Lounge6_Text_DontTradeForAnythingButMon, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge6_EventScript_TradeCompleted::
	msgbox BattleFrontier_Lounge6_Text_SkittySoMuchCuterThanImagined, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge6_EventScript_Kobe::
	lock
	faceplayer
	goto_if_set FLAG_KOBE_TRADE_DONE, BattleFrontier_Lounge6_EventScript_KobeTradeCompleted
	setvar VAR_0x8008, INGAME_TRADE_FARFETCHD
	call BattleFrontier_Lounge6_EventScript_Trader_Part1
	msgbox BattleFrontier_Lounge6_Text_IllTradeIfYouWant, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_Lounge6_EventScript_KobeDeclineTrade
	call BattleFrontier_Lounge6_EventScript_Trader_Part2
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, BattleFrontier_Lounge6_EventScript_KobeDeclineTrade
	call BattleFrontier_Lounge6_EventScript_Trader_Part3
	goto_if_ne VAR_RESULT, VAR_0x8009, BattleFrontier_Lounge6_EventScript_KobeNotRequestedMon
	call BattleFrontier_Lounge6_EventScript_Trader_Part4
	msgbox BattleFrontier_Lounge6_Text_PleaseBeGoodToMyPokemon, MSGBOX_DEFAULT
	setflag FLAG_KOBE_TRADE_DONE
	release
	end

BattleFrontier_Lounge6_EventScript_KobeDeclineTrade::
	msgbox BattleFrontier_Lounge6_Text_YouDontWantToThatsOkay, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge6_EventScript_KobeNotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox BattleFrontier_Lounge6_Text_DoesntLookLikeMonToMe, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge6_EventScript_KobeTradeCompleted::
	msgbox BattleFrontier_Lounge6_Text_AnyPokemonCanBeCute, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge6_EventScript_Roman::
	lock
	faceplayer
	goto_if_set FLAG_ROMAN_TRADE_DONE, BattleFrontier_Lounge6_EventScript_RomanTradeCompleted
	setvar VAR_0x8008, INGAME_TRADE_SMOOCHUM
	call BattleFrontier_Lounge6_EventScript_Trader_Part1
	msgbox BattleFrontier_Lounge6_Text_YouWillTradeWontYou, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_Lounge6_EventScript_RomanDeclineTrade
	call BattleFrontier_Lounge6_EventScript_Trader_Part2
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, BattleFrontier_Lounge6_EventScript_RomanDeclineTrade
	call BattleFrontier_Lounge6_EventScript_Trader_Part3
	goto_if_ne VAR_RESULT, VAR_0x8009, BattleFrontier_Lounge6_EventScript_RomanNotRequestedMon
	call BattleFrontier_Lounge6_EventScript_Trader_Part4
	msgbox BattleFrontier_Lounge6_Text_MonYouTakeCare, MSGBOX_DEFAULT
	setflag FLAG_ROMAN_TRADE_DONE
	release
	end

BattleFrontier_Lounge6_EventScript_RomanDeclineTrade::
	msgbox BattleFrontier_Lounge6_Text_YouWontTradeMe, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge6_EventScript_RomanNotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox BattleFrontier_Lounge6_Text_ThisIsntAMon, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge6_EventScript_RomanTradeCompleted::
	msgbox BattleFrontier_Lounge6_Text_GoingToMakeVolbeatStrong, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge6_EventScript_Skylar::
	lock
	faceplayer
	goto_if_set FLAG_SKYLAR_TRADE_DONE, BattleFrontier_Lounge6_EventScript_SkylarTradeCompleted
	setvar VAR_0x8008, INGAME_TRADE_DRATINI
	call BattleFrontier_Lounge6_EventScript_Trader_Part1
	msgbox BattleFrontier_Lounge6_Text_WillingToTradeIt, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_Lounge6_EventScript_SkylarDeclineTrade
	call BattleFrontier_Lounge6_EventScript_Trader_Part2
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, BattleFrontier_Lounge6_EventScript_SkylarDeclineTrade
	call BattleFrontier_Lounge6_EventScript_Trader_Part3
	goto_if_ne VAR_RESULT, VAR_0x8009, BattleFrontier_Lounge6_EventScript_SkylarNotRequestedMon
	call BattleFrontier_Lounge6_EventScript_Trader_Part4
	msgbox BattleFrontier_Lounge6_Text_ItsSubtlyDifferentThankYou, MSGBOX_DEFAULT
	setflag FLAG_SKYLAR_TRADE_DONE
	release
	end

BattleFrontier_Lounge6_EventScript_SkylarDeclineTrade::
	msgbox BattleFrontier_Lounge6_Text_NotDesperateOrAnything, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge6_EventScript_SkylarNotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox BattleFrontier_Lounge6_Text_WontAcceptAnyLessThanRealMon, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge6_EventScript_SkylarTradeCompleted::
	msgbox BattleFrontier_Lounge6_Text_ReallyWantedToGetBagon, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge6_Text_WouldYouLikeToTrade:
	.string "My Pokémon is an {STR_VAR_2}.\n"
	.string "Do you know it?\l"
	.string "It's quite cute and rather nice.\p"
	.string "This little one, I could trade with\n"
	.string "pride!\p"
	.string "Would you like to trade me a {STR_VAR_1}\n"
	.string "for my {STR_VAR_2}?$"

BattleFrontier_Lounge6_Text_PromiseIllBeGoodToIt:
	.string "Oh, it's adorable!\n"
	.string "Thank you!\l"
	.string "I promise I'll be good to it!\p"
	.string "Oh! I hope you'll be good to\n"
	.string "my {STR_VAR_2}, too!$"

BattleFrontier_Lounge6_Text_DontTradeForAnythingButMon:
	.string "Oh, I'm sorry!\n"
	.string "I don't intend to trade for anything\l"
	.string "but a {STR_VAR_1}.$"

BattleFrontier_Lounge6_Text_WellThatsFineToo:
	.string "Oh, you won't?\n"
	.string "Well, that's fine, too.\l"
	.string "Please come visit us again.$"

BattleFrontier_Lounge6_Text_SkittySoMuchCuterThanImagined:
	.string "Giggle!\n"
	.string "A Pikachu is so much cuter than I had\l"
	.string "imagined. I'm delighted!$"

BattleFrontier_Lounge6_Text_IllTradeIfYouWant:
	.string "Huh? My Pokémon is cute?\n"
	.string "Sure, I knew that.\p"
	.string "But if you really want, I'm willing\n"
	.string "to trade it to you.\p"
	.string "I'll trade you my {STR_VAR_2} for\n"
	.string "a {STR_VAR_1} if you want.$"

BattleFrontier_Lounge6_Text_PleaseBeGoodToMyPokemon:
	.string "Eheheh…\n"
	.string "Please be good to my Pokémon.$"

BattleFrontier_Lounge6_Text_DoesntLookLikeMonToMe:
	.string "Huh? That doesn't look anything like\n"
	.string "a {STR_VAR_1} to me.$"

BattleFrontier_Lounge6_Text_YouDontWantToThatsOkay:
	.string "Oh, if you don't want to, that's okay.\n"
	.string "But my Pokémon is cute, you know…$"

BattleFrontier_Lounge6_Text_AnyPokemonCanBeCute:
	.string "Any Pokémon can be cute if you raise\n"
	.string "it with care and kindness.$"

BattleFrontier_Lounge6_Text_YouWillTradeWontYou:
	.string "Wrooooaaar! I need it!\n"
	.string "I have to get me a {STR_VAR_1}!\l"
	.string "I'll do anything for it!\p"
	.string "…Uh… Did you hear that?\n"
	.string "My shout from the bottom of my heart?\p"
	.string "Having heard that, you will trade\n"
	.string "your {STR_VAR_1} for my {STR_VAR_2},\l"
	.string "won't you?$"

BattleFrontier_Lounge6_Text_MonYouTakeCare:
	.string "Oh, yeah, right on!\p"
	.string "{STR_VAR_1}, welcome!\n"
	.string "{STR_VAR_2}, you take care!$"

BattleFrontier_Lounge6_Text_ThisIsntAMon:
	.string "Uh, no, I don't think so.\n"
	.string "That isn't a {STR_VAR_1}.$"

BattleFrontier_Lounge6_Text_YouWontTradeMe:
	.string "No? You won't trade me?\n"
	.string "Even after I bared my heart to you?$"

BattleFrontier_Lounge6_Text_GoingToMakeVolbeatStrong:
	.string "I'm going to make Volbeat super\n"
	.string "strong from this moment on!\p"
	.string "I hope you do the same with Smoochum!$"

BattleFrontier_Lounge6_Text_WillingToTradeIt:
	.string "Check out this {STR_VAR_2}!\p"
	.string "It's the {STR_VAR_2} that I caught\n"
	.string "yesterday to celebrate my birthday!\p"
	.string "Oh, I can see that you want it!\n"
	.string "After all, it's priceless!\p"
	.string "I'll tell you what. I might be willing\n"
	.string "to trade it for a {STR_VAR_1}.$"

BattleFrontier_Lounge6_Text_ItsSubtlyDifferentThankYou:
	.string "Oh, so this is a {STR_VAR_1}?\p"
	.string "It's sort of like a {STR_VAR_2},\n"
	.string "and yet it's subtly different.\p"
	.string "Thank you!$"

BattleFrontier_Lounge6_Text_WontAcceptAnyLessThanRealMon:
	.string "No, no, no! I won't accept any\n"
	.string "less than a real {STR_VAR_1}!$"

BattleFrontier_Lounge6_Text_NotDesperateOrAnything:
	.string "Oh, so you're not going to go through\n"
	.string "with this?\p"
	.string "That's cool. I'm not desperate to make\n"
	.string "a trade or anything.$"

BattleFrontier_Lounge6_Text_ReallyWantedToGetBagon:
	.string "I know I could go looking for one\n"
	.string "on my own, but…\p"
	.string "But I really wanted to get a Bagon\n"
	.string "that another Trainer caught…$"
