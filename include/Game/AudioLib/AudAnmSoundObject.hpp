#pragma once

#include "Game/AudioLib/AudSoundObject.hpp"

class JAISoundStarter;
class JASSoundParams;
class JAUSoundAnimationSound;

class AudAnmSoundObject : public AudSoundObject {
public:
    AudAnmSoundObject(TVec3f *, unsigned char, JKRHeap *);

    void update(float);
    void modifySoundParams(JASSoundParams *, const JAUSoundAnimationSound *, float);
    void playsSound(const JAUSoundAnimationSound *, const TVec3f &, float);
    void getSoundID(const JAUSoundAnimationSound *, const TVec3f &, float);
    void getFreeHandle(const JAUSoundAnimationSound *);
    void skip(float);
    void setStartPos(float);
    void updateAnimSound(float, const TVec3f &, JAISoundStarter *);
    void startAnimSound(const TVec3f &, float, JAISoundStarter *);
    void releaseHandleIfNecessary(JAISoundHandle *, unsigned long);

    u8 _48[0x2C];
};