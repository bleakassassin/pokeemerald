BattleFrontier_ExchangeServiceCorner_MapScripts:: @ 825F070
	.byte 0

BattleFrontier_ExchangeServiceCorner_EventScript_ClerkWelcome:: @ 825F071
	msgbox BattleFrontier_ExchangeServiceCorner_Text_WelcomePleaseChoosePrize, MSGBOX_DEFAULT
	special ShowBattlePointsWindow
	return

BattleFrontier_ExchangeServiceCorner_EventScript_ClerkGoodbye:: @ 825F07D
	msgbox BattleFrontier_ExchangeServiceCorner_Text_ThankYouVisitWithPoints, MSGBOX_DEFAULT
	special CloseBattlePointsWindow
	release
	end

BattleFrontier_ExchangeServiceCorner_EventScript_TryPurchasePrize:: @ 825F08A
	specialvar VAR_TEMP_1, GetFrontierBattlePoints
	compare VAR_TEMP_1, VAR_0x8008
	goto_if_ge BattleFrontier_ExchangeServiceCorner_EventScript_TryGivePrize
	msgbox BattleFrontier_ExchangeServiceCorner_Text_DontHaveEnoughPoints, MSGBOX_DEFAULT
	compare VAR_TEMP_2, EXCHANGE_CORNER_DECOR1_CLERK
	goto_if_eq BattleFrontier_ExchangeServiceCorner_EventScript_ChooseDecor1
	compare VAR_TEMP_2, EXCHANGE_CORNER_DECOR2_CLERK
	goto_if_eq BattleFrontier_ExchangeServiceCorner_EventScript_ChooseDecor2Branch
	compare VAR_TEMP_2, EXCHANGE_CORNER_VITAMIN_CLERK
	goto_if_eq BattleFrontier_ExchangeServiceCorner_EventScript_ChooseVitamin
	goto BattleFrontier_ExchangeServiceCorner_EventScript_ChooseHoldItem
	end

BattleFrontier_ExchangeServiceCorner_EventScript_TryGivePrize:: @ 825F0C9
	compare VAR_TEMP_2, EXCHANGE_CORNER_DECOR1_CLERK
	goto_if_eq BattleFrontier_ExchangeServiceCorner_EventScript_TryGiveDecor
	compare VAR_TEMP_2, EXCHANGE_CORNER_DECOR2_CLERK
	goto_if_eq BattleFrontier_ExchangeServiceCorner_EventScript_TryGiveDecor
	goto BattleFrontier_ExchangeServiceCorner_EventScript_TryGiveItem
	end

BattleFrontier_ExchangeServiceCorner_EventScript_TryGiveDecor:: @ 825F0E5
	checkdecorspace VAR_0x8009
	compare VAR_RESULT, FALSE
	goto_if_eq BattleFrontier_ExchangeServiceCorner_EventScript_NoRoomForDecor
	copyvar VAR_0x8004, VAR_0x8008
	special TakeFrontierBattlePoints
	adddecoration VAR_0x8009
	special UpdateBattlePointsWindow
	playse SE_SHOP
	msgbox BattleFrontier_ExchangeServiceCorner_Text_WellSendItToPC, MSGBOX_DEFAULT
	compare VAR_TEMP_2, EXCHANGE_CORNER_DECOR1_CLERK
	goto_if_eq BattleFrontier_ExchangeServiceCorner_EventScript_ChooseDecor1
	goto BattleFrontier_ExchangeServiceCorner_EventScript_ChooseDecor2Branch
	end

BattleFrontier_ExchangeServiceCorner_EventScript_NoRoomForDecor:: @ 825F11D
	msgbox BattleFrontier_ExchangeServiceCorner_Text_PCIsFull, MSGBOX_DEFAULT
	special CloseBattlePointsWindow
	release
	end

