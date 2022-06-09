#include "global.h"
#include "main.h"
#include "overworld.h"
#include "fldeff.h"
#include "field_specials.h"
#include "pokeblock.h"
#include "event_data.h"
#include "script.h"
#include "random.h"
#include "string_util.h"
#include "item.h"
#include "constants/items.h"
#include "item_menu.h"
#include "text.h"
#include "easy_chat.h"
#include "lilycove_lady.h"
#include "contest.h"
#include "strings.h"
#include "constants/lilycove_lady.h"

#include "data/lilycove_lady.h"

static void QuizLadyPickQuestion(void);
static void FavorLadyPickFavorAndBestItem(void);
static void InitLilycoveFavorLady(void);
static void ResetQuizLadyForRecordMix(void);
static void ResetFavorLadyForRecordMix(void);
static u8 BufferQuizAuthorName(void);
static bool8 IsQuizTrainerIdNotPlayer(void);
static u8 GetPlayerNameLength(const u8 *);

static EWRAM_DATA struct LilycoveLady *sLilycoveLadyPtr = NULL;

extern EWRAM_DATA u16 gSpecialVar_ItemId;

u8 GetLilycoveLadyId(void)
{
    return gSaveBlock1Ptr->lilycoveLady.id;
}

void SetLilycoveLadyGfx(void)
{
    VarSet(VAR_OBJ_GFX_ID_0, sContestLadyMonGfxId[gSaveBlock1Ptr->contestLady.category]);
}

void InitLilycoveLady(void)
{
    InitLilycoveQuizLady();
    InitLilycoveFavorLady();
    gSaveBlock1Ptr->contestLady.givenPokeblock = FALSE;
    ResetContestLadyContestData();
}

static void InitLilycoveFavorLady(void)
{    
    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;

    sLilycoveLadyPtr->hackId = LILYCOVE_LADY_FAVOR;
    sLilycoveLadyPtr->favorState = LILYCOVE_LADY_STATE_READY;
    sLilycoveLadyPtr->favorPlayerName[0] = EOS;
    sLilycoveLadyPtr->likedItem = FALSE;
    sLilycoveLadyPtr->numItemsGiven = 0;
    sLilycoveLadyPtr->itemId = ITEM_NONE;
    sLilycoveLadyPtr->favorLanguage = gGameLanguage;
    FavorLadyPickFavorAndBestItem();
}

void ResetLilycoveLadyForRecordMix(void)
{
    ResetQuizLadyForRecordMix();
    ResetFavorLadyForRecordMix();
}

void Script_GetLilycoveLadyId(void)
{
    gSpecialVar_Result = GetLilycoveLadyId();
}

static u8 GetNumAcceptedItems(const u16 *itemsArray)
{
    u8 numItems;

    for (numItems = 0; *itemsArray != ITEM_NONE; numItems++, itemsArray++);
    return numItems;
}

static void FavorLadyPickFavorAndBestItem(void)
{
    u8 numItems;
    u8 bestItem;

    sLilycoveLadyPtr->favorId = Random() % ARRAY_COUNT(sFavorLadyRequests);
    numItems = GetNumAcceptedItems(sFavorLadyAcceptedItemLists[sLilycoveLadyPtr->favorId]);
    bestItem = Random() % numItems;
    sLilycoveLadyPtr->bestItem = sFavorLadyAcceptedItemLists[sLilycoveLadyPtr->favorId][bestItem];
}

static void ResetFavorLadyForRecordMix(void)
{
    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    sLilycoveLadyPtr->favorState = LILYCOVE_LADY_STATE_READY;
}

u8 GetFavorLadyState(void)
{
    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    if (sLilycoveLadyPtr->favorState == LILYCOVE_LADY_STATE_PRIZE)
        return LILYCOVE_LADY_STATE_PRIZE;
    else if (sLilycoveLadyPtr->favorState == LILYCOVE_LADY_STATE_COMPLETED)
        return LILYCOVE_LADY_STATE_COMPLETED;
    else
        return LILYCOVE_LADY_STATE_READY;
}

