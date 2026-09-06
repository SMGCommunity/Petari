#include "Game/RhythmLib/AudMeTrackCallback.hpp"
#include <JSystem/JAudio2/JASCriticalSection.hpp>

AudCallback AudMeTrackCallback::sMeTrackCallback;

bool AudCallback::regist(AudCallbackFunc pFunc, void* pData) {
    JASCriticalSection crit;
    for (s32 i = 0; i < 32; i++) {
        if (mCallbackData[i].mFunc == nullptr) {
            mCallbackData[i].mFunc = pFunc;
            mCallbackData[i].mData = pData;
            return true;
        }
    }
    return false;
}

void AudCallback::callback(s32 a1, s32 a2) {
    for (s32 i = 0; i < 32; i++) {
        if (mCallbackData[i].mFunc != nullptr) {
            s32 ret = (*mCallbackData[i].mFunc)(a1, a2, mCallbackData[i].mData);
            if (ret < 0) {
                mCallbackData[i].mFunc = nullptr;
                mCallbackData[i].mData = nullptr;
            }
        }
    }
}

bool AudMeTrackCallback::regist(AudCallbackFunc pFunc, void* pData) {
    return sMeTrackCallback.regist(pFunc, pData);
}

void AudMeTrackCallback::callback(s32 a1, s32 a2) {
    sMeTrackCallback.callback(a1, a2);
}
