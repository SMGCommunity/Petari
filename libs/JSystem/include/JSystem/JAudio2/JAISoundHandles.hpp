#pragma once

#include "JSystem/JAudio2/JAISound.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class JAISeqDataUser;

class JAISoundHandles {
public:
    JAISoundHandles(JAISoundHandle* handles, int handleNum) {
        mHandles = handles;
        mNumHandles = handleNum;
    };

    JAISoundHandle& operator[](int n) {
        return mHandles[n];
    }

    JAISoundHandle* getHandle(int n) {
        return &mHandles[n];
    }

    JAISound* getSound(int n) {
        return getHandle(n)->getSound();
    }

    JAISoundHandle* getHandleSoundID(JAISoundID);
    JAISoundHandle* getFreeHandle();
    JAISoundHandle* getHandleUserData(u32);

    void setPos(const TVec3f&);

    /* 0x00 */ JAISoundHandle* mHandles;
    /* 0x04 */ int mNumHandles;
};
