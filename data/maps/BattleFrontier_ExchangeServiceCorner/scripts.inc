BattleFrontier_ExchangeServiceCorner_MapScripts::
	.byte 0

BattleFrontier_ExchangeServiceCorner_EventScript_ClerkWelcome::
	msgbox BattleFrontier_ExchangeServiceCorner_Text_WelcomePleaseChoosePrize, MSGBOX_DEFAULT
	special ShowBattlePointsWindow
	return

BattleFrontier_ExchangeServiceCorner_EventScript_ClerkGoodbye::
	msgbox BattleFrontier_ExchangeServiceCorner_Text_ThankYouVisitWithPoints, MSGBOX_DEFAULT
	special CloseBattlePointsWindow
	release
	end

BattleFrontier_ExchangeServiceCorner_EventScript_TryPurchasePrize::
	specialvar VAR_TEMP_1, GetFrontierBattlePoints
	goto_if_ge VAR_TEMP_1, VAR_0x8008, BattleFrontier_ExchangeServiceCorner_EventScript_TryGivePrize
	msgbox BattleFrontier_ExchangeServiceCorner_Text_DontHaveEnoughPoints, MSGBOX_DEFAULT
	goto_if_eq VAR_TEMP_2, EXCHANGE_CORNER_DECOR1_CLERK, BattleFrontier_ExchangeServiceCorner_EventScript_ChooseDecor1
	goto_if_eq VAR_TEMP_2, EXCHANGE_CORNER_DECOR2_CLERK, BattleFrontier_ExchangeServiceCorner_EventScript_ChooseDecor2Branch
	goto_if_eq VAR_TEMP_2, EXCHANGE_CORNER_VITAMIN_CLERK, BattleFrontier_ExchangeServiceCorner_EventScript_ChooseVitaminBranch
	goto_if_eq VAR_TEMP_2, EXCHANGE_CORNER_DECOR1_CLERK, BattleFrontier_ExchangeServiceCorner_EventScript_ChooseDecor1
	goto_if_eq VAR_TEMP_2, EXCHANGE_CORNER_DECOR2_CLERK, BattleFrontier_ExchangeServiceCorner_EventScript_ChooseDecor2
	goto_if_eq VAR_TEMP_2, EXCHANGE_CORNER_VITAMIN_CLERK, BattleFrontier_ExchangeServiceCorner_EventScript_ChooseVitamin
	goto BattleFrontier_ExchangeServiceCorner_EventScript_ChooseHoldItem
	end

BattleFrontier_ExchangeServiceCorner_EventScript_TryGivePrize::
	goto_if_eq VAR_TEMP_2, EXCHANGE_CORNER_DECOR1_CLERK, BattleFrontier_ExchangeServiceCorner_EventScript_TryGiveDecor
	goto_if_eq VAR_TEMP_2, EXCHANGE_CORNER_DECOR2_CLERK, BattleFrontier_ExchangeServiceCorner_EventScript_TryGiveDecor
	goto BattleFrontier_ExchangeServiceCorner_EventScript_TryGiveItem
	end

BattleFrontier_ExchangeServiceCorner_EventScript_TryGiveDecor::
	checkdecorspace VAR_0x8009
	goto_if_eq VAR_RESULT, FALSE, BattleFrontier_ExchangeServiceCorner_EventScript_NoRoomForDecor
	copyvar VAR_0x8004, VAR_0x8008
	special TakeFrontierBattlePoints
	adddecoration VAR_0x8009
	special UpdateBattlePointsWindow
	playse SE_SHOP
	msgbox BattleFrontier_ExchangeServiceCorner_Text_WellSendItToPC, MSGBOX_DEFAULT
	goto_if_eq VAR_TEMP_2, EXCHANGE_CORNER_DECOR1_CLERK, BattleFrontier_ExchangeServiceCorner_EventScript_ChooseDecor1
	goto BattleFrontier_ExchangeServiceCorner_EventScript_ChooseDecor2Branch
	end

BattleFrontier_ExchangeServiceCorner_EventScript_NoRoomForDecor::
	msgbox BattleFrontier_ExchangeServiceCorner_Text_PCIsFull, MSGBOX_DEFAULT
	special CloseBattlePointsWindow
	release
	end

BattleFrontier_ExchangeServiceCorner_EventScript_TryGiveItem::
	checkitemspace VAR_0x8009
	goto_if_eq VAR_RESULT, FALSE, BattleFrontier_ExchangeServiceCorner_EventScript_BagFull
	copyvar VAR_0x8004, VAR_0x8008
	special TakeFrontierBattlePoints
	additem VAR_0x8009
	special UpdateBattlePointsWindow
	playse SE_SHOP
	msgbox BattleFrontier_ExchangeServiceCorner_Text_HereIsYourPrize, MSGBOX_DEFAULT
	goto_if_eq VAR_TEMP_2, EXCHANGE_CORNER_VITAMIN_CLERK, BattleFrontier_ExchangeServiceCorner_EventScript_ChooseVitaminBranch
	goto BattleFrontier_ExchangeServiceCorner_EventScript_ChooseHoldItem
	end

BattleFrontier_ExchangeServiceCorner_EventScript_BagFull::
	msgbox BattleFrontier_ExchangeServiceCorner_Text_DontHaveSpaceToHoldIt, MSGBOX_DEFAULT
	special CloseBattlePointsWindow
	release
	end

