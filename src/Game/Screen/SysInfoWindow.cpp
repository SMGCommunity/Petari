#include "Game/Screen/SysInfoWindow.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <cstdio>

namespace {
    MessageChangeFuncTable cMessageChangeFuncTable[2] = {
        { MR::setTextBoxSystemMessageRecursive },
        { MR::setTextBoxGameMessageRecursive }
    };
};

SysInfoWindow::SysInfoWindow(SysInfoWindowType windowType, SysInfoExecuteType executeType) 
    : LayoutActor("システム用インフォメーションウィンドウ", true) {
    _20 = windowType;
    _24 = 0;
    _28 = 0;
    _2C = 0;
    _30 = nullptr;
    _34 = nullptr;
    _38 = (executeType - 1 == 0);

    switch (windowType) {
        case WINDOWTYPE_0:
            _30 = "InfoWindow";
            _34 = "WinInfoWindow";
        break;
        case WINDOWTYPE_1:
            _30 = "InfoWindowM";
            _34 = "WinInfoWindowM";
        break;
    }
}

void SysInfoWindow::init(const JMapInfoIter& rIter) {
    initLayoutManager(getLayoutName(), 1);
    MR::createAndAddPaneCtrl(this, _30, 1);
    MR::createAndAddPaneCtrl(this, _34, 1);
    if (!_20) {
        MR::createAndAddPaneCtrl(this, "Left", 1);
        MR::createAndAddPaneCtrl(this, "Right", 1);
        _28 = new YesNoController(this);
        MR::setTextBoxGameMessageRecursive(this, "Left", "System_Save00_No");
        MR::setTextBoxGameMessageRecursive(this, "Right", "System_Save00_Yes");
    }
    _2C = MR::createAndSetupIconAButton(this, (s8)(_38 == 0), false);
    initNerve(&NrvSysInfoWindow::SysInfoWindowNrvAppear::sInstance);
}

void SysInfoWindow::movement() {
    LayoutActor::movement();

    if (_38)
        _2C->movement();
}

void SysInfoWindow::calcAnim() {
    LayoutActor::calcAnim();

    if (_38)
        _2C->calcAnim();
}

void SysInfoWindow::draw() const {
    LayoutActor::draw();
    
    if (_38)
        _2C->draw();
}

void SysInfoWindow::appear() {
    const char* str = "ButtonAppear";
    MR::startPaneAnim(this, _30, str, 0);
    MR::startPaneAnim(this, _34, str, 0);
    setNerve(&NrvSysInfoWindow::SysInfoWindowNrvAppear::sInstance);
    LayoutActor::appear();
}

#ifdef NON_MATCHING
// Instruction swap when the function pointer table is accessed. Line 108
void SysInfoWindow::appear(const char* pStr, SysInfoType type, SysInfoTextPos textPos, SysInfoMessageType msgType) {
    _24 = type;

    switch (type) {
        case INFOTYPE_0:
        case INFOTYPE_1:
            if (_20 != 1) {
                MR::hidePaneRecursive(this, "Left");
                MR::hidePaneRecursive(this, "Right");
            }
        break;
        case INFOTYPE_2:
            MR::showPaneRecursive(this, "Left");
            MR::showPaneRecursive(this, "Right");
        break;
    }

    switch (textPos) {
        case TEXTPOS_0:
            MR::setTextBoxVerticalPositionCenterRecursive(this, _30);
        break;
        case TEXTPOS_1:
            MR::setTextBoxVerticalPositionBottomRecursive(this, _30);
        break;
    }

    MessageChangeFuncPtr funcptr = cMessageChangeFuncTable[msgType].mFuncPtr;
    funcptr(this, _30, pStr);

    if (type == INFOTYPE_2) {
        char bufYes[48];
        char bufNo[48];
        snprintf(bufYes, 48, "%s_Yes", pStr);
        snprintf(bufNo, 48, "%s_No", pStr);
        funcptr(this, "Right", bufYes);
        funcptr(this, "Left", bufNo);
    }

    appear();

    switch (type) {
        case INFOTYPE_0:
            _2C->openWithoutMessage();
        break;
        case INFOTYPE_1:
        break;
        case INFOTYPE_2:
            _28->appear();
        break;
    }
}
#endif

void SysInfoWindow::disappear() {
    setNerve(&NrvSysInfoWindow::SysInfoWindowNrvDisappear::sInstance);
}

void SysInfoWindow::kill() {
    LayoutActor::kill();
    _2C->kill();
}

