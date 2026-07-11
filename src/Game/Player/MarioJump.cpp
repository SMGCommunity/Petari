#include "Game/Enemy/KariKariDirector.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioRabbit.hpp"
#include "Game/Player/MarioSwim.hpp"
#include "Game/Player/MarioWall.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    f32 cDropFrontSpeed = 2.0f;
};  // namespace

bool Mario::isRising() const {
    if (getPlayerMode() == 4 || getPlayerMode() == 6) {
        if (_16C.dot(*getGravityVec()) < 0.0f) {
            return true;
        }

        return false;
    }

    return mJumpVec.dot(*getGravityVec()) < 0.0f;
}

void Mario::checkWallRiseAndSlipFront() {
    mMovementStates._6 = false;
    if (mMovementStates._8) {
        mMovementStates._9 = false;
        _278 = 0.0f;
        return;
    }

    mMovementStates._9 = true;
}

void Mario::tryJump() {
    bool isMudFloorJump = false;
    bool useStickJumpAnim = false;

    TVec3f stickTotal(_190 + _19C);

    if (mMovementStates.jumping) {
        return;
    }

    if (_735 != 0) {
        if (checkCurrentFloorCodeSevere(0x19)) {
            if (_735 > 0x20) {
                _735 -= 0x20;
            }

            if (_735 < 0x40) {
                _735 = 0x40;
            }

            mMovementStates._38 = false;
            return;
        }

        if (checkCurrentFloorCodeSevere(0x1F)) {
            return;
        }

        if (_735 > 100) {
            if (_735 > 0xC8) {
                _735 = 0x80;
                changeAnimation("埋まりジャンプA", static_cast< const char* >(nullptr));
            } else {
                _735 = 0x20;
                changeAnimation("埋まりジャンプB", static_cast< const char* >(nullptr));
            }

            if (checkCurrentFloorCodeSevere(0x12)) {
                playSound("毒沼脱出", -1);
            } else {
                playSound("砂脱出", -1);
            }

            playSound("声砂脱出", -1);
            return;
        }
    }

    playEffect("共通跳躍");
    stopEffectForce("スピンリング");

    const bool isSquat = checkSquat(true);
    mMovementStates._21 = true;
    mMovementStates._29 = false;
    mMovementStates._38 = false;
    _10._23 = true;
    mMovementStates._B = false;
    _42A = 0;

    if (_3CE > mActor->getConst().getTable()->mJumpConnectTime || !mMovementStates._5) {
        _430 = 0;
    } else {
        _430++;
    }

    if (_430 > 2) {
        _430 = 0;
    }

    if (_430 == 2 && _278 < mActor->getConst().getTable()->mJumpConnectSpeed) {
        _430 = 0;
    }

    if (getPlayerMode() == 4 || getPlayerMode() == 6) {
        _430 = 0;
    }

    if (_1C._B) {
        _430 = 2;
        MR::start2PJumpAssistJustSound();
    } else if (_1C._A) {
        MR::start2PJumpAssistSound();
    }

    if (isPlayerModeInvincible() && _278 > 1.0f) {
        _430 = 1;
        _42A = 1;
    }

    if (!MR::isNearZero(stickTotal) && _735 != 0) {
        _430 = 0;
        mMovementStates._4 = false;
        _3D2 = 0;
    }

    forceStopTornado();

    u32 floorCode = getFloorCode();
    if (floorCode != 0x20 && getPlayerMode() != 4 && (((mMovementStates._4) != 0) || (_3D2 != 0 && static_cast< u8 >(checkStickFrontBack()) == 2))) {
        if ((mActor->getConst().getTable()->mTurnSlipTime - _3D0) >= mActor->getConst().getTable()->mTurnJumpInhibitTime) {
            tryTurnJump();
            return;
        }

        _3D0 = 0;
        mMovementStates._4 = false;
    }

    checkWallRiseAndSlipFront();

    const f32 floorAngle = calcPolygonAngleD(mGroundPolygon);
    useStickJumpAnim = false;
    if (!MR::isNearZero(_8F8) && (mMovementStates._23)) {
        useStickJumpAnim = true;
    }

    if (floorAngle < 15.0f) {
        mMovementStates._23 = false;
        _8F8.zero();
    }

    if (getPlayerMode() != 6 && !_10._1A) {
        if (mDrawStates._C && isSlipPolygon(_45C)) {
            const bool dotFront = mFrontVec.dot(_368) < 0.0f;
            const bool dotPad = mWorldPadDir.dot(_368) <= 0.0f;
            const bool diff = MR::diffAngleAbsHorizontal(_16C, _368, getAirGravityVec()) > 1.5707964f;
            const bool movementstate = mMovementStates._A;

            if (dotFront || dotPad || diff || movementstate) {
                if (dotFront && !dotPad) {
                    mMovementStates._2B = true;
                    _430 = 0;
                } else {
                    _278 *= 0.5f;
                    if (getPlayerMode() != 4) {
                        _402 = 0;
                        _430 = 3;
                        mMovementStates._2B = true;
                    }

                    useStickJumpAnim = false;
                }
            } else {
                mMovementStates._23 = false;
            }
        }

        if (checkCurrentFloorCodeSevere(0x11)) {
            isMudFloorJump = true;
            _430 = 3;
            _278 *= 0.5f;
        }
    }

    floorCode = getFloorCode();
    if (floorCode != 0x20 && getPlayerMode() != 4 && _430 != 3) {
        if (isSquat && _278 > 0.3f && ((mMovementStates._23) == 0)) {
            trySquatJump();
            return;
        }

        if ((mMovementStates._A) && ((mMovementStates._23) == 0)) {
            tryBackJump();
            return;
        }
    }

    if (mDrawStates._5) {
        mJumpVec = _334 * _278 * mActor->getConst().getTable()->mJumpFrontSpeed;
    } else {
        mJumpVec = mFrontVec * _278 * mActor->getConst().getTable()->mJumpFrontSpeed;
    }

    _10._17 = false;
    if (mDrawStates._B) {
        recordJumpEnforceMove();

        f32 enforce;
        if (getStickP() == 0.0f) {
            mJumpVec = _184;
            mJumpVec.zero();
            enforce = 1.0f;
        } else {
            const f32 angle = MR::diffAngleAbsHorizontal(mFrontVec, _184, getAirGravityVec());
            enforce = 1.0f - (angle / 1.5707964f);
            if (enforce < 0.0f) {
                enforce = 0.0f;
            }

            const f32 speed = mJumpVec.length();
            const f32 speedBonus = MR::clamp(1.0f - ((speed - 4.0f) / 10.0f), 0.0f, 1.0f);
            enforce += speedBonus;
            enforce = MR::clamp(enforce, 0.0f, 1.0f);
        }

        doEnforceJump(enforce);
    }

    if (!MR::isNearZero(stickTotal)) {
        addVelocity(-stickTotal);

        MR::vecKillElement(stickTotal, mActor->_240, &stickTotal);
        cutGravityElementFromJumpVec(true);

        if (stickTotal.dot(mJumpVec) > 0.0f) {
            mJumpVec += stickTotal * 1.0f;
        } else {
            mJumpVec += stickTotal * 0.5f;
        }
    }

    if (useStickJumpAnim) {
        mJumpVec += _16C;
        cutGravityElementFromJumpVec(true);
    }

    _340 = 1.0f - _278;
    if (_340 < 0.0f) {
        _340 = 0.0f;
    }

    f32 jumpRatio = 1.0f;
    if (getFloorCode() == 0x20) {
        jumpRatio = mActor->getConst().getTable()->mMudFloorJumpWeakRatio;
    }
    if (_1C._B) {
        jumpRatio = 0.9f;
    }

    mJumpVec += -mActor->_240 * mActor->getConst().getTable()->mJumpHeight[_430] * jumpRatio;

    mMovementStates._E = false;
    mMovementStates._1 = false;
    if (getPlayerMode() != 4 && getFloorCode() == 0x20) {
        _430 = 3;
        isMudFloorJump = true;
        mMovementStates._2B = true;
    }

    if (_10._1A) {
        _430 = 0;
    }

    procJump(true);
    mMovementStates.jumping = true;

    if (_42A == 1 && _430 == 1) {
        _430 = 2;
    }

    switch (_430) {
    case 0:
        if ((mMovementStates._17) != 0) {
            changeAnimation("壁上昇", "落下");
        } else {
            changeAnimation("ジャンプ", "落下");
        }

        if (mActor->mAlphaEnable) {
            playSound("ハチ壁ジャンプ", -1);
        } else {
            playSound("小ジャンプ", -1);
        }

        if (getPlayerMode() != 6) {
            playSound("声小ジャンプ", -1);
            playSound("ジャンプ踏切", -1);
        }

        if (useStickJumpAnim) {
            changeAnimation("尻滑りジャンプ", static_cast< const char* >(nullptr));
            _428 = 0x1E;
        }

        if (getPlayerMode() == 4) {
            changeAnimation("ハチジャンプ", static_cast< const char* >(nullptr));
            mActor->syncJumpBeeStickMode();
        }

        if (_10._1A) {
            changeAnimation("ショートジャンプ", "落下");
        }
        break;
    case 1:
        changeAnimationNonStop("ジャンプB");
        changeAnimation(static_cast< const char* >(nullptr), "落下");
        playSound("中ジャンプ", -1);
        playSound("声中ジャンプ", -1);
        playSound("ジャンプ踏切", -1);
        break;
    case 2:
        playSound("大ジャンプ", -1);
        playSound("声大ジャンプ", -1);
        playSound("ジャンプ踏切", -1);
        playEffect("共通ハイジャンプ");

        if (_1C._B) {
            changeAnimation("スカイラブジャンプ", "落下");
        } else if (getPlayerMode() == 1 && _278 > 1.0f) {
            changeAnimation("ダッシュジャンプ", "落下");
        } else {
            changeAnimation("ジャンプC", "落下");
        }
        break;
    case 3:
        if (isMudFloorJump) {
            changeAnimation("埋まり脱出ジャンプ", "落下");
        } else {
            changeAnimation("腹ばいジャンプ", "落下");
            playSound("声小ジャンプ", -1);
            playSound("ジャンプ踏切", -1);
        }
        break;
    default:
        break;
    }

    if (_42A == 1 && _430 == 2) {
        _430 = 1;
    }

    const f32 waterDist = mSwim->checkUnderWaterFull(mFrontVec);
    if (waterDist > 500.0f && mStickPos.z == 0.0f) {
        mMovementStates._E = true;
        changeAnimationNonStop("飛び込みジャンプ");
        playSound("声高飛び込み", -1);
        playSound("ジャンプ踏切", -1);

        if (mStickPos.z < 0.1f) {
            mJumpVec += mFrontVec * 5.0f;
        }
    }

    mMovementStates._5 = true;
    return;
}

