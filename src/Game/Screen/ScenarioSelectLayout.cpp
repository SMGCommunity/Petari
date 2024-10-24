#include "Game/Screen/ScenarioSelectLayout.hpp"
#include "Game/Effect/MultiEmitter.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/Util.hpp"

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
        MR::setFollowPos(&mQuestionPane[i], this, cQuestionPaneName[i]);
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

// void ScenarioSelectLayout::appear

void ScenarioSelectLayout::kill() {
    for (s32 i = 0; i < 7; i++) {
        ScenarioSelectStar* star = mStars[i];

        if (!star->_30) {
            star->kill();
        }
    }

    mScenarioSky->kill();
    mBackButton->kill();
    LayoutActor::kill();
}

void ScenarioSelectLayout::movement() {
    LayoutActor::movement();

    for (s32 i = 0; i < 7; i++) {
        mStars[i]->movement();
    }

    mScenarioSky->movement();
    mBackButton->movement();
}

void ScenarioSelectLayout::calcAnim() {
    LayoutActor::calcAnim();

    for (s32 i = 0; i < 7; i++) {
        mStars[i]->calcAnim();
    }

    mScenarioSky->calcAnim();
    mBackButton->calcAnim();
}

void ScenarioSelectLayout::draw() const {
    LayoutActor::draw();
    mBackButton->draw();
}

void ScenarioSelectLayout::calcViewAndEntryStarModel() {
    for (s32 i = 0; i < 7; i++) {
        mStars[i]->calcViewAndEntry();
    }

    mScenarioSky->calcViewAndEntry();
}

s32 ScenarioSelectLayout::getSelectedScenarioNo() const {
    return mSelectedScenarioNo;
}

bool ScenarioSelectLayout::isReadyToDisappear() const {
    if (isNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvAfterScenarioSelected::sInstance)) {
        return MR::isGreaterStep(this, 210);
    }

    return false;
}

void ScenarioSelectLayout::disappear() {
    setNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvDisappear::sInstance);
}

/*
bool ScenarioSelectLayout::trySelect() {
    if (!MR::testDPDMenuPadDecideTrigger() || mSelectedScenarioNo <= 0) {
        return false;
    }

    if (MR::isStarPointerInScreen(0)) {
        return false;
    }

    for (s32 i = 0; i < 7; i++) {
        if (!mStars[i]->_30 && !mStars[i]->_44) {
            mSelectedScenarioNo = mStars[i]->mFrame;
            return true;
        }
    }

    return false;
}
*/

ScenarioSelectStar* ScenarioSelectLayout::getSelectedStar() const {
    for (s32 i = 0; i < 7; i++) {
        if (!mStars[i]->_30 && mSelectedScenarioNo == mStars[i]->mFrame) {
            return mStars[i];
        }
    }

    return nullptr;
}

// ScenarioSelectLayout::calcWorldPositionFromScreenPos
// ScenarioSelectLayout::calcDisplayScenarioNum

/*
void ScenarioSelectLayout::appearAllStar(s32 starCount, s32 a3, bool a4) {
    if (MR::isScenarioDecided()) {
        s32 v7 = 2;

        if (a4) {
            v7 = 0;
        }

        appearStar(0, MR::getCurrentScenarioNo(), v7);
    }
    else {
        for (s32 i = 0; i < starCount; i++) {
            s32 v10;
            
            if (a4) {
                v10 = starCount % 80;
            }
            else {
                v10 = 2;
            }

            appearStar(i, i + 1, v10);
        }

        s32 v11 = 3;

        for (s32 j = 0; ; j++) {
            s32 v14 = MR::getCurrentStageNormalScenarioNum();

            if (j >= MR::getCurrentStagePowerStarNum() - v14) {
                break;
            }

            s32 v13 = j + MR::getCurrentStageNormalScenarioNum() + 1;

            if (MR::hasPowerStarInCurrentStage(v13)) {
                appearStar(v11++, v13, (a4 == 0) - 1 + 2);
            }
        }
    }
}
*/

