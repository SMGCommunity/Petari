#include "Game/Ride/SwingRopePoint.hpp"
#include "Game/Util/MathUtil.hpp"

SwingRopePoint::SwingRopePoint(const TVec3f& rPos)
    : mPosition(rPos), mVelocity(0.0f, 0.0f, 0.0f), mSide(1.0f, 0.0f, 0.0f), mUp(0.0f, 1.0f, 0.0f), mFront(0.0f, 0.0f, 1.0f) {}

void SwingRopePoint::addAccel(const TVec3f& rAccel) {
    mVelocity.add(rAccel);
}

bool SwingRopePoint::restrict(const TVec3f& rAnchor, f32 length, const TVec3f* pVec) {
    TVec3f nextPosDiff(mPosition);
    nextPosDiff.add(mVelocity);
    nextPosDiff.sub(rAnchor);

    if (pVec != nullptr) {
        nextPosDiff.sub(*pVec);
    }

    TVec3f v2(nextPosDiff);
    if (MR::isNearZero(v2)) {
        v2.set(mUp);
        v2.scale(-1.0f);
    } else {
        MR::normalize(&v2);
        mUp.set(v2);
        mUp.mult(-1.0f);
    }

    if (nextPosDiff.squared() >= length * length) {
        TVec3f restriction = v2.scaleInline(length);
        mVelocity.x -= nextPosDiff.x - restriction.x;
        mVelocity.y -= nextPosDiff.y - restriction.y;
        mVelocity.z -= nextPosDiff.z - restriction.z;
        return true;
    }

    return false;
}

void SwingRopePoint::strain(const TVec3f& rAnchor, f32 length) {
    TVec3f nextPosDiff(mPosition);
    nextPosDiff.add(mVelocity);
    nextPosDiff.sub(rAnchor);

    TVec3f v2(nextPosDiff);
    if (MR::isNearZero(v2)) {
        v2.set(mUp);
        v2.scale(-1.0f);
    } else {
        MR::normalize(&v2);
        mUp.set(v2);
        mUp.mult(-1.0f);
    }

    TVec3f restriction = v2.scaleInline(length);
    mVelocity.x -= nextPosDiff.x - restriction.x;
    mVelocity.y -= nextPosDiff.y - restriction.y;
    mVelocity.z -= nextPosDiff.z - restriction.z;
}

void SwingRopePoint::updatePos(f32 vel) {
    mPosition.add(mVelocity);
    mVelocity.scale(vel);
}

void SwingRopePoint::updateAxis(const TVec3f& rAxis) {
    TVec3f side(mSide);
    PSVECCrossProduct(&mUp, &rAxis, &mSide);
    if (MR::isNearZero(mSide)) {
        mSide.set(side);
    } else {
        MR::normalize(&mSide);
    }

    TVec3f front(mFront);
    PSVECCrossProduct(&mSide, &mUp, &mFront);
    if (MR::isNearZero(mFront)) {
        mFront.set(front);
    } else {
        MR::normalize(&mFront);
    }
}

void SwingRopePoint::updatePosAndAxis(const TVec3f& rAxis, f32 vel) {
    updatePos(vel);
    updateAxis(rAxis);
}

void SwingRopePoint::stop() {
    mVelocity.zero();
}

void SwingRopePoint::setInfo(const TVec3f& rTarget, const TVec3f& rVelocity, const TVec3f& rAnchor, f32 length) {
    mVelocity.set(rVelocity);

    TVec3f direction(rTarget);
    direction.sub(rAnchor);

    if (MR::isNearZero(direction)) {
        direction.set< f32 >(0.0f, -1.0f, 0.0f);
    } else {
        MR::normalize(&direction);
    }

    mPosition.set(direction);
    mPosition.scale(length);
    mPosition.add(rAnchor);

    mUp.set(direction);
    mUp.mult(-1.0f);

    PSVECCrossProduct(&mUp, &mFront, &mSide);
    PSVECCrossProduct(&mSide, &mUp, &mFront);
    MR::normalize(&mFront);
    PSVECCrossProduct(&mUp, &mFront, &mSide);
    MR::normalize(&mSide);
}

void SwingRopePoint::setPosAndAxis(const TVec3f& rPos, const TVec3f& rSide, const TVec3f& rUp, const TVec3f& rFront) {
    mPosition.set(rPos);
    mSide.set(rSide);
    mUp.set(rUp);
    mFront.set(rFront);
}

void SwingRopePoint::setAndUpdatePosAndAxis(const TVec3f& rPos, const TVec3f& rUp, const TVec3f& rAxis) {
    mPosition.set(rPos);
    mVelocity.zero();
    mUp.set(rUp);
    updatePos(1.0f);
    updateAxis(rAxis);
}
