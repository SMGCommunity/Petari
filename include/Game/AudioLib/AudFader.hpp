#pragma once

#include <revolution/types.h>

class AudFader {
public:
    enum FadeState {
        FadeState_FadeOut = 0,
        FadeState_FadeIn = 1,
        FadeState_Mute = 2,
        FadeState_Unmute = 3,
    };

    AudFader();

    ~AudFader(){};

    void set(f32 desiredVolume, s32 fadeTime);
    void update();

    f32 getVolume() {
        return mCurrentVolume;
    }

    /* 0x00 */ f32 mCurrentVolume;
    /* 0x04 */ f32 mFinalVolume;
    /* 0x08 */ f32 mStepVolume;  ///< Volume intensity increase/decrease per AudFader::update() call until mCurrentVolume reaches mFinalVolume value
};
