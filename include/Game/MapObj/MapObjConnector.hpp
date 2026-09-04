#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class MapObjConnector {
public:
    MapObjConnector(LiveActor*);

    bool attachToUnder();
    bool attachToBack();
    bool attach(const TVec3f&);
    void connect();
    void connect(LiveActor*);

    /* 0x0 */ LiveActor* mHost;
    /* 0x4 */ CollisionParts* mParts;
    /* 0x8 */ TMtx34f _8;
};