void Mario::tryTurnJump() {
    _430 = 0x4;

    if (_3D2 != 0) {
        TVec3f horizontal;
        MR::vecKillElement(_3E4, getAirGravityVec(), &horizontal);
        MR::normalizeOrZero(&horizontal);

        const f32 turnJumpFrontSpeed = mActor->getConst().getTable()->mTurnJumpFrontSpeed;
        mJumpVec = -horizontal * turnJumpFrontSpeed;

        setFrontVecKeepUp(-_3E4);
        _220 = _3E4;
    } else {
        TVec3f horizontal;
        MR::vecKillElement(_220, getAirGravityVec(), &horizontal);
        MR::normalizeOrZero(&horizontal);

        const f32 turnJumpFrontSpeed = mActor->getConst().getTable()->mTurnJumpFrontSpeed;
        mJumpVec = -horizontal * turnJumpFrontSpeed;
    }

    const f32 jumpHeight = mActor->getConst().getTable()->mJumpHeight[_430];
    mJumpVec += -mActor->_240 * jumpHeight;

    _10._17 = false;
    if (mDrawStates._B) {
        recordJumpEnforceMove();
        doEnforceJump(1.0f);
    }

    mMovementStates._E = true;
    procJump(true);
    changeAnimation("ターンジャンプ", "落下");
    playSound("後ジャンプ", -1);
    playSound("声後ジャンプ", -1);
    playSound("ジャンプ踏切", -1);
    playEffect("共通ハイジャンプ");

    _3D0 = 0;
    mMovementStates.jumping = true;
    mMovementStates._1 = false;
    mMovementStates._4 = false;
    mMovementStates._5 = false;
    _278 = 0.0f;
    _10._A = false;
    return;
}

void Mario::trySquatJump() {
    mMovementStates._21 = false;
    _430 = 0x5;

    mJumpVec = getAirFrontVec() * mActor->getConst().getTable()->mSquatJumpFrontSpeed;

    TVec3f stickVec(_190 + _19C);
    if (!MR::isNearZero(stickVec)) {
        addVelocity(-stickVec);

        MR::vecKillElement(stickVec, mActor->_240, &stickVec);
        cutGravityElementFromJumpVec(true);

        mJumpVec += stickVec * 1.0f;
    }

    const f32 jumpHeight = mActor->getConst().getTable()->mJumpHeight[_430];
    mJumpVec += -mActor->_240 * jumpHeight;

    _10._17 = false;
    if (mDrawStates._B) {
        recordJumpEnforceMove();

        TVec3f moveKeep;
        f32 gravityMove = MR::vecKillElement(_184, getAirGravityVec(), &moveKeep);

        f32 enforceScale = 1.0f - (MR::diffAngleAbs(mFrontVec, moveKeep) / 1.5707964f);
        if (enforceScale < 0.0f) {
            enforceScale = 0.0f;
        }

        const f32 jumpMag = mJumpVec.length();
        const f32 jumpScale = MR::clamp(1.0f - ((jumpMag - 4.0f) / 10.0f), 0.0f, 1.0f);
        enforceScale += jumpScale;
        enforceScale = MR::clamp(enforceScale, 0.0f, 1.0f);

        if (mJumpVec.dot(moveKeep) < 0.0f) {
            mDrawStates._1B = true;
            initActiveJumpVec();
        }

        mVelocity -= moveKeep;

        mJumpVec += moveKeep * enforceScale;

        if (gravityMove < 0.0f) {
            mJumpVec += getAirGravityVec() * gravityMove;
        }

        invalidateRelativePosition();
    }

    mMovementStates._E = true;
    mMovementStates._1 = false;
    procJump(true);
    changeAnimationNonStop("幅とび");
    changeAnimation(static_cast< const char* >(nullptr), "落下");
    playSound("幅ジャンプ", -1);
    playSound("声幅ジャンプ", -1);
    playSound("ジャンプ踏切", -1);

    mMovementStates.jumping = true;
    _3D0 = 0;
    mMovementStates._5 = false;
    mMovementStates._4 = false;
    return;
}

void Mario::tryBackJump() {
    mMovementStates._21 = true;
    mMovementStates._A = false;
    _430 = 0x6;

    const f32 backSpeed = mActor->getConst().getTable()->mSquatJumpBackSpeed;
    mJumpVec = -getAirFrontVec() * backSpeed;

    const f32 jumpHeight = mActor->getConst().getTable()->mJumpHeight[_430];
    mJumpVec += -mActor->_240 * jumpHeight;

    _10._17 = false;
    if (mDrawStates._B) {
        recordJumpEnforceMove();
        doEnforceJump(1.0f);
    }

    mMovementStates._E = true;
    procJump(true);
    changeAnimation("しゃがみジャンプ", "落下");
    playSound("後ジャンプ", -1);
    playSound("声後ジャンプ", -1);
    playSound("ジャンプ踏切", -1);
    playEffect("共通ハイジャンプ");

    if (mSwim->checkUnderWaterFull(-mFrontVec) > 500.0f && mStickPos.z == 0.0f) {
        changeAnimationNonStop("後方飛び込みジャンプ");
        playSound("声高飛び込み", -1);
        playSound("ジャンプ踏切", -1);
    }

    _278 = 0.0f;
    mMovementStates.jumping = true;
    _3D0 = 0;
    mMovementStates._1 = false;
    mMovementStates._4 = false;
    mMovementStates._5 = false;
    return;
}

void Mario::tryTornadoJump() {
    _430 = 0x8;
    _42A = 0;

    const f32 frontSpeed = _278;
    const f32 tornadoSpeed = mActor->getConst().getTable()->mJumpTornadoSpeed;

    mJumpVec = mFrontVec * frontSpeed * tornadoSpeed;

    const f32 jumpHeight = mActor->getConst().getTable()->mJumpHeight[_430];
    mJumpVec += -mActor->_240 * jumpHeight;

    mMovementStates._E = false;
    mMovementStates._F = true;
    procJump(true);
    changeAnimation("ジャンプ", "落下");
    playSound("声スピン", -1);
    playSound("トルネードジャンプ", -1);

    mMovementStates.jumping = true;
    _3D0 = 0;
    mMovementStates._4 = false;
    mMovementStates._5 = false;
    mMovementStates._1D = false;
    _278 = 0.0f;

    _402 = mActor->getConst().getTable()->mAirWalkTimeTornado;
    _424 = mActor->getConst().getTable()->mTornadoZeroGravityTimer;

    const f32 boosterPower = mActor->getConst().getTable()->mTornadoBoostPower;
    setRocketBooster(-(*getGravityVec()) * boosterPower, mActor->getConst().getTable()->mTornadoBoostAttn,
                     mActor->getConst().getTable()->mTornadoBoostTimer);
}

void Mario::startTornadoCentering(HitSensor* pSensor) {
    pushTask(reinterpret_cast< Task >(&Mario::taskOnTornadoCentering), 0x400);
    _A38 = reinterpret_cast< u32 >(pSensor);
    _A34 = 0x1E;
}

bool Mario::taskOnTornadoCentering(u32 a1) {
    if (_A34 != 0) {
        TVec3f sensorOffset(reinterpret_cast< HitSensor* >(_A38)->mHost->mPosition - mPosition);
        MR::vecKillElement(sensorOffset, *getGravityVec(), &sensorOffset);

        f32 mag = sensorOffset.length();
        if (mag < 10.0f) {
            mag = 10.0f;
        }

        f32 ratio = 10.0f / mag;
        if (ratio < 0.1f) {
            ratio = 0.1f;
        }

        addTrans(sensorOffset * ratio, nullptr);

        _A34--;

        if (_A34 == 0) {
            return false;
        }

        return true;
    }

    return false;
}

void Mario::trySpinJump(u8 a1) {
    if (mMovementStates._B || _430 == 0x8 || isStatusActive(0x17) || getPlayerMode() == 4 || getPlayerMode() == 6) {
        return;
    }

    if (getPlayerMode() == 7 && mVerticalSpeed > 100.0f) {
        tryStartFoo();
        return;
    }

    if (isAnimationRun("水泳スピン移動") || isAnimationRun("水泳スピンジャンプ")) {
        return;
    }

    _430 = 0x8;
    _42A = a1 + 1;

    const f32 frontSpeed = _278;
    const f32 tornadoSpeed = mActor->getConst().getTable()->mJumpTornadoSpeed;
    mJumpVec = mFrontVec * frontSpeed * tornadoSpeed;

    const f32 spinJumpHeight = mActor->getConst().getTable()->mSpinJumpHeight;
    mJumpVec += -mActor->_240 * spinJumpHeight;

    mMovementStates._E = false;
    _402 = mActor->getConst().getTable()->mAirWalkTimeSpin;
    procJump(true);

    if (!mMovementStates._B) {
        stopAnimationUpper(nullptr, nullptr);
        changeAnimation("空中ひねり", "落下");
        playSound("声スピン", -1);
        playSound("スピンジャンプ", -1);
        startPadVib(2);

        mMovementStates.jumping = true;
        mMovementStates._4 = false;
        mMovementStates._5 = false;
        mMovementStates._1D = false;
        _10._19 = true;
        _3D0 = 0;
        _278 = 0.0f;
        _10._1 = true;

        if (isStatusActive(0x17)) {
            mRabbit->hop();
        }
    }
}

void Mario::tryForceJumpDelay(const TVec3f& rVec) {
    mMovementStates._2D = true;
    _304 = rVec;
}

void Mario::tryFreeJumpDelay(const TVec3f& rVec) {
    mMovementStates._2E = true;
    _304 = rVec;
}

