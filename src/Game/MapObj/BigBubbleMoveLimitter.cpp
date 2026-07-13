#include "Game/MapObj/BigBubbleMoveLimitter.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/BigBubbleHolder.hpp"
#include "Game/Util.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/MathUtil.hpp"

BigBubbleMoveLimitter::BigBubbleMoveLimitter(const char* pName, s32 limitterType)
    : NameObj(pName), mID(-1), mPosition(0, 0, 0), mAxis(0, 1, 0), mSide(1, 0, 0), mUp(0, 1, 0), mFront(0, 0, 1),
      mLimitMarginType(LimitterMarginType_Center), mCylinderRadius(500.0f), mCylinderHeight(500.0f), mPlaneWidth(500.0f), mPlaneHeight(500.0f),
      mType(limitterType) {
}

void BigBubbleMoveLimitter::init(const JMapInfoIter& rIter) {
    MR::getJMapInfoArg0NoInit(rIter, &mID);
    MR::getJMapInfoArg1WithInit(rIter, &mLimitMarginType);

    TPos3f mtx;
    MR::getJMapInfoMatrixFromRT(rIter, &mtx);
    mtx.getTrans(mPosition);
    mtx.getYDir(mAxis);
    mtx.getXYZDir(mSide, mUp, mFront);

    TVec3f scale;
    MR::getJMapInfoScale(rIter, &scale);
    mCylinderRadius = scale.x * 500.0f;
    mCylinderHeight = scale.y * 500.0f;
    mPlaneWidth = scale.x * 500.0f;
    mPlaneHeight = scale.y * 1000.0f;

    MR::createBigBubbleHolder();
    MR::addBigBubbleMoveLimitter(this);
}

bool BigBubbleMoveLimitter::limitPosition(TVec3f* pPos, f32 offset) const {
    bool limitted = false;

    offset = getLimitterOffset(offset);

    if (mType == LimitterType_Cylinder) {
        TVec3f ortho;
        f32 proj = MR::vecKillElement(*pPos - mPosition, mAxis, &ortho);
        MR::normalizeOrZero(&ortho);

        if (proj < 0.0f) {
            proj = 0.0f;
            limitted = true;
        }

        if (proj > mCylinderHeight) {
            proj = mCylinderHeight;
            limitted = true;
        }

        pPos->set(mPosition + mAxis * proj + ortho * (mCylinderRadius + offset));
    } else {
        TVec3f pos;
        MR::vecKillElement(*pPos - mPosition, mFront, &pos);
        pos.add(mFront * offset);

        f32 planeX = pos.dot(mSide);
        if (planeX < -mPlaneWidth) {
            pos.add(mSide * (-mPlaneWidth - planeX));
            limitted = true;
        } else if (planeX > mPlaneWidth) {
            pos.add(mSide * (mPlaneWidth - planeX));
            limitted = true;
        }

        f32 planeY = pos.dot(mUp);
        if (planeY < 0.0f) {
            pos.add(mUp * -planeY);
            limitted = true;
        } else if (planeY > mPlaneHeight) {
            pos.add(mUp * (mPlaneHeight - planeY));
            limitted = true;
        }

        pPos->set(mPosition + pos);
    }

    return limitted;
}

bool BigBubbleMoveLimitter::limitVelocity(TVec3f* pPos, const TVec3f& rVel, f32 offset) const {
    TVec3f nextPos;
    nextPos.set(rVel + *pPos);
    bool limitted = limitPosition(&nextPos, offset);
    pPos->set(nextPos - rVel);
    return limitted;
}

NameObj* MR::createBigBubbleMoveLimitterCylinder(const char* pName) {
    return new BigBubbleMoveLimitter(pName, BigBubbleMoveLimitter::LimitterType_Cylinder);
}

NameObj* MR::createBigBubbleMoveLimitterPlane(const char* pName) {
    return new BigBubbleMoveLimitter(pName, BigBubbleMoveLimitter::LimitterType_Plane);
}