BattleFrontier_ExchangeServiceCorner_EventScript_TryGiveItem:: @ 825F12A
	checkitemspace VAR_0x8009, 1
	compare VAR_RESULT, FALSE
	goto_if_eq BattleFrontier_ExchangeServiceCorner_EventScript_BagFull
	copyvar VAR_0x8004, VAR_0x8008
	special TakeFrontierBattlePoints
	additem VAR_0x8009
	special UpdateBattlePointsWindow
	playse SE_SHOP
	msgbox BattleFrontier_ExchangeServiceCorner_Text_HereIsYourPrize, MSGBOX_DEFAULT
	compare VAR_TEMP_2, EXCHANGE_CORNER_VITAMIN_CLERK
	goto_if_eq BattleFrontier_ExchangeServiceCorner_EventScript_ChooseVitamin
	goto BattleFrontier_ExchangeServiceCorner_EventScript_ChooseHoldItem
	end

BattleFrontier_ExchangeServiceCorner_EventScript_BagFull:: @ 825F166
	msgbox BattleFrontier_ExchangeServiceCorner_Text_DontHaveSpaceToHoldIt, MSGBOX_DEFAULT
	special CloseBattlePointsWindow
	release
	end

BattleFrontier_ExchangeServiceCorner_EventScript_DecorClerk1:: @ 825F173
	lock
	faceplayer
	setvar VAR_TEMP_2, EXCHANGE_CORNER_DECOR1_CLERK
	call BattleFrontier_ExchangeServiceCorner_EventScript_ClerkWelcome
	goto BattleFrontier_ExchangeServiceCorner_EventScript_ChooseDecor1
	end

BattleFrontier_ExchangeServiceCorner_EventScript_ChooseDecor1:: @ 825F185
	setvar VAR_0x8004, SCROLL_MULTI_BF_EXCHANGE_CORNER_DECOR_VENDOR_1
	special ShowFrontierExchangeCornerItemIconWindow
	special ShowScrollableMultichoice
	waitstate
	special CloseFrontierExchangeCornerItemIconWindow
	switch VAR_RESULT
	case 0, BattleFrontier_ExchangeServiceCorner_EventScript_KissPoster
	case 1, BattleFrontier_ExchangeServiceCorner_EventScript_KissCushion
	case 2, BattleFrontier_ExchangeServiceCorner_EventScript_SmoochumDoll
	case 3, BattleFrontier_ExchangeServiceCorner_EventScript_TogepiDoll
	case 4, BattleFrontier_ExchangeServiceCorner_EventScript_MeowthDoll
	case 5, BattleFrontier_ExchangeServiceCorner_EventScript_ClefairyDoll
	case 6, BattleFrontier_ExchangeServiceCorner_EventScript_DittoDoll
	case 7, BattleFrontier_ExchangeServiceCorner_EventScript_CyndaquilDoll
	case 8, BattleFrontier_ExchangeServiceCorner_EventScript_ChikoritaDoll
	case 9, BattleFrontier_ExchangeServiceCorner_EventScript_TotodileDoll
	case 10, BattleFrontier_ExchangeServiceCorner_EventScript_ClerkGoodbye
	case MULTI_B_PRESSED, BattleFrontier_ExchangeServiceCorner_EventScript_ClerkGoodbye
	end

BattleFrontier_ExchangeServiceCorner_EventScript_KissPoster:: @ 825F21E
	setvar VAR_0x8008, 16
	setvar VAR_0x8009, DECOR_KISS_POSTER
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor1

BattleFrontier_ExchangeServiceCorner_EventScript_KissCushion:: @ 825F241
	setvar VAR_0x8008, 32
	setvar VAR_0x8009, DECOR_KISS_CUSHION
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor1

BattleFrontier_ExchangeServiceCorner_EventScript_SmoochumDoll:: @ 825F264
	setvar VAR_0x8008, 32
	setvar VAR_0x8009, DECOR_SMOOCHUM_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor1

BattleFrontier_ExchangeServiceCorner_EventScript_TogepiDoll:: @ 825F287
	setvar VAR_0x8008, 48
	setvar VAR_0x8009, DECOR_TOGEPI_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor1

BattleFrontier_ExchangeServiceCorner_EventScript_MeowthDoll:: @ 825F2AA
	setvar VAR_0x8008, 48
	setvar VAR_0x8009, DECOR_MEOWTH_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor1

BattleFrontier_ExchangeServiceCorner_EventScript_ClefairyDoll:: @ 825F2CD
	setvar VAR_0x8008, 48
	setvar VAR_0x8009, DECOR_CLEFAIRY_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor1