static const u8 *GetFavorLadyRequest(u8 idx)
{
    return sFavorLadyRequests[idx];
}

void BufferFavorLadyRequest(void)
{
    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    StringCopy(gStringVar1, GetFavorLadyRequest(sLilycoveLadyPtr->favorId));
}

bool8 HasAnotherPlayerGivenFavorLadyItem(void)
{
    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    if (sLilycoveLadyPtr->favorPlayerName[0] != EOS)
    {
        StringCopy_PlayerName(gStringVar3, sLilycoveLadyPtr->favorPlayerName);
        ConvertInternationalString(gStringVar3, sLilycoveLadyPtr->favorLanguage);
        return TRUE;
    }
    return FALSE;
}

static void BufferItemName(u8 *dest, u16 itemId)
{
    StringCopy(dest, ItemId_GetName(itemId));
}

void BufferFavorLadyItemName(void)
{
    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    BufferItemName(gStringVar2, sLilycoveLadyPtr->itemId);
}

static void SetFavorLadyPlayerName(const u8 *src, u8 *dest)
{
    memset(dest, EOS, PLAYER_NAME_LENGTH + 1);
    StringCopy_PlayerName(dest, src);
}

void BufferFavorLadyPlayerName(void)
{
    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    SetFavorLadyPlayerName(sLilycoveLadyPtr->favorPlayerName, gStringVar3);
    ConvertInternationalString(gStringVar3, sLilycoveLadyPtr->favorLanguage);
}

// Only used to determine if a record-mixed player had given her an item she liked
bool8 DidFavorLadyLikeItem(void)
{
    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    return sLilycoveLadyPtr->likedItem ? TRUE : FALSE;
}

void Script_FavorLadyOpenBagMenu(void)
{
    FavorLadyOpenBagMenu();
}

static bool8 DoesFavorLadyLikeItem(u16 itemId)
{
    u8 numItems;
    u8 i;
    bool8 likedItem;

    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    numItems = GetNumAcceptedItems(sFavorLadyAcceptedItemLists[sLilycoveLadyPtr->favorId]);
    sLilycoveLadyPtr->favorState = LILYCOVE_LADY_STATE_COMPLETED;
    BufferItemName(gStringVar2, itemId);
    sLilycoveLadyPtr->itemId = itemId;
    SetFavorLadyPlayerName(gSaveBlock2Ptr->playerName, sLilycoveLadyPtr->favorPlayerName);
    sLilycoveLadyPtr->favorLanguage = gGameLanguage;
    likedItem = FALSE;
    for (i = 0; i < numItems; i ++)
    {
        if (sFavorLadyAcceptedItemLists[sLilycoveLadyPtr->favorId][i] == itemId)
        {
            likedItem = TRUE;
            sLilycoveLadyPtr->numItemsGiven++;
            sLilycoveLadyPtr->likedItem = TRUE;
            if (sLilycoveLadyPtr->bestItem == itemId)
                sLilycoveLadyPtr->numItemsGiven = LILYCOVE_LADY_GIFT_THRESHOLD;
            break;
        }
        sLilycoveLadyPtr->likedItem = FALSE;
    }
    return likedItem;
}

bool8 Script_DoesFavorLadyLikeItem(void)
{
    return DoesFavorLadyLikeItem(gSpecialVar_ItemId);
}

bool8 IsFavorLadyThresholdMet(void)
{
    u8 numItemsGiven;

    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    numItemsGiven = sLilycoveLadyPtr->numItemsGiven;
    return numItemsGiven < LILYCOVE_LADY_GIFT_THRESHOLD ? FALSE : TRUE;
}

static void FavorLadyBufferPrizeName(u16 prize)
{
    BufferItemName(gStringVar2, prize);
}

u16 FavorLadyGetPrize(void)
{
    u16 prize;

    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    prize = sFavorLadyPrizes[sLilycoveLadyPtr->favorId];
    FavorLadyBufferPrizeName(prize);
    sLilycoveLadyPtr->favorState = LILYCOVE_LADY_STATE_PRIZE;
    return prize;
}

