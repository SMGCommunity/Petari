#include "Game/NPC/RosettaDemoAstroDome.hpp"
#include "Game/Demo/DemoFunction.hpp"
#include "Game/NPC/Rosetta.hpp"
#include "Game/Screen/IconAButton.hpp"
#include <cstdio>

// TODO: this file is not linkable yet due to a mismatch in .data related to TalkMessageFunc

namespace NrvRosettaDemoAstroDomeExplain {
    NEW_NERVE(RosettaDemoAstroDomeExplainNrvExplainDemo, RosettaDemoAstroDomeExplain, Demo);
}  // namespace NrvRosettaDemoAstroDomeExplain

namespace NrvRosettaDemoAstroDomeFinalBattle {
    NEW_NERVE(RosettaDemoAstroDomeFinalBattleNrvFinalBattleDemo, RosettaDemoAstroDomeFinalBattle, Demo);
}  // namespace NrvRosettaDemoAstroDomeFinalBattle

namespace NrvRosettaDemoAstroDomeTalk {
    NEW_NERVE(RosettaDemoAstroDomeTalkNrvTalkDemo, RosettaDemoAstroDomeTalk, Demo);
}  // namespace NrvRosettaDemoAstroDomeTalk

template < typename T >
static void EntryDemo(T* caller, const char* pDemoName, const char* pRootName, const JMapInfoIter& rIter) {
    if (MR::isDemoExist(pDemoName)) {
        TVec3f offset(MR::getMessageBalloonFollowOffset(caller->mRosetta->mMsgCtrl));
        TalkMessageCtrl* ctrl = MR::createTalkCtrlDirectOnRootNodeAutomatic(caller->mRosetta, rIter, pRootName, offset, nullptr);
        MR::registerEventFunc(ctrl, TalkMessageFunc(caller->mRosetta, &Rosetta::eventFunc));
        DemoFunction::registerDemoTalkMessageCtrlDirect(caller->mRosetta, ctrl, pDemoName);
        MR::registerDemoActionFunctorDirect(caller->mRosetta, MR::Functor_Inline(caller, &T::startDemo), pDemoName, "開始");
    }
}

RosettaMonologue::RosettaMonologue() : SimpleLayout("ロゼッタの語り", "PrologueStarSteward", 2, -1), mTextFormer(this, "Text00") {
    MR::createAndAddPaneCtrl(this, "TalkBalloon", 1);
    MR::createAndAddPaneCtrl(this, "CrossFade1", 1);
    MR::createAndAddPaneCtrl(this, "CrossFade2", 1);
    MR::createAndAddPaneCtrl(this, "CrossFade3", 1);

    mIcon = new IconAButton(true, false);
    mIcon->initWithoutIter();
    mIcon->kill();
    mMsgID = 0;
}

void RosettaMonologue::appear() {
    LayoutActor::appear();
    mMsgID = 0;

    MR::startAnim(this, "FadeIn", 0);
    MR::startPaneAnim(this, "TalkBalloon", "WinFadeIn", 0);

    char buff[256];
    snprintf(buff, 256, "RosettaMonologue%03d", mMsgID);
    mTextFormer.formMessage(MR::getGameMessageDirect(buff), 1);
}

void RosettaMonologue::calcAnim() {
    LayoutActor::calcAnim();
    mIcon->calcAnim();
}

void RosettaMonologue::movement() {
    LayoutActor::movement();
    mIcon->movement();
}

void RosettaMonologue::control() {
    if (mMsgID >= 4) {
        if (MR::isAnimStopped(this, 0)) {
            MR::forceCloseWipeFade();
            kill();
        }
        return;
    }

    mTextFormer.updateTalking();

    if (!mTextFormer.isTextAppearedAll()) {
        return;
    }

    if (!mIcon->isOpen()) {
        mIcon->openWithoutMessage();
    }

    MR::setLayoutPosAtPaneTrans(mIcon, this, "AButtonPosition");

    if (!MR::testCorePadTriggerA(WPAD_CHAN0)) {
        return;
    }

    mIcon->term();

    if (mTextFormer.nextPage()) {
        MR::startSystemSE("SE_SY_TALK_FOCUS_ITEM", -1, -1);
        return;
    }

    mMsgID++;
    if (mMsgID == 4) {
        MR::startSystemSE("SE_SY_TALK_OK", -1, -1);
        MR::startCSSound("CS_CLICK_CLOSE", nullptr, WPAD_CHAN0);
    } else {
        MR::startSystemSE("SE_SY_TALK_FOCUS_ITEM", -1, -1);
        char buff[256];
        snprintf(buff, 256, "RosettaMonologue%03d", mMsgID);
        mTextFormer.formMessage(MR::getGameMessageDirect(buff), 1);
    }

    switch (mMsgID) {
    case 1:
        MR::startPaneAnim(this, "CrossFade1", "CrossFade1", 0);
        break;
    case 2:
        MR::startPaneAnim(this, "CrossFade2", "CrossFade2", 0);
        break;
    case 3:
        MR::startPaneAnim(this, "CrossFade3", "CrossFade3", 0);
        break;
    case 4:
        MR::startAnim(this, "FadeOut", 0);
        MR::startPaneAnim(this, "TalkBalloon", "WinFadeOut", 0);
        break;
    }
}

