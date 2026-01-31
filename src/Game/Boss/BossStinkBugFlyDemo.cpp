#include "Game/Boss/BossStinkBugFlyDemo.hpp"
#include "Game/Boss/BossStinkBug.hpp"
#include "Game/Boss/BossStinkBugActionBase.hpp"
#include "Game/Demo/DemoPositionController.hpp"
#include "Game/LiveActor/ActorJointCtrl.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "revolution/types.h"

namespace NrvBossStinkBugFlyDemo {
    NEW_NERVE(BossStinkBugFlyDemoNrvTryStart, BossStinkBugFlyDemo, TryStart);
    NEW_NERVE(BossStinkBugFlyDemoNrvDemo, BossStinkBugFlyDemo, Demo);
}  // namespace NrvBossStinkBugFlyDemo

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

    if (MR::isGreaterStep(this, 113)) {
        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_ADD", 500, 126, -1);
    }

    if (MR::isGreaterStep(this, 260)) {
        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_BASE", 500, 126, -1);
        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_ALARM", 500, 126, -1);
    }

    if (MR::isGreaterStep(this, 48) && MR::isLessStep(this, 90)) {
        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_SHAKE_BODY", -1, -1, -1);
    }

    if (MR::isStep(this, 230)) {
        MR::startSound(getHost(), "SE_BM_BOSS_BUG_CLOSE_FEET", -1, -1);
    }

    if (MR::isGreaterStep(this, 110) && MR::isLessStep(this, 263)) {
        MR::startLevelSound(getHost(), "SE_BM_LV_BOSS_BUG_FLY_START", -1, -1, -1);
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
