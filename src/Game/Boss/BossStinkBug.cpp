#include "Game/Boss/BossStinkBug.hpp"
#include "Game/Boss/BossStinkBugActionSequencer.hpp"
#include "Game/Boss/BossStinkBugBomb.hpp"
#include "Game/Boss/BossStinkBugBombHolder.hpp"
#include "Game/LiveActor/ActorJointCtrl.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/Map/GroundChecker.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/BaseMatrixFollowTargetHolder.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointController.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "revolution/mtx.h"
#include "revolution/types.h"

namespace {
    const char* sSensorNameList[] = {"Body", "BodyHipDrop"};

    struct collisionInfo {
        const char* _0;
        const char* _4;
        const char* _8;
    };

    const collisionInfo sCollisionInfo[] = {{"Body", "Body", "Body"}, {"BodyGround", "Body", "Body"}, {"BodyHipDrop", "Body", "BodyHipDrop"}};

    const char* sLegJointNameList[] = {"RightFootA3", "LeftFootA3", "RightFootB3", "LeftFootB3", "RightFootC3", "LeftFootC3"};

    static TVec3f sAppearStarPieceOffset(0.0f, 200.0f, 0.0f);
}  // namespace

BossStinkBug::BossStinkBug(const char* pName)
    : LiveActor(pName), _8C(0), _94(nullptr), _98(nullptr), _CC(nullptr), _D0(nullptr), _D4(nullptr), _D8(nullptr), _DC(0.0f, 0.0f, 0.0f, 1.0f),
      _EC(0.0f, 0.0f, 1.0f), _F8(0.0f, 0.0f, 0.0f), _104(-1), _108(0.15f), _10C(1.0f), _110(false), _111(false) {
    _9C.identity();
    _90 = new GroundChecker*[6];

    for (int i = 0; i < 6; i++) {
        _90[i] = new GroundChecker("地面チェック", 30.0f, 0.0f);
        _90[i]->makeActorDead();
        MR::setBinderExceptActor(_90[i], this);
    }
}

void BossStinkBug::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    invalidateCollision();
}

void BossStinkBug::makeActorDead() {
    LiveActor::makeActorDead();
    invalidateCollision();
}

void BossStinkBug::kill() {
    LiveActor::kill();
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
}

void BossStinkBug::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("BossStinkBug", nullptr, false);
    MR::connectToSceneCollisionEnemy(this);
    MR::makeQuatAndFrontFromRotate(&_DC, &_EC, this);

    // is .zero because z is set first
    mRotation.zero();
    initSensor();
    initCollision();
    initRailRider(rIter);
    initBinder(300.0f, 300.0f, false);
    MR::setBinderExceptActor(this, this);
    initEffectKeeper(0, nullptr, false);
    MR::onCalcGravity(this);
    initCamera(rIter);
    MR::initShadowFromCSV(this, "Shadow");
    MR::excludeCalcShadowToActorAll(this, this);
    _98 = new ActorJointCtrl(this);
    MR::addBaseMatrixFollowTarget(this, rIter, &_9C, new BossStinkBugFollowValidater(this));
    MR::invalidateClipping(this);
    _D0 = new BossStinkBugActionSequencer(this, rIter);
    _94 = new BossStinkBugBombHolder(this);
    _D4 = MR::createPartsModelMapObj(this, "羽モデル", "BossStinkBugWing", nullptr);
    _D4->initFixedPosition("Switch");
    _D8 = MR::createPartsModelMapObj(this, "ボム発射口", "BossStinkBugBombLauncher", nullptr);
    _D8->initFixedPosition("Body");
    MR::declarePowerStar(this);
    initSound(12, false);
    MR::startBrk(this, "Normal");
    if (MR::useStageSwitchReadA(this, rIter)) {
        MR::listenStageSwitchOnA(this, MR::Functor_Inline(this, &start));
        makeActorDead();
    } else {
        makeActorAppeared();
    }
    MR::useStageSwitchWriteDead(this, rIter);
}

void BossStinkBug::start() {
    if(_D0->startAction()) {
        appear();
    }
}


void BossStinkBug::initCamera(const JMapInfoIter& rIter) {
    MR::getJMapInfoArg7WithInit(rIter, &_104);
    if (_104 != -1) {
        MR::declareCameraRegisterVec(this, _104, &_F8);
        MR::declareCameraRegisterMtx(this, _104, _9C.toMtxPtr());
    }
    MR::initMultiActorCamera(this, rIter, &_CC, "Fall");
}

