#include "Game/Screen/PauseMenu.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/ButtonPaneController.hpp"
#include "Game/Screen/LuigiLetter.hpp"
#include "Game/Screen/SysInfoWindow.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/System/GameSequenceFunction.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SequenceUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    const s32 cBackSequenceFadeFrame = 90;

    bool isInvalidBackAstroDome() {
        return !MR::isOnGameEventFlagUseAstroDome() || (MR::isStageAstroLocation() && !MR::isEqualStageName("HeavensDoorGalaxy"));
    }

    bool isStageHideScenarioTitle() {
        return MR::isStageBeginPrologueEvent() || MR::isStageAstroLocation();
    }
}  // namespace

namespace NrvPauseMenu {
    NEW_NERVE(PauseMenuNrvSelecting, PauseMenu, Selecting);
    NEW_NERVE(PauseMenuNrvDecided, PauseMenu, Decided);
    NEW_NERVE(PauseMenuNrvDisappear, PauseMenu, Disappear);
    NEW_NERVE(PauseMenuNrvConfirm, PauseMenu, Confirm);
    NEW_NERVE(PauseMenuNrvGameDataSave, PauseMenu, GameDataSave);
    NEW_NERVE(PauseMenuNrvLuigiLetter, PauseMenu, LuigiLetter);
}  // namespace NrvPauseMenu

PauseMenu::PauseMenu()
    : LayoutActor("ポーズメニュー", true), _20(), _24(), _28(0.0f, 50.0f), _30(), mLuigiLetter(), _38(), mStageTitleOffset(0.0f, 39.0f),
      mCoinNumPos(0.0f, 0.0f), mStarPieceNumPos(0.0f, 0.0f), _54(true) {
}

void PauseMenu::init(const JMapInfoIter& rIter) {
    initLayoutManager("PauseMenu", 1);
    MR::createAndAddPaneCtrl(this, "BG", 1);
    MR::createAndAddPaneCtrl(this, "PicStagebase", 1);
    MR::createAndAddPaneCtrl(this, "PicStagebase1", 1);
    MR::createAndAddPaneCtrl(this, "Counter", 1);
    MR::createAndAddPaneCtrl(this, "Stars", 2);
    MR::createAndAddPaneCtrl(this, "Coin", 1);
    MR::createAndAddPaneCtrl(this, "Piece", 1);
    MR::setFollowPos(&mCoinNumPos, this, "Coin");
    MR::setFollowTypeReplace(this, "Coin");
    MR::setFollowPos(&mStarPieceNumPos, this, "Piece");
    MR::setFollowTypeReplace(this, "Piece");

    if (::isStageHideScenarioTitle()) {
        const char* pPaneName = "ShaStageTitle";

        MR::createAndAddPaneCtrl(this, pPaneName, 1);
        MR::setFollowPos(&mStageTitleOffset, this, pPaneName);
        MR::setFollowTypeAdd(this, pPaneName);
    }

    MR::connectToSceneLayoutOnPause(this);

    _20 = new ButtonPaneController(this, "NBackUp", "BoxButton1", 0, true);
    _20->_22 = false;

    _24 = new ButtonPaneController(this, "NBackDown", "BoxButton2", 0, true);
    _24->_22 = false;

    MR::setTextBoxGameMessageRecursive(this, "NBackDown", ::isInvalidBackAstroDome() ? "PauseMenu_ButtonEndGame" : "PauseMenu_ButtonBackAstroDome");

    if (MR::isEqualStageName("AstroDome")) {
        MR::setTextBoxMessageRecursive(this, "ShaStageTitle", MR::getCurrentScenarioNameOnCurrentLanguage());
    } else {
        MR::setTextBoxMessageRecursive(this, "ShaStageTitle", MR::getCurrentGalaxyNameOnCurrentLanguage());
    }

    if (::isStageHideScenarioTitle()) {
        MR::hidePaneRecursive(this, "ScenarioTitle");
    } else {
        MR::setTextBoxMessageRecursive(this, "ScenarioTitle", MR::getCurrentScenarioNameOnCurrentLanguage());
    }

    updateStarPane();

    _30 = MR::createSysInfoWindow();

    MR::connectToSceneLayoutOnPause(_30);

    if (MR::getLuigiLetterGalaxyName()) {
        mLuigiLetter = MR::createLuigiLetterForMenu();
        mLuigiLetter->initWithoutIter();

        _38 = new ButtonPaneController(this, "LetterButton", "BoxButton3", 0, true);
        _38->_22 = false;
    } else {
        MR::hidePaneRecursive(this, "LetterButton");
    }

    initNerve(GET_NERVE(PauseMenu, PauseMenuNrvSelecting));
}

