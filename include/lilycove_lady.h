#ifndef GUARD_LILYCOVE_LADY_H
#define GUARD_LILYCOVE_LADY_H

u8 GetLilycoveLadyId(void);
void InitLilycoveLady(void);
void ResetLilycoveLadyForRecordMix(void);
void InitLilycoveQuizLady(void);
void FieldCallback_FavorLadyEnableScriptContexts(void);
void FieldCallback_QuizLadyEnableScriptContexts(void);
void QuizLadyClearQuestionForRecordMix(struct LilycoveLady *lilycoveLady);
bool8 GivePokeblockToContestLady(struct Pokeblock *pokeblock);
void BufferContestLadyMonName(u8 *dest1, u8 *dest2);
void BufferContestName(u8 *dest, u8 category);
u8 GetContestLadyPokeblockState(void);
void ResetContestLadyContestData(void);
void LowerContesetLadyOtherBlockCount(void);

#endif //GUARD_LILYCOVE_LADY_H
