#pragma once

#include "Game/AudioLib/AudSoundObject.hpp"
#include <JSystem/JAudio2/JAUSoundAnimator.hpp>

class JAISoundStarter;
class JASSoundParams;
class JAUSoundAnimationSound;

class AudAnmSoundObject : public AudSoundObject, JAUSoundAnimator {
public:
    AudAnmSoundObject(TVec3f*, u8, JKRHeap*);

    void update(f32);
    void modifySoundParams(JASSoundParams*, const JAUSoundAnimationSound*, f32);
    void playsSound(const JAUSoundAnimationSound*, const TVec3f&, f32);
    s32 getSoundID(const JAUSoundAnimationSound*, const TVec3f&, f32);
    u32* getFreeHandle(const JAUSoundAnimationSound*);
    void skip(f32);
    void setStartPos(f32);
    void updateAnimSound(f32, const TVec3f&, JAISoundStarter*);
    void startAnimSound(const TVec3f&, f32, JAISoundStarter*);
    void releaseHandleIfNecessary(JAISoundHandle*, u32);
};
