#include "Game/Boss/PoltaDemo.hpp"
#include "Game/Boss/Polta.hpp"
#include "Game/Boss/PoltaActionBase.hpp"
#include "Game/Boss/PoltaArm.hpp"
#include "Game/Boss/PoltaFunction.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "revolution/types.h"

namespace NrvPoltaDemo {
    NEW_NERVE(PoltaDemoNrvTryStartDemo, PoltaDemo, TryStartDemo);
    NEW_NERVE(PoltaDemoNrvOpeningDemo, PoltaDemo, OpeningDemo);
    NEW_NERVE(PoltaDemoNrvPowerUpDemo, PoltaDemo, PowerUpDemo);
    NEW_NERVE(PoltaDemoNrvDownDemo, PoltaDemo, DownDemo);
};  // namespace NrvPoltaDemo

PoltaDemo::PoltaDemo(Polta* pPolta) : PoltaActionBase("ポルタデモ", pPolta), mCurDemoNerve(nullptr), mCurDemoName(nullptr) {}

void PoltaDemo::init() {
    initNerve(&NrvPoltaDemo::PoltaDemoNrvOpeningDemo::sInstance);
}

void PoltaDemo::control() {}

void PoltaDemo::startOpeningDemo() {
    startTryDemo("ポルタ開始デモ", &NrvPoltaDemo::PoltaDemoNrvOpeningDemo::sInstance);
}

void PoltaDemo::startPowerUpDemo() {
    startTryDemo("ポルタパワーアップデモ", &NrvPoltaDemo::PoltaDemoNrvPowerUpDemo::sInstance);
}

void PoltaDemo::startDownDemo() {
    startTryDemo("ポルタダウンデモ", &NrvPoltaDemo::PoltaDemoNrvDownDemo::sInstance);
}

void PoltaDemo::startDemoSetting() {
    MR::overlayWithPreviousScreen(2);
    setNerve(mCurDemoNerve);
    MR::resetPlayerEffect();
    mCurDemoName = nullptr;
    mCurDemoNerve = nullptr;
}

void PoltaDemo::startTryDemo(const char* pDemoName, const Nerve* pDemoNerve) {
    appear();
    mCurDemoNerve = pDemoNerve;
    mCurDemoName = pDemoName;
    if (MR::tryStartDemoMarioPuppetable(getHost(), pDemoName)) {
        startDemoSetting();
        return;
    }
    setNerve(&NrvPoltaDemo::PoltaDemoNrvTryStartDemo::sInstance);
}

void PoltaDemo::exeTryStartDemo() {
    if (MR::tryStartDemoMarioPuppetable(getHost(), mCurDemoName)) {
        startDemoSetting();
    }
}

void PoltaDemo::exeOpeningDemo() {
    if (MR::isFirstStep(this)) {
        MR::showModel(getHost());
        PoltaFunction::killGroundRock(getHost());
        getHost()->setStartPose();
        MR::stopStageBGM(60);
        MR::startAnimCameraTargetSelf(getHost(), getHost()->mCameraInfo, "OpeningDemo", 0, 1.0f);
        MR::startAction(getHost(), "OpeningDemo");
        MR::startBckPlayer("BattleWait", (const char*)nullptr);
        PoltaFunction::onMovement(getHost());
        MR::setPlayerPos("ポルタ開始デモプレイヤー位置");
        getHost()->mFormationModel->appear();
        MR::startAction(getHost()->mFormationModel, "OpeningDemo");
    }

    if (MR::isStep(this, 300)) {
        MR::startCenterScreenBlur(70, 18.0f, 80, 5, 30);
    }

    if (MR::isStep(this, 60)) {
        MR::startStageBGM("MBGM_BOSS_09_A", false);
    }

    if (MR::isActionEnd(getHost())) {
        MR::endAnimCamera(getHost(), getHost()->mCameraInfo, "OpeningDemo", -1, true);
        MR::endDemo(getHost(), "ポルタ開始デモ");
        getHost()->mFormationModel->kill();
        kill();
    }
}

void PoltaDemo::exePowerUpDemo() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::killGroundRock(getHost());
        getHost()->setStartPose();
        MR::invalidateShadowAll(getHost()->mLeftArm);
        MR::invalidateShadowAll(getHost()->mRightArm);
        MR::stopStageBGM(60);
        PoltaFunction::onMovement(getHost());
        PoltaFunction::startArm(getHost());
        MR::startAnimCameraTargetSelf(getHost(), getHost()->mCameraInfo, "PowerUpDemo", 0, 1.0f);
        MR::startAction(getHost(), "PowerUpDemo");
        MR::startAction(PoltaFunction::getLeftArmActor(getHost()), "PowerUpDemo");
        MR::startAction(PoltaFunction::getRightArmActor(getHost()), "PowerUpDemo");
        MR::startBckPlayer("BattleWait", (const char*)nullptr);
        MR::setPlayerPos("ポルタデモプレイヤー位置");
        getHost()->mFormationModel->appear();
        getHost()->mLeftArm->mFormationModel->appear();
        getHost()->mRightArm->mFormationModel->appear();
        MR::startAction(getHost()->mFormationModel, "PowerUpDemo");
        MR::startAction(getHost()->mLeftArm->mFormationModel, "PowerUpDemoArmL");
        MR::startAction(getHost()->mRightArm->mFormationModel, "PowerUpDemoArmR");
    }

    if (MR::isStep(this, 530)) {
        MR::startCenterScreenBlur(80, 20.0f, 80, 5, 40);
    }
    if (MR::isActionEnd(getHost())) {
        MR::endDemo(getHost(), "ポルタパワーアップデモ");
        MR::endAnimCamera(getHost(), getHost()->mCameraInfo, "PowerUpDemo", -1, true);
        MR::startStageBGM("MBGM_BOSS_09_B", false);
        MR::validateShadowAll(getHost()->mLeftArm);
        MR::validateShadowAll(getHost()->mRightArm);
        getHost()->mFormationModel->kill();
        getHost()->mLeftArm->mFormationModel->kill();
        getHost()->mRightArm->mFormationModel->kill();
        kill();
    }
}

void PoltaDemo::exeDownDemo() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::killGroundRock(getHost());
        getHost()->setStartPose();
        MR::stopStageBGM(30);
        MR::startAnimCameraTargetSelf(getHost(), getHost()->mCameraInfo, "DownDemo", 0, 1.0f);
        MR::startAction(getHost(), "DownDemo");
        MR::startBckPlayer("BattleWait", (const char*)nullptr);
        PoltaFunction::onMovement(getHost());
        MR::setPlayerPos("ポルタデモプレイヤー位置");
    }
    if (MR::isActionEnd(getHost())) {
        MR::endDemo(getHost(), "ポルタダウンデモ");
        MR::endAnimCamera(getHost(), getHost()->mCameraInfo, "DownDemo", -1, true);
        getHost()->kill();
        kill();
    }
}

PoltaDemo::~PoltaDemo() {}
