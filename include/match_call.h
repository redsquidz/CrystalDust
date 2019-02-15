#ifndef GUARD_MATCH_CALL_H
#define GUARD_MATCH_CALL_H

enum {
    MATCH_CALL_TYPE_NON_TRAINER,
    MATCH_CALL_TYPE_TRAINER,
    MATCH_CALL_TYPE_WALLY,
    MATCH_CALL_TYPE_BIRCH,
    MATCH_CALL_TYPE_MAY_BRENDAN,
    MATCH_CALL_TYPE_GYMLEADER_ELITEFOUR
};

s32 GetRematchIdxByTrainerIdx(s32 trainerIdx);
void InitMatchCallCounters(void);
bool32 SelectMatchCallMessage(int trainerId, u8 *str, bool8 isCallingPlayer);

#endif //GUARD_MATCH_CALL_H
