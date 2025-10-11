#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/IconAButton.hpp"
#include "Game/Screen/SysInfoWindow.hpp"
#include "Game/Screen/YesNoController.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <cstdio>

namespace {
    const MessageChangeFuncTableEntry cMessageChangeFuncTable[] = {
        { MR::setTextBoxSystemMessageRecursive },
        { MR::setTextBoxGameMessageRecursive },
    };
};

namespace NrvSysInfoWindow {
    NEW_NERVE(SysInfoWindowNrvAppear, SysInfoWindow, Appear);
    NEW_NERVE(SysInfoWindowNrvWait, SysInfoWindow, Wait);
    NEW_NERVE(SysInfoWindowNrvDisappear, SysInfoWindow, Disappear);
};

SysInfoWindow::SysInfoWindow(SysInfoWindowType windowType, SysInfoExecuteType executeType) :
    LayoutActor("システム用インフォメーションウィンドウ", true),
    mWindowType(windowType),
    mType(Type_AButton),
    mYesNoSelector(nullptr),
    mIconAButton(nullptr),
    mTextParentPaneName(nullptr),
    mWindowParentPaneName(nullptr),
    _38(executeType == ExecuteType_Children)
{
    switch (windowType) {
    case WindowType_Normal:
        mTextParentPaneName = "InfoWindow";
        mWindowParentPaneName = "WinInfoWindow";
        break;
    case WindowType_Mini:
        mTextParentPaneName = "InfoWindowM";
        mWindowParentPaneName = "WinInfoWindowM";
        break;
    }
}

void SysInfoWindow::init(const JMapInfoIter& rIter) {
    initLayoutManager(getLayoutName(), 1);
    MR::createAndAddPaneCtrl(this, mTextParentPaneName, 1);
    MR::createAndAddPaneCtrl(this, mWindowParentPaneName, 1);

    if (mWindowType == WindowType_Normal) {
        MR::createAndAddPaneCtrl(this, "Left", 1);
        MR::createAndAddPaneCtrl(this, "Right", 1);
        mYesNoSelector = new YesNoController(this);
        MR::setTextBoxGameMessageRecursive(this, "Left", "System_Save00_No");
        MR::setTextBoxGameMessageRecursive(this, "Right", "System_Save00_Yes");
    }

    mIconAButton = MR::createAndSetupIconAButton(this, !_38, false);

    initNerve(&NrvSysInfoWindow::SysInfoWindowNrvAppear::sInstance);
}

void SysInfoWindow::movement() {
    LayoutActor::movement();

    if (_38) {
        mIconAButton->movement();
    }
}

void SysInfoWindow::calcAnim() {
    LayoutActor::calcAnim();

    if (_38) {
        mIconAButton->calcAnim();
    }
}

void SysInfoWindow::draw() const {
    LayoutActor::draw();

    if (_38) {
        mIconAButton->draw();
    }
}

void SysInfoWindow::appear() {
    const char* pAnimName = "ButtonAppear";

    MR::startPaneAnim(this, mTextParentPaneName, pAnimName, 0);
    MR::startPaneAnim(this, mWindowParentPaneName, pAnimName, 0);
    setNerve(&NrvSysInfoWindow::SysInfoWindowNrvAppear::sInstance);
    LayoutActor::appear();
}

void SysInfoWindow::appear(const char* pMessageId, SysInfoType type, SysInfoTextPos textPos, SysInfoMessageType messageType) {
    mType = type;

    switch (type) {
    case Type_AButton:
    case Type_Blocking:
        if (mWindowType != WindowType_Mini) {
            MR::hidePaneRecursive(this, "Left");
            MR::hidePaneRecursive(this, "Right");
        }
        break;
    case Type_YesNo:
        MR::showPaneRecursive(this, "Left");
        MR::showPaneRecursive(this, "Right");
        break;
    }

    switch (textPos) {
    case TextPos_Center:
        MR::setTextBoxVerticalPositionCenterRecursive(this, mTextParentPaneName);
        break;
    case TextPos_Bottom:
        MR::setTextBoxVerticalPositionBottomRecursive(this, mTextParentPaneName);
        break;
    }

    MessageChangeFunc* pMessageChangeFunc = cMessageChangeFuncTable[messageType].mFuncPtr;

    pMessageChangeFunc(this, mTextParentPaneName, pMessageId);

    if (type == Type_YesNo) {
        char yesMessageId[48];
        char noMessageId[48];
        snprintf(yesMessageId, sizeof(yesMessageId), "%s_Yes", pMessageId);
        snprintf(noMessageId, sizeof(noMessageId), "%s_No", pMessageId);
        pMessageChangeFunc(this, "Right", yesMessageId);
        pMessageChangeFunc(this, "Left", noMessageId);
    }

    appear();

    switch (type) {
    case Type_AButton:
        mIconAButton->openWithoutMessage();
        break;
    case Type_Blocking:
        break;
    case Type_YesNo:
        mYesNoSelector->appear();
        break;
    }
}

