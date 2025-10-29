#include "Game/Camera/CameraContext.hpp"
#include "Game/Effect/MultiEmitter.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/ScenarioSelectStar.hpp"
#include "Game/Screen/BackButton.hpp"
#include "Game/Screen/ScenarioSelectLayout.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace {
    const char* cStarPaneName[] = {
        "Star1",
        "Star2",
        "Star3",
        "Star4",
        "Star5",
        "Star6",
        "Star7"
    };

    const char* cQuestionPaneName[] = {
        "NewStar1",
        "NewStar2",
    };
    const f32 cPointingTargetRadius = 60.0f;
    const s32 cDecideFrame = 40;
    const s32 cSelectedAccelStartStep = 60;
    const s32 cSelectedAccelFrame = 150;
    const f32 cSelectedEffectRateMin = 2.0f;
    const f32 cSelectedEffectRateMax = 15.0f;
    const f32 cSelectedEffectSpeedMin = 9.0f;
    const f32 cSelectedEffectSpeedMax = 15.0f;
    const s32 cSelectedEndAnimStartStep = 60;
    const s32 cSelectedFadeOutStartStep = 180;
    const s32 cSelectedFadeOutFrame = 30;
    const s32 cSelectedFadeInFrame = 180;
    const s32 cCancelFadeOutFrame = 90;
    const TVec3f cSkyTrans = TVec3f(0.0f, 0.0f, -10000.0f);
    const f32 cSkyScale = 10.0f;
    const s32 cCometWarningWaitFrame = 15;
    const s32 cCometWarningFrame = 180;
};

namespace NrvScenarioSelectLayout {
    NEW_NERVE(ScenarioSelectLayoutNrvAppearStar, ScenarioSelectLayout, AppearStar);
    NEW_NERVE(ScenarioSelectLayoutNrvAppear, ScenarioSelectLayout, Appear);
    NEW_NERVE(ScenarioSelectLayoutNrvWaitScenarioSelect, ScenarioSelectLayout, WaitScenarioSelect);
    NEW_NERVE(ScenarioSelectLayoutNrvDecide, ScenarioSelectLayout, Decide);
    NEW_NERVE(ScenarioSelectLayoutNrvAfterScenarioSelected, ScenarioSelectLayout, AfterScenarioSelected);
    NEW_NERVE(ScenarioSelectLayoutNrvDisappear, ScenarioSelectLayout, Disappear);
    NEW_NERVE(ScenarioSelectLayoutNrvCancel, ScenarioSelectLayout, Cancel);
    NEW_NERVE(ScenarioSelectLayoutNrvCancelFadeOut, ScenarioSelectLayout, CancelFadeOut);
    NEW_NERVE(ScenarioSelectLayoutNrvAppearCometWarning, ScenarioSelectLayout, AppearCometWarning);
    NEW_NERVE(ScenarioSelectLayoutNrvWaitCometWarning, ScenarioSelectLayout, WaitCometWarning);
    NEW_NERVE(ScenarioSelectLayoutNrvDisappearCometWarning, ScenarioSelectLayout, DisappearCometWarning);
};

