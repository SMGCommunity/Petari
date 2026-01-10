#include "Game/MapObj/SpiderThreadHangInfo.hpp"
#include "Game/MapObj/SpiderThreadPart.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/MathUtil.hpp"

SpiderThreadHangInfo::SpiderThreadHangInfo()
    : mBindedActor(nullptr), mPosition(nullptr), mThreadPart(nullptr), mHangIndex(-1), mIsHanging(false), _14(0) {}

void SpiderThreadHangInfo::startBind(LiveActor* pActor, const TVec3f* pPos, SpiderThreadPart* pThreadPart, s32 hangIndex, const TVec3f& rVel,
                                     s32 a2) {
    mBindedActor = pActor;
    mPosition = pPos;
    mThreadPart = pThreadPart;
    mHangIndex = hangIndex;
    mIsHanging = false;
    _14 = a2;

    if (!MR::isNearZero(rVel)) {
        mThreadPart->touchActor(mHangIndex, rVel);
    }
}

void SpiderThreadHangInfo::endBind() {
    if (mIsHanging) {
        endHang(false);
    }

    mBindedActor = nullptr;
    mThreadPart = nullptr;
    mHangIndex = -1;
    mIsHanging = false;
}

void SpiderThreadHangInfo::startHang() {
    mIsHanging = true;
    mThreadPart->startHang(mHangIndex);
}

void SpiderThreadHangInfo::endHang(bool b) {
    if (b) {
        MR::sendSimpleMsgToActor(ACTMES_HANG_CANCEL, mBindedActor);
    }

    mThreadPart->endHang(mHangIndex);
    mIsHanging = false;
}

void SpiderThreadHangInfo::updateHang() {
    mThreadPart->updateHang(*mPosition, mHangIndex);
}

void SpiderThreadHangInfo::copyInfo(const SpiderThreadHangInfo* pOther) {
    *this = *pOther;
}
