#include "Game/MapObj/SpiderThreadPoint.hpp"
#include "Game/MapObj/SpiderThread.hpp"
#include "Game/MapObj/SpiderThreadWindCtrl.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/MathUtil.hpp"
#include <revolution/mtx.h>

inline SpiderThread* getThreadObj() {
    return reinterpret_cast< SpiderThread* >(MR::getSceneObjHolder()->getObj(SceneObj_SpiderThread));
}

SpiderThreadPoint::SpiderThreadPoint(const TVec3f& rPos, f32 springFactor)
    : mBasePos(rPos), mPosition(rPos), mVelocity(0.0f, 0.0f, 0.0f), mSpringFactor(springFactor), mUp(0.0f, 1.0f, 0.0f), mFront(0.0f, 0.0f, 1.0f) {
    mWindStartTime = getThreadObj()->mWindCtrl->getTimeToStartWind();
    mWindTime = getThreadObj()->mWindCtrl->getWindTime();
    mPrevStretchDist = 0.0f;
}

bool SpiderThreadPoint::updateSpring() {
    mVelocity.scale(mSpringFactor);
    TVec3f v1(mBasePos);
    v1.sub(mPosition);

    TVec3f v2(v1);
    v2.scale(0.02f);
    mVelocity.add(v2);
    mPosition.add(mVelocity);

    TVec3f v3(mBasePos);
    v3.sub(mPosition);
    v3.z = 0.0f;

    f32 mag1 = v3.length();
    f32 mag2 = mVelocity.length();

    if (mag2 < 0.1f && mag1 < 1.0f) {
        mVelocity.zero();
        return true;
    }

    startThreadLevelSound();
    return false;
}

void SpiderThreadPoint::updateDrawVec(const TVec3f* pPosA, const TVec3f* pPosB) {
    TVec3f diff(*pPosB);
    diff.sub(*pPosA);
    diff.z = 0.0f;

    if (!MR::isNearZero(diff)) {
        MR::normalize(&diff);
        mUp.set< f32 >(diff.y, -diff.x, 0.0f);
    }
}

void SpiderThreadPoint::updateWind(f32 dampener) {
    if (mWindStartTime > 0) {
        mWindStartTime--;
    } else {
        if (--mWindTime > 0) {
            mVelocity.add(getThreadObj()->mWindCtrl->mWind);
        } else {
            mWindStartTime = getThreadObj()->mWindCtrl->getTimeToStartWind();
            mWindTime = getThreadObj()->mWindCtrl->getWindTime();
        }
    }
    mVelocity.scale(dampener);
}

void SpiderThreadPoint::updatePos() {
    mPosition.add(mVelocity);
}

void SpiderThreadPoint::restrict(const TVec3f* pAnchor, f32 length) {
    TVec3f nextPosDiff(mPosition);
    nextPosDiff.add(mVelocity);
    nextPosDiff.sub(*pAnchor);

    TVec3f v2(nextPosDiff);
    if (!MR::isNearZero(v2)) {
        MR::normalize(&v2);

        if (nextPosDiff.squared() >= length * length) {
            TVec3f restriction = v2.scaleInline(length);
            mVelocity.x -= nextPosDiff.x - restriction.x;
            mVelocity.y -= nextPosDiff.y - restriction.y;
            mVelocity.z -= nextPosDiff.z - restriction.z;
        }
    }
}

void SpiderThreadPoint::updateHang(const TVec3f& rPos) {
    mPosition.setPS2(rPos);
    mVelocity.zero();
}

bool SpiderThreadPoint::tryTouch(f32 radius, const TVec3f& rVec, s32 channel) {
    if (!MR::isStarPointerPointing(mPosition, radius, channel, false)) {
        return false;
    }

    mVelocity.add(rVec);
    return true;
}

bool SpiderThreadPoint::tryPush(const TVec3f& rPos, f32 radius) {
    f32 dist = MR::calcDistanceXY(mPosition, rPos);
    if (dist > radius || mPosition.z < rPos.z) {
        return false;
    }

    mVelocity.z += getPosZDiff(rPos);

    return true;
}

void SpiderThreadPoint::startThreadLevelSound() {
    f32 stretchDist = PSVECDistance(&mBasePos, &mPosition);
    if (mPrevStretchDist > 0.0f && stretchDist > 50.0f) {
        f32 diff = __fabsf(mPrevStretchDist - stretchDist);
        if (diff > 1.0f) {
            MR::startSystemLevelSE("SE_OJ_LV_SPIDER_THREAD_PULL", systemLevelParam(stretchDist, diff * 100.0f), -1);
        }
    }
    mPrevStretchDist = stretchDist;
}
