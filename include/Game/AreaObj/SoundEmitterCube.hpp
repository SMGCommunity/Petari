#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class AudAnmSoundObject;

class SoundEmitterCube : public AreaObj {
public:
    SoundEmitterCube(int, const char*);
    /* 0x08 */ virtual ~SoundEmitterCube();

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x14 */ virtual void movement();

    /* 0x3C */ AudAnmSoundObject* mSoundObj;
    /* 0x40 */ TVec3f _40;
    /* 0x4C */ const char* mSoundName;
};
