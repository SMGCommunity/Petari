#include "Game/Boss/BossStinkBugFlyDemo.hpp"
#include "Game/Boss/BossStinkBug.hpp"
#include "Game/Demo/DemoPositionController.hpp"
#include "Game/LiveActor/ActorJointCtrl.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const s32 sFlySoundStartStep1 = 113;
    static const s32 sFlySoundStartStep2 = 260;
    static const s32 sShakeSoundStartStep = 48;
    static const s32 sShakeSoundEndStep = 90;
    static const s32 sCloseFeetSoundStartStep = 230;
    static const s32 sFlyStartSoundStartStep = 110;
    static const s32 sFlyStartSoundEndStep = 263;
};  // namespace

namespace NrvBossStinkBugFlyDemo {
    NEW_NERVE(BossStinkBugFlyDemoNrvTryStart, BossStinkBugFlyDemo, TryStart);
    NEW_NERVE(BossStinkBugFlyDemoNrvDemo, BossStinkBugFlyDemo, Demo);
};  // namespace NrvBossStinkBugFlyDemo

BossStinkBugFlyDemo::BossStinkBugFlyDemo(BossStinkBug* pStinkBug, const JMapInfoIter& rIter)
    : BossStinkBugActionBase("終了デモ", pStinkBug), mDemoPositionController(nullptr) {
    initNerve(&NrvBossStinkBugFlyDemo::BossStinkBugFlyDemoNrvDemo::sInstance);
    mDemoPositionController = new DemoPositionController("BossStinkBugDemo", rIter);
    mDemoPositionController->initAnimCamera("FlyDemo");
}

void BossStinkBugFlyDemo::appear() {
    ActorStateBase::appear();
    setNerve(&NrvBossStinkBugFlyDemo::BossStinkBugFlyDemoNrvTryStart::sInstance);
    MR::requestStartDemoMarioPuppetable(this, getHost(), "ボスカメムシ飛行デモ", &NrvBossStinkBugFlyDemo::BossStinkBugFlyDemoNrvDemo::sInstance,
                                        nullptr);
}

void BossStinkBugFlyDemo::exeDemo() {
    if (MR::isFirstStep(this)) {
        MR::overlayWithPreviousScreen(2);
        MR::stopStageBGM(30);
        getHost()->offBindLeg();
        getHost()->_98->endIKCtrlAll();
        MR::startBckPlayer("BattleWait", "BattleWaitNoInter");
        mDemoPositionController->startDemo("FlyDemo");
        getHost()->reuestMovementOnParts();
        MR::startBck(getHost(), "FlyDemo", nullptr);
        MR::startBck(getHost()->getBombLauncher(), "FlyDemo", nullptr);
        MR::tryStartAllAnim(getHost()->getWingModel(), "FlyDemo");
    }

    if (MR::isGreaterStep(this, ::sFlySoundStartStep1)) {
        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_ADD", 500, 126);
    }

    if (MR::isGreaterStep(this, ::sFlySoundStartStep2)) {
        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_BASE", 500, 126);
        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_ALARM", 500, 126);
    }

    if (MR::isGreaterStep(this, ::sShakeSoundStartStep) && MR::isLessStep(this, ::sShakeSoundEndStep)) {
        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_SHAKE_BODY");
    }

    if (MR::isStep(this, ::sCloseFeetSoundStartStep)) {
        MR::startSound(getHost(), "SE_BM_BOSS_BUG_CLOSE_FEET");
    }

    if (MR::isGreaterStep(this, ::sFlyStartSoundStartStep) && MR::isLessStep(this, ::sFlyStartSoundEndStep)) {
        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_START");
    }

    mDemoPositionController->movement();
    MR::setPlayerBaseMtx(MR::getJointMtx(mDemoPositionController, "MarioPosition"));
    getHost()->setPose(MR::getJointMtx(mDemoPositionController, "Boss"));

    if (MR::isBckStopped(getHost())) {
        mDemoPositionController->endDemo("FlyDemo");
        MR::endDemo(getHost(), "ボスカメムシ飛行デモ");
        kill();
    }
}

void BossStinkBugFlyDemo::exeTryStart() {
}
