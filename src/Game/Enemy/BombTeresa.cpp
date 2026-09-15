#include "Game/Enemy/BombTeresa.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/System/NerveExecutor.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointController.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "revolution/mtx.h"
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
};  // namespace NrvBombTeresa

BombTeresa::BombTeresa(const char* pName)
    : LiveActor(pName), mJointDelegator(nullptr), mJointDelegator2(nullptr), mScaleController(nullptr), mBindStarPointer(nullptr),
      _9C(0.0f, 0.0f, 0.0f, 1.0f), _AC(0, 0, 1), _B8(0, 0, 0), _C4(0, 0, 0), _D0(0, 0, 0), _DC(1.0f), _E0(1.0f), _E4(1.0f), _E8(0), _EC(false),
      mDisableRespawning(false), _EE(true), _EF(false) {
}

BombTeresa::~BombTeresa() {
}

void BombTeresa::init(const JMapInfoIter& rIter) {
    initFromJMapParam(rIter);
    initModelManagerWithAnm("BombTeresa", 0, 0);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    initSensor();
    f32 scale = mScale.x;
    initBinder(scale * 70.0f, 0.0f, 0);
    initEffectKeeper(1, nullptr, false);
    initSound(8, false);
    MR::initShadowVolumeSphere(this, 80.0f);
    MR::onShadowFollowHostScale(this, nullptr);
    initTongue();
    initNerve(GET_NERVE(BombTeresa, BombTeresaNrvAppear));
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

void BombTeresa::initFromJMapParam(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        MR::initDefaultPos(this, rIter);
        mScale.mult(1.0f);
        MR::makeQuatAndFrontFromRotate(&_9C, &_AC, this);
        _D0 = mPosition;
        bool arg1Bool;
        MR::getJMapInfoArg1WithInit(rIter, &arg1Bool);
        mDisableRespawning = arg1Bool == false;
        MR::useStageSwitchWriteDead(this, rIter);
    }
}

void BombTeresa::initTongue() {
    mJointDelegator = new JointControlDelegator< BombTeresa >(this, &BombTeresa::rootTongueMtxCallBack, &BombTeresa::rootTongueMtxCallBack);
    MR::setJointControllerParam(mJointDelegator, this, "Tongue1");
    mJointDelegator2 = new JointControlDelegator< BombTeresa >(this, &BombTeresa::endTongueMtxCallBack, &BombTeresa::rootTongueMtxCallBack);
    MR::setJointControllerParam(mJointDelegator, this, "Tongue2");
}

void BombTeresa::initSensor() {
    f32 mScaleXTemp = mScale.x;
    initHitSensor(2);
    MR::addHitSensorPosEye(this, "tungue", 8, (mScaleXTemp * 20.0f), &_C4, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorEnemy(this, "body", 8, (mScaleXTemp * 85.0f), TVec3f(0.0f, 0.0f, 0.0f));
    MR::initStarPointerTarget(this, (mScaleXTemp * 85.0f), TVec3f(0.0f, 0.0f, 0.0f));
}

void BombTeresa::makeActorAppeared() {
    MR::showModel(this);
    LiveActor::makeActorAppeared();
    MR::onCalcGravity(this);
    MR::invalidateClipping(this);
    MR::startBrk(this, "Normal");
    mScale.set(1.0f, 1.0f, 1.0f);
    _C4 = mPosition;
    _DC = 0.0f;
    setNerve(GET_NERVE(BombTeresa, BombTeresaNrvAppear));
}

void BombTeresa::control() {
    if (isNerve(GET_NERVE(BombTeresa, BombTeresaNrvWait)) || isNerve(GET_NERVE(BombTeresa, BombTeresaNrvWander)) ||
        isNerve(GET_NERVE(BombTeresa, BombTeresaNrvChase))) {
        MR::startLevelSound(this, "SE_EM_LV_BOMBTERE_MOVE");
    }
    mScaleController->updateNerve();
    if (isEnablePointBind()) {
        if (mBindStarPointer->tryStartPointBind()) {
            setNerve(GET_NERVE(BombTeresa, BombTeresaNrvBindStarPointer));
        }
    }
}

void BombTeresa::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _9C);
    TVec3f scale = mScaleController->_C * mScale;
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

