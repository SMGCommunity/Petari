#include "Game/RhythmLib/AudMeObject.hpp"

AudMeObject::AudMeObject(TVec3f* pPos, u8 numHandles, JKRHeap* pHeap)
    : AudMeHandles(new (pHeap, 0) AudMeHandle[numHandles], numHandles), mPos(pPos), mIsAllocated(true) {
}

AudMeObject::~AudMeObject() {
    delete[] mHandles;
}

void AudMeObject::dispose() {
    for (u32 i = 0; i < mNumHandles; i++) {
        if (getHandle(i)->isMeAttached()) {
            getHandle(i)->releaseMe();
        }
    }
    mIsAllocated = false;
}

AudMeHandle* AudMeObject::startMe(u32 id) {
    if (!mIsAllocated) {
        return nullptr;
    }
    if (id == -1) {
        return nullptr;
    }

    AudMeHandle* handle = getHandleMeID(id);
    if (handle != nullptr && handle->isMeAttached() && handle->getMe()->_28 == 0) {
        return nullptr;
    }

    if (handle == nullptr) {
        handle = getFreeHandle();
    }

    if (handle == nullptr) {
        handle = getLowestPrioHandle(id);
    }

    if (handle != nullptr) {
        AudMeMgr::getInstance()->startMe(id, handle, mPos);
    }

    return handle;
}

AudMeHandle* AudMeObject::getLowestPrioHandle(u32 id) {
    u8 lowestPrio = 0xFF;
    AudMeHandle* lowestPrioHandle = nullptr;
    for (s32 i = 0; i < mNumHandles; i++) {
        AudMe* me = getHandle(i)->getMe();
        if (me == nullptr) {
            return getHandle(i);
        }
        if (me->mMeId == 0xFFFFFFFF) {
            return getHandle(i);
        }

        u8 prio = AudMeMgr::getInstance()->getPriority(me->mMeId);
        if (prio < lowestPrio) {
            lowestPrio = prio;
            lowestPrioHandle = getHandle(i);
        }
    }

    if (AudMeMgr::getInstance()->getPriority(id) >= lowestPrio) {
        return lowestPrioHandle;
    }

    return nullptr;
}
