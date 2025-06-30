#include "Game/Enemy/BombTeresa.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/System/NerveExecutor.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointController.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "revolution/types.h"

namespace NrvBombTeresa {
    NEW_NERVE(BombTeresaNrvAppear, BombTeresa, Appear);
    NEW_NERVE(BombTeresaNrvShadowAppear, BombTeresa, ShadowAppear);
    NEW_NERVE(BombTeresaNrvBallAppear, BombTeresa, BallAppear);
    NEW_NERVE(BombTeresaNrvWait, BombTeresa, Wait);
    NEW_NERVE(BombTeresaNrvWander, BombTeresa, Wander);
    NEW_NERVE(BombTeresaNrvChase, BombTeresa, Chase);
    NEW_NERVE(BombTeresaNrvAttackTongueFailed, BombTeresa, AttackTongueFailed);
    NEW_NERVE(BombTeresaNrvDirectTackleSign, BombTeresa, DirectTackleSign);
    NEW_NERVE(BombTeresaNrvDirectTackle, BombTeresa, DirectTackle);
    NEW_NERVE(BombTeresaNrvDrift, BombTeresa, Drift);
    NEW_NERVE(BombTeresaNrvDriftRelease, BombTeresa, DriftRelease);
    NEW_NERVE_ONEND(BombTeresaNrvBindStarPointer, BombTeresa, BindStarPointer, BindStarPointer);
    NEW_NERVE(BombTeresaNrvExplosion, BombTeresa, Explosion);
    NEW_NERVE(BombTeresaNrvDisperse, BombTeresa, Disperse);
    NEW_NERVE(BombTeresaNrvShock, BombTeresa, Shock);
    NEW_NERVE(BombTeresaNrvReadyRestart, BombTeresa, ReadyRestart);
}

BombTeresa::BombTeresa(const char* pName) 
    : LiveActor(pName),
      mJointDelegator(nullptr),
      mJointDelegator2(nullptr),
      mScaleController(nullptr),
      mBindStarPointer(nullptr),
      _9C(0.0f, 0.0f, 0.0f, 1.0f),
      _AC(0 ,0 ,1),
      _B8(0, 0, 0),
      _C4(0, 0, 0),
      _D0(0, 0, 0),
      _DC(1.0f),
      _E0(1.0f),
      _E4(1.0f),
      _E8(0),
      _EC(false),
      mDisableRespawning(false),
      _EE(true),
      _EF(false)
{
}

BombTeresa::~BombTeresa() {}

void BombTeresa::init(const JMapInfoIter & rIter) {
    initFromJMapParam(rIter);
    initModelManagerWithAnm("BombTeresa", 0, 0);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    initSensor();
    f32 scale = mScale.x;
    scale = scale *  70.0f;
    initBinder(scale, 0.0f, 0);
    initEffectKeeper(1, nullptr, false);
    initSound(8, false);
    MR::initShadowVolumeSphere(this, 80.0f);
    MR::onShadowFollowHostScale(this, nullptr);
    initTongue();
    initNerve(&NrvBombTeresa::BombTeresaNrvAppear::sInstance);
    mScaleController = new AnimScaleController(nullptr);
    mBindStarPointer = new WalkerStateBindStarPointer(this, mScaleController);
    if (MR::isValidInfo(rIter)) {
        if (MR::useStageSwitchReadAppear(this, rIter)) {
            MR::syncStageSwitchAppear(this);
            makeActorDead();
            return;
        }
        makeActorAppeared();
        return;
    }
    makeActorDead();
}

void BombTeresa::initAfterPlacement() {
    MR::trySetMoveLimitCollision(this);
}

//The 2.0 needs to be 1.0
void BombTeresa::initFromJMapParam(const JMapInfoIter & rIter) {
    if (MR::isValidInfo(rIter)) {
        MR::initDefaultPos(this, rIter);
        applyScale(1.0f);
        MR::makeQuatAndFrontFromRotate(&_9C, &_AC, this);
        _D0 = mPosition;
        bool Arg1Bool;
        MR::getJMapInfoArg1WithInit(rIter, &Arg1Bool);
        mDisableRespawning = Arg1Bool == false;
        MR::useStageSwitchWriteDead(this, rIter);
    }
}


