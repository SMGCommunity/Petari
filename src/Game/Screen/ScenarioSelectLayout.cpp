#include "Game/Screen/ScenarioSelectLayout.hpp"
#include "Game/Camera/CameraContext.hpp"
#include "Game/Effect/MultiEmitter.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/RaceManager.hpp"
#include "Game/Map/ScenarioSelectStar.hpp"
#include "Game/Screen/BackButton.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/System/GameDataConst.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/StringUtil.hpp"

namespace {
    const char* const cStarPaneName[] = {"Star1", "Star2", "Star3", "Star4", "Star5", "Star6", "Star7"};
    const char* const cQuestionPaneName[] = {
        "NewStar1",
        "NewStar2",
    };
    const Vec cSkyTrans = {0.0f, 0.0f, -10000.0f};
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
    const f32 cSkyScale = 10.0f;
    const s32 cCometWarningWaitFrame = 15;
    const s32 cCometWarningFrame = 180;
};  // namespace

namespace {
    bool isCurrentStageRaceComplete(s32 scenarioNo) {
        const char* stageName = MR::getCurrentStageName();

        return MR::isOnGameEventFlagPowerStarSuccess(stageName, scenarioNo) && RaceManagerFunction::getRaceId(stageName, scenarioNo) >= 0;
    }
};  // namespace

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
};  // namespace NrvScenarioSelectLayout

ScenarioSelectLayout::ScenarioSelectLayout(EffectSystem* pEffectSystem, const CameraContext* pCameraContext)
    : LayoutActor("シナリオ選択レイアウト", true), mSelectedScenarioNo(-1), _24(0), _28(false), mStar(nullptr), mScenarioSky(nullptr),
      mEffectSystem(pEffectSystem), mCameraContext(pCameraContext), mNewTextFollowPos(0.0f, 0.0f), mNewGreenTextFollowPos(0.0f, 0.0f),
      mStarTopFollowPos(0.0f, 0.0f), mBackButton(nullptr), mMarioPaneFollowPos(0.0f, 0.0f), mMarioPaneName(nullptr), _A4(0), _A8(-1) {
    mEffectHostMtx.identity();

    for (u32 i = 0; i < ARRAY_SIZE(mQuestionPane); i++) {
        mQuestionPane[i].zero();
    }
}

void ScenarioSelectLayout::init(const JMapInfoIter& rIter) {
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

    for (u32 i = 0; i < ARRAY_SIZE(mQuestionPane); i++) {
        MR::createAndAddPaneCtrl(this, ::cQuestionPaneName[i], 1);
        MR::setFollowPos(&mQuestionPane[i], this, ::cQuestionPaneName[i]);
    }

    initPointingTarget(7);
    initEffectKeeper(0, "ScenarioSelect", mEffectSystem);
    MR::setEffectHostMtx(this, "ScenarioSelectEffect", mEffectHostMtx);

    mStar = new ScenarioSelectStar*[7];

    for (s32 i = 0; i < 7; i++) {
        mStar[i] = new ScenarioSelectStar(mEffectSystem);
        mStar[i]->initWithoutIter();

        MR::addStarPointerTargetCircle(this, ::cStarPaneName[i], ::cPointingTargetRadius, TVec2f(0.0f, 0.0f), nullptr);
    }

    mScenarioSky = new MultiSceneActor("シナリオ選択の空", "ScenarioSelectSky", false);
    mScenarioSky->initWithoutIter();

    mBackButton = new BackButton("戻るボタン", false);
    mBackButton->initWithoutIter();

    initNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvAppearStar::sInstance);
}

