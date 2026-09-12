#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioMessenger.hpp"
#include "Game/Player/MarioState.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"

static f32 mSensorRadiusAttack = 120.0f;
static f32 mSensorRadiusTornadoAttack = 150.0f;
static f32 mSensorRadiusHipDropAttack = 80.0f;
static f32 mSensorRadiusClapCatch = 600.0f;
static f32 mSensorRadiusClapCatchSwim = 2000.0f;
static f32 mSensorRadiusTrampleAttack = 80.0f;
static f32 mSensorRadiusSpinStorm = 600.0f;
static f32 mSensorRadiusTornadoStorm = 1000.0f;
static f32 mSensorRadiusSpinPull = 450.0f;
static f32 mSensorRadiusSpinPullOnGround = 450.0f;

void MarioActor::attackOrPushSensor(HitSensor* sensor, f32 distance) {
    f32 radius = sensor->mRadius;
    mMario->checkOnimasu(sensor);
    bool pull = false;
    if (mMario->mMovementStates._F) {
        if (distance < radius + mSensorRadiusTornadoAttack && tryTornadoAttack(sensor)) {
            return;
        }
        if (distance < radius + mSensorRadiusTornadoStorm) {
            sensor->receiveMessage(ACTMES_TORNADO_STORM_RANGE, getSensor("body"));
            if (mMario->isSwimming()) {
                if (distance < mSensorRadiusSpinPull) {
                    pull = true;
                }
            } else if (distance < mSensorRadiusSpinPullOnGround) {
                pull = true;
            }
        }
    } else {
        if (_3E5 && _945 < 15) {
            if (distance < radius + mSensorRadiusSpinStorm) {
                if (selectAction("スピンアタック") < 4) {
                    sensor->receiveMessage(ACTMES_SPIN_STORM_RANGE, getSensor("body"));
                }
                if (mMario->isSwimming()) {
                    if (distance < mSensorRadiusSpinPull) {
                        pull = true;
                    }
                } else if (distance < mSensorRadiusSpinPullOnGround) {
                    pull = true;
                }
            }
        } else if (isAnimationRun("ファイアスピン") && distance < radius + mSensorRadiusSpinStorm) {
            sensor->receiveMessage(ACTMES_SPIN_STORM_RANGE, getSensor("body"));
        }
    }
    if (pull && !_424) {
        tryTornadoPull(sensor);
    }
    if (mMario->mMovementStates._B && !mMario->mMovementStates._1 &&
        distance < radius + mSensorRadiusHipDropAttack + mMario->mJumpVec.dot(getGravityVector())) {
        if (tryHipDropAttack(sensor) || tryGetItem(sensor)) {
            return;
        }
        mMario->_10._27 = false;
    }
    if (distance < radius + mSensorRadiusAttack) {
        if (_944 && tryPunchAirAfter(sensor)) {
            return;
        }
        if (checkAndTryTrampleAttack(sensor, distance, false)) {
            return;
        }
        bool touching = false;
        if (mPlayerMode == PlayerMode_Teresa) {
            TVec3f offset = _2A0 - sensor->mPosition;
            f32 separation = offset.length();
            if (separation < sensor->mRadius + mConst->getTable()->mTeresaBodyRadius) {
                if (selectTeresaThru(sensor)) {
                    return;
                }
                if (tryGetItem(sensor)) {
                    return;
                }
                offset.setLength(sensor->mRadius + mConst->getTable()->mTeresaBodyRadius - separation);
                if (!MR::isSensorAutoRush(sensor)) {
                    mMario->doTeresaReflection(offset, false);
                    MR::sendArbitraryMsg(ACTMES_TERESA_PLAYER_TOUCH, sensor, getSensor("body"));
                    touchSensor(sensor);
                }
                if (MR::isSensorEnemy(sensor)) {
                    setPlayerMode(PlayerMode_Normal, true);
                }
            }
        } else {
            f32 height = _4B4;
            f32 width = 20.0f + _4B0;
            if (!mMario->isActiveTaskID(0x200)) {
                if (isJumping()) {
                    height += 10.0f;
                }
                if (mMario->mTargetWalkSpeedIndex >= 5) {
                    width += 15.0f;
                }
            } else {
                width -= 20.0f;
            }
            f32 sensorRadius = sensor->mRadius;
            if (cylinderPushCheck(sensor->mPosition - _2A0, sensorRadius, width, height)) {
                if (sendBodyAttack(sensor) || tryGetItem(sensor)) {
                    return;
                }
                touchSensor(sensor);
                touching = true;
                if (!MR::sendMsgPush(sensor, getSensor("dummy")) && !sensor->isType(ATYPE_POWER_STAR_BIND)) {
                    mMario->_10._27 = false;
                }
            }
        }
        addRushSensor(sensor, touching);
    } else if (mMario->_10._6) {
        f32 height = _4B4;
        f32 width = 20.0f + _4B0;
        if (isJumping()) {
            height += 10.0f;
        }
        if (mMario->mTargetWalkSpeedIndex >= 5) {
            width += 15.0f;
        }
        f32 sensorRadius = sensor->mRadius;
        TVec3f offset = sensor->mPosition - _2A0;
        f32 depth = MR::vecKillElement(offset, mCamDirZ, &offset);
        if (depth < 250.0f && depth > -250.0f && cylinderPushCheck(offset, sensorRadius, width, height) && tryGetItem(sensor)) {
            return;
        }
    }
    f32 catchRadius = mSensorRadiusClapCatch;
    if (mMario->isSwimming()) {
        catchRadius = mSensorRadiusClapCatchSwim;
    }
    if (distance < radius + catchRadius) {
        tryAddClapCoin(sensor);
    }
    trySetLockOnTarget(sensor);
    if (mPlayerMode == PlayerMode_Bee && distance < 100.0f + radius) {
        mMario->tryBeeStick(sensor);
    }
    if (isUnderTarget(sensor) && !selectNotHomingSensor(sensor)) {
        f32 angle = MR::diffAngleAbs(sensor->mPosition - _2A0, getGravityVector());
        if (_4AC > angle) {
            _4AC = angle;
            _4A8 = sensor;
        }
    }
}

