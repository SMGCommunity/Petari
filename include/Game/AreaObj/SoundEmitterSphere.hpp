#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class AudAnmSoundObject;

class SoundEmitterSphere : public AreaObj {
public:
    SoundEmitterSphere(int formType, const char* pName);

    virtual ~SoundEmitterSphere();
    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();

    /* 0x3C */ AudAnmSoundObject* mSoundObj;
    /* 0x40 */ TVec3f _40;
    /* 0x4C */ const char* mSoundName;
    /* 0x50 */ s32 mSoundIndex;
};
