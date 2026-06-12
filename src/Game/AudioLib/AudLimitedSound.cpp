#include "Game/AudioLib/AudLimitedSound.hpp"

AudLimitedSoundInfo::AudLimitedSoundInfo() {
    init();
}

void AudLimitedSoundInfo::init() {
    mSoundID.setAnonymous();
    _4 = 0;
}

void AudLimitedSoundInfo::set(JAISoundID soundID, s32 param2) {
    mSoundID = soundID;
    _4 = param2;
}

void AudLimitedSoundInfo::update() {
    if (isFree()) {
        return;
    }

    _4--;

    if (_4 > 0) {
        return;
    }

    init();
}

bool AudLimitedSoundInfo::isFree() {
    return mSoundID.isAnonymous();
}
