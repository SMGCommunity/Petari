#pragma once

#include "Game/LiveActor/LiveActor.h"

class MapObjConnector {
public:
    MapObjConnector(LiveActor *);

    void attachToUnder();
    void attachToBack();
    void attach(const TVec3f &);
    void connect();
    void connect(LiveActor *);

    LiveActor* mHost;   // _0
    u32 _4;
    TMtx34f _8;
};