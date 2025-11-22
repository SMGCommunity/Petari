#include "Game/Screen/GalaxyMapCometIcon.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"

GalaxyMapCometIcon::GalaxyMapCometIcon(const char* pName, LayoutActor* pParam2, const char* pParam3)
    : LayoutActor(pName, true), _20(pName), _24(pParam2), _28(pParam3) {}

void GalaxyMapCometIcon::appear() {
    LayoutActor::appear();
    syncStatus();
}

void GalaxyMapCometIcon::calcAnim() {
    MR::setLayoutScalePosAtPaneScaleTransIfExecCalcAnim(this, _24, _28);
    LayoutActor::calcAnim();
}

void GalaxyMapCometIcon::calcAnimForCapture(const nw4r::lyt::DrawInfo& rDrawInfo) {
    MR::setLayoutScalePosAtPaneScaleTransIfExecCalcAnim(this, _24, _28);
    MR::calcAnimLayoutWithDrawInfo(this, rDrawInfo);
}

void GalaxyMapCometIcon::control() {}

GalaxyMapCometIcon::~GalaxyMapCometIcon() {}

void GalaxyMapCometIcon::init(const JMapInfoIter& rIter) {
    initLayoutManager("IconComet", 2);
    MR::startAnim(this, "Wait", 0);
}

void GalaxyMapCometIcon::syncStatus() {
    int cometId = -1;

    if (MR::isGalaxyCometLandInStage(_20)) {
        cometId = MR::getEncounterGalaxyCometNameId(_20);
    }

    if (cometId < 0) {
        MR::hideLayout(this);
    } else {
        MR::showLayout(this);
        MR::startAnim(this, "Color", 1);
        MR::setCometAnimFromId(this, cometId, 1);
    }
}
