#include "Game/Speaker/SpkSound.h"

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

    mSoundHandle->mSound = NULL;
    mSoundHandle = NULL;
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