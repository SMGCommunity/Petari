#include "Game/Boss/Polta.hpp"
#include "Game/Boss/PoltaActionBase.hpp"
#include "Game/Boss/PoltaActionSequencer.hpp"
#include "Game/Boss/PoltaArm.hpp"
#include "Game/Boss/PoltaBattleLv1.hpp"
#include "Game/Boss/PoltaBattleLv2.hpp"
#include "Game/Boss/PoltaDemo.hpp"
#include "Game/Boss/PoltaFunction.hpp"
#include "Game/Boss/PoltaGroundRock.hpp"
#include "Game/Boss/PoltaGroundRockHolder.hpp"
#include "Game/Boss/PoltaRock.hpp"
#include "Game/Boss/PoltaRockHolder.hpp"
#include "Game/Boss/PoltaSensorCtrl.hpp"
#include "Game/Enemy/BombTeresa.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/BaseMatrixFollowTargetHolder.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "revolution/types.h"

BombTeresaHolder::BombTeresaHolder() : DeriveActorGroup< BombTeresa >("ポルタ地面岩管理", 6) {
    for (int i = 0; i < 6; i++) {
        BombTeresa* curBombTeresa = new BombTeresa("ポルタ用ボムテレサ");
        curBombTeresa->initWithoutIter();
        curBombTeresa->makeActorDead();
        curBombTeresa->_EF = true;
        registerActor(curBombTeresa);
    }
}

void Polta::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("Polta", nullptr, false);
    MR::connectToSceneCollisionEnemy(this);
    MR::initLightCtrl(this);
    MR::makeQuatAndFrontFromRotate(&_B4, &_C4, this);
    //_D0.set<f32>(_B4);
    _E0.set(mPosition);
    MR::addBaseMatrixFollowTarget(this, rIter, nullptr, nullptr);
    initArm();
    initPartsModel();
    initBombTeresaHolder();
    initSensor();
    mSequencer = new PoltaActionSequencer(this, rIter);
    initEffectKeeper(0, nullptr, false);
    initSound(12, false);
    initCamera(rIter);
    MR::onCalcGravity(this);
    MR::useStageSwitchWriteDead(this, rIter);
    if (MR::useStageSwitchReadA(this, rIter)) {
        MR::listenStageSwitchOnA(this, MR::Functor_Inline(this, &Polta::start));
    }
    MR::startBva(this, "BreakLevel");
    MR::setBvaFrameAndStop(this, 0.0f);
    MR::startBtk(this, "Inner");
    MR::declarePowerStar(this);
    MR::declareStarPiece(this, 50);
    MR::declareCoin(this, 20);
    makeActorAppeared();
}

void Polta::initArm() {
    calcAnim();
    mLeftArm = new PoltaArm("左腕", "PoltaLeftArm", getBaseMtx());
    mRightArm = new PoltaArm("右腕", "PoltaRightArm", getBaseMtx());
    mLeftArm->initWithoutIter();
    mRightArm->initWithoutIter();
}

void Polta::initPartsModel() {
    mBreakModel = MR::createModelObjEnemy("壊れモデル", "PoltaBodyBreak", getBaseMtx());
    mBreakModel->initWithoutIter();
    MR::initLightCtrl(mBreakModel);
    MR::invalidateClipping(mBreakModel);
    mBreakModel->makeActorDead();
    mFormationModel = MR::createModelObjEnemy("合体演出岩モデル", "PoltaFormationRock", getBaseMtx());
    mFormationModel->initWithoutIter();
    MR::initLightCtrl(mFormationModel);
    MR::invalidateClipping(mFormationModel);
    mFormationModel->makeActorDead();
}

void Polta::initCamera(const JMapInfoIter& rIter) {
    s32 cameraRegisterIndex = -1;
    MR::getJMapInfoArg7WithInit(rIter, &cameraRegisterIndex);
    if (cameraRegisterIndex != -1) {
        MR::declareCameraRegisterVec(this, cameraRegisterIndex, &mPosition);
    }
    mCameraInfo = new ActorCameraInfo(rIter);
    MR::initAnimCamera(this, mCameraInfo, "OpeningDemo");
    MR::initAnimCamera(this, mCameraInfo, "PowerUpDemo");
    MR::initAnimCamera(this, mCameraInfo, "DownDemo");
}

void Polta::appearBreakModelFirst(const TVec3f& rVec) {
    appearBreakModel(rVec, "BreakFirst");
}

void Polta::appearBreakModelMiddle(const TVec3f& rVec) {
    appearBreakModel(rVec, "BreakSecond");
}

void Polta::appearBreakModelLast(const TVec3f& rVec) {
    appearBreakModel(rVec, "BreakThird");
}