void Mario::tryForceJump(const TVec3f& rVec, bool a2) {
    if (rVec.dot(*getGravityVec()) > 0.0f) {
        mMovementStates._20 = true;
    }

    mMovementStates.jumping = true;
    mMovementStates._1 = false;
    mMovementStates._5 = false;
    mMovementStates._E = a2;
    mMovementStates._B = false;
    mJumpVec = rVec;

    if (getPlayerMode() == 4 || getPlayerMode() == 6) {
        _16C = mJumpVec;
    }

    resetTornado();

    _3BC = 0;
    _3CA = 0;
    _3CC = 0;
    mMovementStates._13 = true;
    mMovementStates._11 = false;
    _10._8 = false;
    _3BE = 0;
    _344 = mSideVec;
    _4B0 = mPosition;
    _42C = 0;
    _76C = 0;
    procJump(false);
    changeAnimation(static_cast< const char* >(nullptr), "埋まりジャンプA");
    mRabbit->forceJump();
    return;
}

void Mario::tryForceFreeJump(const TVec3f& rVec) {
    mMovementStates._6 = false;
    _430 = 0xC;
    mMovementStates._2A = true;
    mMovementStates._9 = false;

    tryForceJump(rVec, true);
    _3BC = 0;
    mMovementStates._11 = false;
    _402 = mActor->getConst().getTable()->mAirWalkTime;

    if (getPlayerMode() == 4) {
        playSound("ジャンプ踏切", -1);
    }

    _76C = 0;
    return;
}

void Mario::tryForcePowerJump(const TVec3f& rVec, bool a2) {
    mMovementStates._6 = false;
    _430 = 0xD;
    mMovementStates._2A = true;
    mMovementStates._9 = false;
    const bool _2FPrev = mMovementStates._2F;
    mMovementStates._2F = true;
    tryForceJump(rVec, true);
    mMovementStates._2F = _2FPrev;
    _3BC = 0;
    mMovementStates._11 = false;

    if (!a2) {
        _402 = mActor->getConst().getTable()->mAirWalkTime;

        if (getPlayerMode() == 4) {
            playSound("ハチ体力完全回復", -1);
        }
    }

    _76C = 0;
    return;
}

void Mario::tryFreeJump(const TVec3f& rVec, bool a2) {
    _430 = 0;

    if (rVec.dot(*getGravityVec()) > 0.0f) {
        mMovementStates._20 = true;
    }

    mMovementStates.jumping = true;
    mMovementStates._1 = false;
    mMovementStates._5 = false;
    mMovementStates._E = a2;
    mMovementStates._B = false;
    mJumpVec = rVec;
    resetTornado();

    _3BC = 0;
    _3CA = 0;
    _3CC = 0;
    mMovementStates._13 = true;
    mMovementStates._11 = false;
    _3BE = 0;
    _76C = 0;
    _344 = mSideVec;
    procJump(false);
    changeAnimation(static_cast< const char* >(nullptr), "落下");
    startPadVib(2);
    mRabbit->forceJump();
    return;
}

void Mario::tryWallJump(const TVec3f& rVec, bool a2) {
    _430 = 0x7;

    mMovementStates.jumping = true;
    mMovementStates._5 = false;
    mMovementStates._E = true;
    mMovementStates._B = false;
    mMovementStates._6 = false;
    mMovementStates._28 = false;
    _24 &= 0xFF7FFFFF;
    mMovementStates._29 = false;

    if (a2) {
        mMovementStates._9 = true;
    } else {
        mMovementStates._9 = false;
    }

    mJumpVec = rVec;
    _4B0 = mPosition;
    _3BC = 0;
    mMovementStates._13 = true;
    mMovementStates._11 = false;
    _3CA = 0;
    _3CC = 0;
    _3BE = 0;
    mMovementStates._1D = true;

    if (getPlayerMode() == 5) {
        changeAnimationNonStop("ホッパー壁ジャンプ");
        playSound("ホッパージャンプ", -1);
    } else {
        changeAnimation("壁ジャンプ", "落下");
        playSound("ジャンプ踏切", -1);
    }

    playSound("声小ジャンプ", -1);

    if (a2) {
        setFrontVecKeepUp(rVec);
    }

    stopWalk();
    mRabbit->forceJump();
    return;
}

void Mario::tryStickJump(const TVec3f& rVec) {
    stopJump();
    initJumpParam();

    mMovementStates.jumping = true;
    mMovementStates._5 = false;
    mMovementStates._E = true;
    mMovementStates._B = false;
    _430 = 0;
    mMovementStates._6 = false;
    mMovementStates._9 = true;
    _24 &= 0xFF7FFFFF;
    mMovementStates._28 = false;
    mMovementStates._29 = false;
    mJumpVec = rVec;
    _3CA = 0;
    mMovementStates._13 = true;
    mMovementStates._11 = false;
    mMovementStates._1D = true;
    _3CC = 0;
    _3BE = 0;
    _76C = mActor->getConst().getTable()->mBeeGravityReviveTime;
    _770 = 0.0f;
    _3BC = 0xA;
    playSound("ハチ壁ジャンプ", -1);
    playSound("声小ジャンプ", -1);
    setFrontVecKeepUp(rVec);
    changeAnimationNonStop("ハチ壁ジャンプ");
    return;
}

void Mario::trySlipUpJump() {
    if (mMovementStates._1) {
        mMovementStates._6 = false;
        mMovementStates._28 = false;
        return;
    }

    changeAnimation("スリップアップ", static_cast< const char* >(nullptr));
    playSound("スリップアップ", -1);
    playEffect("スリップアップ");
    startPadVib("マリオ[スリップアップ]");

    mJumpVec = -getAirGravityVec() * mActor->getConst().getTable()->mSlipUpHeight;

    f32 ratio = 1.0f;
    TVec3f predictPos(mPosition - *getGravityVec() * 100.0f);

    TVec3f frontVec;
    MR::vecKillElement(mJumpVec, mFrontVec, &frontVec);
    MR::vecKillElement(frontVec, *getGravityVec(), &frontVec);
    frontVec += mFrontVec * mActor->getConst().getTable()->mSlipUpFront;
    MR::normalizeOrZero(&frontVec);

    u32 hitCount;
    TVec3f hitPos;
    for (hitCount = 0; hitCount < 10; hitCount++) {
        Triangle* pTmpPolygon = getTmpPolygon();
        predictPos += frontVec * 50.0f;

        if (!MR::getFirstPolyOnLineBFast(predictPos, *getGravityVec() * 150.0f, &hitPos, pTmpPolygon)) {
            break;
        }
    }

    ratio *= static_cast< f32 >(hitCount) / 10.0f;
    cutVecElementFromJumpVec(mFrontVec);

    if (mMovementStates._9) {
        mJumpVec += mFrontVec * mActor->getConst().getTable()->mSlipUpFront * ratio;
        mMovementStates._29 = true;
    } else {
        mJumpVec += mFrontVec * mActor->getConst().getTable()->mSlipUpFrontWeak * ratio;
    }

    mMovementStates._6 = false;
    mMovementStates._E = true;
    initJumpParam();
    _430 = 0xA;
    return;
}

void Mario::tryHangSlipUp() {
    mMovementStates._5 = false;
    getPlayer()->tryJump();
    changeAnimation("つかまりスリップアップ", static_cast< const char* >(nullptr));
    playSound("スリップアップ", -1);
    playEffect("スリップアップ");
    startPadVib("マリオ[スリップアップ]");

    const f32 slipUpHeight = mActor->getConst().getTable()->mSlipUpHeightHang;
    mJumpVec = -getAirGravityVec() * slipUpHeight;
    cutVecElementFromJumpVec(mFrontVec);

    const f32 slipUpFront = mActor->getConst().getTable()->mSlipUpFrontHang;
    mJumpVec += mFrontVec * slipUpFront;

    mMovementStates._6 = false;
    mMovementStates._E = true;
    _430 = 0xA;
    mActor->setBlendMtxTimer(0x8);
    _71C = 0x7;
    _278 = 0.6f;
    return;
}

void Mario::tryDrop() {
    if ((mMovementStates._8) && !mDrawStates._B) {
        TVec3f horizontal;
        MR::vecKillElement(_4E8 - mGroundPos, *getGravityVec(), &horizontal);

        if (horizontal.dot(*mFrontWallTriangle->getNormal(0)) < 0.0f && horizontal.length() < 80.0f) {
            getPlayer()->mMovementStates._1 = true;
            push(*mFrontWallTriangle->getNormal(0) * (80.0f - horizontal.length()) * 0.1f);
            return;
        }
    }

    if ((mMovementStates._1A) && !mDrawStates._B) {
        TVec3f horizontal;
        MR::vecKillElement(_500 - mGroundPos, *getGravityVec(), &horizontal);

        if (horizontal.dot(*mSideWallTriangle->getNormal(0)) < 0.0f && horizontal.length() < 80.0f) {
            push(*mSideWallTriangle->getNormal(0) * (80.0f - horizontal.length()) * 0.1f);
            getPlayer()->mMovementStates._1 = true;
            return;
        }
    }

    mMovementStates._21 = true;
    mMovementStates._B = false;

    mJumpVec = mFrontVec * _278 * ::cDropFrontSpeed;

    if (MR::isNearZero(_8F8) && (mMovementStates._23)) {
        TVec3f side;
        side.cross(getAirGravityVec(), _368);
        MR::normalizeOrZero(&side);
        _8F8.cross(_368, side);
        MR::normalizeOrZero(&_8F8);
    }

    if (!MR::isNearZero(_8F8)) {
        mJumpVec = _8F8;
        cutGravityElementFromJumpVec(false);

        if (_16C.length() > 20.0f) {
            mJumpVec += -mActor->getAirGravityVec() * 10.0f;
        } else {
            f32 gravScale = mActor->_288.length();
            if (gravScale > 8.0f) {
                gravScale = 8.0f;
            }

            mJumpVec += -mActor->getAirGravityVec() * gravScale;
            _8F8.zero();
        }
    }

    TVec3f carryVec(_184);
    MR::vecKillElement(carryVec, *getGravityVec(), &carryVec);
    invalidateRelativePosition();

    TVec3f groundDeltaNoGravity;
    MR::vecKillElement(mGroundPos - mPosition, *getGravityVec(), &groundDeltaNoGravity);

    if (carryVec.dot(groundDeltaNoGravity) > 0.0f) {
        carryVec = carryVec * 0.1f;
    }

    mJumpVec += carryVec;

    _340 = 1.0f - _278;
    if (_340 < 0.0f) {
        _340 = 0.0f;
    }

    _430 = 0;
    procJump(true);

    mMovementStates.jumping = true;
    mMovementStates._5 = false;
    mMovementStates._E = true;
    _10._23 = false;
    mMovementStates._B = false;

    if (isAnimationRun("坂すべり上向きうつぶせ", 2) || isAnimationRun("坂すべり下向きあおむけ", 3)) {
        _428 = 0xF;
    }

    changeAnimation(nullptr, "落下");

    if (getPlayerMode() == 4) {
        _408 = mActor->getConst().getTable()->mBeeGravityPowerTimeD;
        _3BC = mActor->getConst().getTable()->mBeeAirWalkInhibitTimeD - 5;
    }

    if (isAnimationRun("ショート着地")) {
        mMovementStates._23 = true;
    }

    if (mMovementStates._8) {
        stopWalk();
    }

    return;
}

