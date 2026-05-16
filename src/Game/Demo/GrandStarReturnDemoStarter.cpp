#include "Game/Demo/GrandStarReturnDemoStarter.hpp"
#include "Game/Demo/AstroDemoFunction.hpp"
#include "Game/Demo/ReturnDemoRailMove.hpp"
#include "Game/MapObj/PowerStar.hpp"
#include "Game/Screen/StageResultInformer.hpp"
#include "Game/System/GameSequenceFunction.hpp"
#include <cstdio>

namespace {
    const char* cDemoMovePartName = "移動";
    const char* cDemoWaitPartName = "ウェイト→コア突入";
}  // namespace

GrandStarReturnDemoStarter::GrandStarReturnDemoStarter(const char* pName)
    : LiveActor(pName), mReturnDemoRailMove(), mStageResultInformer(), mPowerStar(), mDistanceToCore(0.0f, 0.0f, 1.0f), mPowerStarPosition(gZeroVec),
      mActorCameraInfo() {
    mPrevTransform.identity();
    mTransform.identity();
}

namespace NrvGrandStarReturnDemoStarter {
    NEW_NERVE(GrandStarReturnDemoStarterNrvMove, GrandStarReturnDemoStarter, Move);
    NEW_NERVE(GrandStarReturnDemoStarterNrvFlyWait, GrandStarReturnDemoStarter, FlyWait);
    NEW_NERVE(GrandStarReturnDemoStarterNrvRushToCore, GrandStarReturnDemoStarter, RushToCore);
    NEW_NERVE(GrandStarReturnDemoStarterNrvRevival, GrandStarReturnDemoStarter, Revival);
    NEW_NERVE(GrandStarReturnDemoStarterNrvStageResult, GrandStarReturnDemoStarter, StageResult);
    NEW_NERVE(GrandStarReturnDemoStarterNrvFadeOut, GrandStarReturnDemoStarter, FadeOut);
    NEW_NERVE(GrandStarReturnDemoStarterNrvWaitDemoEnd, GrandStarReturnDemoStarter, WaitDemoEnd);
}  // namespace NrvGrandStarReturnDemoStarter

void GrandStarReturnDemoStarter::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneMapObjMovement(this);
    MR::invalidateClipping(this);

    mStageResultInformer = new StageResultInformer();
    mStageResultInformer->initWithoutIter();

    mPowerStar = reinterpret_cast< PowerStar* >(MR::createModelObjNoSilhouettedMapObjStrongLight("スターデモモデル", "GrandStar", mTransform));
    MR::invalidateClipping(mPowerStar);
    mPowerStar->kill();

    mReturnDemoRailMove = new ReturnDemoRailMove(this, mPowerStar, rIter, true, &mPrevTransform);
    mReturnDemoRailMove->setupPathDrawForGraneStarReturnDemo();

    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::tryRegisterDemoCast(mPowerStar, rIter);
    } else {
        for (int i = 1; i <= 6; i++) {
            const char* pDemoName = AstroDemoFunction::getGrandStarReturnDemoName(i);
            if (MR::isDemoExist(pDemoName) && MR::tryRegisterDemoCast(this, pDemoName, rIter)) {
                MR::tryRegisterDemoCast(mPowerStar, pDemoName, rIter);
            }
        };
    }

    MR::initMultiActorCamera(this, rIter, &mActorCameraInfo, "移動");
    MR::initMultiActorCamera(this, rIter, &mActorCameraInfo, "ウェイト");
    MR::initMultiActorCamera(this, rIter, &mActorCameraInfo, "リザルト");

    for (int i = 0; i < 6; i++) {
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "DemoAstroReturn%d", i + 1);
        MR::initAnimCamera(this, mActorCameraInfo, "DemoAstroReturn.arc", buffer);
    }

    initNerve(&NrvGrandStarReturnDemoStarter::GrandStarReturnDemoStarterNrvMove::sInstance);
    makeActorDead();
}

void GrandStarReturnDemoStarter::appear() {
    LiveActor::appear();

    mPowerStar->appear();
    MR::showJointAndChildren(mPowerStar, "PowerStar");

    mReturnDemoRailMove->posToStart();
    mReturnDemoRailMove->start();

    PowerStar::setupColorAtResultSequence(mPowerStar, true);

    setNerve(&NrvGrandStarReturnDemoStarter::GrandStarReturnDemoStarterNrvMove::sInstance);
}

