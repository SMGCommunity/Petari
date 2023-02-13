#include "Game/MapObj/WatchTowerRotateStep.h"
#include "Game/Util.h"
#include "Inline.h"

NrvWatchTowerRotateStep::WatchTowerRotateStepNrvWait NrvWatchTowerRotateStep::WatchTowerRotateStepNrvWait::sInstance;
NrvWatchTowerRotateStep::WatchTowerRotateStepNrvMoveStart NrvWatchTowerRotateStep::WatchTowerRotateStepNrvMoveStart::sInstance;
NrvWatchTowerRotateStep::WatchTowerRotateStepNrvMove NrvWatchTowerRotateStep::WatchTowerRotateStepNrvMove::sInstance;

WatchTowerRotateStep::WatchTowerRotateStep(const char *pName) : LiveActor(pName) {
    mRotDeg.x = 0.0f;
    mRotDeg.y = 0.0f;
    mRotDeg.z = 0.0f;
    mLifts = nullptr;
}

void WatchTowerRotateStep::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("WatchTowerRotateStep", nullptr, false);
    MR::connectToSceneMapObj(this);

    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionParts(this, "WatchTowerRotateStep", getSensor(nullptr), nullptr);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false); 
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 1500.0f);
    
    bool demoCast = MR::tryRegisterDemoCast(this, rIter);

    if (demoCast) {
        MR::registerDemoActionNerve(this, &NrvWatchTowerRotateStep::WatchTowerRotateStepNrvWait::sInstance, nullptr);
    }

    MR::calcUpVec(&mRotDeg, this);
    initLift(rIter);

    if (demoCast) {
        initNerve(&NrvWatchTowerRotateStep::WatchTowerRotateStepNrvWait::sInstance);
    }
    else {
        initNerve(&NrvWatchTowerRotateStep::WatchTowerRotateStepNrvMove::sInstance);
    }

    makeActorAppeared();
}

void WatchTowerRotateStep::calcAndSetBaseMtx() {
    TVec3f frontVec;
    TPos3f baseMtx;

    MR::calcFrontVec(&frontVec, this);
    MR::makeMtxFrontUpPos(&baseMtx, frontVec, mRotDeg, mPosition);
    MR::setBaseTRMtx(this, baseMtx);
}

void WatchTowerRotateStep::initLift(const JMapInfoIter &rIter) {
    mLifts = new PartsModel*[4];

    for (s32 i = 0; i < 4; i++) {
        MtxPtr mtx = MR::getJointMtx(this, i + 1);
        mLifts[i] = new PartsModel(this, "物見の塔リフト", "WatchTowerRotateStepLift", mtx, -1, false);
        mLifts[i]->mCalcOwnMtx = false;

        MR::initCollisionParts(mLifts[i], "WatchTowerRotateStepLift", getSensor(nullptr), nullptr);
        TVec3f local68 = CALL_INLINE_FUNC(TVec3f, 600.0f, 200.0f, 400.0f);
        MR::initShadowVolumeBox(mLifts[i], local68, mLifts[i]->getBaseMtx());
        MR::setShadowVolumeStartDropOffset(mLifts[i], "WatchTowerRotateStepLift", 300.0f);
        MR::setShadowDropLength(mLifts[i], "WatchTowerRotateStepLift", 370.0f);

        if (MR::isDemoCast(this, nullptr)) {
            MR::tryRegisterDemoCast(mLifts[i], rIter);
        }

        mLifts[i]->initWithoutIter();
    }
}

// WatchTowerRotateStep::attachLift();

void WatchTowerRotateStep::exeMoveStart() {
    TVec3f upVec;

    if (MR::isFirstStep(this)) {
        MR::startSystemSE("SE_SY_READ_RIDDLE_S", -1, -1);
        MR::startSound(this, "SE_OJ_WATCH_TOWER_START", -1, -1);
    }

    MR::startLevelSound(this, "SE_OJ_LV_WATCH_TOWER_ROTATE", -1, -1, -1);
    f32 easeIn = MR::getEaseInValue((s32)getNerveStep(), 0.0f, 0.3f, 180.0f);
    MR::calcFrontVec(&upVec, this);
    MR::rotateVecDegree(&mRotDeg, upVec, easeIn);
    attachLift();

    if (MR::isStep(this, 180)) {
        setNerve(&NrvWatchTowerRotateStep::WatchTowerRotateStepNrvMove::sInstance);
    }
}

namespace NrvWatchTowerRotateStep {
    void WatchTowerRotateStepNrvMove::execute(Spine *pSpine) const {
        WatchTowerRotateStep* pActor = reinterpret_cast<WatchTowerRotateStep*>(pSpine->mExecutor);
        TVec3f frontVec;

        MR::calcFrontVec(&frontVec, pActor);
        MR::rotateVecDegree(&pActor->mRotDeg, frontVec, 0.3f);
        pActor->attachLift();
        MR::startLevelSound(pActor, "SE_OJ_LV_WATCH_TOWER_ROTATE", -1, -1, -1);
    }

    void WatchTowerRotateStepNrvMoveStart::execute(Spine *pSpine) const {
        reinterpret_cast<WatchTowerRotateStep*>(pSpine->mExecutor)->exeMoveStart();
    }

    void WatchTowerRotateStepNrvWait::execute(Spine *pSpine) const {}
};