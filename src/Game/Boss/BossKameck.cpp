#include "Game/Boss/BossKameck.hpp"
#include "Game/Boss/BossKameckMoveRail.hpp"
#include "Game/Boss/BossKameckSequencer.hpp"
#include "Game/Boss/BossKameckVs1.hpp"
#include "Game/Boss/BossKameckVs2.hpp"
#include "Game/Enemy/Kameck.hpp"
#include "Game/Enemy/KameckBeamHolder.hpp"
#include "Game/Enemy/KameckHolder.hpp"
#include "Game/LiveActor/ActiveActorList.hpp"
#include "Game/LiveActor/ActorJointCtrl.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"

void BossKameck_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)3.0f;
    (void)2.0f;

    // function order is weird here
    (void)300.0f;
    (void)240.0f;
    (void)30.0f;
    (void)40.0f;
}

namespace {
    static const f32 sFrontVecBlendRate = 0.2f;
    static const f32 sUpVecBlendRate = 0.04f;
    // static const f32 sMaxBendAccel = _;
    // static const f32 sMaxBendPose = _;
};  // namespace

BossKameck::BossKameck(const char* pName, const char* pModelName)
    : LiveActor(pName), mModelName(pModelName), _90(0.0f, 1.0f), _A0(0, 0, 1), mSequencer(), mKameckHolder(), mJointCtrl(), mActorList(),
      mBeamEventListener(), mMoveRail(), mMoveRailNum(), _C8(0, 0, 0), mObjArg7(-1) {
    mBeamEventListener = new BossKameckBeamEventListener(this);
    mActorList = new ActiveActorList(8);
}

BossKameckMoveRail* BossKameck::getMoveRail(s32 no) {
    for (s32 i = 0; i < mMoveRailNum; i++) {
        if (mMoveRail[i]->_8C == no) {
            return mMoveRail[i];
        }
    }

    return nullptr;
}

void BossKameck::startSequence() {
    mSequencer->start();
    appear();
    MR::hideModel(this);
}

void BossKameck::startDemoAppearKameck() {
    if (mKameckHolder != nullptr) {
        mKameckHolder->startDemoAppear();
    }
}

void BossKameck::endDemoAppearKameck() {
    if (mKameckHolder != nullptr) {
        mKameckHolder->endDemoAppear();
    }
}

void BossKameck::appearKameck() {
    if (mKameckHolder != nullptr) {
        mKameckHolder->appearAll();
    }
}

void BossKameck::deadKameck() {
    if (mKameckHolder != nullptr) {
        mKameckHolder->deadForceAll();
    }
}

s32 BossKameck::getLivingKameckNum() const {
    if (mKameckHolder != nullptr) {
        return mKameckHolder->getLivingActorNum();
    }

    return 0;
}

void BossKameck::appearStarPieceToPlayer(s32 num) {
    TVec3f dir;
    MR::calcVecToPlayerH(&dir, this, nullptr);
    dir.sub(mGravity);
    MR::normalizeOrZero(&dir);
    MR::appearStarPieceToDirection(this, mPosition, dir, num, 30.0f, 65.0f, false);
    MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
}

void BossKameck::makeActorDead() {
    LiveActor::makeActorDead();
    MR::invalidateShadowAll(this);
}

void BossKameck::control() {
    if (mSequencer != nullptr) {
        mSequencer->update();
    }

    mActorList->removeDeadActor();
    _C8.set< f32 >(mVelocity);
    mJointCtrl->update();
}

void BossKameck::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _90);
    MR::setBaseScale(this, mScale);
    mJointCtrl->setCallBackFunction();
}

void BossKameck::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (mSequencer != nullptr) {
        mSequencer->attackSensor(pSender, pReceiver);
    }
}

bool BossKameck::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (mSequencer != nullptr) {
        return mSequencer->receiveMsgPlayerAttack(msg, pSender, pReceiver);
    }

    return false;
}

void BossKameck::endDemo() {
    _C8.zero();
    MR::zeroVelocity(this);
    MR::calcGravity(this);
    mJointCtrl->startDynamicCtrl("Suso1", -1);
    mJointCtrl->startDynamicCtrl("Cap1", -1);
}

void BossKameck::setPose(MtxPtr pPoseMtx) {
    TPos3f mtx;
    mtx.set(pPoseMtx);
    mtx.getQuat(_90);
    mtx.getTrans(mPosition);
    _90.getZDir(_A0);
}

void BossKameck::killAllBeam() {
    mActorList->killAll();
}

void BossKameck::updatePose() {
    TVec3f v19;
    TVec3f v18;
    v19.set(-mGravity);
    v18.killElement(mVelocity - _C8, mGravity);
    f32 mag = v18.length();

    if (!MR::isNearZero(mag)) {
        f32 v4 = MR::normalize(mag, 0.0f, 3.0f);

        v19 += v18 * ((4.0f * v4) / mag);

        if (!MR::isNearZero(v19)) {
            MR::normalize(&v19);
        } else {
            v19.set(-mGravity);
        }
    }

    MR::blendQuatUpFront(&_90, v19, _A0, ::sUpVecBlendRate, ::sFrontVecBlendRate);
}

