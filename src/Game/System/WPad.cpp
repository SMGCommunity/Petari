#include "Game/Speaker/SpkSystem.hpp"
#include "Game/System/WPad.hpp"
#include "Game/System/WPadAcceleration.hpp"
#include "Game/System/WPadButton.hpp"
#include "Game/System/WPadHVSwing.hpp"
#include "Game/System/WPadHolder.hpp"
#include "Game/System/WPadInfoChecker.hpp"
#include "Game/System/WPadLeaveWatcher.hpp"
#include "Game/System/WPadPointer.hpp"
#include "Game/System/WPadRumble.hpp"
#include "Game/System/WPadStick.hpp"

WPad::WPad(s32 channel) :
    mChannel(channel),
    mReadInfo(nullptr),
    mButton(nullptr),
    mPointer(nullptr),
    mCorePadAccel(nullptr),
    mCorePadSwing(nullptr),
    _18(nullptr),
    _1C(nullptr),
    mStick(nullptr),
    mSubPadAccel(nullptr),
    mSubPadSwing(nullptr),
    mLeaveWatcher(nullptr),
    mInfoChecker(nullptr),
    _34(true),
    mIsConnected(false),
    mIsSubPadConnected(false)
{
    mButton = new WPadButton(this);
    mPointer = new WPadPointer(this);
    mCorePadAccel = new WPadAcceleration(this, WPAD_DEV_CORE);
    mCorePadSwing = new WPadHVSwing(this, WPAD_DEV_CORE);
    _18 = new WPadRumble(this);
    _1C = new WPadRumble(this);
    _18->registInstance();
    mSubPadAccel = new WPadAcceleration(this, WPAD_DEV_FREESTYLE);
    mSubPadSwing = new WPadHVSwing(this, WPAD_DEV_FREESTYLE);
    mStick = new WPadStick(this);
    mLeaveWatcher = new WPadLeaveWatcher(this);
    mInfoChecker = new WPadInfoChecker(this);
    mSubPadSwing->_8 = 2.0f;
}

void WPad::setReadInfo(WPadReadDataInfo* pReadInfo) {
    mReadInfo = pReadInfo;
}

void WPad::update() {
    if (mChannel == -1) {
        return;
    }

    _34 = false;

    mButton->update();
    mCorePadAccel->update();
    mPointer->update();
    mCorePadSwing->update();
    _18->update();
    _1C->update();
    mSubPadAccel->update();
    mSubPadSwing->update();
    mStick->update();
    mLeaveWatcher->update();
    mInfoChecker->update();

    _34 = true;
}

WPadRumble* WPad::getRumbleInstance() const {
    return _18->getRumbleInstance();
}

void WPad::startLeaveWatcher() {
    mLeaveWatcher->start();
}

void WPad::stopLeaveWatcher() {
    mLeaveWatcher->stop();
}

void WPad::restartLeaveWatcher() {
    mLeaveWatcher->restart();
}

s32 WPad::getBattery() const {
    return mInfoChecker->getBattery();
}

void WPad::resetPad() {
    
}

void WPad::connected() {
    mIsConnected = true;

    mInfoChecker->startCheck();
    WPADSetExtensionCallback(mChannel, extensionCallback);
    SpkSystem::connect(mChannel);
}

void WPad::disconnected() {
    mIsConnected = false;
    mIsSubPadConnected = false;

    mInfoChecker->reset();
    SpkSystem::disconnect(mChannel);
}

void WPad::connectCallback(s32 chan, s32 reason) {
    WPad* pWPad = MR::getWPad(chan);

    if (pWPad == nullptr) {
        return;
    }

    switch (reason) {
    case WPAD_ERR_NONE:
        pWPad->connected();
        break;
    case WPAD_ERR_NO_CONTROLLER:
        pWPad->disconnected();
        break;
    default:
        break;
    }
}

void WPad::extensionCallback(s32 chan, s32 result) {
    WPad* pWPad = MR::getWPad(chan);

    if (pWPad == nullptr) {
        return;
    }

    SpkSystem::extensionProcess(chan, result);

    switch (result) {
    case WPAD_DEV_CORE:
        pWPad->mIsSubPadConnected = false;
        break;
    case WPAD_DEV_FREESTYLE:
        pWPad->mIsSubPadConnected = true;
        break;
    case WPAD_DEV_UNKNOWN:
        break;
    }
}

void WPad::getInfoCallback(s32 chan, s32 result) {
    if (result != WPAD_ERR_NONE) {
        return;
    }

    MR::getWPad(chan)->mInfoChecker->successGetInfo();
}

namespace MR {
    bool isDeviceFreeStyle(const KPADStatus* pStatus) {
        return pStatus != nullptr
            && pStatus->wpad_err == WPAD_ERR_NONE
            && pStatus->dev_type == WPAD_DEV_FREESTYLE;
    }

    // getPadDataForExceptionNoInit
};

KPADStatus* WPad::getKPadStatus(u32 index) const {
    return mReadInfo->getKPadStatus(index);
}

s32 WPad::getValidStatusCount() const {
    return mReadInfo->getValidStatusCount();
}

/*
s32 WPad::getEnableAccelPastCount(u32 deviceType) const {
    WPadAcceleration* pAcceleration;

    if (deviceType == WPAD_DEV_CORE) {
        pAcceleration = mCorePadAccel;
    }
    else {
        pAcceleration = mSubPadAccel;
    }

    return pAcceleration->_628;
}
*/

bool WPad::getAcceleration(TVec3f* pAccel, u32 deviceType) const {
    WPadAcceleration* pAcceleration;

    if (deviceType == WPAD_DEV_CORE) {
        pAcceleration = mCorePadAccel;
    }
    else {
        pAcceleration = mSubPadAccel;
    }

    return pAcceleration->getAcceleration(pAccel);
}

bool WPad::getPastAcceleration(TVec3f* pAccel, s32 param2, u32 deviceType) const {
    WPadAcceleration* pAcceleration;

    if (deviceType == WPAD_DEV_CORE) {
        pAcceleration = mCorePadAccel;
    }
    else {
        pAcceleration = mSubPadAccel;
    }

    return pAcceleration->getPastAcceleration(pAccel, param2);
}
