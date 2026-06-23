#include "Game/Speaker/SpkSpeakerCtrl.hpp"
#include "Game/Speaker/SpkMixingBuffer.hpp"
#include "Game/Speaker/SpkSound.hpp"
#include <JSystem/JAudio2/JASCriticalSection.hpp>
#include <mem.h>
#include <revolution/os/OSAlarm.h>
#include <revolution/wenc.h>
#include <revolution/wpad.h>

OSAlarm sSpeakerAlarm;
SpeakerInfo sSpeakerInfo[WPAD_MAX_CONTROLLERS];
SpkSoundHandle sAdjustSoundHandle[WPAD_MAX_CONTROLLERS];
SpkMixingBuffer* sMixingBuffer;

namespace {
    SpeakerInfo& getSpeakerInfo(s32 idx) {
        return sSpeakerInfo[idx];
    }
};  // namespace

void SpkSpeakerCtrl::setMixingBuffer(SpkMixingBuffer* pMixingBuffer) {
    BOOL status = OSDisableInterrupts();
    sMixingBuffer = pMixingBuffer;
    OSRestoreInterrupts(status);
}

void SpkSpeakerCtrl::setup() {
    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        sSpeakerInfo[i].mIsConnected = false;
        sSpeakerInfo[i].mIsPlaying = false;
        sSpeakerInfo[i].mIsUpdated = true;
        sSpeakerInfo[i].mIsMuted = false;
        sSpeakerInfo[i].mState = SpeakerInfo::State_ENABLE;
        sSpeakerInfo[i].mVolume = 64;
        sSpeakerInfo[i].mRadioSensitivityTimer = 0;
        sSpeakerInfo[i].mExtensionTimer = 0;
        SpkSpeakerCtrl::initReconnect(i);
        sSpeakerInfo[i].mUsingTimeOut = -1;
        memset(&sSpeakerInfo[i].mWENCInfo, 0, sizeof(WENCInfo));
    }

    OSCreateAlarm(&sSpeakerAlarm);
    OSSetPeriodicAlarm(&sSpeakerAlarm, OSGetTime(), OSNanosecondsToTicks(6666667), SpkSpeakerCtrl::updateSpeaker);
}

void SpkSpeakerCtrl::connect(s32 padChannel) {
    JASCriticalSection crit;
    sSpeakerInfo[padChannel].mIsConnected = true;
    sSpeakerInfo[padChannel].mIsPlaying = false;
    sSpeakerInfo[padChannel].mState = SpeakerInfo::State_ENABLE;
    SpkSpeakerCtrl::initReconnect(padChannel);
    sSpeakerInfo[padChannel].mUsingTimeOut = -1;
    SpkSpeakerCtrl::setSpeakerOn(padChannel);
}

void SpkSpeakerCtrl::disconnect(s32 padChannel) {
    JASCriticalSection crit;
    sSpeakerInfo[padChannel].mIsConnected = false;
    sSpeakerInfo[padChannel].mIsPlaying = false;
    sSpeakerInfo[padChannel].mState = SpeakerInfo::State_ENABLE;
    SpkSpeakerCtrl::setSpeakerOff(padChannel);
    SpkSpeakerCtrl::initReconnect(padChannel);
    sSpeakerInfo[padChannel].mUsingTimeOut = -1;
}

void SpkSpeakerCtrl::setSpeakerOn(s32 padChannel) {
    JASCriticalSection crit;
    // TODO: WPAD command magic numbers
    s32 err = WPADControlSpeaker(padChannel, 1, SpkSpeakerCtrl::setSpeakerOnCallback);

    if (err == WPAD_ERR_BUSY) {
        sSpeakerInfo[padChannel].mState = SpeakerInfo::State_ON;
    } else {
        sSpeakerInfo[padChannel].mState = SpeakerInfo::State_ENABLE;
    }
}

void SpkSpeakerCtrl::setSpeakerOnCallback(s32 padChannel, s32 err) {
    JASCriticalSection crit;
    if (err == WPAD_ERR_NONE) {
        sSpeakerInfo[padChannel].mState = SpeakerInfo::State_ENABLE;
        SpkSpeakerCtrl::setSpeakerPlay(padChannel);
    } else if (err == WPAD_ERR_TRANSFER) {
        sSpeakerInfo[padChannel].mState = SpeakerInfo::State_ON;
    }
}

void SpkSpeakerCtrl::setSpeakerPlay(s32 padChannel) {
    JASCriticalSection crit;
    // TODO: WPAD command magic numbers
    s32 err = WPADControlSpeaker(padChannel, 4, SpkSpeakerCtrl::startPlayCallback);

    if (err == WPAD_ERR_BUSY) {
        sSpeakerInfo[padChannel].mState = SpeakerInfo::State_PLAY;
    } else {
        sSpeakerInfo[padChannel].mState = SpeakerInfo::State_ENABLE;
    }
}

void SpkSpeakerCtrl::startPlayCallback(s32 padChannel, s32 err) {
    JASCriticalSection crit;

    if (err == WPAD_ERR_NONE) {
        sSpeakerInfo[padChannel].mIsPlaying = true;
        sSpeakerInfo[padChannel].mIsUpdated = true;
        sSpeakerInfo[padChannel].mState = SpeakerInfo::State_ENABLE;
        sSpeakerInfo[padChannel].mUsingTimeOut = 8 * 60 * 60;
        memset(&sSpeakerInfo[padChannel].mWENCInfo, 0, sizeof(WENCInfo));
    } else if (err == WPAD_ERR_TRANSFER) {
        sSpeakerInfo[padChannel].mState = SpeakerInfo::State_PLAY;
    }
}