BattleFrontier_ExchangeServiceCorner_EventScript_DittoDoll:: @ 825F2F0
	setvar VAR_0x8008, 48
	setvar VAR_0x8009, DECOR_DITTO_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor1

BattleFrontier_ExchangeServiceCorner_EventScript_CyndaquilDoll:: @ 825F313
	setvar VAR_0x8008, 80
	setvar VAR_0x8009, DECOR_CYNDAQUIL_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor1

BattleFrontier_ExchangeServiceCorner_EventScript_ChikoritaDoll:: @ 825F336
	setvar VAR_0x8008, 80
	setvar VAR_0x8009, DECOR_CHIKORITA_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor1

BattleFrontier_ExchangeServiceCorner_EventScript_TotodileDoll:: @ 825F359
	setvar VAR_0x8008, 80
	setvar VAR_0x8009, DECOR_TOTODILE_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor1
	
BattleFrontier_ExchangeServiceCorner_EventScript_Decor1:: @ 825F21E
	bufferdecorationname 0, VAR_0x8009
	msgbox BattleFrontier_ExchangeServiceCorner_Text_ConfirmPrize, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq BattleFrontier_ExchangeServiceCorner_EventScript_ChooseDecor1
	goto BattleFrontier_ExchangeServiceCorner_EventScript_TryPurchasePrize	

BattleFrontier_ExchangeServiceCorner_EventScript_DecorClerk2:: @ 825F37C
	lock
	faceplayer
	setvar VAR_TEMP_2, EXCHANGE_CORNER_DECOR2_CLERK
	call BattleFrontier_ExchangeServiceCorner_EventScript_ClerkWelcome
	
BattleFrontier_ExchangeServiceCorner_EventScript_ChooseDecor2Branch::
	goto_if_set FLAG_REGI_DOORS_OPENED, BattleFrontier_ExchangeServiceCorner_EventScript_ChooseDecor2_Regi
	goto BattleFrontier_ExchangeServiceCorner_EventScript_ChooseDecor2

BattleFrontier_ExchangeServiceCorner_EventScript_ChooseDecor2::
	setvar VAR_0x8004, SCROLL_MULTI_BF_EXCHANGE_CORNER_DECOR_VENDOR_2
	special ShowFrontierExchangeCornerItemIconWindow
	special ShowScrollableMultichoice
	waitstate
	special CloseFrontierExchangeCornerItemIconWindow
	switch VAR_RESULT
	case 0, BattleFrontier_ExchangeServiceCorner_EventScript_LaprasDoll
	case 1, BattleFrontier_ExchangeServiceCorner_EventScript_SnorlaxDoll
	case 2, BattleFrontier_ExchangeServiceCorner_EventScript_VenusaurDoll
	case 3, BattleFrontier_ExchangeServiceCorner_EventScript_CharizardDoll
	case 4, BattleFrontier_ExchangeServiceCorner_EventScript_BlastoiseDoll
	case 5, BattleFrontier_ExchangeServiceCorner_EventScript_ClerkGoodbye
	case MULTI_B_PRESSED, BattleFrontier_ExchangeServiceCorner_EventScript_ClerkGoodbye
	end

BattleFrontier_ExchangeServiceCorner_EventScript_ChooseDecor2_Regi::
	setvar VAR_0x8004, SCROLL_MULTI_BF_EXCHANGE_CORNER_DECOR_VENDOR_2R
	special ShowFrontierExchangeCornerItemIconWindow
	special ShowScrollableMultichoice
	waitstate
	special CloseFrontierExchangeCornerItemIconWindow
	switch VAR_RESULT
	case 0, BattleFrontier_ExchangeServiceCorner_EventScript_LaprasDoll
	case 1, BattleFrontier_ExchangeServiceCorner_EventScript_SnorlaxDoll
	case 2, BattleFrontier_ExchangeServiceCorner_EventScript_VenusaurDoll
	case 3, BattleFrontier_ExchangeServiceCorner_EventScript_CharizardDoll
	case 4, BattleFrontier_ExchangeServiceCorner_EventScript_BlastoiseDoll
	case 5, BattleFrontier_ExchangeServiceCorner_EventScript_RegirockDoll
	case 6, BattleFrontier_ExchangeServiceCorner_EventScript_RegiceDoll
	case 7, BattleFrontier_ExchangeServiceCorner_EventScript_RegisteelDoll
	case 8, BattleFrontier_ExchangeServiceCorner_EventScript_ClerkGoodbye
	case MULTI_B_PRESSED, BattleFrontier_ExchangeServiceCorner_EventScript_ClerkGoodbye
	end

