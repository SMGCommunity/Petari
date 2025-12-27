#include "Game/AudioLib/AudSystem.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/BatteryInfo.hpp"
#include "Game/Screen/ErrorMessageWindow.hpp"
#include "Game/System/GameSystemErrorWatcher.hpp"
#include "Game/System/WPad.hpp"
#include "Game/System/WPadHolder.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/TriggerChecker.hpp"
#include <revolution/wpad.h>

#define WII_REMOTE_STATUS_OK 0
#define WII_REMOTE_STATUS_DISCONNECT 1
#define WII_REMOTE_STATUS_NO_BATTERY 2
#define WII_REMOTE_STATUS_UNPLAGGED 3

namespace {
    const GameSystemErrorWatcher::Message cMessageFatalError = {
        "DISC_04",
        ErrorMessageWindow::MessageType_System,
    };
    const GameSystemErrorWatcher::Message cMessageNoDisk = {
        "DISC_01",
        ErrorMessageWindow::MessageType_System,
    };
    const GameSystemErrorWatcher::Message cMessageWrongDisk = {
        "DISC_02",
        ErrorMessageWindow::MessageType_System,
    };
    const GameSystemErrorWatcher::Message cMessageRetry = {
        "DISC_03",
        ErrorMessageWindow::MessageType_System,
    };
    const GameSystemErrorWatcher::Message cMessageUnplagged = {
        "CONT_01",
        ErrorMessageWindow::MessageType_Game,
    };
    const GameSystemErrorWatcher::Message cMessageDisconnect = {
        "CONT_10",
        ErrorMessageWindow::MessageType_Game,
    };
    const GameSystemErrorWatcher::Message cMessageNoBattery = {
        "CONT_10_2",
        ErrorMessageWindow::MessageType_Game,
    };

    static const s32 sCounterMaxIgnoreCheckFreeStyle = 90;
    static const s32 sCounterMaxDecideDisconnect = 120;
};  // namespace

namespace {
    NEW_NERVE(GameSystemErrorWatcherNoError, GameSystemErrorWatcher, NoError);
    NEW_NERVE(GameSystemErrorWatcherErrorWindowIn, GameSystemErrorWatcher, ErrorWindowIn);
    NEW_NERVE(GameSystemErrorWatcherErrorWindowDisplay, GameSystemErrorWatcher, ErrorWindowDisplay);
    NEW_NERVE(GameSystemErrorWatcherErrorWindowOut, GameSystemErrorWatcher, ErrorWindowOut);
};  // namespace

GameSystemErrorWatcher::GameSystemErrorWatcher() :
    NerveExecutor("ErrorWatcher"),
    mWindow(nullptr),
    mMessage(nullptr),
    mUnplaggedTexMap(nullptr),
    mBatteryLayout(nullptr),
    mDriveStatus(DVD_STATE_END),
    _1C(nullptr),
    mWiiRemoteStatus(WII_REMOTE_STATUS_OK),
    mCounterIgnoreCheckFreeStyle(0),
    mCounterDecideDisconnect(sCounterMaxDecideDisconnect),
    mWiiRemoteBattery(WPAD_BATTERY_LEVEL_MAX),
    mPermissionUpdateWiiRemoteStatus(false)
{
    mWindow = new ErrorMessageWindow();
    mWindow->initWithoutIter();

    _1C = new TriggerChecker();
    _1C->setInput(false);

    initNerve(&GameSystemErrorWatcherNoError::sInstance);
}

void GameSystemErrorWatcher::initAfterResourceLoaded() {
    mBatteryLayout = new BatteryLayout();
    mBatteryLayout->initWithoutIter();

    mUnplaggedTexMap = MR::createLytTexMap("ErrorMessageImage.arc", "FreeStyleUnplagged.bti");
}

void GameSystemErrorWatcher::movement() {
    mWindow->movement();
    mWindow->calcAnim();

    mDriveStatus = DVDGetDriveStatus();

    updateWiiRemoteStatus();
    updateNerve();

    if (mPermissionUpdateWiiRemoteStatus && mBatteryLayout != nullptr) {
        mBatteryLayout->movement();
        mBatteryLayout->calcAnim();
    }
}

void GameSystemErrorWatcher::draw() const {
    if (mPermissionUpdateWiiRemoteStatus && mBatteryLayout != nullptr) {
        mBatteryLayout->draw();
    }

    mWindow->draw();
}

bool GameSystemErrorWatcher::isWarning() const {
    return !isNerve(&GameSystemErrorWatcherNoError::sInstance);
}

bool GameSystemErrorWatcher::setPermissionUpdateWiiRemoteStatus(bool permission) {
    bool permissionPrev = mPermissionUpdateWiiRemoteStatus;

    mPermissionUpdateWiiRemoteStatus = permission;

    if (permission) {
        mCounterDecideDisconnect = 0;

        if (mBatteryLayout != nullptr && MR::isDead(mBatteryLayout)) {
            mBatteryLayout->appear();
        }
    }
    else {
        mWiiRemoteStatus = WII_REMOTE_STATUS_OK;

        if (mBatteryLayout != nullptr && !MR::isDead(mBatteryLayout)) {
            mBatteryLayout->kill();
        }
    }

    return permissionPrev;
}

