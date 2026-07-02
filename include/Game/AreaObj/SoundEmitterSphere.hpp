#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class AudAnmSoundObject;

class SoundEmitterSphere : public AreaObj {
public:
    SoundEmitterSphere(int, const char*);

    virtual ~SoundEmitterSphere();
    virtual void init(const JMapInfoIter&);
    virtual void movement();

    /* 0x3C */ AudAnmSoundObject* mSoundObj;
    /* 0x40 */ TVec3f _40;
    /* 0x4C */ const char* mSoundName;
    /* 0x50 */ s32 mSoundIndex;
};
