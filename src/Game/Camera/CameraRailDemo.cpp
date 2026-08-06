#include "Game/Camera/CameraRailDemo.hpp"
#include "Game/Camera/CamTranslatorRailDemo.hpp"
#include "Game/Camera/CameraDirector.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraRailHolder.hpp"
#include "Game/LiveActor/RailRider.hpp"

void CameraRailDemo_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

CameraRailDemo::CameraRailDemo(const char* pName) : Camera(pName) {
}

void CameraRailDemo::reset() {
    mDemoTimer = 0;
    mCoord = 0.0f;
}

CameraTargetObj* CameraRailDemo::calc() {
    switch (mCalcType) {
    case 0:
        calcLinear();
        break;
    case 1:
        calcEaseInOut();
        break;
    case 2:
        calcDamp();
        break;
    }

    mDemoTimer++;
    return nullptr;
}

void CameraRailDemo::setParam(s32 zoneID, s32 railID, s32 calcType, s32 demoTime, f32 dampRatio) {
    mRailRider = MR::getCameraDirector()->mRailHolder->getRider(zoneID, railID);
    mCalcType = calcType;
    mDemoTime = demoTime;
    mDampRatio = dampRatio;
}

CamTranslatorBase* CameraRailDemo::createTranslator() {
    return new CamTranslatorRailDemo(this);
}

void CameraRailDemo::calcLinear() {
    if (mRailRider == nullptr) {
        return;
    }

    f32 t;
    if (mDemoTime <= 0) {
        t = 0.0f;
    } else if (mDemoTimer >= mDemoTime) {
        t = 1.0f;
    } else {
        t = static_cast< f32 >(mDemoTimer) / static_cast< f32 >(mDemoTime);
    }

    mRailRider->setCoord(t * mRailRider->getTotalLength());
    CameraLocalUtil::setPos(this, mRailRider->mCurPos);

    mRailRider->setCoord((t * mRailRider->getTotalLength()) + 1000.0f);
    CameraLocalUtil::setWatchPos(this, mRailRider->mCurPos);
    CameraLocalUtil::setUpVecAndWatchUpVec(this, TVec3f(0.0f, 1.0f, 0.0f));
}

void CameraRailDemo::calcEaseInOut() {
    if (mRailRider == nullptr) {
        return;
    }

    f32 t;
    if (mDemoTime <= 0) {
        t = 0.0f;
    } else if (mDemoTimer >= mDemoTime) {
        t = 1.0f;
    } else {
        t = static_cast< f32 >(mDemoTimer) / static_cast< f32 >(mDemoTime);
        t *= t;
        t *= t;
    }

    mCoord += t * (mRailRider->getTotalLength() - mCoord);

    mRailRider->setCoord(mCoord);
    CameraLocalUtil::setPos(this, mRailRider->mCurPos);

    mRailRider->setCoord(mCoord + 1000.0f);
    CameraLocalUtil::setWatchPos(this, mRailRider->mCurPos);
    CameraLocalUtil::setUpVecAndWatchUpVec(this, TVec3f(0.0f, 1.0f, 0.0f));
}

void CameraRailDemo::calcDamp() {
    if (mRailRider == nullptr) {
        return;
    }

    mCoord = mCoord * mDampRatio + mRailRider->getTotalLength() * (1.0f - mDampRatio);

    mRailRider->setCoord(mCoord);
    CameraLocalUtil::setPos(this, mRailRider->mCurPos);

    mRailRider->setCoord(mCoord + 1000.0f);
    CameraLocalUtil::setWatchPos(this, mRailRider->mCurPos);
    CameraLocalUtil::setUpVecAndWatchUpVec(this, TVec3f(0.0f, 1.0f, 0.0f));
}
