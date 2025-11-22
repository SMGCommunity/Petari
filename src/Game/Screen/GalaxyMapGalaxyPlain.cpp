#include "Game/Screen/GalaxyMapGalaxyPlain.hpp"
#include "Game/Screen/GalaxyNamePlate.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <JSystem/JGeometry/TBox.hpp>
#include <JSystem/JKernel/JKRSolidHeap.hpp>

#define GALAXY_NAME_BUFFER_SIZE 128

#ifdef __cplusplus
extern "C" {
extern int swprintf(wchar_t*, size_t, const wchar_t*, ...);
};
#endif

GalaxyMapGalaxyPlain::GalaxyMapGalaxyPlain(const LayoutActor* pHost)
    : LayoutActor("Galaxy情報簡易表示", true),
      mHost(pHost),
      mPaneName(nullptr),
      mNamePlate(nullptr),
      mGalaxyName(nullptr),
      _30(false) {
}

void GalaxyMapGalaxyPlain::show(const char* pGalaxyName, const char* pPaneName) {
    wchar_t*       pDstGalaxyName;
    const wchar_t* pShortGalaxyName;
    wchar_t*       pFontTagIter;

    appear();

    mPaneName = pPaneName;
    pDstGalaxyName = mGalaxyName;

    if (!MR::isOnGameEventFlagGalaxyOpen(pGalaxyName)) {
        swprintf(pDstGalaxyName, GALAXY_NAME_BUFFER_SIZE, L"?");
    } else {
        pShortGalaxyName = MR::getGalaxyNameShortOnCurrentLanguage(pGalaxyName);
        pFontTagIter = pDstGalaxyName + swprintf(pDstGalaxyName, GALAXY_NAME_BUFFER_SIZE, L"%ls ", pShortGalaxyName);

        if (MR::isStarComplete(pGalaxyName)) {
            MR::addPictureFontTag(pFontTagIter, 80);
        } else {
            pFontTagIter = MR::addPictureFontTag(pFontTagIter, 55);
            pFontTagIter = MR::addPictureFontTag(pFontTagIter, 64);

            MR::addNumberFontTag(pFontTagIter, MR::getPowerStarNumSucceed(pGalaxyName));
        }
    }

    _30 = true;
}

// GalaxyMapGalaxyPlain::adjustTransInScreen

void GalaxyMapGalaxyPlain::init(const JMapInfoIter& rIter) {
    mNamePlate = new GalaxyNamePlate(nullptr, false);
    mNamePlate->initWithoutIter();
    mNamePlate->setShowBalloonNozzle(false);

    mGalaxyName = new (MR::getSceneHeapGDDR3(), 0) wchar_t[GALAXY_NAME_BUFFER_SIZE];
}

void GalaxyMapGalaxyPlain::appear() {
    LayoutActor::appear();
}

void GalaxyMapGalaxyPlain::kill() {
    LayoutActor::kill();
    mNamePlate->kill();
}

void GalaxyMapGalaxyPlain::movement() {
    LayoutActor::movement();
    mNamePlate->movement();
}

void GalaxyMapGalaxyPlain::calcAnim() {
    LayoutActor::calcAnim();
    mNamePlate->calcAnim();

    if (MR::isExecuteCalcAnimLayout(mNamePlate)) {
        adjustTransInScreen();
        mNamePlate->calcAnim();
    }
}

void GalaxyMapGalaxyPlain::draw() const {
    if (!MR::isDead(this)) {
        mNamePlate->draw();
    }
}

void GalaxyMapGalaxyPlain::show(const wchar_t* pGalaxyName, const char* pPaneName) {
    appear();

    mPaneName = pPaneName;

    MR::copyString(mGalaxyName, pGalaxyName, GALAXY_NAME_BUFFER_SIZE);

    _30 = true;
}

void GalaxyMapGalaxyPlain::control() {
    if (mPaneName != nullptr) {
        MR::setLayoutScalePosAtPaneScaleTrans(mNamePlate, mHost, mPaneName);
    }

    if (_30) {
        mNamePlate->show(mGalaxyName, true);
    }

    _30 = false;
}