bool BombTeresa::rootTongueMtxCallBack(TPos3f* arg0, const JointControllerInfo& arg1) {
    if (_DC == 0.0f) {
        return false;
    }
    TVec3f pos;
    TVec3f tonguePos;
    TVec3f side;
    arg0->getTrans(pos);
    MR::copyJointPos(this, "Tongue2", &tonguePos);
    arg0->getXDir(side);
    TVec3f dir = tonguePos - pos;
    if (MR::normalizeOrZero(&dir)) {
        dir.set(side);
    }
    MR::turnMtxToXDirRate(arg0, dir, (0.5f * _DC));
    f32 dist = MR::getEaseInOutValue(MR::normalize(pos.distance(tonguePos), 400.0f, 1000.0f), 1.0f, 0.5f, 1.0f);
    MR::preScaleMtx(arg0->toMtxPtr(), TVec3f(1.0f, dist, dist));
    return true;
}

bool BombTeresa::endTongueMtxCallBack(TPos3f* arg0, const JointControllerInfo& arg1) {
    if (_DC == 0.0f) {
        return false;
    }
    TVec3f pos;
    TVec3f tonguePos;
    TVec3f side;
    arg0->getTrans(pos);
    MR::copyJointPos(this, "Tongue1", &tonguePos);
    MR::vecBlend(pos, _C4, &pos, _DC);
    arg0->getXDir(side);
    TVec3f dir = pos - tonguePos;
    if (MR::normalizeOrZero(&dir)) {
        dir.set(side);
    }
    MR::turnMtxToXDirRate(arg0, dir, _DC);
    TVec3f side2;
    arg0->getXDir(side2);
    pos += side2 * -35.0f * _DC;
    arg0->setTrans(pos);
    f32 dist = MR::getEaseInOutValue(MR::normalize(pos.distance(tonguePos), 400.0f, 1000.0f), 1.0f, 0.5f, 1.0f);
    MR::preScaleMtx(arg0->toMtxPtr(), TVec3f(1.0f, dist, dist));
    return true;
}

void BombTeresa::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorEye(pSender) && MR::isSensorPlayer(pReceiver)) {
        if (isTouchTongue() && !MR::sendArbitraryMsg(ACTMES_BOMB_TERESA_TONGUE_TOUCH, pReceiver, pSender)) {
            setNerve(GET_NERVE(BombTeresa, BombTeresaNrvAttackTongueFailed));
        }
    } else if (MR::isSensorEnemy(pSender)) {
        if (MR::isSensorPlayer(pReceiver)) {
            if (isEnableHitExplosionToPlayer() && MR::sendMsgEnemyAttackExplosion(pReceiver, pSender)) {
                MR::zeroVelocity(this);
                setNerve(GET_NERVE(BombTeresa, BombTeresaNrvExplosion));
            }
        } else if (isEnableHitExplosionToEnemy()) {
            if (MR::sendMsgEnemyAttackExplosion(pReceiver, pSender)) {
                MR::tryRumbleDefaultHit(this, WPAD_CHAN0);
                MR::zeroVelocity(this);
                setNerve(GET_NERVE(BombTeresa, BombTeresaNrvExplosion));
            } else {
                MR::sendMsgPush(pReceiver, pSender);
            }
        } else {
            MR::sendMsgPush(pReceiver, pSender);
        }
    }
}

bool BombTeresa::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorEnemy(pReceiver)) {
        return false;
    }

    if (MR::isSensorEnemy(pSender)) {
        TVec3f deltaPos = MR::getSensorPos(pReceiver) - MR::getSensorPos(pSender);

        if (MR::normalizeOrZero(&deltaPos)) {
            MR::getRandomVector(&deltaPos, 1.0);
            MR::normalizeOrZero(&deltaPos);
        }

        mVelocity += deltaPos * 3.0f;
        return true;
    }

    return false;
}

