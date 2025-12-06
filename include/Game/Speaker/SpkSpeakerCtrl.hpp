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
    u32 _3C;
};

class SpkSpeakerCtrl {
public:
    static void setMixingBuffer(SpkMixingBuffer*);
    static void setup();
    static void connect(s32);
    static void disconnect(s32);
    static void setSpeakerOn(s32);
    static void setSpeakerOnCallback(s32, s32);
    static void setSpeakerPlay(s32);
    static void startPlayCallback(s32, s32);
    static void setSpeakerOff(s32);
    static void retryConnection(s32);
    static void reconnect(s32);
    static void framework();
    static void reconnectProcess(s32);
    static void initReconnect(s32);
    static void continuousUsingProcess(s32);
    static void updateSpeaker(OSAlarm*, OSContext*);
    static bool isEnable(s32);
    static void extensionProcess(s32, s32);
    static f32 getDeviceVolume(s32);
};
