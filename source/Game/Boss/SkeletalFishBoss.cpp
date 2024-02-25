#include "Game/Boss/SkeletalFishBoss.hpp"
#include "Game/Boss/SkeletalFishBossInfo.hpp"
#include "Game/Boss/SkeletalFishBossBattleDirector.hpp"
#include "Game/Boss/SkeletalFishBossRail.hpp"
#include "Game/Boss/SkeletalFishBossRailHolder.hpp"
#include "Game/Boss/SkeletalFishGuardHolder.hpp"
#include "Game/Boss/SkeletalFishRailControl.hpp"
#include "Game/Camera/CameraTargetMtx.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/SensorHitChecker.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/System/ResourceHolder.hpp"
#include <cstdio>
#include <JSystem/JMath/JMath.hpp>

namespace {
    static SkeletalFishBoss::SensorToCollider sColInfo[0xE] = {
        { "Joint01", "BackBone01" },
        { "Joint02", "BackBone02" },
        { "Joint03", "BackBone03" },
        { "Joint04", "BackBone04" },
        { "Joint05", "BackBone05" },
        { "Joint06", "BackBone06" },
        { "Joint07", "BackBone07" },
        { "Joint08", "BackBone08" },
        { "Joint09", "BackBone09" },
        { "Joint10", "BackBone10" },
        { "Joint11", "BackBone11" },
        { "Joint12", "BackFin" },
        { "LeftFinJoint", "LeftFinJoint" },
        { "RightFinJoint", "RightFinJoint" },
    };

    static SkeletalFishBoss::JointToShadow sShadowInfo = {
         "Shadow00", "Joint02", "SkeletalFishBossShadow"
    };
};

// SkeletalFishBoss::SkeletalFishBoss
// SkeletalFishBoss::init

void SkeletalFishBoss::initAfterPlacement() {
    NameObj::initAfterPlacement();
    _110 = -1;
    powerUp();
    makeActorDead();
    calcPlanetCenterPosition();
    mBossDirector->initiate();
}

void SkeletalFishBoss::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    validateCollision();
    _110 = -1;
    powerUp();
}

void SkeletalFishBoss::appear() {
    LiveActor::appear();
    MR::hideModel(this);
    setNerve(&::SkeletalFishBossNrvAppearWait::sInstance);
}

void SkeletalFishBoss::kill() {
    LiveActor::kill();
    invalidateCollision();
    mScarFlash->kill();
}

// SkeletalFishBoss::control
// SkeletalFishBoss::calcAnim
// SkeletalFishBoss::calcJoint
// SkeletalFishBoss::damage
// SkeletalFishBoss::exeSwim

// ...

void SkeletalFishBoss::notifyAttack(SkeletalFishGuard *pGuard) {
    mGuardHolder->notifyAttack(pGuard);
}

LiveActor* SkeletalFishBoss::getCurrentRail() {
    return mRailControl->_8;
}

f32 SkeletalFishBoss::getBodyThickness() const {
    return 400.0f;
}

SkeletalFishBossHead* SkeletalFishBoss::getHeadActor() const {
    return mBossHead;
}

void SkeletalFishBoss::calcAndSetBaseMtx() {
    bool isInDemo = false;

    if (isNerve(&::SkeletalFishBossNrvAppearDemo::sInstance) || isNerve(&::SkeletalFishBossNrvPowerUpDemo::sInstance) || isNerve(&::SkeletalFishBossNrvDeadDemo::sInstance)) {
        isInDemo = true;
    }

    if (isInDemo) {
        MR::setBaseTRMtx(this, _150.toMtxPtr());
    }
    else {
        for (u32 i = 0; i < 0xD; i++) {
            mControllers[i]->registerCallBack();
        }

        TPos3f railMtx;
        mRailControl->getMtx(&railMtx, 0.0f);
        MR::setBaseTRMtx(this, railMtx.toMtxPtr());
        JMath::gekko_ps_copy12(&_120, &railMtx);
        _120.invert(_120);

        if (_D4) {
            JMath::gekko_ps_copy12(&_D8, &railMtx);
            _D8.mMtx[0][3] = _D4->mPosition.x;
            _D8.mMtx[1][3] = _D4->mPosition.y;
            _D8.mMtx[2][3] = _D4->mPosition.z;
        }
    }
}