bool Mario::isDigitalJump() const NO_INLINE {
    return mMovementStates._E;
}

void Mario::initActiveJumpVec() {
    _2EC = getAirGravityVec();
    MR::vecKillElement(mJumpVec, _2EC, &_2E0);

    if (MR::normalizeOrZero(&_2E0)) {
        _2E0 = mFrontVec;
    }
}

void Mario::initJumpParam() {
    const u32 flags1 = _1C_WORD;
    mMovementStates._13 = true;

    if ((_1C._A) || (_1C._B)) {
        _10._1F = true;
    }

    mMovementStates._38 = false;
    _10._8 = false;
    mMovementStates._2F = false;
    _10._E = false;

    if (!mDrawStates._1B) {
        initActiveJumpVec();
    }

    _42C = 0;
    _3BC = 0;
    _3CA = 0;
    _3CC = 0;
    _774 = 0;
    mMovementStates._11 = false;
    _3BE = 0;
    _344 = mSideVec;

    _408 = 0;
    _76C = 0;
    mMovementStates._C = false;
    mMovementStates._10 = false;
    mMovementStates._4 = false;
    mMovementStates._20 = false;
    _10._C = false;
    _3D2 = 0;
    _3D0 = 0;
    _3CE = 0;
    mMovementStates._2A = mMovementStates._8;

    if (_430 != 5) {
        cancelSquatMode();
    }

    _4B0 = mPosition;
    _426 = 0;

    if (_735 != 0) {
        mJumpVec.scale(0.2f + (0.8f * ((0x100 - _735) * 0.00390625f)));
        _735 = 0;
    }
}

bool Mario::isEnableFutureJump() const {
    if (_430 == 0x3) {
        return false;
    }

    if (!isRising()) {
        const f32 dot = mJumpVec.dot(*getGravityVec());
        const s16 limit = mActor->getConst().getTable()->mFutureJumpReqLimitTime;
        if (mVerticalSpeed < dot * static_cast< f32 >(limit)) {
            return true;
        }
    }

    return false;
}

void Mario::procJump(bool a1) {
    f32 gravityScale = 1.0f;
    if (!a1 && !_10._1F && _430 == 0 && _3BC < 6 && mActor->isRequestJump2P()) {
        _1C_WORD |= 0x00100000;
        mMovementStates.jumping = false;
        _3BC = 6;
        _10._1F = true;
        tryJump();
        _3BC = 6;
    }

    if (mMovementStates._B) {
        procHipDrop();
        return;
    }

    if (_424 != 0) {
        _424--;
        return;
    }

    procRocketBooster();

    if (_10._13) {
        fixFrontVecByGravity();
    }

    if (_10._1F) {
        if (!mActor->isKeepJump2P()) {
            mMovementStates._13 = false;
        }
    } else if (!mActor->isKeepJump()) {
        mMovementStates._13 = false;
    }

    if (checkTrgA()) {
        _558 = mActor->_37C;
        if (isEnableFutureJump()) {
            mMovementStates._38 = true;
        }
    }

    if (mActor->isRequestJump2P()) {
        _558 = mActor->_37C;
        if (isEnableFutureJump()) {
            mMovementStates._38 = true;
        }
    }

    bool useConnectGravity = false;
    if (mMovementStates._13 && !isDigitalJump() && _430 == 9 && isRising()) {
        useConnectGravity = true;
    }

    if (a1 || mMovementStates._13 || isDigitalJump() || !isRising() || _430 == 2) {
        useConnectGravity = true;
    }

    if (_430 == 8 && _42A == 0) {
        s32 timer = _426;
        if (timer == 0) {
            useConnectGravity = true;
        } else {
            useConnectGravity = timer > (mActor->getConst().getTable()->mTornadoBoostTimer - 0x10);
        }
    }

    if (useConnectGravity) {
        if (_3BC < 0x2E) {
        }
        gravityScale = mActor->getConst().getTable()->mGravityRatioA;
        if (_3BC < 0x2E) {
        }
    }

    if (a1) {
        initJumpParam();
    } else if (mActor->isRequestHipDrop() && jumpToHipDrop()) {
        return;
    }

    if (!isRising()) {
        if (isAnimationRun("ジャンプB")) {
            stopAnimation(static_cast< const char* >(nullptr), static_cast< const char* >(nullptr));
        }

        if (!mSwim->_1B2) {
            if (isAnimationRun("水泳スピン移動")) {
                stopAnimation(static_cast< const char* >(nullptr), static_cast< const char* >(nullptr));
            }

            if (isAnimationRun("水泳スピンジャンプ")) {
                stopAnimation(static_cast< const char* >(nullptr), static_cast< const char* >(nullptr));
            }

            if (isAnimationRun("水泳ジェット")) {
                changeAnimation("空中一回転", static_cast< const char* >(nullptr));
            }
        }

        _3BE++;
    } else {
        f32 ceilDist = calcDistToCeil(false);
        if (ceilDist < 160.0f) {
            f32 jumpGravity = cutGravityElementFromJumpVec(true);

            playEffectTrans("天井ヒット", mPosition - getAirGravityVec() * ceilDist);

            f32 reduce;
            switch (_430) {
            case 4:
            case 6:
                reduce = 0.9f;
                break;
            case 5:
            default:
                reduce = 0.6f;
                break;
            }

            mJumpVec += getAirGravityVec() * jumpGravity * reduce;
        }
    }

    bool wasAirWalk = false;
    if (_3CC != 0) {
        wasAirWalk = true;
        _3CC--;
    }

    if (_3BC > 10 && checkTrgA() && (mMovementStates._17)) {
        mMovementStates._17 = false;
        if (mWall->startJump()) {
            return;
        }
    }

    mMovementStates._12 = false;
    if (_430 == 8) {
        if (checkTrgZ()) {
            resetTornado();
            cancelTornadoJump();
            stopAnimation(static_cast< const char* >(nullptr), static_cast< const char* >(nullptr));
            changeAnimationInterpoleFrame(1);
            mDrawStates._8 = true;
            playSound("声小ジャンプ", -1);

            if (_42A != 0) {
                playEffect("スピンライト消去");
            }
        } else {
            mMovementStates._11 = false;
        }

        if (_42A != 0) {
            if (_402 != 0) {
                _402--;
            }

            if (_402 == 0) {
                cancelTornadoJump();
                playEffect("スピンライト消去");

                if (getPlayerMode() != 4) {
                    _402 = mActor->getConst().getTable()->mAirWalkTime;
                }
            }
        }
    }

    if (beeMarioOnAir()) {
        _76C = mActor->getConst().getTable()->mBeeGravityReviveTime;
    }

    if (_406 != 0) {
        _406--;
    }

    f32 jumpGravity = 0.0f;
    if (_430 == 0xC) {
        jumpGravity = 1.8f;
    } else if (_430 == 0xD) {
        jumpGravity = mActor->getConst().getTable()->mGravityJumping[9];
    } else {
        jumpGravity = mActor->getConst().getTable()->mGravityJumping[_430];
    }

    if (_430 == 0xB && (isRising() || _3BE < mActor->getConst().getTable()->mTrampleBegomaOpenTime)) {
        jumpGravity = mActor->getConst().getTable()->mGravityJumping[0];
    }

    if (_430 == 8 && _42A == 1) {
        jumpGravity = mActor->getConst().getTable()->mSpinJumpGravity;
    }

    if (_430 == 8 && !isRising()) {
        jumpGravity *= 0.25f;
    }

    f32 clingScale = 1.0f;
    const s32 clingNum = MR::getKarikariClingNum();
    if (clingNum != 0) {
        clingScale = (2.0f + static_cast< f32 >(clingNum)) * 0.5f;
        if (getPlayerMode() == 4) {
            clingScale = 1.0f;
        }

        jumpGravity *= clingScale;
    }

    if (wasAirWalk) {
        jumpGravity = mActor->getConst().getTable()->mGravityAirWalk;
    }

    if (_76C != 0) {
        const u16 maxTimer = mActor->getConst().getTable()->mBeeGravityReviveTime;
        const f32 t = static_cast< f32 >(_76C) / static_cast< f32 >(maxTimer);
        _76C--;
        jumpGravity = ((1.0f - t) + (t * mActor->getConst().getTable()->mGravityAirWalk)) * clingScale;
    }

    if (_430 == 0xD && (isDigitalJump() || !isRising())) {
        gravityScale = 1.0f;
    }

    gravityScale *= mActor->getGravityRatio();
    if (mActor->_334 != 0) {
        gravityScale *= 0.3f;
        if (!mActor->isInZeroGravitySpot() && mVerticalSpeed < 300.0f) {
            gravityScale = 0.0f;
            if (mVerticalSpeed < 200.0f) {
                cutGravityElementFromJumpVec(true);
                gravityScale = -0.1f;
            }
        }
    }

    if (getPlayerMode() == 6) {
        gravityScale = 0.0f;
    }

    if (mMovementStates._17) {
        f32 wallRatio = static_cast< f32 >(_3BC) / static_cast< f32 >(mActor->getConst().getTable()->mSlipUpSpdCtrlTimer);
        if (wallRatio > 1.0f) {
            wallRatio = 1.0f;
        }

        const f32 wallScale =
            mActor->getConst().getTable()->mSlipUpSpdRatio + ((1.0f - wallRatio) * (1.0f - mActor->getConst().getTable()->mSlipUpSpdRatio));
        addVelocity(mJumpVec, wallScale);

        mJumpVec += mActor->_240 * jumpGravity * gravityScale * wallScale;
        moveWallSlide(1.0f);
    } else if ((mMovementStates._1) == 0) {
        addVelocity(mJumpVec);

        mJumpVec += mActor->_240 * jumpGravity * gravityScale;
    }

    checkWallRising();
    checkWallJumpHit();

    if ((mMovementStates._1) == 0) {
        fixFrontVecFromUpSide();
    }

    decideSlipUp();

    if (_430 != 0xD || !mMovementStates._20) {
        TVec3f side;
        const f32 dropSpeed = MR::vecKillElement(mJumpVec, *getGravityVec(), &side);

        f32 maxDrop = mActor->getConst().getTable()->mMaxDropSpeed[mActor->getGravityLevel() & 0xFF];
        if (_430 == 0xB && !isRising() && _3BE >= mActor->getConst().getTable()->mTrampleBegomaOpenTime) {
            maxDrop = mActor->getConst().getTable()->mMaxBackJumpSpeed;
        }

        if (getPlayerMode() == 4 && !isRising()) {
            maxDrop = mActor->getConst().getTable()->mMaxDropSpeed[4];
        }

        if (maxDrop <= dropSpeed) {
            mJumpVec = *getGravityVec() * maxDrop + side;
        }
    }

    if ((mMovementStates._1) != 0 && !a1) {
        doLanding();

        if (calcDistToCeil(false) < 80.0f) {
            bool pressedA = false;
            const HitSensor* sensorA = reinterpret_cast< const HitSensor* >(_730);
            if (sensorA != nullptr && MR::isSensorPressObj(sensorA)) {
                pressedA = true;
            }

            bool pressedB = false;
            const HitSensor* sensorB = mGroundPolygon->mSensor;
            if (sensorB != nullptr && MR::isSensorPressObj(sensorB)) {
                pressedB = true;
            }

            if (sensorA != sensorB && (pressedA || pressedB)) {
                mActor->setPress(0, 0);
                mActor->_3B4 = *mGroundPolygon->getNormal(0);
            }
        }
    } else {
        doAirWalk();
    }

    _3BC++;
    return;
}

