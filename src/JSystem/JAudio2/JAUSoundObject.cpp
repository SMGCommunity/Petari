#include "JSystem/JAudio2/JAUSoundObject.hpp"
#include "JSystem/JAudio2/JAISeMgr.hpp"
#include "JSystem/JAudio2/JAISoundHandles.hpp"
#include "JSystem/JAudio2/JAISoundInfo.hpp"
#include "JSystem/JAudio2/JAISoundStarter.hpp"
#include "JSystem/JAudio2/JASGlobal.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"

JAUSoundObject::JAUSoundObject(TVec3f* pPos, u8 numHandles, JKRHeap* pHeap)
    : JAISoundHandles(new(pHeap, 0) JAISoundHandle[numHandles], numHandles), _10(0), mPos(pPos) {
    mIsAllocated = true;
}

JAUSoundObject::~JAUSoundObject() {
    dispose();
    delete[] mHandles;
}

void JAUSoundObject::dispose() {
    for (u32 idx = 0; idx < mNumHandles; idx++) {
        if (mHandles[idx].isSoundAttached()) {
            if (stopOK(mHandles[idx])) {
                mHandles[idx].getSound()->stop();
            } else {
                mHandles[idx].releaseSound();
            }
        }
    }
    mIsAllocated = false;
}

bool JAUSoundObject::stopOK(JAISoundHandle& rHandle) {
    return true;
}

void JAUSoundObject::process() {
    if (!mIsAllocated) {
        return;
    }
    setPos(*mPos);
}

JAISoundHandle* JAUSoundObject::startSound(JAISoundID soundID) {
    if (!mIsAllocated) {
        return nullptr;
    }

    JAISoundStarter* starter = JASGlobalInstance< JAISoundStarter >::getInstance();

    JAISoundHandle* handle = getHandleSoundID(soundID);
    if (handle == nullptr) {
        handle = getFreeHandleNotReserved();
    }

    if (handle == nullptr) {
        handle = getLowPrioSound(soundID);
    }

    if (handle != nullptr) {
        starter->startSound(soundID, handle, mPos);
    }

    return handle;
}

JAISoundHandle* JAUSoundObject::startSoundIndex(JAISoundID soundID, u8 index) {
    if (!mIsAllocated) {
        return nullptr;
    }

    JAISoundHandle* handle = &mHandles[index];
    JASGlobalInstance< JAISoundStarter >::getInstance()->startSound(soundID, handle, mPos);
    return handle;
}

JAISoundHandle* JAUSoundObject::startLevelSound(JAISoundID soundID) {
    if (!mIsAllocated) {
        return nullptr;
    }

    JAISeMgr* seMgr = JASGlobalInstance< JAISeMgr >::getInstance();

    JAISoundHandle* handle = getHandleSoundID(soundID);
    if (handle != nullptr) {
        handle->getSound()->updateLifeTime(1);
    } else {
        handle = getFreeHandleNotReserved();

        if (handle == nullptr) {
            handle = getLowPrioSound(soundID);
        }

        if (handle != nullptr) {
            seMgr->startSound(soundID, handle, mPos);

            if (handle->getSound() != nullptr) {
                handle->getSound()->setLifeTime(1, false);
            }
        }
    }

    return handle;
}

JAISoundHandle* JAUSoundObject::startLevelSoundIndex(JAISoundID soundID, u8 index) {
    if (!mIsAllocated) {
        return nullptr;
    }
    JAISeMgr* seMgr = JASGlobalInstance< JAISeMgr >::getInstance();
    JAISoundHandle* handle = getHandleSoundID(soundID);
    if (handle != nullptr) {
        handle->getSound()->updateLifeTime(1);
        return handle;
    }

    handle = &mHandles[index];
    seMgr->startSound(soundID, handle, mPos);
    if (handle->getSound() != nullptr) {
        handle->getSound()->setLifeTime(1, false);
    }

    return handle;
}

void JAUSoundObject::stopSound(JAISoundID soundID, u32 fadeTime) {
    JAISoundHandle* handle = getHandleSoundID(soundID);
    if (handle != nullptr) {
        handle->getSound()->stop(fadeTime);
    }
}

JAISoundHandle* JAUSoundObject::getLowPrioSound(JAISoundID soundID) {
    JAISoundHandle* handle;
    JAISoundInfo* info = JASGlobalInstance< JAISoundInfo >::getInstance();
    u32 lowPrio = 0xFFFF;
    JAISoundHandle* prioHandle = nullptr;

    for (int idx = 0; idx < mNumHandles; idx++) {
        if ((_10 & (1 << idx)) == 0) {
            handle = &mHandles[idx];
            u32 prio = info->getPriority(handle->getSound()->getID());
            if (prio < lowPrio) {
                lowPrio = prio;
                prioHandle = handle;
            }
        }
    }

    if (info->getPriority(soundID) >= lowPrio) {
        return prioHandle;
    }
    return nullptr;
}

JAISoundHandle* JAUSoundObject::getFreeHandleNotReserved() {
    for (int idx = 0; idx < mNumHandles; idx++) {
        if ((_10 & (1 << idx)) == 0 && mHandles[idx].getSound() == nullptr) {
            return &mHandles[idx];
        }
    }
    return nullptr;
}