BattleFrontier_ExchangeServiceCorner_EventScript_DecorClerk1::
	lock
	faceplayer
	setvar VAR_TEMP_2, EXCHANGE_CORNER_DECOR1_CLERK
	call BattleFrontier_ExchangeServiceCorner_EventScript_ClerkWelcome
	goto BattleFrontier_ExchangeServiceCorner_EventScript_ChooseDecor1
	end

BattleFrontier_ExchangeServiceCorner_EventScript_ChooseDecor1::
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

BattleFrontier_ExchangeServiceCorner_EventScript_KissPoster::
	setvar VAR_0x8008, 8
	setvar VAR_0x8009, DECOR_KISS_POSTER
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor1

BattleFrontier_ExchangeServiceCorner_EventScript_KissCushion::
	setvar VAR_0x8008, 12
	setvar VAR_0x8009, DECOR_KISS_CUSHION
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor1

BattleFrontier_ExchangeServiceCorner_EventScript_SmoochumDoll::
	setvar VAR_0x8008, 16
	setvar VAR_0x8009, DECOR_SMOOCHUM_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor1

BattleFrontier_ExchangeServiceCorner_EventScript_TogepiDoll::
	setvar VAR_0x8008, 16
	setvar VAR_0x8009, DECOR_TOGEPI_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor1

BattleFrontier_ExchangeServiceCorner_EventScript_MeowthDoll::
	setvar VAR_0x8008, 16
	setvar VAR_0x8009, DECOR_MEOWTH_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor1

BattleFrontier_ExchangeServiceCorner_EventScript_ClefairyDoll::
	setvar VAR_0x8008, 16
	setvar VAR_0x8009, DECOR_CLEFAIRY_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor1

BattleFrontier_ExchangeServiceCorner_EventScript_DittoDoll::
	setvar VAR_0x8008, 16
	setvar VAR_0x8009, DECOR_DITTO_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor1

BattleFrontier_ExchangeServiceCorner_EventScript_CyndaquilDoll::
	setvar VAR_0x8008, 24
	setvar VAR_0x8009, DECOR_CYNDAQUIL_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor1

BattleFrontier_ExchangeServiceCorner_EventScript_ChikoritaDoll::
	setvar VAR_0x8008, 24
	setvar VAR_0x8009, DECOR_CHIKORITA_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor1

BattleFrontier_ExchangeServiceCorner_EventScript_TotodileDoll::
	setvar VAR_0x8008, 24
	setvar VAR_0x8009, DECOR_TOTODILE_DOLL
	
BattleFrontier_ExchangeServiceCorner_EventScript_Decor1::
	bufferdecorationname 0, VAR_0x8009
	msgbox BattleFrontier_ExchangeServiceCorner_Text_ConfirmPrize, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_ExchangeServiceCorner_EventScript_ChooseDecor1
	goto BattleFrontier_ExchangeServiceCorner_EventScript_TryPurchasePrize	

BattleFrontier_ExchangeServiceCorner_EventScript_DecorClerk2::
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
	goto BattleFrontier_ExchangeServiceCorner_EventScript_ClerkGoodbye

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
	goto BattleFrontier_ExchangeServiceCorner_EventScript_ClerkGoodbye

BattleFrontier_ExchangeServiceCorner_EventScript_LaprasDoll::
	setvar VAR_0x8008, 40
	setvar VAR_0x8009, DECOR_LAPRAS_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor2

BattleFrontier_ExchangeServiceCorner_EventScript_SnorlaxDoll::
	setvar VAR_0x8008, 40
	setvar VAR_0x8009, DECOR_SNORLAX_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor2

BattleFrontier_ExchangeServiceCorner_EventScript_VenusaurDoll::
	setvar VAR_0x8008, 64
	setvar VAR_0x8009, DECOR_VENUSAUR_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor2

BattleFrontier_ExchangeServiceCorner_EventScript_CharizardDoll::
	setvar VAR_0x8008, 64
	setvar VAR_0x8009, DECOR_CHARIZARD_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor2

BattleFrontier_ExchangeServiceCorner_EventScript_BlastoiseDoll::
	setvar VAR_0x8008, 64
	setvar VAR_0x8009, DECOR_BLASTOISE_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor2

BattleFrontier_ExchangeServiceCorner_EventScript_RegirockDoll::
	setvar VAR_0x8008, 128
	setvar VAR_0x8009, DECOR_REGIROCK_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor2

BattleFrontier_ExchangeServiceCorner_EventScript_RegiceDoll::
	setvar VAR_0x8008, 128
	setvar VAR_0x8009, DECOR_REGICE_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor2

BattleFrontier_ExchangeServiceCorner_EventScript_RegisteelDoll::
	setvar VAR_0x8008, 128
	setvar VAR_0x8009, DECOR_REGISTEEL_DOLL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Decor2
	
BattleFrontier_ExchangeServiceCorner_EventScript_Decor2::
	bufferdecorationname 0, VAR_0x8009
	msgbox BattleFrontier_ExchangeServiceCorner_Text_ConfirmPrize, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_ExchangeServiceCorner_EventScript_ChooseDecor2Branch
	goto BattleFrontier_ExchangeServiceCorner_EventScript_TryPurchasePrize	