void Mario::checkWallRising() {
}

void Mario::checkWallJumpHit() {
    if (_430 == 0x5) {
        if ((mMovementStates._8) && !checkWallJumpCode()) {
            const f32 wallDot = cutVecElementFromJumpVec(getWallNorm());
            if (checkWallCode("NoAction", true)) {
                return;
            }

            mJumpVec -= getWallNorm() * wallDot;

            blown(mJumpVec * 0.2f);

            TVec3f horizontal;
            MR::vecKillElement(mJumpVec, *getGravityVec(), &horizontal);
            MR::normalizeOrZero(&horizontal);
            if (!MR::isNearZero(horizontal)) {
                setFrontVecKeepUp(-horizontal);
            }

            mMovementStates._2B = true;
            _402 = 0;
            _428 = 0x3C;
        }
        return;
    }

    if (!isRising() && _3BC > 10 && (mMovementStates._8) && calcPolygonAngleD(mFrontWallTriangle) < 80.0f) {
        if ((_10._E) == 0) {
            mJumpVec = *mFrontWallTriangle->getNormal(0) * 5.0f + getAirGravityVec() * cutGravityElementFromJumpVec(true);
            _10._E = true;
            changeAnimation("壁はじき", static_cast< const char* >(nullptr));
        } else {
            const f32 jumpMag = mJumpVec.length();
            cutVecElementFromJumpVec(*mFrontWallTriangle->getNormal(0));
            mJumpVec.setLength(jumpMag);
        }
    }
}

void Mario::decideSlipUp() {
    if (isAnimationRun("壁はじき")) {
        return;
    }

    if (!mMovementStates._9) {
        return;
    }

    if (mMovementStates._F) {
        mMovementStates._6 = false;
        return;
    }

    if (!mMovementStates._6) {
        if (checkWallCode("NotWallSlip", true)) {
            return;
        }

        if (checkWallCode("NoAction", true)) {
            return;
        }

        bool slipFront = false;
        if (isStickOn()) {
            const TVec3f& worldPadDir = getWorldPadDir();
            if (worldPadDir.dot(getWallNorm()) < -0.8f) {
                slipFront = true;
            }
        }

        if (_430 == 0x7) {
            slipFront = true;
        }

        if ((mMovementStates._2A) == 0 && (mMovementStates._8) && isRising() && slipFront) {
            mMovementStates._6 = true;
        }

        if (isRising() && getPlayerMode() != 4 && getPlayerMode() != 6 && getPlayerMode() != 5) {
            if (_4E0 < mActor->getConst().getTable()->mSlipUpContinueHeight) {
                if (mMovementStates._8 && mMovementStates._9 && slipFront && mMovementStates._29) {
                    mMovementStates._6 = true;
                    cutGravityElementFromJumpVec(true);

                    mJumpVec += -getAirGravityVec() * 30.0f;
                }
            }
        }
    } else if (!mMovementStates._7 && isRising() && !mMovementStates._8 && (mMovementStates._15)) {
        trySlipUpJump();
    }
}

void Mario::moveWallSlide(f32 a1) {
    TVec3f crossVec;
    const TVec3f& wallNorm = getWallNorm();
    crossVec.cross(*getGravityVec(), wallNorm);

    if (!MR::normalizeOrZero(&crossVec) && isStickOn()) {
        f32 dot = crossVec.dot(getWorldPadDir());
        const f32 absDot = __fabsf(dot);
        if (absDot > 0.2f) {
            if (dot < -0.2f) {
                dot = (0.2f + dot) / 0.8f;
            } else {
                dot = (dot - 0.2f) / 0.8f;
            }

            addVelocity(crossVec, (10.0f * dot) * a1);
        }
    }
}

bool Mario::jumpToHipDrop() {
    if (mMovementStates._B) {
        return false;
    }

    switch (_430) {
    case 5:
        return false;

    case 9:
    case 11:
    case 12:
    case 13:
        if (getPlayerMode() != 4 && isRising()) {
            return false;
        }
    }

    if (mVerticalSpeed < mActor->getConst().getTable()->mHipDropLimitHeight) {
        return false;
    }

    if (MR::isNearZero(mActor->_240)) {
        return false;
    }

    if (getPlayerMode() == 6) {
        return false;
    }

    bool isNormalDrop = false;
    if (mActor->_3E5) {
        isNormalDrop = true;
    }

    resetTornado();
    cancelTornadoJump();

    mDrawStates._8 = true;
    mMovementStates._21 = false;
    mMovementStates._B = true;
    mJumpVec.zero();
    _422 = 0;

    if (_430 == 4) {
        setFrontVecKeepUp(-_220);
        _430 = 0;
    }

    if (isAnimationRun("カリカリ限界")) {
        stopAnimationUpper(nullptr, nullptr);
    }

    if (isPlayerModeHopper()) {
        _720 = getAnimationStringPointer("ホッパーヒップドロップ開始");
        _724 = getAnimationStringPointer("ホッパーヒップドロップ");
        _728 = getAnimationStringPointer("ヒップドロップ着地");
        startHipDropBlur();
    } else if (getPlayerMode() == 4) {
        _720 = getAnimationStringPointer("ハチヒップドロップ開始");
        _724 = getAnimationStringPointer("ハチヒップドロップ");

        if (mActor->mAlphaEnable) {
            _728 = getAnimationStringPointer("ハチヒップドロップ壁着地");
        } else {
            _728 = getAnimationStringPointer("ハチヒップドロップ着地");
        }
    } else if (isNormalDrop) {
        _720 = getAnimationStringPointer("スピンヒップドロップ開始");
        _724 = getAnimationStringPointer("スピンヒップドロップ");
        _728 = getAnimationStringPointer("スピンヒップドロップ着地");
        stopEffect("スピンライト");
        _10._27 = true;
    } else {
        _720 = getAnimationStringPointer("ヒップドロップ開始");
        _724 = getAnimationStringPointer("ヒップドロップ");
        _728 = getAnimationStringPointer("ヒップドロップ着地");
        startHipDropBlur();
    }

    changeAnimation(_720, _724);
    playSound("尻ドロップ回転", -1);

    if (isNormalDrop) {
        playSound("声スピン尻ドロップ", -1);
        playSound("スピン尻ドロップ回転", -1);

    } else {
        playSound("声尻ドロップ", -1);
    }

    _424 = mActor->getConst().getTable()->mHipDropZeroGrTime;
    mRabbit->forceJump();
    mMovementStates._3E = 0;
    return true;
}

