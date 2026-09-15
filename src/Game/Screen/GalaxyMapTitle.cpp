#include "Game/Screen/GalaxyMapTitle.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/LayoutUtil.hpp"

namespace {
    NEW_NERVE(GalaxyMapTitleHide, GalaxyMapTitle, Hide);
    NEW_NERVE(GalaxyMapTitleDisplay, GalaxyMapTitle, Display);
    NEW_NERVE(GalaxyMapTitleDisappear, GalaxyMapTitle, Disappear);
    NEW_NERVE(GalaxyMapTitleAppearToAnother, GalaxyMapTitle, AppearToAnother);
    NEW_NERVE(GalaxyMapTitleDisappearToAnother, GalaxyMapTitle, DisappearToAnother);
};  // namespace

GalaxyMapTitle::GalaxyMapTitle() : LayoutActor("MapTitle", true), mMessageID(nullptr) {
}

void GalaxyMapTitle::init(const JMapInfoIter& rIter) {
    initLayoutManager("MapTitle", 1);
    initNerve(GET_NERVE_GLOBAL(GalaxyMapTitleHide));
    kill();
}

void GalaxyMapTitle::appear() {
    LayoutActor::appear();

    mMessageID = nullptr;
}

void GalaxyMapTitle::setModeNormal() {
    MR::showPane(this, "Guidance");
}

void GalaxyMapTitle::setModeHideInformation() {
    MR::hidePane(this, "Guidance");
}

void GalaxyMapTitle::startAstroMap() {
    appear();

    mMessageID = "GalaxyMap_AstroMap";

    setNerve(GET_NERVE_GLOBAL(GalaxyMapTitleAppearToAnother));
}

void GalaxyMapTitle::startGalaxyMap() {
    appear();

    mMessageID = "GalaxyMap_GrandGalaxyMap";

    setNerve(GET_NERVE_GLOBAL(GalaxyMapTitleAppearToAnother));
}

void GalaxyMapTitle::changeToAstroMap() {
    mMessageID = "GalaxyMap_AstroMap";

    setNerve(GET_NERVE_GLOBAL(GalaxyMapTitleDisappearToAnother));
}

void GalaxyMapTitle::changeToGalaxyMap() {
    mMessageID = "GalaxyMap_GrandGalaxyMap";

    setNerve(GET_NERVE_GLOBAL(GalaxyMapTitleDisappearToAnother));
}

void GalaxyMapTitle::exeHide() {
    kill();
}

void GalaxyMapTitle::exeDisplay() {
    MR::startAnimAtFirstStep(this, "TitleWait", 0);
}

void GalaxyMapTitle::exeDisappear() {
    if (tryDisappear()) {
        kill();
    }
}

void GalaxyMapTitle::exeAppearToAnother() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "TitleAppear", 0);
        MR::setTextBoxGameMessageRecursive(this, "TextTitle", mMessageID);
    }

    MR::setNerveAtAnimStopped(this, GET_NERVE_GLOBAL(GalaxyMapTitleDisplay), 0);
}

void GalaxyMapTitle::exeDisappearToAnother() {
    if (tryDisappear()) {
        setNerve(GET_NERVE_GLOBAL(GalaxyMapTitleAppearToAnother));
    }
}

bool GalaxyMapTitle::tryDisappear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "TitleEnd", 0);
    }

    if (MR::isAnimStopped(this, 0)) {
        return true;
    }

    return false;
}
