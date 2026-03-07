#pragma once

#include "Game/AudioLib/AudSoundObject.hpp"
#include <JSystem/JAudio2/JAUSoundAnimator.hpp>

class JAISoundStarter;
class JASSoundParams;
class JAUSoundAnimationSound;

class AudAnmSoundObject : public AudSoundObject, public JAUSoundAnimator {
public:
    AudAnmSoundObject(TVec3f*, u8, JKRHeap*);

    void update(f32);
    void modifySoundParams(JASSoundParams*, const JAUSoundAnimationSound*, f32);
    void playsSound(const JAUSoundAnimationSound*, const TVec3f&, f32);
    s32 getSoundID(const JAUSoundAnimationSound*, const TVec3f&, f32);
    u32* getFreeHandle(const JAUSoundAnimationSound*);
    void skip(f32);
    void setStartPos(f32);
    void setLoopFrame(f32 a1, f32 a2) { JAUSoundAnimator::setLoopFrame(a1, a2); }
    bool hasAnimHandles() const { return JAUSoundAnimator::mHandles != nullptr; }
    void updateAnimSound(f32, const TVec3f&, JAISoundStarter*);
    void startAnimSound(const TVec3f&, f32, JAISoundStarter*);
    void releaseHandleIfNecessary(JAISoundHandle*, u32);
};