RosettaDemoAstroDomeExplain::RosettaDemoAstroDomeExplain(Rosetta* pRosetta, const JMapInfoIter& rIter)
    : NerveExecutor("ロゼッタ状況説明デモ実行者"), mRosetta(pRosetta) {
    const char* sDemoExplain = "ロゼッタ状況説明デモ";
    DemoFunction::tryCreateDemoTalkAnimCtrlForSceneDirect(mRosetta, sDemoExplain, rIter, "DemoWithButler", nullptr, 0, 0);
    DemoFunction::registerDemoTalkMessageCtrlDirect(mRosetta, mRosetta->mMsgCtrl, sDemoExplain);
    MR::registerDemoActionFunctorDirect(mRosetta, MR::Functor_Inline(this, &RosettaDemoAstroDomeExplain::startDemo), sDemoExplain, "状況説明[開始]");
    mMonologue = new RosettaMonologue();
    initNerve(&NrvRosettaDemoAstroDomeExplain::RosettaDemoAstroDomeExplainNrvExplainDemo::sInstance);
}

void RosettaDemoAstroDomeExplain::makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter) {
    pCollector->addArchive("PrologueStarSteward");
}

void RosettaDemoAstroDomeExplain::startDemo() {
    mRosetta->startDemo(this);
}

void RosettaDemoAstroDomeExplain::exeDemo() {
    if (MR::isDemoPartActive("状況説明[絵本表示]")) {
        if (MR::isDemoPartFirstStep("状況説明[絵本表示]")) {
            MR::tryFrameToScreenCinemaFrame();
            mMonologue->appear();
            MR::pauseTimeKeepDemo(mRosetta);
        }

        mMonologue->movement();
        mMonologue->calcAnim();

        if (MR::isDead(mMonologue)) {
            MR::resumeTimeKeepDemo(mRosetta);
        }
    }

    if (MR::isDemoLastStep()) {
        mRosetta->endDemo();
    }
}

RosettaDemoAstroDomeFinalBattle::RosettaDemoAstroDomeFinalBattle(Rosetta* pRosetta, const JMapInfoIter& rIter)
    : NerveExecutor("ロゼッタ最終決戦デモ実行者"), mRosetta(pRosetta) {
    EntryDemo(this, "ロゼッタ最終決戦デモ", "AstroGalaxy_Rosetta300", rIter);
    EntryDemo(this, "ロゼッタノーマルエンディング後デモ", "AstroGalaxy_Rosetta400", rIter);

    MR::needStageSwitchWriteA(mRosetta, rIter);
    initNerve(&NrvRosettaDemoAstroDomeFinalBattle::RosettaDemoAstroDomeFinalBattleNrvFinalBattleDemo::sInstance);
}

void RosettaDemoAstroDomeFinalBattle::startDemo() {
    mRosetta->startDemo(this);
}

void RosettaDemoAstroDomeFinalBattle::exeDemo() {
    if (MR::isDemoLastStep()) {
        mRosetta->endDemoWithInterpole();
    }
}

RosettaDemoAstroDomeTalk::RosettaDemoAstroDomeTalk(Rosetta* pRosetta, const JMapInfoIter& rIter)
    : NerveExecutor("ロゼッタ会話デモ実行者"), mRosetta(pRosetta) {
    EntryDemo(this, "ロゼッタキノピオ探検隊デモ", "AstroGalaxy_Rosetta080", rIter);
    EntryDemo(this, "ロゼッタトーチの炎説明デモ", "AstroGalaxy_Rosetta020", rIter);
    EntryDemo(this, "ロゼッタコメット説明デモ", "AstroGalaxy_Rosetta030", rIter);
    EntryDemo(this, "ロゼッタ銀河の中心説明デモ", "AstroGalaxy_Rosetta040", rIter);
    EntryDemo(this, "ロゼッタ天文台機能回復デモ", "AstroGalaxy_Rosetta050", rIter);
    EntryDemo(this, "ロゼッタカウントダウン開始デモ", "AstroGalaxy_Rosetta060", rIter);
    EntryDemo(this, "ロゼッタトーチの炎進捗デモ", "AstroGalaxy_Rosetta084", rIter);
    EntryDemo(this, "ロゼッタルイージデモ", "AstroGalaxy_Rosetta054", rIter);

    initNerve(&NrvRosettaDemoAstroDomeTalk::RosettaDemoAstroDomeTalkNrvTalkDemo::sInstance);
}

void RosettaDemoAstroDomeTalk::startDemo() {
    mRosetta->startDemo(this);
}

void RosettaDemoAstroDomeTalk::exeDemo() {
    if (MR::isDemoLastStep()) {
        mRosetta->endDemo();
    }
}