void MarioActor::attackOrPushSensorInDamage(HitSensor* pReceiver, f32 radius) {
    f32 sensorRadius = pReceiver->mRadius;

    if (!isEnableMoveMario()) {
        return;
    }

    if (radius < sensorRadius + mSensorRadiusAttack) {
        bool b1 = false;
        f32 f1 = _4B4;
        f32 f2 = 20.0f + _4B0;

        if (isJumping()) {
            f1 += 10.0f;
        }

        if (mMario->mTargetWalkSpeedIndex >= 5) {
            f2 += 15.0f;
        }

        f32 f3 = pReceiver->mRadius;

        if (cylinderPushCheck(pReceiver->mPosition - _2A0, f3, f2, f1)) {
            if (tryGetItem(pReceiver)) {
                return;
            }

            touchSensor(pReceiver);
            MR::sendMsgPush(pReceiver, getSensor("dummy"));
            b1 = true;
        }

        addRushSensor(pReceiver, b1);
        return;
    }

    if (!mMario->_10._6) {
        return;
    }

    f32 f1 = _4B4;
    f32 f2 = 20.0f + _4B0;

    if (isJumping()) {
        f1 += 10.0f;
    }

    if (mMario->mTargetWalkSpeedIndex >= 5) {
        f2 += 15.0f;
    }

    f32 f3 = pReceiver->mRadius;
    TVec3f diff = pReceiver->mPosition - _2A0;
    f32 vecKill = MR::vecKillElement(diff, mCamDirZ, &diff);
    if (vecKill < 250.0f && vecKill > -250.0f && cylinderPushCheck(diff, f3, f2, f1) && tryGetItem(pReceiver)) {
        return;
    }
}

void MarioActor::attackOrPushSensorInRush(HitSensor* pSensor, f32 radius) {
    f32 sensorRadius = pSensor->mRadius;
    if (radius < sensorRadius + mSensorRadiusAttack) {
        f32 height = _4B4;
        f32 width = 20.0f + _4B0;
        if (cylinderPushCheck(pSensor->mPosition - _2A0, sensorRadius, width, height)) {
            if (tryGetItem(pSensor)) {
                return;
            }

            sendMsgToSensor(pSensor, ACTMES_RUSH_PLAYER_TOUCH);
        }

        addRushSensor(pSensor, false);
    }

    if (radius < sensorRadius + mSensorRadiusClapCatch) {
        tryAddClapCoin(pSensor);
    }

    trySetLockOnTarget(pSensor);
}

