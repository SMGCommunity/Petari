#pragma once

#include "Game/NameObj/NameObj.hpp"

class AudEffectDirector : public NameObj {
public:
    AudEffectDirector();

    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();

    void setEffectType(s32, s32);
    void setAudioEffectParam(s32);
    void playEffectSound(s32) NO_INLINE;

    s32 _C;
    s32 _10;
    s32 _14;
};