void ScenarioSelectLayout::appearStar(s32 starIdx, s32 a2, int a3) {
    ScenarioSelectStar* star = mStars[starIdx];
    TVec2f starPane;
    MR::copyPaneTrans(&starPane, this, cStarPaneName[starIdx]);
    TVec3f worldPos;
    calcWorldPositionFromScreenPos(&worldPos, starPane, 1000.0f);
    star->setup(a2, a3, worldPos, starIdx);
    star->appear();
}

bool ScenarioSelectLayout::isAppearStarEndAll() const {
    for (s32 i = 0; i < 7; i++) {
        if (!mStars[i]->_30 && !mStars[i]->isAppearEnd()) {
            return false;
        }
    }

    return true;
}

bool ScenarioSelectLayout::tryCancel() {
    if (mBackButton->_24) {
        setNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvCancel::sInstance);
        return true;
    }
    else if (MR::testCorePadTriggerB(0)) {
        MR::startSystemSE("SE_SY_GALAXY_DECIDE_CANCEL", -1, -1);
        mBackButton->disappear();
        setNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvCancel::sInstance);
        return true;
    }

    return false;
}

void ScenarioSelectLayout::setPlayerLeft() {
    MR::setTextBoxGameMessageRecursive(this, "ShaMario", "PlayerLeft");
    s32 playerLeft = MR::getPlayerLeft();
    MR::setTextBoxNumberRecursive(this, "ShaMarioNum", playerLeft);

    if (MR::getPlayerLeft() >= 10) {
        _A0 = "MarioPosition10";
    }
    else {
        _A0 = "MarioPosition01";
    }
}

void ScenarioSelectLayout::startAnimAllNewPane(const char *pAnim) {
    MR::startPaneAnim(this, "New", pAnim, 0);
    MR::startPaneAnim(this, "NewStarGreen", pAnim, 0);

    for (u32 i = 0; i < 2; i++) {
        MR::startPaneAnim(this, cQuestionPaneName[i], pAnim, 0);
    }
}

void ScenarioSelectLayout::setAnimRateAllNewPane(f32 rate) {
    MR::setPaneAnimRate(this, "New", rate, 0);
    MR::setPaneAnimRate(this, "NewStarGreen", rate, 0);

    for (u32 i = 0; i < 2; i++) {
        MR::setPaneAnimRate(this, cQuestionPaneName[i], rate, 0);
    }
}

void ScenarioSelectLayout::exeDisappearCometWarning() {
    MR::startPaneAnimAtFirstStep(this, "CometAppear", "CometEnd", 0);
    MR::setNerveAtPaneAnimStopped(this, "CometAppear", &NrvScenarioSelectLayout::ScenarioSelectLayoutNrvAppear::sInstance, 0);
}
 
void ScenarioSelectLayout::exeCancelFadeOut() {
    kill();
}

void ScenarioSelectLayout::exeAppearStar() {
    if (MR::isFirstStep(this)) {
        MR::permitTrigSE();
        MR::getEffect(this, "ScenarioSelectEffect")->create(mEffectSystem);
        MR::setEffectRate(this, "ScenarioSelectEffect", 2.0f);
        MR::setEffectDirectionalSpeed(this, "ScenarioSelectEffect", 9.0f);

        if (!MR::isPlayingStageBgmName("BGM_SENARIO_SEL")) {
            MR::startStageBGM("BGM_SENARIO_SE_2", 0);
        }
    }

    MR::startSystemLevelSE("SE_DM_LV_SENARIO_SE_FLY", -1, -1);

    if (isAppearStarEndAll()) {
        s32 scenarioNo;
        bool needsAppearComet;
        if (!MR::isScenarioDecided()) {
            needsAppearComet = false;
        }
        else {
            scenarioNo = MR::getCurrentScenarioNo();
            needsAppearComet = false;
            GalaxyStatusAccessor galaxyAccessor = MR::makeCurrentGalaxyStatusAccessor();

            if (galaxyAccessor.isCometStar(scenarioNo)) {
                if (scenarioNo == MR::getEncounterGalaxyCometPowerStarId(MR::getCurrentStageName())) {
                    needsAppearComet = true;
                }
            }
        }

        if (needsAppearComet) {
            setNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvAppearCometWarning::sInstance);
        }
        else {
            setNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvAppear::sInstance);
        }
    }
}