void MarioActor::tryAddClapCoin(HitSensor* pSensor) {
    if (_7DC != 64 && pSensor->isType(ATYPE_STAR_PIECE)) {
        _6DC[_7DC] = pSensor;
        _7DC++;
    }
}

bool MarioActor::tryTornadoAttack(HitSensor* pSensor) {
    bool isMessageReceived = pSensor->receiveMessage(ACTMES_TORNADO_ATTACK, getSensor("body"));
    if (isMessageReceived) {
        mMario->startPadVib(1);
    }

    return isMessageReceived;
}

bool MarioActor::isUnderTarget(HitSensor* pSensor) {
    TVec3f down;
    if (mBeeWallWalk != 0) {
        down = _240;
    } else {
        MR::calcGravityVectorOrZero(this, pSensor->mPosition, &down, mGravityInfo, 0);
    }

    if (down.dot(mPosition - pSensor->mPosition) <= 0.0f && mMario->_424 == 0) {
        return true;
    }

    return false;
}

bool MarioActor::tryHipDropAttack(HitSensor* pSensor) {
    if (isUnderTarget(pSensor) && cylinderHorizontalCheck(pSensor)) {
        if (pSensor->isType(ATYPE_PLAYER_AUTO_JUMP)) {
            return tryTrampleAttack(pSensor);
        }

        return pSensor->receiveMessage(ACTMES_PLAYER_HIP_DROP, getSensor("body"));
    }

    return false;
}

bool MarioActor::checkAndTryTrampleAttack(HitSensor* sensor, f32 distance, bool force) {
    f32 radius = sensor->mRadius;
    bool trample = getMovementStates()._B != true;
    bool falling = false;
    if (isJumping() && !mMario->isRising()) {
        falling = true;
    }
    bool near = distance < radius + mSensorRadiusTrampleAttack + 1.5f * mMario->mJumpVec.dot(getGravityVector());
    if (force) {
        falling = isJumping();
    }
    if (trample && falling && near) {
        bool attacked = false;
        if (tryTrampleAttack(sensor)) {
            attacked = true;
        }
        if (mPlayerMode == PlayerMode_Invincible && MR::sendArbitraryMsg(ACTMES_INVINCIBLE_ATTACK, sensor, getSensor("body"))) {
            mMario->startPadVib(2);
            printHitMark(sensor);
        }
        return attacked;
    }
    return false;
}

bool MarioActor::tryTrampleAttack(HitSensor* pSensor) {
    if (_FCD) {
        return false;
    }

    if (mMario->isStatusActive(MarioStatus_Blown)) {
        return false;
    }

    bool isUnder = isUnderTarget(pSensor);
    bool isHorizontalCheck = cylinderHorizontalCheck(pSensor);
    if (isUnder && isHorizontalCheck) {
        _FCD = true;

        bool isReceiveMessage = pSensor->receiveMessage(ACTMES_PLAYER_TRAMPLE, getSensor("body"));

        _FCD = false;

        if (isReceiveMessage) {
            doTrampleJump(pSensor);
            tryGetItem(pSensor);
        }

        return isReceiveMessage;
    }

    return false;
}

bool MarioActor::cylinderHorizontalCheck(HitSensor* pSensor) {
    f32 radius = pSensor->mRadius;
    TVec3f diff = pSensor->mPosition - (_2A0 + mMario->_350 + mMario->_35C + mVelocity);
    TVec3f vec;
    MR::vecKillElement(diff, getGravityVector(), &vec);

    if (radius + _4B0 - vec.length() > 0.0f) {
        return true;
    }

    MR::vecKillElement(diff, _4C4, &vec);

    return radius + _4B0 - vec.length() > 0.0f;
}

bool MarioActor::tryJetAttack(HitSensor* pReceiver) {
    return MR::sendArbitraryMsg(ACTMES_JET_TURTLE_ATTACK, pReceiver, getSensor("dummy"));
}

