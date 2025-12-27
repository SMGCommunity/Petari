#pragma once

#include <revolution/types.h>

class AudBgm;

class AudBgmRhythmStrategy {
public:
    /// @brief Creates a new `AudBgmRhythmStrategy`.
    AudBgmRhythmStrategy();

    virtual void set(AudBgm*, s32);

    void reject();
    void setDominant();
    bool isDominant() const;

    /* 0x04 */ s32 _4;
    /* 0x08 */ AudBgm* _8;
};