void GameSystemErrorWatcher::exeNoError() {
    const Message* pMessage = getProperMessageId();

    if (pMessage != nullptr) {
        mMessage = pMessage;

        setNerve(&GameSystemErrorWatcherErrorWindowIn::sInstance);

        if (AudWrap::getSystem() != nullptr) {
            AudWrap::getSystem()->doDvdErrorProcess();
        }
    }
}

void GameSystemErrorWatcher::exeErrorWindowIn() {
    if (MR::isFirstStep(this)) {
        mWindow->appearWithMessage(mMessage->mId, mMessage->mType, getProperErrorTexMap());
    }

    if (mWindow->isAnimEnd()) {
        setNerve(&GameSystemErrorWatcherErrorWindowDisplay::sInstance);
    }

    MR::requestStarPointerModeErrorWindow(this);
}

void GameSystemErrorWatcher::exeErrorWindowDisplay() {
    const Message* pMessage = getProperMessageId();

    if (pMessage != nullptr && pMessage != mMessage) {
        mMessage = pMessage;

        setNerve(&GameSystemErrorWatcherErrorWindowIn::sInstance);
    }

    if (pMessage == nullptr) {
        setNerve(&GameSystemErrorWatcherErrorWindowOut::sInstance);
        mWindow->disappear();

        mMessage = nullptr;
    }

    MR::requestStarPointerModeErrorWindow(this);
}

void GameSystemErrorWatcher::exeErrorWindowOut() {
    if (MR::isDead(mWindow)) {
        const Message* pMessage = getProperMessageId();

        if (pMessage != nullptr) {
            mMessage = pMessage;

            setNerve(&GameSystemErrorWatcherErrorWindowIn::sInstance);
        }
        else {
            if (AudWrap::getSystem() != nullptr) {
                AudWrap::getSystem()->exitDvdErrorProcess();
            }

            setNerve(&GameSystemErrorWatcherNoError::sInstance);
        }
    } else {
        MR::requestStarPointerModeErrorWindow(this);
    }
}

void GameSystemErrorWatcher::updateWiiRemoteStatus() {
    bool isWaitDecideDisconnect;
    bool isOk;

    if (!mPermissionUpdateWiiRemoteStatus) {
        mWiiRemoteStatus = WII_REMOTE_STATUS_OK;
    } else {
        isOk = true;

        _1C->update(MR::getWPad(WPAD_CHAN0)->mIsConnected);

        if (MR::getWPad(WPAD_CHAN0)->mIsConnected) {
            mWiiRemoteBattery = MR::getWPad(WPAD_CHAN0)->getBattery();
        }

        if (_1C->getLevel()) {
            mCounterDecideDisconnect = sCounterMaxDecideDisconnect;
        }
        else if (mCounterDecideDisconnect != 0) {
            mCounterDecideDisconnect--;
        }

        isWaitDecideDisconnect = mCounterDecideDisconnect != 0;

        if (_1C->getOnTrigger()) {
            mCounterIgnoreCheckFreeStyle = sCounterMaxIgnoreCheckFreeStyle;
        }

        if (mCounterIgnoreCheckFreeStyle != 0) {
            mCounterIgnoreCheckFreeStyle--;
        }

        if (mCounterIgnoreCheckFreeStyle == 0) {
            if (_1C->getLevel() && !MR::getWPad(WPAD_CHAN0)->mIsSubPadConnected) {
                mWiiRemoteStatus = WII_REMOTE_STATUS_UNPLAGGED;
                isOk = false;
            }

            if (!isWaitDecideDisconnect) {
                if (mWiiRemoteBattery <= WPAD_BATTERY_LEVEL_LOW) {
                    mWiiRemoteStatus = WII_REMOTE_STATUS_NO_BATTERY;
                }
                else {
                    mWiiRemoteStatus = WII_REMOTE_STATUS_DISCONNECT;
                }

                isOk = false;
            }

            if (isOk) {
                mWiiRemoteStatus = WII_REMOTE_STATUS_OK;
            }
        }
    }
}

const GameSystemErrorWatcher::Message* GameSystemErrorWatcher::getProperMessageId() const {
    switch (mDriveStatus) {
    case DVD_STATE_FATAL_ERROR:
        return &cMessageFatalError;
    case DVD_STATE_NO_DISK:
        return &cMessageNoDisk;
    case DVD_STATE_WRONG_DISK:
        return &cMessageWrongDisk;
    case DVD_STATE_RETRY:
        return &cMessageRetry;
    }

    switch (mWiiRemoteStatus) {
    case WII_REMOTE_STATUS_UNPLAGGED:
        return &cMessageUnplagged;
    case WII_REMOTE_STATUS_DISCONNECT:
        return &cMessageDisconnect;
    case WII_REMOTE_STATUS_NO_BATTERY:
        return &cMessageNoBattery;
    default:
        return nullptr;
    }
}

const nw4r::lyt::TexMap* GameSystemErrorWatcher::getProperErrorTexMap() const {
    switch (mDriveStatus) {
    case DVD_STATE_FATAL_ERROR:
        return nullptr;
    case DVD_STATE_NO_DISK:
        return nullptr;
    case DVD_STATE_WRONG_DISK:
        return nullptr;
    case DVD_STATE_RETRY:
        return nullptr;
    }

    switch (mWiiRemoteStatus) {
    case WII_REMOTE_STATUS_UNPLAGGED:
        return mUnplaggedTexMap;
    default:
        return nullptr;
    }
}