void SkeletalFishBoss::initLevelStatus() {
    mBossInfo = new SkeletalFishBossInfo(this, _1A0, _1A4, "ボス戦パラメータ");
} 

void SkeletalFishBoss::initJoint() {
    mJointIndicies = new s32[MR::getJointNum(this)];

    for (u32 i = 0; i < MR::getJointNum(this); i++) {
        mJointIndicies[i] = -1;
    }

    for (s32 i = 0; i < 0xD; i++) {
        char buf[16];
        snprintf(buf, sizeof(buf), "Joint%02d", i);
        mControllers[i] = MR::createJointDelegatorWithNullChildFunc(this, &SkeletalFishBoss::calcJoint, buf);
        mJointIndicies[MR::getJointIndex(this, buf)] = i;
    }
}

void SkeletalFishBoss::initCollision() {
    mPartsArray = new CollisionParts*[0xE];

    for (u32 i = 0; i < 0xE; i++) {
        mPartsArray[i] = MR::createCollisionPartsFromLiveActor(this, sColInfo[i].mColliderName, mBossHead->getSensor("body"), MR::getJointMtx(this, sColInfo[i].mJointName), (MR::CollisionScaleType)2);
        MR::validateCollisionParts(mPartsArray[i]);
    } 
} 

void SkeletalFishBoss::initHead() {
    mBossHead = new SkeletalFishBossHead(this);
    mBossHead->initWithoutIter();
    _D4 = nullptr;
}

void SkeletalFishBoss::initScarFlash() {
    mScarFlash = new SkeletalFishBossScarFlash(this);
    mScarFlash->initWithoutIter();
}

void SkeletalFishBoss::initBreakModel() {
    mBreakModel = new ModelObj("壊れモデル", "SkeletalFishBossBreak", _150.mMtx, 0x12, -2, -2, false);
    mBreakModel->initWithoutIter();
    MR::initLightCtrl(mBreakModel);
    mBreakModel->makeActorDead();
    MR::invalidateClipping(mBreakModel);
}

void SkeletalFishBoss::initSwitch(const JMapInfoIter &rIter) {
    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
    }
    else if (MR::useStageSwitchReadA(this, rIter)) {
        MR::listenStageSwitchOnA(this, MR::FunctorV0M<SkeletalFishBoss *, void (SkeletalFishBoss::*)(void)>(this, &SkeletalFishBoss::appear));
    }
}

void SkeletalFishBoss::createGuards() {
    mGuardHolder = new SkeletalFishGuardHolder(this, _1A4, "スカルシャークガード管理");
}

#ifdef NON_MATCHING
void SkeletalFishBoss::initShadow() {
    MR::initShadowController(this, 1);
    MR::addShadowVolumeFlatModel(this, "Shadow00", sShadowInfo.mShadowName, MR::getJointMtx(this, sShadowInfo.mJointName));
    MR::setShadowDropLength(this, sShadowInfo.mName, 4000.0f);
    MR::excludeCalcShadowToSensorAll(this, mBossHead->getSensor("body"));
}
#endif

void SkeletalFishBoss::initCamera() {
    ActorCameraInfo cameraInfo(-1, 0);
    const char* fileName = "SkeletalFishBossBattleStart.canm";
    MR::declareEventCameraAnim(&cameraInfo, "スカルシャーク出現", MR::getResourceHolder(this)->mFileInfoTable->getRes(fileName));
    fileName = "SkeletalFishBossPowerUp1.canm";
    MR::declareEventCameraAnim(&cameraInfo, "スカルシャークパワーアップ", MR::getResourceHolder(this)->mFileInfoTable->getRes(fileName));
    fileName = "SkeletalFishBossPowerUp2.canm";
    MR::declareEventCameraAnim(&cameraInfo, "スカルシャークパワーアップ２", MR::getResourceHolder(this)->mFileInfoTable->getRes(fileName));
    fileName = "SkeletalFishBossDown.canm";
    MR::declareEventCameraAnim(&cameraInfo, "スカルシャーク死亡", MR::getResourceHolder(this)->mFileInfoTable->getRes(fileName));
    MR::declareEventCameraProgrammable("デモ終了後カメラ");
    mCameraTargetMtx = new CameraTargetMtx("カメラターゲットダミー");
    JMath::gekko_ps_copy12(&mCameraTargetMtx->mMatrix, &_150);
}