void BombTeresa::initTongue() {
    mJointDelegator = new JointControlDelegator<BombTeresa>(this, &BombTeresa::rootTongueMtxCallBack, &BombTeresa::rootTongueMtxCallBack);
    MR::setJointControllerParam(mJointDelegator, this, "Tongue1");
    mJointDelegator2 = new JointControlDelegator<BombTeresa>(this, &BombTeresa::endTongueMtxCallBack, &BombTeresa::rootTongueMtxCallBack);
    MR::setJointControllerParam(mJointDelegator, this, "Tongue2");
}


void BombTeresa::initSensor() {
    f32 mScaleXTemp = mScale.x;
    initHitSensor(2);
    MR::addHitSensorPosEye(this, "tungue", 8, (mScaleXTemp * 20.0f), &_C4, TVec3f(0.0f,0.0f,0.0f));
    MR::addHitSensorEnemy(this, "body", 8, (mScaleXTemp * 85.0f), TVec3f(0.0f,0.0f,0.0f));
    MR::initStarPointerTarget(this, (mScaleXTemp * 85.0f), TVec3f(0.0f,0.0f,0.0f));
}

void BombTeresa::makeActorAppeared() {
    MR::showModel(this);
    LiveActor::makeActorAppeared();
    MR::onCalcGravity(this);
    MR::invalidateClipping(this);
    MR::startBrk(this, "Normal");
    mScale.set(1.0f,1.0f,1.0f);
    _C4 = mPosition;
    _DC = 0.0f;
    setNerve(&NrvBombTeresa::BombTeresaNrvAppear::sInstance);
}

void BombTeresa::control() {
    if (isNerve(&NrvBombTeresa::BombTeresaNrvWait::sInstance)
    || isNerve(&NrvBombTeresa::BombTeresaNrvWander::sInstance)
    || isNerve(&NrvBombTeresa::BombTeresaNrvChase::sInstance)) {
        MR::startLevelSound(this, "SE_EM_LV_BOMBTERE_MOVE", -1, -1, -1);
    }
    mScaleController->updateNerve();
    if (isEnablePointBind()) {
        if (mBindStarPointer->tryStartPointBind()) {
            setNerve(&NrvBombTeresa::BombTeresaNrvBindStarPointer::sInstance);
        }
    }
}

void BombTeresa::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _9C);
    TVec3f scale;
    //scale.x = 
    MR::setBaseScale(this, scale);
    mJointDelegator->registerCallBack();
    mJointDelegator2->registerCallBack();
}

void BombTeresa::updateNormalVelocity() {
    MR::addVelocityKeepHeightUseShadow(this, 150.0f, 0.4f, 40.0f, nullptr);
    MR::attenuateVelocity(this, 0.9f);
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    MR::blendQuatUpFront(&_9C, -mGravity, _AC, 0.1f, 0.6f);
}

void BombTeresa::updateDriftReleaseVelocity() {
    MR::addVelocityKeepHeightUseShadow(this, 150.0f, 0.4f, 40.0f, nullptr);
    MR::attenuateVelocity(this, 0.9f);
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    MR::blendQuatUpFront(&_9C, -mGravity, _AC, 0.1f, 0.6f);
}

void BombTeresa::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorEye(pSender) && MR::isSensorPlayer(pReceiver)) {
        if (isTouchTongue() && !MR::sendArbitraryMsg(70, pReceiver, pSender)) {
            setNerve(&NrvBombTeresa::BombTeresaNrvAttackTongueFailed::sInstance);
        }
    } else if (MR::isSensorEnemy(pSender)) {
        if (MR::isSensorPlayer(pReceiver)) {
            if (isEnableHitExplosionToPlayer() && MR::sendMsgEnemyAttackExplosion(pReceiver, pSender)) {
                MR::zeroVelocity(this);
                setNerve(&NrvBombTeresa::BombTeresaNrvExplosion::sInstance);
            }
        } else if (isEnableHitExplosionToEnemy()) {
            if (MR::sendMsgEnemyAttackExplosion(pReceiver, pSender)) {
                MR::tryRumbleDefaultHit(this, 0);
                MR::zeroVelocity(this);
                setNerve(&NrvBombTeresa::BombTeresaNrvExplosion::sInstance);
                return;
            }
            MR::sendMsgPush(pReceiver, pSender);
        } else {
            MR::sendMsgPush(pReceiver, pSender);
        }
    }
}