ScenarioSelectLayout::ScenarioSelectLayout(EffectSystem *pEffectSystem, const CameraContext *pCameraContext) :
    LayoutActor("シナリオ選択レイアウト", true),
    mSelectedScenarioNo(-1),
    _24(0),
    _28(false),
    mStar(nullptr),
    mScenarioSky(nullptr),
    mEffectSystem(pEffectSystem),
    mCameraContext(pCameraContext),
    mNewTextFollowPos(0.0f, 0.0f),
    mNewGreenTextFollowPos(0.0f, 0.0f),
    mStarTopFollowPos(0.0f, 0.0f),
    mBackButton(nullptr),
    mMarioPaneFollowPos(0.0f, 0.0f),
    mMarioPaneName(nullptr),
    _A4(0),
    _A8(-1)
{
    mEffectHostMtx.identity();

    for (s32 i = 0; i < sizeof(mQuestionPane) / sizeof(*mQuestionPane); i++) {
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
    MR::setFollowPos(&mMarioPaneFollowPos, this, "Mario");

    for (s32 i = 0; i < sizeof(mQuestionPane) / sizeof(*mQuestionPane); i++) {
        MR::createAndAddPaneCtrl(this, cQuestionPaneName[i], 1);
        MR::setFollowPos(&mQuestionPane[i], this, cQuestionPaneName[i]);
    }

    initPointingTarget(7);
    initEffectKeeper(0, "ScenarioSelect", mEffectSystem);
    MR::setEffectHostMtx(this, "ScenarioSelectEffect", mEffectHostMtx);

    mStar = new ScenarioSelectStar*[7];

    for (s32 i = 0; i < 7; i++) {
        mStar[i] = new ScenarioSelectStar(mEffectSystem);
        mStar[i]->initWithoutIter();

        MR::addStarPointerTargetCircle(this, cStarPaneName[i], cPointingTargetRadius, TVec2f(0.0f, 0.0f), nullptr);
    }

    mScenarioSky = new MultiSceneActor("シナリオ選択の空", "ScenarioSelectSky", false);
    mScenarioSky->initWithoutIter();

    mBackButton = new BackButton("戻るボタン", false);
    mBackButton->initWithoutIter();

    initNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvAppearStar::sInstance);
}

// ScenarioSelectLayout::appear

void ScenarioSelectLayout::kill() {
    for (s32 i = 0; i < 7; i++) {
        ScenarioSelectStar* pStar = mStar[i];

        if (pStar->_30) {
            continue;
        }

        pStar->kill();
    }

    mScenarioSky->kill();
    mBackButton->kill();
    LayoutActor::kill();
}

void ScenarioSelectLayout::movement() {
    LayoutActor::movement();

    for (s32 i = 0; i < 7; i++) {
        mStar[i]->movement();
    }

    mScenarioSky->movement();
    mBackButton->movement();
}

void ScenarioSelectLayout::calcAnim() {
    LayoutActor::calcAnim();

    for (s32 i = 0; i < 7; i++) {
        mStar[i]->calcAnim();
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
        mStar[i]->calcViewAndEntry();
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

bool ScenarioSelectLayout::isAppearStarEnd() const {
    return !MR::isDead(this)
        && !isNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvAppearStar::sInstance)
        && !isNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvAppearCometWarning::sInstance)
        && !isNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvWaitCometWarning::sInstance)
        && !isNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvDisappearCometWarning::sInstance);
}

bool ScenarioSelectLayout::isEndAnimStartStep() const {
    return isNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvAfterScenarioSelected::sInstance)
        && MR::isStep(this, cSelectedEndAnimStartStep);
}

bool ScenarioSelectLayout::isCanceled() const {
    return isNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvCancel::sInstance)
        || isNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvCancelFadeOut::sInstance);
}

void ScenarioSelectLayout::control() {
    if (!_28 && mSelectedScenarioNo > 0) {
        getSelectedStar()->tryPointing();
    }

    MR::copyPaneTrans(&mMarioPaneFollowPos, this, mMarioPaneName);
}

// ScenarioSelectLayout::updateSelectedScenario

void ScenarioSelectLayout::updateScenarioText() {
    bool isEndAnimStep = isNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvAfterScenarioSelected::sInstance)
        && MR::isGreaterEqualStep(this, cSelectedEndAnimStartStep);

    if (!MR::isHiddenPane(this, "Scenario") && (mSelectedScenarioNo <= 0 || _A8 == mSelectedScenarioNo)) {
        fadeInText();
    }
    else if (!isEndAnimStep && mSelectedScenarioNo > 0) {
        if (_A4 == 0) {
            if (MR::isPaneAnimStopped(this, "Scenario", 0) && mSelectedScenarioNo <= 0) {
                fadeOutText();
            }
        }
        else if (MR::isPaneAnimStopped(this, "Scenario", 0)) {
            MR::hidePaneRecursive(this, "Scenario");
        }
    }
}

