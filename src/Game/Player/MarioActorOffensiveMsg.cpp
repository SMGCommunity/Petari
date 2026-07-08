#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioMessenger.hpp"
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
static f32 mSensorRadiusTornadoStorm = 600.0f;
static f32 mSensorRadiusSpinPull = 1000.0f;
static f32 mSensorRadiusSpinPullOnGround = 450.0f;
static f32 mUnk1 = 450.0f;

// void MarioActor::attackOrPushSensor(HitSensor*, f32);

void MarioActor::attackOrPushSensorInDamage(HitSensor* pReceiver, f32 radius) {
    f32 sensorRadius = pReceiver->mRadius;

    if (!isEnableMoveMario()) {
        return;
    }

    if (radius > sensorRadius + mSensorRadiusAttack) {
        bool b1 = false;
        f32 f1 = _4B4;
        f32 f2 = 20.0f + _4B0;

        if (isJumping()) {
            f1 += 10.0f;
        }

        if (mMario->_71C >= 5) {
            f2 += 15.0f;
        }

        f32 f3 = pReceiver->mRadius;

        if (cylinderPushCheck(pReceiver->mPosition - _2A0, f3, f2, f1)) {
            if (tryGetItem(pReceiver)) {
                return;
            }

            // touchSensor(pReceiver);
            MR::sendMsgPush(pReceiver, getSensor("dummy"));
            b1 = true;
        }

        // addRushSensor(pReceiver, b1);
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

    if (mMario->_71C >= 5) {
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
        if (cylinderPushCheck(pSensor->mPosition - _2A0, sensorRadius, 20.0f + _4B0, _4B4)) {
            if (tryGetItem(pSensor)) {
                return;
            }

            sendMsgToSensor(pSensor, ACTMES_RUSH_PLAYER_TOUCH);
        }

        // addRushSensor(pSensor, false)
    }

    if (radius < sensorRadius + mSensorRadiusAttack) {
        tryAddClapCoin(pSensor);
    }

    // trySetLockOnTarget(pSensor);
}

void MarioActor::tryAddClapCoin(HitSensor* pSensor) {
    if (_7DC != 64 && pSensor->isType(ACTMES_ENEMY_ATTACK_FLIP_VERYWEAK)) {
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
    if (mAlphaEnable) {
        down = _240;
    } else {
        MR::calcGravityVectorOrZero(this, pSensor->mPosition, &down, _36C, 0);
    }

    if (down.dot(mPosition - pSensor->mPosition) <= 0.0f && mMario->_424 == 0) {
        return true;
    }

    return false;
}

bool MarioActor::tryHipDropAttack(HitSensor* pSensor) {
    if (isUnderTarget(pSensor) && cylinderHorizontalCheck(pSensor)) {
        if (pSensor->isType(ACTMES_TAKEN)) {
            return tryTrampleAttack(pSensor);
        }

        return pSensor->receiveMessage(ACTMES_PLAYER_HIP_DROP, getSensor("body"));
    }

    return false;
}

// bool MarioActor::checkAndTryTrampleAttack(HitSensor*, f32, bool);

bool MarioActor::tryTrampleAttack(HitSensor* pSensor) {
    if (_FCD) {
        return false;
    }

    if (mMario->isStatusActive(4)) {
        return false;
    }

    bool isUnder = isUnderTarget(pSensor);
    bool isHorizontalCheck = cylinderHorizontalCheck(pSensor);
    if (isUnder && isHorizontalCheck) {
        _FCD = true;

        bool isReceiveMessage = pSensor->receiveMessage(ACTMES_PLAYER_TRAMPLE, getSensor("body"));

        _FCD = false;

        if (isReceiveMessage) {
            // doTrampleJump(pSensor);
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

// bool MarioActor::tryGetItem(HitSensor* pSensor);

// bool MarioActor::cylinderPushCheck(const TVec3f&, f32, f32, f32);

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
        if (_3E8 && mMario->getMovementStates()._1) {
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

    if (pSensor != nullptr && !MR::isDead(pSensor->mHost) && pSensor->receiveMessage(MR::MovementType_SensorHitChecker, getSensor("body"))) {
        playSound("声蹴り", -1);

        if (!mMario->isSwimming()) {
            changeAnimation("アッパーパンチ", nullptr);
        }

        return true;
    }

    return false;
}
