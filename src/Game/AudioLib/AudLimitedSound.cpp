#include "Game/AudioLib/AudLimitedSound.hpp"

AudLimitedSoundInfo::AudLimitedSoundInfo() {
    init();
}

void AudLimitedSoundInfo::init() {
    mSoundID.setAnonymous();
    mDelay = 0;
}

void AudLimitedSoundInfo::set(JAISoundID soundID, s32 delay) {
    mSoundID = soundID;
    mDelay = delay;
}

void AudLimitedSoundInfo::update() {
    if (isFree()) {
        return;
    }

    mDelay--;

    if (mDelay > 0) {
        return;
    }

    init();
}

bool AudLimitedSoundInfo::isFree() {
    return mSoundID.isAnonymous();
}
