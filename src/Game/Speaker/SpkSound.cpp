#include "Game/Speaker/SpkSound.hpp"
#include "JSystem/JAudio2/JASHeapCtrl.hpp"

void SpkSoundHandle::releaseSound() {
    if (!mSound) {
        return;
    }

    mSound->mSoundHandle = nullptr;
    mSound = nullptr;
}

SpkSoundVolume::SpkSoundVolume() {
    _0 = 1.0f;
    _4 = 0.0f;
    _8 = 1.0f;
    _C = 0.0f;
    _10 = 1.0f;
    _14 = 1.0f;
    _18 = 0.0f;
    _1C = 1.0f;
}

void SpkSoundVolume::setRelease(s32 release) {
    if (release > 0) {
        _4 = 1.0f / release;
    }
    else {
        _4 = 1.0f;
    }

    _0 = 1.0f;
}

void SpkSoundVolume::setFadeOut(s32 fadeOut) {
    BOOL status = OSDisableInterrupts();

    if (fadeOut > 0) {
        _C = 1.0f / fadeOut;
    }
    else {
        _C = 1.0f;
    }

    _8 = 1.0f;
    OSRestoreInterrupts(status);
}

SpkSound::SpkSound() : JSUPtrLink(this), mSoundHandle(nullptr), _14(-1), _18(0), _1C(0), _20(0), _24(100), _28(0), _2C(0), _30(-1), _34(-1), _38(0), _3C(-1) {
    
}

#ifdef NON_MATCHING
SpkSound::~SpkSound() {
    releaseHandle();

    if (this) {
        JASPoolAllocObject<SpkSound>::memPool_.free(this, 0x60);
    }
}
#endif

// SpkSound::update

void SpkSound::stop(s32 a1) {
    _38 = 3;
    s32 fade = (6 * a1) / 40;

    if (fade) {
        mVolume.setFadeOut(fade);
    }
    else {
        mVolume.setRelease(_28);
    }
}

void SpkSound::attachHandle(SpkSoundHandle *pHandle) {
    if (mSoundHandle) {
        releaseHandle();
    }

    mSoundHandle = pHandle;
    pHandle->mSound = this;
}

void SpkSound::releaseHandle() {
    if (!mSoundHandle) {
        return;
    }

    mSoundHandle->mSound = nullptr;
    mSoundHandle = nullptr;
}

bool SpkSound::start(s32 a1, s32 a2) {
    _20 = a1;
    bool res = setWaveData(a2);

    if (!res) {
        return false;
    }

    _38 = 1;
    return true;
}

void SpkSound::unlock() {
    if (_38 != 1) {
        return; 
    }

    _38 = 2;
}

SpkSoundHolder::SpkSoundHolder() : JASGlobalInstance(this) {
    _44 = 0xA;
    _40 = 1.0f;

    for (int i = 0; i < 4; i++) {
        _30[i] = 1.0f;
    }
}

bool SpkSoundHolder::update(s32 a1) {
    BOOL status = OSDisableInterrupts();
    bool ret = updateEachSound(a1);
    OSRestoreInterrupts(status);
    return ret;
} 