void Polta::appearStarPiece(s32 starPieceAmount) {
    TVec3f head3JointPos;
    MR::copyJointPos(this, "Head3", &head3JointPos);
    MR::appearStarPieceToDirection(this, head3JointPos, -mGravity, starPieceAmount, 50.0f, 60.0f, false);
    MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
}

void BombTeresaHolder::disperseAll() {
    s32 objectCount = mObjectCount;
    for (s32 i = 0; i < objectCount; i++) {
        BombTeresa* curBombTeresa = reinterpret_cast< BombTeresa* >(getActor(i));
        curBombTeresa->requestDisperse();
    }
}

Polta::Polta(const char* pName)
    : LiveActor(pName), mSensorCtrl(nullptr), mLeftArm(nullptr), mRightArm(nullptr), mSequencer(nullptr), mBombTeresaHolder(nullptr),
      mRockHolder(nullptr), mGroundRockHolder(nullptr), mCameraInfo(nullptr), mBreakModel(nullptr), mFormationModel(nullptr), _B4(0, 0, 0, 1),
      _C4(0, 0, 1), _D0(0, 0, 0, 1), _E0(0, 0, 0), _EC(0.0f) {}

void Polta::initSensor() {
    mSensorCtrl = new PoltaSensorCtrl(this);
    initHitSensor(mSensorCtrl->getSensorCount());
    mSensorCtrl->initSensor();
}

void Polta::initBombTeresaHolder() {
    mBombTeresaHolder = new BombTeresaHolder();
    mRockHolder = new PoltaRockHolder();
    mGroundRockHolder = new PoltaGroundRockHolder();
}

// rVec is left unused
void Polta::appearBreakModel(const TVec3f& rVec, const char* pName) {
    mBreakModel->makeActorAppeared();
    MR::invalidateClipping(mBreakModel);
    MR::startAction(mBreakModel, pName);
}

bool Polta::isEndBreakModel() const {
    return MR::isActionEnd(mBreakModel);
}

void Polta::killBreakModel() {
    mBreakModel->kill();
}

void Polta::rotateToPlayer() {
    _EC = calcToPlayerRotateSpeed();
    MR::rotateDirectionGravityDegree(this, &_C4, _EC);
}

f32 Polta::calcToPlayerRotateSpeed() const {
    f32 v4 = _EC;
    if (MR::isFaceToPlayerHorizontalDegree(this, _C4, 10.0f)) {
        v4 *= 0.99f;
    } else {
        TVec3f v18;
        MR::calcSideVec(&v18, this);
        if (v18.dot(*MR::getPlayerPos() - mPosition) < 0.0f) {
            v4 += 0.1f;
        } else {
            v4 -= 0.1f;
        }
        v4 *= 0.998f;
    }
    if (v4 < -1.1f) {
        return -1.1f;
    } else if (v4 > 1.1f) {
        return 1.1f;
    }
    return v4;
}

void Polta::makeActorAppeared() {
    LiveActor::makeActorAppeared();
}

void Polta::makeActorDead() {
    LiveActor::makeActorDead();
}

void Polta::kill() {
    LiveActor::kill();
    mLeftArm->kill();
    mRightArm->kill();
    MR::startAfterBossBGM();
    MR::requestAppearPowerStar(this, mPosition);
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
}

void Polta::control() {
    updateAction();
}

void Polta::calcAnim() {
    LiveActor::calcAnim();
}

void Polta::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _B4);
}

void Polta::updateAction() {
    if (mSequencer != nullptr) {
        mSequencer->updateNerve();
    }
}

void Polta::updatePose(f32 param1, f32 param2) {
    TVec3f* mGravTemp = &mGravity;
    JMAVECScaleAdd(mGravTemp, &_C4, &_C4, -mGravTemp->dot(_C4));
    if (MR::isNearZero(_C4)) {
        _B4.getZDir(_C4);
    } else {
        MR::normalize(&_C4);
    }
    MR::blendQuatUpFront(&_B4, -mGravity, _C4, param1, param2);
}

void Polta::start() {
    MR::invalidateClipping(this);
    if (mSequencer != nullptr) {
        mSequencer->startAction();
    }
}

void Polta::setStartPose() {
    //_B4.set(_D0);
    MR::resetPosition(this, _E0);
    _B4.getZDir(_C4);
}

void Polta::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (mSequencer != nullptr) {
        mSequencer->attackSensor(pSender, pReceiver);
    }
}

bool Polta::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (mSequencer != nullptr) {
        return mSequencer->receiveMsgPlayerAttack(msg, pSender, pReceiver);
    }
    return false;
}

bool Polta::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (mSequencer != nullptr) {
        return mSequencer->receiveMsgEnemyAttack(msg, pSender, pReceiver);
    }
    return false;
}

bool Polta::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (mSequencer != nullptr) {
        return mSequencer->receiveOtherMsg(msg, pSender, pReceiver);
    }
    return false;
}
