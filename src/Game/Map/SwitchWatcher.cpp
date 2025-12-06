#include "Game/Map/SwitchWatcher.hpp"
#include "Game/Map/ActorAppearSwitchListener.hpp"
#include "Game/Map/StageSwitch.hpp"
#include "Game/Screen/MoviePlayerSimple.hpp"

SwitchWatcher::SwitchWatcher(const StageSwitchCtrl* pCtrl) : NameObj("スイッチ監視") {
    mFlags = 0;
    mSwitchCtrl = pCtrl;
    mSwitchAListener = nullptr;
    mSwitchBListener = nullptr;
    mSwitchApprListener = nullptr;
}

void SwitchWatcher::movement() {
    if (mSwitchAListener != nullptr) {
        checkSwitch(mSwitchAListener, 1, mSwitchCtrl->isOnSwitchA());
    }

    if (mSwitchBListener != nullptr) {
        checkSwitch(mSwitchBListener, 2, mSwitchCtrl->isOnSwitchB());
    }

    if (mSwitchApprListener != nullptr) {
        checkSwitch(mSwitchApprListener, 4, mSwitchCtrl->isOnSwitchAppear());
    }
}

void SwitchWatcher::checkSwitch(SwitchEventListener* pListener, u32 type, bool isOn) {
    if (isOn) {
        if ((mFlags & type) == 0) {
            pListener->listenSwitchOnEvent();
        }

        mFlags |= type;
    } else {
        if ((mFlags & type) != 0) {
            pListener->listenSwitchOffEvent();
        }

        mFlags &= ~type;
    }
}

bool SwitchWatcher::isSameSwitch(const StageSwitchCtrl* pCtrl) const {
    return mSwitchCtrl == pCtrl;
}

void SwitchWatcher::addSwitchListener(SwitchEventListener* pListener, u32 type) {
    switch (type) {
    case 1:
        mSwitchAListener = pListener;
        break;
    case 2:
        mSwitchBListener = pListener;
        break;
    case 4:
        mSwitchApprListener = pListener;
        break;
    }
}

SwitchWatcher::~SwitchWatcher() {}