BattleFrontier_ExchangeServiceCorner_EventScript_VitaminClerk::
	lock
	faceplayer
	setvar VAR_TEMP_2, EXCHANGE_CORNER_VITAMIN_CLERK
	call BattleFrontier_ExchangeServiceCorner_EventScript_ClerkWelcome
	goto BattleFrontier_ExchangeServiceCorner_EventScript_ChooseVitaminBranch
	
BattleFrontier_ExchangeServiceCorner_EventScript_ChooseVitaminBranch::
	goto_if_set FLAG_COLLECTED_ALL_GOLD_SYMBOLS, BattleFrontier_ExchangeServiceCorner_EventScript_ChooseVitaminGold
	goto_if_set FLAG_COLLECTED_ALL_SILVER_SYMBOLS, BattleFrontier_ExchangeServiceCorner_EventScript_ChooseVitaminSilver
	goto BattleFrontier_ExchangeServiceCorner_EventScript_ChooseVitamin

BattleFrontier_ExchangeServiceCorner_EventScript_ChooseVitamin::
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
	case 6, BattleFrontier_ExchangeServiceCorner_EventScript_MachoBrace
	case 7, BattleFrontier_ExchangeServiceCorner_EventScript_AbilityCapsule
	case 8, BattleFrontier_ExchangeServiceCorner_EventScript_RareCandy
	case 9, BattleFrontier_ExchangeServiceCorner_EventScript_LiechiBerry
	case 10, BattleFrontier_ExchangeServiceCorner_EventScript_GanlonBerry
	case 11, BattleFrontier_ExchangeServiceCorner_EventScript_SalacBerry
	case 12, BattleFrontier_ExchangeServiceCorner_EventScript_PetayaBerry
	case 13, BattleFrontier_ExchangeServiceCorner_EventScript_ApicotBerry
	goto BattleFrontier_ExchangeServiceCorner_EventScript_ClerkGoodbye

BattleFrontier_ExchangeServiceCorner_EventScript_ChooseVitaminSilver::
	setvar VAR_0x8004, SCROLL_MULTI_BF_EXCHANGE_CORNER_VITAMIN_VENDOR_S
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
	case 6, BattleFrontier_ExchangeServiceCorner_EventScript_MachoBrace
	case 7, BattleFrontier_ExchangeServiceCorner_EventScript_AbilityCapsule
	case 8, BattleFrontier_ExchangeServiceCorner_EventScript_RareCandy
	case 9, BattleFrontier_ExchangeServiceCorner_EventScript_LiechiBerry
	case 10, BattleFrontier_ExchangeServiceCorner_EventScript_GanlonBerry
	case 11, BattleFrontier_ExchangeServiceCorner_EventScript_SalacBerry
	case 12, BattleFrontier_ExchangeServiceCorner_EventScript_PetayaBerry
	case 13, BattleFrontier_ExchangeServiceCorner_EventScript_ApicotBerry
	case 14, BattleFrontier_ExchangeServiceCorner_EventScript_LansatBerry
	goto BattleFrontier_ExchangeServiceCorner_EventScript_ClerkGoodbye

BattleFrontier_ExchangeServiceCorner_EventScript_ChooseVitaminGold::
	setvar VAR_0x8004, SCROLL_MULTI_BF_EXCHANGE_CORNER_VITAMIN_VENDOR_G
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
	case 6, BattleFrontier_ExchangeServiceCorner_EventScript_MachoBrace
	case 7, BattleFrontier_ExchangeServiceCorner_EventScript_AbilityCapsule
	case 8, BattleFrontier_ExchangeServiceCorner_EventScript_RareCandy
	case 9, BattleFrontier_ExchangeServiceCorner_EventScript_LiechiBerry
	case 10, BattleFrontier_ExchangeServiceCorner_EventScript_GanlonBerry
	case 11, BattleFrontier_ExchangeServiceCorner_EventScript_SalacBerry
	case 12, BattleFrontier_ExchangeServiceCorner_EventScript_PetayaBerry
	case 13, BattleFrontier_ExchangeServiceCorner_EventScript_ApicotBerry
	case 14, BattleFrontier_ExchangeServiceCorner_EventScript_LansatBerry
	case 15, BattleFrontier_ExchangeServiceCorner_EventScript_StarfBerry
	goto BattleFrontier_ExchangeServiceCorner_EventScript_ClerkGoodbye

BattleFrontier_ExchangeServiceCorner_EventScript_Protein::
	setvar VAR_0x8008, 1
	setvar VAR_0x8009, ITEM_PROTEIN
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_Calcium::
	setvar VAR_0x8008, 1
	setvar VAR_0x8009, ITEM_CALCIUM
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_Iron::
	setvar VAR_0x8008, 1
	setvar VAR_0x8009, ITEM_IRON
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_Zinc::
	setvar VAR_0x8008, 1
	setvar VAR_0x8009, ITEM_ZINC
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_Carbos::
	setvar VAR_0x8008, 1
	setvar VAR_0x8009, ITEM_CARBOS
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_HPUp::
	setvar VAR_0x8008, 1
	setvar VAR_0x8009, ITEM_HP_UP
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_MachoBrace::
	setvar VAR_0x8008, 8
	setvar VAR_0x8009, ITEM_MACHO_BRACE
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_AbilityCapsule::
	setvar VAR_0x8008, 24
	setvar VAR_0x8009, ITEM_ABILITY_CAPSULE
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_RareCandy::
	setvar VAR_0x8008, 24
	setvar VAR_0x8009, ITEM_RARE_CANDY
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_LiechiBerry::
	setvar VAR_0x8008, 64
	setvar VAR_0x8009, ITEM_LIECHI_BERRY
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_GanlonBerry::
	setvar VAR_0x8008, 64
	setvar VAR_0x8009, ITEM_GANLON_BERRY
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_SalacBerry::
	setvar VAR_0x8008, 64
	setvar VAR_0x8009, ITEM_SALAC_BERRY
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_PetayaBerry::
	setvar VAR_0x8008, 64
	setvar VAR_0x8009, ITEM_PETAYA_BERRY
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_ApicotBerry::
	setvar VAR_0x8008, 64
	setvar VAR_0x8009, ITEM_APICOT_BERRY
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_LansatBerry::
	setvar VAR_0x8008, 128
	setvar VAR_0x8009, ITEM_LANSAT_BERRY
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_StarfBerry::
	setvar VAR_0x8008, 128
	setvar VAR_0x8009, ITEM_STARF_BERRY
	goto BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin

BattleFrontier_ExchangeServiceCorner_EventScript_Vitamin::
	bufferitemname 0, VAR_0x8009
	msgbox BattleFrontier_ExchangeServiceCorner_Text_ConfirmPrize, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_ExchangeServiceCorner_EventScript_ChooseVitaminBranch
	goto BattleFrontier_ExchangeServiceCorner_EventScript_TryPurchasePrize

BattleFrontier_ExchangeServiceCorner_EventScript_HoldItemClerk::
	lock
	faceplayer
	setvar VAR_TEMP_2, EXCHANGE_CORNER_HOLD_ITEM_CLERK
	call BattleFrontier_ExchangeServiceCorner_EventScript_ClerkWelcome
	goto BattleFrontier_ExchangeServiceCorner_EventScript_ChooseHoldItem
	end

BattleFrontier_ExchangeServiceCorner_EventScript_ChooseHoldItem::
	setvar VAR_0x8004, SCROLL_MULTI_BF_EXCHANGE_CORNER_HOLD_ITEM_VENDOR
	special ShowFrontierExchangeCornerItemIconWindow
	special ShowScrollableMultichoice
	waitstate
	special CloseFrontierExchangeCornerItemIconWindow
	switch VAR_RESULT
	case 0, BattleFrontier_ExchangeServiceCorner_EventScript_Everstone
	case 1, BattleFrontier_ExchangeServiceCorner_EventScript_WhiteHerb
	case 2, BattleFrontier_ExchangeServiceCorner_EventScript_MentalHerb
	case 3, BattleFrontier_ExchangeServiceCorner_EventScript_KingsRock
	case 4, BattleFrontier_ExchangeServiceCorner_EventScript_UpGrade
	case 5, BattleFrontier_ExchangeServiceCorner_EventScript_DeepSeaTooth
	case 6, BattleFrontier_ExchangeServiceCorner_EventScript_DeepSeaScale
	case 7, BattleFrontier_ExchangeServiceCorner_EventScript_DragonScale
	case 8, BattleFrontier_ExchangeServiceCorner_EventScript_Leftovers
	case 9, BattleFrontier_ExchangeServiceCorner_EventScript_QuickClaw
	case 10, BattleFrontier_ExchangeServiceCorner_EventScript_Brightpowder
	case 11, BattleFrontier_ExchangeServiceCorner_EventScript_SootheBell
	case 12, BattleFrontier_ExchangeServiceCorner_EventScript_ChoiceBand
	case 13, BattleFrontier_ExchangeServiceCorner_EventScript_FocusBand
	case 14, BattleFrontier_ExchangeServiceCorner_EventScript_ScopeLens
	goto BattleFrontier_ExchangeServiceCorner_EventScript_ClerkGoodbye

BattleFrontier_ExchangeServiceCorner_EventScript_Everstone::
	setvar VAR_0x8008, 8
	setvar VAR_0x8009, ITEM_EVERSTONE
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HoldItem

BattleFrontier_ExchangeServiceCorner_EventScript_WhiteHerb::
	setvar VAR_0x8008, 12
	setvar VAR_0x8009, ITEM_WHITE_HERB
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HoldItem

BattleFrontier_ExchangeServiceCorner_EventScript_MentalHerb::
	setvar VAR_0x8008, 12
	setvar VAR_0x8009, ITEM_MENTAL_HERB
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HoldItem

BattleFrontier_ExchangeServiceCorner_EventScript_KingsRock::
	setvar VAR_0x8008, 16
	setvar VAR_0x8009, ITEM_KINGS_ROCK
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HoldItem

BattleFrontier_ExchangeServiceCorner_EventScript_UpGrade::
	setvar VAR_0x8008, 16
	setvar VAR_0x8009, ITEM_UP_GRADE
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HoldItem

BattleFrontier_ExchangeServiceCorner_EventScript_DeepSeaTooth::
	setvar VAR_0x8008, 16
	setvar VAR_0x8009, ITEM_DEEP_SEA_TOOTH
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HoldItem

BattleFrontier_ExchangeServiceCorner_EventScript_DeepSeaScale::
	setvar VAR_0x8008, 16
	setvar VAR_0x8009, ITEM_DEEP_SEA_SCALE
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HoldItem

BattleFrontier_ExchangeServiceCorner_EventScript_DragonScale::
	setvar VAR_0x8008, 16
	setvar VAR_0x8009, ITEM_DRAGON_SCALE
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HoldItem