void MarioActor::tryCounterJetAttack(HitSensor* pReceiver) {
    _1BC->addRequest(pReceiver, MR::MovementType_MsgSharedGroup);
}

bool MarioActor::tryGetItem(HitSensor* sensor) {
    if (_934) {
        switch (sensor->mType) {
        case ATYPE_JET_TURTLE:
        case ATYPE_JET_TURTLE_SLOW:
        case ATYPE_BOMBHEI:
        case ATYPE_NOKONOKO:
            return false;
        }
    }
    if (!isEnableNerveChange()) {
        return false;
    }
    if (!mHealth) {
        switch (sensor->mType) {
        case ATYPE_JET_TURTLE:
        case ATYPE_JET_TURTLE_SLOW:
        case ATYPE_BOMBHEI:
        case ATYPE_NOKONOKO:
        case ATYPE_MORPH_ITEM:
            return false;
        }
    }
    switch (sensor->mType) {
    case ATYPE_SWITCH:
        break;
    case ATYPE_NOKONOKO:
        if (isDamaging()) {
            return false;
        }
        if (!isActionOk("カメ持ち")) {
            return false;
        }
        if (_468 != 0) {
            return false;
        }
        if (!sensor->receiveMessage(ACTMES_ITEM_GET, getSensor("dummy"))) {
            return false;
        }
        _38C = 2;
        return false;
    case ATYPE_JET_TURTLE:
    case ATYPE_JET_TURTLE_SLOW:
    case ATYPE_COINTHROW:
    case ATYPE_BOMBHEI: {
        if (_3AC) {
            return false;
        }
        if (isDamaging()) {
            return false;
        }
        if (!isActionOk("カメ持ち")) {
            return false;
        }
        if (_468 != 0) {
            return false;
        }
        bool moving = false;
        TVec3f horizontal;
        if (MR::vecKillElement(*getShadowPos() - sensor->mPosition, getGravityVector(), &horizontal) > 100.0f) {
            moving = true;
        }
        if (!MR::isNearZero(sensor->mHost->mVelocity)) {
            moving = true;
        }
        if (getMovementStates()._B) {
            moving = false;
        }
        if (!moving && !mMario->isSwimming() && !getMovementStates()._1 && !_424) {
            if (getMovementStates()._B) {
                return false;
            }
            doTrampleJump(sensor);
        }
        break;
    }
    case ATYPE_MORPH_ITEM:
        if (_3D8) {
            return false;
        }
        if (mMario->mMorphResetTimer) {
            return false;
        }
        break;
    }
    if (!sensor->receiveMessage(ACTMES_ITEM_GET, getSensor("dummy"))) {
        return false;
    }
    switch (sensor->mType) {
    case ATYPE_COIN:
    case ATYPE_SWITCH:
    case ATYPE_KINOKO_ONEUP:
        break;
    case ATYPE_JET_TURTLE:
    case ATYPE_JET_TURTLE_SLOW:
    case ATYPE_COINTHROW:
    case ATYPE_BOMBHEI:
        mMarioAnim->changePickupAnimation(sensor);
        if (!mMario->isSwimming()) {
            if (getMovementStates()._1 && _B92 != -3) {
                if (sensor->mType == ATYPE_BOMBHEI) {
                    _38C = 40;
                } else {
                    _38C = 25;
                }
            }
            mVelocity.zero();
            mMario->mWalkSpeed = 0.0f;
            if (getMovementStates()._1) {
                mMario->stopJump();
                mMario->mVerticalSpeed = 0.0f;
            }
        }
        _424 = sensor;
        _480 = 1;
        break;
    case ATYPE_MORPH_ITEM:
        if (_4A4 != nullptr) {
            _4A4->mHost->kill();
        }
        _4A4 = sensor;
        break;
    }
    return true;
}

