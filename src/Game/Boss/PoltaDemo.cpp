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
    if (MR::tryStartDemoMarioPuppetable(mPoltaPtr, pDemoName)) {
        startDemoSetting();
        return;
    }
    setNerve(&NrvPoltaDemo::PoltaDemoNrvTryStartDemo::sInstance);
}

void PoltaDemo::exeTryStartDemo() {
    if (MR::tryStartDemoMarioPuppetable(mPoltaPtr, mCurDemoName)) {
        startDemoSetting();
    }
}

void PoltaDemo::exeOpeningDemo() {
    if (MR::isFirstStep(this)) {
        MR::showModel(mPoltaPtr);
        PoltaFunction::killGroundRock(mPoltaPtr);
        mPoltaPtr->setStartPose();
        MR::stopStageBGM(60);
        MR::startAnimCameraTargetSelf(mPoltaPtr, mPoltaPtr->mCameraInfo, "OpeningDemo", 0, 1.0f);
        MR::startAction(mPoltaPtr, "OpeningDemo");
        MR::startBckPlayer("BattleWait", (const char*)nullptr);
        PoltaFunction::onMovement(mPoltaPtr);
        MR::setPlayerPos("ポルタ開始デモプレイヤー位置");
        mPoltaPtr->mFormationModel->appear();
        MR::startAction(mPoltaPtr->mFormationModel, "OpeningDemo");
    }

    if (MR::isStep(this, 300)) {
        MR::startCenterScreenBlur(70, 18.0f, 80, 5, 30);
    }

    if (MR::isStep(this, 60)) {
        MR::startStageBGM("MBGM_BOSS_09_A", false);
    }

    if (MR::isActionEnd(mPoltaPtr)) {
        MR::endAnimCamera(mPoltaPtr, mPoltaPtr->mCameraInfo, "OpeningDemo", -1, true);
        MR::endDemo(mPoltaPtr, "ポルタ開始デモ");
        mPoltaPtr->mFormationModel->kill();
        kill();
    }
}

void PoltaDemo::exePowerUpDemo() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::killGroundRock(mPoltaPtr);
        mPoltaPtr->setStartPose();
        MR::invalidateShadowAll(mPoltaPtr->mLeftArm);
        MR::invalidateShadowAll(mPoltaPtr->mRightArm);
        MR::stopStageBGM(60);
        PoltaFunction::onMovement(mPoltaPtr);
        PoltaFunction::startArm(mPoltaPtr);
        MR::startAnimCameraTargetSelf(mPoltaPtr, mPoltaPtr->mCameraInfo, "PowerUpDemo", 0, 1.0f);
        MR::startAction(mPoltaPtr, "PowerUpDemo");
        MR::startAction(PoltaFunction::getLeftArmActor(mPoltaPtr), "PowerUpDemo");
        MR::startAction(PoltaFunction::getRightArmActor(mPoltaPtr), "PowerUpDemo");
        MR::startBckPlayer("BattleWait", (const char*)nullptr);
        MR::setPlayerPos("ポルタデモプレイヤー位置");
        mPoltaPtr->mFormationModel->appear();
        mPoltaPtr->mLeftArm->mFormationModel->appear();
        mPoltaPtr->mRightArm->mFormationModel->appear();
        MR::startAction(mPoltaPtr->mFormationModel, "PowerUpDemo");
        MR::startAction(mPoltaPtr->mLeftArm->mFormationModel, "PowerUpDemoArmL");
        MR::startAction(mPoltaPtr->mRightArm->mFormationModel, "PowerUpDemoArmR");
    }

    if (MR::isStep(this, 530)) {
        MR::startCenterScreenBlur(80, 20.0f, 80, 5, 40);
    }
    if (MR::isActionEnd(mPoltaPtr)) {
        MR::endDemo(mPoltaPtr, "ポルタパワーアップデモ");
        MR::endAnimCamera(mPoltaPtr, mPoltaPtr->mCameraInfo, "PowerUpDemo", -1, true);
        MR::startStageBGM("MBGM_BOSS_09_B", false);
        MR::validateShadowAll(mPoltaPtr->mLeftArm);
        MR::validateShadowAll(mPoltaPtr->mRightArm);
        mPoltaPtr->mFormationModel->kill();
        mPoltaPtr->mLeftArm->mFormationModel->kill();
        mPoltaPtr->mRightArm->mFormationModel->kill();
        kill();
    }
}

void PoltaDemo::exeDownDemo() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::killGroundRock(mPoltaPtr);
        mPoltaPtr->setStartPose();
        MR::stopStageBGM(30);
        MR::startAnimCameraTargetSelf(mPoltaPtr, mPoltaPtr->mCameraInfo, "DownDemo", 0, 1.0f);
        MR::startAction(mPoltaPtr, "DownDemo");
        MR::startBckPlayer("BattleWait", (const char*)nullptr);
        PoltaFunction::onMovement(mPoltaPtr);
        MR::setPlayerPos("ポルタデモプレイヤー位置");
    }
    if (MR::isActionEnd(mPoltaPtr)) {
        MR::endDemo(mPoltaPtr, "ポルタダウンデモ");
        MR::endAnimCamera(mPoltaPtr, mPoltaPtr->mCameraInfo, "DownDemo", -1, true);
        mPoltaPtr->kill();
        kill();
    }
}

PoltaDemo::~PoltaDemo() {}
