MysteryGiftScript_OldSeaMap::
	setvaddress MysteryGiftScript_OldSeaMap
	lock
	faceplayer
	vgoto_if_set FLAG_RECEIVED_OLD_SEA_MAP, OldSeaMap_Obtained
	removeitem ITEM_OLD_SEA_MAP
	vmessage sText_MysteryGiftOldSeaMapForYou
	waitmessage
	waitbuttonpress
	checkitemspace ITEM_OLD_SEA_MAP
	vgoto_if_eq VAR_RESULT, FALSE, OldSeaMap_NoBagSpace
	goto_if_eq VAR_OLD_SEA_MAP_STATE, 4, OldSeaMap_Give
	setvar VAR_OLD_SEA_MAP_STATE, 4
	setvar VAR_TEMP_1, 1
OldSeaMap_Give:
	setvar VAR_TEMP_0, 1
	giveitem ITEM_OLD_SEA_MAP
	setflag FLAG_HIDE_OCEANIC_MUSEUM_REPORTER
	setflag FLAG_RECEIVED_OLD_SEA_MAP
OldSeaMap_Obtained:
	vmessage MysteryGift_Text_TryUsingItAtLilycovePort
	waitmessage
	waitbuttonpress
	release
	end

OldSeaMap_NoBagSpace:
	vmessage MysteryGift_Text_BagFull
	waitmessage
	waitbuttonpress
	release
	end

sText_MysteryGiftOldSeaMapForYou:
	.string "Let me confirm--you are {PLAYER}?\p"
	.string "We received this OLD SEA MAP\n"
	.string "addressed to you.$"