BattleFrontier_ExchangeServiceCorner_EventScript_Leftovers::
	setvar VAR_0x8008, 24
	setvar VAR_0x8009, ITEM_LEFTOVERS
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HoldItem

BattleFrontier_ExchangeServiceCorner_EventScript_QuickClaw::
	setvar VAR_0x8008, 24
	setvar VAR_0x8009, ITEM_QUICK_CLAW
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HoldItem

BattleFrontier_ExchangeServiceCorner_EventScript_Brightpowder::
	setvar VAR_0x8008, 24
	setvar VAR_0x8009, ITEM_BRIGHT_POWDER
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HoldItem

BattleFrontier_ExchangeServiceCorner_EventScript_SootheBell::
	setvar VAR_0x8008, 24
	setvar VAR_0x8009, ITEM_SOOTHE_BELL
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HoldItem

BattleFrontier_ExchangeServiceCorner_EventScript_ChoiceBand::
	setvar VAR_0x8008, 24
	setvar VAR_0x8009, ITEM_CHOICE_BAND
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HoldItem

BattleFrontier_ExchangeServiceCorner_EventScript_FocusBand::
	setvar VAR_0x8008, 24
	setvar VAR_0x8009, ITEM_FOCUS_BAND
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HoldItem

BattleFrontier_ExchangeServiceCorner_EventScript_ScopeLens::
	setvar VAR_0x8008, 24
	setvar VAR_0x8009, ITEM_SCOPE_LENS
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HoldItem

BattleFrontier_ExchangeServiceCorner_EventScript_HoldItem::
	bufferitemname 0, VAR_0x8009
	msgbox BattleFrontier_ExchangeServiceCorner_Text_ConfirmPrize, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_ExchangeServiceCorner_EventScript_ChooseHoldItem
	goto BattleFrontier_ExchangeServiceCorner_EventScript_TryPurchasePrize

BattleFrontier_ExchangeServiceCorner_EventScript_Man::
	msgbox BattleFrontier_ExchangeServiceCorner_Text_GoGetYourOwnDoll, MSGBOX_NPC
	end

BattleFrontier_ExchangeServiceCorner_EventScript_Sailor::
	msgbox BattleFrontier_ExchangeServiceCorner_Text_ItemsWillGetMonTougher, MSGBOX_NPC
	end

BattleFrontier_ExchangeServiceCorner_EventScript_PokefanF::
	lock
	msgbox BattleFrontier_ExchangeServiceCorner_Text_GetYouAnythingYouWant, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ExchangeServiceCorner_EventScript_RichBoy::
	msgbox BattleFrontier_ExchangeServiceCorner_Text_WishIHadAllDolls, MSGBOX_NPC
	end

BattleFrontier_ExchangeServiceCorner_EventScript_Girl::
	msgbox BattleFrontier_ExchangeServiceCorner_Text_MoreBattlePointsForRecord, MSGBOX_NPC
	end

BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainingExpert::
	lock
	faceplayer
	goto_if_set FLAG_SPOKE_TO_HYPER_TRAINING_EXPERT, BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainingMet
	msgbox BattleFrontier_ExchangeServiceCorner_Text_HyperTrainingIntro, MSGBOX_DEFAULT
	setflag FLAG_SPOKE_TO_HYPER_TRAINING_EXPERT
BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainingMet::
	msgbox BattleFrontier_ExchangeServiceCorner_Text_WannaHyperTrain, MSGBOX_YESNO
BattleFrontier_ExchangeServiceCorner_EventScript_MoreHyperTraining::
	goto_if_eq VAR_RESULT, NO, BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainingDeclined
	specialvar VAR_TEMP_1, GetFrontierBattlePoints
	goto_if_le VAR_TEMP_1, 24, BattleFrontier_Lounge7_EventScript_NotEnoughPointsForTraining
	msgbox BattleFrontier_ExchangeServiceCorner_Text_HyperTrainingWhichMon, MSGBOX_DEFAULT
	special ChoosePartyMon
	waitstate
	copyvar VAR_0x8006, VAR_0x8004
	goto_if_eq VAR_0x8006, PARTY_NOTHING_CHOSEN, BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainingDeclined
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_EGG, BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainingEgg
	bufferspeciesname STR_VAR_1, VAR_RESULT
	specialvar VAR_RESULT, ScriptGetPartyMonLevel
	goto_if_lt VAR_RESULT, 50, BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainingLowLevel
	specialvar VAR_RESULT, CheckIfIVsMaxed
	goto_if_eq VAR_RESULT, TRUE, BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainingIVsMaxed
	goto_if_eq VAR_0x8004, 24, BattleFrontier_ExchangeServiceCorner_EventScript_OneStatToHyperTrain
BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainingPickStat::
	message BattleFrontier_ExchangeServiceCorner_Text_HyperTrainingPickStat
	waitmessage
	special ShowBattlePointsWindow
BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainingCallbackPickStat::
	multichoicegrid 8, 1, MULTI_HYPER_TRAINING, 2, FALSE
	goto_if_ge VAR_RESULT, 7, BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainingCanceled
	goto_if_eq VAR_RESULT, 6, BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainAllStats
	copyvar VAR_0x8005, VAR_RESULT
	specialvar VAR_RESULT, CheckIfSelectedStatMaxed
	goto_if_eq VAR_RESULT, TRUE, BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainingSelectedStatMaxed
BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainXStat::
	special BufferStatName
	msgbox BattleFrontier_ExchangeServiceCorner_Text_HyperTrainingRaiseXStat, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainingReturnToPickStat
	setvar VAR_0x8004, 24
BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainStats::
	special TakeFrontierBattlePoints
	special HyperTrainStats
	special UpdateBattlePointsWindow
	playse SE_SHOP
	msgbox BattleFrontier_ExchangeServiceCorner_Text_TrainingStartsNow, MSGBOX_DEFAULT
	closemessage
	special CloseBattlePointsWindow
	fadescreen FADE_TO_BLACK
	delay 15
	playse SE_M_MEGA_KICK
	waitse
	delay 30
	fadescreen FADE_FROM_BLACK
	msgbox BattleFrontier_ExchangeServiceCorner_Text_HyperTrainingDone, MSGBOX_DEFAULT
	msgbox BattleFrontier_ExchangeServiceCorner_Text_DoMoreHyperTraining, MSGBOX_YESNO
	goto BattleFrontier_ExchangeServiceCorner_EventScript_MoreHyperTraining

BattleFrontier_ExchangeServiceCorner_EventScript_OneStatToHyperTrain::
	special ShowBattlePointsWindow
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainXStat

BattleFrontier_Lounge7_EventScript_NotEnoughPointsForTraining::
	msgbox BattleFrontier_ExchangeServiceCorner_Text_HyperTrainingNotEnoughBP, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainAllStats::
	goto_if_lt VAR_TEMP_1, VAR_0x8004, BattleFrontier_Lounge7_EventScript_NotEnoughPointsForAll
	buffernumberstring STR_VAR_3, VAR_0x8004
	msgbox BattleFrontier_ExchangeServiceCorner_Text_HyperTrainingRaiseAllStats, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainingReturnToPickStat
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainStats

BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainingCanceled::
	special CloseBattlePointsWindow
BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainingDeclined::
	msgbox BattleFrontier_ExchangeServiceCorner_Text_HyperTrainingDeclined, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainingEgg::
	msgbox BattleFrontier_ExchangeServiceCorner_Text_HyperTrainingEgg, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainingLowLevel::
	msgbox BattleFrontier_ExchangeServiceCorner_Text_HyperTrainingLowLevel, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainingIVsMaxed::
	msgbox BattleFrontier_ExchangeServiceCorner_Text_HyperTrainingIVsMaxed, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainingSelectedStatMaxed::
	msgbox BattleFrontier_ExchangeServiceCorner_Text_HyperTrainingStatMaxed, MSGBOX_DEFAULT
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainingReturnToPickStat

BattleFrontier_Lounge7_EventScript_NotEnoughPointsForAll::
	msgbox BattleFrontier_ExchangeServiceCorner_Text_HyperTrainingNotEnoughBP, MSGBOX_DEFAULT
BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainingReturnToPickStat::
	message BattleFrontier_ExchangeServiceCorner_Text_HyperTrainingPickStat
	waitmessage
	goto BattleFrontier_ExchangeServiceCorner_EventScript_HyperTrainingCallbackPickStat

BattleFrontier_ExchangeServiceCorner_Text_WelcomePleaseChoosePrize:
	.string "Hello, this is the EXCHANGE SERVICE\n"
	.string "CORNER.\p"
	.string "We exchange the BATTLE POINTS you\n"
	.string "have collected for lovely prizes.\p"
	.string "Please choose a prize from this list.$"

@ Unused
BattleFrontier_ExchangeServiceCorner_Text_PleaseChoosePrize:
	.string "Please choose a prize from this list.$"

BattleFrontier_ExchangeServiceCorner_Text_ConfirmPrize:
	.string "You've chosen the {STR_VAR_1}.\n"
	.string "Is that correct?$"

BattleFrontier_ExchangeServiceCorner_Text_WellSendItToPC:
	.string "Thank you!\n"
	.string "We'll send it to your PC at home.$"

BattleFrontier_ExchangeServiceCorner_Text_HereIsYourPrize:
	.string "Here is your prize!$"

BattleFrontier_ExchangeServiceCorner_Text_DontHaveEnoughPoints:
	.string "I'm so sorry…\n"
	.string "You don't have enough BATTLE POINTS…$"

BattleFrontier_ExchangeServiceCorner_Text_PCIsFull:
	.string "I'm so sorry…\n"
	.string "Your PC appears to be full…$"

BattleFrontier_ExchangeServiceCorner_Text_DontHaveSpaceToHoldIt:
	.string "I'm so sorry…\n"
	.string "You don't have space to hold it…$"

BattleFrontier_ExchangeServiceCorner_Text_ThankYouVisitWithPoints:
	.string "Thank you very much.\p"
	.string "Please visit us when you have\n"
	.string "saved up BATTLE POINTS.$"

BattleFrontier_ExchangeServiceCorner_Text_WishIHadAllDolls:
	.string "Oh, they're so nice!\n"
	.string "I wish I had them!\l"
	.string "Cute cushions!\l"
	.string "Big plush DOLLS!\l"
	.string "Little plush DOLLS!\l"
	.string "I wish I had them all!$"

BattleFrontier_ExchangeServiceCorner_Text_GetYouAnythingYouWant:
	.string "Leave it to your mommy!\p"
	.string "If it's anything you want, I'll go\n"
	.string "through anything to get it, honey!$"