bool BombTeresa::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceAttack(msg) && isEnableStarPieceAttack()) {
        setNerve(GET_NERVE(BombTeresa, BombTeresaNrvDisperse));
        return true;
    }

    if (MR::isMsgPlayerSpinAttack(msg)) {
        return requestDrift();
    }

    if (MR::isMsgInvincibleAttack(msg) && isEnableShockWave()) {
        setNerve(GET_NERVE(BombTeresa, BombTeresaNrvDisperse));
        return true;
    }

    return false;
}

bool BombTeresa::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgToEnemyAttackBlow(msg) && isEnableHitExplosion()) {
        MR::zeroVelocity(this);
        setNerve(GET_NERVE(BombTeresa, BombTeresaNrvDisperse));
        return false;
    }

    if (MR::isMsgToEnemyAttackShockWave(msg) && isEnableShockWave()) {
        TVec3f uVar1;
        MR::calcSensorDirectionNormalize(&uVar1, pSender, pReceiver);
        TVec3f v16(uVar1);
        v16 *= 20.0f;
        mVelocity.set< f32 >(v16);
        setNerve(GET_NERVE(BombTeresa, BombTeresaNrvShock));
        return false;
    }

    if (MR::isMsgExplosionAttack(msg) && isEnableHitExplosion()) {
        MR::zeroVelocity(this);
        setNerve(GET_NERVE(BombTeresa, BombTeresaNrvExplosion));
        return true;
    }

    return false;
}

bool BombTeresa::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_DISPERSE_BOMB_TERESA && isEnableDisperse()) {
        MR::zeroVelocity(this);
        setNerve(GET_NERVE(BombTeresa, BombTeresaNrvDisperse));
        return true;
    }

    if (MR::isMsgSpinStormRange(msg)) {
        return requestDrift();
    }

    return false;
}

bool BombTeresa::requestDrift() {
    if (isEnableDrift()) {
        MR::invalidateClipping(this);
        setNerve(GET_NERVE(BombTeresa, BombTeresaNrvDrift));
        return true;
    }
    return false;
}

bool BombTeresa::tryCheseEnd() {
    if (MR::isGreaterStep(this, 240) || MR::isPlayerElementModeTeresa() || isOutOfRange(2000.0f)) {
        setNerve(GET_NERVE(BombTeresa, BombTeresaNrvWait));
        return true;
    }
    return false;
}

bool BombTeresa::tryDirectTackle() {
    if (!MR::isPlayerDamaging() && MR::isNearPlayer(this, 900.0f) && MR::isFaceToPlayerHorizontalDegree(this, _AC, 30.0f)) {
        setNerve(GET_NERVE(BombTeresa, BombTeresaNrvDirectTackleSign));
        return true;
    }
    return false;
}

bool BombTeresa::tryAbortDrift() {
    if (MR::isExistMapCollision(mPosition, _C4 - mPosition)) {
        setNerve(GET_NERVE(BombTeresa, BombTeresaNrvAttackTongueFailed));
        return true;
    }
    return false;
}

bool BombTeresa::tryDriftRelease() {
    if (_EC) {
        setNerve(GET_NERVE(BombTeresa, BombTeresaNrvDriftRelease));
        return true;
    }
    return false;
}

bool BombTeresa::tryDriftReleaseEnd() {
    if (MR::isGreaterStep(this, 60)) {
        MR::deleteEffect(this, "SpinBlur");
        setNerve(GET_NERVE(BombTeresa, BombTeresaNrvWait));
        return true;
    }
    return false;
}

