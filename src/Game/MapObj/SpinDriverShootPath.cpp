#include "Game/MapObj/SpinDriverShootPath.hpp"
#include "Game/LiveActor/RailRider.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ParabolicPath.hpp"

SpinDriverShootPath::SpinDriverShootPath() : mRailRider(), mPath(), mStartPosition(0.0f, 0.0f, 0.0f), mUsesParabolic() {
}

void SpinDriverShootPath::init(const JMapInfoIter& rIter) {
    mRailRider = new RailRider(rIter);
    mUsesParabolic = false;
}

void SpinDriverShootPath::initUsingParabolic(const JMapInfoIter& rIter, const TVec3f& rStartingPosition) {
    mRailRider = new RailRider(rIter);
    mUsesParabolic = mRailRider->getPointNum() <= 2;

    if (mUsesParabolic) {
        mPath = new ParabolicPath();
    }

    setStartPosition(rStartingPosition);
}

void SpinDriverShootPath::setStartPosition(const TVec3f& rStartPos) {
    if (mUsesParabolic) {
        TVec3f startPos(mRailRider->mStartPos);
        TVec3f endPos(mRailRider->mEndPos);
        mPath->initFromMaxHeight(rStartPos, endPos, startPos);
    }

    TVec3f position;
    calcPosition(&position, 0.0f);
    mStartPosition = rStartPos - position;
}

void SpinDriverShootPath::calcPosition(TVec3f* pOutPosition, f32 a2) const {
    if (mUsesParabolic) {
        mPath->calcPosition(pOutPosition, a2);
        return;
    }

    mRailRider->calcPosAtCoord(pOutPosition, a2 * mRailRider->getTotalLength());

    pOutPosition->add(mStartPosition * MR::getEaseOutValue(MR::normalize(a2, 0.0f, 0.5f), 1.0f, 0.0f, 1.0f));
}

void SpinDriverShootPath::calcDirection(TVec3f* pOutDirection, f32 a2, f32 a3) const {
    f32 v6;
    f32 v7;

    if (a2 < a3) {
        v6 = a3;
        v7 = 0.0f;
    } else {
        v6 = 1.0f;
        v7 = (1.0f - a3);

        if (a2 > v7) {
            v7 = v7;
        } else {
            v7 = a2;
            v6 = (a2 + a3);
        }
    }

    TVec3f stack_20;
    calcPosition(&stack_20, v7);
    TVec3f stack_14;
    calcPosition(&stack_14, v6);
    TVec3f stack_8 = stack_14;
    stack_8.sub(stack_8, stack_20);
    pOutDirection->set(stack_8);
    MR::normalizeOrZero(pOutDirection);
}

void SpinDriverShootPath::calcInitPose(TVec3f* pPos1, TVec3f* pPos2, TVec3f* pPos3, f32 f1) const {
    // FIXME
    TVec3f pos0;
    calcPosition(&pos0, 0.0f);

    TVec3f pos1;
    calcPosition(&pos1, 1.0f);

    TVec3f pos00;
    calcPosition(&pos00, 0.0f);

    TVec3f pos001;
    calcPosition(&pos001, 0.01f);

    TVec3f vec90(pos001);
    vec90 -= pos00;

    TVec3f vec60(vec90);
    MR::normalizeOrZero(&vec60);

    TVec3f vec84(pos1);
    vec84 -= pos0;

    TVec3f vec6C;
    vec6C.orthogonalize(vec60);
    MR::normalizeOrZero(&vec6C);

    if (MR::isNearZero(vec6C)) {
        MR::makeAxisVerticalZX(&vec6C, vec60);
    }

    MR::rotateVecDegree(&vec6C, vec60, f1);

    TVec3f vec78;
    vec78.cross(vec6C, vec60);
    MR::normalize(&vec78);

    if (pPos1 != nullptr) {
        pPos1->set(vec60);
    }

    if (pPos2 != nullptr) {
        pPos2->set(vec6C);
    }

    if (pPos3 != nullptr) {
        pPos3->set(vec78);
    }
}

f32 SpinDriverShootPath::getTotalLength() const {
    if (mUsesParabolic) {
        return mPath->getTotalLength(32);
    }

    return mRailRider->getTotalLength();
}

void SpinDriverShootPath::calcClippingInfo(TVec3f* pVec, f32* f1, f32 f2, f32 f3) {
    // FIXME: stack
    s32 totalSteps = getTotalLength() / f2;

    TVec3f pos0;
    calcPosition(&pos0, 0.0f);

    TBox3f box;
    box.set(pos0, pos0);

    for (s32 i = 1; i <= totalSteps; i++) {
        TVec3f pos;
        calcPosition(&pos, static_cast< f32 >(i) / totalSteps);

        box.extend(pos);
    }

    box.pad(f3);
    box.getCenter(pVec);

    TVec3f sizeVec;
    sizeVec.sub(box.f, box.i);

    *f1 = 0.5f * sizeVec.length();
}