void PauseMenu::appear() {
    s32 coinNum = MR::getCoinNum();
    s32 starPieceNum = MR::getStarPieceNum();

    _54 = MR::isActiveDefaultGameLayout();

    if (_54) {
        MR::deactivateDefaultGameLayout();
    }

    MR::setTextBoxNumberRecursive(this, "ShaStarNum", MR::getPowerStarNum());
    MR::setTextBoxNumberRecursive(this, "ShaMarioNum", MR::getPlayerLeft());
    MR::setTextBoxNumberRecursive(this, "ShaCoinNum", coinNum);
    MR::setTextBoxNumberRecursive(this, "ShaPieceNum", starPieceNum);
    MR::setTextBoxGameMessageRecursive(this, "ShaMario", "PlayerLeft");

    wchar_t starPieceMessage[3];

    starPieceMessage[0] = MR::isStageAstroLocation() ? 0x57 : 0x3B;
    starPieceMessage[1] = 0x40;
    starPieceMessage[2] = '\0';

    MR::setTextBoxMessageRecursive(this, "ShaPiece", starPieceMessage);
    startPaneAnimWithoutButton("Appear");
    _20->appear();

    if (_24 != nullptr) {
        _24->appear();
    }

    if (MR::isOnLuigiHiding()) {
        _38->appear();
    }

    setNerve(GET_NERVE(PauseMenu, PauseMenuNrvSelecting));
    LayoutActor::appear();

    const char* pCoinPanePos = "CPosition1";
    const char* pPiecePanePos = "PPosition1";

    if (coinNum >= 100) {
        pCoinPanePos = "CPosition3";
    } else if (coinNum >= 10) {
        pCoinPanePos = "CPosition2";
    }

    if (starPieceNum >= 1000) {
        pPiecePanePos = "PPosition4";
    } else if (starPieceNum >= 100) {
        pPiecePanePos = "PPosition3";
    } else if (starPieceNum >= 10) {
        pPiecePanePos = "PPosition2";
    }

    MR::copyPaneTrans(&mCoinNumPos, this, pCoinPanePos);
    MR::copyPaneTrans(&mStarPieceNumPos, this, pPiecePanePos);
}

void PauseMenu::draw() const {
    LayoutActor::draw();

    if (mLuigiLetter != nullptr) {
        mLuigiLetter->draw();
    }
}

void PauseMenu::kill() {
    LayoutActor::kill();

    if (_54) {
        MR::activateDefaultGameLayout();
    }
}

void PauseMenu::control() {
    _20->update();

    if (_24 != nullptr) {
        _24->update();
    } else {
        _28.x = 0.0f;
        _28.y = 50.0f;
    }

    if (_38 != nullptr) {
        _38->update();
    }

    mStageTitleOffset.y = 39.0f;
}

