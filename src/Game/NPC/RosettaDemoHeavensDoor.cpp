#include "Game/NPC/RosettaDemoHeavensDoor.hpp"
#include "Game/Demo/DemoFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/NPC/Rosetta.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util.hpp"

namespace {
    static const s32 sRosettaSwingVoiceFrame = 70;
    static const s32 sRosettaSwingFrame = 80;
    static const s32 sRosettaHideFrame = 10;
};  // namespace

namespace NrvRosettaDemoHeavensDoor1 {
    NEW_NERVE(RosettaDemoHeavensDoor1NrvWait, RosettaDemoHeavensDoor1, Wait);
    NEW_NERVE(RosettaDemoHeavensDoor1NrvFade, RosettaDemoHeavensDoor1, Fade);
    NEW_NERVE(RosettaDemoHeavensDoor1NrvDemo, RosettaDemoHeavensDoor1, Demo);
};  // namespace NrvRosettaDemoHeavensDoor1

RosettaDemoHeavensDoor1::RosettaDemoHeavensDoor1(Rosetta* pHost, const JMapInfoIter& rIter) : NerveExecutor("ロゼッタデモ実行者"), mHost(pHost) {
    DemoFunction::tryCreateDemoTalkAnimCtrlForActor(mHost, "DemoGetPower", "スピンゲット[デモ1]");
    initNerve(&NrvRosettaDemoHeavensDoor1::RosettaDemoHeavensDoor1NrvWait::sInstance);

    mLightDomeModel = MR::createPartsModelNpc(mHost, "ライトドーム", "LightDome", nullptr);
    mLightDomeModel->makeActorDead();
    mLightDomeModel->initFixedPosition(TVec3f(0.0f, -13.0f, -30.0f), TVec3f(0.0f, 0.0f, 0.0f), "Center");
    MR::startBrk(mLightDomeModel, "LightDome");
    MR::startBck(mLightDomeModel, "Appear", nullptr);

    if (MR::isDemoCast(mHost, nullptr)) {
        MR::tryRegisterDemoCast(mLightDomeModel, rIter);
    }

    DemoFunction::tryCreateDemoTalkAnimCtrlForActor(mLightDomeModel, "DemoGetPower", "スピンゲット[デモ1]");
    DemoFunction::registerDemoTalkMessageCtrl(mHost, mHost->mMsgCtrl);

    mLightHaloModel = MR::createPartsModelNpc(mHost, "ライト後光", "DomeHalo", nullptr);
    mLightHaloModel->initFixedPosition(TVec3f(0.0f, 25.14f, -6.16f), TVec3f(0.0f, 0.0f, 0.0f), "Center");
    mLightHaloModel->makeActorDead();

    if (MR::isDemoCast(mHost, nullptr)) {
        MR::tryRegisterDemoCast(mLightHaloModel, rIter);
    }

    MR::needStageSwitchWriteA(mHost, rIter);
    MR::needStageSwitchWriteB(mHost, rIter);
    // MR::registerDemoActionFunctor(mHost, MR::Functor(this, &RosettaDemoHeavensDoor1::preDemo, "高楼出現[デモ]");
    // MR::registerDemoActionFunctor(mHost, MR::Functor(this, &RosettaDemoHeavensDoor1::pstDemo, "高楼出現[デモ後]");
    // MR::registerDemoActionFunctor(mHost, MR::Functor(this, &RosettaDemoHeavensDoor1::fadeOut, "高楼出現[フェードアウト]");
    // MR::registerDemoActionFunctor(mHost, MR::Functor(this, &RosettaDemoHeavensDoor1::fadeIn, "高楼出現[フェードイン]");
    // MR::registerDemoActionFunctor(mHost, MR::Functor(this, &RosettaDemoHeavensDoor1::changeNerve, "スピンゲット[デモ1]");
    MR::invalidateShadowAll(mHost);
    MR::invalidateHitSensors(mHost);
    MR::setClippingTypeSphere(mHost, 1500.0f);
    mHost->startDemo(this);
    mHost->makeActorDead();
}

void RosettaDemoHeavensDoor1::makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter) {
    pCollector->addArchive("LightDome");
    pCollector->addArchive("DomeHalo");
}

void RosettaDemoHeavensDoor1::preDemo() {
    MR::hidePlayer();
}

void RosettaDemoHeavensDoor1::pstDemo() {
    MR::startSound(mHost, "SE_OJ_ROSETTA_HALO_APPEAR");
}

void RosettaDemoHeavensDoor1::fadeOut() {
    MR::timeKeepDemoFadeOut();
}

void RosettaDemoHeavensDoor1::fadeIn() {
    mLightHaloModel->mCalcOwnMtx = true;

    MR::emitEffect(mLightDomeModel, "Light");
    MR::showPlayer();
    MR::timeKeepDemoFadeIn();
    MR::onSwitchB(mHost);
}

