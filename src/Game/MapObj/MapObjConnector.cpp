#include "Game/MapObj/MapObjConnector.hpp"
#include "Game/Util.hpp"
#include "JSystem/JMath.hpp"

MapObjConnector::MapObjConnector(LiveActor* pActor) {
    mHost = pActor;
    _4 = 0;
    _8.identity();
}

bool MapObjConnector::attachToUnder() {
    TVec3f up_vec;
    MR::calcUpVec(&up_vec, mHost);

    return attach(up_vec);
}

bool MapObjConnector::attachToBack() {
    TVec3f front_vec;
    MR::calcFrontVec(&front_vec, mHost);

    return attach(front_vec);
}

void MapObjConnector::connect() {
    connect(mHost);
}