void SetFavorLadyState_Complete(void)
{
    InitLilycoveFavorLady();
    sLilycoveLadyPtr->favorState = LILYCOVE_LADY_STATE_COMPLETED;
}

void FieldCallback_FavorLadyEnableScriptContexts(void)
{
    EnableBothScriptContexts();
}

static void QuizLadyPickQuestion(void)
{
    u8 questionId;
    u8 i;

    questionId = Random() % ARRAY_COUNT(sQuizLadyQuizQuestions);
    for (i = 0; i < QUIZ_QUESTION_LEN; i ++)
        sLilycoveLadyPtr->question[i] = sQuizLadyQuizQuestions[questionId][i];
    sLilycoveLadyPtr->correctAnswer = sQuizLadyQuizAnswers[questionId];
    sLilycoveLadyPtr->prize = sQuizLadyPrizes[questionId];
    sLilycoveLadyPtr->questionId = questionId;
    sLilycoveLadyPtr->playerName[0] = EOS;
}

void InitLilycoveQuizLady(void)
{
    u8 i;

    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    sLilycoveLadyPtr->id = LILYCOVE_LADY_QUIZ;
    sLilycoveLadyPtr->state = LILYCOVE_LADY_STATE_READY;

    for (i = 0; i < QUIZ_QUESTION_LEN; i ++)
        sLilycoveLadyPtr->question[i] = EC_EMPTY_WORD;

    sLilycoveLadyPtr->correctAnswer = EC_EMPTY_WORD;
    sLilycoveLadyPtr->playerAnswer = EC_EMPTY_WORD;

    for (i = 0; i < TRAINER_ID_LENGTH; i ++)
        sLilycoveLadyPtr->playerTrainerId[i] = 0;

    sLilycoveLadyPtr->prize = ITEM_NONE;
    sLilycoveLadyPtr->waitingForChallenger = FALSE;
    sLilycoveLadyPtr->prevQuestionId = ARRAY_COUNT(sQuizLadyQuizQuestions);
    sLilycoveLadyPtr->language = gGameLanguage;
    QuizLadyPickQuestion();
}

static void ResetQuizLadyForRecordMix(void)
{
    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    sLilycoveLadyPtr->id = LILYCOVE_LADY_QUIZ;
    sLilycoveLadyPtr->state = LILYCOVE_LADY_STATE_READY;
    sLilycoveLadyPtr->waitingForChallenger = FALSE;
    sLilycoveLadyPtr->playerAnswer = EC_EMPTY_WORD;
}

u8 GetQuizLadyState(void)
{
    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    if (sLilycoveLadyPtr->state == LILYCOVE_LADY_STATE_PRIZE)
        return LILYCOVE_LADY_STATE_PRIZE;
    else if (sLilycoveLadyPtr->state == LILYCOVE_LADY_STATE_COMPLETED)
        return LILYCOVE_LADY_STATE_COMPLETED;
    else
        return LILYCOVE_LADY_STATE_READY;
}

u8 GetQuizAuthor(void)
{
    s32 i, j;
    u8 authorNameId;
    struct LilycoveLady *quiz = &gSaveBlock1Ptr->lilycoveLady;

    if (IsEasyChatAnswerUnlocked(quiz->correctAnswer) == FALSE)
    {
        i = quiz->questionId;
        do
        {
            if (++i >= (int)ARRAY_COUNT(sQuizLadyQuizQuestions))
                i = 0;
        } while (IsEasyChatAnswerUnlocked(sQuizLadyQuizAnswers[i]) == FALSE);

        for (j = 0; j < QUIZ_QUESTION_LEN; j++)
            quiz->question[j] = sQuizLadyQuizQuestions[i][j];
        quiz->correctAnswer = sQuizLadyQuizAnswers[i];
        quiz->prize = sQuizLadyPrizes[i];
        quiz->questionId = i;
        quiz->playerName[0] = EOS;
    }
    authorNameId = BufferQuizAuthorName();
    if (authorNameId == QUIZ_AUTHOR_NAME_LADY)
        return QUIZ_AUTHOR_LADY;
    else if (authorNameId == QUIZ_AUTHOR_NAME_OTHER_PLAYER || IsQuizTrainerIdNotPlayer())
        return QUIZ_AUTHOR_OTHER_PLAYER;
    else
        return QUIZ_AUTHOR_PLAYER;
}