bool BombTeresa::tryExplosion() {
    if (MR::isBinded(this)) {
        if (MR::sendMsgEnemyAttackExplosionToBindedSensor(this, getSensor("body"))) {
            MR::zeroVelocity(this);
            setNerve(GET_NERVE(BombTeresa, BombTeresaNrvExplosion));
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
        setNerve(GET_NERVE(BombTeresa, BombTeresaNrvReadyRestart));
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
        MR::startSound(this, "SE_EM_BOMBTERE_APPEAR");
        _DC = 0.0f;
    }
    _C4 = mPosition;
    updateNormalVelocity();
    if (MR::isActionEnd(this)) {
        setNerve(GET_NERVE(BombTeresa, BombTeresaNrvWait));
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
        setNerve(GET_NERVE(BombTeresa, BombTeresaNrvAppear));
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
        MR::startSound(this, "SE_EM_BOMBTERE_BOUND");
    }
    if (MR::isLessStep(this, 160)) {
        MR::rotateQuatRollBall(&_9C, mVelocity, -mGravity, 70.0f);
        _9C.getZDir(_AC);
    } else {
        _AC.orthogonalize(mGravity);
        if (MR::normalizeOrZero(&_AC)) {
            MR::makeAxisVerticalZX(&_AC, mGravity);
        }
        MR::blendQuatUpFront(&_9C, -mGravity, _AC, 0.1f, 0.1f);
    }
    if (MR::isGreaterStep(this, 70)) {
        MR::deleteEffect(this, "Ball");
        MR::emitEffect(this, "Appear");
        MR::startSound(this, "SE_EM_BOMBTERE_APPEAR");
        MR::showModel(this);
        setNerve(GET_NERVE(BombTeresa, BombTeresaNrvDirectTackleSign));
    }
}

void BombTeresa::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
        MR::startAction(this, "Wait");
        MR::startBrk(this, "Normal");
        _DC = 0.0f;
    }
    _C4 = mPosition;
    updateNormalVelocity();
    if (MR::isInDeath(this, TVec3f(0.0f, 0.0f, 0.0f))) {
        tryRevival();
    } else if (MR::isNearPlayerAnyTime(this, 1800.0f)) {
        setNerve(GET_NERVE(BombTeresa, BombTeresaNrvChase));
    } else if (_EF) {
        if (MR::isGreaterStep(this, 60)) {
            setNerve(GET_NERVE(BombTeresa, BombTeresaNrvWander));
        }
    }
}

void BombTeresa::exeWander() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Wait");
        MR::startBrk(this, "Normal");
        _DC = 0.0f;
    }
    _C4 = mPosition;

    MR::rotateDirectionGravityDegree(this, &_AC, getNerveStep() % 600 < 400 ? -0.5f : 0.5f);
    MR::addVelocityMoveToDirection(this, _AC, 0.5f);
    updateNormalVelocity();
    if (MR::isInDeath(this, TVec3f(0.0f, 0.0f, 0.0f))) {
        tryRevival();
    } else if (MR::isNearPlayerAnyTime(this, 1800.0f)) {
        setNerve(GET_NERVE(BombTeresa, BombTeresaNrvChase));
    }
}

void BombTeresa::exeChase() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Wait");
        MR::startSound(this, "SE_EV_BOMBTERE_LAUGH");
        _DC = 0.0f;
    }
    MR::turnDirectionToPlayerDegree(this, &_AC, 1.4f);
    MR::addVelocityMoveToDirection(this, _AC, (0.9f * MR::calcNerveRate(this, 60)));
    _C4 = mPosition;
    updateNormalVelocity();
    if (!tryCheseEnd() && tryDirectTackle())
        return;
}

void BombTeresa::exeAttackTongueFailed() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Return");
    }
    MR::startLevelSound(this, "SE_EM_LV_BOMBTERE_TONGUE_BACK");
    _DC = MR::calcNerveEaseInOutValue(this, 0, 1.0f, 0.0f);
    updateNormalVelocity();
    if (MR::isGreaterStep(this, 40)) {
        setNerve(GET_NERVE(BombTeresa, BombTeresaNrvWait));
    }
}

void BombTeresa::exeDirectTackleSign() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_EV_BOMBTERE_TACKLE");
        MR::startAction(this, "TackleSign");
        _DC = 0.0f;
    }
    MR::turnDirectionToPlayerDegree(this, &_AC, 10.0f);
    MR::addVelocityKeepHeight(this, *MR::getPlayerCenterPos(), 100.0f, 6.0f, 100.0f);
    updateNormalVelocity();
    _C4 = mPosition;
    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(BombTeresa, BombTeresaNrvDirectTackle));
    }
}

