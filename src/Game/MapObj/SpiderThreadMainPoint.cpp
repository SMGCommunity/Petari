#include "Game/MapObj/SpiderThreadMainPoint.hpp"
#include "Game/MapObj/SpiderThreadPart.hpp"
#include "Game/MapObj/SpiderThreadRadialLine.hpp"
#include "Game/Util/MathUtil.hpp"
#include <revolution/mtx.h>

namespace {
    static const f32 sFrictionRate = 0.9f;
    // static const _32 sSpeedMin =
    static const f32 sWindFrictionRate = 0.9f;
    static const f32 sBasePosAccelRate = 0.3f;
    static const f32 sBasePosDiffMin = 1.0f;
    static const f32 sNearMainPointAccelRate = 0.1f;
    static const f32 sNearHangPointAccelRate = 0.02f;
    static const f32 sNearPointDiffMin = 1.0f;
}  // namespace

SpiderThreadPointNearInfo::SpiderThreadPointNearInfo()
    : mMainPoint(nullptr), mPartPoint(nullptr), mDistToPoint(0.0f), mSpringFactor(0.0f), mThreadPart(nullptr) {
}

void SpiderThreadPointNearInfo::setInfo(f32 springFactor, SpiderThreadPart* pThreadPart, const TVec3f& rVec) {
    mSpringFactor = springFactor;
    mThreadPart = pThreadPart;

    if (mMainPoint != nullptr) {
        mDistToPoint = rVec.distance(mMainPoint->mPosition);
    } else if (mPartPoint != nullptr) {
        mDistToPoint = rVec.distance(mPartPoint->mPosition);
    } else {
        mDistToPoint = 0.0f;
    }
}

void SpiderThreadPointNearInfo::clear() {
    mMainPoint = nullptr;
    mPartPoint = nullptr;
    mSpringFactor = 0.0f;
    mThreadPart = nullptr;
    mDistToPoint = 0.0f;
}

const TVec3f* SpiderThreadPointNearInfo::getPos() const {
    if (mMainPoint != nullptr) {
        return &mMainPoint->mPosition;
    }

    if (mPartPoint != nullptr) {
        return &mPartPoint->mPosition;
    }

    return nullptr;
}

SpiderThreadMainPoint::SpiderThreadMainPoint(const TVec3f& rPos, int nearInfoBufferSize)
    : SpiderThreadPoint(rPos, sFrictionRate), mRadialLine(nullptr), mNearInfoBufferSize(nearInfoBufferSize), mNumNearInfos(0), mNearInfos(nullptr) {
    mNearInfos = new SpiderThreadPointNearInfo[nearInfoBufferSize];
}

void SpiderThreadMainPoint::addNearMainPoint(SpiderThreadMainPoint* pMainPoint, SpiderThreadPart* pThreadPart) {
    getNearPointInfo(mNumNearInfos)->mMainPoint = pMainPoint;
    getNearPointInfo(mNumNearInfos)->setInfo(sNearMainPointAccelRate, pThreadPart, mPosition);
    mNumNearInfos++;
}

void SpiderThreadMainPoint::addNearPartPoint(SpiderThreadPoint* pThreadPoint, f32 springFactor) {
    getNearPointInfo(mNumNearInfos)->mPartPoint = pThreadPoint;
    getNearPointInfo(mNumNearInfos)->setInfo(springFactor, nullptr, mPosition);
    mNumNearInfos++;
}

void SpiderThreadMainPoint::removeNearPoint(const SpiderThreadPoint* pThreadPoint) {
    for (s32 idx = 0; idx < mNumNearInfos; idx++) {
        if (mNearInfos[idx].getPos() == &pThreadPoint->mPosition) {
            mNearInfos[idx] = *getLastInfo();
            getLastInfo()->clear();
            mNumNearInfos--;
            return;
        }
    }
}

void SpiderThreadMainPoint::addNearPointToRadial(SpiderThreadPoint* pThreadPoint) {
    if (mRadialLine != nullptr) {
        for (s32 idx = 0; idx < mRadialLine->mNumPoints; idx++) {
            if (mRadialLine->getPoint(idx) == this) {
                addNearPartPoint(pThreadPoint, sNearMainPointAccelRate);
            } else {
                mRadialLine->getPoint(idx)->addNearPartPoint(pThreadPoint, sNearHangPointAccelRate);
            }
        }
    }
}

void SpiderThreadMainPoint::removeNearPointFromRadial(const SpiderThreadPoint* pThreadPoint) {
    if (mRadialLine != nullptr) {
        for (s32 idx = 0; idx < mRadialLine->mNumPoints; idx++) {
            mRadialLine->getPoint(idx)->removeNearPoint(pThreadPoint);
        }
    }
}

void SpiderThreadMainPoint::cutNearPoints(s32* numCutPoints, SpiderThreadMainPoint** pMainPoints) {
    SpiderThreadPart* threadPart;
    for (s32 idx = 0; idx < mNumNearInfos; idx++) {
        SpiderThreadMainPoint* mainPoint;
        if (getNearPointInfo(idx)->mMainPoint != nullptr) {
            getNearPointInfo(idx)->mMainPoint->removeNearPoint(this);

            mainPoint = pMainPoints[*numCutPoints];
            threadPart = getNearPointInfo(idx)->mThreadPart;

            getNearPointInfo(idx)->mMainPoint->addNearMainPoint(mainPoint, threadPart);
            mainPoint->mPosition.x = mPosition.x;
            mainPoint->mPosition.y = mPosition.y;
            mainPoint->mPosition.z = mPosition.z;
            mainPoint->addNearMainPoint(getNearPointInfo(idx)->mMainPoint, threadPart);
            threadPart->cut(this, mainPoint);

            *numCutPoints = *numCutPoints + 1;
        }
    }
}

void SpiderThreadMainPoint::updateVelocity() {
    updateWind(sWindFrictionRate);

    TVec3f springBack(mBasePos);
    springBack.sub(mPosition);

    TVec3f diff(springBack);

    if (springBack.length() > sBasePosDiffMin) {
        MR::normalizeOrZero(&springBack);
        springBack.scale(sBasePosAccelRate);
        mVelocity.add(springBack);
    }

    for (s32 idx = 0; idx < mNumNearInfos; idx++) {
        closeToNearPoint(getNearPointInfo(idx));
    }
}

void SpiderThreadMainPoint::closeToNearPoint(const SpiderThreadPointNearInfo* pNearInfo) {
    if (pNearInfo->getPos() != nullptr) {
        TVec3f springBack(*pNearInfo->getPos());
        springBack.sub(mPosition);
        f32 dist = springBack.length();

        if (dist < sNearPointDiffMin) {
            return;
        }

        f32 distDiff = dist - pNearInfo->mDistToPoint;

        if (distDiff < 0.0f) {
            return;
        }

        MR::normalizeOrZero(&springBack);
        springBack.scale(distDiff * pNearInfo->mSpringFactor);
        mVelocity.add(springBack);
    }
}
