#include "Game/Demo/DemoFunction.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NPC/Rosetta.hpp"
#include "Game/NPC/RosettaDemoEpilogue.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace NrvRosettaDemoEpilogue {
    NEW_NERVE(RosettaDemoEpilogueNrvDemo, RosettaDemoEpilogue, Demo);
};

RosettaDemoEpilogue::RosettaDemoEpilogue(Rosetta* pRosetta, const JMapInfoIter& rIter) :
    NerveExecutor("ロゼッタデモ実行者"),
    mRosetta(pRosetta),
    mIsFadeOut(false)
{
    DemoFunction::tryCreateDemoTalkAnimCtrlForScene(pRosetta, rIter, "DemoEpilogueB", nullptr, 0, 0);
    DemoFunction::registerDemoTalkMessageCtrl(mRosetta, mRosetta->mMsgCtrl);
    MR::registerDemoActionFunctor(
        mRosetta,
        MR::Functor_Inline(this, &RosettaDemoEpilogue::startDemo),
        "エピローグ[開始]");
    mRosetta->mLodCtrl->invalidate();
    initNerve(&NrvRosettaDemoEpilogue::RosettaDemoEpilogueNrvDemo::sInstance);
}

void RosettaDemoEpilogue::startDemo() {
    mIsFadeOut = false;

    mRosetta->startDemo(this);
}

void RosettaDemoEpilogue::exeDemo() {
    if (MR::isDemoPartActive("エピローグ[フェードアウト]")) {
        mIsFadeOut = true;
    }

    if (!mIsFadeOut) {
        MR::startAtmosphereLevelSE("SE_DM_LV_EPILOGUE_BABY_CRY", -1, -1);
    }

    if (MR::isDemoLastStep()) {
        mRosetta->endDemo();
    }
}