void PauseMenu::updateStarPane() {
    GalaxyStatusAccessor accessor = MR::makeCurrentGalaxyStatusAccessor();
    const char* starPaneNames[7] = {"ShaStarA", "ShaStarB", "ShaStarC", "ShaStarD", "ShaStarE", "ShaStarF", "ShaStarG"};
    const char* starPicPaneNames[7] = {"PicStarA", "PicStarB", "PicStarC", "PicStarD", "PicStarE", "PicStarF", "PicStarG"};

    for (s32 i = 0; i < ARRAY_SIZEU(starPaneNames); i++) {
        MR::hidePaneRecursive(this, starPaneNames[i]);
    }

    if (::isStageHideScenarioTitle()) {
        return;
    }

    s32 extraStarCount = 0;
    bool isInvalidAstroDome = !MR::isOnGameEventFlagUseAstroDome();

    for (s32 i = 0; i < ARRAY_SIZEU(starPaneNames); i++) {
        if (i < accessor.getPowerStarNum()) {
            if (MR::hasPowerStarInCurrentStage(i + 1)) {
                if (i < accessor.getNormalScenarioNum()) {
                    MR::showPaneRecursive(this, starPaneNames[i]);
                } else {
                    MR::showPaneRecursive(this, starPaneNames[accessor.getNormalScenarioNum() + extraStarCount]);
                    extraStarCount++;
                }
            } else if (i < accessor.getNormalScenarioNum()) {
                MR::showPaneRecursive(this, starPaneNames[i]);
                MR::hidePaneRecursive(this, starPicPaneNames[i]);
            }
        }

        if (isInvalidAstroDome) {
            break;
        }
    }

    MR::startPaneAnim(this, "Stars", "Star", 1);

    f32 frame = isInvalidAstroDome ? 0.0f : accessor.getNormalScenarioNum() + extraStarCount - 1;
    MR::setPaneAnimFrameAndStop(this, "Stars", frame, 1);
}

void PauseMenu::startPaneAnimWithoutButton(const char* pAnimName) {
    MR::startPaneAnim(this, "BG", pAnimName, 0);
    MR::startPaneAnim(this, "PicStagebase", pAnimName, 0);
    MR::startPaneAnim(this, "PicStagebase1", pAnimName, 0);
    MR::startPaneAnim(this, "Stars", pAnimName, 0);
    MR::startPaneAnim(this, "Counter", pAnimName, 0);
}

bool PauseMenu::isPaneAnimStoppedWithoutButton() const {
    return MR::isPaneAnimStopped(this, "BG", 0) && MR::isPaneAnimStopped(this, "PicStagebase", 0) &&
           MR::isPaneAnimStopped(this, "PicStagebase1", 0) && MR::isPaneAnimStopped(this, "Stars", 0) && MR::isPaneAnimStopped(this, "Counter", 0);
}

void PauseMenu::forceToWaitAllButton() {
    _20->forceToWait();

    if (_24 != nullptr) {
        _24->forceToWait();
    }

    bool b = _38 == nullptr || _38->isHidden();

    if (!b) {
        _38->forceToWait();
    }
}

void PauseMenu::exeSelecting() {
    bool isPointingTrigger = _20->isPointingTrigger() || (_24 != nullptr && _24->isPointingTrigger()) ||
                             (!(_38 == nullptr || _38->isHidden()) && _38->isPointingTrigger());

    if (isPointingTrigger) {
        MR::startSystemSE("SE_SY_SELECT_PAUSE_ITEM", -1, -1);
    }

    bool isLetterHidden;

    if (_20->trySelect() || (_24 != nullptr && _24->trySelect()) || (!(isLetterHidden = (_38 == nullptr || _38->isHidden())) && _38->trySelect())) {
        setNerve(GET_NERVE(PauseMenu, PauseMenuNrvDecided));
        return;
    }

    bool isAnimStopped = isPaneAnimStoppedWithoutButton() && !_20->isAppearing() && (_24 == nullptr || !_24->isAppearing()) &&
                         ((_38 == nullptr || _38->isHidden()) || !_38->isAppearing());

    if (isAnimStopped && (MR::testCorePadTriggerPlus(0) || MR::testCorePadTriggerMinus(0))) {
        MR::startSystemSE("SE_SY_PAUSE_OFF", -1, -1);
        MR::startCSSound("CS_CLICK_CLOSE", nullptr, 0);
        setNerve(GET_NERVE(PauseMenu, PauseMenuNrvDisappear));
    }
}