void BombTeresa::exeDirectTackle() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Tackle");
        _DC = 0.0f;
    }
    MR::startLevelSound(this, "SE_EM_LV_BOMBTERE_TACKLE");
    MR::turnDirectionToPlayerDegree(this, &_AC, 0.8f);
    MR::addVelocityMoveToDirection(this, _AC, (1.5f * MR::calcNerveRate(this, 50)));
    MR::addVelocityKeepHeight(this, *MR::getPlayerCenterPos(), 100.0f, 6.0f, 100.0f);
    updateNormalVelocity();
    _C4 = mPosition;
    if (MR::isGreaterStep(this, 120)) {
        setNerve(GET_NERVE(BombTeresa, BombTeresaNrvWander));
    }
}

void BombTeresa::exeDrift() {
    TVec3f v14;
    TPos3f v16;
    TVec3f v15;
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Spin");
        MR::startBrk(this, "Normal");
        MR::emitEffect(this, "SpinBlur");
        MR::startSound(this, "SE_EM_BOMBTERE_TONGUE_CATCH");
        _EE = MR::isLeftSideFromPlayer(this) == false;
        _E8 = 40;
        _EC = false;
        _E0 = 7.0f;
        _E4 = 7.0f;
        _DC = 1.0f;
    }

    v14.cross(mGravity, _AC);
    v15.cross(mGravity, *MR::getPlayerPos() - MR::getCamPos());

    if (MR::isNearZero(v14)) {
        MR::makeAxisVerticalZX(&v14, mGravity);
    }
    if (MR::isNearZero(v15)) {
        MR::makeAxisVerticalZX(&v15, mGravity);
    }
    MR::normalize(&v14);
    MR::normalize(&v15);
    if (v14.dot(v15) >= 0.8f) {
        MR::startLevelSound(this, "SE_EM_LV_BOMBTERE_SWING");
    }

    MR::calcPlayerJointMtx(&v16, "HandR");
    TVec3f v13;
    v16.getTrans(v13);
    TVec3f v12;
    v16.getZDir(v12);
    if (!MR::normalizeOrZero(&v12)) {
        v13.scaleAdd(20.0f, v12, v13);
    }
    MR::vecBlend(_C4, v13, &_C4, MR::calcNerveEaseInRate(this, 15));
    MR::turnDirectionToTarget(this, &_AC, *MR::getPlayerPos(), 0.0f);
    if (_E8 <= 0) {
        if (MR::isPadSwing(WPAD_CHAN0)) {
            MR::tryRumblePadMiddle(this, WPAD_CHAN0);
            _E8 = 40;
            _E0 += 20.0f;
            _E4 += 20.0f;
            if (MR::isGreaterStep(this, 190)) {
                _EC = true;
            }
        }
    }
    addTeresaSpinPullVelocity(MR::calcNerveEaseInOutValue(this, 190, 240, 150.0f, 170.0f));
    updateNormalVelocity();
    if (MR::isStep(this, 190)) {
        MR::startBrk(this, "CountDown");
    }
    _E4 = MR::getInterpolateValue(0.15f, _E4, 7.0f);
    if (MR::isLessStep(this, 190)) {
        _E0 = MR::getInterpolateValue(0.15f, _E0, 7.0f);
    } else {
        if (_E0 > 7.0f) {
            _E0 = MR::getInterpolateValue(0.15f, _E0, 7.0f);
        }
        _E0 *= 0.992f;
    }
    _E8--;
    if (!tryExplosion() && !tryDriftRelease() && tryAbortDrift())
        return;
}

void BombTeresa::exeDriftRelease() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Drift");
        MR::tryRumblePadStrong(this, WPAD_CHAN0);
        MR::startSound(this, "SE_EM_BOMBTERE_THROW");
    }
    _DC = MR::calcNerveEaseInOutValue(this, 12, 1.0f, 0.0f);
    if (MR::isLessStep(this, 0)) {
        addTeresaSpinPullVelocity(150.0f);
        updateNormalVelocity();
    } else {
        updateDriftReleaseVelocity();
    }
    if (!tryExplosion() && tryDriftReleaseEnd())
        return;
}

void BombTeresa::exeBindStarPointer() {
    MR::updateActorStateAndNextNerve(this, mBindStarPointer, GET_NERVE(BombTeresa, BombTeresaNrvWait));
}

void BombTeresa::endBindStarPointer() {
    mBindStarPointer->kill();
}

