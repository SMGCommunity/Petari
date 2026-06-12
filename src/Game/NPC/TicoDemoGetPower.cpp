#include "Game/NPC/TicoDemoGetPower.hpp"
#include "Game/Demo/DemoFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NPC/Tico.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace NrvTicoDemoGetPower {
    NEW_NERVE(TicoDemoGetPowerNrvWait, TicoDemoGetPower, Wait);
    NEW_NERVE(TicoDemoGetPowerNrvDemo, TicoDemoGetPower, Demo);
};  // namespace NrvTicoDemoGetPower

TicoDemoGetPower::TicoDemoGetPower(Tico* pTico, const JMapInfoIter& rIter) : NerveExecutor("チコゲットパワーデモ実行者"), mTico(pTico) {
    initNerve(&NrvTicoDemoGetPower::TicoDemoGetPowerNrvWait::sInstance);
    DemoFunction::tryCreateDemoTalkAnimCtrlForScene(mTico, rIter, "DemoGetPower", "スピンゲット[デモ1]", 0, 0);
    MR::registerDemoActionFunctor(mTico, MR::Functor_Inline(this, &TicoDemoGetPower::startDemo), "スピンゲット[デモ1]");
    mTico->makeActorDead();
}

void TicoDemoGetPower::startDemo() {
    mTico->makeActorAppeared();
    setNerve(&NrvTicoDemoGetPower::TicoDemoGetPowerNrvDemo::sInstance);
    exeDemo();
}

void TicoDemoGetPower::exeWait() {
}

void TicoDemoGetPower::exeDemo() {
    if (MR::isDemoPartFirstStep("スピンゲット[デモ5]")) {
        MR::explainEnableToSpin(mTico);
    }

    if (MR::isDemoPartActive("スピンゲット[デモ3]")) {
        s32 partStep = MR::getDemoPartStep("スピンゲット[デモ3]");

        if (partStep == 110) {
            MR::startSound(mTico, "SE_SM_TICO_OP_APPEAR");
        }

        if (partStep == 140) {
            MR::startSound(mTico, "SE_SM_TICO_OP_APPEAR_END");
        }

        if (partStep == 180) {
            MR::startSound(mTico, "SE_SV_TICO_OP_APPEAR");
        }
    }

    if (MR::isDemoPartActive("スピンゲット[デモ4]")) {
        s32 partStep = MR::getDemoPartStep("スピンゲット[デモ4]");

        if (partStep == 10) {
            MR::startSubBGM("BGM_STAR_POW_GET", false);
        }

        if (partStep <= 65) {
            MR::startLevelSound(mTico, "SE_SM_LV_TICO_WAIT");
        }

        if (partStep == 72) {
            MR::startSound(mTico, "SE_SM_TICO_FLASH_SPIN");
        }

        if (partStep == 170) {
            MR::startSound(mTico, "SE_SM_TICO_FLASH_LIGHT");
        }

        if (partStep == 255) {
            MR::startSound(mTico, "SE_SM_TICO_FUSION");
        }

        if (partStep == 420) {
            MR::startSoundPlayer("SE_PV_POWER_STAR_GET", -1);
        }
    }
}