// ScenarioSelectLayout::fadeInText
// ScenarioSelectLayout::fadeOutText

bool ScenarioSelectLayout::trySelect() {
    if (MR::testDPDMenuPadDecideTrigger()) {
        if (mSelectedScenarioNo > 0) {
            return true;
        }

        if (!MR::isStarPointerInScreen(0)) {
            for (s32 i = 0; i < 7; i++) {
                if (mStar[i]->_30) {
                    continue;
                }

                if (mStar[i]->_44) {
                    continue;
                }

                mSelectedScenarioNo = mStar[i]->mFrame;

                return true;
            }
        }
    }

    return false;
}

ScenarioSelectStar* ScenarioSelectLayout::getSelectedStar() const {
    for (s32 i = 0; i < 7; i++) {
        if (mStar[i]->_30) {
            continue;
        }

        if (mSelectedScenarioNo != mStar[i]->mFrame) {
            continue;
        }

        return mStar[i];
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
    ScenarioSelectStar* pStar = mStar[starIdx];

    TVec2f starPaneTrans;
    MR::copyPaneTrans(&starPaneTrans, this, cStarPaneName[starIdx]);

    TVec3f worldPos;
    calcWorldPositionFromScreenPos(&worldPos, starPaneTrans, 1000.0f);

    pStar->setup(a2, a3, worldPos, starIdx);
    pStar->appear();
}

bool ScenarioSelectLayout::isAppearStarEndAll() const {
    for (s32 i = 0; i < 7; i++) {
        if (mStar[i]->_30) {
            continue;
        }

        if (mStar[i]->isAppearEnd()) {
            continue;
        }

        return false;
    }

    return true;
}

bool ScenarioSelectLayout::tryCancel() {
    if (mBackButton->_24) {
        setNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvCancel::sInstance);
        return true;
    }

    if (MR::testCorePadTriggerB(WPAD_CHAN0)) {
        MR::startSystemSE("SE_SY_GALAXY_DECIDE_CANCEL", -1, -1);
        mBackButton->disappear();
        setNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvCancel::sInstance);
        return true;
    }

    return false;
}

void ScenarioSelectLayout::setPlayerLeft() {
    MR::setTextBoxGameMessageRecursive(this, "ShaMario", "PlayerLeft");
    MR::setTextBoxNumberRecursive(this, "ShaMarioNum", MR::getPlayerLeft());

    if (MR::getPlayerLeft() < 10) {
        mMarioPaneName = "MarioPosition10";
    }
    else {
        mMarioPaneName = "MarioPosition01";
    }
}

void ScenarioSelectLayout::startAnimAllNewPane(const char *pAnimName) {
    MR::startPaneAnim(this, "New", pAnimName, 0);
    MR::startPaneAnim(this, "NewStarGreen", pAnimName, 0);

    for (s32 i = 0; i < sizeof(cQuestionPaneName) / sizeof(*cQuestionPaneName); i++) {
        MR::startPaneAnim(this, cQuestionPaneName[i], pAnimName, 0);
    }
}

void ScenarioSelectLayout::setAnimRateAllNewPane(f32 rate) {
    MR::setPaneAnimRate(this, "New", rate, 0);
    MR::setPaneAnimRate(this, "NewStarGreen", rate, 0);

    for (s32 i = 0; i < sizeof(cQuestionPaneName) / sizeof(*cQuestionPaneName); i++) {
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
        MR::setEffectRate(this, "ScenarioSelectEffect", cSelectedEffectRateMin);
        MR::setEffectDirectionalSpeed(this, "ScenarioSelectEffect", cSelectedEffectSpeedMin);

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

    if (tryCancel()) {
        return;
    }

    if (trySelect()) {
        _28 = true;
        MR::startSystemSE("SE_SY_DECIDE_1", -1, -1);
        MR::startCSSound("CS_CLICK_CLOSE", nullptr, 0);
        mBackButton->disappear();
        setNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvDecide::sInstance);
    }
}

