#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class AudAnmSoundObject;

class SoundEmitterCube : public AreaObj {
public:
    SoundEmitterCube(int, const char*);
    virtual ~SoundEmitterCube();

    virtual void init(const JMapInfoIter&);
    virtual void movement();

    AudAnmSoundObject* mSoundObj;  // 0x3C
    TVec3f _40;
    const char* mSoundName;  // 0x4C
};