void SkeletalFishBoss::validateCollision() {
    for (u32 i = 0; i < 0xE; i++) {
        MR::validateCollisionParts(mPartsArray[i]);
    }

    SkeletalFishBossHead* head = getHeadActor();
    MR::validateCollisionParts(head->_CC);
    MR::validateCollisionParts(head->_100);
}

void SkeletalFishBoss::invalidateCollision() {
    for (u32 i = 0; i < 0xE; i++) {
        MR::invalidateCollisionParts(mPartsArray[i]);
    }

    SkeletalFishBossHead* head = getHeadActor();
    MR::invalidateCollisionParts(head->_CC);
    MR::invalidateCollisionParts(head->_100);
}

void SkeletalFishBoss::getMouthSensorCenterPos(TVec3f &rPos, f32 a2) const {
    TMtx34f mtx;
    JMath::gekko_ps_copy12(&mtx, MR::getJointMtx(this, "Head"));
    rPos.z = a2;
    rPos.x = 0.0f;
    rPos.y = 0.0f;
    mtx.mult(rPos, rPos);
}

void SkeletalFishBoss::updateCollision() {
    for (u32 i = 0; i < 0xE; i++) {
        mPartsArray[i]->setMtx();
    }

    SkeletalFishBossHead* head = getHeadActor();
    head->_CC->setMtx();
    head->_100->setMtx();
}

void SkeletalFishBoss::powerUp() {
    _110++;
    _114 = mBossInfo->getLevelStatus(_110)->mEnergyLevel;
    resetRail();
}

void SkeletalFishBoss::startDamageAnim() {
    MR::startBck(this, "Damage", nullptr);
    MR::startBck(mBossHead, "Damage", nullptr);
    mScarFlash->appear();
}

void SkeletalFishBoss::calcPlanetCenterPosition() {
    TVec3f firstPnt;
    MR::calcRailPointPos(&firstPnt, MR::getSkeletalFishBossRailHolder()->getInOrder(0), 0);
    TVec3f secondPnt;
    MR::calcRailPointPos(&firstPnt, MR::getSkeletalFishBossRailHolder()->getInOrder(0), 1);
    calcGravityCenter(&_180, firstPnt, secondPnt);
}

bool SkeletalFishBoss::calcGravityCenter(TVec3f *pOut, const TVec3f &rFirstPoint, const TVec3f &rSecondPoint) {
    TVec3f firstGravityCenter(rFirstPoint);
    TVec3f firstGravityVector;
    MR::calcGravityVector(this, firstGravityCenter, &firstGravityVector, nullptr, 0);
    firstGravityVector.negate();
    
    TVec3f secondGravityCenter(rSecondPoint);
    TVec3f secondGravityVector;
    MR::calcGravityVector(this, secondGravityCenter, &secondGravityVector, nullptr, 0);
    secondGravityVector.negate();

    f32 firstGravitySquared = firstGravityVector.squared();
    f32 secondGravitySquared = secondGravityVector.squared();
    f32 dotProduct = firstGravityVector.dot(secondGravityVector);
    f32 dotProductSquared = dotProduct * dotProduct;
    
    TVec3f distanceVector(firstGravityCenter - secondGravityCenter);
    TVec3f reversedDistanceVector(secondGravityCenter - firstGravityCenter);
    f32 dotFirst = distanceVector.dot(secondGravityVector);
    f32 dotSecond = reversedDistanceVector.dot(firstGravityVector);

    if (((firstGravitySquared * secondGravitySquared) - dotProductSquared) < 0.001f) {
        return false;
    }

    f32 scaleFactor = ((1.0f / ((firstGravitySquared * secondGravitySquared) - dotProductSquared)) * ((secondGravitySquared * dotSecond) + (dotProduct * dotFirst)));
    pOut->setInline(firstGravityCenter + MR::createVecAndScale(firstGravityVector, scaleFactor));
    return true; 
}

void SkeletalFishBoss::resetRail() {
    SkeletalFishBossInfo::LevelStatus* lvl = mBossInfo->getLevelStatus(_110);
    SkeletalFishBossRail* rail = MR::getSkeletalFishBossRailHolder()->getByID(lvl->mRailIDLevel);
    mRailControl->setRailActor(rail, nullptr, true);
    mRailControl->_14 = mBossInfo->getLevelStatus(_110)->mSpeedLevel;
}

