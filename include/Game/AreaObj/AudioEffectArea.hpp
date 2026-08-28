#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class AudioEffectArea : public AreaObj {
public:
    AudioEffectArea(int, const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x14 */ virtual void movement();

    /* 0x28 */ virtual const char* getManagerName() const {
        return "AudioEffectArea";
    }

    /* 0x3C */ s32 _3C;
    /* 0x40 */ s32 _40;
};