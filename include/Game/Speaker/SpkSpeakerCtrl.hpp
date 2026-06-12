#pragma once

#include <revolution.h>
#include <revolution/wenc.h>

class SpkMixingBuffer;

struct SpeakerInfo {
    u8 _0;
    u8 _1;
    WENCInfo _2;
    u8 _22;
    u8 _23;
    u32 _24;
    u32 _28;
    s32 _2C;
    s32 _30;
    u32 _34;
    u32 _38;
    // u32 _3C;
};

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
