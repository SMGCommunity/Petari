#include <revolution/types.h>

f32 JMAAcosRadian(f32) NO_INLINE;

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioShadow.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"

const TVec3f& MarioActor::getGravityVec() const {
    return *mMario->getGravityVec();
}

const TVec3f& MarioActor::getGravityVector() const {
    return *mMario->getGravityVec();
}

const TVec3f& MarioActor::getAirGravityVec() const {
    return mMario->getAirGravityVec();
}

void MarioActor::getGravityVector(TVec3f* pVec) const {
    pVec->set(mMario->getAirGravityVec());
}

f32 MarioActor::getGravityRatio() const {
    return mGravityRatio;
}

GravityInfo* MarioActor::getGravityInfo() const {
    return mGravityInfo;
}

u8 MarioActor::getGravityLevel() const {
    return MR::isLightGravity(*mGravityInfo);
}

bool MarioActor::checkBeeWallStick(TVec3f& rVec) {
    if (getMovementStates()._8 && mMario->checkWallCode("Fur", false) && mBeeWallWalk == 0 && !_9F2) {
        TVec3f vec20 = mMario->mHeadVec;

        mBeeWallWalk = 5;

        rVec = -mMario->getWallNorm();

        mPosition = mMario->getWallPos();

        mMario->stopJump();
        mMario->stopAnimation(nullptr);
        mMario->stopWalk();

        _240 = rVec;
        mMario->setGravityVec(rVec);
        mMario->setHeadVec(-rVec);
        mMario->setFrontVecKeepUp(vec20, 1UL);
        setBlendMtxTimer(2);

        _38C = 5;
        mMario->mMovementStates._38 = false;
        _214->_305 = true;

        return true;
    }

    return false;
}

bool MarioActor::checkBeeFloorStick(TVec3f& rVec) {
    if (mMario->getMovementStates()._1 && strcmp("Fur", MR::getWallCodeString(mMario->getGroundPolygon())) == 0 && mBeeWallWalk == 0) {
        mBeeWallWalk = 5;
        rVec = -mMario->_368;

        return true;
    }

    return false;
}

void MarioActor::syncJumpBeeStickMode() {
    if (mBeeWallWalk != 0 && selectQuickResetBeeWallGravity(mMario->_45C->mSensor->mHost->mName)) {
        mBeeWallWalk = 0;
        _9F2 = 30;
    }

    if (!(MR::diffAngleAbs(_360, getGravityVec()) < MR::pi() / 36.0f)) {
        return;
    }

    _33C = mPosition - mMario->mFrontVec * 2.0f;
    _354 = mPosition - _33C;
    _360 = _2A0 - _33C;
    MR::normalize(&_360);
}

void MarioActor::updateBeeModeGravity(TVec3f& rVec) {
    u8 alpha = mBeeWallWalk;

    if (_9F2 != 0) {
        _9F2--;
    }

    if (_9F2 != 0 || (!checkBeeCeilStick(rVec) && !checkBeeWallStick(rVec) && !checkBeeFloorStick(rVec))) {
        updateBeeStickMode(rVec);
    }

    if (mBeeWallWalk == 5) {
        MR::vecBlendSphere(_240, rVec, &rVec, 0.1f);
        return;
    }

    if (mBeeWallWalk != 0) {
        MR::vecBlendSphere(_240, rVec, &rVec, 0.3f);
        return;
    }

    if (alpha != 0) {
        _9F2 = 60;
    }
}

bool MarioActor::isInZeroGravitySpot() const {
    return MR::isNearZero(_24C);
}

