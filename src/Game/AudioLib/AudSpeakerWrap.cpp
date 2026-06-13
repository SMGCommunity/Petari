#include "Game/AudioLib/AudSpeakerWrap.hpp"
#include "Game/Speaker/SpkSound.hpp"
#include "Game/Speaker/SpkSystem.hpp"
#include <JSystem/JAudio2/JASGlobal.hpp>
#include <revolution/wpad.h>

namespace {
    static SpkSoundHandle sSoundHandle[WPAD_MAX_CONTROLLERS];
    static SpkSoundHandle sSoundLevelHandle[WPAD_MAX_CONTROLLERS];
};  // namespace

const char* AudSpeakerWrap::getResName() {
    return "/AudioRes/SpkRes/SpkRes.arc";
}

u32 AudSpeakerWrap::getRequiredHeapSize() {
    return 0x40000;
}

void AudSpeakerWrap::start(s32 waveID, s32 padChannel) {
    SpkSystem* system = JASGlobalInstance< SpkSystem >::getInstance();

    if (system == nullptr) {
        return;
    }

    if (padChannel < WPAD_CHAN0) {
        for (s32 idx = 0; idx < WPAD_MAX_CONTROLLERS; idx++) {
            system->startSound(idx, waveID, &sSoundHandle[idx]);
        }
    } else {
        system->startSound(padChannel, waveID, &sSoundHandle[padChannel]);
    }
}

bool AudSpeakerWrap::isPlayable(s32 padChannel) {
    SpkSystem* system = JASGlobalInstance< SpkSystem >::getInstance();

    if (system == nullptr) {
        return false;
    }

    return system->getDeviceVolume(padChannel) != 0.0f;
}
