#include "Game/MapObj/SpinDriverShootPath.hpp"
#include "JSystem/JMath/JMath.hpp"

SpinDriverShootPath::SpinDriverShootPath() {
    mRailRider = nullptr;
    mPath = nullptr;
    mStartPosition.x = 0.0f;
    mStartPosition.y = 0.0f;
    mStartPosition.z = 0.0f; 
    mUsesParabolic = false;
}

void SpinDriverShootPath::init(const JMapInfoIter &rIter) {
    mRailRider = new RailRider(rIter);
    mUsesParabolic = false;
}

void SpinDriverShootPath::initUsingParabolic(const JMapInfoIter &rIter, const TVec3f &rStartingPosition) {
    mRailRider = new RailRider(rIter);
    mUsesParabolic = mRailRider->getPointNum() <= 2;

    if (mUsesParabolic) {
        mPath = new ParabolicPath();
    }

    setStartPosition(rStartingPosition); 
} 

void SpinDriverShootPath::setStartPosition(const TVec3f &rStartPos) {
    if (mUsesParabolic) {
        TVec3f start(mRailRider->mStartPos);
        TVec3f end(mRailRider->mEndPos);
        mPath->initFromMaxHeight(rStartPos, end, start);
    }

    TVec3f position;
    TVec3f pos_diff;

    calcPosition(&position, 0.0f);
    TVec3f startPos(rStartPos);
    JMathInlineVEC::PSVECSubtract(startPos.toCVec(), position.toCVec(), startPos.toVec());
    mStartPosition.setInlinePS(startPos);
}

void SpinDriverShootPath::calcPosition(TVec3f *pOutPosition, f32 a2) const {
    if (mUsesParabolic) {
        mPath->calcPosition(pOutPosition, a2);
    }
    else {
        f32 length = mRailRider->getTotalLength();
        mRailRider->calcPosAtCoord(pOutPosition, a2 * length);
        f32 norm = MR::normalize(a2, 0.0f, 0.5f);
        f32 easeOut = MR::getEaseOutValue(norm, 1.0f, 0.0f, 1.0f);
        TVec3f pos(mStartPosition);
        pos.x *= easeOut;
        pos.y *= easeOut;
        pos.z *= easeOut;
        pOutPosition->add(pos);
    }
}

void SpinDriverShootPath::calcDirection(TVec3f *pOutDirection, f32 a2, f32 a3) const {
    f32 v6;
    f32 v7;
    
    if (a2 < a3) {
        v6 = a3;
        v7 = 0.0f;
    }
    else {
        v6 = 1.0f;
        v7 = (1.0f - a3);

        if (a2 > v7) {
            v7 = v7;
        }
        else {
            v7 = a2;
            v6 = (a2 + a3);
        }
    }

    TVec3f stack_20;
    calcPosition(&stack_20, v7);
    TVec3f stack_14;
    calcPosition(&stack_14, v6);
    TVec3f stack_8(stack_14);
    JMathInlineVEC::PSVECSubtract(stack_8.toCVec(), stack_14.toCVec(), stack_8.toVec());
    pOutDirection->set(stack_8);
    MR::normalize(pOutDirection);
}

// SpinDriverShootPath::calcInitPose

f32 SpinDriverShootPath::getTotalLength() const {
    if (mUsesParabolic) {
        return mPath->getTotalLength(0x20);
    }

    return mRailRider->getTotalLength();
}