#include "Game/MapObj/ShockWaveGenerator.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

void ShockWaveGenerator_FORCE_MATCH_SDATA2() {
    (void)0.0f;
}

namespace {
    static const char* cDemoCameraName = "衝撃波カメラ";
    static const s32 sStepForGenerate = 50;
    static const s32 sStepForDemoEcho = 1;
    static const f32 sHitCylinderRadius = 400.0f;
    static const f32 sEnableCameraStartDistance = 2000.0f;
};  // namespace

namespace NrvShockWaveGenerator {
    NEW_NERVE(ShockWaveGeneratorNrvWait, ShockWaveGenerator, Wait);
    NEW_NERVE(ShockWaveGeneratorNrvDemoEcho, ShockWaveGenerator, DemoEcho);
    NEW_NERVE(ShockWaveGeneratorNrvGenerate, ShockWaveGenerator, Generate);
};  // namespace NrvShockWaveGenerator

ShockWaveGenerator::ShockWaveGenerator(const char* pName) : LiveActor(pName), mCameraInfo() {
}

void ShockWaveGenerator::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("ShockWaveGenerator", nullptr, false);
    MR::connectToSceneIndirectMapObj(this);
    initHitSensor(3);
    MR::addBodyMessageSensorMapObj(this);
    MR::addHitSensorMapObjSimple(this, "spin", 8, ::sHitCylinderRadius, TVec3f(0.0f, 200.0f, 0.0f));
    MR::addHitSensorEnemyAttack(this, "shock", 16, 1000.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::initCollisionParts(this, "ShockWaveGenerator", getSensor("body"), nullptr);

    if (!MR::initActorCamera(this, rIter, &mCameraInfo)) {
        mCameraInfo = nullptr;
    }

    initEffectKeeper(0, nullptr, false);
    initSound(2, false);

    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    MR::useStageSwitchSleep(this, rIter);
    initNerve(&NrvShockWaveGenerator::ShockWaveGeneratorNrvWait::sInstance);
    makeActorAppeared();
}

void ShockWaveGenerator::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "SpinHit");
        MR::startBtk(this, "SpinHit");
        MR::setBrkFrameAndStop(this, 0.0f);
        MR::setBtkFrameAndStop(this, 0.0f);
        MR::validateClipping(this);
    }
}

void ShockWaveGenerator::exeDemoEcho() {
    if (MR::isFirstStep(this)) {
        MR::startActorCameraTargetSelf(this, mCameraInfo, -1);
    }

    MR::tryRumblePadVeryWeak(this, WPAD_CHAN0);

    if (MR::isStep(this, ::sStepForDemoEcho)) {
        MR::endDemo(this, ::cDemoCameraName);
        setNerve(&NrvShockWaveGenerator::ShockWaveGeneratorNrvGenerate::sInstance);
    }
}

void ShockWaveGenerator::exeGenerate() {
    if (MR::isFirstStep(this)) {
        MR::startAllAnim(this, "SpinHit");
        sendMsgShockWaveToNearEnemy();
        MR::emitEffect(this, "ShockWave");
        MR::shakeCameraWeak();
        MR::tryRumblePadStrong(this, WPAD_CHAN0);
    }

    if (MR::isStep(this, ::sStepForGenerate)) {
        if (mCameraInfo != nullptr) {
            MR::endActorCamera(this, mCameraInfo, false, -1);
        }
        setNerve(&NrvShockWaveGenerator::ShockWaveGeneratorNrvWait::sInstance);
    }
}

bool ShockWaveGenerator::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    bool isActive = isNerve(&NrvShockWaveGenerator::ShockWaveGeneratorNrvGenerate::sInstance) ||
                    isNerve(&NrvShockWaveGenerator::ShockWaveGeneratorNrvDemoEcho::sInstance);

    if (isActive) {
        return false;
    }

    if (MR::isDemoActive()) {
        return false;
    }

    if (pReceiver == getSensor("body") && MR::isMsgStarPieceAttack(msg)) {
        startShockWave();
        return true;
    }

    if (pReceiver == getSensor("spin") && MR::isMsgPlayerSpinAttack(msg)) {
        if (isHitCylinder(pSender, pReceiver)) {
            startShockWave();
            return true;
        }
    }

    return false;
}

bool ShockWaveGenerator::isPlayerInCameraStartRange() const {
    return mCameraInfo != nullptr && MR::isNearPlayerAnyTime(this, ::sEnableCameraStartDistance);
}

void ShockWaveGenerator::startShockWave() {
    MR::invalidateClipping(this);
    MR::startSound(this, "SE_OJ_SHOCK_WAVE_GENERATE");
    if (mCameraInfo != nullptr) {
        if (isPlayerInCameraStartRange()) {
            if (MR::tryStartDemoWithoutCinemaFrame(this, ::cDemoCameraName)) {
                setNerve(&NrvShockWaveGenerator::ShockWaveGeneratorNrvDemoEcho::sInstance);
            }
        } else {
            setNerve(&NrvShockWaveGenerator::ShockWaveGeneratorNrvGenerate::sInstance);
        }
        return;
    }

    MR::stopSceneForDefaultHit(1);
    setNerve(&NrvShockWaveGenerator::ShockWaveGeneratorNrvGenerate::sInstance);
}

void ShockWaveGenerator::sendMsgShockWaveToNearEnemy() {
    HitSensor* other;
    HitSensor* shockSensor = getSensor("shock");
    for (s32 idx = 0; idx < shockSensor->mSensorCount; idx++) {
        other = shockSensor->mSensors[idx];
        if (MR::isSensorEnemy(other)) {
            MR::sendMsgToEnemyAttackShockWave(other, shockSensor);
        }
    }
}

bool ShockWaveGenerator::isHitCylinder(HitSensor* pSender, HitSensor* pReceiver) const {
    TVec3f diff;
    diff.sub(pSender->mPosition, pReceiver->mPosition);
    TVec3f up;
    MR::calcUpVec(&up, this);
    TVec3f horiz = diff.killElement(up);
    return horiz.length() <= pSender->getRadius() + 140.0f;
}