void BossKameck::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm(mModelName, nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    MR::makeQuatAndFrontFromRotate(&_90, &_A0, this);
    MR::calcGravity(this);
    MR::invalidateClipping(this);
    MR::initShadowFromCSV(this, "Shadow");
    initHitSensor(2);
    MR::addHitSensorEnemy(this, "body", 8, 300.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorEnemyAttack(this, "attack", 8, 240.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initEffectKeeper(0, nullptr, false);
    MR::createKameckBeamHolder();
    MR::createKameckFireBallHolder();
    MR::createKameckBeamTurtleHolder();
    initKameckHolder(rIter);
    initMoveRail(rIter);
    MR::getJMapInfoArg7WithInit(rIter, &mObjArg7);

    if (mObjArg7 != -1) {
        MR::declareCameraRegisterVec(this, mObjArg7, &mPosition);
    }

    mJointCtrl = new ActorJointCtrl(this);
    MR::addToAttributeGroupSearchTurtle(this);
    initSound(4, false);
    mSequencer->init(this, rIter);
    MR::declarePowerStar(this);
    s32 hasPowerStar = MR::hasPowerStarInCurrentStageWithDeclarer(mName, -1);
    MR::startBrk(this, "Star");
    MR::setBrkFrameAndStop(this, hasPowerStar);
    MR::needStageSwitchReadA(this, rIter);
    MR::listenStageSwitchOnA(this, MR::Functor_Inline(this, &BossKameck::startSequence));
    makeActorDead();
}

void BossKameck::initKameckHolder(const JMapInfoIter& rIter) {
    s32 childNum = MR::getChildObjNum(rIter);
    s32 childKameckNum = 0;
    const char* objName;

    for (s32 i = 0; i < childNum; i++) {
        MR::getChildObjName(&objName, rIter, i);

        if (MR::isEqualString(objName, "ChildKameck")) {
            childKameckNum++;
        }
    }

    if (childKameckNum <= 0) {
        return;
    }

    mKameckHolder = new KameckHolder(childKameckNum);

    for (s32 j = 0; j < childNum; j++) {
        MR::getChildObjName(&objName, rIter, j);

        if (MR::isEqualString(objName, "ChildKameck")) {
            Kameck* kameck = new Kameck("カメック[保持用]");
            kameck->setBeamType(2);
            MR::initChildObj(kameck, rIter, j);
            kameck->makeActorDead();
            mKameckHolder->registerActor(kameck);
        }
    }
}

void BossKameck::initMoveRail(const JMapInfoIter& rIter) {
    s32 childObjNum = MR::getChildObjNum(rIter);
    mMoveRailNum = 0;
    const char* objName;

    for (s32 i = 0; i < childObjNum; i++) {
        MR::getChildObjName(&objName, rIter, i);

        if (MR::isEqualString(objName, "BossKameckMoveRail")) {
            mMoveRailNum++;
        }
    }

    if (mMoveRailNum <= 0) {
        return;
    }

    mMoveRail = new BossKameckMoveRail*[mMoveRailNum];

    s32 moveRailNo = 0;

    for (s32 j = 0; j < childObjNum; j++) {
        MR::getChildObjName(&objName, rIter, j);

        if (MR::isEqualString(objName, "BossKameckMoveRail")) {
            mMoveRail[moveRailNo] = new BossKameckMoveRail("ボスカメック移動経路");
            MR::initChildObj(mMoveRail[moveRailNo], rIter, j);
            moveRailNo++;
        }
    }
}

void BossKameck::appearStarPieceToUp(s32 num) {
    MR::appearStarPieceToDirection(this, mPosition, -mGravity, num, 30.0f, 40.0f, false);
    MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
}

void BossKameck::startDemo() {
    _C8.zero();
    MR::zeroVelocity(this);
    MR::calcGravity(this);
    mJointCtrl->endDynamicCtrl("Suso1", 0);
    mJointCtrl->endDynamicCtrl("Cap1", 0);
}

void BossKameck::hitBeam(s32 beamType) {
    if (MR::isDead(this)) {
        return;
    }

    switch (beamType) {
    case 1:
        MR::startSound(this, "SE_BV_KAMECK_ATK_SUCCESS");
        break;
    case 2:
    case 3:
    case 4:
        MR::startSound(this, "SE_BV_KAMECK_ATK_SUCCESS");
        break;
    }
}

namespace MR {
    NameObj* createBossKameck1(const char* pName) {
        BossKameck* bossKameck1;

        bossKameck1 = new BossKameck(pName, "BossKameck");
        bossKameck1->mSequencer = new BossKameckVs1();

        return bossKameck1;
    }

    NameObj* createBossKameck2(const char* pName) {
        BossKameck* bossKameck2;

        bossKameck2 = new BossKameck(pName, "BossKameck");
        bossKameck2->mSequencer = new BossKameckVs2();

        return bossKameck2;
    }
};  // namespace MR