void BombTeresa::exeExplosion() {
    if (MR::isFirstStep(this)) {
        MR::shakeCameraNormalStrong();
        MR::tryRumblePadStrong(this, WPAD_CHAN0);
        MR::deleteEffect(this, "SpinBlur");
        MR::emitEffect(this, "Explosion");
        MR::hideModel(this);
        MR::startSound(this, "SE_EM_BOMBTERE_EXPLOSION");
        _DC = 0.0f;
        MR::zeroVelocity(this);
    }
    _C4 = mPosition;
    if (MR::isGreaterStep(this, 20)) {
        tryRevival();
    }
}

void BombTeresa::exeShock() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "SpinBlur");
        _DC = 0.0f;
    }
    _C4 = mPosition;
    MR::attenuateVelocity(this, 0.98f);
    if (MR::isGreaterStep(this, 20)) {
        MR::deleteEffect(this, "SpinBlur");
        MR::emitEffect(this, "MisFire");
        MR::startSound(this, "SE_EM_BOMBTERE_HIDE");
        MR::hideModel(this);
        tryRevival();
    }
}

void BombTeresa::exeDisperse() {
    if (MR::isFirstStep(this)) {
        MR::deleteEffect(this, "SpinBlur");
        MR::emitEffect(this, "MisFire");
        MR::startSound(this, "SE_EM_BOMBTERE_HIDE");
        MR::hideModel(this);
        _DC = 0.0f;
        MR::zeroVelocity(this);
    }
    _C4 = mPosition;
    if (MR::isGreaterStep(this, 40)) {
        tryRevival();
    }
}

void BombTeresa::exeReadyRestart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Hide");
        MR::resetPosition(this, _D0);
        MR::makeQuatAndFrontFromRotate(&_9C, &_AC, this);
        MR::zeroVelocity(this);
    }
    _C4 = mPosition;
    if (MR::isGreaterStep(this, 120)) {
        setNerve(GET_NERVE(BombTeresa, BombTeresaNrvAppear));
    }
}

bool BombTeresa::isTouchTongue() const {
    return isNerve(GET_NERVE(BombTeresa, BombTeresaNrvDrift));
}

bool BombTeresa::isEnableDrift() const {
    if (isNerve(GET_NERVE(BombTeresa, BombTeresaNrvBallAppear)) || isNerve(GET_NERVE(BombTeresa, BombTeresaNrvShadowAppear)) ||
        isNerve(GET_NERVE(BombTeresa, BombTeresaNrvDrift)) || isNerve(GET_NERVE(BombTeresa, BombTeresaNrvDriftRelease)) ||
        isNerve(GET_NERVE(BombTeresa, BombTeresaNrvExplosion)) || isNerve(GET_NERVE(BombTeresa, BombTeresaNrvShock)) ||
        isNerve(GET_NERVE(BombTeresa, BombTeresaNrvDisperse)) || isNerve(GET_NERVE(BombTeresa, BombTeresaNrvReadyRestart))) {
        return false;
    }
    return true;
}

bool BombTeresa::isEnablePointBind() const {
    if (isNerve(GET_NERVE(BombTeresa, BombTeresaNrvWait)) || isNerve(GET_NERVE(BombTeresa, BombTeresaNrvWander)) ||
        isNerve(GET_NERVE(BombTeresa, BombTeresaNrvChase)) || isNerve(GET_NERVE(BombTeresa, BombTeresaNrvAttackTongueFailed)) ||
        isNerve(GET_NERVE(BombTeresa, BombTeresaNrvDirectTackleSign)) || isNerve(GET_NERVE(BombTeresa, BombTeresaNrvDirectTackle)) ||
        isNerve(GET_NERVE(BombTeresa, BombTeresaNrvDriftRelease))) {
        return true;
    }
    return false;
}

bool BombTeresa::isEnableDisperse() const {
    if (isNerve(GET_NERVE(BombTeresa, BombTeresaNrvDrift))) {
        return false;
    }
    return isEnableDrift();
}

bool BombTeresa::isEnableHitExplosionToPlayer() const {
    if (isNerve(GET_NERVE(BombTeresa, BombTeresaNrvDrift))) {
        return MR::isGreaterStep(this, 190);
    }
    return isEnableHitExplosion();
}