void SysInfoWindow::disappear() {
    setNerve(&NrvSysInfoWindow::SysInfoWindowNrvDisappear::sInstance);
}

void SysInfoWindow::kill() {
    LayoutActor::kill();
    mIconAButton->kill();
}

void SysInfoWindow::forceKill() {
    kill();

    if (mYesNoSelector != nullptr) {
        mYesNoSelector->kill();
    }
}

bool SysInfoWindow::isWait() const {
    return isNerve(&NrvSysInfoWindow::SysInfoWindowNrvWait::sInstance);
}

bool SysInfoWindow::isSelectedYes() const {
    return mYesNoSelector->isSelectedYes();
}

bool SysInfoWindow::isDisappear() const {
    return isNerve(&NrvSysInfoWindow::SysInfoWindowNrvDisappear::sInstance);
}

void SysInfoWindow::control() {
    if (mYesNoSelector != nullptr) {
        mYesNoSelector->update();
    }
}

const char* SysInfoWindow::getLayoutName() const {
    switch (mWindowType) {
    case WindowType_Normal:
        return "SysInfoWindow";
        break;
    case WindowType_Mini:
        return "SysInfoWindowMini";
        break;
    }

    return nullptr;
}

void SysInfoWindow::exeAppear() {
    MR::setNerveAtPaneAnimStopped(this, mTextParentPaneName, &NrvSysInfoWindow::SysInfoWindowNrvWait::sInstance, 0);
}

void SysInfoWindow::exeWait() {
    if (MR::isFirstStep(this)) {
        const char* pAnimName = "ButtonWait";

        MR::startPaneAnim(this, mTextParentPaneName, pAnimName, 0);
        MR::startPaneAnim(this, mWindowParentPaneName, pAnimName, 0);
    }

    if (mType == Type_AButton) {
        if (MR::testSystemPadTriggerDecide()) {
            MR::startSystemSE("SE_SY_TALK_OK", -1, -1);
            MR::startCSSound("CS_CLICK_CLOSE", 0, 0);
            setNerve(&NrvSysInfoWindow::SysInfoWindowNrvDisappear::sInstance);
        }
    }
    else if (mYesNoSelector != nullptr) {
        if (mYesNoSelector->isDisappearStart()) {
            setNerve(&NrvSysInfoWindow::SysInfoWindowNrvDisappear::sInstance);
        }
    }
};

void SysInfoWindow::exeDisappear() {
    if (MR::isFirstStep(this)) {
        const char* pAnimName = "ButtonEnd";

        MR::startPaneAnim(this, mTextParentPaneName, pAnimName, 0);
        MR::startPaneAnim(this, mWindowParentPaneName, pAnimName, 0);

        if (mType == Type_AButton) {
            mIconAButton->term();
        }
    }

    if (MR::isPaneAnimStopped(this, mTextParentPaneName, 0)) {
        if (mType == Type_AButton
            || mType == Type_Blocking
            || mYesNoSelector != nullptr && !mYesNoSelector->_C)
        {
            kill();
        }
    }
}

void SysInfoWindow::setYesNoSelectorSE(const char* pCursorSE, const char* pYesSE, const char* pNoSE) {
    if (mYesNoSelector != nullptr) {
        mYesNoSelector->setSE(pCursorSE, pYesSE, pNoSE);
    }
}

void SysInfoWindow::resetYesNoSelectorSE() {
    if (mYesNoSelector != nullptr) {
        mYesNoSelector->setSE(nullptr, nullptr, nullptr);
    }
}

void SysInfoWindow::setTextBoxArgNumber(s32 arg, s32 param2) {
    MR::setTextBoxArgNumberRecursive(this, mTextParentPaneName, arg, param2);
}

void SysInfoWindow::setTextBoxArgString(const wchar_t* pArg, s32 param2) {
    MR::setTextBoxArgStringRecursive(this, mTextParentPaneName, pArg, param2);
}

namespace MR {
    SysInfoWindow* createSysInfoWindow() {
        SysInfoWindow* pWindow = new SysInfoWindow(
            SysInfoWindow::WindowType_Normal,
            SysInfoWindow::ExecuteType_Normal);

        pWindow->initWithoutIter();

        return pWindow;
    }

    SysInfoWindow* createSysInfoWindowExecuteWithChildren() {
        SysInfoWindow* pWindow = new SysInfoWindow(
            SysInfoWindow::WindowType_Normal,
            SysInfoWindow::ExecuteType_Children);

        pWindow->initWithoutIter();

        return pWindow;
    }

    SysInfoWindow* createSysInfoWindowMiniExecuteWithChildren() {
        SysInfoWindow* pWindow = new SysInfoWindow(
            SysInfoWindow::WindowType_Mini,
            SysInfoWindow::ExecuteType_Children);

        pWindow->initWithoutIter();

        return pWindow;
    }
};
