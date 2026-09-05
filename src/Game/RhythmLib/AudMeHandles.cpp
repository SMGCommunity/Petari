#include "Game/RhythmLib/AudMeHandles.hpp"

AudMeHandles::AudMeHandles(AudMeHandle* pHandles, int numHandles) : mHandles(pHandles), mNumHandles(numHandles) {
}

AudMeHandle* AudMeHandles::getHandleMeID(u32 id) {
    for (s32 i = 0; i < mNumHandles; i++) {
        AudMeHandle* handle = getHandle(i);
        if (handle->isMeAttached()) {
            if (handle->getMe()->mMeId == id) {
                return handle;
            }
        }
    }
    return nullptr;
}

AudMeHandle* AudMeHandles::getFreeHandle() {
    for (s32 i = 0; i < mNumHandles; i++) {
        AudMeHandle* handle = getHandle(i);
        if (!handle->isMeAttached()) {
            return handle;
        }
    }
    return nullptr;
}