void BossStinkBug::initSensor() {
    initHitSensor(5);
    MR::addHitSensorEye(this, "eye", 8, 1000.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::invalidateHitSensor(this, "eye");
    MR::addHitSensorEnemyAttack(this, "attack_left", 8, 1000.0f, TVec3f(-120.0f, 50.0f, 300.0f));
    MR::addHitSensorEnemyAttack(this, "attack_right", 8, 1000.0f, TVec3f(120.0f, 50.0f, 300.0f));
    MR::invalidateHitSensor(this, "attack_left");
    MR::invalidateHitSensor(this, "attack_right");
    for (int i = 0; i < (s32)(sizeof(::sSensorNameList) / sizeof(char*)); i++) {
        MR::addMessageSensorReceiver(this, ::sSensorNameList[i]);
    }
}

void BossStinkBug::initCollision() {
    _8C = new CollisionParts*[3];
    for (int i = 0; i < 3; i++) {
        _8C[i] = MR::createCollisionPartsFromLiveActor(this, ::sCollisionInfo[i]._0, getSensor(::sCollisionInfo[i]._8),
                                                       MR::getJointMtx(this, sCollisionInfo[i]._4), MR::AutoEqualScale);
    }
}

void BossStinkBug::disposeBomb() {
    _94->killAll();
}

void BossStinkBug::reuestMovementOnParts() {
    MR::requestMovementOn(_D4);
    MR::requestMovementOn(_D8);
}

LiveActor* BossStinkBug::getWingModel() {
    return _D4;
}

LiveActor* BossStinkBug::getBombLauncher() {
    return _D8;
}

void BossStinkBug::control() {
    updateAction();
    updatePose();
    _110 = false;

    if (_111) {
        updateJointControl();
    }

    updateCamera();
}

void BossStinkBug::calcAndSetBaseMtx() {
    _98->setCallBackFunction();
    TPos3f pos;
    pos.setQuat(_DC);
    pos.setPos(mPosition);

    JMath::gekko_ps_copy12(_9C, pos);

    MR::rotateMtxLocalXDegree(pos.toMtxPtr(), mRotation.x);
    MR::rotateMtxLocalYDegree(pos.toMtxPtr(), mRotation.y);
    MR::rotateMtxLocalZDegree(pos.toMtxPtr(), mRotation.z);

    MR::setBaseTRMtx(this, pos);
    MR::setBaseScale(this, mScale);
}

void BossStinkBug::calcUpVec(TVec3f* pOut) const {
    TPos3f pos;
    pos.identity();
    pos.setQuat(_DC);

    MR::rotateMtxLocalXDegree(pos.toMtxPtr(), mRotation.x);
    MR::rotateMtxLocalYDegree(pos.toMtxPtr(), mRotation.y);
    MR::rotateMtxLocalZDegree(pos.toMtxPtr(), mRotation.z);

    pos.getYDir(*pOut);
}

void BossStinkBug::calcAnim() {
    LiveActor::calcAnim();
    for (int i = 0; i < 3; i++) {
        _8C[i]->setMtx();
    }
}

void BossStinkBug::updateAction() {
    if (_D0 != nullptr) {
        _D0->updateNerve();
    }
}

void BossStinkBug::updatePose() {
    if (!_110) {
        _EC.rejection(mGravity);
        if (MR::isNearZero(_EC)) {
            _DC.getZDir(_EC);
        } else {
            MR::normalize(&_EC);
        }
        MR::blendQuatUpFront(&_DC, -mGravity, _EC, 0.2f, 0.2f);
        _110 = true;
    }
}

void BossStinkBug::setPose(MtxPtr pMtx) {
    TPos3f pos;
    JMath::gekko_ps_copy12(&pos, pMtx);
    pos.getQuat(_DC);
    pos.getTrans(mPosition);
    _DC.getZDir(_EC);
    MR::normalize(&_EC);
    _110 = true;
}

void BossStinkBug::updateCamera() {
    _F8.setPS2(mPosition);
}


bool BossStinkBug::isValidFollowId(s32 id) const{
    if (_D0 != nullptr) {
        return _D0->isValidFollowId(id);
    }else{
        return false;
    }
}


void BossStinkBug::validateCollisionGround() {
    MR::invalidateCollisionParts(_8C[0]);
    MR::validateCollisionParts(_8C[1]);
    MR::validateCollisionParts(_8C[2]);
}

void BossStinkBug::validateCollisionFly() {
    MR::validateCollisionParts(_8C[0]);
    MR::invalidateCollisionParts(_8C[1]);
    MR::validateCollisionParts(_8C[2]);
}

void BossStinkBug::validateCollisionHitOnly() {
    MR::invalidateCollisionParts(_8C[0]);
    MR::invalidateCollisionParts(_8C[1]);
    MR::validateCollisionParts(_8C[2]);
}

void BossStinkBug::invalidateCollision() {
    for (int i = 0; i < 3; i++) {
        MR::invalidateCollisionParts(_8C[i]);
    }
}

void BossStinkBug::onBindLeg() {
    if (!_111) {
        for (int i = 0; i < 6; i++) {
            MR::copyJointPos(this, ::sLegJointNameList[i], &_90[i]->mPosition);
            _90[i]->appear();
            MR::resetPosition(_90[i], _90[i]->mPosition);
        }

        _111 = true;
    }
}

void BossStinkBug::offBindLeg() {
    if (_111) {
        _98->endIKCtrlAll();
        for (int i = 0; i < 6; i++) {
            _90[i]->kill();
        }
        _111 = false;
    }
}

bool BossStinkBug::isLegInWater() const {
    for (int i = 0; i < 6; i++) {
        if (MR::isBindedGroundWater(_90[i])) {
            return true;
        }
    }
    return false;
}

void BossStinkBug::updateJointControl() {
    ActorJointCtrl* jointCtrl = _98;
    jointCtrl->startUpdate();
    for (int i = 0; i < 6; i++) {
        TVec3f legJointPos;
        MR::copyJointPos(this, ::sLegJointNameList[i], &legJointPos);
        TVec3f groundCheckPos = _90[i]->mPosition;
        TVec3f dirGroundCheckToLegJoint = legJointPos - groundCheckPos;

        if (MR::isBinded(_90[i])) {
            _90[i]->bringFriction(_108, 0.2f);
        } else {
            _90[i]->bringFriction(1.0f, 0.08f);
        }

        dirGroundCheckToLegJoint.scale(_90[i]->_8C);
        _90[i]->mVelocity.set(dirGroundCheckToLegJoint);
        _90[i]->movement();
        jointCtrl->setIKEndPosition(::sLegJointNameList[i], _90[i]->mPosition, _10C);

        if (MR::isBinded(_90[i])) {
            TVec3f fix = *MR::getBindedFixReactionVector(_90[i]);
            if (!MR::isNearZero(fix)) {
                MR::normalize(&fix);
                jointCtrl->setIKEndDirection(::sLegJointNameList[i], fix, 0.1f * _10C);
            }
        } else {
            TVec3f yDir;
            MR::extractMtxYDir(MR::getJointMtx(this, ::sLegJointNameList[i]), &yDir);
            jointCtrl->setIKEndDirection(::sLegJointNameList[i], yDir, 0.05f * _10C);
        }
    }
    jointCtrl->endUpdate();
}

void BossStinkBug::appearStarPiece(s32 num) {
    TVec3f offsetVec;
    PSMTXMultVec(MR::getJointMtx(this, "Body"), ::sAppearStarPieceOffset, offsetVec);
    MR::appearStarPieceToDirection(this, offsetVec, -mGravity, num, 30.0f, 40.0f, false);
    MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
}

void BossStinkBug::attackSensor(HitSensor* pSender, HitSensor* pReciever) {
    if(_D0 != nullptr) {
        _D0->attackSensor(pSender, pReciever);
    }
}

bool BossStinkBug::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if(_D0 != nullptr) {
        _D0->receiveMsgPlayerAttack(msg, pSender, pReceiver);
    } else {
        return false;
    }
}

