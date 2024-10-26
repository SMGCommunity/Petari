#include "Game/Screen/GalaxyNamePlate.hpp"

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