#include "Game/LiveActor/Nerve.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/IconAButton.hpp"
#include "Game/Screen/InformationMessage.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace NrvInformationMessage {
    NEW_NERVE(InformationMessageNrvAppear, InformationMessage, Appear);
    NEW_NERVE(InformationMessageNrvWait, InformationMessage, Wait);
    NEW_NERVE(InformationMessageNrvDisappear, InformationMessage, Disappear);
};

InformationMessage::InformationMessage() :
    LayoutActor("インフォメーションメッセージ", true),
    mAButtonIcon(NULL),
    mIsCenterPane(false)
{}

void InformationMessage::init(const JMapInfoIter& rIter) {
    initLayoutManager("InformationWindow", 2);
    MR::createAndAddPaneCtrl(this, "AButtonPosC", 1);
    MR::createAndAddPaneCtrl(this, "AButtonPosU", 1);
    MR::connectToSceneLayoutDecoration(this);

    mAButtonIcon = new IconAButton(true, false);
    mAButtonIcon->initWithoutIter();

    if (MR::isExistSceneObj(SceneObj_DemoDirector)) {
        MR::registerDemoSimpleCastAll(this);
        MR::registerDemoSimpleCastAll(mAButtonIcon);
    }

    initNerve(&NrvInformationMessage::InformationMessageNrvAppear::sInstance);
}

void InformationMessage::appear() {
    if (mIsCenterPane) {
        MR::hidePaneRecursive(this, "InfoWindowUp");
        MR::showPaneRecursive(this, "InfoWindowCenter");
    }
    else {
        MR::showPaneRecursive(this, "InfoWindowUp");
        MR::hidePaneRecursive(this, "InfoWindowCenter");
    }

    MR::startAnim(this, "Appear", 0);
    MR::startAnim(this, "Line", 1);
    MR::setAnimFrameAndStopAdjustTextHeight(this, mIsCenterPane ? "InfConfirmC" : "InfConfirmU", 1);
    setNerve(&NrvInformationMessage::InformationMessageNrvAppear::sInstance);
    LayoutActor::appear();
}

void InformationMessage::appearWithButtonLayout() {
    appear();
    mAButtonIcon->setFollowActorPane(this, mIsCenterPane ? "AButtonPosC" : "AButtonPosU");
    mAButtonIcon->openWithoutMessage();
    MR::requestMovementOn(mAButtonIcon);
}

void InformationMessage::disappear() {
    setNerve(&NrvInformationMessage::InformationMessageNrvDisappear::sInstance);
}

void InformationMessage::setMessage(const char* pMessage) {
    MR::setTextBoxGameMessageRecursive(this, NULL, pMessage);
}

void InformationMessage::setMessage(const wchar_t* pMessage) {
    MR::setTextBoxMessageRecursive(this, NULL, pMessage);
}

void InformationMessage::setReplaceString(const wchar_t* pMessage, s32 param2) {
    MR::setTextBoxArgStringRecursive(this, NULL, pMessage, param2);
}

void InformationMessage::exeAppear() {
    MR::setNerveAtAnimStopped(this, &NrvInformationMessage::InformationMessageNrvWait::sInstance, 0);
}

void InformationMessage::exeWait() {
    MR::startAnimAtFirstStep(this, "Wait", 0);
}

void InformationMessage::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "End", 0);

        if (!MR::isDead(mAButtonIcon)) {
            mAButtonIcon->term();
        }
    }

    MR::killAtAnimStopped(this, 0);
}

InformationMessage::~InformationMessage() {
    
}
