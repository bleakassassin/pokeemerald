MauvilleCity_BikeShop_MapScripts::
	.byte 0

MauvilleCity_BikeShop_EventScript_Rydel::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_BIKE, MauvilleCity_BikeShop_EventScript_MostOutOfBike
	goto_if_set FLAG_DECLINED_BIKE, MauvilleCity_BikeShop_EventScript_SkipGreeting
	msgbox MauvilleCity_BikeShop_Text_RydelGreeting, MSGBOX_DEFAULT
MauvilleCity_BikeShop_EventScript_SkipGreeting::
	msgbox MauvilleCity_BikeShop_Text_DidYouComeFromFarAway, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, MauvilleCity_BikeShop_EventScript_YesFar
	setflag FLAG_DECLINED_BIKE
	msgbox MauvilleCity_BikeShop_Text_GuessYouDontNeedBike, MSGBOX_DEFAULT
	release
	end

MauvilleCity_BikeShop_EventScript_YesFar::
	setflag FLAG_RECEIVED_BIKE
	msgbox MauvilleCity_BikeShop_Text_GiveYouABike, MSGBOX_DEFAULT
	giveitem ITEM_BICYCLE
	msgbox MauvilleCity_BikeShop_Text_ExplainBike, MSGBOX_DEFAULT
	special SwapRegisteredBike
	release
	end

MauvilleCity_BikeShop_EventScript_MostOutOfBike::
	msgbox MauvilleCity_BikeShop_Text_MostOutOfBike, MSGBOX_NPC
	end

MauvilleCity_BikeShop_EventScript_Assistant::
	msgbox MauvilleCity_BikeShop_Text_HandbooksAreInBack, MSGBOX_NPC
	end

MauvilleCity_BikeShop_EventScript_MachBikeHandbook::
	message MauvilleCity_BikeShop_Text_MachHandbookWhichPage
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseMachHandbookPage
	end

MauvilleCity_BikeShop_EventScript_ChooseMachHandbookPage::
	multichoice 0, 0, MULTI_MACH_BIKE_INFO, FALSE
	switch VAR_RESULT
	case 0, MauvilleCity_BikeShop_EventScript_HowToRide
	case 1, MauvilleCity_BikeShop_EventScript_HowToTurn
	case 2, MauvilleCity_BikeShop_EventScript_SandySlopes
	case 3, MauvilleCity_BikeShop_EventScript_ExitMachHandbook
	case MULTI_B_PRESSED, MauvilleCity_BikeShop_EventScript_ExitMachHandbook
	end

MauvilleCity_BikeShop_EventScript_HowToRide::
	message MauvilleCity_BikeShop_Text_HowToRideMachBike
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseMachHandbookPage
	end

MauvilleCity_BikeShop_EventScript_HowToTurn::
	message MauvilleCity_BikeShop_Text_HowToTurnMachBike
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseMachHandbookPage
	end

MauvilleCity_BikeShop_EventScript_SandySlopes::
	message MauvilleCity_BikeShop_Text_SandySlopes
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseMachHandbookPage
	end

MauvilleCity_BikeShop_EventScript_ExitMachHandbook::
	release
	end

MauvilleCity_BikeShop_EventScript_AcroBikeHandbook::
	message MauvilleCity_BikeShop_Text_AcroHandbookWhichPage
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseAcroHandbookPage
	end

MauvilleCity_BikeShop_EventScript_ChooseAcroHandbookPage::
	multichoice 0, 0, MULTI_ACRO_BIKE_INFO, FALSE
	switch VAR_RESULT
	case 0, MauvilleCity_BikeShop_EventScript_Wheelies
	case 1, MauvilleCity_BikeShop_EventScript_BunnyHops
	case 2, MauvilleCity_BikeShop_EventScript_Jumps
	case 3, MauvilleCity_BikeShop_EventScript_ExitAcroHandbook
	case MULTI_B_PRESSED, MauvilleCity_BikeShop_EventScript_ExitAcroHandbook
	end

MauvilleCity_BikeShop_EventScript_Wheelies::
	message MauvilleCity_BikeShop_Text_Wheelies
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseAcroHandbookPage
	end

MauvilleCity_BikeShop_EventScript_BunnyHops::
	message MauvilleCity_BikeShop_Text_BunnyHops
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseAcroHandbookPage
	end

MauvilleCity_BikeShop_EventScript_Jumps::
	message MauvilleCity_BikeShop_Text_Jumps
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseAcroHandbookPage
	end

MauvilleCity_BikeShop_EventScript_ExitAcroHandbook::
	release
	end

MauvilleCity_BikeShop_Text_RydelGreeting:
	.string "Well, well, what have we here?\n"
	.string "A most energetic customer!\p"
	.string "Me? You may call me Rydel.\n"
	.string "I'm the owner of this cycle shop.$"