void Mario::procHipDrop() {
    if (isAnimationRun(_720)) {
        if (!isAnimationTerminate(nullptr)) {
            return;
        }

        if (_424 != 0) {
            _424--;
            return;
        }

        stopAnimation(static_cast< const char* >(nullptr), static_cast< const char* >(nullptr));
        return;
    }

    if (_424 != 0) {
        _424--;
        return;
    }

    if (mActor->_38C != 0) {
        return;
    }

    if (MR::isNearZero(mActor->_240)) {
        mMovementStates.digitalJump = true;
    }

    if (mActor->_334 != 0) {
        _2F8.x *= 0.5f;
        _2F8.y *= 0.5f;
        _2F8.z *= 0.5f;
        mJumpVec.x *= 0.5f;
        mJumpVec.y *= 0.5f;
        mJumpVec.z *= 0.5f;
    }

    if ((mMovementStates._1) != 0 || isAnimationRun(_728)) {
        fixFrontVecByGravity();

        if (isAnimationRun(_728)) {
            bool shouldEnd = false;

            if (mActor->_334 != 0) {
                shouldEnd = true;
            } else if ((mMovementStates._1) == 0 && _3BC > 3 && mVerticalSpeed > 10.0f) {
                changeAnimation(_724, static_cast< const char* >(nullptr));
                mJumpVec = _2F8;
                goto PROC_HIP_DROP_MOVE;
            }

            u16 endLimit = 0xF;
            if (isAnimationRun("スピンヒップドロップ着地")) {
                endLimit = 0x2D;
            }

            if (_3CE > endLimit) {
                if (isStickOn()) {
                    shouldEnd = true;
                }

                if (mDrawStates._C) {
                    shouldEnd = true;
                }
            }

            if (_3CE > 5 && checkTrgA()) {
                shouldEnd = true;
            }

            if (isAnimationTerminate(nullptr) || shouldEnd) {
                mMovementStates.jumping = false;
                mMovementStates._B = false;
                stopAnimation(static_cast< const char* >(nullptr), "基本");
                stopEffect("属性尻ドロップ");

                if (checkTrgA()) {
                    tryJump();
                    return;
                }
            }

            _278 = 0.0f;
        } else {
            _3CE = 0;
            _278 = 0.0f;
            _71E = 0;

            if (isCurrentFloorSand()) {
                changeStatus(reinterpret_cast< MarioState* >(mBury));
                return;
            }

            if (isCurrentFloorSink()) {
                mMovementStates.jumping = false;
                mMovementStates._B = false;
                stopAnimation(static_cast< const char* >(nullptr), "基本");
                _735 = 200;
                return;
            }

            playSound("尻ドロップ着地", -1);
            playSound("声尻ドロップ着地", -1);
            playEffectRT("属性尻ドロップ", _368, mPosition);
            startPadVib("最強");
            startCamVib(0);
            mDrawStates._14 = true;
            MR::removeAllClingingKarikari();

            if (_960 == 0x1B || _960 == 0x1C || _960 == 9) {
                mMovementStates.jumping = false;
                mMovementStates._B = false;
                stopAnimation(static_cast< const char* >(nullptr), "基本");
                return;
            }

            changeAnimationWithAttr(_728, 1);
            _2F8 = mJumpVec;
            clearSlope();

            if (mActor->_B90) {
                mMovementStates.jumping = false;
                mMovementStates._B = false;
            }
        }

        mJumpVec.zero();
    } else {
        _3BC++;

        if (MR::isNearZero(mActor->getLastMove(), 0.001f)) {
            _422++;
            if (_422 == 0xF) {
                stopAnimation(static_cast< const char* >(nullptr), "基本");
                mMovementStates.jumping = false;
                mMovementStates._B = false;
                mJumpVec.zero();
                mMovementStates._1 = true;
                return;
            }
        }
    }
PROC_HIP_DROP_MOVE:
    TVec3f moveVec(mJumpVec);
    if (moveVec.length() >= mVerticalSpeed) {
        moveVec.setLength(mVerticalSpeed);
    }
    addVelocity(moveVec);

    if (!mMovementStates._1) {
        const f32 speedRate = MR::clamp(0.1f + (1.0f - (mJumpVec.length() / mActor->getConst().getTable()->mLimitSpeedHipDrop)), 0.0f, 1.0f);
        const f32 gravityHipDrop = mActor->getConst().getTable()->mGravityHipDrop;

        mJumpVec += *getGravityVec() * gravityHipDrop * speedRate;

        const f32 jumpGravity = cutGravityElementFromJumpVec(true);
        if (mJumpVec.length() > 10.0f) {
            mJumpVec.x *= 0.5f;
            mJumpVec.y *= 0.5f;
            mJumpVec.z *= 0.5f;
        }

        mJumpVec += getAirGravityVec() * jumpGravity;

        if (mJumpVec.length() > mActor->getConst().getTable()->mLimitSpeedHipDrop) {
            mJumpVec.setLength(mActor->getConst().getTable()->mLimitSpeedHipDrop);
        }

        if (_10._27 && isAnimationRun("スピンヒップドロップ")) {
            playSound("スピン尻ドロップ落下", -1);

            Triangle strikeTriangles[0x20];
            HitSensor* homingSensor = nullptr;

            f32 speed = mVerticalSpeed;
            if (speed >= 500.0f) {
                speed = 500.0f;
            }

            s32 strikeNum = Collision::checkStrikeBallToMapWithThickness(mShadowPos, speed, speed, nullptr, nullptr);

            for (u32 i = 0; i < strikeNum; i++) {
                const HitInfo* strikeInfo = Collision::getStrikeInfoMap(i);

                if (mActor->selectHomingInSuperHipDrop(strikeInfo->mParentTriangle.mSensor->mHost->mName)) {
                    TVec3f sensorPos(strikeInfo->mParentTriangle.mSensor->mPosition);

                    if (MR::diffAngleAbs(sensorPos - mActor->_2A0, *getGravityVec()) < 45.0f) {
                        homingSensor = strikeInfo->mParentTriangle.mSensor;
                    }
                }
            }

            if (homingSensor == nullptr) {
                homingSensor = reinterpret_cast< HitSensor* >(mActor->_4A8);
            }

            if (homingSensor != nullptr) {
                TVec3f targetJump(homingSensor->mPosition - mActor->_2A0);
                targetJump.setLength(mJumpVec.length());

                TVec3f lastMove;
                mActor->getLastMove(&lastMove);
                if (lastMove.dot(mJumpVec) < 0.0f) {
                    const f32 hitGravity = cutGravityElementFromJumpVec(true);
                    mJumpVec = -mJumpVec + getAirGravityVec() * hitGravity;
                } else if (mJumpVec.dot(targetJump) > 0.0f) {
                    mJumpVec = targetJump;
                }
            }

            if ((mMovementStates._8) || (mMovementStates._1A) || (mMovementStates._19)) {
                _10._27 = false;
            }
        }

        fixFrontVecFromUpSide();
    }
}

void Mario::doAirWalk() {
    if (isAnimationRun("水泳ジェット")) {
        return;
    }

    if (_430 == 3) {
        return;
    }

    if (mMovementStates._2F) {
        return;
    }

    if (isAnimationRun("壁はじき")) {
        return;
    }

    if (mMovementStates._1D) {
        if (_3BC > mActor->getConst().getTable()->mWaitNeutralTimer) {
            mMovementStates._1D = false;
        }
        return;
    }

    if (_428 != 0) {
        _428--;
        if (_428 == 0) {
            stopAnimation("坂すべり上向きうつぶせ", 2);
            stopAnimation("坂すべり下向きあおむけ", 3);
        }
        return;
    }

    TVec3f moveDir;
    calcMoveDir(mStickPos.x, mStickPos.y, &moveDir, true);

    if (mActor->_334 != 0) {
        moveDir.zero();
    }

    if (mDrawStates._D) {
        if (MR::diffAngleAbs(getAirGravityVec(), _2EC) > 0.7853982f) {
            TVec3f sideVec;
            sideVec.cross(mFrontVec, getAirGravityVec());
            MR::normalizeOrZero(&sideVec);

            Mtx rotMtx;
            PSMTXRotAxisRad(rotMtx, &sideVec, MR::diffAngleSignedHorizontal(_2EC, getAirGravityVec(), sideVec));

            TVec3f frontRot;
            PSMTXMultVecSR(rotMtx, &_2E0, &frontRot);
            if (frontRot.dot(mFrontVec) < 0.0f) {
                moveDir = -mFrontVec;
            } else {
                moveDir = mFrontVec;
            }
        }
    }

    MR::vecKillElement(moveDir, *getGravityVec(), &moveDir);
    TVec3f sideMove;
    bool inhibitFrontAdjust = false;
    f32 frontDot = MR::vecKillElement(moveDir, mFrontVec, &sideMove);

    if (_430 == 0xB || _430 == 8 || getPlayerMode() == 4 || getPlayerMode() == 6) {
        inhibitFrontAdjust = true;
    } else if (frontDot >= 0.0f) {
        f32 frontReduction = mActor->getConst().getTable()->mJumpFrontReduction;
        s16 reduceBegin = mActor->getConst().getTable()->mJumpFrontReductionBeginTime;
        if (_430 == 5) {
            frontReduction = mActor->getConst().getTable()->mSquatJumpFrontReduction;
            reduceBegin = mActor->getConst().getTable()->mSquatJumpFrontReductionBTime;
        }

        if (_10._17 && _8D8 == _45C->mSensor) {
            reduceBegin = 0xB4;
            if (MR::isSameMtx(_8E8->getBaseMtx()->toMtxPtr(), _8E8->getPrevBaseMtx()->toMtxPtr())) {
                MR::vecKillElement(_8DC, getAirGravityVec(), &_8DC);
                mJumpVec -= _8DC;
                _8DC.zero();
            }
        }

        if ((mMovementStates._11) == 0 && _3BC > reduceBegin) {
            const f32 jumpGravity = cutGravityElementFromJumpVec(true);
            const f32 jumpMag = mJumpVec.length();
            mJumpVec.setLength(frontReduction * jumpMag);

            mJumpVec += getAirGravityVec() * jumpGravity;
        }
    } else {
        if (!isRising() && mVerticalSpeed < mActor->getConst().getTable()->mLandTurnHeight && (mMovementStates._11) == 0) {
            mMovementStates._10 = true;
        } else {
            _3CA++;
        }

        f32 speedKiller;
        if (isAnimationRun("ターンジャンプ")) {
            _3CA = 0;
            speedKiller = 0.1f;
        } else if (_3CA < mActor->getConst().getTable()->mBackJumpLimitFrame) {
            const f32 remain = static_cast< f32 >(mActor->getConst().getTable()->mBackJumpLimitFrame - _3CA);
            speedKiller =
                mActor->getConst().getTable()->mBackJumpRatio * (remain / static_cast< f32 >(mActor->getConst().getTable()->mBackJumpLimitFrame));
        } else {
            speedKiller = 0.0f;
        }

        if (mMovementStates._11) {
            moveDir = sideMove + mFrontVec * mActor->getConst().getTable()->mAirWalkBackBonus * frontDot;
        } else {
            moveDir = sideMove + mFrontVec * frontDot * speedKiller;
        }

        const f32 backDot = -moveDir.dot(mFrontVec);
        if (backDot > mActor->getConst().getTable()->mMaxBackJumpSpeed) {
            moveDir.setLength(mActor->getConst().getTable()->mMaxBackJumpSpeed);
        } else if (backDot < 0.0f && _3CA < mActor->getConst().getTable()->mBackJumpLimitFrame) {
            _3CC = 0xA;
        }
    }

    TVec3f moveNorm(moveDir);
    MR::normalizeOrZero(&moveNorm);

    TVec3f jumpNorm(mJumpVec);
    MR::normalizeOrZero(&jumpNorm);

    if (!inhibitFrontAdjust) {
        frontDot = moveNorm.dot(jumpNorm);
        if (frontDot < 0.0f) {
            const f32 jumpGravity = cutGravityElementFromJumpVec(true);
            mJumpVec += mJumpVec * mActor->getConst().getTable()->mAirWalkSpeedKiller * frontDot;

            mJumpVec += getAirGravityVec() * jumpGravity;
        }
    }

    if (_430 == 8) {
        mJumpVec += moveDir * mActor->getConst().getTable()->mWalkSpeed * 5.0f / mActor->getConst().getTable()->mAirWalkTimerFact2;
    } else if (_430 == 0xB && !isRising()) {
        mJumpVec += moveDir * mActor->getConst().getTable()->mWalkSpeed * 5.0f / mActor->getConst().getTable()->mAirWalkTimerFact2;
        playSound("滞空中", -1);
    } else if (getPlayerMode() == 4) {
        mJumpVec += moveDir * mActor->getConst().getTable()->mBeeAirWalkAcc;
        playSound("滞空中", -1);
    } else if (getPlayerMode() == 6) {
        if (mJumpVec.dot(moveDir) >= 0.0f) {
            if (moveDir.dot(mFrontVec) > 0.0f) {
            }
            TVec3f normJump(mJumpVec);
            if (!MR::normalizeOrZero(&normJump)) {
                mJumpVec *= 0.998f - (0.01f * (1.0f - (0.5f * (1.0f + normJump.dot(mFrontVec)))));
            }
        }
    } else {
        mJumpVec += moveDir * mActor->getConst().getTable()->mWalkSpeed * mActor->getConst().getTable()->mAirWalkTimerFact1 /
                    (_3BC + mActor->getConst().getTable()->mAirWalkTimerFact2);
    }

    frontDot = 1.0f;
    if (_430 != 0xB && getPlayerMode() != 6 && !isRising()) {
        addVelocity(moveDir, mActor->getConst().getTable()->mWalkSpeed * mActor->getConst().getTable()->mAirWalkTimerFact3 * frontDot);
    }

    const f32 jumpGravity = cutGravityElementFromJumpVec(true);
    if (_430 == 8) {
        if (mJumpVec.length() > 6.0f) {
            mJumpVec.setLength(6.0f);
        }
    } else if (_430 == 0xB) {
        if (mJumpVec.length() > 7.0f) {
            mJumpVec.setLength(7.0f);
        }
    } else if (getPlayerMode() == 4) {
        if (_774 == 0) {
            if (mJumpVec.length() > mActor->getConst().getTable()->mBeeAirWalkLimit) {
                mJumpVec.setLength(mActor->getConst().getTable()->mBeeAirWalkLimit);
            }
        } else if (_774 > 0x1E) {
            if (mJumpVec.length() > mActor->getConst().getTable()->mMaxJumpSpeed) {
                mJumpVec.setLength(mActor->getConst().getTable()->mMaxJumpSpeed);
            }
        } else {
            f32 t = static_cast< f32 >(_774) / 30.0f;
            const f32 limit = (t * mActor->getConst().getTable()->mMaxJumpSpeed) + ((1.0f - t) * mActor->getConst().getTable()->mBeeAirWalkLimit);
            if (mJumpVec.length() > limit) {
                mJumpVec.setLength(limit);
            }
        }
    } else if (getPlayerMode() != 6) {
        if (mJumpVec.length() > mActor->getConst().getTable()->mMaxJumpSpeed) {
            mJumpVec.setLength(mActor->getConst().getTable()->mMaxJumpSpeed);
        }
    }
    mJumpVec += getAirGravityVec() * jumpGravity;
}

