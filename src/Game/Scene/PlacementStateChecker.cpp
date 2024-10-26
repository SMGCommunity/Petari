#include "Game/Scene/PlacementStateChecker.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

PlacementStateChecker::PlacementStateChecker(const char *pName) : NameObj(pName), mCurrentZoneId(-1) {

}

void PlacementStateChecker::setCurrentPlacementZoneId(int zoneID) {
    mCurrentZoneId = zoneID;
}

void PlacementStateChecker::clearCurrentPlacementZoneId() {
    mCurrentZoneId = -1;
}

int PlacementStateChecker::getCurrentPlacementZoneId() const {
    return mCurrentZoneId;
}

namespace MR {
    PlacementStateChecker* getPlacementStateChecker() {
        return reinterpret_cast<PlacementStateChecker*>(MR::getSceneObjHolder()->getObj(SceneObj_PlacementStateChecker));
    }
};

PlacementStateChecker::~PlacementStateChecker() {

}