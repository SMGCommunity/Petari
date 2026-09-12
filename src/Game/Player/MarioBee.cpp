#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioShadow.hpp"
#include "Game/Player/MarioState.hpp"
#include "Game/Util/MathUtil.hpp"

void Mario::beeMarioOnGround() {
    if (getPlayerMode() == PlayerMode_Bee && !mDrawStates._C && mMovementStates._1 && !mMovementStates._23 && !isStatusActive(MarioStatus_Slider)) {
        getPlayer()->incAirWalkTimer();
        getPlayer()->incAirWalkTimer();
        getPlayer()->incAirWalkTimer();
        getPlayer()->incAirWalkTimer();
    }
}

void MarioActor::entryWallWalkMode(const TVec3f& position, const TVec3f& normal) {
    if (mBeeWallWalk) {
        return;
    }
    if (!_9F2) {
        mBeeWallWalk = 5;
        TVec3f gravity(-normal);
        _240 = gravity;
        mPosition = position;
        mMario->setTrans(position, nullptr);
        mMario->stopJump();
        mMario->stopAnimation(nullptr, static_cast< const char* >(nullptr));
        mMario->stopWalk();
        TVec3f front(mMario->mHeadVec);
        mMario->setGravityVec(gravity);
        mMario->setHeadVec(-gravity);
        mMario->setFrontVecKeepUp(front, 1UL);
        setBlendMtxTimer(2);
        _38C = 5;
        mMario->mMovementStates._38 = false;
        _214->_305 = true;
    }
}

