#pragma once

#include "Game/AudioLib/AudFader.hpp"

class JAISoundHandle;

class AudTrackController {
public:
    /// @brief Creates a new `AudTrackController`.
    AudTrackController();

    void start(JAISoundHandle*);
    void stop();
    void mute();
    void unmute();
    void setMuteState(u8, s32, bool);
    void update();
    void muteIfVolumeZero();

    /* 0x00 */ s32 _0;
    /* 0x04 */ u32 _4;
    /* 0x08 */ AudFader mFader;
    /* 0x14 */ f32 _14;
    /* 0x18 */ u8 _18;
};
