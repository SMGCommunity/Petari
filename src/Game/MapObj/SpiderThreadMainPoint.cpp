#include "Game/MapObj/SpiderThreadMainPoint.hpp"
#include "Game/MapObj/SpiderThreadPart.hpp"
#include "Game/MapObj/SpiderThreadRadialLine.hpp"
#include "Game/Util/MathUtil.hpp"
#include <revolution/mtx.h>

SpiderThreadPointNearInfo::SpiderThreadPointNearInfo()
    : mMainPoint(nullptr), mPartPoint(nullptr), mDistToPoint(0.0f), mSpringFactor(0.0f), mThreadPart(nullptr) {}

void SpiderThreadPointNearInfo::setInfo(f32 springFactor, SpiderThreadPart* pThreadPart, const TVec3f& rVec) {
    mSpringFactor = springFactor;
    mThreadPart = pThreadPart;

    if (mMainPoint != nullptr) {
        mDistToPoint = PSVECDistance(&rVec, &mMainPoint->mPosition);
    } else if (mPartPoint != nullptr) {
        mDistToPoint = PSVECDistance(&rVec, &mPartPoint->mPosition);
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
    : SpiderThreadPoint(rPos, 0.9f), mRadialLine(nullptr), mNearInfoBufferSize(nearInfoBufferSize), mNumNearInfos(0), mNearInfos(nullptr) {
    mNearInfos = new SpiderThreadPointNearInfo[nearInfoBufferSize];
}

void SpiderThreadMainPoint::addNearMainPoint(SpiderThreadMainPoint* pMainPoint, SpiderThreadPart* pThreadPart) {
    mNearInfos[mNumNearInfos].mMainPoint = pMainPoint;
    mNearInfos[mNumNearInfos].setInfo(0.1f, pThreadPart, mPosition);
    mNumNearInfos++;
}

void SpiderThreadMainPoint::addNearPartPoint(SpiderThreadPoint* pThreadPoint, f32 springFactor) {
    mNearInfos[mNumNearInfos].mPartPoint = pThreadPoint;
    mNearInfos[mNumNearInfos].setInfo(springFactor, nullptr, mPosition);
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
            if (mRadialLine->mPoints[idx] == this) {
                addNearPartPoint(pThreadPoint, 0.1f);
            } else {
                mRadialLine->mPoints[idx]->addNearPartPoint(pThreadPoint, 0.02f);
            }
        }
    }
}

void SpiderThreadMainPoint::removeNearPointFromRadial(const SpiderThreadPoint* pThreadPoint) {
    if (mRadialLine != nullptr) {
        for (s32 idx = 0; idx < mRadialLine->mNumPoints; idx++) {
            mRadialLine->mPoints[idx]->removeNearPoint(pThreadPoint);
        }
    }
}

void SpiderThreadMainPoint::cutNearPoints(s32* numCutPoints, SpiderThreadMainPoint** pMainPoints) {
    SpiderThreadPart* threadPart;
    for (s32 idx = 0; idx < mNumNearInfos; idx++) {
        SpiderThreadMainPoint* mainPoint;
        if (mNearInfos[idx].mMainPoint != nullptr) {
            mNearInfos[idx].mMainPoint->removeNearPoint(this);

            mainPoint = pMainPoints[*numCutPoints];
            threadPart = mNearInfos[idx].mThreadPart;

            mNearInfos[idx].mMainPoint->addNearMainPoint(mainPoint, threadPart);
            mainPoint->mPosition.x = mPosition.x;
            mainPoint->mPosition.y = mPosition.y;
            mainPoint->mPosition.z = mPosition.z;
            mainPoint->addNearMainPoint(mNearInfos[idx].mMainPoint, threadPart);
            threadPart->cut(this, mainPoint);

            *numCutPoints = *numCutPoints + 1;
        }
    }
}

void SpiderThreadMainPoint::updateVelocity() {
    updateWind(0.9f);

    TVec3f springBack(mBasePos);
    springBack.sub(mPosition);

    TVec3f diff(springBack);

    if (springBack.length() > 1.0f) {
        MR::normalizeOrZero(&springBack);
        springBack.scale(0.3f);
        mVelocity.add(springBack);
    }

    for (s32 idx = 0; idx < mNumNearInfos; idx++) {
        closeToNearPoint(&mNearInfos[idx]);
    }
}

void SpiderThreadMainPoint::closeToNearPoint(const SpiderThreadPointNearInfo* pNearInfo) {
    if (pNearInfo->getPos() != nullptr) {
        TVec3f springBack(*pNearInfo->getPos());
        springBack.sub(mPosition);
        f32 dist = springBack.length();

        if (dist < 1.0f) {
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
