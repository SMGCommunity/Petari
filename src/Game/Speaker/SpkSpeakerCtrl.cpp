#include "Game/Speaker/SpkSpeakerCtrl.hpp"
#include <revolution/wpad.h>
#include <mem.h>

SpkSoundHandle sAdjustSoundHandle[4];

void SpkSpeakerCtrl::setMixingBuffer(SpkMixingBuffer *pMixingBuffer) {
    BOOL status = OSDisableInterrupts();
    sMixingBuffer = pMixingBuffer;
    OSRestoreInterrupts(status);
}

// SpkSpeakerCtrl::setup

void SpkSpeakerCtrl::connect(s32 idx) {
    BOOL state = OSDisableInterrupts();
    sSpeakerInfo[idx]._0 = 1;
    sSpeakerInfo[idx]._1 = 0;
    sSpeakerInfo[idx]._24 = 0;
    SpkSpeakerCtrl::initReconnect(idx);
    sSpeakerInfo[idx]._30 = -1;
    SpkSpeakerCtrl::setSpeakerOn(idx);
    OSRestoreInterrupts(state);
}

void SpkSpeakerCtrl::setSpeakerOn(s32 idx) {
    BOOL state = OSDisableInterrupts();
    s32 val = WPADControlSpeaker(idx, 1, SpkSpeakerCtrl::setSpeakerOnCallback);

    if (val == -2) {
        sSpeakerInfo[idx]._24 = 1;
    }
    else {
        sSpeakerInfo[idx]._24 = 0;
    }

    OSRestoreInterrupts(state);
}

void SpkSpeakerCtrl::setSpeakerOnCallback(s32 idx, s32 a2) {
    BOOL state = OSDisableInterrupts();
    if (a2 == 0) {
        sSpeakerInfo[idx]._24 = 0;
        SpkSpeakerCtrl::setSpeakerPlay(idx);
    }
    else {
        if (a2 == -3) {
            sSpeakerInfo[idx]._24 = 1;
        }
    }

    OSRestoreInterrupts(state);
}

void SpkSpeakerCtrl::setSpeakerPlay(s32 idx) {
    BOOL state = OSDisableInterrupts();
    s32 val = WPADControlSpeaker(idx, 4, SpkSpeakerCtrl::startPlayCallback);
    
    if (val == -2) {
        sSpeakerInfo[idx]._24 = 2;
    }
    else {
        sSpeakerInfo[idx]._24 = 0;
    }

    OSRestoreInterrupts(state);
}

#ifdef NON_MATCHING
void SpkSpeakerCtrl::startPlayCallback(s32 idx, s32 a2) {
    BOOL enabled = OSDisableInterrupts();

    if (!enabled) {
        SpeakerInfo* inf = &sSpeakerInfo[idx];
        inf->_1 = 1;
        inf->_22 = 1;
        inf->_24 = 0;
        inf->_30 = 28800;
        memset(&sSpeakerInfo[idx]._2, 0, 0x20);
    }
    else {
        if (enabled == -3) {
            sSpeakerInfo[idx]._24 = 2;
        }
    }

    OSRestoreInterrupts(enabled);
}
#endif

void SpkSpeakerCtrl::setSpeakerOff(s32 idx) {
    sSpeakerInfo[idx]._1 = 0;
    sSpeakerInfo[idx]._24 = 0;
    sSpeakerInfo[idx]._30 = -1;
    WPADControlSpeaker(idx, 0, 0);
}

void SpkSpeakerCtrl::retryConnection(s32 idx) {
    switch (sSpeakerInfo[idx]._24) {
        case 0:
        case 3:
            break;
        case 1:
            SpkSpeakerCtrl::setSpeakerOn(idx);
            break;
        case 2:
            SpkSpeakerCtrl::setSpeakerPlay(idx);
            break;
    }
}

void SpkSpeakerCtrl::reconnect(s32 idx) {
    if (sSpeakerInfo[idx]._0) {
        sSpeakerInfo[idx]._28 = 1;
    }
}

void SpkSpeakerCtrl::framework() {
    for (int i = 0; i < 4; i++) {
        SpkSpeakerCtrl::continuousUsingProcess(i);
        SpkSpeakerCtrl::reconnectProcess(i);
        SpkSpeakerCtrl::retryConnection(i);
    }
}

void SpkSpeakerCtrl::reconnectProcess(s32 idx) {
    if (sSpeakerInfo[idx]._0) {
        switch (sSpeakerInfo[idx]._28) {
            case 0:
                break;
            case 1:
                SpkSpeakerCtrl::setSpeakerOff(idx);
                sSpeakerInfo[idx]._28 = 2;
                sSpeakerInfo[idx]._2C = 0x14;
                break;
            case 2:
                sSpeakerInfo[idx]._2C--;

                if (sSpeakerInfo[idx]._2C > 0) {
                    sSpeakerInfo[idx]._2C = -1;
                    sSpeakerInfo[idx]._28 = 3;
                }
                break;
            case 3:
                SpkSpeakerCtrl::setSpeakerOn(idx);
                sSpeakerInfo[idx]._28 = 0;
                break;
        }
    }
}

void SpkSpeakerCtrl::initReconnect(s32 idx) {
    sSpeakerInfo[idx]._28 = 0;
    sSpeakerInfo[idx]._2C = -1;
}

void SpkSpeakerCtrl::continuousUsingProcess(s32 idx) {
    if (sSpeakerInfo[idx]._30 >= 0) {
        sSpeakerInfo[idx]._30 = sSpeakerInfo[idx]._30 - 1;
        if (sSpeakerInfo[idx]._30 <= 0) {
            sSpeakerInfo[idx]._30 = -1;
            SpkSpeakerCtrl::reconnect(idx);
        }
    }
}

void SpkSpeakerCtrl::updateSpeaker(OSAlarm *, OSContext *) {
    for (s32 i = 0; i < 4; i++) {
        if (sMixingBuffer != nullptr) {
            SpeakerInfo& inf = getSpeakerInfo(i);

            if (inf._1) {
                if (!inf._23) {
                    if (sMixingBuffer->update(i)) {
                        bool v5 = true;

                        if (inf._1 && sMixingBuffer != nullptr) {
                            BOOL en = OSDisableInterrupts();

                            if (!WPADCanSendStreamData(i)) {
                                
                                OSRestoreInterrupts(en);
                            }
                            else {
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
                    }
                    else {
                        inf._22 = 1;
                    }
                }
            }
        }
    }
}

bool SpkSpeakerCtrl::isEnable(s32 idx) {
    bool ret = false;

    if (WPADIsSpeakerEnabled(idx) && sSpeakerInfo[idx]._1 != 0 && sSpeakerInfo[idx]._24 != 0 && sMixingBuffer != 0) {
        ret = true;
    }

    return ret;
}

void SpkSpeakerCtrl::extensionProcess(s32, s32) {

}

f32 SpkSpeakerCtrl::getDeviceVolume(s32 channel) {
    if (channel >= 0 && !WPADIsSpeakerEnabled(channel)) {
        return 0.0f;   
    }

    return WPADGetSpeakerVolume() / 127.0f;
}