void MarioActor::updateGravityVec(bool reset, bool usePosition) {
    Mtx rotation;
    TVec3f gravity;
    TVec3f positionGravity;
    TVec3f frontGravity;
    TVec3f specialGravity;
    bool resetGround = false;
    MR::calcGravityVectorOrZero(this, mPosition, &positionGravity, mGravityInfo, 0);
    if (!usePosition) {
        MR::calcGravityVectorOrZero(this, _2A0, &gravity, mGravityInfo, 0);
    } else {
        gravity = positionGravity;
    }
    {
        TVec3f frontOffset(mMario->mFrontVec);
        frontOffset.mult(100.0f);
        MR::calcGravityVectorOrZero(this, mPosition + frontOffset, &frontGravity, nullptr, 0);
    }
    if (MR::isNearZero(positionGravity - frontGravity)) {
        _370 = true;
    } else {
        _370 = false;
    }
    if (mMario->isUseFooSpecialGravity(mPosition, &specialGravity)) {
        gravity = specialGravity;
        positionGravity = specialGravity;
        _370 = false;
    }
    MR::normalizeOrZero(&gravity);
    MR::normalizeOrZero(&positionGravity);
    if (gravity.dot(positionGravity) < 0.9f) {
        f32 dot = mMario->getAirGravityVec().dot(gravity);
        if (mMario->getAirGravityVec().dot(positionGravity) > dot) {
            gravity = positionGravity;
        }
    }
    f32 angle = MR::diffAngleAbs(_24C, gravity);
    if (MR::isNearZero(_24C)) {
        angle = 0.0f;
    } else if (MR::isNearZero(gravity)) {
        angle = 0.0f;
    }
    if (!reset) {
        if (mMario->isPlayerModeBee()) {
            if (angle >= MR::pi() / 6.0f) {
                setBlendMtxTimer(8);
            }
        }
        if (_334) {
            gravity = _24C;
        } else if (angle >= MR::pi() * 0.4f) {
            if (!isJumping()) {
                mMario->_10._1A = true;
            } else {
                if (mMario->_430 == 11) {
                    mMario->_430 = 0;
                }
                changeAnimation("ショートジャンプ", nullptr);
                resetGround = true;
            }
            if (_F74) {
                resetGround = true;
            }
            _334 = 15;
            _30C = mMario->mHeadVec;
            if (!MR::isNearZero(gravity)) {
                if (angle >= MR::pi() * (2.0f / 3.0f) || !getMovementStates()._37) {
                    mMario->cutGravityElementFromJumpVec(false);
                    mMario->mJumpVec.mult(0.5f);
                } else {
                    const TVec3f& previousGravity = *mMario->getGravityVec();
                    f32 speed = MR::vecKillElement(mMario->mJumpVec, previousGravity, &mMario->mJumpVec);
                    mMario->mJumpVec.mult(0.75f);
                    mMario->mJumpVec += *mMario->getGravityVec() * speed * 0.75f;
                }
            }
            mMario->clear2DStick();
            mMario->_10.turning = true;
            _3AA = 15;
            if (!getMovementStates()._37) {
                mMario->_2E0 = -mMario->_2E0;
            }
        }
        if (MR::isNearZero(gravity)) {
            mMario->_10.turning = true;
        }
    } else {
        _334 = 0;
    }
    if (!MR::isNearZero(gravity) && !MR::isNearZero(_24C)) {
        TVec3f axis;
        TVec3f side;
        f32 cosine;
        if (MR::makeAxisAndCosignVecToVec(&axis, &cosine, _24C, gravity)) {
            PSMTXRotAxisRad(rotation, &axis, JMAAcosRadian(cosine));
            PSMTXMultVecSR(rotation, &_258, &_258);
            side.cross(_258, gravity);
            if (!MR::normalizeOrZero(&side)) {
                _258.cross(gravity, side);
            }
            MR::normalize(&_258);
        }
    } else if (!MR::isNearZero(gravity) && MR::isNearZero(_24C)) {
        calcBaseFrontVec(-gravity);
    } else if (MR::isNearZero(gravity) && MR::isNearZero(_24C)) {
        calcBaseFrontVec(mMario->_1FC);
    }
    _24C = gravity;
    if (!MR::isNearZero(gravity)) {
        MR::normalize(&gravity);
        _3A0 = 0;
    } else {
        if (!mMario->mMovementStates._1) {
            gravity = _240;
        } else {
            gravity = -mMario->_368;
        }
        _3A0++;
    }
    if (mPlayerMode == PlayerMode_Bee) {
        updateBeeModeGravity(gravity);
    } else {
        mBeeWallWalk = 0;
        _9F2 = 0;
    }
    mMario->setGravityVec(gravity);
    _240 = gravity;
    if (resetGround) {
        mMario->setGroundNorm(-gravity);
    }
    calcCenterPos();
    f32 height = 70.0f;
    TVec3f up;
    if (getDrawStates()._F) {
        MR::vecBlendSphere(mMario->mHeadVec, mMario->_380, &up, 0.5f * _984);
    } else {
        MR::vecBlendSphere(mMario->mHeadVec, mMario->_368, &up, _984);
    }
    TVec3f oldOffset(_2C4);
    if (mMario->mMovementStates._A) {
        height = 50.0f;
    }
    if (mMario->mMovementStates._1) {
        _2C4 = up * height;
    } else if (mMario->_10._23 || _934) {
        TVec3f offset = -_240 * 70.0f;
        if (_334 || mMario->calcPolygonAngleD(mMario->mGroundPolygon) >= 59.0f) {
            MR::vecBlend(_2C4, offset, &_2C4, 0.1f);
        } else {
            MR::vecBlend(_2C4, offset, &_2C4, 0.5f);
        }
    } else {
        TVec3f offset = -_240 * 70.0f;
        MR::vecBlend(_2C4, offset, &_2C4, 0.1f);
    }
    if (_3AA) {
        _3AA--;
        f32 rate = static_cast< f32 >(_3AA) / 15.0f;
        _2C4 = _2C4 * (1.0f - rate) + oldOffset * rate;
        mMario->_10.turning = true;
    }
    if (_334) {
        if (mMario->mVerticalSpeed > 160.0f) {
            TVec3f head = mMario->mHeadVec * 80.0f;
            TVec3f oldHead = _30C * 80.0f;
            TVec3f displacement = oldHead - head;
            f32 distance = MR::vecKillElement(displacement, _240, &displacement);
            mMario->push2(_240 * distance);
            _30C = mMario->mHeadVec;
        }
        _334--;
    }
}

