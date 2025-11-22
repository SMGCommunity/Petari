#include "Game/Screen/GalaxyMapTicoIcon.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include <cstdio>

GalaxyMapTicoIcon::GalaxyMapTicoIcon(int exclamationGalaxyIndex, const char* pGalaxyName, LayoutActor* pHost, const char* pPaneName)
    : LayoutActor(pGalaxyName, true),
      mExclamationGalaxyIndex(exclamationGalaxyIndex),
      mGalaxyName(pGalaxyName),
      mHost(pHost),
      mPaneName(nullptr),
      mMode(0) {
    u32 paneNameLen = strlen(pPaneName) + 1;

    mPaneName = new char[paneNameLen];

    snprintf(mPaneName, paneNameLen, "%s", pPaneName);
}

void GalaxyMapTicoIcon::init(const JMapInfoIter&) {
    initLayoutManager("MapTicoIcon", 3);
    MR::startAnim(this, "Appear", 0);
}

void GalaxyMapTicoIcon::appear() {
    LayoutActor::appear();
    MR::startAnim(this, "Wait", 0);
    syncStatus();
}

void GalaxyMapTicoIcon::syncStatus() {
    f32 starPieceRate = static_cast<f32>(MR::getStarPieceNumGivingToTicoGalaxy(mExclamationGalaxyIndex)) / MR::getStarPieceNumMaxGivingToTicoGalaxy(mExclamationGalaxyIndex);

    if (!MR::isAppearGalaxy(mGalaxyName) || 1.0f <= starPieceRate) {
        MR::hideLayout(this);
    } else {
        MR::startAnim(this, "TicoMeter", 2);
        MR::setAnimFrameAndStop(this, starPieceRate * MR::getAnimFrameMax(this, 2), 2);

        if (isModeDiscoverAndIsAppear()) {
            MR::startAnim(this, "SelectIn", 0);
            MR::setAnimFrameAndStopAtEnd(this, 0);
            MR::startAnim(this, "Blink", 1);
        } else {
            MR::startAnim(this, "Wait", 0);
            MR::startAnim(this, "Blink", 1);
            MR::setAnimFrameAndStop(this, 0.0f, 1);
        }
    }
}

void GalaxyMapTicoIcon::setModeNormal() {
    mMode = 0;

    syncStatus();
}

void GalaxyMapTicoIcon::setModeDiscover() {
    mMode = 1;

    syncStatus();
}

void GalaxyMapTicoIcon::control() {
    if (MR::isHiddenLayout(this)) {
        return;
    }

    MR::setLayoutPosAtPaneTrans(this, mHost, mPaneName);
    MR::setLayoutScaleAtPaneScale(this, mHost, mPaneName);
}

bool GalaxyMapTicoIcon::isModeDiscoverAndIsAppear() const {
    if (mMode == 0) {
        return false;
    }

    return GameDataFunction::isOnJustAppearGalaxy(mGalaxyName);
}
