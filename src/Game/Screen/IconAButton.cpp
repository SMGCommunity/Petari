#include "Game/Screen/IconAButton.hpp"
#include "Game/Util.hpp"
#include <cstdio>

IconAButton::IconAButton(bool connectToScene, bool connectToPause) : LayoutActor("Aボタンアイコン", true) {
    mFollowPos.x = 0.0f;
    mFollowPos.y = 0.0f;
    mFollowActor = nullptr;

    if (connectToScene) {
        if (connectToPause) {
            MR::connectToSceneLayoutOnPause(this);
        }
        else {
            MR::connectToSceneTalkLayout(this);
        }
    }

    mFollowPane[0] = 0;
}

void IconAButton::init(const JMapInfoIter &rIter) {
    initLayoutManager("IconAButton", 1);
    initNerve(&NrvIconAButton::IconAButtonNrvOpen::sInstance);
    MR::startAnim(this, "Appear", 0);
    MR::setAnimFrameAndStop(this, 0.0f, 0);
    kill();
}

void IconAButton::setFollowActorPane(LayoutActor *pActor, const char *pName) {
    mFollowActor = pActor;
    snprintf(mFollowPane, sizeof(mFollowPane), "%s", pName);
    MR::setFollowPos(&mFollowPos, this, nullptr);
}

bool IconAButton::isOpen() {
    return MR::isDead(this) == false;
}

bool IconAButton::isWait() {
    return isNerve(&NrvIconAButton::IconAButtonNrvWait::sInstance);
}

void IconAButton::openWithTalk() {
    appear();
    updateFollowPos();
    MR::showPane(this, "PicPlate");
    setNerve(&NrvIconAButton::IconAButtonNrvOpen::sInstance);
    MR::setTextBoxLayoutMessageRecursive(this, "IconAButton", "Layout_SystemTalk");
}

void IconAButton::openWithRead() {
    appear();
    updateFollowPos();
    MR::showPane(this, "PicPlate");
    setNerve(&NrvIconAButton::IconAButtonNrvOpen::sInstance);
    MR::setTextBoxLayoutMessageRecursive(this, "IconAButton", "Layout_SystemRead");
}

void IconAButton::openWithTurn() {
    appear();
    updateFollowPos();
    MR::showPane(this, "PicPlate");
    setNerve(&NrvIconAButton::IconAButtonNrvOpen::sInstance);
    MR::setTextBoxLayoutMessageRecursive(this, "IconAButton", "Layout_SystemTurn");
}

void IconAButton::openWithoutMessage() {
    appear();
    updateFollowPos();
    MR::showPane(this, "PicPlate");
    setNerve(&NrvIconAButton::IconAButtonNrvOpen::sInstance);
    MR::hidePane(this, "PicPlate");
    MR::clearTextBoxMessageRecursive(this, "IconAButton");
}

// IconAButton::term

void IconAButton::exeOpen() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Appear", 0);
    }

    if (MR::isAnimStopped(this, 0)) {
        setNerve(&NrvIconAButton::IconAButtonNrvWait::sInstance);
    }
}

void IconAButton::exeTerm() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "End", 0);
    }

    if (MR::isAnimStopped(this, 0)) {
        kill();
    }
}

void IconAButton::control() {
    updateFollowPos();
}

void IconAButton::updateFollowPos() {
    if (mFollowActor != nullptr) {
        MR::copyPaneTrans(&mFollowPos, mFollowActor, mFollowPane);
    }
}

IconAButton::~IconAButton() {

}

namespace NrvIconAButton {
    INIT_NERVE(IconAButtonNrvOpen);
    INIT_NERVE(IconAButtonNrvWait);
    INIT_NERVE(IconAButtonNrvTerm);
};