void RosettaDemoHeavensDoor1::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "DemoGetPowerStartWait");
    }

    if (MR::isNearPlayer(mHost->mMsgCtrl, 500.0f) && !MR::isTimeKeepDemoActive()) {
        MR::offPlayerControl();
        MR::timeKeepDemoFadeOut();
        MR::startBrk(mLightHaloModel, "Disappear");
        setNerve(&NrvRosettaDemoHeavensDoor1::RosettaDemoHeavensDoor1NrvFade::sInstance);
    }
}

void RosettaDemoHeavensDoor1::exeFade() {
    if (MR::isStep(this, 90)) {
        MR::onPlayerControl(true);
        MR::startTimeKeepDemoMarioPuppetable(mHost, "チコガイドデモ", "スピンゲット[デモ1]");
        mLightHaloModel->kill();
    }
}

void RosettaDemoHeavensDoor1::exeDemo() {
    if (MR::isDemoPartFirstStep("スピンゲット[デモ6]")) {
        MR::deleteEffect(mLightDomeModel, "Light");
    }

    if (MR::isDemoPartActive("スピンゲット[デモ3]")) {
        s32 step = MR::getDemoPartStep("スピンゲット[デモ3]");

        if (step == ::sRosettaSwingVoiceFrame) {
            MR::startSound(mHost, "SE_SV_ROSETTA_SWING");
        }

        if (step == ::sRosettaSwingFrame) {
            MR::startSound(mHost, "SE_SM_ROSETTA_OP_SWING");
        }
    }

    if (MR::isDemoPartActive("スピンゲット[会話1]") || MR::isDemoPartActive("スピンゲット[会話2]") || MR::isDemoPartActive("スピンゲット[会話3]") ||
        MR::isDemoPartActive("スピンゲット[会話4]") || MR::isDemoPartActive("スピンゲット[デモ2]") || MR::isDemoPartActive("スピンゲット[デモ3]") ||
        MR::isDemoPartActive("スピンゲット[デモ4]") || MR::isDemoPartActive("スピンゲット[デモ5]")) {
        MR::startLevelSound(mHost, "SE_SM_LV_TICO_OP_WAIT");
    }

    if (MR::isDemoPartActive("スピンゲット[デモ6]")) {
        if (MR::getDemoPartStep("スピンゲット[デモ6]") < ::sRosettaHideFrame) {
            MR::startLevelSound(mHost, "SE_SM_LV_TICO_OP_WAIT");
        } else {
            MR::startLevelSound(mHost, "SE_SM_LV_ROSETTA_OP_HIDE");
        }
    }
}

namespace NrvRosettaDemoHeavensDoor2 {
    NEW_NERVE(RosettaDemoHeavensDoor2NrvWait, RosettaDemoHeavensDoor2, Wait);
    NEW_NERVE(RosettaDemoHeavensDoor2NrvDemo, RosettaDemoHeavensDoor2, Demo);
};  // namespace NrvRosettaDemoHeavensDoor2

RosettaDemoHeavensDoor2::RosettaDemoHeavensDoor2(Rosetta* pHost, const JMapInfoIter& rIter)
    : NerveExecutor("ロゼッタデモ実行者"), mDemoStarter(pHost), mHost(pHost) {
    DemoFunction::tryCreateDemoTalkAnimCtrlForScene(mHost, rIter, "DemoRedStar", "郷愁[開始]", 0, 0);
    DemoFunction::registerDemoTalkMessageCtrl(mHost, mHost->mMsgCtrl);
    // MR::registerDemoActionFunctor(mHost, MR::Functor(this, &RosettaDemoHeavensDoor2::changeNerve<
    // NrvRosettaDemoHeavensDoor2::RosettaDemoHeavensDoor2 >), "郷愁[開始]");
    MR::needStageSwitchWriteA(mHost, rIter);

    if (MR::isOnGameEventFlagRosettaTalkAboutTicoInTower()) {
        mHost->kill();
    } else {
        MR::onSwitchA(mHost);
    }

    MR::invalidateShadowAll(mHost);
    MR::invalidateHitSensors(mHost);
    mHost->startDemo(this);
    initNerve(&NrvRosettaDemoHeavensDoor2::RosettaDemoHeavensDoor2NrvWait::sInstance);
}

void RosettaDemoHeavensDoor2::makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter) {
}

void RosettaDemoHeavensDoor2::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "WaitB");
    }

    if (MR::isNearPlayer(mHost, 400.0f)) {
        mDemoStarter.start();
    }

    if (mDemoStarter.update()) {
        MR::tryStartTimeKeepDemoMarioPuppetable(mHost, "赤いスター", "郷愁[開始]");
        MR::onGameEventFlagRosettaTalkAboutTicoInTower();
    }
}

void RosettaDemoHeavensDoor2::exeDemo() {
}
