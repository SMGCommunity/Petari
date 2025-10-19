#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/GalaxyNamePlate.hpp"
#include "Game/Util/MessageUtil.hpp"

namespace NrvGalaxyNamePlate {
    NEW_NERVE(GalaxyNamePlateNrvAppearReady, GalaxyNamePlate, AppearReady);
    NEW_NERVE(GalaxyNamePlateNrvAppear, GalaxyNamePlate, Appear);
    NEW_NERVE(GalaxyNamePlateNrvWait, GalaxyNamePlate, Wait);
};

void GalaxyNamePlate::showUnknown(bool a1) {
    show(MR::getGameMessageDirect("GalaxyNameShort_Unkonwn"), 0, true, a1);
}

void GalaxyNamePlate::showNew(bool a1) {
    show(MR::getGameMessageDirect("GalaxyNameShort_New"), 1, true, a1);
}

void GalaxyNamePlate::showOpen(bool a1) {
    show(MR::getGalaxyNameShortOnCurrentLanguage(mGalaxyName), 2, true, a1);
}

void GalaxyNamePlate::show(const wchar_t *pName, bool a2) {
    show(pName, 2, a2, true);
}

void GalaxyNamePlate::setShowBalloonNozzle(bool show) {
    mShowBalloonNozzle = show;
}

void GalaxyNamePlate::control() {
    _24 = false;
}