BattleFrontier_ExchangeServiceCorner_EventScript_LaprasDoll::
	setvar VAR_0x8008, 128
	setvar VAR_0x8009, DECOR_LAPRAS_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor2

BattleFrontier_ExchangeServiceCorner_EventScript_SnorlaxDoll::
	setvar VAR_0x8008, 128
	setvar VAR_0x8009, DECOR_SNORLAX_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor2

BattleFrontier_ExchangeServiceCorner_EventScript_VenusaurDoll::
	setvar VAR_0x8008, 256
	setvar VAR_0x8009, DECOR_VENUSAUR_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor2

BattleFrontier_ExchangeServiceCorner_EventScript_CharizardDoll::
	setvar VAR_0x8008, 256
	setvar VAR_0x8009, DECOR_CHARIZARD_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor2

BattleFrontier_ExchangeServiceCorner_EventScript_BlastoiseDoll::
	setvar VAR_0x8008, 256
	setvar VAR_0x8009, DECOR_BLASTOISE_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor2

BattleFrontier_ExchangeServiceCorner_EventScript_RegirockDoll::
	setvar VAR_0x8008, 256
	setvar VAR_0x8009, DECOR_REGIROCK_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor2

BattleFrontier_ExchangeServiceCorner_EventScript_RegiceDoll::
	setvar VAR_0x8008, 256
	setvar VAR_0x8009, DECOR_REGICE_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor2

BattleFrontier_ExchangeServiceCorner_EventScript_RegisteelDoll::
	setvar VAR_0x8008, 256
	setvar VAR_0x8009, DECOR_REGISTEEL_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor2
	
BattleFrontier_ExchangeServiceCorner_EventScript_Decor2::
	bufferdecorationname 0, VAR_0x8009
	msgbox BattleFrontier_ExchangeServiceCorner_Text_ConfirmPrize, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq BattleFrontier_ExchangeServiceCorner_EventScript_ChooseDecor2Branch
	goto BattleFrontier_ExchangeServiceCorner_EventScript_TryPurchasePrize	

BattleFrontier_ExchangeServiceCorner_EventScript_VitaminClerk:: @ 825F49F
	lock
	faceplayer
	setvar VAR_TEMP_2, EXCHANGE_CORNER_VITAMIN_CLERK
	call BattleFrontier_ExchangeServiceCorner_EventScript_ClerkWelcome
	goto BattleFrontier_ExchangeServiceCorner_EventScript_ChooseVitamin
	end

BattleFrontier_ExchangeServiceCorner_EventScript_ChooseVitamin:: @ 825F4B1
	setvar VAR_0x8004, SCROLL_MULTI_BF_EXCHANGE_CORNER_VITAMIN_VENDOR
	special ShowFrontierExchangeCornerItemIconWindow
	special ShowScrollableMultichoice
	waitstate
	special CloseFrontierExchangeCornerItemIconWindow
	switch VAR_RESULT
	case 0, BattleFrontier_ExchangeServiceCorner_EventScript_Protein
	case 1, BattleFrontier_ExchangeServiceCorner_EventScript_Calcium
	case 2, BattleFrontier_ExchangeServiceCorner_EventScript_Iron
	case 3, BattleFrontier_ExchangeServiceCorner_EventScript_Zinc
	case 4, BattleFrontier_ExchangeServiceCorner_EventScript_Carbos
	case 5, BattleFrontier_ExchangeServiceCorner_EventScript_HPUp
	case 6, BattleFrontier_ExchangeServiceCorner_EventScript_LiechiBerry
	case 7, BattleFrontier_ExchangeServiceCorner_EventScript_GanlonBerry
	case 8, BattleFrontier_ExchangeServiceCorner_EventScript_SalacBerry
	case 9, BattleFrontier_ExchangeServiceCorner_EventScript_PetayaBerry
	case 10, BattleFrontier_ExchangeServiceCorner_EventScript_ApicotBerry
	case MULTI_B_PRESSED, BattleFrontier_ExchangeServiceCorner_EventScript_ClerkGoodbye
	end