static u8 BufferQuizAuthorName(void)
{
    u8 authorNameId;
    u8 nameLen;
    u8 i;

    authorNameId = QUIZ_AUTHOR_NAME_PLAYER;
    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    if (sLilycoveLadyPtr->playerName[0] == EOS)
    {
        StringCopy_PlayerName(gStringVar1, gText_QuizLady_Lady);
        authorNameId = QUIZ_AUTHOR_NAME_LADY;
    }
    else
    {
        StringCopy_PlayerName(gStringVar1, sLilycoveLadyPtr->playerName);
        ConvertInternationalString(gStringVar1, sLilycoveLadyPtr->language);
        nameLen = GetPlayerNameLength(sLilycoveLadyPtr->playerName);
        if (nameLen == GetPlayerNameLength(gSaveBlock2Ptr->playerName))
        {
            u8 *name = sLilycoveLadyPtr->playerName;
            for (i = 0; i < nameLen; i++)
            {
                name = sLilycoveLadyPtr->playerName;
                if (name[i] != gSaveBlock2Ptr->playerName[i])
                {
                    authorNameId = QUIZ_AUTHOR_NAME_OTHER_PLAYER;
                    break;
                }
            }
        }

    }
    return authorNameId;
}

static bool8 IsQuizTrainerIdNotPlayer(void)
{
    bool8 notPlayer;
    u8 i;

    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    notPlayer = FALSE;
    for (i = 0; i < TRAINER_ID_LENGTH; i++)
    {
        if (sLilycoveLadyPtr->playerTrainerId[i] != gSaveBlock2Ptr->playerTrainerId[i])
        {
            notPlayer = TRUE;
            break;
        }
    }
    return notPlayer;
}

static u8 GetPlayerNameLength(const u8 *playerName)
{
    u8 len;
    const u8 *ptr;

    for (len = 0, ptr = playerName; *ptr != EOS; len++, ptr++);
    return len;
}

void BufferQuizPrizeName(void)
{
    StringCopy(gStringVar1, ItemId_GetName(sLilycoveLadyPtr->prize));
}

bool8 BufferQuizAuthorNameAndCheckIfLady(void)
{
    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    if (BufferQuizAuthorName() == QUIZ_AUTHOR_NAME_LADY)
    {
        sLilycoveLadyPtr->language = gGameLanguage;
        return TRUE;
    }
    return FALSE;
}

bool8 IsQuizLadyWaitingForChallenger(void)
{
    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    return sLilycoveLadyPtr->waitingForChallenger;
}

void QuizLadyGetPlayerAnswer(void)
{
    ShowEasyChatScreen();
}

bool8 IsQuizAnswerCorrect(void)
{
    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    CopyEasyChatWord(gStringVar1, sLilycoveLadyPtr->correctAnswer);
    CopyEasyChatWord(gStringVar2, sLilycoveLadyPtr->playerAnswer);
    return StringCompare(gStringVar1, gStringVar2) ? FALSE : TRUE;
}

void BufferQuizPrizeItem(void)
{
    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    gSpecialVar_0x8005 = sLilycoveLadyPtr->prize;
}

void SetQuizLadyState_Complete(void)
{
    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    sLilycoveLadyPtr->state = LILYCOVE_LADY_STATE_COMPLETED;
}

void SetQuizLadyState_GivePrize(void)
{
    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    sLilycoveLadyPtr->state = LILYCOVE_LADY_STATE_PRIZE;
}

void ClearQuizLadyPlayerAnswer(void)
{
    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    sLilycoveLadyPtr->playerAnswer = EC_EMPTY_WORD;
}

void Script_QuizLadyOpenBagMenu(void)
{
    QuizLadyOpenBagMenu();
}

