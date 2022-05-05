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