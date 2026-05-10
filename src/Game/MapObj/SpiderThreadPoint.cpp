#include "Game/MapObj/SpiderThreadPoint.hpp"
#include "Game/MapObj/SpiderThread.hpp"
#include "Game/MapObj/SpiderThreadWindCtrl.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/MathUtil.hpp"
#include <revolution/mtx.h>

namespace {
    SpiderThreadWindCtrl* getWindCtrl() {
        return MR::getSceneObj< SpiderThread >(SceneObj_SpiderThread)->mWindCtrl;
    }

    static const f32 sAccelRate = 0.02f;
    static const f32 sSpeedMinToStop = 0.1f;
    static const f32 sDistanceToStop = 1.0f;
    // static const _32 sPushAccelRate =
}  // namespace

SpiderThreadPoint::SpiderThreadPoint(const TVec3f& rPos, f32 friction)
    : mBasePos(rPos), mPosition(rPos), mVelocity(0.0f, 0.0f, 0.0f), mFriction(friction), mUp(0.0f, 1.0f, 0.0f), mFront(0.0f, 0.0f, 1.0f) {
    mWindStartTime = getWindCtrl()->getTimeToStartWind();
    mWindTime = getWindCtrl()->getWindTime();
    mPrevStretchDist = 0.0f;
}

bool SpiderThreadPoint::updateSpring() {
    mVelocity.scale(mFriction);
    TVec3f v1(mBasePos);
    v1.sub(mPosition);

    TVec3f v2(v1);
    v2.scale(sAccelRate);
    mVelocity.add(v2);
    mPosition.add(mVelocity);

    TVec3f v3(mBasePos);
    v3.sub(mPosition);
    v3.z = 0.0f;

    f32 mag1 = v3.length();
    f32 mag2 = mVelocity.length();

    if (mag2 < sSpeedMinToStop && mag1 < sDistanceToStop) {
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
            mVelocity.add(getWindCtrl()->mWind);
        } else {
            mWindStartTime = getWindCtrl()->getTimeToStartWind();
            mWindTime = getWindCtrl()->getWindTime();
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
    f32 stretchDist = mBasePos.distance(mPosition);
    if (mPrevStretchDist > 0.0f && stretchDist > 50.0f) {
        f32 diff = __fabsf(mPrevStretchDist - stretchDist);
        if (diff > 1.0f) {
            MR::startSystemLevelSE("SE_OJ_LV_SPIDER_THREAD_PULL", systemLevelParam(stretchDist, diff * 100.0f), -1);
        }
    }
    mPrevStretchDist = stretchDist;
}