BattleFrontier_ExchangeServiceCorner_EventScript_Protein:: @ 825F51E
	setvar VAR_0x8008, 1
	setvar VAR_0x8009, ITEM_PROTEIN
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_Calcium:: @ 825F541
	setvar VAR_0x8008, 1
	setvar VAR_0x8009, ITEM_CALCIUM
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_Iron:: @ 825F564
	setvar VAR_0x8008, 1
	setvar VAR_0x8009, ITEM_IRON
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_Zinc:: @ 825F587
	setvar VAR_0x8008, 1
	setvar VAR_0x8009, ITEM_ZINC
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_Carbos:: @ 825F5AA
	setvar VAR_0x8008, 1
	setvar VAR_0x8009, ITEM_CARBOS
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_HPUp:: @ 825F5CD
	setvar VAR_0x8008, 1
	setvar VAR_0x8009, ITEM_HP_UP
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_LiechiBerry::
	setvar VAR_0x8008, 128
	setvar VAR_0x8009, ITEM_LIECHI_BERRY
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_GanlonBerry::
	setvar VAR_0x8008, 128
	setvar VAR_0x8009, ITEM_GANLON_BERRY
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_SalacBerry::
	setvar VAR_0x8008, 128
	setvar VAR_0x8009, ITEM_SALAC_BERRY
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_PetayaBerry::
	setvar VAR_0x8008, 128
	setvar VAR_0x8009, ITEM_PETAYA_BERRY
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_ApicotBerry::
	setvar VAR_0x8008, 128
	setvar VAR_0x8009, ITEM_APICOT_BERRY
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin::
	bufferitemname 0, VAR_0x8009
	msgbox BattleFrontier_ExchangeServiceCorner_Text_ConfirmPrize, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq BattleFrontier_ExchangeServiceCorner_EventScript_ChooseVitamin
	goto BattleFrontier_ExchangeServiceCorner_EventScript_TryPurchasePrize

BattleFrontier_ExchangeServiceCorner_EventScript_HoldItemClerk:: @ 825F5F0
	lock
	faceplayer
	setvar VAR_TEMP_2, EXCHANGE_CORNER_HOLD_ITEM_CLERK
	call BattleFrontier_ExchangeServiceCorner_EventScript_ClerkWelcome
	goto BattleFrontier_ExchangeServiceCorner_EventScript_ChooseHoldItem
	end

BattleFrontier_ExchangeServiceCorner_EventScript_ChooseHoldItem:: @ 825F602
	setvar VAR_0x8004, SCROLL_MULTI_BF_EXCHANGE_CORNER_HOLD_ITEM_VENDOR
	special ShowFrontierExchangeCornerItemIconWindow
	special ShowScrollableMultichoice
	waitstate
	special CloseFrontierExchangeCornerItemIconWindow
	switch VAR_RESULT
	case 0, BattleFrontier_ExchangeServiceCorner_EventScript_Leftovers
	case 1, BattleFrontier_ExchangeServiceCorner_EventScript_WhiteHerb
	case 2, BattleFrontier_ExchangeServiceCorner_EventScript_QuickClaw
	case 3, BattleFrontier_ExchangeServiceCorner_EventScript_MentalHerb
	case 4, BattleFrontier_ExchangeServiceCorner_EventScript_Brightpowder
	case 5, BattleFrontier_ExchangeServiceCorner_EventScript_ChoiceBand
	case 6, BattleFrontier_ExchangeServiceCorner_EventScript_KingsRock
	case 7, BattleFrontier_ExchangeServiceCorner_EventScript_FocusBand
	case 8, BattleFrontier_ExchangeServiceCorner_EventScript_ScopeLens
	case 9, BattleFrontier_ExchangeServiceCorner_EventScript_ClerkGoodbye
	case MULTI_B_PRESSED, BattleFrontier_ExchangeServiceCorner_EventScript_ClerkGoodbye
	end

BattleFrontier_ExchangeServiceCorner_EventScript_Leftovers:: @ 825F690
	setvar VAR_0x8008, 48
	setvar VAR_0x8009, ITEM_LEFTOVERS
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HoldItem

