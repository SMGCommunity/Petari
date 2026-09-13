#include "Game/Enemy/Dossun.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

void Dossun_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

namespace {
    // sShadowBoxSize
    const f32 sUpperHeight = 1000.0f;
    const s32 sUpperFrame = 180;
    const f32 sFallingSpeed = 30.0f;
    const s32 sOnGroundFrame = 120;
    const f32 sRisingSpeed = 10.0f;
    const f32 sQuickRatio = 2.5f;
    const f32 sCamShakeDistanceStrong = 2000.0f;
    const f32 sCamShakeDistanceWeak = 3500.0f;
    const f32 sShadowDropDepth = 0.0f;
}  // namespace

namespace NrvDossun {
    NEW_NERVE(DossunNrvReady, Dossun, Ready);
    NEW_NERVE(DossunNrvUpper, Dossun, Upper);
    NEW_NERVE(DossunNrvFallSign, Dossun, FallSign);
    NEW_NERVE(DossunNrvFalling, Dossun, Falling);
    NEW_NERVE(DossunNrvOnGround, Dossun, OnGround);
    NEW_NERVE(DossunNrvRising, Dossun, Rising);
};  // namespace NrvDossun

Dossun::Dossun(const char* pName) : LiveActor(pName) {
    _8C.x = 0.0f;
    _8C.y = 0.0f;
    _8C.z = 0.0f;
    _98.x = 0.0f;
    _98.y = 0.0f;
    _98.z = 0.0f;
    mFallingTime = 0;
    mHoldTime = 0;
    mMovementDist = sUpperHeight;
    mRisenWaitTime = sUpperFrame;
    mShadowType = -1;
    mHasShadow = false;
}

void Dossun::init(const JMapInfoIter& rIter) {
    MR::useStageSwitchReadA(this, rIter);
    MR::useStageSwitchSleep(this, rIter);
    initMapToolInfo(rIter);
    initModelManagerWithAnm("Dossun", nullptr, false);
    MR::startBva(this, "Wait");
    if (mHasShadow) {
        MR::connectToSceneCollisionEnemyStrongLight(this);
    } else {
        MR::connectToSceneCollisionEnemyNoShadowedMapObjStrongLight(this);
    }

    MR::initLightCtrl(this);
    initHitSensor(1);
    MR::initCollisionParts(this, "Dossun", MR::addBodyMessageSensorMapObjPress(this), nullptr);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    initShadow();
    initNerve(GET_NERVE(Dossun, DossunNrvUpper));
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    s16 frameMax = MR::getBckFrameMax(this, "FallStart");
    mRisenWaitTime = (mRisenWaitTime - frameMax <= 0) ? 0 : (mRisenWaitTime - frameMax);
    appear();
}

void Dossun::appear() {
    LiveActor::appear();
    getSensor("body")->invalidate();
    calcParameters();
    mPosition.set< f32 >(_8C);
    if (MR::isValidSwitchA(this)) {
        setNerve(GET_NERVE(Dossun, DossunNrvReady));
    } else {
        setNerve(GET_NERVE(Dossun, DossunNrvUpper));
    }
}

void Dossun::initMapToolInfo(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &mMovementDist);
    MR::getJMapInfoArg1NoInit(rIter, &mRisenWaitTime);
    MR::getJMapInfoArg2NoInit(rIter, &mShadowType);
    s32 shadowType = -1;
    MR::getJMapInfoArg3NoInit(rIter, &shadowType);
    mHasShadow = shadowType != -1;
    MR::setGroupClipping(this, rIter, 16);
    _98.set< f32 >(mPosition);
}

void Dossun::initShadow() {
    if (mShadowType != 1) {
        calcParameters();
        if (mShadowType == 0) {
            MR::initShadowFromCSV(this, "Shadow2D");
        } else {
            MR::initShadowFromCSV(this, "Shadow");
            MR::excludeCalcShadowToMyCollision(this, nullptr);
        }
    }
}