void Mario::stopJump() {
    fixFrontVecByGravity();
    mJumpVec.zero();

    mMovementStates.jumping = false;
    mMovementStates._B = false;
    mMovementStates._6 = false;

    mMovementStates._28 = false;
    mMovementStates._2B = false;

    _10._17 = false;
    _10._19 = false;
    _10._1F = false;

    if (getPlayerMode() != 4) {
        _402 = mActor->getConst().getTable()->mAirWalkTime;
    }

    _3BC = 0;
    _424 = 0;
    _426 = 0;
    _428 = 0;

    if (isDefaultAnimationRun("落下")) {
        changeAnimation(static_cast< const char* >(nullptr), "基本");
    }

    if (isDefaultAnimationRun(_724)) {
        changeAnimation(static_cast< const char* >(nullptr), "基本");
    }

    mWall->_1C = 0;
}

void Mario::cancelTornadoJump() {
    _430 = 0;

    if ((mMovementStates._1) == 0) {
        stopAnimation("スピンジャンプ", "落下");
    } else {
        stopAnimation("スピンジャンプ", static_cast< const char* >(nullptr));
    }

    _4B0 = mPosition;
    mMovementStates._2B = true;
    _426 = 0;
}

void Mario::setRocketBooster(const TVec3f& rVec, f32 a2, u16 a3) {
    _426 = a3;
    _454 = a2;
    _448 = rVec;
}

void Mario::procRocketBooster() {
    if (_426 == 0) {
        return;
    }

    _426--;
    mJumpVec += _448;
    _448.scale(_454);
}

bool Mario::isSoftLandingFloor() const {
    switch (_960) {
    case 0x6:
    case 0x7:
    case 0x8:
    case 0xB:
    case 0x22:
        return true;
    default:
        return false;
    }
}

void Mario::checkAndTryForceJump() {
    bool doForceJump = false;
    TVec3f forceJumpVec;

    if ((mMovementStates._1 && !mMovementStates.jumping) || isAnimationRun(_728)) {
        switch (_960) {
        case 0x6:
        case 0x22: {
            const f32 jumpPower = mActor->getConst().getTable()->mCodeJumpPower[0];
            forceJumpVec = *mGroundPolygon->getNormal(0) * jumpPower;
            doForceJump = true;
            break;
        }
        case 0x7: {
            const f32 jumpPower = mActor->getConst().getTable()->mCodeJumpPower[1];
            forceJumpVec = *mGroundPolygon->getNormal(0) * jumpPower;
            doForceJump = true;
            break;
        }
        case 0x8: {
            const f32 jumpPower = mActor->getConst().getTable()->mCodeJumpPower[2];
            forceJumpVec = *mGroundPolygon->getNormal(0) * jumpPower;
            doForceJump = true;
            break;
        }
        case 0xB: {
            MR::vecKillElement(_16C, *getGravityVec(), &forceJumpVec);
            forceJumpVec.setLength(10.0f);

            const f32 jumpPower = mActor->getConst().getTable()->mCodeJumpPower[3];
            forceJumpVec += -(*getGravityVec()) * jumpPower;
            doForceJump = true;
            break;
        }
        default:
            break;
        }
    }

    if (mMovementStates._2D) {
        doForceJump = false;

        if (mActor->mHealth != 0) {
            if (getCurrentStatus() == 2) {
                closeStatus(nullptr);
            }

            tryForceFreeJump(_304);
        }

        mMovementStates._2D = false;
    }

    if (mMovementStates._2E) {
        doForceJump = false;

        if (mActor->mHealth != 0) {
            if (getCurrentStatus() == 2) {
                closeStatus(nullptr);
            }

            tryFreeJump(_304, true);
        }

        mMovementStates._2E = false;
    }

    if (doForceJump) {
        mActor->sendMsgToSensor(mGroundPolygon->mSensor, 2);

        if (getCurrentStatus() == 2) {
            if (mActor->mHealth == 0) {
                mActor->forceGameOverNonStop();
            }

            closeStatus(nullptr);
        }

        mMovementStates._21 = true;
        mMovementStates._2E = true;
        _304 = forceJumpVec;
        changeAnimation("フリージャンプ", static_cast< const char* >(nullptr));

        if (mActor->isEnableNerveChange()) {
            playSound("トランポリンジャンプ大", -1);
            playSound("声大ジャンプ", -1);
            playSound("ジャンプ踏切", -1);
        }
    }
}

