#pragma once

#include <revolution/types.h>

typedef s32 JASCallback(void*);

#define JASCALLBACK_MAX_CALLBACKS 32

struct JASCallbackMgr {
    struct TCallback {
        TCallback() : mCallback(nullptr), mArg(nullptr) {}

        /* 0x0 */ JASCallback* mCallback;
        /* 0x4 */ void* mArg;
    };

    bool regist(JASCallback*, void*);
    u32 reject(JASCallback*, void*);
    void callback();

    /* 0x0 */ TCallback mCallbacks[JASCALLBACK_MAX_CALLBACKS];
};
