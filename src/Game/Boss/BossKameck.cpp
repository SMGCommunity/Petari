#include "Game/Boss/BossKameck.hpp"
#include "Game/Boss/BossKameckMoveRail.hpp"
#include "Game/Boss/BossKameckSequencer.hpp"
#include "Game/Boss/BossKameckVs1.hpp"
#include "Game/Boss/BossKameckVs2.hpp"
#include "Game/Enemy/Kameck.hpp"
#include "Game/Enemy/KameckHolder.hpp"
#include "Game/Enemy/KameckBeamHolder.hpp"
#include "Game/LiveActor/ActorJointCtrl.hpp"

BossKameck::BossKameck(const char *pName, const char *pType) : LiveActor(pName),
    _8C(pType), _90(0.0f, 1.0f), _A0(0, 0, 1), mSequencer(nullptr), mKameckHolder(nullptr), mJointCtrl(nullptr),
    mActorList(nullptr), mBeamListener(nullptr), mMoveRail(nullptr), _C4(0), _C8(0, 0, 0), _D4(-1) {

    mBeamListener = new BossKameckBeamEventListener(this);
    mActorList = new ActiveActorList(8);
}

BossKameckMoveRail* BossKameck::getMoveRail(s32 no) {
    for (s32 i = 0; i < _C4; i++) {
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
    MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
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
    _C8.set<f32>(mVelocity);
    mJointCtrl->update();
}

void BossKameck::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _90);
    MR::setBaseScale(this, mScale);
    mJointCtrl->setCallBackFunction();
}

void BossKameck::attackSensor(HitSensor *a1, HitSensor *a2) {
    if (mSequencer != nullptr) {
        mSequencer->attackSensor(a1, a2);
    }
}

bool BossKameck::receiveMsgPlayerAttack(u32 msg, HitSensor *a1, HitSensor *a2) {
    if (mSequencer != nullptr) {
        return mSequencer->receiveMsgPlayerAttack(msg, a1, a2);
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

/* functionally matches */
void BossKameck::setPose(MtxPtr mtx) {
    TPos3f pos;
    pos.setInline(mtx);
    pos.getQuat(_90);

    f32 z = pos.mMtx[2][3];
    f32 y = pos.mMtx[1][3];
    f32 x = pos.mMtx[0][3];
    mPosition.set(x, y, z);

    f32 v1 = (2.0f * (_90.x * _90.z)) + (2.0f * (_90.w * _90.y));
    f32 v2 = (2.0f * (_90.y * _90.z)) - (2.0f * (_90.w * _90.x));
    f32 v3 = (1.0f - (2.0f * (_90.x * _90.x))) - (2.0f * (_90.y * _90.y));
    _A0.set(v1, v2, v3);
}

void BossKameck::killAllBeam() {
    mActorList->killAll();
}

/* stack is off */
/*
void BossKameck::updatePose() {
    TVec3f v17(-mGravity);
    TVec3f v19;
    v19.set<f32>(v17);
    TVec3f v16(mVelocity);
    v16.sub(_C8);
    TVec3f* gravPtr = &mGravity;
    f32 dot = gravPtr->dot(v16);
    TVec3f v18;
    JMAVECScaleAdd(gravPtr, &v16, &v18, -dot);
    f32 mag = PSVECMag(&v18);

    TVec3f stack_44;
    TVec3f* ptr = &stack_44;

    if (!MR::isNearZero(mag, 0.001f)) {
        f32 v4 = MR::normalize(mag, 0.0f, 3.0f);
        TVec3f v13(v18);

        
        ptr->scaleInline((4.0f * v4) / mag);
        ptr->addInline(v13);

        if (!MR::isNearZero(*ptr, 0.001f)) {
            MR::normalize(ptr);
        }
        else {
            ptr->set(-mGravity);
        }
    }

    MR::blendQuatUpFront(&_90, stack_44, _A0, 0.039999999f, 0.2f);
}
*/

void BossKameck::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm(_8C, nullptr, false);
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
    MR::getJMapInfoArg7WithInit(rIter, &_D4);

    if (_D4 != -1) {
        MR::declareCameraRegisterVec(this, _D4, &mPosition);
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
    MR::listenStageSwitchOnA(this, MR::FunctorV0M<BossKameck*, void (BossKameck::*)(void)>(this, &BossKameck::startSequence));
    makeActorDead();
}

void BossKameck::initKameckHolder(const JMapInfoIter &rIter) {
    s32 childNum = MR::getChildObjNum(rIter);
    s32 kameckChildNum = 0;
    const char* objName;

    for (s32 i = 0; i < childNum; i++) {
        MR::getChildObjName(&objName, rIter, i);

        if (MR::isEqualString(objName, "ChildKameck")) {
            kameckChildNum++;
        }
    }

    if (kameckChildNum > 0) {
        mKameckHolder = new KameckHolder(kameckChildNum);

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
}

void BossKameck::initMoveRail(const JMapInfoIter &rIter) {
    s32 childNum = MR::getChildObjNum(rIter);
    _C4 = 0;
    const char* objName;

    for (s32 i = 0; i < childNum; i++) {
        MR::getChildObjName(&objName, rIter, i);

        if (MR::isEqualString(objName, "BossKameckMoveRail")) {
            _C4++;
        }
    }

    if (_C4 > 0) {
        mMoveRail = new BossKameckMoveRail*[_C4];
        s32 curRails = 0;

        for (s32 j = 0; j < childNum; j++) {
            MR::getChildObjName(&objName, rIter, j);

            if (MR::isEqualString(objName, "BossKameckMoveRail")) {
                mMoveRail[curRails] = new BossKameckMoveRail("ボスカメック移動経路");
                MR::initChildObj(mMoveRail[curRails], rIter, j);
                curRails++;
            }
        } 
    }
}

void BossKameck::appearStarPieceToUp(s32 num) {
    TVec3f neg = -mGravity;
    MR::appearStarPieceToDirection(this, mPosition, neg, num, 30.0f, 40.0f, false);
    MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
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
            MR::startSound(this, "SE_BV_KAMECK_ATK_SUCCESS", -1, -1);
            break;
        case 2:
        case 3:
        case 4:
            MR::startSound(this, "SE_BV_KAMECK_ATK_SUCCESS", -1, -1);
            break;
    }
}


namespace MR {
    BossKameck* createBossKameck1(const char *pName) {
        BossKameck* boss = new BossKameck(pName, "BossKameck");
        boss->mSequencer = new BossKameckVs1();
        return boss;
    }

    BossKameck* createBossKameck2(const char *pName) {
        BossKameck* boss = new BossKameck(pName, "BossKameck");
        boss->mSequencer = new BossKameckVs2();
        return boss;
    }
};

BossKameck::~BossKameck() {

} 