bool BombTeresa::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorEnemy(pSender)) return false;
        if (MR::isSensorEnemy(pReceiver)) {
            TVec3f* sensorPos1 = MR::getSensorPos(pReceiver);
            TVec3f* sensorPos2 = MR::getSensorPos(pSender);
            sensorPos2->sub(*sensorPos1);
            if (MR::normalizeOrZero(sensorPos2)) {
                MR::getRandomVector(sensorPos2, 1.0);
                MR::normalizeOrZero(sensorPos2);
            }

            mVelocity += mPosition;
            return true;
        }
        return false;
}

bool BombTeresa::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReciever) {
    if (MR::isMsgStarPieceAttack(msg) && isEnableStarPieceAttack()) {
        setNerve(&NrvBombTeresa::BombTeresaNrvDisperse::sInstance);
        return true;
    }
    if (MR::isMsgPlayerSpinAttack(msg)) {
        return requestDrift();
    }
    if (MR::isMsgInvincibleAttack(msg) && isEnableShockWave()) {
        setNerve(&NrvBombTeresa::BombTeresaNrvDisperse::sInstance);
        return true;
    }
    return false;
}

bool BombTeresa::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReciever) {
    if (MR::isMsgToEnemyAttackBlow(msg) && isEnableHitExplosion()) {
        MR::zeroVelocity(this);
        setNerve(&NrvBombTeresa::BombTeresaNrvDisperse::sInstance);
        return false;
    } else if (MR::isMsgToEnemyAttackShockWave(msg) && isEnableShockWave()) {
        TVec3f uVar1;
        MR::calcSensorDirectionNormalize(&uVar1, pSender, pReciever);
        TVec3f* v16(&uVar1);
        v16->mult(20.0f);
        v16->set<f32>(mVelocity);
        setNerve(&NrvBombTeresa::BombTeresaNrvShock::sInstance);
        return false;
    } else if (MR::isMsgExplosionAttack(msg) && isEnableHitExplosion()) {
        MR::zeroVelocity(this);
        setNerve(&NrvBombTeresa::BombTeresaNrvExplosion::sInstance);
        return true;
    }
    return false;
}

bool BombTeresa::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReciever) {
    if (msg == 171 && isEnableDisperse()) {
        MR::zeroVelocity(this);
        setNerve(&NrvBombTeresa::BombTeresaNrvDisperse::sInstance);
        return true;
    } else if (MR::isMsgSpinStormRange(msg)) {
        return requestDrift();
    }
    return false;
}

bool BombTeresa::requestDrift() {
    if (isEnableDrift()) {
        MR::invalidateClipping(this);
        setNerve(&NrvBombTeresa::BombTeresaNrvDrift::sInstance);
        return true;
    }   
    return false;
}

bool BombTeresa::tryCheseEnd() {
    if (MR::isGreaterStep(this, 240) || MR::isPlayerElementModeTeresa() || MR::isNearPlayer(this, 2000.0f)) {
        setNerve(&NrvBombTeresa::BombTeresaNrvWait::sInstance);
        return true;
    }
    return false;
}

bool BombTeresa::tryDirectTackle() {
    if (!MR::isPlayerDamaging() && MR::isNearPlayer(this,900.0f) && MR::isFaceToPlayerHorizontalDegree(this, _AC, 30.0f)) {
        setNerve(&NrvBombTeresa::BombTeresaNrvDirectTackleSign::sInstance);
        return true;
    }
    return false;
}

bool BombTeresa::tryAbortDrift() {
    if (MR::isExistMapCollision(mPosition, -_C4)) {
        setNerve(&NrvBombTeresa::BombTeresaNrvAttackTongueFailed::sInstance);
        return true;
    }
    return false;
}

bool BombTeresa::tryDriftRelease() {
    if (_EC) {
        setNerve(&NrvBombTeresa::BombTeresaNrvDriftRelease::sInstance);
        return true;
    }
    return false;
}

bool BombTeresa::tryDriftReleaseEnd() {
    if (MR::isGreaterStep(this, 60)) {
        MR::deleteEffect(this, "SpinBlur");
        setNerve(&NrvBombTeresa::BombTeresaNrvWait::sInstance);
        return true;
    }
    return false;
}

