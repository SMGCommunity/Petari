#pragma once

#include <revolution/types.h>

class AudBgm;
class JAISoundHandle;

class AudBgmRhythmStrategy {
public:
    /// @brief Creates a new `AudBgmRhythmStrategy`.
    AudBgmRhythmStrategy() {};

    virtual bool set(AudBgm*, s32);

    void reject();
    bool setDominant();
    bool isDominant() const;

    /* 0x04 */ s32 mBgmIdx;
    /* 0x08 */ AudBgm* mBgm;
};