void Dossun::calcParameters() {
    TPos3f mtx;
    mtx.set(getBaseMtx());
    TVec3f offset(0.0f, mMovementDist, 0.0f);
    mtx.mult33(offset);
    _8C.add(_98, offset);

    mFallingTime = static_cast< s16 >(mMovementDist / (MR::isGalaxyQuickCometAppearInCurrentStage() ? sFallingSpeed * sQuickRatio : sFallingSpeed));
    mHoldTime = static_cast< s16 >(mMovementDist / (MR::isGalaxyQuickCometAppearInCurrentStage() ? sRisingSpeed * sQuickRatio : sRisingSpeed));
}

s32 Dossun::getUpperFrame() const {
    if (MR::isGalaxyQuickCometAppearInCurrentStage()) {
        f32 frame = MR::getBckFrameMax(this, "FallStart") / sQuickRatio;
        s32 waitFrames = mRisenWaitTime / sQuickRatio;
        if (waitFrames - static_cast< s32 >(frame) <= 0) {
            return 0;
        } else {
            return waitFrames - static_cast< s32 >(frame);
        }
    }

    return mRisenWaitTime;
}

void Dossun::exeReady() {
    if (MR::isOnSwitchA(this)) {
        setNerve(GET_NERVE(Dossun, DossunNrvFallSign));
    }
}

void Dossun::exeUpper() {
    if (MR::isFirstStep(this)) {
        mPosition.set< f32 >(_8C);
    }

    if (MR::isStep(this, getUpperFrame())) {
        setNerve(GET_NERVE(Dossun, DossunNrvFallSign));
    }
}

void Dossun::exeFallSign() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "FallStart", nullptr);
        MR::startBva(this, "Attack");

        if (MR::isGalaxyQuickCometAppearInCurrentStage()) {
            MR::setBckRate(this, sQuickRatio);
            MR::setBvaRate(this, sQuickRatio);
        }

        MR::startSound(this, "SE_OJ_DOSSUN_FALL_SIGN");
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(Dossun, DossunNrvFalling));
    }
}

void Dossun::exeFalling() {
    f32 ease = MR::getEaseInValue(getNerveStep(), 0.0f, 1.0f, mFallingTime);
    mPosition.lerp(_8C, _98, ease);
    MR::startLevelSound(this, "SE_OJ_LV_DOSSUN_FALL");
    if (MR::isStep(this, mFallingTime)) {
        setNerve(GET_NERVE(Dossun, DossunNrvOnGround));
    }
}

void Dossun::exeOnGround() {
    if (MR::isFirstStep(this)) {
        mPosition.set< f32 >(_98);
        MR::startRumbleWithShakeCameraNormalWeak(this, "中", "弱", sCamShakeDistanceStrong, sCamShakeDistanceWeak);
        MR::startSound(this, "SE_OJ_DOSSUN_LAND");
        MR::emitEffect(this, "Land");
    }

    bool b = MR::isGalaxyQuickCometAppearInCurrentStage();
    if (MR::isStep(this, b ? static_cast< s32 >(sOnGroundFrame / sQuickRatio) : sOnGroundFrame)) {
        setNerve(GET_NERVE(Dossun, DossunNrvRising));
    }
}

void Dossun::exeRising() {
    if (MR::isFirstStep(this)) {
        MR::startBva(this, "Wait");
    }

    f32 ease = MR::getEaseInOutValue(getNerveStep(), 0.0f, 1.0f, mHoldTime);
    mPosition.lerp(_98, _8C, ease);
    MR::startLevelSound(this, "SE_OJ_LV_DOSSUN_UPPER");
    if (MR::isStep(this, mHoldTime)) {
        MR::startSound(this, "SE_OJ_DOSSUN_STOP");
        setNerve(GET_NERVE(Dossun, DossunNrvUpper));
    }
}