void GrandStarReturnDemoStarter::control() {
    if (isNerve(&NrvGrandStarReturnDemoStarter::GrandStarReturnDemoStarterNrvMove::sInstance) ||
        isNerve(&NrvGrandStarReturnDemoStarter::GrandStarReturnDemoStarterNrvFlyWait::sInstance)) {
        MR::setPlayerBaseMtx(mPrevTransform);

        mTransform.setInline(mPrevTransform);
    }

    if (!MR::isDead(mPowerStar)) {
        PowerStar::requestPointLightAtResultSequence(mPowerStar);
    }
}

void GrandStarReturnDemoStarter::calcOffsetStarToCore(TVec3f* pOffset) const {
    TVec3f namePos;
    TVec3f jointPos;

    MR::findNamePos("コア中心", &namePos, nullptr);
    MR::copyJointPos(mPowerStar, "PowerStar", &jointPos);

    pOffset->subInline(namePos, jointPos);
}

void GrandStarReturnDemoStarter::updateRailMoveEndDir() {
    TVec3f offset;
    calcOffsetStarToCore(&offset);
    MR::normalize(&offset);

    mReturnDemoRailMove->mForward.set(offset);
}

void GrandStarReturnDemoStarter::emitEffectRush() {
    MR::emitEffect(mPowerStar, "Blur");
    MR::emitEffect(mPowerStar, "Blur1");
    MR::emitEffect(mPowerStar, "Blur2");
    MR::emitEffect(mPowerStar, "Blur3");
    MR::emitEffect(mPowerStar, "Blur4");
    MR::emitEffect(mPowerStar, "Blur5");
}

void GrandStarReturnDemoStarter::updateRushStarPos(const TVec3f& rPosition, s32 frame) {
    TVec3f offset;
    TVec3f newPosition;

    offset.scale(100.0f * frame, mDistanceToCore);
    newPosition.add(rPosition, offset);
    mTransform.setTrans(newPosition);
}

void GrandStarReturnDemoStarter::tryStartStageResult(const char* pDemoName) {
    if (MR::isDemoPartLastStep(pDemoName)) {
        if (GameSequenceFunction::hasStageResultSequence()) {
            mStageResultInformer->appear();
            MR::requestMovementOn(mStageResultInformer);
        }

        MR::pauseTimeKeepDemo(this);
        setNerve(&NrvGrandStarReturnDemoStarter::GrandStarReturnDemoStarterNrvStageResult::sInstance);
    }
}

inline bool isActiveGrandStarReturnDemoIndex(s32 index) {
    return AstroDemoFunction::getActiveGrandStarReturnDemoIndex() == index;
}

void GrandStarReturnDemoStarter::exeMove() {
    if (MR::isFirstStep(this)) {
        if (isActiveGrandStarReturnDemoIndex(0)) {
            MR::startStageBGM("STM_FIRST_ASTRO", false);
        } else {
            MR::startStageBGM("STM_SECOND_ASTRO", false);
        }
    }

    if (!isActiveGrandStarReturnDemoIndex(0)) {
        if (MR::isFirstStep(this)) {
            MR::startMultiActorCameraTargetPlayer(this, mActorCameraInfo, "移動", -1);
        }

        if (MR::isDemoPartStep(cDemoMovePartName, 300)) {
            MR::startMultiActorCameraTargetPlayer(this, mActorCameraInfo, "ウェイト", -1);
        }
    }

    const char* pDemoName = cDemoMovePartName;
    mReturnDemoRailMove->update(MR::getDemoPartStep(pDemoName) + 1, MR::getDemoPartTotalStep(pDemoName));

    MR::startLevelSoundPlayer("SE_PM_LV_SPIN_DRV_FLY", -1);

    updateRailMoveEndDir();

    if (MR::isDemoPartLastStep(pDemoName)) {
        setNerve(&NrvGrandStarReturnDemoStarter::GrandStarReturnDemoStarterNrvFlyWait::sInstance);
    }
}

void GrandStarReturnDemoStarter::exeFlyWait() {
    if (MR::isFirstStep(this)) {
        updateRailMoveEndDir();
        mReturnDemoRailMove->posToEnd();
    }

    if (MR::isStep(this, 60)) {
        emitEffectRush();
    }

    if (MR::isLessStep(this, 60)) {
        MR::startLevelSound(mPowerStar, "SE_OJ_LV_RET_POW_STAR_FLY", -1, -1, -1);
    }

    if (MR::isBckOneTimeAndStoppedPlayer()) {
        setNerve(&NrvGrandStarReturnDemoStarter::GrandStarReturnDemoStarterNrvRushToCore::sInstance);
    }
}