void ScenarioSelectLayout::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::setPaneAnimRate(this, "ScenarioSelect", 1.0f, 0);
        setAnimRateAllNewPane(1.0f);
    }

    MR::startSystemLevelSE("SE_DM_LV_SENARIO_SEL_FLY", -1, -1);
    MR::setNerveAtPaneAnimStopped(this, "ScenarioSelect", &NrvScenarioSelectLayout::ScenarioSelectLayoutNrvWaitScenarioSelect::sInstance, 0);
}

void ScenarioSelectLayout::exeWaitScenarioSelect() {
    if (MR::isFirstStep(this)) {
        startAnimAllNewPane("NewWait");
        mBackButton->appear();
    }

    updateSelectedScenario();
    updateScenarioText();
    MR::startSystemLevelSE("SE_DM_LV_SENARIO_SEL_FLY", -1, -1);

    if (!tryCancel()) {
        if (trySelect()) {
            _28 = 1;
            MR::startSystemSE("SE_SY_DECIDE_1", -1, -1);
            MR::startCSSound("CS_CLICK_CLOSE", nullptr, 0);
            mBackButton->disappear();
            setNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvDecide::sInstance);
        }
    }
}

void ScenarioSelectLayout::exeDecide() {
    if (MR::isFirstStep(this)) {
        for (s32 i = 0; i < 7; i++) {
            if (!mStars[i]->_30) {
                if (mStars[i] == getSelectedStar()) {
                    mStars[i]->select();
                }
                else {
                    mStars[i]->notSelect();
                }
            }
        }

        startAnimAllNewPane("End");
    }

    updateScenarioText();
    MR::startSystemSE("SE_DM_LV_SENARIO_SEL_FLY", -1, -1);
    MR::setNerveAtStep(this,  &NrvScenarioSelectLayout::ScenarioSelectLayoutNrvAfterScenarioSelected::sInstance, 40);
}

void ScenarioSelectLayout::exeAfterScenarioSelected() {
    MR::startPaneAnimAtStep(this, "ScenarioFrame", "End", 60, 0);
    updateScenarioText();
    MR::startSystemLevelSE("SE_DM_LV_SENARIO_SEL_FLY", -1, -1);
    f32 v2 = MR::calcNerveEaseInValue(this, 60, 150, 2.0f, 15.0f);
    f32 v3 = MR::calcNerveEaseInValue(this, 60, 150, 9.0f, 15.0f);
    MR::setEffectRate(this, "ScenarioSelectEffect", v2);
    MR::setEffectDirectionalSpeed(this, "ScenarioSelectEffect", v3);

    if (MR::isStep(this, 180)) {
        MR::stopStageBGM(30);
        MR::closeSystemWipeWhiteFade(30);
    }
}

void ScenarioSelectLayout::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::hideLayout(this);
        MR::getEffect(this, "ScenarioSelectEffect")->forceDelete(mEffectSystem);
    
        for (s32 i = 0; i < 7; i++) {
            ScenarioSelectStar* star = mStars[i];

            if (!star->_30) {
                star->kill();
            }
        }

        mScenarioSky->kill();
        MR::openSystemWipeWhiteFade(180);
    }

    if (!MR::isSystemWipeActive()) {
        kill();
    }
}

void ScenarioSelectLayout::exeCancel() {
    if (MR::isFirstStep(this)) {
        mSelectedScenarioNo = -1;
    }

    updateScenarioText();

    if (mBackButton->isHidden()) {
        MR::closeSystemWipeCircleWithCaptureScreen(90);
        MR::stopStageBGM(1);
        setNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvCancelFadeOut::sInstance);
    }
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