bool BombTeresa::isEnableHitExplosionToEnemy() const {
    if (isNerve(GET_NERVE(BombTeresa, BombTeresaNrvDrift)) || isNerve(GET_NERVE(BombTeresa, BombTeresaNrvDriftRelease))) {
        return true;
    }
    return false;
}

bool BombTeresa::isEnableHitExplosion() const {
    if (isNerve(GET_NERVE(BombTeresa, BombTeresaNrvBallAppear)) || isNerve(GET_NERVE(BombTeresa, BombTeresaNrvShadowAppear)) ||
        isNerve(GET_NERVE(BombTeresa, BombTeresaNrvExplosion)) || isNerve(GET_NERVE(BombTeresa, BombTeresaNrvShock)) ||
        isNerve(GET_NERVE(BombTeresa, BombTeresaNrvDisperse)) || isNerve(GET_NERVE(BombTeresa, BombTeresaNrvDrift)) ||
        isNerve(GET_NERVE(BombTeresa, BombTeresaNrvReadyRestart))) {
        return false;
    }
    return true;
}

bool BombTeresa::isEnableStarPieceAttack() const {
    if (isNerve(GET_NERVE(BombTeresa, BombTeresaNrvBallAppear)) || isNerve(GET_NERVE(BombTeresa, BombTeresaNrvShadowAppear)) ||
        isNerve(GET_NERVE(BombTeresa, BombTeresaNrvExplosion)) || isNerve(GET_NERVE(BombTeresa, BombTeresaNrvShock)) ||
        isNerve(GET_NERVE(BombTeresa, BombTeresaNrvDisperse)) || isNerve(GET_NERVE(BombTeresa, BombTeresaNrvDrift)) ||
        isNerve(GET_NERVE(BombTeresa, BombTeresaNrvReadyRestart))) {
        return false;
    }
    return true;
}

bool BombTeresa::isEnableShockWave() const {
    if (isNerve(GET_NERVE(BombTeresa, BombTeresaNrvExplosion)) || isNerve(GET_NERVE(BombTeresa, BombTeresaNrvShock)) ||
        isNerve(GET_NERVE(BombTeresa, BombTeresaNrvDisperse))) {
        return false;
    }
    return true;
}

bool BombTeresa::appearNormal(const TVec3f& rPosition, const TVec3f& rVelocity) {
    if (!MR::isDead(this)) {
        return false;
    }
    setNerve(GET_NERVE(BombTeresa, BombTeresaNrvAppear));
    mPosition.set(rPosition);
    appear();
    MR::showModel(this);
    mVelocity.set(rVelocity);
    MR::invalidateClipping(this);
    MR::trySetMoveLimitCollision(this);
    return true;
}

bool BombTeresa::appearShadow(const TVec3f& rPosition, const TVec3f& rVelocity) {
    if (!appearNormal(rPosition, rVelocity)) {
        return false;
    }
    MR::invalidateHitSensors(this);
    MR::hideModelAndOnCalcAnim(this);
    setNerve(GET_NERVE(BombTeresa, BombTeresaNrvShadowAppear));
    return true;
}

void BombTeresa::addTeresaSpinPullVelocity(f32 arg0) {
    TVec3f PlayerCenterPos = TVec3f(*MR::getPlayerCenterPos());
    MR::addVelocityClockwiseToTarget(this, PlayerCenterPos, _E0);
    MR::addVelocityMoveToTarget(this, PlayerCenterPos, 0.0f, _E4, 0.0f, 150.0f);
    MR::addVelocityKeepHeight(this, PlayerCenterPos, arg0, 3.0f, 100.0f);
}

bool BombTeresa::requestDisperse() {
    if (isNerve(GET_NERVE(BombTeresa, BombTeresaNrvExplosion)) || isNerve(GET_NERVE(BombTeresa, BombTeresaNrvShock)) ||
        isNerve(GET_NERVE(BombTeresa, BombTeresaNrvDisperse))) {
        return false;
    }
    setNerve(GET_NERVE(BombTeresa, BombTeresaNrvDisperse));
    return true;
}