BattleFrontier_ExchangeServiceCorner_Text_ItemsWillGetMonTougher:
	.string "If I can get hold of the items here,\n"
	.string "my POKéMON will get tougher.\p"
	.string "You bet they will!\n"
	.string "No question about it!$"

BattleFrontier_ExchangeServiceCorner_Text_GoGetYourOwnDoll:
	.string "Hah?\n"
	.string "What are you gawking at?\p"
	.string "I don't like you staring at my plush\n"
	.string "doll like that.\p"
	.string "If you want it, go get one yourself!\n"
	.string "Isn't that right, SMOOCHUM?$"

BattleFrontier_ExchangeServiceCorner_Text_MoreBattlePointsForRecord:
	.string "Did you know?\p"
	.string "If you stretch your record at any of\n"
	.string "the BATTLE facilities, they start\l"
	.string "giving you more and more BATTLE POINTS.$"

BattleFrontier_ExchangeServiceCorner_Text_HyperTrainingIntro:
	.string "Hoho!\n"
	.string "I'm the HYPER TRAINING EXPERT!\p"
	.string "If you have any POKéMON at Lv. 50 or\n"
	.string "above, I can train their stats!\p"
	.string "Each stat I train will cost you 24\n"
	.string "BATTLE POINTS.\p"
	.string "I can also HYPER TRAIN all of your\n"
	.string "POKéMON's stats at once if you have\l"
	.string "the BATTLE POINTS to spare.\p"
	.string "I can assure you that your POKéMON will\n"
	.string "come out of it in tip-top shape!$"

BattleFrontier_ExchangeServiceCorner_Text_WannaHyperTrain:
	.string "What do you say? Want your POKéMON to\n"
	.string "go through some HYPER TRAINING?$"

BattleFrontier_ExchangeServiceCorner_Text_HyperTrainingWhichMon:
	.string "Which POKéMON do you want me to HYPER\n"
	.string "TRAIN?$"

BattleFrontier_ExchangeServiceCorner_Text_HyperTrainingPickStat:
	.string "Which stat needs training?$"

BattleFrontier_ExchangeServiceCorner_Text_HyperTrainingDeclined:
	.string "Come back anytime you're ready for\n"
	.string "some HYPER TRAINING!$"

BattleFrontier_ExchangeServiceCorner_Text_HyperTrainingEgg:
	.string "An EGG?!\n"
	.string "How am I supposed to train that?$"

BattleFrontier_ExchangeServiceCorner_Text_HyperTrainingLowLevel:
	.string "Your {STR_VAR_1} is underleveled!\n"
	.string "Come back once it reaches Lv. 50.$"

BattleFrontier_ExchangeServiceCorner_Text_HyperTrainingIVsMaxed:
	.string "Your {STR_VAR_1} doesn't need training!\n"
	.string "All of its stats are in peak condition!$"

BattleFrontier_ExchangeServiceCorner_Text_HyperTrainingNotEnoughBP:
	.string "You don't have enough BATTLE POINTS!\n"
	.string "I need 24 BATTLE POINTS to train a stat.$"

BattleFrontier_ExchangeServiceCorner_Text_HyperTrainingRaiseXStat:
	.string "You want me to raise your {STR_VAR_1}'s\n"
	.string "{STR_VAR_2}, then?\l"
	.string "That will cost 24 BATTLE POINTS.$"

BattleFrontier_ExchangeServiceCorner_Text_HyperTrainingRaiseAllStats:
	.string "You want me to raise all of your\n"
	.string "{STR_VAR_1}'s stats, then?\p"
	.string "That will cost {STR_VAR_3} BATTLE POINTS.$"

BattleFrontier_ExchangeServiceCorner_Text_HyperTrainingStatMaxed:
	.string "That stat is already in peak condition!\n"
	.string "There's nothing more I can do with it.$"

BattleFrontier_ExchangeServiceCorner_Text_TrainingStartsNow:
	.string "The training starts now!$"

BattleFrontier_ExchangeServiceCorner_Text_HyperTrainingDone:
	.string "All done!\n"
	.string "Your {STR_VAR_1} looks mightily trained.$"

BattleFrontier_ExchangeServiceCorner_Text_DoMoreHyperTraining:
	.string "Want to do some more HYPER TRAINING?$"

BattleFrontier_ExchangeServiceCorner_Text_KissPosterDesc::
	.string "A large poster with a SMOOCHUM print.$"

BattleFrontier_ExchangeServiceCorner_Text_KissCushionDesc::
	.string "A SMOOCHUM cushion.\n"
	.string "Place it on a mat or a desk.$"

BattleFrontier_ExchangeServiceCorner_Text_SmoochumDollDesc::
	.string "A SMOOCHUM DOLL.\n"
	.string "Place it on a mat or a desk.$"

BattleFrontier_ExchangeServiceCorner_Text_TogepiDollDesc::
	.string "A TOGEPI DOLL.\n"
	.string "Place it on a mat or a desk.$"

BattleFrontier_ExchangeServiceCorner_Text_MeowthDollDesc::
	.string "A MEOWTH DOLL.\n"
	.string "Place it on a mat or a desk.$"

BattleFrontier_ExchangeServiceCorner_Text_ClefairyDollDesc::
	.string "A CLEFAIRY DOLL.\n"
	.string "Place it on a mat or a desk.$"