void ScenarioSelectLayout::appear() {
    MR::showLayout(this);

    f32 scale;
    s32 extraScenarioCompleteNum;
    s32 normalScenarioCompleteNum;
    bool isNewScenario = calcDisplayScenarioNum(&normalScenarioCompleteNum, &extraScenarioCompleteNum);

    MR::startPaneAnim(this, "ScenarioSelect", "Appear", 0);
    MR::setPaneAnimFrameAndStop(this, "ScenarioSelect", 0.0f, 0);
    startAnimAllNewPane("Appear");
    setAnimRateAllNewPane(0.0f);
    MR::startPaneAnim(this, "ScenarioFrame", "Wait", 0);
    MR::startPaneAnim(this, "StarTop", "StarPositionTop", 0);
    MR::setPaneAnimFrameAndStop(this, "StarTop", normalScenarioCompleteNum - 1, 0);

    scale = 50.0f;

    if (extraScenarioCompleteNum > 0) {
        MR::startPaneAnim(this, "StarDown", "StarPositionDown", 0);
        MR::setPaneAnimFrameAndStop(this, "StarDown", extraScenarioCompleteNum - 1, 0);

        scale = 0.0f;
        mStarTopFollowPos.x = 0.0f;
        mStarTopFollowPos.y = scale;
    } else {
        mStarTopFollowPos.x = 0.0f;
        mStarTopFollowPos.y = scale;
    }

    MR::setTextBoxMessageRecursive(this, "Galaxy", MR::getCurrentGalaxyNameOnCurrentLanguage());
    MR::hidePaneRecursive(this, "Scenario");
    setPlayerLeft();
    LayoutActor::appear();
    appearAllStar(normalScenarioCompleteNum, extraScenarioCompleteNum, isNewScenario);
    MR::hidePaneRecursive(this, "New");
    MR::hidePaneRecursive(this, "NewGreenStar");

    for (u32 i = 0; i < ARRAY_SIZE(::cQuestionPaneName); i++) {
        MR::hidePaneRecursive(this, ::cQuestionPaneName[i]);
    }

    if (isNewScenario) {
        MR::showPaneRecursive(this, "New");
        MR::copyPaneTrans(&mNewTextFollowPos, this, ::cStarPaneName[normalScenarioCompleteNum - 1]);
    } else if (!MR::isScenarioDecided()) {
        const char* stageName = MR::getCurrentStageName();

        for (s32 scenarioNo = MR::getCurrentStageNormalScenarioNum(); scenarioNo <= MR::getCurrentStagePowerStarNum(); scenarioNo++) {
            if (!MR::makeCurrentGalaxyStatusAccessor().isHiddenStar(scenarioNo)) {
                continue;
            }

            if (MR::isOnGameEventFlagPowerStarSuccess(stageName, scenarioNo)) {
                continue;
            }

            if (GameDataConst::isPowerStarLuigiHas(stageName, scenarioNo) && !MR::isLuigiHidingGalaxyAndScenario(stageName, scenarioNo)) {
                continue;
            }

            const char* starPaneName = ::cStarPaneName[MR::getPlacedHiddenStarScenarioNo(stageName, scenarioNo) - 1];

            if (MR::isPowerStarGreenInCurrentStage(scenarioNo)) {
                MR::showPaneRecursive(this, "NewStarGreen");
                MR::copyPaneTrans(&mNewGreenTextFollowPos, this, starPaneName);
            } else {
                MR::showPaneRecursive(this, ::cQuestionPaneName[scenarioNo]);
                MR::copyPaneTrans(&mQuestionPane[scenarioNo], this, starPaneName);
            }
        }
    }

    MR::startPaneAnim(this, "BestScore", "SelectIn", 0);
    MR::startPaneAnim(this, "BestTime", "SelectIn", 0);
    MR::setPaneAnimFrameAndStop(this, "BestScore", 0.0f, 0);
    MR::setPaneAnimFrameAndStop(this, "BestTime", 0.0f, 0);

    mScenarioSky->mTranslation = ::cSkyTrans;
    mScenarioSky->mScale = scale;
    mScenarioSky->appear();
    MultiScene::startBtk(mScenarioSky, "ScenarioSelectSky");
    MR::hidePaneRecursive(this, "CometAppear");
    mSelectedScenarioNo = -1;
    _24 = 0;
    _28 = 0;

    setNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvAppearStar::sInstance);
}

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
    return !MR::isDead(this) && !isNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvAppearStar::sInstance) &&
           !isNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvAppearCometWarning::sInstance) &&
           !isNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvWaitCometWarning::sInstance) &&
           !isNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvDisappearCometWarning::sInstance);
}

bool ScenarioSelectLayout::isEndAnimStartStep() const {
    return isNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvAfterScenarioSelected::sInstance) &&
           MR::isStep(this, ::cSelectedEndAnimStartStep);
}

bool ScenarioSelectLayout::isCanceled() const {
    return isNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvCancel::sInstance) ||
           isNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvCancelFadeOut::sInstance);
}

void ScenarioSelectLayout::control() {
    if (!_28 && mSelectedScenarioNo > 0) {
        getSelectedStar()->tryPointing();
    }

    MR::copyPaneTrans(&mMarioPaneFollowPos, this, mMarioPaneName);
}

