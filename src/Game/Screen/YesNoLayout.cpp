#include "Game/Screen/YesNoController.hpp"
#include "Game/Screen/YesNoLayout.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace NrvYesNoLayout {
    NEW_NERVE(YesNoLayoutNrvSelecting, YesNoLayout, Selecting);
};

YesNoLayout::YesNoLayout() :
    SimpleLayout("Yes/Noレイアウト", "SelectButton", 1, 68),
    mController(nullptr),
    _24(false)
{
    
}

void YesNoLayout::init(const JMapInfoIter& rIter) {
    MR::createAndAddPaneCtrl(this, "Left", 1);
    MR::createAndAddPaneCtrl(this, "Right", 1);
    initEffectKeeper(0, nullptr, nullptr);

    mController = new YesNoController(this);

    MR::registerDemoSimpleCastAll(this);
    initNerve(&NrvYesNoLayout::YesNoLayoutNrvSelecting::sInstance);
}

void YesNoLayout::appear() {
    LayoutActor::appear();
    mController->appear();

    _24 = false;

    MR::startStarPointerModeChooseYesNo(this);
    setNerve(&NrvYesNoLayout::YesNoLayoutNrvSelecting::sInstance);
}

void YesNoLayout::kill() {
    MR::endStarPointerMode(this);
    LayoutActor::kill();
}

void YesNoLayout::setInformationMessage(const char* pMessage) {
    _24 = true;

    MR::appearInformationMessage(pMessage, false);
}

void YesNoLayout::setSelectMessage(const char* pRightSelectMessage, const char* pLeftSelectMessage) {
    MR::setTextBoxGameMessageRecursive(this, "Right", pRightSelectMessage);
    MR::setTextBoxGameMessageRecursive(this, "Left", pLeftSelectMessage);
}

void YesNoLayout::control() {
    mController->update();
}

void YesNoLayout::exeSelecting() {
    if (_24 && mController->isDisappearStart()) {
        MR::disappearInformationMessage();
    }

    if (!mController->_C) {
        kill();
    }
}