BattleFrontier_ExchangeServiceCorner_EventScript_WhiteHerb:: @ 825F6B3
	setvar VAR_0x8008, 48
	setvar VAR_0x8009, ITEM_WHITE_HERB
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HoldItem

BattleFrontier_ExchangeServiceCorner_EventScript_QuickClaw:: @ 825F6D6
	setvar VAR_0x8008, 48
	setvar VAR_0x8009, ITEM_QUICK_CLAW
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HoldItem

BattleFrontier_ExchangeServiceCorner_EventScript_MentalHerb:: @ 825F6F9
	setvar VAR_0x8008, 48
	setvar VAR_0x8009, ITEM_MENTAL_HERB
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HoldItem

BattleFrontier_ExchangeServiceCorner_EventScript_Brightpowder:: @ 825F71C
	setvar VAR_0x8008, 64
	setvar VAR_0x8009, ITEM_BRIGHT_POWDER
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HoldItem

BattleFrontier_ExchangeServiceCorner_EventScript_ChoiceBand:: @ 825F73F
	setvar VAR_0x8008, 64
	setvar VAR_0x8009, ITEM_CHOICE_BAND
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HoldItem

BattleFrontier_ExchangeServiceCorner_EventScript_KingsRock:: @ 825F762
	setvar VAR_0x8008, 64
	setvar VAR_0x8009, ITEM_KINGS_ROCK
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HoldItem

BattleFrontier_ExchangeServiceCorner_EventScript_FocusBand:: @ 825F785
	setvar VAR_0x8008, 64
	setvar VAR_0x8009, ITEM_FOCUS_BAND
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HoldItem

BattleFrontier_ExchangeServiceCorner_EventScript_ScopeLens:: @ 825F7A8
	setvar VAR_0x8008, 64
	setvar VAR_0x8009, ITEM_SCOPE_LENS
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HoldItem

BattleFrontier_ExchangeServiceCorner_EventScript_HoldItem:: @ 825F51E
	bufferitemname 0, VAR_0x8009
	msgbox BattleFrontier_ExchangeServiceCorner_Text_ConfirmPrize, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq BattleFrontier_ExchangeServiceCorner_EventScript_ChooseHoldItem
	goto BattleFrontier_ExchangeServiceCorner_EventScript_TryPurchasePrize

BattleFrontier_ExchangeServiceCorner_EventScript_Man:: @ 825F7CB
	msgbox BattleFrontier_ExchangeServiceCorner_Text_GoGetYourOwnDoll, MSGBOX_NPC
	end

BattleFrontier_ExchangeServiceCorner_EventScript_Sailor:: @ 825F7D4
	msgbox BattleFrontier_ExchangeServiceCorner_Text_ItemsWillGetMonTougher, MSGBOX_NPC
	end

BattleFrontier_ExchangeServiceCorner_EventScript_PokefanF:: @ 825F7DD
	lock
	msgbox BattleFrontier_ExchangeServiceCorner_Text_GetYouAnythingYouWant, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ExchangeServiceCorner_EventScript_RichBoy:: @ 825F7E8
	msgbox BattleFrontier_ExchangeServiceCorner_Text_WishIHadAllDolls, MSGBOX_NPC
	end

BattleFrontier_ExchangeServiceCorner_EventScript_Girl:: @ 825F7F1
	msgbox BattleFrontier_ExchangeServiceCorner_Text_MoreBattlePointsForRecord, MSGBOX_NPC
	end

BattleFrontier_ExchangeServiceCorner_Text_WelcomePleaseChoosePrize: @ 825F7FA
	.string "Hello, this is the EXCHANGE SERVICE\n"
	.string "CORNER.\p"
	.string "We exchange the Battle Points you\n"
	.string "have collected for lovely prizes.\p"
	.string "Please choose a prize from this list.$"

@ Unused
BattleFrontier_ExchangeServiceCorner_Text_PleaseChoosePrize: @ 825F890
	.string "Please choose a prize from this list.$"

BattleFrontier_ExchangeServiceCorner_Text_ConfirmPrize: @ 825F8B6
	.string "You've chosen the {STR_VAR_1}.\n"
	.string "Is that correct?$"

