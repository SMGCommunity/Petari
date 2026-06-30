#pragma once

#include <revolution.h>
#include <revolution/wenc.h>

class SpkMixingBuffer;

// NOTE: __attribute__((packed)) does not work for some reason.
#pragma pack(1)
struct SpeakerInfo {
    enum State {
        State_ENABLE = 0,
        State_ON = 1,
        State_PLAY = 2,
        State_3 = 3,
    };

    enum ReconnectState {
        Reconnect_NONE = 0,
        Reconnect_OFF = 1,
        Reconnect_WAIT = 2,
        Reconnect_ON = 3,
    };

    void setUsingTimeout(s32 time) {
        mUsingTimeOut = time;
    }

    bool mIsConnected;
    bool mIsPlaying;
    WENCInfo mWENCInfo;
    bool mIsUpdated;
    bool mIsMuted;
    u32 mState;
    u32 mReconnectState;
    s32 mReconnectTime;
    s32 mUsingTimeOut;
    s32 mRadioSensitivityTimer;
    s32 mExtensionTimer;
    u8 mVolume;
    u8 pad[3];
};
#pragma pack()

namespace SpkSpeakerCtrl {
    void setMixingBuffer(SpkMixingBuffer*);
    void setup();
    void connect(s32);
    void disconnect(s32);
    void setSpeakerOn(s32);
    void setSpeakerOnCallback(s32, s32);
    void setSpeakerPlay(s32);
    void startPlayCallback(s32, s32);
    void setSpeakerOff(s32);
    void retryConnection(s32);
    void reconnect(s32);
    void framework();
    void reconnectProcess(s32);
    void initReconnect(s32);
    void continuousUsingProcess(s32);
    void updateSpeaker(OSAlarm*, OSContext*);
    bool isEnable(s32);
    void extensionProcess(s32, s32);
    f32 getDeviceVolume(s32);
};  // namespace SpkSpeakerCtrl
