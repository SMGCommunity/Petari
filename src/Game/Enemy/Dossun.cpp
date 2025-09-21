#include "Game/Enemy/Dossun.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace NrvDossun {
    NEW_NERVE(DossunNrvReady, Dossun, Ready);
    NEW_NERVE(DossunNrvUpper, Dossun, Upper);
    NEW_NERVE(DossunNrvFallSign, Dossun, FallSign);
    NEW_NERVE(DossunNrvFalling, Dossun, Falling);
    NEW_NERVE(DossunNrvOnGround, Dossun, OnGround);
    NEW_NERVE(DossunNrvRising, Dossun, Rising);
};

Dossun::Dossun(const char *pName) : LiveActor(pName) {
    _8C.x = 0.0f;
    _8C.y = 0.0f;
    _8C.z = 0.0f;
    _98.x = 0.0f;
    _98.y = 0.0f;
    _98.z = 0.0f;
    mFallingTime = 0;
    mHoldTime = 0;
    mMovementDist = 1000.0f;
    mRisenWaitTime = 180;
    mShadowType = -1;
    mHasShadow = false;
}

void Dossun::init(const JMapInfoIter &rIter) {
    MR::useStageSwitchReadA(this, rIter);
    MR::useStageSwitchSleep(this, rIter);
    initMapToolInfo(rIter);
    initModelManagerWithAnm("Dossun", nullptr, false);
    MR::startBva(this, "Wait");
    if (mHasShadow) {
        MR::connectToSceneCollisionEnemyStrongLight(this);
    }
    else {
        MR::connectToSceneCollisionEnemyNoShadowedMapObjStrongLight(this);
    }

    MR::initLightCtrl(this);
    initHitSensor(1);
    MR::initCollisionParts(this, "Dossun", MR::addBodyMessageSensorMapObjPress(this), nullptr);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    initShadow();
    initNerve(&NrvDossun::DossunNrvUpper::sInstance);
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    s16 frameMax = MR::getBckFrameMax(this, "FallStart");
    mRisenWaitTime = (mRisenWaitTime - frameMax <= 0) ? 0 : (mRisenWaitTime - frameMax);
    appear();
}

void Dossun::appear() {
    LiveActor::appear();
    getSensor("body")->invalidate();
    calcParameters();
    mPosition.set<f32>(_8C);
    if (MR::isValidSwitchA(this)) {
        setNerve(&NrvDossun::DossunNrvReady::sInstance);
    }
    else {
        setNerve(&NrvDossun::DossunNrvUpper::sInstance);
    }
}

void Dossun::initMapToolInfo(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &mMovementDist);
    MR::getJMapInfoArg1NoInit(rIter, &mRisenWaitTime);
    MR::getJMapInfoArg2NoInit(rIter, &mShadowType);
    s32 shadowType = -1;
    MR::getJMapInfoArg3NoInit(rIter, &shadowType);
    mHasShadow = shadowType != -1;
    MR::setGroupClipping(this, rIter, 16);
    _98.set<f32>(mPosition);
}

void Dossun::initShadow() {
    if (mShadowType != 1) {
        calcParameters();
        if (mShadowType == 0) {
            MR::initShadowFromCSV(this, "Shadow2D");
        }
        else {
            MR::initShadowFromCSV(this, "Shadow");
            MR::excludeCalcShadowToMyCollision(this, nullptr);
        }
    }
}

/*
void Dossun::calcParameters() {
    
}
*/

s32 Dossun::getUpperFrame() const {
    if (MR::isGalaxyQuickCometAppearInCurrentStage()) {
        f32 frame = MR::getBckFrameMax(this, "FallStart") / 2.5f;
        int r5 = mRisenWaitTime / 2.5f;
        if (r5 - (int)frame <= 0) {
            return 0;
        } else {
            return r5 - (int)frame;
        }
    }

    return mRisenWaitTime;
}

void Dossun::exeUpper() {
    if (MR::isFirstStep(this)) {
        mPosition.set<f32>(_8C);
    }

    if (MR::isStep(this, getUpperFrame())) {
        setNerve(&NrvDossun::DossunNrvFallSign::sInstance);
    }
}

void Dossun::exeFallSign() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "FallStart", nullptr);
        MR::startBva(this, "Attack");

        if (MR::isGalaxyQuickCometAppearInCurrentStage()) {
            MR::setBckRate(this, 2.5f);
            MR::setBvaRate(this, 2.5f);
        }

        MR::startSound(this, "SE_OJ_DOSSUN_FALL_SIGN", -1, -1);
    }
    
    if (MR::isBckStopped(this)) {
        setNerve(&NrvDossun::DossunNrvFalling::sInstance);
    }
}

void Dossun::exeFalling() {
    f32 ease = MR::getEaseInValue(getNerveStep(), 0.0f, 1.0f, mFallingTime);
    JMAVECLerp(&_8C, &_98, &mPosition, ease);
    MR::startLevelSound(this, "SE_OJ_LV_DOSSUN_FALL", -1, -1, -1);
    if (MR::isStep(this, mFallingTime)) {
        setNerve(&NrvDossun::DossunNrvOnGround::sInstance);
    }
}

void Dossun::exeOnGround() {
    if (MR::isFirstStep(this)) {
        mPosition.set<f32>(_98);
        MR::startRumbleWithShakeCameraNormalWeak(this, "中", "弱", 2000.0f, 3500.0f);
        MR::startSound(this, "SE_OJ_DOSSUN_LAND", -1, -1);
        MR::emitEffect(this, "Land");
    }

    bool b = MR::isGalaxyQuickCometAppearInCurrentStage();
    if (MR::isStep(this, b ? 0x30 : 0x78)) {
        setNerve(&NrvDossun::DossunNrvRising::sInstance);
    }
}

void Dossun::exeRising() {
    if (MR::isFirstStep(this)) {
        MR::startBva(this, "Wait");
    }

    f32 ease = MR::getEaseInOutValue(getNerveStep(), 0.0f, 1.0f, mHoldTime);
    JMAVECLerp(&_98, &_8C, &mPosition, ease);
    MR::startLevelSound(this, "SE_OJ_LV_DOSSUN_UPPER", -1, -1, -1);
    if (MR::isStep(this, mHoldTime)) {
        MR::startSound(this, "SE_OJ_DOSSUN_STOP", -1, -1);
        setNerve(&NrvDossun::DossunNrvUpper::sInstance);
    }
}

void Dossun::exeReady() {
    if (MR::isOnSwitchA(this)) {
        setNerve(&NrvDossun::DossunNrvFallSign::sInstance);
    }
}

Dossun::~Dossun() {
    
}
