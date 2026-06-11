#include "Game/Speaker/SpkSpeakerCtrl.hpp"
#include "Game/Speaker/SpkMixingBuffer.hpp"
#include "Game/Speaker/SpkSound.hpp"
#include <mem.h>
#include <revolution/wpad.h>

SpkSoundHandle sAdjustSoundHandle[WPAD_MAX_CONTROLLERS];
SpkMixingBuffer* sMixingBuffer;
SpeakerInfo sSpeakerInfo[WPAD_MAX_CONTROLLERS];

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

// SpkSpeakerCtrl::setup

void SpkSpeakerCtrl::connect(s32 padChannel) {
    BOOL state = OSDisableInterrupts();
    sSpeakerInfo[padChannel]._0 = 1;
    sSpeakerInfo[padChannel]._1 = 0;
    sSpeakerInfo[padChannel]._24 = 0;
    SpkSpeakerCtrl::initReconnect(padChannel);
    sSpeakerInfo[padChannel]._30 = -1;
    SpkSpeakerCtrl::setSpeakerOn(padChannel);
    OSRestoreInterrupts(state);
}

void SpkSpeakerCtrl::setSpeakerOn(s32 padChannel) {
    BOOL state = OSDisableInterrupts();
    s32 val = WPADControlSpeaker(padChannel, 1, SpkSpeakerCtrl::setSpeakerOnCallback);

    if (val == -2) {
        sSpeakerInfo[padChannel]._24 = 1;
    } else {
        sSpeakerInfo[padChannel]._24 = 0;
    }

    OSRestoreInterrupts(state);
}

void SpkSpeakerCtrl::setSpeakerOnCallback(s32 padChannel, s32 a2) {
    BOOL state = OSDisableInterrupts();
    if (a2 == 0) {
        sSpeakerInfo[padChannel]._24 = 0;
        SpkSpeakerCtrl::setSpeakerPlay(padChannel);
    } else {
        if (a2 == -3) {
            sSpeakerInfo[padChannel]._24 = 1;
        }
    }

    OSRestoreInterrupts(state);
}

void SpkSpeakerCtrl::setSpeakerPlay(s32 padChannel) {
    BOOL state = OSDisableInterrupts();
    s32 val = WPADControlSpeaker(padChannel, 4, SpkSpeakerCtrl::startPlayCallback);

    if (val == -2) {
        sSpeakerInfo[padChannel]._24 = 2;
    } else {
        sSpeakerInfo[padChannel]._24 = 0;
    }

    OSRestoreInterrupts(state);
}

void SpkSpeakerCtrl::startPlayCallback(s32 padChannel, s32 a2) {
    BOOL enabled = OSDisableInterrupts();

    if (!enabled) {
        SpeakerInfo* inf = &sSpeakerInfo[padChannel];
        inf->_1 = 1;
        inf->_22 = 1;
        inf->_24 = 0;
        inf->_30 = 28800;
        memset(&sSpeakerInfo[padChannel]._2, 0, 0x20);
    } else {
        if (enabled == -3) {
            sSpeakerInfo[padChannel]._24 = 2;
        }
    }

    OSRestoreInterrupts(enabled);
}

void SpkSpeakerCtrl::setSpeakerOff(s32 padChannel) {
    sSpeakerInfo[padChannel]._1 = 0;
    sSpeakerInfo[padChannel]._24 = 0;
    sSpeakerInfo[padChannel]._30 = -1;
    WPADControlSpeaker(padChannel, 0, 0);
}

void SpkSpeakerCtrl::retryConnection(s32 padChannel) {
    switch (sSpeakerInfo[padChannel]._24) {
    case 0:
    case 3:
        break;
    case 1:
        SpkSpeakerCtrl::setSpeakerOn(padChannel);
        break;
    case 2:
        SpkSpeakerCtrl::setSpeakerPlay(padChannel);
        break;
    }
}

void SpkSpeakerCtrl::reconnect(s32 padChannel) {
    if (sSpeakerInfo[padChannel]._0) {
        sSpeakerInfo[padChannel]._28 = 1;
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
    if (sSpeakerInfo[padChannel]._0) {
        switch (sSpeakerInfo[padChannel]._28) {
        case 0:
            break;
        case 1:
            SpkSpeakerCtrl::setSpeakerOff(padChannel);
            sSpeakerInfo[padChannel]._28 = 2;
            sSpeakerInfo[padChannel]._2C = 0x14;
            break;
        case 2:
            sSpeakerInfo[padChannel]._2C--;

            if (sSpeakerInfo[padChannel]._2C <= 0) {
                sSpeakerInfo[padChannel]._2C = -1;
                sSpeakerInfo[padChannel]._28 = 3;
            }
            break;
        case 3:
            SpkSpeakerCtrl::setSpeakerOn(padChannel);
            sSpeakerInfo[padChannel]._28 = 0;
            break;
        }
    }
}

void SpkSpeakerCtrl::initReconnect(s32 padChannel) {
    sSpeakerInfo[padChannel]._28 = 0;
    sSpeakerInfo[padChannel]._2C = -1;
}

void SpkSpeakerCtrl::continuousUsingProcess(s32 padChannel) {
    if (sSpeakerInfo[padChannel]._30 >= 0) {
        sSpeakerInfo[padChannel]._30--;
        if (sSpeakerInfo[padChannel]._30 <= 0) {
            sSpeakerInfo[padChannel]._30 = -1;
            SpkSpeakerCtrl::reconnect(padChannel);
        }
    }
}

void SpkSpeakerCtrl::updateSpeaker(OSAlarm*, OSContext*) {
    for (s32 i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        if (sMixingBuffer != nullptr) {
            SpeakerInfo& inf = ::getSpeakerInfo(i);

            if (inf._1) {
                if (!inf._23) {
                    if (sMixingBuffer->update(i)) {
                        bool v5 = true;

                        if (inf._1 && sMixingBuffer != nullptr) {
                            BOOL en = OSDisableInterrupts();

                            if (!WPADCanSendStreamData(i)) {
                                OSRestoreInterrupts(en);
                            } else {
                                if (inf._22) {
                                    v5 = false;
                                    inf._22 = 0;
                                }

                                const s16* samples = sMixingBuffer->getSamples(i);
                                u8 data[16];
                                WENCGetEncodeData(&inf._2, v5, samples, 40, data);
                                WPADSendStreamData(i, data, 0x14);
                                OSRestoreInterrupts(en);
                            }
                        }
                    } else {
                        inf._22 = 1;
                    }
                }
            }
        }
    }
}

bool SpkSpeakerCtrl::isEnable(s32 padChannel) {
    return WPADIsSpeakerEnabled(padChannel) && sSpeakerInfo[padChannel]._1 != 0 && sSpeakerInfo[padChannel]._24 == 0 && sMixingBuffer != nullptr;
}

void SpkSpeakerCtrl::extensionProcess(s32, s32) {
}

f32 SpkSpeakerCtrl::getDeviceVolume(s32 padChannel) {
    if (padChannel >= WPAD_CHAN0 && !WPADIsSpeakerEnabled(padChannel)) {
        return 0.0f;
    }

    return WPADGetSpeakerVolume() / 127.0f;
}