void Mario::doLanding() {
    bool doHardLanding = false;
    bool blendWait = false;
    bool keepFrontSlip = false;

    if (getPlayerMode() == 6) {
        return;
    }

    if (mMovementStates._37) {
        afterLanding2D();
    }

    mMovementStates.jumping = false;
    _10._1F = false;
    _10._21 = false;
    _3CE = 0;
    _1C_WORD |= 0x2000;

    f32 deltadot = __fabsf((mPosition - _4B0).dot(*getGravityVec()));
    if (_3BC <= 3 && deltadot < 1.0f && mJumpVec.length() < 10.0f) {
        stopAnimation(static_cast< const char* >(nullptr), "基本");
        return;
    }

    if (mActor->_EEB && (mMovementStates._2B) && _10._19 && _430 != 3) {
        _10._19 = false;
        mActor->_946 = 0x10;
    }

    resetTornado();

    if (mActor->mAlphaEnable) {
        playSound("ハチ壁くっつき", -1);
    } else {
        playSound("着地", -1);
    }

    if ((isAnimationRun("スケートアクセルジャンプ") || isAnimationRun("スケートジャンプ2") || isAnimationRun("スケートジャンプ3")) &&
        (getPlayerMode() == 3 || getPlayerMode() == 0)) {
        playEffect("スケート着地");
        if (getStickP() == 0.0f) {
            mJumpVec.zero();
            stopWalk();
            changeAnimation("スケート静止着地", "基本");
        } else {
            changeStatus(reinterpret_cast< MarioState* >(mSkate));
        }
        return;
    }

    if (_430 == 5 && isSkatableFloor()) {
        if (checkSquat(false)) {
            mMovementStates._A = true;
            _278 = 0.0f;
            stopAnimation(static_cast< const char* >(nullptr), "しゃがみ基本");
        } else {
            cancelSquatMode();
            stopAnimationUpper(static_cast< const char* >(nullptr), static_cast< const char* >(nullptr));
            changeAnimation("スケート静止着地", "基本");
        }
        return;
    }

    if (isAnimationRun("フーファイター着地")) {
        stopAnimation(static_cast< const char* >(nullptr), "基本");
        return;
    }

    if (getPlayer()->_10._2) {
        getPlayer()->_10._2 = false;
        _430 = 0xE;
    } else {
        const u32 landingType = mMovementStates._3E;
        if (landingType == 2) {
            getPlayer()->mMovementStates._3E = 0;
            stopWalk();
            playSound("重い着地", -1);
            playSound("声ステージイン着地", -1);
            changeAnimation("ステージインB", "基本");
            startCamVib(4);
            return;
        }

        if (landingType == 1) {
            getPlayer()->mMovementStates._3E = 0;
            playEffect("スーパースピンドライバ終了");
            playEffect("共通着地大");
            if (!isStickFull()) {
                keepFrontSlip = true;
            }
            doHardLanding = true;
            if (_1FC.dot(_368) < 0.0f) {
                if (_960 == 0xD || _960 == 0x1E || _960 == 5 || _960 == 0x17) {
                    changeStatus(reinterpret_cast< MarioState* >(mSukekiyo));
                } else {
                    stopWalk();
                    startCamVib(4);
                    changeAnimation("ハード着地", "基本");
                    _3CE += 0x10;
                    forceSetHeadVecKeepSide(_368);
                }
                return;
            }
        } else {
            if (isCurrentFloorSink()) {
                stopAnimation(static_cast< const char* >(nullptr), "基本");
                _735 = 0x40;
                return;
            }

            if ((_960 == 0x1B || _960 == 0x1C)) {
                if (!strcmp(MR::getSoundCodeString(_45C), "Sand")) {
                    stopAnimation(static_cast< const char* >(nullptr), "基本");
                    _735 = 0x20;
                    return;
                }
            } else if (_3BC > 10) {
                playEffect("共通着地普通");
            }
        }
    }

    switch (_430) {
    case 0:
    case 0xA:
    case 0xB:
    case 0xE:
        startPadVib(static_cast< u32 >(0));
        break;
    default:
        startPadVib(2);
        break;
    case 2:
    case 4:
    case 6:
        startPadVib(3);
        break;
    }

    getAnimator()->addRumblePower(5.0f, 0x3C);
    mMovementStates._21 = false;

    if (getPlayerMode() != 4) {
        _402 = mActor->getConst().getTable()->mAirWalkTime;
    } else {
        _71E = 0;
    }

    if (!isSoftLandingFloor()) {
        f32 deltadot = (_4B0 - mPosition).dot(-(*getGravityVec()));
        if (deltadot > mActor->getConst().getTable()->mHardLandingHeight) {
            doHardLanding = true;
        }
    }

    if (doHardLanding && isCurrentFloorSand()) {
        changeStatus(reinterpret_cast< MarioState* >(mBury));
        return;
    }

    if (isBlendWaitGround()) {
        blendWait = true;
    }
    fixFrontVecByGravity();

    if (!doHardLanding && !mDrawStates._C && _430 != 0xE && !isAnimationRun("飛び込みジャンプ") && !isAnimationRun("後方飛び込みジャンプ") &&
        !_10._8 && !isAnimationRun("水上ダメージ中")) {
        if (_430 == 5 && checkSquat(false)) {
            mMovementStates._A = true;
            _278 = 0.0f;
            stopAnimation(static_cast< const char* >(nullptr), "しゃがみ基本");
            goto POST_LANDING;
        }

        if (((isStickFull() || _60D != 0) && _3CA == 0) || (((mMovementStates._B) == 0) && checkSquat(false))) {
            stopAnimation(static_cast< const char* >(nullptr), "基本");
            if (!_10._A) {
                recordTurnSlipAngle();
            }

            if (_430 == 5) {
                cancelSquatMode();
            }
            goto POST_LANDING;
        }

        switch (_430) {
        case 2:
        case 4:
        case 5:
        case 6:
        case 8:
        case 0xB:
            keepFrontSlip = true;
            break;
        default:
            break;
        }

        if (_10._17 && _8D8 == _45C->mSensor) {
            TVec3f jumpDelta(mJumpVec - _8DC);
            if (jumpDelta.dot(mJumpVec) < 0.0f) {
                mJumpVec.zero();
            } else {
                mJumpVec = jumpDelta;
            }
        }

        const f32 frontDot = mFrontVec.dot(mJumpVec);
        if (frontDot > 0.5f && !keepFrontSlip) {
            _278 = frontDot / mActor->getConst().getTable()->mJumpFrontSpeed;
            _278 = MR::clamp(_278, 0.0f, 1.0f);
            stopAnimation(static_cast< const char* >(nullptr), "基本");
            _3FA = 0;
            _71E = 0;

            if (!_10._A) {
                recordTurnSlipAngle();
            }
            goto POST_LANDING;
        }
    }

    if (_430 == 4) {
        setFrontVecKeepUp(-_220);
    }

    if (isAnimationRun("飛び込みジャンプ") || isAnimationRun("後方飛び込みジャンプ") || _10._8) {
        if (isAnimationRun("後方飛び込みジャンプ")) {
            setFrontVecKeepUp(-mFrontVec);
        }

        if (_10._8) {
            doHardLanding = true;
        }

        stopWalk();
        if (doHardLanding || isAnimationRun("後方飛び込みジャンプ")) {
            if (doHardLanding) {
                changeAnimation("飛び込み失敗回転着地", "基本");
                startSlidingTask(8, 5.0f, 0x19);
                startCamVib(0);
            } else {
                changeAnimation("飛び込み失敗着地", "基本");
            }
        } else {
            changeAnimation("飛び込み失敗着地", "基本");
        }

        goto POST_LANDING;
    }
    if (!mDrawStates._C) {
        clearSlope();
        switch (_430) {
        case 0:
        case 7:
            changeAnimation("着地", "基本");
            break;
        case 1:
            changeAnimation("着地B", "基本");
            break;
        case 2:
            changeAnimation("着地C", "基本");
            break;
        case 4:
            changeAnimation("着地ターン", "基本");
            break;
        case 5:
            if (!checkSquat(false)) {
                cancelSquatMode();
            }

            if (mActor->_468 == 0) {
                stopAnimationUpper(static_cast< const char* >(nullptr), static_cast< const char* >(nullptr));
            }

            changeAnimation("着地幅とび", "基本");
            break;
        case 6:
            changeAnimation("しゃがみジャンプ着地", "基本");
            break;
        default:
            if (isAnimationRun("水上ダメージ中")) {
                changeAnimation("中ダメージ着地", "基本");
            } else {
                changeAnimation("着地", "基本");
            }
            break;
        case 3:
        case 0xA:
            stopAnimation(static_cast< const char* >(nullptr), "基本");
            break;
        case 0xE:
            playSound("倒れ", -1);
            playSound("着地", -1);
            playSound("声小ダメージ", -1);
            playEffect("共通引き戻し着地");
            changeAnimation("引き戻し着地", "基本");
            doHardLanding = false;
            blendWait = false;
            break;
        }
        stopWalk();
        if (blendWait) {
            changeAnimation("ショート着地", "基本");
        }

        if (doHardLanding) {
            changeAnimation("ハード着地", "基本");
            playSound("重い着地", -1);
            playSound("声着地停止", -1);
            startCamVib(4);
            _3CE += 0x10;
        }

        if (mActor->mAlphaEnable) {
            changeAnimation("ハチ匍匐着地", "基本");
        }
    } else {
        if (!isAnimationRun("坂すべり上向きうつぶせ", 2) && !isAnimationRun("坂すべり下向きあおむけ", 3)) {
            stopAnimation(static_cast< const char* >(nullptr), "基本");
        }
        changeAnimation("すべり着地", "基本");
        _71E = 0;
    }

POST_LANDING:
    if (mDrawStates._C) {
        TVec3f jumpNoGrav;
        MR::vecKillElement(mJumpVec, *getGravityVec(), &jumpNoGrav);

        if (jumpNoGrav.dot(*mGroundPolygon->getNormal(0)) > 0.0f) {
            mMovementStates._23 = true;
            _8F0 = 10.0f;
        } else {
            const f32 ratio = (90.0f - calcPolygonAngleD(mGroundPolygon)) / 90.0f;
            _278 = _278 * ratio;
            _278 = MR::clamp(_278, 0.0f, 1.0f);

            const f32 minVel = 10.0f - (10.0f * ratio);
            if (_8F0 < minVel) {
                _8F0 = minVel;
            }
        }

        if (jumpNoGrav.length() < _8F8.length()) {
            _8F8.setLength(0.5f * (_8F8.length() + jumpNoGrav.length()));
        }
    } else {
        _8F8.zero();
    }

    return;
}

void Mario::startSlidingTask(u32 a1, f32 a2, u16 a3) {
    _A40 = mFrontVec * a2;
    _A3C = a3;
    pushTask(reinterpret_cast< Task >(&Mario::taskOnSlide), a1);
}

bool Mario::taskOnSlide(u32 a1) {
    (void)a1;
    addVelocity(_A40);
    _A3C--;
    return _A3C != 0;
}

bool Mario::taskOnWallRising(u32 a1) {
    (void)a1;

    if ((mMovementStates._17) == 0) {
        stopEffect("共通壁上昇");
        return false;
    }

    playSound("スリップ", -1);
    return true;
}

void Mario::incAirWalkTimer() {
    const u16 timer = _402;
    const u16 limit = mActor->getConst().getTable()->mAirWalkTime;

    if (timer >= limit) {
        return;
    }

    _402 = timer + 1;

    if (getPlayerMode() != 4) {
        return;
    }

    const u16 nextTimer = _402;
    const u16 nextLimit = mActor->getConst().getTable()->mAirWalkTime;

    if (nextTimer >= nextLimit) {
        playSound("ハチ体力完全回復", -1);
    } else {
        playSound("ハチ体力回復", -1);
    }
}

namespace NrvMarioActor {
    INIT_NERVE(MarioActorNrvWait);
    INIT_NERVE(MarioActorNrvGameOver);
    INIT_NERVE(MarioActorNrvGameOverAbyss);
    INIT_NERVE(MarioActorNrvGameOverAbyss2);
    INIT_NERVE(MarioActorNrvGameOverFire);
    INIT_NERVE(MarioActorNrvGameOverBlackHole);
    INIT_NERVE(MarioActorNrvGameOverNonStop);
    INIT_NERVE(MarioActorNrvGameOverSink);
    INIT_NERVE(MarioActorNrvTimeWait);
    INIT_NERVE(MarioActorNrvNoRush);
};  // namespace NrvMarioActor
