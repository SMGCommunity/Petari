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
    bool playsSound(const JAUSoundAnimationSound*, const TVec3f&, f32);
    u32 getSoundID(const JAUSoundAnimationSound*, const TVec3f&, f32);
    JAISoundHandle* getFreeHandle(const JAUSoundAnimationSound*);
    void skip(f32);
    void setStartPos(f32);
    bool hasAnim() const {
        return mSoundAnimation != nullptr;
    }
    void updateAnimSound(f32, const TVec3f&, JAISoundStarter*);
    void startAnimSound(const TVec3f&, f32, JAISoundStarter*);
    void releaseHandleIfNecessary(JAISoundHandle*, u32);
};
