LilycoveCity_DepartmentStore_1F_EventScript_QuizLady::
	lock
	faceplayer
	msgbox LilycoveCity_DepartmentStore_1F_Text_ImTheQuizLady, MSGBOX_DEFAULT
	specialvar VAR_RESULT, GetQuizLadyState
	goto_if_eq VAR_RESULT, LILYCOVE_LADY_STATE_COMPLETED, LilycoveCity_DepartmentStore_1F_EventScript_CheckMadeQuiz
	goto_if_eq VAR_RESULT, LILYCOVE_LADY_STATE_PRIZE, LilycoveCity_DepartmentStore_1F_EventScript_ReadyGivePrize
	specialvar VAR_RESULT, GetQuizAuthor
	goto_if_eq VAR_RESULT, QUIZ_AUTHOR_PLAYER, LilycoveCity_DepartmentStore_1F_EventScript_WaitingToTakeYourQuiz
	goto_if_eq VAR_RESULT, QUIZ_AUTHOR_OTHER_PLAYER, LilycoveCity_DepartmentStore_1F_EventScript_PlayerQuizReady
	msgbox LilycoveCity_DepartmentStore_1F_Text_WaitingForChallenger, MSGBOX_DEFAULT
	goto LilycoveCity_DepartmentStore_1F_EventScript_AskTakeQuiz

LilycoveCity_DepartmentStore_1F_EventScript_CheckMadeQuiz::
	specialvar VAR_RESULT, IsQuizLadyWaitingForChallenger
	goto_if_eq VAR_RESULT, FALSE, LilycoveCity_DepartmentStore_1F_EventScript_AskMakeQuiz
LilycoveCity_DepartmentStore_1F_EventScript_WaitingToTakeYourQuiz::
	msgbox LilycoveCity_DepartmentStore_1F_Text_WaitingToTakeYourQuiz, MSGBOX_DEFAULT
	release
	end

LilycoveCity_DepartmentStore_1F_EventScript_PlayerQuizReady::
	msgbox LilycoveCity_DepartmentStore_1F_Text_WaitingForChallenger, MSGBOX_DEFAULT
LilycoveCity_DepartmentStore_1F_EventScript_AskTakeQuiz::
	setvar VAR_0x8004, 0
	msgbox LilycoveCity_DepartmentStore_1F_Text_TakeQuizChallenge, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, LilycoveCity_DepartmentStore_1F_EventScript_TakeQuiz
	msgbox LilycoveCity_DepartmentStore_1F_Text_HowBoringBye, MSGBOX_DEFAULT
	release
	end

LilycoveCity_DepartmentStore_1F_EventScript_ShowQuestion::
	special QuizLadyShowQuizQuestion
	waitstate
	goto LilycoveCity_DepartmentStore_1F_EventScript_CheckQuizTakingState

LilycoveCity_DepartmentStore_1F_EventScript_GetAnswer::
	special QuizLadyGetPlayerAnswer
	waitstate
	goto LilycoveCity_DepartmentStore_1F_EventScript_CheckQuizTakingState

LilycoveCity_DepartmentStore_1F_EventScript_AskQuitQuiz::
	msgbox LilycoveCity_DepartmentStore_1F_Text_YoureGoingToQuit, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, LilycoveCity_DepartmentStore_1F_EventScript_QuitTakingQuiz
LilycoveCity_DepartmentStore_1F_EventScript_TakeQuiz::
	special ClearQuizLadyPlayerAnswer
	goto_if_eq VAR_0x8004, 0, LilycoveCity_DepartmentStore_1F_EventScript_ShowQuestion
	goto_if_eq VAR_0x8004, EASY_CHAT_TYPE_QUIZ_ANSWER, LilycoveCity_DepartmentStore_1F_EventScript_GetAnswer

LilycoveCity_DepartmentStore_1F_EventScript_CheckQuizTakingState::
	goto_if_eq VAR_RESULT, 0, LilycoveCity_DepartmentStore_1F_EventScript_AskQuitQuiz
	goto_if_eq VAR_RESULT, 1, LilycoveCity_DepartmentStore_1F_EventScript_SubmitQuizResponse
	end

LilycoveCity_DepartmentStore_1F_EventScript_QuitTakingQuiz::
	msgbox LilycoveCity_DepartmentStore_1F_Text_TakeTheQuizAnotherTime, MSGBOX_DEFAULT
	release
	end

LilycoveCity_DepartmentStore_1F_EventScript_SubmitQuizResponse::
	special SetQuizLadyState_Complete
	msgbox LilycoveCity_DepartmentStore_1F_Text_WaitForAnswer, MSGBOX_DEFAULT
	specialvar VAR_RESULT, IsQuizAnswerCorrect
	goto_if_eq VAR_RESULT, FALSE, LilycoveCity_DepartmentStore_1F_EventScript_IncorrectResponse
	playse SE_SUCCESS
	delay 10
	playse SE_SUCCESS
	msgbox LilycoveCity_DepartmentStore_1F_Text_YouGotItRight, MSGBOX_DEFAULT
	goto LilycoveCity_DepartmentStore_1F_EventScript_ReadyGivePrize
	end

