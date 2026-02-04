#include "Game/Boss/BossStinkBugFinishDemo.hpp"
#include "Game/Boss/BossStinkBug.hpp"
#include "Game/Boss/BossStinkBugActionBase.hpp"
#include "Game/Demo/DemoPositionController.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "revolution/types.h"

namespace NrvBossStinkBugFinishDemo {
    NEW_NERVE(BossStinkBugFinishDemoNrvTryStart, BossStinkBugFinishDemo, TryStart);
    NEW_NERVE(BossStinkBugFinishDemoNrvDemo, BossStinkBugFinishDemo, Demo);
    NEW_NERVE(BossStinkBugFinishDemoNrvAppearPowerStar, BossStinkBugFinishDemo, AppearPowerStar);
}  // namespace NrvBossStinkBugFinishDemo

BossStinkBugFinishDemo::BossStinkBugFinishDemo(BossStinkBug* pStinkBug, const JMapInfoIter& rIter)
    : BossStinkBugActionBase("終了デモ", pStinkBug), mDemoPositionController(nullptr) {
    initNerve(&NrvBossStinkBugFinishDemo::BossStinkBugFinishDemoNrvDemo::sInstance);
    mDemoPositionController = new DemoPositionController("BossStinkBugDemo", rIter);
    mDemoPositionController->initAnimCamera("FinishDemo");
}

void BossStinkBugFinishDemo::appear() {
    ActorStateBase::appear();
    setNerve(&NrvBossStinkBugFinishDemo::BossStinkBugFinishDemoNrvTryStart::sInstance);
    MR::requestStartDemoMarioPuppetable(this, getHost(), "ボスカメムシ終了デモ", &NrvBossStinkBugFinishDemo::BossStinkBugFinishDemoNrvDemo::sInstance,
                                        nullptr);
}

void BossStinkBugFinishDemo::exeDemo() {
    if (MR::isFirstStep(this)) {
        MR::overlayWithPreviousScreen(2);
        MR::stopStageBGM(30);
        MR::startBckPlayer("BattleWait", "BattleWaitNoInter");
        getHost()->reuestMovementOnParts();
        mDemoPositionController->startDemo("FinishDemo");
        MR::startBck(getHost(), "FinishDemo", nullptr);
        MR::tryStartAllAnim(getHost()->getWingModel(), "FinishDemo");
    }

    mDemoPositionController->movement();
    MR::setPlayerBaseMtx(MR::getJointMtx(mDemoPositionController, "MarioPosition"));
    getHost()->setPose(MR::getJointMtx(mDemoPositionController, "Boss"));

    if (MR::isBckStopped(getHost())) {
        setNerve(&NrvBossStinkBugFinishDemo::BossStinkBugFinishDemoNrvAppearPowerStar::sInstance);
    }
}

void BossStinkBugFinishDemo::exeAppearPowerStar() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(getHost(), "Death");
        MR::startSound(getHost(), "SE_BM_BOSS_BUG_EXPLODE", -1, -1);
        MR::hideModel(getHost());
        MR::tryRumblePadVeryStrong(getHost(), 0);
    }

    if (MR::isStep(this, 30)) {
        mDemoPositionController->endDemo("FinishDemo");
        MR::requestAppearPowerStar(getHost(), getHost()->mPosition);
        MR::endDemo(getHost(), "ボスカメムシ終了デモ");
        MR::overlayWithPreviousScreen(2);
        MR::startAfterBossBGM();
    }

    if (MR::isGreaterStep(this, 60) && MR::isEndPowerStarAppearDemo(getHost())) {
        kill();
        getHost()->kill();
    }
}

void BossStinkBugFinishDemo::exeTryStart() {
}