BattleFrontier_ExchangeServiceCorner_Text_DittoDollDesc::
	.string "A DITTO DOLL.\n"
	.string "Place it on a mat or a desk.$"

BattleFrontier_ExchangeServiceCorner_Text_CyndaquilDollDesc::
	.string "A CYNDAQUIL DOLL.\n"
	.string "Place it on a mat or a desk.$"

BattleFrontier_ExchangeServiceCorner_Text_ChikoritaDollDesc::
	.string "A CHIKORITA DOLL.\n"
	.string "Place it on a mat or a desk.$"

BattleFrontier_ExchangeServiceCorner_Text_TotodileDollDesc::
	.string "A TOTODILE DOLL.\n"
	.string "Place it on a mat or a desk.$"

BattleFrontier_ExchangeServiceCorner_Text_LargeDollDesc::
	.string "A large DOLL.\n"
	.string "Place it on a mat or a desk.$"

BattleFrontier_ExchangeServiceCorner_Text_ProteinDesc::
	.string "Raises the stat ATTACK of one\n"
	.string "POKéMON.$"

BattleFrontier_ExchangeServiceCorner_Text_CalciumDesc::
	.string "Raises the stat SP. ATK of one\n"
	.string "POKéMON.$"

BattleFrontier_ExchangeServiceCorner_Text_IronDesc::
	.string "Raises the stat DEFENSE of one\n"
	.string "POKéMON.$"

BattleFrontier_ExchangeServiceCorner_Text_ZincDesc::
	.string "Raises the stat SP. DEF of one\n"
	.string "POKéMON.$"

BattleFrontier_ExchangeServiceCorner_Text_CarbosDesc::
	.string "Raises the stat SPEED of one\n"
	.string "POKéMON.$"

BattleFrontier_ExchangeServiceCorner_Text_HPUpDesc::
	.string "Raises the HP of one POKéMON.$"

BattleFrontier_ExchangeServiceCorner_Text_MachoBraceDesc::
	.string "A hold item that promotes growth,\n"
	.string "but it reduces SPEED.$"

BattleFrontier_ExchangeServiceCorner_Text_AbilityCapsuleDesc::
	.string "A capsule that swaps the ABILITY of\n"
	.string "a POKéMON.$"

BattleFrontier_ExchangeServiceCorner_Text_RareCandyDesc::
	.string "Raises the level of a POKéMON by one.$"

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
	.string "A hold item that raises SP. ATK in a\n"
	.string "pinch.$"

BattleFrontier_ExchangeServiceCorner_Text_ApicotBerryDesc::
	.string "A hold item that raises SP. DEF in a\n"
	.string "pinch.$"

BattleFrontier_ExchangeServiceCorner_Text_LansatBerryDesc::
	.string "A hold item that ups the critical-hit\n"
	.string "rate in a pinch.$"

BattleFrontier_ExchangeServiceCorner_Text_StarfBerryDesc::
	.string "A hold item that sharply boosts a\n"
	.string "stat in a pinch.$"

BattleFrontier_ExchangeServiceCorner_Text_EverstoneDesc::
	.string "A wondrous hold item that prevents\n"
	.string "evolution.$"

BattleFrontier_ExchangeServiceCorner_Text_WhiteHerbDesc::
	.string "A hold item that restores any\n"
	.string "lowered stat.$"

BattleFrontier_ExchangeServiceCorner_Text_MentalHerbDesc::
	.string "A hold item that snaps POKéMON out\n"
	.string "of infatuation.$"

BattleFrontier_ExchangeServiceCorner_Text_KingsRockDesc::
	.string "A hold item that may cause flinching\n"
	.string "when the foe is hit.$"

BattleFrontier_ExchangeServiceCorner_Text_UpGradeDesc::
	.string "A peculiar box made by SILPH CO.$"

BattleFrontier_ExchangeServiceCorner_Text_DeepSeaToothDesc::
	.string "A hold item that raises the SP. ATK\n"
	.string "of CLAMPERL.$"

BattleFrontier_ExchangeServiceCorner_Text_DeepSeaScaleDesc::
	.string "A hold item that raises the SP. DEF\n"
	.string "of CLAMPERL.$"

BattleFrontier_ExchangeServiceCorner_Text_DragonScaleDesc::
	.string "A strange scale held by DRAGON-type\n"
	.string "POKéMON.$"

BattleFrontier_ExchangeServiceCorner_Text_LeftoversDesc::
	.string "A hold item that gradually restores\n"
	.string "HP in battle.$"

BattleFrontier_ExchangeServiceCorner_Text_QuickClawDesc::
	.string "A hold item that occasionally allows\n"
	.string "the first strike.$"

BattleFrontier_ExchangeServiceCorner_Text_BrightpowderDesc::
	.string "A hold item that casts a glare to\n"
	.string "reduce accuracy.$"

BattleFrontier_ExchangeServiceCorner_Text_SootheBellDesc::
	.string "A hold item that calms spirits and\n"
	.string "fosters friendship.$"

BattleFrontier_ExchangeServiceCorner_Text_ChoiceBandDesc::
	.string "Raises a move's power, but permits\n"
	.string "only that move.$"

BattleFrontier_ExchangeServiceCorner_Text_FocusBandDesc::
	.string "A hold item that occasionally\n"
	.string "prevents fainting.$"

BattleFrontier_ExchangeServiceCorner_Text_ScopeLensDesc::
	.string "A hold item that raises the\n"
	.string "critical-hit rate.$"