LilycoveCity_DepartmentStore_1F_EventScript_IncorrectResponse::
	special BufferQuizCorrectAnswer
	special BufferQuizPrizeName
	playse SE_FAILURE
	delay 10
	playse SE_FAILURE
	msgbox LilycoveCity_DepartmentStore_1F_Text_WrongTheCorrectAnswerIs, MSGBOX_DEFAULT
	msgbox LilycoveCity_DepartmentStore_1F_Text_IGetToKeepPrize, MSGBOX_DEFAULT
	goto LilycoveCity_DepartmentStore_1F_EventScript_AskMakeQuiz
	end

LilycoveCity_DepartmentStore_1F_EventScript_ReadyGivePrize::
	specialvar VAR_RESULT, BufferQuizAuthorNameAndCheckIfLady
	goto_if_eq VAR_RESULT, 0, LilycoveCity_DepartmentStore_1F_EventScript_GivePrizePlayer
	msgbox LilycoveCity_DepartmentStore_1F_Text_YouGotItRightYouveWonLadyPrize, MSGBOX_DEFAULT
	goto LilycoveCity_DepartmentStore_1F_EventScript_GivePrize
	end

LilycoveCity_DepartmentStore_1F_EventScript_GivePrizePlayer::
	msgbox LilycoveCity_DepartmentStore_1F_Text_YouGotItRightYouveWonPersonsPrize, MSGBOX_DEFAULT
LilycoveCity_DepartmentStore_1F_EventScript_GivePrize::
	setvar VAR_0x8005, 0
	special BufferQuizPrizeItem
	special SetQuizLadyState_Complete
	giveitem VAR_0x8005
	goto_if_eq VAR_RESULT, FALSE, LilycoveCity_DepartmentStore_1F_EventScript_NoSpaceForQuizPrize
LilycoveCity_DepartmentStore_1F_EventScript_AskMakeQuiz::
	msgbox LilycoveCity_DepartmentStore_1F_Text_MakeYourOwnQuiz, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, LilycoveCity_DepartmentStore_1F_EventScript_DeclineMakeQuiz
LilycoveCity_DepartmentStore_1F_EventScript_MakeQuiz::
	msgbox LilycoveCity_DepartmentStore_1F_Text_PickYourPrize, MSGBOX_DEFAULT
LilycoveCity_DepartmentStore_1F_EventScript_PickPrize::
	fadescreen FADE_TO_BLACK
	setvar VAR_RESULT, 0
	special Script_QuizLadyOpenBagMenu
	waitstate
	goto_if_eq VAR_RESULT, 1, LilycoveCity_DepartmentStore_1F_EventScript_WriteQuiz
	msgbox LilycoveCity_DepartmentStore_1F_Text_QuitChoosingPrize, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, LilycoveCity_DepartmentStore_1F_EventScript_DeclineMakeQuiz
	goto LilycoveCity_DepartmentStore_1F_EventScript_PickPrize

LilycoveCity_DepartmentStore_1F_EventScript_NoSpaceForQuizPrize::
	msgbox LilycoveCity_DepartmentStore_1F_Text_YourBagIsFilledUp, MSGBOX_DEFAULT
	special SetQuizLadyState_GivePrize
	release
	end

LilycoveCity_DepartmentStore_1F_EventScript_WriteQuiz::
	msgbox LilycoveCity_DepartmentStore_1F_Text_WriteYourQuiz, MSGBOX_DEFAULT
	special ClearQuizLadyQuestionAndAnswer
	special ClearQuizLadyPlayerAnswer
	setvar VAR_0x8004, EASY_CHAT_TYPE_QUIZ_QUESTION
LilycoveCity_DepartmentStore_1F_EventScript_WriteQuizQuestion::
	fadescreen FADE_TO_BLACK
	special QuizLadySetCustomQuestion
	waitstate
	goto_if_eq VAR_RESULT, 0, LilycoveCity_DepartmentStore_1F_EventScript_AskQuitWritingQuizQuestion
	goto LilycoveCity_DepartmentStore_1F_EventScript_FinishMakingQuiz
	end

LilycoveCity_DepartmentStore_1F_EventScript_AskQuitWritingQuizQuestion::
	msgbox LilycoveCity_DepartmentStore_1F_Text_QuitWritingQuizQuestion, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, LilycoveCity_DepartmentStore_1F_EventScript_WriteQuizQuestion
LilycoveCity_DepartmentStore_1F_EventScript_DeclineMakeQuiz::
	special QuizLadyPickNewQuestion
	msgbox LilycoveCity_DepartmentStore_1F_Text_MaybeNextTime, MSGBOX_DEFAULT
	release
	end

