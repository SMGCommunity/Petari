#include "JSystem/JAudio2/JASCallback.hpp"
#include "JSystem/JAudio2/JASTrack.hpp"

// FIXME: `JASCriticalSection` ctor/dtor not inlined
bool JASCallbackMgr::regist(JASCallback* pCallback, void* pArgument) {
    JASCriticalSection criticalSection;
    for (s32 i = 0; i < JASCALLBACK_MAX_CALLBACKS; i++) {
        if (mCallbacks[i].mCallback == nullptr) {
            mCallbacks[i].mCallback = pCallback;
            mCallbacks[i].mArg = pArgument;
            return true;
        }
    }

    return false;
}

// FIXME: `JASCriticalSection` ctor/dtor not inlined
u32 JASCallbackMgr::reject(JASCallback* pCallback, void* pArgument) {
    u32 rejectNum = 0;
    JASCriticalSection criticalSection;
    for (s32 i = 0; i < JASCALLBACK_MAX_CALLBACKS; i++) {
        if (mCallbacks[i].mCallback == pCallback && mCallbacks[i].mArg == pArgument) {
            mCallbacks[i].mCallback = nullptr;
            mCallbacks[i].mArg = nullptr;
            rejectNum++;
        }
    }

    return rejectNum;
}

void JASCallbackMgr::callback() {
    for (s32 i = 0; i < JASCALLBACK_MAX_CALLBACKS; i++) {
        if (mCallbacks[i].mCallback != nullptr) {
            s32 result = mCallbacks[i].mCallback(mCallbacks[i].mArg);
            if (result < 0) {
                mCallbacks[i].mCallback = nullptr;
                mCallbacks[i].mArg = nullptr;
            }
        }
    }
}
