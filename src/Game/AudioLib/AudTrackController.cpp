#include "Game/AudioLib/AudTrackController.hpp"
#include "JSystem/JAudio2/JAISound.hpp"
#include "JSystem/JAudio2/JAISoundChild.hpp"
#include "JSystem/JAudio2/JAISoundParams.hpp"
#include "JSystem/JAudio2/JASCriticalSection.hpp"
#include "JSystem/JAudio2/JASTrack.hpp"

AudTrackController::AudTrackController() : mTrackNo(-1), mHandle(nullptr), mAutoMute(false), mVolume(1.0f) {
}

void AudTrackController::start(JAISoundHandle* pHandle) {
    mHandle = pHandle;
    mAutoMute = false;
}

void AudTrackController::stop() {
    mHandle = nullptr;
    mAutoMute = false;
}

void AudTrackController::mute() {
    if (mHandle == nullptr) {
        return;
    }

    JAISound* sound = mHandle->getSound();
    if (sound == nullptr) {
        return;
    }

    if (mTrackNo < 0) {
        return;
    }

    JASTrack* track = sound->getChildTrack(mTrackNo);
    if (track == nullptr) {
        return;
    }
    track->mute(true);
}

void AudTrackController::unmute() {
    if (mHandle == nullptr) {
        return;
    }

    JAISound* sound = mHandle->getSound();
    if (sound == nullptr) {
        return;
    }

    if (mTrackNo < 0) {
        return;
    }

    JASTrack* track = sound->getChildTrack(mTrackNo);
    if (track == nullptr) {
        return;
    }
    track->mute(false);
}

void AudTrackController::setMuteState(u8 state, s32 fadeTime, bool autoMute) {
    switch (state) {
    case 0:
        mFader.set(0.0f, fadeTime);
        break;
    case 1:
        unmute();
        mFader.set(1.0f, fadeTime);
        break;
    case 2:
        mute();
        break;
    case 3:
        unmute();
        break;
    default:
        return;
    }

    mAutoMute = autoMute;
}

void AudTrackController::update() {
    if (mHandle != nullptr && mHandle->isSoundAttached()) {
        if (mTrackNo < 0) {
            return;
        }

        mFader.update();

        f32 volume = mVolume * mFader.getVolume();
        JAISoundParamsMove* params = mHandle->getSound()->getChild(mTrackNo)->getMoveParams();
        if (params == nullptr) {
            return;
        }
        params->moveVolume(volume, 0);
    }

    muteIfVolumeZero();
}

void AudTrackController::muteIfVolumeZero() {
    if (mTrackNo < 0) {
        return;
    }

    if (!mAutoMute) {
        return;
    }

    if (mHandle == nullptr) {
        return;
    }

    JAISound* sound = mHandle->getSound();
    if (sound == nullptr) {
        return;
    }

    JAISoundChild* child = sound->getChild(mTrackNo);
    if (child == nullptr) {
        return;
    }

    JASTrack* track = mHandle->getSound()->getChildTrack(mTrackNo);
    if (track == nullptr) {
        return;
    }

    if (child->mParams.mVolume == 0.0f) {
        JASCriticalSection crit;
        track->mute(true);
    } else {
        JASCriticalSection crit;
        track->mute(false);
    }
}
