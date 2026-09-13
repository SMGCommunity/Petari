#include "Game/LiveActor/RailRider.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/SceneUtil.hpp"

void RailRider_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)-1.0f;
}

RailRider::RailRider(const JMapInfoIter& rIter)
    : mBezierRail(), mCoord(), mSpeed(), mIsNotReverse(true), mCurPos(0.0f), mCurDirection(1.0f, 0.0f, 0.0f), mStartPos(0.0f), mEndPos(0.0f) {
    const JMapInfo* info = nullptr;
    JMapInfoIter iter;
    MR::getRailInfo(&iter, &info, rIter);
    initBezierRail(iter, info);
}

RailRider::RailRider(s32 a1, s32 a2)
    : mBezierRail(), mCoord(), mSpeed(), mIsNotReverse(true), mCurPos(0.0f), mCurDirection(1.0f, 0.0f, 0.0f), mStartPos(0.0f), mEndPos(0.0f) {
    const JMapInfo* info = nullptr;
    JMapInfoIter iter;
    MR::getCameraRailInfo(&iter, &info, a1, a2);
    initBezierRail(iter, info);
}

void RailRider::move() {
    if (mIsNotReverse) {
        mCoord += mSpeed;
    } else {
        mCoord -= mSpeed;
    }

    mCoord = mBezierRail->normalizePos(mCoord, 1);
    syncPosDir();
}

void RailRider::moveToNearestPos(const TVec3f& rPos) {
    mCoord = mBezierRail->getNearestRailPosCoord(rPos);
    syncPosDir();
}

void RailRider::moveToNearestPoint(const TVec3f& rPos) {
    f32 nearestDistance = FLOAT_MAX;
    int nearestPoint = 0;
    TVec3f pointPos;
    for (int i = 0; i < getPointNum(); i++) {
        copyPointPos(&pointPos, i);
        f32 distance = rPos.squared(pointPos);
        if (distance < nearestDistance) {
            nearestDistance = distance;
            nearestPoint = i;
        }
    }

    mCoord = getPointCoord(nearestPoint);
    syncPosDir();
}

void RailRider::moveToNextPoint() {
    mCoord = mBezierRail->getRailPosCoord(getNextPointNo());
    syncPosDir();
}

void RailRider::reverse() {
    if (mIsNotReverse != false) {
        mIsNotReverse = false;
    } else {
        mIsNotReverse = true;
    }

    syncPosDir();
}

void RailRider::calcPosAtCoord(TVec3f* pOutVec, f32 a2) const {
    mBezierRail->calcPos(pOutVec, a2);
}

void RailRider::calcDirectionAtCoord(TVec3f* pOutVec, f32 a2) const {
    mBezierRail->calcDirection(pOutVec, a2);
}

f32 RailRider::calcNearestPos(const TVec3f& rPos) const {
    return mBezierRail->getNearestRailPosCoord(rPos);
}

f32 RailRider::getTotalLength() const {
    return mBezierRail->getTotalLength();
}

f32 RailRider::getPartLength(int idx) const {
    return mBezierRail->getPartLength(idx);
}

bool RailRider::isLoop() const {
    return mBezierRail->mIsClosed;
}

bool RailRider::isReachedGoal() const {
    if (isLoop()) {
        return false;
    }

    bool result;
    bool forwardGoal;
    result = true;
    forwardGoal = false;
    if (mIsNotReverse) {
        if (MR::isNearZero(mCoord - getTotalLength())) {
            forwardGoal = true;
        }
    }

    if (!forwardGoal) {
        bool reverseGoal = false;
        if (!mIsNotReverse && MR::isNearZero(mCoord)) {
            reverseGoal = true;
        }

        if (!reverseGoal) {
            result = false;
        }
    }

    return result;
}

bool RailRider::isReachedEdge() const {
    bool ret;

    if (mBezierRail->mIsClosed) {
        return false;
    } else {
        ret = true;

        if (!MR::isNearZero(mCoord)) {
            f32 val = mCoord - mBezierRail->getTotalLength();
            if (!MR::isNearZero(val)) {
                ret = false;
            }
        }
    }

    return ret;
}

void RailRider::setCoord(f32 coord) {
    mCoord = coord;
    mCoord = mBezierRail->normalizePos(mCoord, 1);
    syncPosDir();
}

void RailRider::setSpeed(f32 coord) {
    mSpeed = coord;
}

bool RailRider::getRailArgWithInit(const char* pStr, s32* pOut) const {
    s32 val;
    if (!mBezierRail->mIter->getValue< s32 >(pStr, &val)) {
        return false;
    }

    *pOut = val;
    return true;
}