void ScenarioSelectLayout::updateSelectedScenario() {
    ScenarioSelectStar* star;

    if (isNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvWaitScenarioSelect::sInstance)) {
        for (s32 i = 0; i < 7; i++) {
            star = mStar[i];

            if (!star->_30 && MR::isStarPointerPointingTarget(this, ::cStarPaneName[i], 0, true, "弱")) {
                mSelectedScenarioNo = star->mScenarioNo;
                _24 = 0;
                return;
            }
        }
    }

    if (_24 < 0) {
        _24++;
    }

    if (_24 >= 0) {
        mSelectedScenarioNo = -1;
    }
}

void ScenarioSelectLayout::updateScenarioText() {
    bool isEndAnimStep = isNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvAfterScenarioSelected::sInstance) &&
                         MR::isGreaterEqualStep(this, ::cSelectedEndAnimStartStep);

    if (MR::isHiddenPane(this, "Scenario") || (mSelectedScenarioNo > 0 && getSelectedScenarioNo() != _A8)) {
        if (!isEndAnimStep && mSelectedScenarioNo > 0) {
            fadeInText();
        }
    } else if (_A4 == 0) {
        if (MR::isPaneAnimStopped(this, "Scenario", 0) && mSelectedScenarioNo <= 0) {
            fadeOutText();
        }
    } else if (MR::isPaneAnimStopped(this, "Scenario", 0)) {
        MR::hidePaneRecursive(this, "Scenario");
    }
}

void ScenarioSelectLayout::fadeInText() {
    const char* stageName = MR::getCurrentStageName();

    _A4 = 0;
    _A8 = mSelectedScenarioNo;

    MR::setTextBoxMessageRecursive(this, "Scenario", MR::getScenarioNameOnCurrentLanguage(stageName, mSelectedScenarioNo));
    MR::showPaneRecursive(this, "Scenario");
    MR::startPaneAnim(this, "Scenario", "SelectIn", 0);

    bool isPlacedCoin = MR::isPlacedCoin(MR::getCurrentStageName(), mSelectedScenarioNo);

    MR::startPaneAnim(this, "BestScore", "SelectIn", 0);

    if (isPlacedCoin) {
        MR::setTextBoxFormatRecursive(this, "ShaCoinnum", L"%03d", MR::getCoinBestScore(stageName, mSelectedScenarioNo));
    } else {
        MR::setPaneAnimFrameAndStop(this, "BestScore", 0.0f, 0);
    }

    MR::startPaneAnim(this, "BestTime", "SelectIn", 0);

    if (::isCurrentStageRaceComplete(mSelectedScenarioNo)) {
        wchar_t bestTime[16];
        MR::makeRaceBestTimeString(bestTime, RaceManagerFunction::getRaceId(stageName, mSelectedScenarioNo));
        MR::setTextBoxMessageRecursive(this, "ShaTime", bestTime);
    } else {
        MR::setPaneAnimFrameAndStop(this, "BestTime", 0.0f, 0);
        MR::clearTextBoxMessageRecursive(this, "ShaTime");
    }
}

void ScenarioSelectLayout::fadeOutText() {
    const char* stageName = MR::getCurrentStageName();

    _A4 = 1;

    MR::startPaneAnim(this, "Scenario", "SelectOut", 0);

    if (MR::isPlacedCoin(MR::getCurrentStageName(), _A8)) {
        MR::startPaneAnim(this, "BestScore", "SelectOut", 0);
    }

    if (::isCurrentStageRaceComplete(_A8)) {
        MR::startPaneAnim(this, "BestTime", "SelectOut", 0);
    }
}

