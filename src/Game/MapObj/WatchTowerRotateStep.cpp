#include "Game/MapObj/WatchTowerRotateStep.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Util.hpp"

#define LIFT_NUM 4

namespace {
    static const f32 sRotateSpeed = 0.3f;
    static const s32 sStepForMoveStart = 180;
    static const f32 sClippingRadiusOffset = 1500.0f;
};  // namespace

namespace NrvWatchTowerRotateStep {
    NEW_NERVE(WatchTowerRotateStepNrvWait, WatchTowerRotateStep, Wait);
    NEW_NERVE(WatchTowerRotateStepNrvMoveStart, WatchTowerRotateStep, MoveStart);
    NEW_NERVE(WatchTowerRotateStepNrvMove, WatchTowerRotateStep, Move);
};  // namespace NrvWatchTowerRotateStep

WatchTowerRotateStep::WatchTowerRotateStep(const char* pName) : LiveActor(pName), mRotateVec(0.0f, 0.0f, 0.0f), mLift() {
}

void WatchTowerRotateStep::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("WatchTowerRotateStep", nullptr, false);
    MR::connectToSceneMapObj(this);

    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionParts(this, "WatchTowerRotateStep", getSensor(nullptr), nullptr);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    MR::setClippingTypeSphereContainsModelBoundingBox(this, ::sClippingRadiusOffset);

    bool isRegisteredDemoCast = MR::tryRegisterDemoCast(this, rIter);

    if (isRegisteredDemoCast) {
        MR::registerDemoActionNerve(this, &NrvWatchTowerRotateStep::WatchTowerRotateStepNrvMoveStart::sInstance, nullptr);
    }

    MR::calcUpVec(&mRotateVec, this);
    initLift(rIter);

    if (isRegisteredDemoCast) {
        initNerve(&NrvWatchTowerRotateStep::WatchTowerRotateStepNrvWait::sInstance);
    } else {
        initNerve(&NrvWatchTowerRotateStep::WatchTowerRotateStepNrvMove::sInstance);
    }

    makeActorAppeared();
}

void WatchTowerRotateStep::exeWait() {
}

void WatchTowerRotateStep::exeMoveStart() {
    if (MR::isFirstStep(this)) {
        MR::startSystemSE("SE_SY_READ_RIDDLE_S");
        MR::startSound(this, "SE_OJ_WATCH_TOWER_START");
    }

    MR::startLevelSound(this, "SE_OJ_LV_WATCH_TOWER_ROTATE");

    f32 degree = MR::getEaseInValue(getNerveStep(), 0.0f, ::sRotateSpeed, 180.0f);
    TVec3f upVec;
    MR::calcFrontVec(&upVec, this);
    MR::rotateVecDegree(&mRotateVec, upVec, degree);

    attachLift();

    if (MR::isStep(this, ::sStepForMoveStart)) {
        setNerve(&NrvWatchTowerRotateStep::WatchTowerRotateStepNrvMove::sInstance);
    }
}

void WatchTowerRotateStep::exeMove() {
    TVec3f frontVec;
    MR::calcFrontVec(&frontVec, this);
    MR::rotateVecDegree(&mRotateVec, frontVec, ::sRotateSpeed);

    attachLift();
    MR::startLevelSound(this, "SE_OJ_LV_WATCH_TOWER_ROTATE");
}

void WatchTowerRotateStep::calcAndSetBaseMtx() {
    TVec3f frontVec;
    MR::calcFrontVec(&frontVec, this);

    TPos3f baseMtx;
    MR::makeMtxFrontUpPos(&baseMtx, frontVec, mRotateVec, mPosition);
    MR::setBaseTRMtx(this, baseMtx);
}

void WatchTowerRotateStep::initLift(const JMapInfoIter& rIter) {
    mLift = new PartsModel*[LIFT_NUM];

    for (s32 i = 0; i < LIFT_NUM; i++) {
        MtxPtr mtx = MR::getJointMtx(this, i + 1);

        mLift[i] = new PartsModel(this, "物見の塔リフト", "WatchTowerRotateStepLift", mtx, -1, false);
        mLift[i]->mCalcOwnMtx = false;

        MR::initCollisionParts(mLift[i], "WatchTowerRotateStepLift", getSensor(nullptr), nullptr);
        MR::initShadowVolumeBox(mLift[i], TVec3f(600.0f, 200.0f, 400.0f), mLift[i]->getBaseMtx());
        MR::setShadowVolumeStartDropOffset(mLift[i], "WatchTowerRotateStepLift", 300.0f);
        MR::setShadowDropLength(mLift[i], "WatchTowerRotateStepLift", 370.0f);

        if (MR::isDemoCast(this, nullptr)) {
            MR::tryRegisterDemoCast(mLift[i], rIter);
        }

        mLift[i]->initWithoutIter();
    }
}

void WatchTowerRotateStep::attachLift() {
    for (s32 i = 0; i < LIFT_NUM; i++) {
        TPos3f mtx;
        mtx.set(MR::getJointMtx(this, i + 1));

        TVec3f pos;
        mtx.getTrans(pos);

        mLift[i]->mPosition.set(pos);
    }
}