bool MarioActor::cylinderPushCheck(const TVec3f& offset, f32 radius, f32 width, f32 height) {
    TVec3f radial;
    f32 axial = MR::vecKillElement(offset, _4C4, &radial);
    if (axial > -radius) {
        f32 radialPenetration = radius + width - radial.length();
        f32 axialPenetration = radius + height - axial;
        if (radialPenetration > 0.0f && axialPenetration > 0.0f) {
            if (radialPenetration > 0.0f) {
                const TVec3f& axialOffset = _4C4 * axial;
                HitSensor* dummy = getSensor("dummy");
                dummy->mPosition.set(_2A0 + axialOffset);
                getSensor("dummy")->mRadius = width;
            } else {
                HitSensor* dummy = getSensor("dummy");
                dummy->mPosition.set(_2A0 + radial);
                getSensor("dummy")->mRadius = height;
            }
            return true;
        }
    }
    if (getMovementStates()._A && getMovementStates()._1) {
        return false;
    }
    axial = MR::vecKillElement(offset, _4B8, &radial);
    if (axial < -radius) {
        return false;
    }
    f32 radialPenetration = radius + width - radial.length();
    f32 axialPenetration = radius + height - axial;
    if (radialPenetration > 0.0f && axialPenetration > 0.0f) {
        if (radialPenetration > 0.0f) {
            const TVec3f& axialOffset = _4B8 * axial;
            HitSensor* dummy = getSensor("dummy");
            dummy->mPosition.set(_2A0 + axialOffset);
            getSensor("dummy")->mRadius = width;
        } else {
            HitSensor* dummy = getSensor("dummy");
            dummy->mPosition.set(_2A0 + radial);
            getSensor("dummy")->mRadius = height;
        }
        return true;
    }
    return false;
}

void MarioActor::attackOrPushPolygons() {
    // FIXME: if chain has a mistake
    HitSensor* bodySensor = getSensor("body");

    _FCC = true;

    if (mMario->getMovementStates()._1 && _390 == 0) {
        HitSensor* groundSensor = mMario->mGroundPolygon->mSensor;

        if (groundSensor != nullptr && !MR::isDead(groundSensor->mHost)) {
            groundSensor->receiveMessage(ACTMES_FLOOR_TOUCH, bodySensor);
        }
    }

    if (mMario->getMovementStates()._8) {
        sendWallTouch(mMario->mFrontWallTriangle->mSensor, bodySensor);
    }

    if (mMario->getMovementStates()._19) {
        sendWallTouch(mMario->mBackWallTriangle->mSensor, bodySensor);
    }

    if (mMario->getMovementStates()._1A) {
        sendWallTouch(mMario->mSideWallTriangle->mSensor, bodySensor);
    }

    if (mMario->getMovementStates().jumping && !mMario->getMovementStates()._1 && mMario->getMovementStates()._B) {
        _3E8 = true;
    } else {
        if (_3E8) {
            if (getMovementStates()._1) {
                if (mMario->mGroundPolygon->isValid()) {
                    HitSensor* groundSensor = mMario->mGroundPolygon->mSensor;

                    if (groundSensor != nullptr && !MR::isDead(groundSensor->mHost)) {
                        groundSensor = mMario->mGroundPolygon->mSensor;
                        if (!groundSensor->receiveMessage(ACTMES_PLAYER_HIP_DROP_FLOOR, getSensor("body"))) {
                            _3E8 = false;
                        }
                    }
                }
            } else {
                _3E8 = false;
            }
        }

        if (!mMario->getMovementStates().jumping) {
            _3E8 = false;
        }
    }

    _FCC = false;
}

void MarioActor::sendWallTouch(HitSensor* pReceiver, HitSensor* pSender) {
    if (pReceiver == nullptr) {
        return;
    }

    if (MR::isDead(pReceiver->mHost)) {
        return;
    }

    pReceiver->receiveMessage(ACTMES_WALL_TOUCH, pSender);

    if (!mMario->getMovementStates()._F) {
        return;
    }

    pReceiver->receiveMessage(ACTMES_TORNADO_ATTACK, pSender);
}

bool MarioActor::sendMsgUpperPunch(HitSensor* pSensor) {
    if (!isActionOk("アッパーパンチ")) {
        return false;
    }

    if (pSensor != nullptr && !MR::isDead(pSensor->mHost) && pSensor->receiveMessage(ACTMES_PLAYER_UPPER_PUNCH, getSensor("body"))) {
        playSound("声蹴り", -1);

        if (!mMario->isSwimming()) {
            changeAnimation("アッパーパンチ", nullptr);
        }

        return true;
    }

    return false;
}
