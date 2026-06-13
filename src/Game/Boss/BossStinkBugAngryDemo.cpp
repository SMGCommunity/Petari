#include "Game/Boss/BossStinkBugAngryDemo.hpp"
#include "Game/Boss/BossStinkBug.hpp"
#include "Game/Demo/DemoPositionController.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const s32 sFlySoundStartStep1 = 155;
    static const s32 sFlySoundStartStep2 = 200;
};  // namespace

namespace NrvBossStinkBugAngryDemo {
    NEW_NERVE(BossStinkBugAngryDemoNrvTryStart, BossStinkBugAngryDemo, TryStart);
    NEW_NERVE(BossStinkBugAngryDemoNrvDemo, BossStinkBugAngryDemo, Demo);
};  // namespace NrvBossStinkBugAngryDemo

BossStinkBugAngryDemo::BossStinkBugAngryDemo(BossStinkBug* pHost, const JMapInfoIter& rIter)
    : BossStinkBugActionBase("終了デモ", pHost), mDemoPositionController(nullptr) {
    initNerve(&NrvBossStinkBugAngryDemo::BossStinkBugAngryDemoNrvDemo::sInstance);
    mDemoPositionController = new DemoPositionController("BossStinkBugDemo", rIter);
    mDemoPositionController->initAnimCamera("AngryDemo");
}

void BossStinkBugAngryDemo::appear() {
    ActorStateBase::appear();
    setNerve(&NrvBossStinkBugAngryDemo::BossStinkBugAngryDemoNrvTryStart::sInstance);
    MR::requestStartDemoMarioPuppetable(this, getHost(), "ボスカメムシ怒りデモ", &NrvBossStinkBugAngryDemo::BossStinkBugAngryDemoNrvDemo::sInstance,
                                        nullptr);
}

void BossStinkBugAngryDemo::exeDemo() {
    if (MR::isFirstStep(this)) {
        MR::overlayWithPreviousScreen(2);
        MR::startBckPlayer("BattleWait", "BattleWaitNoInter");
        mDemoPositionController->startDemo("AngryDemo");
        getHost()->reuestMovementOnParts();
        MR::startBck(getHost(), "AngryDemo", nullptr);
        MR::startBrk(getHost(), "Angry");
        MR::tryStartAllAnim(getHost()->getWingModel(), "AngryDemo");
    }

    if (MR::isGreaterStep(this, ::sFlySoundStartStep1) && MR::isLessStep(this, ::sFlySoundStartStep2 + 1)) {
        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_ADD", 550, 120);
        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_ALARM", 250, 20);
    }

    if (MR::isGreaterStep(this, ::sFlySoundStartStep2)) {
        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_ADD", 550, 126);
        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_ALARM", 550, 126);
        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_BASE2", 550, 126);
    }

    mDemoPositionController->movement();
    MR::setPlayerBaseMtx(MR::getJointMtx(mDemoPositionController, "MarioPosition"));
    getHost()->setPose(MR::getJointMtx(mDemoPositionController, "Boss"));

    if (MR::isBckStopped(getHost())) {
        mDemoPositionController->endDemo("AngryDemo");
        MR::endDemo(getHost(), "ボスカメムシ怒りデモ");
        kill();
    }
}

void BossStinkBugAngryDemo::exeTryStart() {
}
