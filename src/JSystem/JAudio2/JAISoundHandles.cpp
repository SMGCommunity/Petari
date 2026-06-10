#include "JSystem/JAudio2/JAISoundHandles.hpp"
#include "JSystem/JAudio2/JAISeqDataMgr.hpp"
#include "JSystem/JAudio2/JAISound.hpp"

JAISoundHandle* JAISoundHandles::getHandleSoundID(JAISoundID id) {
    for (int i = 0; i < mNumHandles; i++) {
        if (mHandles[i].isSoundAttached()) {
            if (mHandles[i]->mSoundID == id) {
                return &mHandles[i];
            }
        }
    }

    return nullptr;
}

JAISoundHandle* JAISoundHandles::getFreeHandle() {
    for (int i = 0; i < mNumHandles; i++) {
        if (!mHandles[i].isSoundAttached()) {
            return &mHandles[i];
        }
    }

    return nullptr;
}

JAISoundHandle* JAISoundHandles::getHandleUserData(u32 addr) {
    for (int i = 0; i < mNumHandles; i++) {
        if (mHandles[i].isSoundAttached()) {
            if ((u32)mHandles[i]->getUserData() == addr) {
                return &mHandles[i];
            }
        }
    }

    return nullptr;
}

void JAISoundHandles::setPos(const TVec3f& pos) {
    for (int i = 0; i < mNumHandles; i++) {
        if (mHandles[i].isSoundAttached()) {
            mHandles[i].getSound()->setPos(pos);
        }
    }
}
