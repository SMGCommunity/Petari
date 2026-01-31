#include "Game/Boss/BossStinkBugOpeningDemo.hpp"
#include "Game/Boss/BossStinkBug.hpp"
#include "Game/Boss/BossStinkBugActionBase.hpp"
#include "Game/Demo/DemoPositionController.hpp"
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

namespace NrvBossStinkBugOpeningDemo {
    NEW_NERVE(BossStinkBugOpeningDemoNrvTryStart, BossStinkBugOpeningDemo, TryStart);
    NEW_NERVE(BossStinkBugOpeningDemoNrvDemo, BossStinkBugOpeningDemo, Demo);
}  // namespace NrvBossStinkBugOpeningDemo

BossStinkBugOpeningDemo::BossStinkBugOpeningDemo(BossStinkBug* pStinkBug, const JMapInfoIter& rIter)
    : BossStinkBugActionBase("オープニングデモ", pStinkBug), mDemoPositionController(nullptr) {
    initNerve(&NrvBossStinkBugOpeningDemo::BossStinkBugOpeningDemoNrvDemo::sInstance);
    mDemoPositionController = new DemoPositionController("BossStinkBugDemo", rIter);
    mDemoPositionController->initAnimCamera("OpeningDemo");
}

void BossStinkBugOpeningDemo::appear() {
    mIsDead = false;
    setNerve(&NrvBossStinkBugOpeningDemo::BossStinkBugOpeningDemoNrvTryStart::sInstance);
    MR::requestStartDemoMarioPuppetable(this, getHost(), "ボスカメムシオープニングデモ",
                                        &NrvBossStinkBugOpeningDemo::BossStinkBugOpeningDemoNrvDemo::sInstance, nullptr);
}

void BossStinkBugOpeningDemo::exeDemo() {
    if (MR::isFirstStep(this)) {
        MR::overlayWithPreviousScreen(2);
        MR::stopStageBGM(60);
        MR::startBckPlayer("BattleWait", (const char*)nullptr);
        mDemoPositionController->startDemo("OpeningDemo");
        getHost()->reuestMovementOnParts();
        MR::startBck(getHost(), "OpeningDemo", nullptr);
        MR::tryStartAllAnim(getHost()->getWingModel(), "OpeningDemo");
    }
    mDemoPositionController->movement();
    MR::setPlayerBaseMtx(MR::getJointMtx(mDemoPositionController, "MarioPosition"));
    getHost()->setPose(MR::getJointMtx(mDemoPositionController, "Boss"));

    if (MR::isBckStopped(getHost())) {
        mDemoPositionController->endDemo("OpeningDemo");
        MR::endDemo(getHost(), "ボスカメムシオープニングデモ");
        kill();
    }
}

void BossStinkBugOpeningDemo::exeTryStart() {
}