void QuizLadyPickNewQuestion(void)
{
    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    if (BufferQuizAuthorNameAndCheckIfLady())
        sLilycoveLadyPtr->prevQuestionId = sLilycoveLadyPtr->questionId;
    else
        sLilycoveLadyPtr->prevQuestionId = ARRAY_COUNT(sQuizLadyQuizQuestions);
    QuizLadyPickQuestion();
}

void ClearQuizLadyQuestionAndAnswer(void)
{
    u8 i;

    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    for (i = 0; i < QUIZ_QUESTION_LEN; i++)
        sLilycoveLadyPtr->question[i] = EC_EMPTY_WORD;
    sLilycoveLadyPtr->correctAnswer = EC_EMPTY_WORD;
}

void QuizLadySetCustomQuestion(void)
{
    gSpecialVar_0x8004 = EASY_CHAT_TYPE_QUIZ_SET_QUESTION;
    ShowEasyChatScreen();
}

void QuizLadyTakePrizeForCustomQuiz(void)
{
    RemoveBagItem(gSpecialVar_ItemId, 1);
}

void QuizLadyRecordCustomQuizData(void)
{
    u8 i;

    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    sLilycoveLadyPtr->prize = gSpecialVar_ItemId;
    for (i = 0; i < TRAINER_ID_LENGTH; i++)
        sLilycoveLadyPtr->playerTrainerId[i] = gSaveBlock2Ptr->playerTrainerId[i];
    StringCopy_PlayerName(sLilycoveLadyPtr->playerName, gSaveBlock2Ptr->playerName);
    sLilycoveLadyPtr->language = gGameLanguage;
}

void QuizLadySetWaitingForChallenger(void)
{
    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    sLilycoveLadyPtr->waitingForChallenger = TRUE;
}

void BufferQuizCorrectAnswer(void)
{
    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    CopyEasyChatWord(gStringVar3, sLilycoveLadyPtr->correctAnswer);
}


void FieldCallback_QuizLadyEnableScriptContexts(void)
{
    EnableBothScriptContexts();
}

void QuizLadyClearQuestionForRecordMix(struct LilycoveLady *lilycoveLady)
{
    u8 i;

    sLilycoveLadyPtr = &gSaveBlock1Ptr->lilycoveLady;
    if (lilycoveLady->prevQuestionId < ARRAY_COUNT(sQuizLadyQuizQuestions)
        && sLilycoveLadyPtr->id == LILYCOVE_LADY_QUIZ)
    {
        for (i = 0; i < 4; i++)
        {
            if (lilycoveLady->prevQuestionId != sLilycoveLadyPtr->questionId)
                break;
            sLilycoveLadyPtr->questionId = Random() % ARRAY_COUNT(sQuizLadyQuizQuestions);
        }
        if (lilycoveLady->prevQuestionId == sLilycoveLadyPtr->questionId)
            sLilycoveLadyPtr->questionId = (sLilycoveLadyPtr->questionId + 1) % (int)ARRAY_COUNT(sQuizLadyQuizQuestions);

        sLilycoveLadyPtr->prevQuestionId = lilycoveLady->prevQuestionId;
    }
}

void ResetContestLadyContestData(void)
{
    gSaveBlock1Ptr->contestLady.numGoodPokeblocksGiven = 0;
    gSaveBlock1Ptr->contestLady.numOtherPokeblocksGiven = 0;
    gSaveBlock1Ptr->contestLady.maxSheen = 0;
    gSaveBlock1Ptr->contestLady.category = Random() % CONTEST_CATEGORIES_COUNT;
}