void SpkSpeakerCtrl::setSpeakerOff(s32 padChannel) {
    sSpeakerInfo[padChannel].mIsPlaying = false;
    sSpeakerInfo[padChannel].mState = SpeakerInfo::State_ENABLE;
    sSpeakerInfo[padChannel].mUsingTimeOut = -1;
    // TODO: WPAD command magic numbers
    WPADControlSpeaker(padChannel, 0, nullptr);
}

void SpkSpeakerCtrl::retryConnection(s32 padChannel) {
    switch (sSpeakerInfo[padChannel].mState) {
    case SpeakerInfo::State_ENABLE:
    case SpeakerInfo::State_3:
        break;
    case SpeakerInfo::State_ON:
        SpkSpeakerCtrl::setSpeakerOn(padChannel);
        break;
    case SpeakerInfo::State_PLAY:
        SpkSpeakerCtrl::setSpeakerPlay(padChannel);
        break;
    }
}

void SpkSpeakerCtrl::reconnect(s32 padChannel) {
    if (sSpeakerInfo[padChannel].mIsConnected) {
        sSpeakerInfo[padChannel].mReconnectState = SpeakerInfo::Reconnect_OFF;
    }
}

void SpkSpeakerCtrl::framework() {
    for (s32 i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        SpkSpeakerCtrl::continuousUsingProcess(i);
        SpkSpeakerCtrl::reconnectProcess(i);
        SpkSpeakerCtrl::retryConnection(i);
    }
}

void SpkSpeakerCtrl::reconnectProcess(s32 padChannel) {
    if (sSpeakerInfo[padChannel].mIsConnected) {
        switch (sSpeakerInfo[padChannel].mReconnectState) {
        case SpeakerInfo::Reconnect_NONE:
            break;
        case SpeakerInfo::Reconnect_OFF:
            SpkSpeakerCtrl::setSpeakerOff(padChannel);
            sSpeakerInfo[padChannel].mReconnectState = SpeakerInfo::Reconnect_WAIT;
            sSpeakerInfo[padChannel].mReconnectTime = 20;
            break;
        case SpeakerInfo::Reconnect_WAIT:
            sSpeakerInfo[padChannel].mReconnectTime--;

            if (sSpeakerInfo[padChannel].mReconnectTime <= 0) {
                sSpeakerInfo[padChannel].mReconnectTime = -1;
                sSpeakerInfo[padChannel].mReconnectState = SpeakerInfo::Reconnect_ON;
            }
            break;
        case SpeakerInfo::Reconnect_ON:
            SpkSpeakerCtrl::setSpeakerOn(padChannel);
            sSpeakerInfo[padChannel].mReconnectState = SpeakerInfo::Reconnect_NONE;
            break;
        }
    }
}

void SpkSpeakerCtrl::initReconnect(s32 padChannel) {
    sSpeakerInfo[padChannel].mReconnectState = SpeakerInfo::Reconnect_NONE;
    sSpeakerInfo[padChannel].mReconnectTime = -1;
}

void SpkSpeakerCtrl::continuousUsingProcess(s32 padChannel) {
    if (sSpeakerInfo[padChannel].mUsingTimeOut >= 0) {
        sSpeakerInfo[padChannel].mUsingTimeOut--;
        if (sSpeakerInfo[padChannel].mUsingTimeOut <= 0) {
            sSpeakerInfo[padChannel].mUsingTimeOut = -1;
            SpkSpeakerCtrl::reconnect(padChannel);
        }
    }
}

void SpkSpeakerCtrl::updateSpeaker(OSAlarm*, OSContext*) {
    for (s32 i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        if (sMixingBuffer == nullptr) {
            continue;
        }

        SpeakerInfo& inf = ::getSpeakerInfo(i);

        if (!inf.mIsPlaying) {
            continue;
        }

        if (inf.mIsMuted) {
            continue;
        }

        if (sMixingBuffer->update(i)) {
            u32 flags = WENC_FLAG_USER_INFO;

            if (inf.mIsPlaying && sMixingBuffer != nullptr) {
                BOOL en = OSDisableInterrupts();

                if (!WPADCanSendStreamData(i)) {
                    OSRestoreInterrupts(en);
                    continue;
                }

                if (inf.mIsUpdated) {
                    flags = 0;
                    inf.mIsUpdated = false;
                }

                const s16* samples = sMixingBuffer->getSamples(i);
                u8 data[16];
                WENCGetEncodeData(&inf.mWENCInfo, flags, samples, 40, data);
                WPADSendStreamData(i, data, 20);

                OSRestoreInterrupts(en);
            }
            continue;
        }

        inf.mIsUpdated = true;
    }
}

bool SpkSpeakerCtrl::isEnable(s32 padChannel) {
    return WPADIsSpeakerEnabled(padChannel) && sSpeakerInfo[padChannel].mIsPlaying && sSpeakerInfo[padChannel].mState == SpeakerInfo::State_ENABLE &&
           sMixingBuffer != nullptr;
}

void SpkSpeakerCtrl::extensionProcess(s32, s32) {
}

f32 SpkSpeakerCtrl::getDeviceVolume(s32 padChannel) {
    if (padChannel >= WPAD_CHAN0 && !WPADIsSpeakerEnabled(padChannel)) {
        return 0.0f;
    }

    return WPADGetSpeakerVolume() / 127.0f;
}
