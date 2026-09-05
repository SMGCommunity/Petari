#pragma once

#include "Game/RhythmLib/AudMePlayer.hpp"
#include <JSystem/JAudio2/JAISoundHandles.hpp>

class AudMe;

class AudMeHandle {
public:
    /// @brief Creates a new `AudMeHandle`.
    AudMeHandle() : mMe(nullptr){};

    /// @brief Destroys the `AudMeHandle`.
    ~AudMeHandle() {
        releaseMe();
    }

    void releaseMe() {
        if (isMeAttached()) {
            mMe->mHandle = nullptr;
            mMe = nullptr;
        }
    }

    bool isMeAttached() const {
        return mMe != nullptr;
    }

    AudMe* getMe() {
        return mMe;
    }

    AudMe* operator->() const {
        return mMe;
    }

    /* 0x00 */ AudMe* mMe;
};

class AudMeHandles {
public:
    AudMeHandles(AudMeHandle*, int);

    AudMeHandle* getHandleMeID(u32);
    AudMeHandle* getFreeHandle();

    AudMeHandle* getHandle(int index) {
        return &mHandles[index];
    }

    AudMe* getMe(int n) {
        return getHandle(n)->getMe();
    }

    int getNumHandles() const {
        return mNumHandles;
    }

    /* 0x00 */ AudMeHandle* mHandles;
    /* 0x04 */ int mNumHandles;
};