void GrandStarReturnDemoStarter::exeRushToCore() {
    TVec3f position;
    mPrevTransform.getTransInline(position);

    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("ResultFlyGrandStarRush", reinterpret_cast< char* >(nullptr));
        MR::startBck(mPowerStar, "ResultFlyGrandStarRush", nullptr);
        MR::startSound(mPowerStar, "SE_OJ_GND_STAR_RUSH", -1, -1);

        MR::hideJointAndChildren(mPowerStar, "PowerStar");
        calcOffsetStarToCore(&mDistanceToCore);
        mPowerStarPosition.add(position, mDistanceToCore);
        MR::normalize(&mDistanceToCore);
    }

    updateRushStarPos(position, getNerveStep());
    MR::startLevelSound(mPowerStar, "SE_OJ_LV_GND_STAR_RUSH", -1, -1, -1);

    if (MR::isDemoPartLastStep(cDemoWaitPartName)) {
        setNerve(&NrvGrandStarReturnDemoStarter::GrandStarReturnDemoStarterNrvRevival::sInstance);
    }
}

void GrandStarReturnDemoStarter::exeRevival() {
    char buffer[32];
    int index = AstroDemoFunction::getActiveGrandStarReturnDemoIndex() + 1;
    snprintf(buffer, sizeof(buffer), "DemoAstroReturn%d", index);

    if (MR::isFirstStep(this)) {
        mReturnDemoRailMove->offPathDraw();
        MR::forceDeleteEffectAll(mPowerStar);
        emitEffectRush();

        mPosition.zero();
        mRotation.zero();

        MR::startAnimCameraTargetSelf(this, mActorCameraInfo, buffer, 0, 1.0f);
    }

    if (MR::isLessStep(this, 40)) {
        updateRushStarPos(mPowerStarPosition, -(40 - getNerveStep()));
        MR::startLevelSound(mPowerStar, "SE_OJ_LV_GND_STAR_RUSH", -1, -1, -1);
    }

    if (MR::isStep(this, 40)) {
        MR::startSound(mPowerStar, "SE_OJ_GND_STAR_ENTER_CORE", -1, -1);
        MR::forceDeleteEffect(mPowerStar, "DemoKoopaGrandStarVs3");
        mPowerStar->kill();
    }

    if (MR::isDemoPartLastStep("ドーム復活")) {
        MR::endAnimCamera(this, mActorCameraInfo, buffer, 0, true);
        MR::overlayWithPreviousScreen(2);
    }

    if (!MR::isDemoPartFirstStep("リザルト画面")) {
        MR::startMultiActorCameraTargetPlayer(this, mActorCameraInfo, "リザルト", -1);
    }

    tryStartStageResult("リザルト画面");
}

void GrandStarReturnDemoStarter::exeStageResult() {
    if (MR::isDead(mStageResultInformer)) {
        setNerve(&NrvGrandStarReturnDemoStarter::GrandStarReturnDemoStarterNrvFadeOut::sInstance);
    }
}

void GrandStarReturnDemoStarter::exeFadeOut() {
    if (MR::isFirstStep(this)) {
        MR::closeWipeFade(60);
    }

    if (!MR::isWipeActive()) {
        MR::endMultiActorCamera(this, mActorCameraInfo, "リザルト", false, -1);
        setNerve(&NrvGrandStarReturnDemoStarter::GrandStarReturnDemoStarterNrvWaitDemoEnd::sInstance);
    }
}

void GrandStarReturnDemoStarter::exeWaitDemoEnd() {
    if (MR::isFirstStep(this)) {
        if (isActiveGrandStarReturnDemoIndex(0)) {
            MR::startStageBGM("MBGM_GALAXY_24", false);
        } else if (isActiveGrandStarReturnDemoIndex(1)) {
            MR::startStageBGM("STM_ASTRO_OUT", false);
        } else if (isActiveGrandStarReturnDemoIndex(2)) {
            MR::startStageBGM("STM_ASTRO_OUT_2", false);
        } else if (isActiveGrandStarReturnDemoIndex(3)) {
            MR::startStageBGM("STM_ASTRO_OUT_2", false);
        } else {
            MR::startStageBGM("STM_ASTRO_OUT_3", false);
        }

        MR::resumeTimeKeepDemo(this);
    }

    if (!MR::isTimeKeepDemoActive()) {
        kill();
    }
}

GrandStarReturnDemoStarter::~GrandStarReturnDemoStarter() {
}
