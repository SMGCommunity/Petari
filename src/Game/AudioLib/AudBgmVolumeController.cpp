#include "Game/AudioLib/AudBgmVolumeController.hpp"
#include "Game/AudioLib/AudParams.hpp"

#define FADER_VOLZERO 0
#define FADER_VOLDOWN 1
#define FADER_INTERRUPTED 2
#define FADER_AUX 3
#define FADER_NOTEFAIRY 4

void AudBgmVolumeController_FORCE_MATCH_SDATA() {
    (void)1.0f;
}

AudBgmVolumeController::AudBgmVolumeController() : mIsMuted() {
    for (s32 i = 0; i < NUM_FADERS; i++) {
        mTimers[i] = -1;
    }
    mFaderCtrl[FADER_VOLZERO].mVolume = AudParams::faderVolZeroVolume;
    mFaderCtrl[FADER_VOLZERO].mTime = AudParams::faderVolZeroTime;
    mFaderCtrl[FADER_VOLZERO].mRecoverTime = AudParams::faderVolZeroRecoverTime;
    mFaderCtrl[FADER_VOLDOWN].mVolume = AudParams::faderVolDownVolume;
    mFaderCtrl[FADER_VOLDOWN].mTime = AudParams::faderVolDownTime;
    mFaderCtrl[FADER_VOLDOWN].mRecoverTime = AudParams::faderVolDownRecoverTime;
    mFaderCtrl[FADER_INTERRUPTED].mVolume = AudParams::faderInterruptedVolume;
    mFaderCtrl[FADER_INTERRUPTED].mTime = AudParams::faderInterruptedTime;
    mFaderCtrl[FADER_INTERRUPTED].mRecoverTime = AudParams::faderInterruptedRecoverTime;
    mFaderCtrl[FADER_AUX].mVolume = 0.0f;
    mFaderCtrl[FADER_AUX].mTime = 5;
    mFaderCtrl[FADER_AUX].mRecoverTime = 5;
    mFaderCtrl[FADER_NOTEFAIRY].mVolume = 0.0f;
    mFaderCtrl[FADER_NOTEFAIRY].mTime = 5;
    mFaderCtrl[FADER_NOTEFAIRY].mRecoverTime = 5;
}

void AudBgmVolumeController::update() {
    for (s32 i = 0; i < NUM_FADERS; i++) {
        if (mTimers[i] > 0) {
            mTimers[i]--;
            if (mTimers[i] <= 0) {
                mFaders[i].set(1.0f, mFaderCtrl[i].mRecoverTime);
                mTimers[i] = -1;
            }
        }
        mFaders[i].update();
    }
}

f32 AudBgmVolumeController::getVolume() const {
    if (mIsMuted) {
        return 0.0f;
    }

    f32 volume = 1.0f;
    for (s32 i = 0; i < NUM_FADERS; i++) {
        volume *= mFaders[i].mCurrentVolume;
    }
    return volume;
}

void AudBgmVolumeController::volDown(bool immediate) {
    if (immediate) {
        mFaders[FADER_VOLDOWN].set(1.0f, 1);
        mTimers[FADER_VOLDOWN] = -1;
        return;
    }

    if (mTimers[FADER_VOLDOWN] < 0) {
        mFaders[FADER_VOLDOWN].set(mFaderCtrl[FADER_VOLDOWN].mVolume, mFaderCtrl[FADER_VOLDOWN].mTime);
    }
    mTimers[FADER_VOLDOWN] = 2;
}

void AudBgmVolumeController::interruptedByOther() {
    if (mTimers[FADER_INTERRUPTED] < 0) {
        mFaders[FADER_INTERRUPTED].set(mFaderCtrl[FADER_INTERRUPTED].mVolume, mFaderCtrl[FADER_INTERRUPTED].mTime);
    }
    mTimers[FADER_INTERRUPTED] = 2;
}

void AudBgmVolumeController::moveAuxVolume(f32 volume, s32 time) {
    mFaders[FADER_AUX].set(volume, time);
}

void AudBgmVolumeController::moveNoteFairyVolume(f32 volume, s32 time) {
    mFaders[FADER_NOTEFAIRY].set(volume, time);
}
