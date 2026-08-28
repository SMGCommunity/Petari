#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class AudAnmSoundObject;

class SoundEmitterSphere : public AreaObj {
public:
    SoundEmitterSphere(int, const char*);

    /* 0x08 */ virtual ~SoundEmitterSphere();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x14 */ virtual void movement();

    /* 0x3C */ AudAnmSoundObject* mSoundObj;
    /* 0x40 */ TVec3f _40;
    /* 0x4C */ const char* mSoundName;
    /* 0x50 */ s32 mSoundIndex;
};
