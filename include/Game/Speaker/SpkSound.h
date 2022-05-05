#pragma once

#include <revolution.h>

class SpkSoundVolume {
public:
    SpkSoundVolume();

    void setRelease(s32);
    void setFadeOut(s32);
    f32 calc(bool &);

    f32 _0;
    f32 _4;
    f32 _8;
    f32 _C;
    f32 _10;
    f32 _14;
    f32 _18;
    f32 _1C;
};