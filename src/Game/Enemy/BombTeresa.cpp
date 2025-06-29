#include "Game/Enemy/BombTeresa.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/System/NerveExecutor.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointController.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
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
    NEW_NERVE(BombTeresaNrvBindStarPointer, BombTeresa, BindStarPointer);
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
    MR::setJointControllerParam(mJointDelegator, this, "TongueOne");
    mJointDelegator2 = new JointControlDelegator<BombTeresa>(this, &BombTeresa::endTongueMtxCallBack, &BombTeresa::rootTongueMtxCallBack);
    MR::setJointControllerParam(mJointDelegator, this, "TongueTwo");
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

inline void BombTeresa::exeBindStarPointer() {
    MR::updateActorStateAndNextNerve(this, mBindStarPointer, &NrvBombTeresa::BombTeresaNrvWait::sInstance);
}