LilycoveCity_DepartmentStore_1F_EventScript_FinishMakingQuiz::
	special QuizLadyTakePrizeForCustomQuiz
	special QuizLadyRecordCustomQuizData
	special QuizLadySetWaitingForChallenger
	msgbox LilycoveCity_DepartmentStore_1F_Text_IllLookForAChallenger, MSGBOX_DEFAULT
	release
	end

LilycoveCity_DepartmentStore_1F_Text_ImTheQuizLady:
	.string "I'm the QUIZ LADY!\n"
	.string "I love quizzes!$"

LilycoveCity_DepartmentStore_1F_Text_WaitingToTakeYourQuiz:
	.string "Oh?\p"
	.string "I'm waiting for a challenger to answer\n"
	.string "the quiz you made.\p"
	.string "We can chat another time, okay?$"

LilycoveCity_DepartmentStore_1F_Text_WaitingForChallenger:
	.string "I'm waiting for someone to challenge\n"
	.string "a quiz this {STR_VAR_1} thought up!$"

LilycoveCity_DepartmentStore_1F_Text_TakeQuizChallenge:
	.string "If you answer correctly, you can win\n"
	.string "fabulous prizes!\p"
	.string "Would you like to take the quiz\n"
	.string "challenge?$"

LilycoveCity_DepartmentStore_1F_Text_WaitForAnswer:
	.string "… … … … … …\n"
	.string "… … … … … …$"

LilycoveCity_DepartmentStore_1F_Text_HowBoringBye:
	.string "Oh, how boring!\n"
	.string "Bye-bye!$"

LilycoveCity_DepartmentStore_1F_Text_YoureGoingToQuit:
	.string "Awww!\n"
	.string "You're going to quit?$"

LilycoveCity_DepartmentStore_1F_Text_TakeTheQuizAnotherTime:
	.string "Please take the quiz challenge\n"
	.string "another time!$"

LilycoveCity_DepartmentStore_1F_Text_YouGotItRight:
	.string "You're amazing! You've got it right!\n"
	.string "You're one sharp customer!$"

LilycoveCity_DepartmentStore_1F_Text_YouGotItRightYouveWonLadyPrize:
	.string "Congratulations!\n"
	.string "You've got the quiz right!\p"
	.string "You've won a prize provided by me!$"

LilycoveCity_DepartmentStore_1F_Text_YouGotItRightYouveWonPersonsPrize:
	.string "Congratulations!\n"
	.string "You've got the quiz right!\p"
	.string "You've won a prize provided by\n"
	.string "{STR_VAR_1}!$"

LilycoveCity_DepartmentStore_1F_Text_YourBagIsFilledUp:
	.string "Oh? Your BAG is filled up!\n"
	.string "Come see me when you have room.$"

LilycoveCity_DepartmentStore_1F_Text_WrongTheCorrectAnswerIs:
	.string "Hmm… Wrong!\n"
	.string "The correct answer is “{STR_VAR_3}”!$"

LilycoveCity_DepartmentStore_1F_Text_IGetToKeepPrize:
	.string "Too bad!\p"
	.string "I get to keep the quiz prize\n"
	.string "{STR_VAR_1} now!$"

LilycoveCity_DepartmentStore_1F_Text_MakeYourOwnQuiz:
	.string "Listen, listen!\n"
	.string "Would you like to make your own quiz?$"

LilycoveCity_DepartmentStore_1F_Text_MaybeNextTime:
	.string "Oh, I see…\n"
	.string "Well, maybe next time!$"

LilycoveCity_DepartmentStore_1F_Text_PickYourPrize:
	.string "Okay, the first thing you have to do\n"
	.string "is pick the prize for the person that\l"
	.string "answers your quiz correctly.\p"
	.string "But beware, if the person taking\n"
	.string "the quiz can't get it right, I get to\l"
	.string "keep the prize!$"

LilycoveCity_DepartmentStore_1F_Text_QuitChoosingPrize:
	.string "If you don't choose a prize,\n"
	.string "your quiz can't be made.\p"
	.string "Are you going to quit making\n"
	.string "your quiz?$"

LilycoveCity_DepartmentStore_1F_Text_WriteYourQuiz:
	.string "Oh, how nice!\n"
	.string "That's a wonderful prize!\p"
	.string "Next, you need to write your quiz\n"
	.string "question and its answer.$"

LilycoveCity_DepartmentStore_1F_Text_QuitWritingQuizQuestion:
	.string "Are you going to quit writing\n"
	.string "your quiz question?$"

LilycoveCity_DepartmentStore_1F_Text_IllLookForAChallenger:
	.string "Thank you!\n"
	.string "You've put together a nice quiz.\p"
	.string "I'll go look for someone who'll take\n"
	.string "your quiz challenge right away.$"
