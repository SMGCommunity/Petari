#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MapObjConnector {
public:
    MapObjConnector(LiveActor*);

    bool attachToUnder();
    bool attachToBack();
    bool attach(const TVec3f&);
    void connect();
    void connect(LiveActor*);

    LiveActor* mHost;  // 0x0
    u32 _4;
    TMtx34f _8;
};