bool8 GivePokeblockToContestLady(struct Pokeblock *pokeblock)
{
    u8 sheen = 0;
    bool8 correctFlavor = FALSE;

    switch (gSaveBlock1Ptr->contestLady.category)
    {
    case CONTEST_CATEGORY_COOL:
        if (pokeblock->spicy != 0)
        {
            sheen = pokeblock->spicy;
            correctFlavor = TRUE;
        }
        break;
    case CONTEST_CATEGORY_BEAUTY:
        if (pokeblock->dry != 0)
        {
            sheen = pokeblock->dry;
            correctFlavor = TRUE;
        }
        break;
    case CONTEST_CATEGORY_CUTE:
        if (pokeblock->sweet != 0)
        {
            sheen = pokeblock->sweet;
            correctFlavor = TRUE;
        }
        break;
    case CONTEST_CATEGORY_SMART:
        if (pokeblock->bitter != 0)
        {
            sheen = pokeblock->bitter;
            correctFlavor = TRUE;
        }
        break;
    case CONTEST_CATEGORY_TOUGH:
        if (pokeblock->sour != 0)
        {
            sheen = pokeblock->sour;
            correctFlavor = TRUE;
        }
        break;
    }
    if (correctFlavor == TRUE)
    {
        gSaveBlock1Ptr->contestLady.maxSheen = sheen;
        gSaveBlock1Ptr->contestLady.numGoodPokeblocksGiven++;
    }
    else if (gSaveBlock1Ptr->contestLady.numOtherPokeblocksGiven < LILYCOVE_LADY_GIFT_THRESHOLD)
        gSaveBlock1Ptr->contestLady.numOtherPokeblocksGiven++;
    return correctFlavor;
}

static void BufferContestLadyCategoryAndMonName(u8 *category, u8 *nickname)
{
    StringCopy(category, sContestLadyCategoryNames[gSaveBlock1Ptr->contestLady.category]);
    StringCopy_Nickname(nickname, sContestLadyMonNames[gSaveBlock1Ptr->contestLady.category]);
}

void BufferContestLadyMonName(u8 *category, u8 *nickname)
{
    *category = gSaveBlock1Ptr->contestLady.category;
    StringCopy(nickname, sContestLadyMonNames[gSaveBlock1Ptr->contestLady.category]);
}

void BufferContestName(u8 *dest, u8 category)
{
    StringCopy(dest, sContestNames[category]);
}

// Used by the Contest Lady's TV show to determine how well she performed
u8 GetContestLadyPokeblockState(void)
{
    if (gSaveBlock1Ptr->contestLady.numGoodPokeblocksGiven >= LILYCOVE_LADY_GIFT_THRESHOLD)
        return CONTEST_LADY_GOOD;
    else if (gSaveBlock1Ptr->contestLady.numGoodPokeblocksGiven == 0)
        return CONTEST_LADY_BAD;
    else
        return CONTEST_LADY_NORMAL;
}


bool8 HasPlayerGivenContestLadyPokeblock(void)
{
    if (gSaveBlock1Ptr->contestLady.givenPokeblock == TRUE)
        return TRUE;
    return FALSE;
}

bool8 ShouldContestLadyShowGoOnAir(void)
{
    bool8 putOnAir = FALSE;

    if (gSaveBlock1Ptr->contestLady.numGoodPokeblocksGiven >= LILYCOVE_LADY_GIFT_THRESHOLD
     || gSaveBlock1Ptr->contestLady.numOtherPokeblocksGiven >= LILYCOVE_LADY_GIFT_THRESHOLD)
        putOnAir = TRUE;

    return putOnAir;
}

void LowerContesetLadyOtherBlockCount(void)
{
    gSaveBlock1Ptr->contestLady.numOtherPokeblocksGiven--;
}

void Script_BufferContestLadyCategoryAndMonName(void)
{
    BufferContestLadyCategoryAndMonName(gStringVar2, gStringVar1);
}

void OpenPokeblockCaseForContestLady(void)
{
    OpenPokeblockCase(PBLOCK_CASE_GIVE, CB2_ReturnToField);
}

void SetContestLadyGivenPokeblock(void)
{
    gSaveBlock1Ptr->contestLady.givenPokeblock = TRUE;
}

void GetContestLadyMonSpecies(void)
{
    gSpecialVar_0x8005 = sContestLadyMonSpecies[gSaveBlock1Ptr->contestLady.category];
}

u8 GetContestLadyCategory(void)
{
    return gSaveBlock1Ptr->contestLady.category;
}
