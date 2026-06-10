#pragma once

#include <JSystem/JAudio2/JAISound.hpp>
#include <revolution/types.h>

class AudLimitedSoundInfo {
public:
    /// @brief Creates a new `AudLimitedSoundInfo`.
    AudLimitedSoundInfo();

    void init();
    void set(JAISoundID, s32);
    void update();
    bool isFree();

    /* 0x00 */ JAISoundID mSoundID;
    /* 0x04 */ s32 _4;
};