void SysInfoWindow::forceKill() {
    kill();

    if (_28) 
        _28->kill();
}

bool SysInfoWindow::isWait() const {
    return isNerve(&NrvSysInfoWindow::SysInfoWindowNrvWait::sInstance);
}

bool SysInfoWindow::isSelectedYes() const {
    return _28->isSelectedYes();
}

bool SysInfoWindow::isDisappear() const {
    return isNerve(&NrvSysInfoWindow::SysInfoWindowNrvDisappear::sInstance);
}

void SysInfoWindow::control() {
    if (_28)
        _28->update();
}

const char* SysInfoWindow::getLayoutName() const {
    switch (_20) {
        case WINDOWTYPE_0:
            return "SysInfoWindow";
        break;
        case WINDOWTYPE_1:
            return "SysInfoWindowMini";
        break;
    }

    return nullptr;
}

void SysInfoWindow::exeWait() {
    if (MR::isFirstStep(this)) {
        const char* pStr = "ButtonWait";
        MR::startPaneAnim(this, _30, pStr, 0);
        MR::startPaneAnim(this, _34, pStr, 0);
    }

    if (_24 == 0) {
        if (MR::testSystemPadTriggerDecide()) {
            MR::startSystemSE("SE_SY_TALK_OK", -1, -1);
            MR::startCSSound("CS_CLICK_CLOSE", 0, 0);
            setNerve(&NrvSysInfoWindow::SysInfoWindowNrvDisappear::sInstance);
        }
    }
    else if (_28) {
        if (_28->isDisappearStart())
            setNerve(&NrvSysInfoWindow::SysInfoWindowNrvDisappear::sInstance);
    }
};

void SysInfoWindow::exeDisappear() {
    if (MR::isFirstStep(this)) {
        const char* pStr = "ButtonEnd";
        MR::startPaneAnim(this, _30, pStr, 0);
        MR::startPaneAnim(this, _34, pStr, 0);

        if (_24 == 0)
            _2C->term();
    }
    if (MR::isPaneAnimStopped(this, _30, 0)) {
        if (_24 == 0 || _24 == 1 || _28 != 0 && _28->_C == 0)
            kill();
    }
}

void SysInfoWindow::setYesNoSelectorSE(const char* pStr1, const char* pStr2, const char* pStr3) {
    if (_28 != 0)
        _28->setSE(pStr1, pStr2, pStr3);
}

void SysInfoWindow::resetYesNoSelectorSE() {
    if (_28 != 0)
        _28->setSE(0, 0, 0);
}

void SysInfoWindow::setTextBoxArgNumber(s32 num, s32 index) {
    MR::setTextBoxArgNumberRecursive(this, _30, num, index);
}

void SysInfoWindow::setTextBoxArgString(const wchar_t* pStr, s32 index) {
    MR::setTextBoxArgStringRecursive(this, _30, pStr, index);
}

SysInfoWindow::~SysInfoWindow() {

}

namespace MR {
    SysInfoWindow* createSysInfoWindow() {
        SysInfoWindow* pWindow = new SysInfoWindow(pWindow->WINDOWTYPE_0, pWindow->EXECUTETYPE_0);
        pWindow->initWithoutIter();
        return pWindow;
    }

    SysInfoWindow* createSysInfoWindowExecuteWithChildren() {
        SysInfoWindow* pWindow = new SysInfoWindow(pWindow->WINDOWTYPE_0, pWindow->EXECUTETYPE_1);
        pWindow->initWithoutIter();
        return pWindow;
    }

    SysInfoWindow* createSysInfoWindowMiniExecuteWithChildren() {
        SysInfoWindow* pWindow = new SysInfoWindow(pWindow->WINDOWTYPE_1, pWindow->EXECUTETYPE_1);
        pWindow->initWithoutIter();
        return pWindow;
    }
};

namespace NrvSysInfoWindow {
    void SysInfoWindowNrvAppear::execute(Spine* pSpine) const {
        SysInfoWindow* pHost = (SysInfoWindow*)pSpine->mExecutor;
        MR::setNerveAtPaneAnimStopped(pHost, pHost->_30, &NrvSysInfoWindow::SysInfoWindowNrvWait::sInstance, 0);
    }
    SysInfoWindowNrvAppear(SysInfoWindowNrvAppear::sInstance);
    
    INIT_NERVE(SysInfoWindowNrvWait);
    INIT_NERVE(SysInfoWindowNrvDisappear);

};