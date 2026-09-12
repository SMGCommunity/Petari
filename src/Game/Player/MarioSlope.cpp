#include "Game/Map/HitInfo.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioState.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"

void Mario::lockGroundCheck(void* owner, bool simple) {
    _574 = owner;
    _10._5 = simple;
    _10._4 = true;
    mMovementStates._36 = simple;
}

void Mario::unlockGroundCheck(void* owner) {
    if (!owner || _574 == owner) {
        _574 = nullptr;
        _10._4 = false;
    }
}

bool Mario::isUseSimpleGroundCheck() const {
    if (_10._4) {
        return _10._5;
    }
    if (mMovementStates._23) {
        return true;
    }
    if (isPlayerModeTeresa()) {
        return true;
    }
    if (!mMovementStates._1) {
        return mMovementStates._23;
    }
    if (calcAngleD(_368) > mActor->getConst().getTable()->mFlatAngle - 5.0f) {
        Triangle ground;
        if (MR::getFirstPolyOnLineBFast(mPosition - *getGravityVec() * 30.0f, *getGravityVec() * 100.0f, nullptr, &ground)) {
            if (calcAngleD(*ground.getNormal(0)) < mActor->getConst().getTable()->mFlatAngle - 5.0f) {
                return false;
            }
        }
        return true;
    }
    getAnimator()->isLandingAnimationRun();
    return false;
}

bool Mario::checkGroundOnSlope() {
    TVec3f normal;
    if (isAnimationRun("崖ふんばり")) {
        normal = -*getGravityVec();
    } else {
        normal = *_45C->getNormal(0);
    }
    if (MR::isNearZero(normal)) {
        return false;
    }
    TVec3f front;
    MR::vecKillElement(mFrontVec, normal, &front);
    if (MR::isNearZero(front)) {
        return false;
    }
    Triangle ground;
    bool hit;
    bool missingGround = false;
    mMovementStates._14 = true;
    f32 distance;
    if (mMovementStates.jumping && isRising()) {
        distance = 10.0f;
    } else if (_414) {
        distance = 20.0f;
    } else {
        distance = 60.0f;
    }
    if (mMovementStates._1) {
        f32 angle = calcAngleD(_368) - 30.0f;
        if (angle < 0.0f) {
            angle = 0.0f;
        }
        if (angle > 40.0f) {
            angle = 40.0f;
        }
        distance += 2.0f * angle;
    }
    if (getCurrentStatus() == MarioStatus_Slider) {
        distance = 100.0f;
    }
    s32 groundCount = 0;
    TVec3f hitPosition;
    TVec3f positionSum;
    positionSum.zero();
    TVec3f groundPosition;
    TVec3f origin(mPosition - *getGravityVec() * 30.0f);
    hit = MR::getFirstPolyOnLineBFast(origin, *getGravityVec() * distance, &hitPosition, &ground);
    if (hit) {
        if (calcAngleD(*ground.getNormal(0)) >= 80.0f) {
            hit = false;
        }
        f32 gravityDot = getGravityVec()->dot(*ground.getNormal(0));
        if (getCurrentStatus() != MarioStatus_Slider && gravityDot > -0.1908f) {
            hit = false;
        }
        if (hit) {
            positionSum += hitPosition;
            groundCount++;
            setGroundNorm(*ground.getNormal(0));
            *mGroundPolygon = ground;
            groundPosition = hitPosition;
            mGroundPos = groundPosition;
            recordLastGround();
        }
    }
    if (!groundCount) {
        missingGround = true;
    }
    if (mMovementStates._D) {
        mMovementStates._D = false;
        TVec3f offset(mGroundPos - mPosition);
        f32 vertical = MR::vecKillElement(offset, *getGravityVec(), &offset);
        addTrans(*getGravityVec() * vertical, "force Trans");
        return true;
    }
    if (mMovementStates.jumping && isRising()) {
        return false;
    }
    if (missingGround) {
        mMovementStates._14 = true;
        return false;
    }
    mMovementStates._14 = false;
    TVec3f down(-_368);
    TVec3f position(mGroundPos);
    if (_10._22) {
        _10._22 = false;
        _1C._C = true;
    } else if ((mMovementStates._23 || mTargetWalkSpeedIndex) && getPlayerMode() != PlayerMode_Teresa) {
        if (__fabsf((position - mPosition).dot(down)) < distance) {
            TVec3f offset(position - mPosition);
            f32 vertical = MR::vecKillElement(offset, down, &offset);
            if (!MR::isNearZero(vertical, 1.0f)) {
                mDrawStates._0 = false;
                TVec3f displacement(down * vertical);
                f32 ratio = MR::clamp(mWalkSpeed, 0.0f, 1.0f);
                if (!mMovementStates._23) {
                    displacement *= ratio;
                }
                bool notSlipping = !isSlipFloorCode(_960);
                if (!notSlipping && !mDrawStates._4 && !mPrevDrawStates._4) {
                    mVelocityAfter += displacement;
                }
            }
            return true;
        }
    }
    if (mVerticalSpeed < 5.0f) {
        return true;
    }
    if (mDrawStates._6) {
        return true;
    }
    if (groundCount) {
        return true;
    }
    return mDrawStates._0;
}