void ScenarioSelectLayout::exeDecide() {
    if (MR::isFirstStep(this)) {
        for (s32 i = 0; i < 7; i++) {
            if (mStar[i]->_30) {
                continue;
            }

            if (mStar[i] == getSelectedStar()) {
                mStar[i]->select();
            }
            else {
                mStar[i]->notSelect();
            }
        }

        startAnimAllNewPane("End");
    }

    updateScenarioText();
    MR::startSystemLevelSE("SE_DM_LV_SENARIO_SEL_FLY", -1, -1);
    MR::setNerveAtStep(this,  &NrvScenarioSelectLayout::ScenarioSelectLayoutNrvAfterScenarioSelected::sInstance, cDecideFrame);
}

void ScenarioSelectLayout::exeAfterScenarioSelected() {
    MR::startPaneAnimAtStep(this, "ScenarioFrame", "End", cSelectedEndAnimStartStep, 0);
    updateScenarioText();
    MR::startSystemLevelSE("SE_DM_LV_SENARIO_SEL_FLY", -1, -1);

    f32 rate = MR::calcNerveEaseInValue(
        this,
        cSelectedAccelStartStep,
        cSelectedAccelFrame,
        cSelectedEffectRateMin,
        cSelectedEffectRateMax);
    f32 directionalSpeed = MR::calcNerveEaseInValue(
        this,
        cSelectedAccelStartStep,
        cSelectedAccelFrame,
        cSelectedEffectSpeedMin,
        cSelectedEffectSpeedMax);

    MR::setEffectRate(this, "ScenarioSelectEffect", rate);
    MR::setEffectDirectionalSpeed(this, "ScenarioSelectEffect", directionalSpeed);

    if (MR::isStep(this, cSelectedFadeOutStartStep)) {
        MR::stopStageBGM(cSelectedFadeOutFrame);
        MR::closeSystemWipeWhiteFade(cSelectedFadeOutFrame);
    }
}

void ScenarioSelectLayout::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::hideLayout(this);
        MR::getEffect(this, "ScenarioSelectEffect")->forceDelete(mEffectSystem);
    
        for (s32 i = 0; i < 7; i++) {
            ScenarioSelectStar* star = mStar[i];

            if (!star->_30) {
                star->kill();
            }
        }

        mScenarioSky->kill();
        MR::openSystemWipeWhiteFade(cSelectedFadeInFrame);
    }

    if (MR::isSystemWipeActive()) {
        return;
    }

    kill();
}

void ScenarioSelectLayout::exeCancel() {
    if (MR::isFirstStep(this)) {
        mSelectedScenarioNo = -1;
    }

    updateScenarioText();

    if (mBackButton->isHidden()) {
        MR::closeSystemWipeFadeWithCaptureScreen(cCancelFadeOutFrame);
        MR::stopStageBGM(1);
        setNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvCancelFadeOut::sInstance);
    }
}

void ScenarioSelectLayout::exeAppearCometWarning() {
    if (MR::isStep(this, cCometWarningWaitFrame)) {
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

    if (MR::isGreaterStep(this, cCometWarningWaitFrame)) {
        MR::setNerveAtPaneAnimStopped(this, "CometAppear", &NrvScenarioSelectLayout::ScenarioSelectLayoutNrvWaitCometWarning::sInstance, 0);
    }
}

void ScenarioSelectLayout::exeWaitCometWarning() {
    if (getNerveStep() % 80 == 25) {
        MR::startSystemSE("SE_SY_COMET_WARNING", -1, -1);
    }

    MR::startPaneAnimAtFirstStep(this, "CometAppear", "CometWait", 0);
    MR::setNerveAtStep(this, &NrvScenarioSelectLayout::ScenarioSelectLayoutNrvDisappearCometWarning::sInstance, cCometWarningFrame);
}