BattleFrontier_ExchangeServiceCorner_Text_WellSendItToPC: @ 825FE45
	.string "Thank you!\n"
	.string "We'll send it to your PC at home.$"

BattleFrontier_ExchangeServiceCorner_Text_HereIsYourPrize: @ 825FE72
	.string "Here is your prize!$"

BattleFrontier_ExchangeServiceCorner_Text_DontHaveEnoughPoints: @ 825FE86
	.string "I'm so sorry…\n"
	.string "You don't have enough Battle Points…$"

BattleFrontier_ExchangeServiceCorner_Text_PCIsFull: @ 825FEB9
	.string "I'm so sorry…\n"
	.string "Your PC appears to be full…$"

BattleFrontier_ExchangeServiceCorner_Text_DontHaveSpaceToHoldIt: @ 825FEE3
	.string "I'm so sorry…\n"
	.string "You don't have space to hold it…$"

BattleFrontier_ExchangeServiceCorner_Text_ThankYouVisitWithPoints: @ 825FF12
	.string "Thank you very much.\p"
	.string "Please visit us when you have\n"
	.string "saved up Battle Points.$"

BattleFrontier_ExchangeServiceCorner_Text_WishIHadAllDolls: @ 825FF5D
	.string "Oh, they're so nice!\n"
	.string "I wish I had them!\l"
	.string "Cute cushions!\l"
	.string "Big plush DOLLS!\l"
	.string "Little plush DOLLS!\l"
	.string "I wish I had them all!$"

BattleFrontier_ExchangeServiceCorner_Text_GetYouAnythingYouWant: @ 825FFD0
	.string "Leave it to your mommy!\p"
	.string "If it's anything you want, I'll go\n"
	.string "through anything to get it, honey!$"

BattleFrontier_ExchangeServiceCorner_Text_ItemsWillGetMonTougher: @ 826002E
	.string "If I can get hold of the items here,\n"
	.string "my POKéMON will get tougher.\p"
	.string "You bet they will!\n"
	.string "No question about it!$"

BattleFrontier_ExchangeServiceCorner_Text_GoGetYourOwnDoll: @ 8260099
	.string "Hah?\n"
	.string "What are you gawking at?\p"
	.string "I don't like you staring at my plush\n"
	.string "doll like that.\p"
	.string "If you want it, go get one yourself!\n"
	.string "Isn't that right, SMOOCHUM?$"

BattleFrontier_ExchangeServiceCorner_Text_MoreBattlePointsForRecord: @ 826012D
	.string "Did you know?\p"
	.string "If you stretch your record at any of\n"
	.string "the BATTLE facilities, they start\l"
	.string "giving you more and more Battle Points.$"

BattleFrontier_ExchangeServiceCorner_Text_KissPosterDesc:: @ 82601AA
	.string "A large poster with a SMOOCHUM print.$"

BattleFrontier_ExchangeServiceCorner_Text_KissCushionDesc:: @ 82601D0
	.string "A SMOOCHUM cushion.\n"
	.string "Place it on a mat or a desk.$"

BattleFrontier_ExchangeServiceCorner_Text_SmoochumDollDesc:: @ 8260201
	.string "A SMOOCHUM DOLL.\n"
	.string "Place it on a mat or a desk.$"

BattleFrontier_ExchangeServiceCorner_Text_TogepiDollDesc:: @ 826022F
	.string "A TOGEPI DOLL.\n"
	.string "Place it on a mat or a desk.$"

BattleFrontier_ExchangeServiceCorner_Text_MeowthDollDesc:: @ 826025B
	.string "A MEOWTH DOLL.\n"
	.string "Place it on a mat or a desk.$"

BattleFrontier_ExchangeServiceCorner_Text_ClefairyDollDesc:: @ 8260287
	.string "A CLEFAIRY DOLL.\n"
	.string "Place it on a mat or a desk.$"

BattleFrontier_ExchangeServiceCorner_Text_DittoDollDesc:: @ 82602B5
	.string "A DITTO DOLL.\n"
	.string "Place it on a mat or a desk.$"

BattleFrontier_ExchangeServiceCorner_Text_CyndaquilDollDesc:: @ 82602E0
	.string "A CYNDAQUIL DOLL.\n"
	.string "Place it on a mat or a desk.$"