bool RailRider::getRailArgNoInit(const char* pStr, s32* pOut) const {
    s32 val;
    if (!mBezierRail->mIter->getValue< s32 >(pStr, &val)) {
        return false;
    }

    if (val == -1) {
        return false;
    }

    *pOut = val;
    return true;
}

f32 RailRider::getNextPointCoord() const {
    return mBezierRail->getRailPosCoord(getNextPointNo());
}

f32 RailRider::getCurrentPointCoord() const {
    return mBezierRail->getRailPosCoord(mCurPoint);
}

s32 RailRider::getPointNum() const {
    return mBezierRail->mPointNum;
}

void RailRider::copyPointPos(TVec3f* pOut, s32 pointNum) const {
    JMapInfoIter iter;
    mBezierRail->calcRailCtrlPointIter(&iter, pointNum);
    MR::getRailPointPos0(iter, pOut);
}

f32 RailRider::getPointCoord(s32 idx) const {
    return mBezierRail->getRailPosCoord(idx);
}

void RailRider::initBezierRail(const JMapInfoIter& rIter, const JMapInfo* pInfo) {
    mBezierRail = new BezierRail(rIter, pInfo);
    syncPosDir();
    setCoord(mBezierRail->getTotalLength());
    mEndPos.set(mCurPos);
    setCoord(0.0f);
    mStartPos.set(mCurPos);
}

bool RailRider::getPointArgS32NoInit(const char* pStr, s32* pOut, s32 pointNum) const {
    s32 val;
    JMapInfoIter iter;

    mBezierRail->calcRailCtrlPointIter(&iter, pointNum);
    val = -1;
    iter.getValue< s32 >(pStr, &val);

    if (val != -1) {
        *pOut = val;
        return true;
    }

    return false;
}

bool RailRider::getPointArgS32WithInit(const char* pStr, s32* pOut, s32 pointNum) const {
    JMapInfoIter iter;
    mBezierRail->calcRailCtrlPointIter(&iter, pointNum);
    iter.getValue< s32 >(pStr, pOut);
    return true;
}

bool RailRider::getCurrentPointArgS32NoInit(const char* pStr, s32* pOut) const {
    s32 val;
    JMapInfoIter iter;

    mBezierRail->calcRailCtrlPointIter(&iter, mCurPoint);
    val = -1;
    iter.getValue< s32 >(pStr, &val);

    if (val != -1) {
        *pOut = val;
        return true;
    }

    return false;
}

bool RailRider::getCurrentPointArgS32WithInit(const char* pStr, s32* pOut) const {
    JMapInfoIter iter;
    mBezierRail->calcRailCtrlPointIter(&iter, mCurPoint);
    iter.getValue< s32 >(pStr, pOut);
    return true;
}

bool RailRider::getNextPointArgS32NoInit(const char* pStr, s32* pOut) const {
    s32 val;
    JMapInfoIter iter;

    mBezierRail->calcRailCtrlPointIter(&iter, getNextPointNo());
    val = -1;
    iter.getValue< s32 >(pStr, &val);

    if (val != -1) {
        *pOut = val;
        return true;
    }

    return false;
}

bool RailRider::getNextPointArgS32WithInit(const char* pStr, s32* pOut) const {
    JMapInfoIter iter;
    mBezierRail->calcRailCtrlPointIter(&iter, getNextPointNo());
    iter.getValue< s32 >(pStr, pOut);
    return true;
}

s32 RailRider::getNextPointNo() const {
    int step = mIsNotReverse ? 1 : -1;
    if (isLoop()) {
        int pointNum = getPointNum();
        int nextPoint = pointNum + mCurPoint;
        int offset = step + pointNum;
        return (offset + nextPoint) % pointNum;
    }

    return MR::clamp(mCurPoint + step, 0, getPointNum() - 1);
}

void RailRider::syncPosDir() {
    if (0.0f < mCoord && mCoord < mBezierRail->getTotalLength()) {
        mBezierRail->calcPosDir(&mCurPos, &mCurDirection, mCoord);
    } else {
        if (mCoord == 0.0f) {
            mBezierRail->calcPos(&mCurPos, mCoord);
            mBezierRail->calcDirection(&mCurDirection, 0.1f);
        } else {
            mBezierRail->calcPos(&mCurPos, mCoord);
            mBezierRail->calcDirection(&mCurDirection, mBezierRail->getTotalLength() - 0.1f);
        }
    }

    if (!mIsNotReverse) {
        mCurDirection.scale(-1.0f);
    }

    JMapInfoIter iter;
    f32 coord = mCoord;
    mBezierRail->calcCurrentRailCtrlPointIter(&iter, coord, mIsNotReverse);
    iter.getValue< s32 >("id", &mCurPoint);
}
