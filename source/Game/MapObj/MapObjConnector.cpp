#include "Game/MapObj/MapObjConnector.h"
#include "Game/Util.h"
#include "JSystem/JMath.h"

MapObjConnector::MapObjConnector(LiveActor *pActor) {
    mHost = pActor;
    _4 = 0;
    _8.identity();
}

void MapObjConnector::attachToUnder() {
    TVec3f up_vec;
    MR::calcUpVec(&up_vec, mHost);
    attach(up_vec);
}

void MapObjConnector::attachToBack() {
    TVec3f front_vec;
    MR::calcFrontVec(&front_vec, mHost);
    attach(front_vec);
}

void MapObjConnector::connect() {
    connect(mHost);
}
