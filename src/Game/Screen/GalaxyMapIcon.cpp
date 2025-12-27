#include "Game/Screen/GalaxyMapIcon.hpp"
#include "Game/Screen/ButtonPaneController.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

#define ICON_STATUS_CLOSED 0
#define ICON_STATUS_NEW 1
#define ICON_STATUS_OPENED 2
#define ICON_STATUS_COMPLETED 3
#define ICON_STATUS_UNK 4

#define MODE_NORMAL 0
#define MODE_NEW_GALAXY_DISCOVER 1

GalaxyMapIcon::GalaxyMapIcon(const char* pName, LayoutActor* pHost, const char* pPaneName)
    : LayoutActor(pName, true), mButtonPaneController(nullptr), mGalaxyName(pName), mHost(pHost), mPaneName(pPaneName), mMode(MODE_NORMAL) {}

void GalaxyMapIcon::appear() {
    LayoutActor::appear();
    mButtonPaneController->appear();
    syncStatus();
}

void GalaxyMapIcon::calcAnim() {
    MR::setLayoutScalePosAtPaneScaleTransIfExecCalcAnim(this, mHost, mPaneName);
    LayoutActor::calcAnim();
}

void GalaxyMapIcon::calcAnimForCapture(const nw4r::lyt::DrawInfo& rDrawInfo) {
    MR::setLayoutScalePosAtPaneScaleTransIfExecCalcAnim(this, mHost, mPaneName);
    MR::calcAnimLayoutWithDrawInfo(this, rDrawInfo);
}

void GalaxyMapIcon::setModeNormal() {
    mMode = MODE_NORMAL;

    syncStatus();
}

void GalaxyMapIcon::setModeNewGalaxyDiscover() {
    mMode = MODE_NEW_GALAXY_DISCOVER;

    syncStatus();
}

bool GalaxyMapIcon::isStatusNew() const {
    s32 iconStatus = examineIconStatus();

    return iconStatus == ICON_STATUS_NEW || iconStatus == ICON_STATUS_UNK;
}

void GalaxyMapIcon::onPointing() {
    mButtonPaneController->onPointing();
}

void GalaxyMapIcon::activatePointing() {
    mButtonPaneController->_24 = true;
}

void GalaxyMapIcon::deactivatePointing() {
    mButtonPaneController->_24 = false;
}

void GalaxyMapIcon::control() {
    if (MR::isHiddenLayout(this)) {
        return;
    }

    if (mMode != MODE_NORMAL) {
        return;
    }

    mButtonPaneController->update();
}

s32 GalaxyMapIcon::examineIconStatus() const {
    if (!MR::isAppearGalaxy(mGalaxyName)) {
        return ICON_STATUS_CLOSED;
    }

    GalaxyStatusAccessor accessor = MR::makeGalaxyStatusAccessor(mGalaxyName);

    if (accessor.isCompleted()) {
        return ICON_STATUS_COMPLETED;
    }

    if (accessor.isOpened()) {
        return ICON_STATUS_OPENED;
    }

    return accessor.canOpen();
}

bool GalaxyMapIcon::isBlink(int iconStatus) const {
    if (mMode != MODE_NORMAL) {
        return GameDataFunction::canOnJustGameEventFlag(mGalaxyName);
    }

    if (iconStatus == ICON_STATUS_NEW || iconStatus == ICON_STATUS_UNK) {
        return true;
    }

    return false;
}

bool GalaxyMapIcon::isNewOnDiscoverMode() const {
    if (mMode == MODE_NORMAL) {
        return false;
    }

    return GameDataFunction::canOnJustGameEventFlag(mGalaxyName);
}

void GalaxyMapIcon::init(const JMapInfoIter& rIter) {
    initLayoutManager("MapGalaxyIcon", 2);
    MR::createAndAddPaneCtrl(this, "GalaxyIcon", 1);

    mButtonPaneController = new ButtonPaneController(this, "GalaxyIcon", "GalaxyIcon", 0, true);
    mButtonPaneController->invalidateAppearance();
    mButtonPaneController->invalidateDecide();
    mButtonPaneController->_23 = true;
}

bool GalaxyMapIcon::isPointing() const {
    if (MR::isDead(this)) {
        return false;
    }

    if (MR::isHiddenLayout(this)) {
        return false;
    }

    TVec2f paneScale;

    MR::copyPaneScale(&paneScale, this, nullptr);

    if (MR::isNearZero(paneScale)) {
        return false;
    }

    return MR::isStarPointerPointingPane(this, "GalaxyIcon", 0, true, "弱");
}

void GalaxyMapIcon::syncStatus() {
    s32 iconStatus = examineIconStatus();

    if (iconStatus < ICON_STATUS_CLOSED || iconStatus == ICON_STATUS_CLOSED) {
        MR::hideLayout(this);
    } else {
        MR::showLayout(this);
        MR::startAnim(this, "Status", 0);
        MR::setAnimFrameAndStop(this, iconStatus, 0);

        if (isBlink(iconStatus)) {
            MR::startAnim(this, "NewBlink", 1);
        } else {
            MR::startAnim(this, "NewWait", 1);
        }

        if (isNewOnDiscoverMode()) {
            MR::startPaneAnim(this, "GalaxyIcon", "ButtonSelectIn", 0);
            MR::setPaneAnimFrameAndStopAtEnd(this, "GalaxyIcon", 0);
        }
    }
}
