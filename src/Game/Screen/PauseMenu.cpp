#include "Game/Screen/PauseMenu.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/MessageUtil.hpp"

namespace {
    bool isInvalidBackAstroDome() {
        return (!MR::isOnGameEventFlagUseAstroDome() || MR::isStageAstroLocation() && !MR::isEqualStageName("HeavensDoorGalaxy"));
    }
};

PauseMenu::PauseMenu() : LayoutActor("ポーズメニュー", 1) {
    _20 = 0;
    _24 = 0;
    _28.x = 0.0f;
    _28.y = 50.0f;
    _30 = 0;
    _34 = 0;
    _38 = 0;
    _3C.x = 0.0f;
    _3C.y = 39.0f;
    _44.x = 0.0f;
    _44.y = 0.0f;
    _4C.x = 0.0f;
    _4C.y = 0.0f;
    _54 = true;
}

void PauseMenu::init(const JMapInfoIter& rIter) {
    initLayoutManager("PauseMenu", 1);
    MR::createAndAddPaneCtrl(this, "BG", 1);
    MR::createAndAddPaneCtrl(this, "PicStagebase", 1);
    MR::createAndAddPaneCtrl(this, "PicStagebase1", 1);
    MR::createAndAddPaneCtrl(this, "Counter", 1);
    MR::createAndAddPaneCtrl(this, "Stars", 1);
    MR::createAndAddPaneCtrl(this, "Coin", 1);
    MR::createAndAddPaneCtrl(this, "Piece", 1);
    MR::setFollowPos(&_44, this, "Coin");
    MR::setFollowTypeReplace(this, "Coin");
    MR::setFollowPos(&_4C, this, "Piece");
    MR::setFollowTypeReplace(this, "Piece");

    if (MR::isStageBeginPrologueEvent() || MR::isStageAstroLocation()) {
        MR::createAndAddPaneCtrl(this, "ShaStageTitle", 1);
        MR::setFollowPos(&_3C, this, "ShaStageTitle");
        MR::setFollowTypeReplace(this, "ShaStageTitle");
    }

    MR::connectToSceneLayoutOnPause(this);

    _20 = new ButtonPaneController(this, "NBackUp", "BoxButton1", 0, true);
    _20->_22 = false;

    _24 = new ButtonPaneController(this, "NBackDown", "BoxButton2", 0, true);
    _24->_22 = false;

    MR::setTextBoxGameMessageRecursive(this, "NBackDown", ::isInvalidBackAstroDome() ? "PauseMenu_ButtonEndGame" : "PauseMenu_ButtonBackAstroDome");

    if (MR::isEqualStageName("AstroDome"))
        MR::setTextBoxMessageRecursive(this, "ShaStageTitle", MR::getCurrentScenarioNameOnCurrentLanguage());
    else
        MR::setTextBoxMessageRecursive(this, "ShaStageTitle", MR::getCurrentGalaxyNameOnCurrentLanguage());
        
    if (MR::isStageBeginPrologueEvent() || MR::isStageAstroLocation())
        MR::hidePaneRecursive(this, "ScenarioTitle");
    else
        MR::setTextBoxMessageRecursive(this, "ScenarioTitle", MR::getCurrentScenarioNameOnCurrentLanguage());

    updateStarPane();
}
void PauseMenu::appear() {}
void PauseMenu::draw() const {}
void PauseMenu::kill() {}
void PauseMenu::control() {}