BattleFrontier_ExchangeServiceCorner_Text_ChikoritaDollDesc:: @ 826030F
	.string "A CHIKORITA DOLL.\n"
	.string "Place it on a mat or a desk.$"

BattleFrontier_ExchangeServiceCorner_Text_TotodileDollDesc:: @ 826033E
	.string "A TOTODILE DOLL.\n"
	.string "Place it on a mat or a desk.$"

BattleFrontier_ExchangeServiceCorner_Text_LargeDollDesc:: @ 826036C
	.string "A large DOLL.\n"
	.string "Place it on a mat or a desk.$"

BattleFrontier_ExchangeServiceCorner_Text_ProteinDesc:: @ 8260397
	.string "Raises the stat ATTACK of one\n"
	.string "POKéMON.$"

BattleFrontier_ExchangeServiceCorner_Text_CalciumDesc:: @ 82603BE
	.string "Raises the stat SP. ATK of one\n"
	.string "POKéMON.$"

BattleFrontier_ExchangeServiceCorner_Text_IronDesc:: @ 82603E6
	.string "Raises the stat DEFENSE of one\n"
	.string "POKéMON.$"

BattleFrontier_ExchangeServiceCorner_Text_ZincDesc:: @ 826040E
	.string "Raises the stat SP. DEF of one\n"
	.string "POKéMON.$"

BattleFrontier_ExchangeServiceCorner_Text_CarbosDesc:: @ 8260436
	.string "Raises the stat SPEED of one\n"
	.string "POKéMON.$"

BattleFrontier_ExchangeServiceCorner_Text_HPUpDesc:: @ 826045C
	.string "Raises the HP of one POKéMON.$"

BattleFrontier_ExchangeServiceCorner_Text_LiechiBerryDesc::
	.string "A hold item that raises ATTACK in a\n"
	.string "pinch.$"

BattleFrontier_ExchangeServiceCorner_Text_GanlonBerryDesc::
	.string "A hold item that raises DEFENSE in a\n"
	.string "pinch.$"

BattleFrontier_ExchangeServiceCorner_Text_SalacBerryDesc::
	.string "A hold item that raises SPEED in a\n"
	.string "pinch.$"

BattleFrontier_ExchangeServiceCorner_Text_PetayaBerryDesc::
	.string "A hold item that raises SP.ATK in a\n"
	.string "pinch.$"

BattleFrontier_ExchangeServiceCorner_Text_ApicotBerryDesc::
	.string "A hold item that raises SP.DEF in a\n"
	.string "pinch.$"

BattleFrontier_ExchangeServiceCorner_Text_LeftoversDesc:: @ 826047A
	.string "A hold item that gradually restores\n"
	.string "HP in battle.$"

BattleFrontier_ExchangeServiceCorner_Text_WhiteHerbDesc:: @ 82604AC
	.string "A hold item that restores any\n"
	.string "lowered stat.$"

BattleFrontier_ExchangeServiceCorner_Text_QuickClawDesc:: @ 82604D8
	.string "A hold item that occasionally allows\n"
	.string "the first strike.$"

BattleFrontier_ExchangeServiceCorner_Text_MentalHerbDesc:: @ 826050F
	.string "A hold item that snaps POKéMON out\n"
	.string "of infatuation.$"

BattleFrontier_ExchangeServiceCorner_Text_BrightpowderDesc:: @ 8260542
	.string "A hold item that casts a glare to\n"
	.string "reduce accuracy.$"

BattleFrontier_ExchangeServiceCorner_Text_ChoiceBandDesc:: @ 8260575
	.string "Raises a move's power, but permits\n"
	.string "only that move.$"

BattleFrontier_ExchangeServiceCorner_Text_KingsRockDesc:: @ 82605A8
	.string "A hold item that may cause flinching\n"
	.string "when the foe is hit.$"

BattleFrontier_ExchangeServiceCorner_Text_FocusBandDesc:: @ 82605E2
	.string "A hold item that occasionally\n"
	.string "prevents fainting.$"

BattleFrontier_ExchangeServiceCorner_Text_ScopeLensDesc:: @ 8260613
	.string "A hold item that raises the\n"
	.string "critical-hit rate.$"