bool ScenarioSelectLayout::trySelect() {
    if (MR::testDPDMenuPadDecideTrigger()) {
        if (mSelectedScenarioNo > 0) {
            return true;
        }

        if (!MR::isStarPointerInScreen(WPAD_CHAN0)) {
            for (s32 i = 0; i < 7; i++) {
                if (mStar[i]->_30) {
                    continue;
                }

                if (mStar[i]->mStarCollectedStatus) {
                    continue;
                }

                mSelectedScenarioNo = mStar[i]->mScenarioNo;

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

        if (mSelectedScenarioNo != mStar[i]->mScenarioNo) {
            continue;
        }

        return mStar[i];
    }

    return nullptr;
}

// ScenarioSelectLayout::calcWorldPositionFromScreenPos

bool ScenarioSelectLayout::calcDisplayScenarioNum(s32* pNormalScenarioCompleteNum, s32* pExtraScenarioCompleteNum) const {
    if (MR::isScenarioDecided()) {
        *pNormalScenarioCompleteNum = 1;
        *pExtraScenarioCompleteNum = 0;

        return !MR::hasPowerStarInCurrentStage(MR::getCurrentScenarioNo());
    }

    bool isNewScenario = false;

    for (*pNormalScenarioCompleteNum = 0; *pNormalScenarioCompleteNum < MR::getCurrentStageNormalScenarioNum(); *pNormalScenarioCompleteNum += 1) {
        if (MR::hasPowerStarInCurrentStage(*pNormalScenarioCompleteNum + 1)) {
            continue;
        }

        *pNormalScenarioCompleteNum += 1;
        isNewScenario = true;
        break;
    }

    *pExtraScenarioCompleteNum = 0;

    for (s32 i = 0; i < MR::getCurrentStagePowerStarNum() - MR::getCurrentStageNormalScenarioNum(); i++) {
        if (MR::hasPowerStarInCurrentStage(MR::getCurrentStageNormalScenarioNum() + i + 1)) {
            *pExtraScenarioCompleteNum += 1;
        }
    }

    return isNewScenario;
}

void ScenarioSelectLayout::appearAllStar(s32 normalScenarioCompleteNum, s32 extraScenarioCompleteNum, bool isNewScenario) {
    if (MR::isScenarioDecided()) {
        s32 v7 = 2;

        if (isNewScenario) {
            v7 = 0;
        }

        appearStar(0, MR::getCurrentScenarioNo(), v7);
    } else {
        for (s32 i = 0; i < normalScenarioCompleteNum; i++) {
            s32 v10;

            if (isNewScenario) {
                v10 = normalScenarioCompleteNum > 0;
            } else {
                v10 = 2;
            }

            appearStar(i, i + 1, v10);
        }

        s32 v11 = 3;

        for (s32 j = 0; j < MR::getCurrentStagePowerStarNum() - MR::getCurrentStageNormalScenarioNum(); j++) {
            s32 v13 = MR::getCurrentStageNormalScenarioNum() + j + 1;

            if (MR::hasPowerStarInCurrentStage(v13)) {
                appearStar(v11++, v13, !isNewScenario - 1 + 2);
            }
        }
    }
}

void ScenarioSelectLayout::appearStar(s32 starId, s32 scenarioNo, int a3) {
    ScenarioSelectStar* pStar = mStar[starId];

    TVec2f starPaneTrans;
    MR::copyPaneTrans(&starPaneTrans, this, ::cStarPaneName[starId]);

    TVec3f worldPos;
    calcWorldPositionFromScreenPos(&worldPos, starPaneTrans, 1000.0f);

    pStar->setup(scenarioNo, a3, worldPos, starId);
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
        MR::startSystemSE("SE_SY_GALAXY_DECIDE_CANCEL");
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
    } else {
        mMarioPaneName = "MarioPosition01";
    }
}

void ScenarioSelectLayout::startAnimAllNewPane(const char* pAnimName) {
    MR::startPaneAnim(this, "New", pAnimName, 0);
    MR::startPaneAnim(this, "NewStarGreen", pAnimName, 0);

    for (u32 i = 0; i < ARRAY_SIZE(::cQuestionPaneName); i++) {
        MR::startPaneAnim(this, ::cQuestionPaneName[i], pAnimName, 0);
    }
}

void ScenarioSelectLayout::setAnimRateAllNewPane(f32 rate) {
    MR::setPaneAnimRate(this, "New", rate, 0);
    MR::setPaneAnimRate(this, "NewStarGreen", rate, 0);

    for (u32 i = 0; i < ARRAY_SIZE(::cQuestionPaneName); i++) {
        MR::setPaneAnimRate(this, ::cQuestionPaneName[i], rate, 0);
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
        MR::setEffectRate(this, "ScenarioSelectEffect", ::cSelectedEffectRateMin);
        MR::setEffectDirectionalSpeed(this, "ScenarioSelectEffect", ::cSelectedEffectSpeedMin);

        if (!MR::isPlayingStageBgmName("BGM_SENARIO_SEL")) {
            MR::startStageBGM("BGM_SENARIO_SE_2", false);
        }
    }

    MR::startSystemLevelSE("SE_DM_LV_SENARIO_SE_FLY");

    if (!isAppearStarEndAll()) {
        return;
    }

    s32 scenarioNo;
    bool needsAppearComet;

    if (!MR::isScenarioDecided()) {
        needsAppearComet = false;
    } else {
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
    } else {
        setNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvAppear::sInstance);
    }
}

