#include "Game/Screen/YesNoController.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/ButtonPaneController.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const char* sDefaultCursorSE = "SE_SY_TALK_FOCUS_ITEM";
    static const char* sDefaultYesSE = "SE_SY_TALK_SELECT_YES";
    static const char* sDefaultNoSE = "SE_SY_TALK_SELECT_NO";
};  // namespace

namespace NrvYesNoController {
    NEW_NERVE(YesNoControllerNrvSelecting, YesNoController, Selecting);
    NEW_NERVE(YesNoControllerNrvDecided, YesNoController, Decided);
    NEW_NERVE(YesNoControllerNrvDisappear, YesNoController, Disappear);
    NEW_NERVE(YesNoControllerNrvSelected, YesNoController, Selected);
    NEW_NERVE(YesNoControllerNrvNotSelected, YesNoController, NotSelected);
};  // namespace NrvYesNoController

YesNoController::YesNoController(LayoutActor* pHost)
    : NerveExecutor("はい／いいえ選択制御"), mHost(pHost), _C(false), mButtonYesPaneCtrl(nullptr), mButtonNoPaneCtrl(nullptr), mCursorSE(nullptr),
      mYesSE(nullptr), mNoSE(nullptr) {
    mButtonYesPaneCtrl = new ButtonPaneController(mHost, "Right", "BoxRight", 0, true);
    mButtonYesPaneCtrl->_22 = false;

    mButtonNoPaneCtrl = new ButtonPaneController(mHost, "Left", "BoxLeft", 0, true);
    mButtonNoPaneCtrl->_22 = false;

    initNerve(&NrvYesNoController::YesNoControllerNrvSelecting::sInstance);
}

void YesNoController::appear() {
    _C = true;

    setNerve(&NrvYesNoController::YesNoControllerNrvSelecting::sInstance);
}

void YesNoController::kill() {
    _C = false;

    setNerve(&NrvYesNoController::YesNoControllerNrvNotSelected::sInstance);
}

void YesNoController::update() {
    if (_C) {
        updateNerve();
        mButtonYesPaneCtrl->update();
        mButtonNoPaneCtrl->update();
    }
}

bool YesNoController::isSelected() const {
    return isNerve(&NrvYesNoController::YesNoControllerNrvSelected::sInstance);
}

bool YesNoController::isSelectedYes() const {
    return mButtonYesPaneCtrl->mIsSelected;
}

bool YesNoController::isDisappearStart() const {
    return isNerve(&NrvYesNoController::YesNoControllerNrvDisappear::sInstance) && MR::isFirstStep(this);
}

void YesNoController::setSE(const char* pCursorSE, const char* pYesSE, const char* pNoSE) {
    mCursorSE = pCursorSE;
    mYesSE = pYesSE;
    mNoSE = pNoSE;
}

bool YesNoController::trySelect() {
    if (mButtonYesPaneCtrl->trySelect()) {
        return true;
    }

    return mButtonNoPaneCtrl->trySelect();
}

void YesNoController::emitEffectIfExist(const char* pEffectName) {
    if (MR::isExistEffectKeeper(mHost)) {
        MR::emitEffect(mHost, pEffectName);
    }
}

void YesNoController::deleteEffectIfExist(const char* pEffectName) {
    if (MR::isExistEffectKeeper(mHost)) {
        MR::deleteEffect(mHost, pEffectName);
    }
}

void YesNoController::forceDeleteEffectAllIfExist() {
    if (MR::isExistEffectKeeper(mHost)) {
        MR::forceDeleteEffect(mHost, "LeftText");
        MR::forceDeleteEffect(mHost, "RightText");
    }
}

void YesNoController::exeSelecting() {
    if (MR::isFirstStep(this)) {
        mButtonYesPaneCtrl->appear();
        mButtonNoPaneCtrl->appear();
    }

    if (mButtonYesPaneCtrl->isPointingTrigger()) {
        if (mCursorSE != nullptr) {
            MR::startSystemSE(mCursorSE, -1, -1);
        } else {
            MR::startSystemSE(sDefaultCursorSE, -1, -1);
        }

        forceDeleteEffectAllIfExist();
        emitEffectIfExist("RightText");
    }

    if (mButtonNoPaneCtrl->isPointingTrigger()) {
        if (mCursorSE != nullptr) {
            MR::startSystemSE(mCursorSE, -1, -1);
        } else {
            MR::startSystemSE(sDefaultCursorSE, -1, -1);
        }

        forceDeleteEffectAllIfExist();
        emitEffectIfExist("LeftText");
    }

    if (!mButtonYesPaneCtrl->isPointing()) {
        deleteEffectIfExist("RightText");
    }

    if (!mButtonNoPaneCtrl->isPointing()) {
        deleteEffectIfExist("LeftText");
    }

    if (trySelect()) {
        setNerve(&NrvYesNoController::YesNoControllerNrvDecided::sInstance);
    }
}

// FIXME: GPR30 and GPR31 are swapped.
void YesNoController::exeDecided() {
    bool isSelectedYes = mButtonYesPaneCtrl->mIsSelected;

    if (MR::isFirstStep(this)) {
        deleteEffectIfExist("LeftText");
        deleteEffectIfExist("RightText");
        MR::startCSSound("CS_CLICK_CLOSE", nullptr, 0);

        if (isSelectedYes) {
            if (mYesSE != nullptr) {
                MR::startSystemSE(mYesSE, -1, -1);
            } else {
                MR::startSystemSE(sDefaultYesSE, -1, -1);
            }
        } else {
            if (mNoSE != nullptr) {
                MR::startSystemSE(mNoSE, -1, -1);
            } else {
                MR::startSystemSE(sDefaultNoSE, -1, -1);
            }
        }
    }

    if ((isSelectedYes && mButtonYesPaneCtrl->isDecidedWait()) || (!isSelectedYes && mButtonNoPaneCtrl->isDecidedWait())) {
        setNerve(&NrvYesNoController::YesNoControllerNrvDisappear::sInstance);
    }
}

void YesNoController::exeDisappear() {
    if (MR::isFirstStep(this)) {
        mButtonYesPaneCtrl->disappear();
        mButtonNoPaneCtrl->disappear();
    }

    if (mButtonYesPaneCtrl->isHidden() && mButtonNoPaneCtrl->isHidden()) {
        if (mButtonYesPaneCtrl->mIsSelected || mButtonNoPaneCtrl->mIsSelected) {
            setNerve(&NrvYesNoController::YesNoControllerNrvSelected::sInstance);
        } else {
            setNerve(&NrvYesNoController::YesNoControllerNrvNotSelected::sInstance);
        }
    }
}

void YesNoController::exeSelected() {
    _C = false;
}

void YesNoController::exeNotSelected() {
    _C = false;
}