bool BombTeresa::tryExplosion() {
    if (MR::isBinded(this)) {
        if (MR::sendMsgEnemyAttackExplosionToBindedSensor(this, getSensor("body"))) {
            MR::zeroVelocity(this);
            setNerve(&NrvBombTeresa::BombTeresaNrvExplosion::sInstance);
            MR::forceDeleteEffect(this, "SpinBlur");
            return true;
        }
    }
    return false;
}

bool BombTeresa::tryRevival() {
    MR::startBrk(this, "Normal");
    if (mDisableRespawning) {
        MR::invalidateHitSensors(this);
        setNerve(&NrvBombTeresa::BombTeresaNrvReadyRestart::sInstance);
        return true;
    }
    kill();
    return false;
}

void BombTeresa::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::validateHitSensor(this, "body");
        MR::validateHitSensor(this, "tungue");
        MR::showModel(this);
        MR::startAction(this, "Appear");
        MR::emitEffect(this, "Appear");
        MR::startSound(this, "SE_EM_BOMBTERE_APPEAR", -1, -1);
        _DC = 0.0f;
    }
    _C4 = mPosition;
    updateNormalVelocity();
    if (MR::isActionEnd(this)) {
        setNerve(&NrvBombTeresa::BombTeresaNrvWait::sInstance);
    }
}

void BombTeresa::exeShadowAppear() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Shadow");
        MR::hideModelAndOnCalcAnim(this);
        MR::startAction(this, "ShadowAppear");
    }
    if (!MR::isBindedGround(this)) {
        MR::addVelocityToGravity(this, 1.0f);
    }
    f32 v2;
    if (MR::isLessStep(this, 130)) {
        v2 = 0.995f;
    } else {
        v2 = 0.8f;
    }
    MR::attenuateVelocity(this, v2);
    MR::turnDirectionToPlayerDegree(this, &_AC, 3.0f);
    _C4 = mPosition;
    MR::blendQuatUpFront(&_9C, -mGravity, _AC, 0.1f, 0.6f);
    if (MR::isGreaterStep(this, 120)) {
        MR::deleteEffect(this, "Shadow");
        setNerve(&NrvBombTeresa::BombTeresaNrvAppear::sInstance);
    }

}

void BombTeresa::exeBallAppear() {
    if (MR::isFirstStep(this)) {
        MR::validateHitSensor(this, "body");
        MR::validateHitSensor(this, "tungue");
        MR::emitEffect(this, "Ball");
        MR::startAction(this, "BallAppear");
        MR::showModel(this);
        _DC = 0.0f;
    }
    _C4 = mPosition;
    MR::addVelocityToGravity(this, 1.0f);
    bool v2 = MR::reboundVelocityFromCollision(this, 0.95f, 2.0f, 1.0f);
    MR::attenuateVelocity(this, 0.99f);
    if (v2) {
        MR::startSound(this, "SE_EM_BOMBTERE_BOUND", -1, -1);
    }
    if (MR::isLessStep(this, 160)) {
        MR::rotateQuatRollBall(&_9C, mVelocity, -mGravity, 70.0f);
        _AC.set<f32>((2.0f * _9C.y * _9C.x) + (2.0f * _9C.y * _9C.z), 
            (2.0f * _9C.z * _9C.x) - (2.0f * _9C.y * _9C.y), 
            (1.0f - (2.0f * _9C.y * _9C.y) - (2.0f * _9C.z * _9C.z)));
    } else {
        f32 v3 = mGravity.dot(_AC);
        JMAVECScaleAdd(&mGravity, &_AC, &_AC, -v3);
        if (MR::normalizeOrZero(&_AC)) {
            MR::makeAxisVerticalZX(&_AC, mGravity);
        }
        MR::blendQuatUpFront(&_9C, -mGravity, _AC, 0.1f, 0.1f);
    }
    if (MR::isGreaterStep(this, 70)) {
        MR::deleteEffect(this, "Ball");
        MR::emitEffect(this, "Appear");
        MR::startSound(this, "SE_EM_BOMBTERE_APPEAR", -1, -1);
        MR::showModel(this);
        setNerve(&NrvBombTeresa::BombTeresaNrvDirectTackleSign::sInstance);
    }
}

inline void BombTeresa::exeOnEndBindStarPointer() {
    mBindStarPointer->kill();
}

inline void BombTeresa::exeBindStarPointer() {
    MR::updateActorStateAndNextNerve(this, mBindStarPointer, &NrvBombTeresa::BombTeresaNrvWait::sInstance);
}