void SkeletalFishBoss::updateBgm() {
    s32 curType = _1B4;
    if (curType < 0 || (_1B4 = curType - 1, curType - 1 < 0)) {
        _1B4 = -1;
        s32 bgType, v4;

        if (MR::calcDistanceToPlayer(this) <= 4200.0f) {
            
            bgType = 2;
            v4 = 60;
        }
        else {
            bgType = 1;
            v4 = 120;
        }

        if (bgType != _1B0) {
            MR::setStageBGMState(bgType, v4);
        }

        _1B0 = bgType;
    }
}

LiveActor* SkeletalFishBoss::getCurrentBossRail() {
    return mRailControl->_8;
}

void SkeletalFishBoss::stopScene(const char *pName, const Nerve *pNerve, SceneFunc func) {
    setNerve(&::SkeletalFishBossNrvDemoWait::sInstance);
    mSceneFunc = func;
    mCurScene = pName;
    mSceneNerve = pNerve;
}

void SkeletalFishBoss::startCamera(const char *pCameraName) {
    ActorCameraInfo cameraInfo(-1, 0);
    CameraTargetArg target(nullptr, mCameraTargetMtx, nullptr, nullptr);
    MR::startEventCamera(&cameraInfo, pCameraName, target, 0);
}

// SkeletalFishBoss::resetCamera

void SkeletalFishBoss::playDamageBrk() {
    char buf[0x80];
    snprintf(buf, sizeof(buf), "Damage%dColor", _118);
    MR::startBrk(this, buf);
    MR::startBrk(mBossHead, buf);
    _118 += 1;
}

void SkeletalFishBoss::startAppearDemo() {
    MR::requestMovementOn(MR::getSceneObj<SensorHitChecker*>(SceneObj_SensorHitChecker));
    Mtx namePosMtx;
    if (MR::tryFindNamePos("マリオ再セット位置1", namePosMtx)) {
        MR::setPlayerBaseMtx(namePosMtx);
    }

    MR::hidePlayer();
    startCamera("スカルシャーク出現");

    if (!MR::isNormalBloomOn()) {
        MR::turnOnNormalBloom();
        MR::setNormalBloomIntensity(0xD5);
        MR::setNormalBloomThreshold(0);
        MR::setNormalBloomBlurIntensity1(0x2A);
        MR::setNormalBloomBlurIntensity2(0x15);
    }
}

void SkeletalFishBoss::endAppearDemo() {
    Mtx namePosMtx;
    if (MR::tryFindNamePos("マリオ再セット位置1", namePosMtx)) {
        MR::setPlayerBaseMtx(namePosMtx);
    }

    MR::showPlayer();
    const char* cameraName = "スカルシャーク出現";
    ActorCameraInfo cameraInfo(-1, 0);  
    MR::endEventCamera(&cameraInfo, cameraName, false, 0);
    resetCamera();

    if (cameraName != nullptr) {
        MR::endDemo(this, cameraName);
    }

    MR::startBck(this, "Swim", nullptr);
    MR::startBck(mBossHead, "Wait", nullptr);
    resetRail();
    setNerve(&::SkeletalFishBossNrvSwim::sInstance);
}

void SkeletalFishBoss::startPowerUpDemo() {
    MR::requestMovementOn(MR::getSceneObj<SensorHitChecker*>(SceneObj_SensorHitChecker));
    MR::overlayWithPreviousScreen(2);
    MR::hidePlayer();

    if (!_110) {
        startCamera("スカルシャークパワーアップ");
    }
    else {
        startCamera("スカルシャークパワーアップ２");
    }
}

void SkeletalFishBoss::endPowerUpDemo() {
    Mtx namePosMtx;
    if (MR::tryFindNamePos("マリオ再セット位置2", namePosMtx)) {
        MR::setPlayerBaseMtx(namePosMtx);
    }

    MR::showPlayer();

    const char* eventCameraName;

    if (_110 == 1) {
        eventCameraName = "スカルシャークパワーアップ";
        ActorCameraInfo cameraInfo(-1, 0);
        MR::endEventCamera(&cameraInfo, eventCameraName, false, 0);
        resetCamera();
        mBossDirector->endPowerUpDemo1();
    }
    else {
        eventCameraName = "スカルシャークパワーアップ２";
        ActorCameraInfo cameraInfo(-1, 0);
        MR::endEventCamera(&cameraInfo, eventCameraName, false, 0);
        resetCamera();
        mBossDirector->endPowerUpDemo2();
    }

    // alright
    if ("スカルシャークパワーアップ" != nullptr) {
        MR::endDemo(this, "スカルシャークパワーアップ");
    }

    resetRail();
    MR::startBck(this, "Swim", nullptr);
    MR::startBck(mBossHead, "Wait", 0);
    mGuardHolder->forceAppearAll();
    setNerve(&::SkeletalFishBossNrvSwim::sInstance);
}