void ScenarioSelectLayout::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::setPaneAnimRate(this, "ScenarioSelect", 1.0f, 0);
        setAnimRateAllNewPane(1.0f);
    }

    MR::startSystemLevelSE("SE_DM_LV_SENARIO_SEL_FLY");
    MR::setNerveAtPaneAnimStopped(this, "ScenarioSelect", &NrvScenarioSelectLayout::ScenarioSelectLayoutNrvWaitScenarioSelect::sInstance, 0);
}

void ScenarioSelectLayout::exeWaitScenarioSelect() {
    if (MR::isFirstStep(this)) {
        startAnimAllNewPane("NewWait");
        mBackButton->appear();
    }

    updateSelectedScenario();
    updateScenarioText();
    MR::startSystemLevelSE("SE_DM_LV_SENARIO_SEL_FLY");

    if (tryCancel()) {
        return;
    }

    if (trySelect()) {
        _28 = true;
        MR::startSystemSE("SE_SY_DECIDE_1");
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
            } else {
                mStar[i]->notSelect();
            }
        }

        startAnimAllNewPane("End");
    }

    updateScenarioText();
    MR::startSystemLevelSE("SE_DM_LV_SENARIO_SEL_FLY");
    MR::setNerveAtStep(this, &NrvScenarioSelectLayout::ScenarioSelectLayoutNrvAfterScenarioSelected::sInstance, ::cDecideFrame);
}

void ScenarioSelectLayout::exeAfterScenarioSelected() {
    MR::startPaneAnimAtStep(this, "ScenarioFrame", "End", ::cSelectedEndAnimStartStep, 0);
    updateScenarioText();
    MR::startSystemLevelSE("SE_DM_LV_SENARIO_SEL_FLY");

    f32 rate = MR::calcNerveEaseInValue(this, ::cSelectedAccelStartStep, ::cSelectedAccelFrame, ::cSelectedEffectRateMin, ::cSelectedEffectRateMax);
    f32 directionalSpeed =
        MR::calcNerveEaseInValue(this, ::cSelectedAccelStartStep, ::cSelectedAccelFrame, ::cSelectedEffectSpeedMin, ::cSelectedEffectSpeedMax);

    MR::setEffectRate(this, "ScenarioSelectEffect", rate);
    MR::setEffectDirectionalSpeed(this, "ScenarioSelectEffect", directionalSpeed);

    if (MR::isStep(this, ::cSelectedFadeOutStartStep)) {
        MR::stopStageBGM(::cSelectedFadeOutFrame);
        MR::closeSystemWipeWhiteFade(::cSelectedFadeOutFrame);
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
        MR::openSystemWipeWhiteFade(::cSelectedFadeInFrame);
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
        MR::closeSystemWipeFadeWithCaptureScreen(::cCancelFadeOutFrame);
        MR::stopStageBGM(1);
        setNerve(&NrvScenarioSelectLayout::ScenarioSelectLayoutNrvCancelFadeOut::sInstance);
    }
}

void ScenarioSelectLayout::exeAppearCometWarning() {
    if (MR::isStep(this, ::cCometWarningWaitFrame)) {
        int id = MR::getEncounterGalaxyCometNameId(MR::getCurrentStageName());
        const char* cometName = MR::getCometNameFromId(id);
        char cometMsg[256];
        MR::makeCometMessageID(cometMsg, sizeof(cometMsg), cometName);
        MR::setTextBoxGameMessageRecursive(this, "TextComet", "GalaxyInfo_CometAppearR");
        MR::setTextBoxArgGameMessageRecursive(this, "TextComet", cometMsg, 0);
        MR::showPaneRecursive(this, "CometAppear");
        MR::startPaneAnim(this, "CometAppear", "CometAppear", 0);
        MR::setCometPaneAnimFromId(this, "CometAppear", id, 1);
        MR::startSystemSE("SE_SY_COMET_WARNING_DISP");
    }

    if (MR::isGreaterStep(this, ::cCometWarningWaitFrame)) {
        MR::setNerveAtPaneAnimStopped(this, "CometAppear", &NrvScenarioSelectLayout::ScenarioSelectLayoutNrvWaitCometWarning::sInstance, 0);
    }
}

void ScenarioSelectLayout::exeWaitCometWarning() {
    if (getNerveStep() % 80 == 25) {
        MR::startSystemSE("SE_SY_COMET_WARNING");
    }

    MR::startPaneAnimAtFirstStep(this, "CometAppear", "CometWait", 0);
    MR::setNerveAtStep(this, &NrvScenarioSelectLayout::ScenarioSelectLayoutNrvDisappearCometWarning::sInstance, ::cCometWarningFrame);
}
