#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class AudAnmSoundObject;

class SoundEmitterSphere : public AreaObj {
public:
    SoundEmitterSphere(int, const char *);
    virtual ~SoundEmitterSphere();

    virtual void init(const JMapInfoIter &);
    virtual void movement();

    AudAnmSoundObject *mSoundObj;   // _3C
    TVec3f _40;
    const char *mSoundName;         // _4C
    s32 mSoundIndex;                // _50
};
