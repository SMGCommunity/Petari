#include "Game/Screen/GalaxyCometScreenFilter.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

GalaxyCometScreenFilter::GalaxyCometScreenFilter() : LayoutActor("コメット用画面塗り潰し", true), _20(true) {
    MR::connectToScene(this, 14, 13, -1, 65);
    initLayoutManager("CometScreenFilter", 1);
    appear();
}

void GalaxyCometScreenFilter::setCometType(const char* pCometName) {
    MR::setCometAnimFromId(this, MR::getCometNameIdFromString(pCometName), 0);
}