void SkeletalFishBoss::startDeadDemo() {
    MR::overlayWithPreviousScreen(2);
    MR::hidePlayer();
    startCamera("スカルシャーク死亡");
}

void SkeletalFishBoss::endBreakDemo() {
    const char* cameraName = "スカルシャーク死亡";
    ActorCameraInfo info(-1, 0);
    MR::endEventCamera(&info, cameraName, false, 0);
    resetCamera();

    if (cameraName != nullptr) {
        MR::endDemo(this, cameraName);
    }

    MR::showPlayer();
    mBreakModel->makeActorDead();
    MR::hideModel(this);
    TVec3f offset(0.0f, 1573.0f, 0.0f);
    _150.mult(offset, offset);
    MR::requestAppearPowerStar(this, offset);
}

bool SkeletalFishBoss::isEnableToBeDamaged() const {
    bool isDmg = false;

    if (!isNerve(&::SkeletalFishBossNrvDamage::sInstance)
        && !isNerve(&::SkeletalFishBossNrvDown::sInstance)
        && !isNerve(&::SkeletalFishBossNrvDeadDamage::sInstance)
        && !isNerve(&::SkeletalFishBossNrvDead::sInstance)
        && !isNerve(&::SkeletalFishBossNrvDemoWait::sInstance)
        && !isNerve(&::SkeletalFishBossNrvAppearDemo::sInstance)
        && !isNerve(&::SkeletalFishBossNrvPowerUpDemo::sInstance)
        && !isNerve(&::SkeletalFishBossNrvDeadDemo::sInstance)) {
            isDmg = true;
    }

    return isDmg;
}

SkeletalFishBossScarFlash::SkeletalFishBossScarFlash(LiveActor *pActor) : PartsModel(pActor, "スカルシャーク傷跡エフェクトモデル", "SkeletalFishBossScarFlash", nullptr, 0x12, false) {
    initFixedPosition("Head");
}

void SkeletalFishBossScarFlash::init(const JMapInfoIter &rIter) {
    PartsModel::init(rIter);
    makeActorDead();
}

void SkeletalFishBossScarFlash::appear() {
    LiveActor::appear();
    MR::startBck(this, "Damage", nullptr);
    MR::startBrk(this, "Damage");
}

void SkeletalFishBossScarFlash::control() {
    if (MR::isBckStopped(this)) {
        kill();
    }
}

namespace {
    INIT_NERVE(SkeletalFishBossNrvSwim);
    INIT_NERVE(SkeletalFishBossNrvOpen);
    INIT_NERVE(SkeletalFishBossNrvOpenWait);
    INIT_NERVE(SkeletalFishBossNrvClose);
    INIT_NERVE(SkeletalFishBossNrvBite);
    INIT_NERVE(SkeletalFishBossNrvDamage);
    INIT_NERVE(SkeletalFishBossNrvDown);
    INIT_NERVE(SkeletalFishBossNrvDeadDamage);
    INIT_NERVE(SkeletalFishBossNrvDead);
    INIT_NERVE(SkeletalFishBossNrvAppearWait);
    INIT_NERVE(SkeletalFishBossNrvAppearDemo);
    INIT_NERVE(SkeletalFishBossNrvPowerUpDemo);
    INIT_NERVE(SkeletalFishBossNrvDeadDemo);
    INIT_NERVE(SkeletalFishBossNrvBreakDemo);
    INIT_NERVE(SkeletalFishBossNrvDemoWait);
};

SkeletalFishBoss::~SkeletalFishBoss() {

}

SkeletalFishBossHead::~SkeletalFishBossHead() {

}

SkeletalFishBossScarFlash::~SkeletalFishBossScarFlash() {

}