bool MarioActor::checkBeeCeilStick(TVec3f& rVec) {
    if ((mMario->isCeiling() || getDrawStates()._15) && mBeeWallWalk == 0 && _9F2 == 0) {
        bool out = false;
        if (getDrawStates()._15) {
            out = true;
        } else {
            const char* wallCodeString = MR::getWallCodeString(mMario->_4C8);
            if (wallCodeString != nullptr && strcmp(wallCodeString, "Fur") == 0) {
                out = true;
            }
        }

        if (out) {
            Triangle triangle = Triangle();
            TVec3f vec;
            if (MR::getFirstPolyOnLineToMap(&vec, &triangle, mPosition, (-_240).multiplyOperatorInline(200.0f))) {
                entryWallWalkMode(vec, *(triangle.getNormal(0)));
            }
        }
    }

    return false;
}

void MarioActor::updateBeeStickMode(TVec3f& gravity) {
    if (mBeeWallWalk != 0) {
        bool detach = false;
        bool fur = getDrawStates()._19;
        if (!fur) {
            const char* wallCode = MR::getWallCodeString(mMario->_45C);
            if (wallCode != nullptr && strcmp(wallCode, "Fur") == 0) {
                fur = true;
            }
        }
        if (fur && _9F2 == 0) {
            f32 radius = mConst->getTable()->mBeeWallWalkCancelRadius;
            if (MR::getAreaObj("BeeWallShortDistArea", mPosition)) {
                radius = mConst->getTable()->mBeeWallWalkCancelRadiusShort;
            }
            if (mMario->mVerticalSpeed > radius) {
                detach = true;
            } else if (isJumping() && mMario->checkWallCode("Normal", false)) {
                detach = true;
            } else {
                mBeeWallWalk = 5;
            }
            if (isJumping() && isRequestRush()) {
                mBeeWallWalk = 0;
            }
        } else if (isJumping()) {
            if (mMario->mVerticalSpeed < 100.0f) {
                mBeeWallWalk--;
            }
            if (mMario->checkWallCode("Normal", false)) {
                detach = true;
            }
            f32 radius = mConst->getTable()->mBeeWallWalkCancelRadius;
            if (MR::getAreaObj("BeeWallShortDistArea", mPosition)) {
                radius = mConst->getTable()->mBeeWallWalkCancelRadiusShort;
            }
            if (mMario->mVerticalSpeed > radius) {
                detach = true;
            }
        } else {
            if (mMario->mMovementStates._B) {
                mMario->mMovementStates._B = false;
            }
            detach = true;
        }
        if (detach) {
            if (mBeeWallWalk != 0) {
                mBeeWallWalk--;
            }
        }
        if (mBeeWallWalk == 0) {
            mMario->stopWalk();
            mMario->tryJump();
            mMario->_408 = mConst->getTable()->mBeeGravityPowerTime;
            mMario->_3BC = mConst->getTable()->mBeeAirWalkInhibitTime - 5;
            mMario->push(*mMario->_45C->getNormal(0) * 100.0f);
            mMario->cutVecElementFromJumpVec(_24C);
            return;
        }
        gravity = -*mMario->_45C->getNormal(0);
    }
}
