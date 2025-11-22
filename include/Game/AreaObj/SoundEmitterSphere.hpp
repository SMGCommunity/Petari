#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class AudAnmSoundObject;

class SoundEmitterSphere : public AreaObj {
public:
    SoundEmitterSphere(int, const char*);
    virtual ~SoundEmitterSphere();

    virtual void init(const JMapInfoIter&);
    virtual void movement();

    AudAnmSoundObject* mSoundObj;  // 0x3C
    TVec3f _40;
    const char* mSoundName;  // 0x4C
    s32 mSoundIndex;         // 0x50
};