MauvilleCity_BikeShop_Text_DidYouComeFromFarAway:
	.string "Rydel: Your Running Shoes…\n"
	.string "They're awfully filthy.\p"
	.string "Did you come from far away?$"

MauvilleCity_BikeShop_Text_GuessYouDontNeedBike:
	.string "Rydel: Is that right?\p"
	.string "Then, I guess you have no need for\n"
	.string "any of my Bikes.$"

MauvilleCity_BikeShop_Text_GiveYouABike:
	.string "Rydel: Hm, hm… … … … …\p"
	.string "You're saying that you came all this\n"
	.string "way from Littleroot?\p"
	.string "My goodness!\n"
	.string "That's ridiculously far!\p"
	.string "If you had one of my Bikes, you could\n"
	.string "go anywhere easily while feeling the\l"
	.string "gentle caress of the wind!\p"
	.string "I'll tell you what!\n"
	.string "I'll give you a Bike!$"

MauvilleCity_BikeShop_Text_ExplainBike:
	.string "Oh, wait a second!\p"
	.string "I forgot to tell you that there are\n"
	.string "two modes for my Bikes!\p"
	.string "They are Mach Gear and Acro Gear!\p"
	.string "Mach Gear is for cyclists who want to\n"
	.string "feel the wind with their bodies!\p"
	.string "And Acro Gear is for those who prefer\n"
	.string "technical rides!\p"
	.string "Be sure to read the handbooks in the"
	.string "back to make the best use of each one!$"
	.string "May the wind always be at your back\n"
	.string "on your adventure!$"

MauvilleCity_BikeShop_Text_MostOutOfBike:
	.string "Rydel: Are you getting the most out of\n"
	.string "that Bike I gave you?\p"
	.string "Both modes come in handy depending on\n"
	.string "the situation!$"

MauvilleCity_BikeShop_Text_HandbooksAreInBack:
	.string "I'm learning about Bikes while\n"
	.string "I work here.\p"
	.string "If you need advice on how to ride your\n"
	.string "Bike, there're a couple handbooks in\l"
	.string "the back.$"

MauvilleCity_BikeShop_Text_MachHandbookWhichPage:
	.string "It's a handbook on Mach Gear.\p"
	.string "Which page do you want to read?$"

MauvilleCity_BikeShop_Text_HowToRideMachBike:
	.string "A Bicycle moves in the direction that\n"
	.string "the + Control Pad is pressed.\p"
	.string "Press the R Button to switcht to\n"
	.string "Mach Gear.\p"
	.string "You can switch back to Acro Gear by\n"
	.string "pressing the R Button again.\p"
	.string "When in Mach Gear, your Bike will\n"
	.string "speed up once it gets rolling.\p"
	.string "To stop, release the + Control Pad.\n"
	.string "The Bike will slow to a stop.\p"
	.string "Want to read a different page?$"

MauvilleCity_BikeShop_Text_HowToTurnMachBike:
	.string "Mach Gear is speedy, but you can't\n"
	.string "stop very quickly in it.\p"
	.string "It gets a little tricky to get around\n"
	.string "a corner.\p"
	.string "Release the + Control Pad a little\n"
	.string "before the corner and slow down.\p"
	.string "Want to read a different page?$"

MauvilleCity_BikeShop_Text_SandySlopes:
	.string "There are small sandy slopes\n"
	.string "throughout the Hoenn region.\p"
	.string "The loose, crumbly sand makes it\n"
	.string "impossible to climb normally.\p"
	.string "But if you're in Mach Gear, you can\n"
	.string "zip up a sandy slope.\p"
	.string "Want to read a different page?$"

MauvilleCity_BikeShop_Text_AcroHandbookWhichPage:
	.string "It's a handbook on Acro Gear.\p"
	.string "Which page do you want to read?$"

MauvilleCity_BikeShop_Text_Wheelies:
	.string "Press the B Button while riding,\n"
	.string "and the front wheel lifts up.\p"
	.string "You can zip around with the front\n"
	.string "wheel up using the + Control Pad.\p"
	.string "This technique is called a wheelie.\p"
	.string "Want to read a different page?$"

MauvilleCity_BikeShop_Text_BunnyHops:
	.string "Keeping the B Button pressed,\n"
	.string "your Bike can hop on the spot.\p"
	.string "This technique is called a bunny hop.\p"
	.string "You can ride while hopping, too.\p"
	.string "Want to read a different page?$"

MauvilleCity_BikeShop_Text_Jumps:
	.string "Press the B Button and the + Control\n"
	.string "Pad at the same time to jump.\p"
	.string "Press the + Control Pad to the side\n"
	.string "to jump sideways.\p"
	.string "Press it backwards to make the Bike\n"
	.string "change directions while jumping.\p"
	.string "Want to read a different page?$"

