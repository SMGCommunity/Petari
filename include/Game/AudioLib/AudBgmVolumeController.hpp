#pragma once

#include "Game/AudioLib/AudFader.hpp"

class AudBgmVolumeController {
public:
    /// @brief Creates a new `AudBgmVolumeController`.
    AudBgmVolumeController();

    void update();
    f32 getVolume() const;
    void volDown(bool);
    void interruptedByOther();
    void moveAuxVolume(f32, s32);
    void moveNoteFairyVolume(f32, s32);

    /* 0x00 */ s32 _0[5];
    /* 0x14 */ f32 mFaderVolZeroVolume;
    /* 0x18 */ s32 mFaderVolZeroTime;
    /* 0x1C */ s32 mFaderVolZeroRecoverTime;
    /* 0x20 */ f32 mFaderVolDownVolume;
    /* 0x24 */ s32 mFaderVolDownTime;
    /* 0x28 */ s32 mFaderVolDownRecoverTime;
    /* 0x2C */ f32 mFaderInterruptedVolume;
    /* 0x30 */ s32 mFaderInterruptedTime;
    /* 0x34 */ s32 mFaderInterruptedRecoverTime;
    /* 0x38 */ f32 _38;
    /* 0x3C */ s32 _3C;
    /* 0x40 */ s32 _40;
    /* 0x44 */ f32 _44;
    /* 0x48 */ s32 _48;
    /* 0x4C */ s32 _4C;
    /* 0x50 */ AudFader mFader[5];
    /* 0x8C */ u8 _8C;
};
