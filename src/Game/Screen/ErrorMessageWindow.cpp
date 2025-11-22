#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/ErrorMessageWindow.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/SystemUtil.hpp"

namespace {
    NEW_NERVE(ErrorMessageWindowHide, ErrorMessageWindow, Hide);
    NEW_NERVE(ErrorMessageWindowAppear, ErrorMessageWindow, Appear);
    NEW_NERVE(ErrorMessageWindowDisplay, ErrorMessageWindow, Display);
    NEW_NERVE(ErrorMessageWindowDisappear, ErrorMessageWindow, Disappear);
    NEW_NERVE(ErrorMessageWindowDisappearBeforeAppear, ErrorMessageWindow, DisappearBeforeAppear);
}; // namespace

ErrorMessageWindow::ErrorMessageWindow()
    : LayoutActor("エラーメッセージ表示", false),
      mMessageId(nullptr),
      mTexture(nullptr) {
}

void ErrorMessageWindow::init(const JMapInfoIter& rIter) {
    initLayoutManagerNoConvertFilename("ErrorMessageWindow", 1);
    initNerve(&ErrorMessageWindowHide::sInstance);
    kill();
}

void ErrorMessageWindow::appear() {
    LayoutActor::appear();
    MR::hideLayout(this);
}

bool ErrorMessageWindow::isAnimEnd() const {
    return isNerve(&ErrorMessageWindowDisplay::sInstance) || isNerve(&ErrorMessageWindowHide::sInstance);
}

void ErrorMessageWindow::appearWithMessage(const char* pMessageId, MessageType messageType, const nw4r::lyt::TexMap* pTexture) {
    mMessageType = messageType;
    mMessageId = pMessageId;
    mTexture = pTexture;

    MR::setTextBoxFontRecursive(this, "ErrorTextHalf", MR::getFontOnCurrentLanguage());
    MR::setTextBoxFontRecursive(this, "ErrorMessage", MR::getFontOnCurrentLanguage());

    if (MR::isDead(this)) {
        appear();
        setNerve(&ErrorMessageWindowAppear::sInstance);
    } else {
        setNerve(&ErrorMessageWindowDisappearBeforeAppear::sInstance);
    }
}

void ErrorMessageWindow::disappear() {
    setNerve(&ErrorMessageWindowDisappear::sInstance);
}

void ErrorMessageWindow::exeHide() {
    if (MR::isFirstStep(this)) {
        kill();
    }
}

void ErrorMessageWindow::exeAppear() {
    if (MR::isFirstStep(this)) {
        const wchar_t* pMessage = nullptr;

        switch (mMessageType) {
        case MessageType_System:
            pMessage = MR::getSystemMessageDirect(mMessageId);
            break;
        case MessageType_Game:
            pMessage = MR::getGameMessageDirect(mMessageId);
            break;
        }

        if (mTexture != nullptr) {
            MR::showPane(this, "PicDummy");
            MR::showPane(this, "ErrorTextHalf");
            MR::hidePane(this, "ErrorMessage");
            MR::replacePaneTexture(this, "PicDummy", mTexture, 0);
            MR::setTextBoxMessageRecursive(this, "ErrorTextHalf", pMessage);
        } else {
            MR::hidePane(this, "PicDummy");
            MR::hidePane(this, "ErrorTextHalf");
            MR::showPane(this, "ErrorMessage");
            MR::setTextBoxMessageRecursive(this, "ErrorMessage", pMessage);
        }

        MR::showLayout(this);
        MR::startAnim(this, "Appear", 0);
    }

    MR::setNerveAtAnimStopped(this, &ErrorMessageWindowDisplay::sInstance, 0);
}

void ErrorMessageWindow::exeDisplay() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Wait", 0);
    }
}

void ErrorMessageWindow::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "End", 0);
    }

    MR::setNerveAtAnimStopped(this, &ErrorMessageWindowHide::sInstance, 0);
}

void ErrorMessageWindow::exeDisappearBeforeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "End", 0);
    }

    MR::setNerveAtAnimStopped(this, &ErrorMessageWindowAppear::sInstance, 0);
}
