#pragma once

#include "Game/AudioLib/AudSoundObject.hpp"
#include <JSystem/JAudio2/JAUSoundAnimator.hpp>

class JAISoundStarter;
class JASSoundParams;
class JAUSoundAnimationSound;

class AudAnmSoundObject : public AudSoundObject, JAUSoundAnimator {
public:
    AudAnmSoundObject(TVec3f *, unsigned char, JKRHeap *);

    void update(float);
    void modifySoundParams(JASSoundParams *, const JAUSoundAnimationSound *, float);
    void playsSound(const JAUSoundAnimationSound *, const TVec3f &, float);
    s32 getSoundID(const JAUSoundAnimationSound *, const TVec3f &, float);
    u32* getFreeHandle(const JAUSoundAnimationSound *);
    void skip(float);
    void setStartPos(float);
    void updateAnimSound(float, const TVec3f &, JAISoundStarter *);
    void startAnimSound(const TVec3f &, float, JAISoundStarter *);
    void releaseHandleIfNecessary(JAISoundHandle *, unsigned long);
};