void PauseMenu::exeDecided() {
    if (_20->mIsSelected) {
        if (_20->isTimingForSelectedSe()) {
            MR::startSystemSE("SE_SY_PAUSE_OFF", -1, -1);
            MR::startCSSound("CS_CLICK_CLOSE", nullptr, 0);
        }

        if (_20->isDecidedWait()) {
            setNerve(GET_NERVE(PauseMenu, PauseMenuNrvDisappear));
        }
    } else if (_24 != nullptr && _24->mIsSelected) {
        if (_24->isTimingForSelectedSe()) {
            MR::startSystemSE("SE_SY_PAUSE_OFF", -1, -1);
            MR::startCSSound("CS_CLICK_CLOSE", nullptr, 0);
        }

        if (_24->isDecidedWait()) {
            if (::isInvalidBackAstroDome()) {
                setNerve(GET_NERVE(PauseMenu, PauseMenuNrvGameDataSave));
            } else {
                setNerve(GET_NERVE(PauseMenu, PauseMenuNrvConfirm));
            }
        }
    } else {
        bool isLetterHidden = _38 == nullptr || _38->isHidden();

        if (!isLetterHidden && _38->mIsSelected) {
            if (_38->isTimingForSelectedSe()) {
                MR::startSystemSE("SE_SY_FILE_SEL_UPPER_DECIDE", -1, -1);
                MR::startCSSound("CS_CLICK_CLOSE", nullptr, 0);
            }

            if (_38->isDecidedWait()) {
                setNerve(GET_NERVE(PauseMenu, PauseMenuNrvLuigiLetter));
            }
        }
    }
}

void PauseMenu::exeDisappear() {
    if (MR::isFirstStep(this)) {
        startPaneAnimWithoutButton("End");
        _20->disappear();

        if (_24 != nullptr) {
            _24->disappear();
        }

        bool b = _38 == nullptr || _38->isHidden();

        if (!b) {
            _38->disappear();
        }
    }

    if (isPaneAnimStoppedWithoutButton() && _20->isHidden() && (_24 == nullptr || _24->isHidden())) {
        bool b = _38 == nullptr || _38->isHidden();

        if (b) {
            kill();
        }
    }
}

void PauseMenu::exeConfirm() {
    if (MR::isFirstStep(this)) {
        const char* pMessageId = ::isInvalidBackAstroDome() ? "PauseMenu_ConfirmEndGame" : "PauseMenu_ConfirmBackAstroDome";

        _30->appear(pMessageId, SysInfoWindow::Type_YesNo, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
    }

    if (MR::isDead(_30)) {
        if (_30->isSelectedYes()) {
            MR::closeSystemWipeFadeWithCaptureScreen(::cBackSequenceFadeFrame);

            if (::isInvalidBackAstroDome()) {
                MR::requestChangeSceneTitle();
            } else {
                MR::requestChangeStageGoBackAstroDome();
            }
        } else {
            forceToWaitAllButton();
            setNerve(GET_NERVE(PauseMenu, PauseMenuNrvSelecting));
        }
    }
}

void PauseMenu::exeGameDataSave() {
    if (MR::isFirstStep(this)) {
        GameSequenceFunction::startGameDataSaveSequence(false, true);
    }

    if (GameSequenceFunction::isActiveSaveDataHandleSequence()) {
        return;
    }

    if (GameSequenceFunction::isSuccessSaveDataHandleSequence()) {
        setNerve(GET_NERVE(PauseMenu, PauseMenuNrvConfirm));
    } else {
        forceToWaitAllButton();
        setNerve(GET_NERVE(PauseMenu, PauseMenuNrvSelecting));
    }
}

void PauseMenu::exeLuigiLetter() {
    if (MR::isFirstStep(this)) {
        mLuigiLetter->appear();
    }

    if (MR::isDead(mLuigiLetter)) {
        forceToWaitAllButton();
        setNerve(GET_NERVE(PauseMenu, PauseMenuNrvSelecting));
    }
}
