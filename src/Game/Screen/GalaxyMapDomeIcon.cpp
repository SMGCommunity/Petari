#include "Game/Screen/GalaxyMapDomeIcon.hpp"
#include "Game/Screen/GalaxyNamePlate.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include <cstdio>

namespace {
    class AstroDomeCheckerBase {
    public:
        AstroDomeCheckerBase(int param1) { snprintf(_4, sizeof(_4), "Galaxy%1d", param1); }

        void operator()(const JMapInfoIter& rIter) {
            const char* pMapPaneName = nullptr;
            const char* pName = nullptr;

            rIter.getValue("MapPaneName", &pMapPaneName);
            rIter.getValue("name", &pName);

            if (pMapPaneName != nullptr && pName != nullptr && strstr(pMapPaneName, _4) != nullptr) {
                execute(pName);
            }
        }

        virtual void execute(const char* pParam1) {}

        /* 0x04 */ char _4[11];
        /* 0x0F */ bool mIsPass;
    };

    class CheckerIsExistNewGalaxy : public AstroDomeCheckerBase {
    public:
        CheckerIsExistNewGalaxy(int param1) : AstroDomeCheckerBase(param1) {}

        virtual void execute(const char* pParam1) {
            if (MR::canOpenGalaxy(pParam1) && !MR::isOnGameEventFlagGalaxyOpen(pParam1)) {
                mIsPass = true;
            }
        }
    };

    class CheckerIsComplete : public AstroDomeCheckerBase {
    public:
        CheckerIsComplete(int param1) : AstroDomeCheckerBase(param1) {}

        virtual void execute(const char* pParam1) {
            GalaxyStatusAccessor accessor = MR::makeGalaxyStatusAccessor(pParam1);

            if (accessor.getPowerStarNum()) {
                if (accessor.getPowerStarNumOwned() != accessor.getPowerStarNum()) {
                    mIsPass = false;
                }
            }
        }
    };
};  // namespace

GalaxyMapDomeIcon::GalaxyMapDomeIcon(int param1, LayoutActor* pHost, const char* pPaneName, const char* pParam4)
    : LayoutActor(pPaneName, true), _20(param1), mHost(pHost), mPaneName(pPaneName), _2C(pParam4), mNamePlate(nullptr), mMode(0) {}

void GalaxyMapDomeIcon::appear() {
    LayoutActor::appear();
    mNamePlate->appear();
    syncStatus();
}

void GalaxyMapDomeIcon::kill() {
    LayoutActor::kill();
    mNamePlate->kill();
}

void GalaxyMapDomeIcon::movement() {
    LayoutActor::movement();
    mNamePlate->movement();
}

void GalaxyMapDomeIcon::calcAnim() {
    MR::setLayoutScalePosAtPaneScaleTrans(this, mHost, mPaneName);
    MR::setLayoutScalePosAtPaneScaleTrans(mNamePlate, mHost, mPaneName);
    LayoutActor::calcAnim();
    mNamePlate->calcAnim();
}

void GalaxyMapDomeIcon::calcAnimForCapture(const nw4r::lyt::DrawInfo& rDrawInfo) {
    MR::setLayoutScalePosAtPaneScaleTrans(this, mHost, mPaneName);
    MR::calcAnimLayoutWithDrawInfo(this, rDrawInfo);
}

void GalaxyMapDomeIcon::setModeNormal() {
    mMode = 0;

    syncStatus();
}

void GalaxyMapDomeIcon::setModeNewDomeDiscover() {
    mMode = 1;

    syncStatus();
}

void GalaxyMapDomeIcon::draw() const {
    LayoutActor::draw();
    mNamePlate->draw();
}

s32 GalaxyMapDomeIcon::calcDomeStatus() const {
    if (isComplete()) {
        return 3;
    } else if (hasNewGalaxy()) {
        return 1;
    } else if (MR::hasGrandStar(_20)) {
        return 2;
    } else {
        return 0;
    }
}

bool GalaxyMapDomeIcon::isComplete() const {
    JMapInfo mapInfo = JMapInfo();

    // TODO: Replace with embedded BCSV file.
    mapInfo.attach((const void*)0x8053DE00);

    CheckerIsComplete checker = CheckerIsComplete(_20);

    for (JMapInfoIter rIter = JMapInfoIter(&mapInfo, 0); rIter != mapInfo.end(); rIter.mIndex++) {
        checker(rIter);
    }

    return checker.mIsPass;
}

void GalaxyMapDomeIcon::init(const JMapInfoIter& rIter) {
    initLayoutManager("MapDomeIcon", 3);

    mNamePlate = new GalaxyNamePlate(nullptr, false);
    mNamePlate->initWithoutIter();
    mNamePlate->kill();

    MR::startAnim(this, "DomeColor", 0);
    MR::setAnimFrameAndStop(this, _20 - 1, 0);
}

void GalaxyMapDomeIcon::control() {
    if (MR::isHiddenPane(mHost, _2C)) {
        MR::hideLayout(mNamePlate);
    } else {
        MR::showLayout(mNamePlate);
        mNamePlate->show(MR::getScenarioNameOnCurrentLanguage("AstroDome", _20), false);
    }
}

void GalaxyMapDomeIcon::syncStatus() {
    s32 domeStatus = calcDomeStatus();

    if (domeStatus == 0) {
        MR::hideLayout(this);
        MR::hidePane(mHost, _2C);
    } else {
        MR::showLayout(this);
        MR::showPane(mHost, _2C);
    }

    MR::startAnim(this, "Status", 0);
    MR::setAnimFrameAndStop(this, domeStatus, 0);

    bool b = mMode != 0 && _20 == 2;

    if (b) {
        MR::startAnim(this, "SelectIn", 1);
        MR::setAnimFrameAndStopAtEnd(this, 1);
        MR::startAnim(this, "Blink", 0);
    } else {
        MR::startAnim(this, "Wait", 1);
    }
}

bool GalaxyMapDomeIcon::hasNewGalaxy() const {
    JMapInfo mapInfo = JMapInfo();

    // TODO: Replace with embedded BCSV file.
    mapInfo.attach((const void*)0x8053DE00);

    CheckerIsExistNewGalaxy checker = CheckerIsExistNewGalaxy(_20);

    for (JMapInfoIter rIter = JMapInfoIter(&mapInfo, 0); rIter != mapInfo.end(); rIter.mIndex++) {
        checker(rIter);
    }

    return checker.mIsPass;
}