bool BossStinkBug::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if(_D0 != nullptr) {
        return _D0->receiveOtherMsg(msg, pSender, pReceiver);
    } else {
        return false;
    }
}

void BossStinkBug::startEventCamera(const char* pName) {
    MR::startMultiActorCameraTargetSelf(this, _CC, pName, -1);
}

void BossStinkBug::endEventCamera(const char* pName) {
    MR::endMultiActorCamera(this, _CC, pName, false, -1);
}

bool BossStinkBug::isSensorBody(const HitSensor* pSensor) const {
    return pSensor == getSensor("BodyHipDrop");
}

bool BossStinkBug::throwBomb(f32 f1, f32 f2) {
    BossStinkBugBomb* throwBomb = _94->getDeadMember();

    if(throwBomb == nullptr) {
        return false;
    }

    TPos3f jointMtx;
    JMath::gekko_ps_copy12(jointMtx, MR::getJointMtx(_D8, "DischargePosition"));

    TVec3f trans;
    TVec3f yDir;

    jointMtx.getTrans(trans);
    jointMtx.getYDir(yDir);
    yDir.scale(-f1);
    MR::addRandomVector(&yDir, yDir, f2);

    throwBomb->start(trans, yDir);

    MR::startSound(this, "SE_BM_BOSS_BUG_BOMB_EMIT", -1, -1);
    MR::startBck(_D8, "Discharge", nullptr);
    return true;
}
