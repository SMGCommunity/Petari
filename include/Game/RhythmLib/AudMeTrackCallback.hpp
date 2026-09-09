#pragma once

#include <revolution.h>

typedef s32 (*AudCallbackFunc)(s32, s32, void*);

struct AudCallbackData {
    AudCallbackData() : mFunc(), mData() {
    }

    /* 0x0 */ AudCallbackFunc mFunc;
    /* 0x4 */ void* mData;
};

class AudCallback {
public:
    AudCallback() {};

    bool regist(AudCallbackFunc, void*);
    void callback(s32, s32);

    /* 0x0 */ AudCallbackData mCallbackData[32];
};

class AudMeTrackCallback : public AudCallback {
public:
    static AudCallback sMeTrackCallback;

    AudMeTrackCallback() {
    }

    static bool regist(AudCallbackFunc, void*);
    static void callback(s32, s32);
};
