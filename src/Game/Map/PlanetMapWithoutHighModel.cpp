#include "Game/Map/PlanetMapWithoutHighModel.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "cstdio"

PlanetMapWithoutHighModel::PlanetMapWithoutHighModel(const char* pName) : MapObjActor(pName) {}

void PlanetMapWithoutHighModel::connectToScene(const MapObjActorInitInfo&) {
    MR::connectToScenePlanet(this);
}

PlanetMapWithoutHighModel::~PlanetMapWithoutHighModel() {}

void PlanetMapWithoutHighModel::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    info.setupHioNode("◆◆ハイモデル無しの惑星◆◆");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupEffect(nullptr);
    info.setupFarClipping(-1.0f);
    char buf[0x100];
    snprintf(buf, sizeof(buf), "%sLow", mObjectName);
    info.setupModelName(buf);
    initialize(rIter, info);
}
