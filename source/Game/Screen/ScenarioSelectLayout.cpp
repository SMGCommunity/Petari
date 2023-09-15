#include "Game/Screen/ScenarioSelectLayout.h"

namespace {
    const char* const cStarPaneName[] = {
        "Star1",
        "Star2",
        "Star3",
        "Star4",
        "Star5",
        "Star6",
        "Star7"
    };

    const char* const cQuestionPaneName[2] = { "NewStar1", "NewStar2" };
};

ScenarioSelectLayout::ScenarioSelectLayout(EffectSystem *pSystem, const CameraContext *pContext) : LayoutActor("シナリオ選択レイアウト", true) {
    mSelectedScenarioNo = -1;
    _24 = 0;
    _28 = 0;
    mStars = nullptr;
    mScenarioSky = nullptr;
    mEffectSystem = pSystem;
    mCameraContext = pContext;
    mNewTextFollowPos.x = 0.0f;
    mNewTextFollowPos.y = 0.0f;
    mNewGreenTextFollowPos.x = 0.0f;
    mNewGreenTextFollowPos.y = 0.0f;
    mStarTopFollowPos.x = 0.0f;
    mStarTopFollowPos.y = 0.0f;
    mBackButton = nullptr;
    mMarioIconFollowPos.x = 0.0f;
    mMarioIconFollowPos.y = 0.0f;
    _A0 = 0;
    _A4 = 0;
    _A8 = -1;
    mEffectHostMtx.identity();

    for (u32 i = 0; i < 2; i++) {
        mQuestionPane[i].zero();
    }
}

void ScenarioSelectLayout::init(const JMapInfoIter &rIter) {
    initLayoutManager("ScenarioSelect", 1);
    MR::createAndAddPaneCtrl(this, "ScenarioSelect", 1);
    MR::createAndAddPaneCtrl(this, "ScenarioFrame", 1);
    MR::createAndAddPaneCtrl(this, "Scenario", 1);
    MR::createAndAddPaneCtrl(this, "StarTop", 1);
    MR::createAndAddPaneCtrl(this, "StarDown", 1);
    MR::createAndAddPaneCtrl(this, "BestScore", 1);
    MR::createAndAddPaneCtrl(this, "BestTime", 1);
    MR::createAndAddPaneCtrl(this, "New", 1);
    MR::createAndAddPaneCtrl(this, "NewStarGreen", 1);
    MR::createAndAddPaneCtrl(this, "Mario", 1);
    MR::createAndAddPaneCtrl(this, "CometAppear", 2);
    MR::setFollowPos(&mStarTopFollowPos, this, "StarTop");
    MR::setFollowTypeAdd(this, "StarTop");
    MR::setFollowPos(&mNewTextFollowPos, this, "New");
    MR::setFollowPos(&mNewGreenTextFollowPos, this, "NewStarGreen");
    MR::setFollowPos(&mMarioIconFollowPos, this, "Mario");

    for (u32 i = 0; i < 2; i++) {
        MR::createAndAddPaneCtrl(this, cQuestionPaneName[i], 1);
        MR::setFollowPos(mQuestionPane[i], this, cQuestionPaneName[i]);
    }

    initPointingTarget(7);
    initEffectKeeper(0, "ScenarioSelect", mEffectSystem);
    MR::setEffectHostMtx(this, "ScenarioSelectEffect", mEffectHostMtx.toMtxPtr());
    mStars = new ScenarioSelectStar*[7];

    for (s32 i = 0; i < 7; i++) {
        ScenarioSelectStar* star = new ScenarioSelectStar(mEffectSystem);
        mStars[i] = star;
        mStars[i]->initWithoutIter();
        TVec2f radius;
        radius.x = 0.0f;
        radius.y = 0.0f;
        MR::addStarPointerTargetCircle(this, cStarPaneName[i], 60.0f, radius, nullptr);
    }

    mScenarioSky = new MultiSceneActor("シナリオ選択の空", "ScenarioSelectSky", false);
    mScenarioSky->initWithoutIter();
    mBackButton = new BackButton("戻るボタン", false);
    mBackButton->initWithoutIter();
    initNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvAppearStar::sInstance);
}

void ScenarioSelectLayout::exeDisappearCometWarning() {
    MR::startPaneAnimAtFirstStep(this, "CometAppear", "CometEnd", 0);
    MR::setNerveAtPaneAnimStopped(this, "CometAppear", &NrvScenarioSelectLayout::ScenarioSelectLayoutNrvAppear::sInstance, 0);
}
 
void ScenarioSelectLayout::exeCancelFadeOut() {
    kill();
}

void ScenarioSelectLayout::exeAppearCometWarning() {
    if (MR::isStep(this, 15)) {
        int id = MR::getEncounterGalaxyCometNameId(MR::getCurrentStageName());
        const char* cometName = MR::getCometNameFromId(id);
        char cometMsg[256];
        MR::makeCometMessageID(cometMsg, sizeof(cometMsg), cometName);
        MR::setTextBoxGameMessageRecursive(this, "TextComet", "GalaxyInfo_CometAppearR");
        MR::setTextBoxArgGameMessageRecursive(this, "TextComet", cometMsg, 0);
        MR::showPaneRecursive(this, "CometAppear");
        MR::startPaneAnim(this, "CometAppear", "CometAppear", 0);
        MR::setCometPaneAnimFromId(this, "CometAppear", id, 1);
        MR::startSystemSE("SE_SY_COMET_WARNING_DISP", -1, -1);
    }

    if (MR::isGreaterStep(this, 15)) {
        MR::setNerveAtPaneAnimStopped(this, "CometAppear", &NrvScenarioSelectLayout::ScenarioSelectLayoutNrvWaitCometWarning::sInstance, 0);
    }
}

void ScenarioSelectLayout::exeWaitCometWarning() {
    if (getNerveStep() % 80 == 25) {
        MR::startSystemSE("SE_SY_COMET_WARNING", -1, -1);
    }

    MR::startPaneAnimAtFirstStep(this, "CometAppear", "CometWait", 0);
    MR::setNerveAtStep(this, &NrvScenarioSelectLayout::ScenarioSelectLayoutNrvDisappearCometWarning::sInstance, 180);
}

ScenarioSelectLayout::~ScenarioSelectLayout() {

}

namespace NrvScenarioSelectLayout {
    INIT_NERVE(ScenarioSelectLayoutNrvAppearStar)
    INIT_NERVE(ScenarioSelectLayoutNrvAppear)
    INIT_NERVE(ScenarioSelectLayoutNrvWaitScenarioSelect)
    INIT_NERVE(ScenarioSelectLayoutNrvDecide)
    INIT_NERVE(ScenarioSelectLayoutNrvAfterScenarioSelected)
    INIT_NERVE(ScenarioSelectLayoutNrvDisappear)
    INIT_NERVE(ScenarioSelectLayoutNrvCancel)
    INIT_NERVE(ScenarioSelectLayoutNrvCancelFadeOut)
    INIT_NERVE(ScenarioSelectLayoutNrvAppearCometWarning)
    INIT_NERVE(ScenarioSelectLayoutNrvWaitCometWarning)
    INIT_NERVE(ScenarioSelectLayoutNrvDisappearCometWarning)
};