bool Mario::beeMarioOnAir() {
    if (_774) {
        _774--;
        if (!_774) {
            mMovementStates._2F = false;
        }
    }

    if (getPlayerMode() == PlayerMode_Bee) {
        if (_3BC == 1 && _402) {
            _402--;
        }
        if (isAnimationRun("ハチ壁ジャンプ") && isAnimationTerminate(nullptr)) {
            if (checkLvlA()) {
                changeAnimation("ハチ飛行中", "落下");
            } else {
                changeAnimation("ハチ飛行中無入力", "落下");
            }
            changeAnimationInterpoleFrame(30);
        }
    }

    if (getPlayerMode() == PlayerMode_Bee) {
        TVec3f horizontal;
        if (MR::vecKillElement(mJumpVec, getAirGravityVec(), &horizontal) > -5.0f || _76C) {
            if (!_774) {
                f32 verticalSpeed = cutGravityElementFromJumpVec(true);
                f32 speed = mJumpVec.length();
                MR::normalizeOrZero(&mJumpVec);
                if (!MR::vecBlendSphere(mJumpVec, mFrontVec, &mJumpVec, mActor->getConst().getTable()->mBeeSpeedRotateRatio)) {
                    Mtx rotation;
                    PSMTXRotAxisRad(rotation, &mHeadVec, 0.1f);
                    PSMTXMultVecSR(rotation, &mJumpVec, &mJumpVec);
                }
                mJumpVec.setLength(speed);
                mJumpVec += getAirGravityVec() * verticalSpeed;
            }

            if (!mMovementStates._11 && _402) {
                _406 = 16;
                if (_76C < 30) {
                    _76C = 30;
                    _770 = 0.0f;
                }
                mMovementStates._12 = true;
                _4B0 = mPosition;
                mMovementStates._11 = true;
            }

            s16 inhibitTime = mActor->getConst().getTable()->mBeeAirWalkInhibitTime;
            s16 powerTime = mActor->getConst().getTable()->mBeeGravityPowerTime;
            TVec3f horizontalVelocity;
            MR::vecKillElement(mJumpVec, getAirGravityVec(), &horizontalVelocity);
            if (horizontalVelocity.length() < 5.0f) {
                inhibitTime = mActor->getConst().getTable()->mBeeAirWalkInhibitTimeV;
                powerTime = mActor->getConst().getTable()->mBeeGravityPowerTimeV;
            }

            if (checkLvlA() && _402 && _3BC > inhibitTime) {
                playSound("ハチ飛行中");
                if (!MR::isNearZero(mStickPos.z)) {
                    setFrontVecKeepUp(getWorldPadDir(), mActor->getConst().getTable()->mBeeAirWalkTurnSpd);
                }
                u16 previousTime = _402;
                if (_402) {
                    if (!mMovementStates._F && _402 > mActor->getConst().getTable()->mAirWalkTime) {
                        _402 = mActor->getConst().getTable()->mAirWalkTime;
                    }
                    _402--;
                }
                if (!_402) {
                    if (previousTime) {
                        playSound("ハチ体力切れ");
                    }
                    mMovementStates._11 = false;
                    stopAnimation("ハチ飛行中", static_cast< const char* >(nullptr));
                } else {
                    if (!mMovementStates._F) {
                        if (!isAnimationRun("ハチ壁ジャンプ") && !isAnimationRun("ハチスピン空中")) {
                            changeAnimation("ハチ飛行中", "落下");
                        }
                        cancelSquatMode();
                        playSound("空中ふんばり");
                        if (_402 < mActor->getConst().getTable()->mAirWalkTime / 2) {
                            getAnimator()->setSpeed(1.5f);
                        }
                        if (_430 == 4) {
                            setFrontVecKeepUp(-_220);
                            _430 = 0;
                        }
                        if (_430 == 5) {
                            _430 = 0;
                        }
                    }

                    s16 time = static_cast< s16 >(_408);
                    if (time > powerTime) {
                        time = powerTime;
                    }
                    f32 ratio = static_cast< f32 >(time) / powerTime;
                    f32 reduction = 0.9f * (ratio * ratio);
                    f32 acceleration = 15.0f * (MR::getRandom() - reduction);
                    acceleration =
                        acceleration * mActor->getConst().getTable()->mBeeFlyRandomFactor - mActor->getConst().getTable()->mBeeFlyConstantFactor;
                    f32 verticalSpeed = cutGravityElementFromJumpVec(true);
                    TVec3f velocity(mJumpVec);
                    f32 factor = 1.0f;
                    if (!MR::isNearZero(mStickPos.z)) {
                        velocity.dot(getWorldPadDir());
                    }
                    if (factor < 0.0f) {
                        factor *= mActor->getConst().getTable()->mBeeUpAccelRatio;
                    }
                    _770 += factor * (acceleration * mActor->getConst().getTable()->mBeeAccelRatio);
                    if (_770 > 0.0f) {
                        _770 *= mActor->getConst().getTable()->mBeeUpDownKiller;
                    }
                    if (!_774) {
                        if (!getPlayer()->_1C._5) {
                            if (_770 < -mActor->getConst().getTable()->mBeeUpSpeedMax) {
                                _770 = -mActor->getConst().getTable()->mBeeUpSpeedMax;
                            }
                        } else if (_770 < -0.5f) {
                            _770 = -0.5f;
                        }
                    }
                    addVelocity(*getGravityVec(), _770);
                    if (!getPlayer()->_1C._5 && verticalSpeed > 0.0f) {
                        verticalSpeed *= mActor->getConst().getTable()->mBeePushRiseGravityEraser;
                    }
                    mJumpVec += getAirGravityVec() * verticalSpeed;
                    _408++;
                    if (_408 > 120) {
                        _408 = 120;
                    }
                    _4B0 = mPosition;
                    return true;
                }
            } else {
                if (!isAnimationRun("ハチジャンプ") && !isAnimationRun("ハチ壁ジャンプ")) {
                    stopAnimation("ハチ飛行中", static_cast< const char* >(nullptr));
                    if (!isAnimationRun(nullptr) || isAnimationTerminate(nullptr)) {
                        changeAnimation("ハチ飛行中無入力", static_cast< const char* >(nullptr));
                    }
                }
                if (_408) {
                    _408--;
                }
                if (_770 < mActor->getConst().getTable()->mBeeFreeDropMaxSpd) {
                    _770 += mActor->getConst().getTable()->mBeeFreeDropAcc;
                }
                f32 factor = 1.0f;
                addVelocity(*getGravityVec(), _770 * factor);
                if (!MR::isNearZero(mStickPos.z)) {
                    setFrontVecKeepUp(getWorldPadDir(), mActor->getConst().getTable()->mBeeAirWalkTurnSpd);
                }
            }
        }
    }
    return false;
}
