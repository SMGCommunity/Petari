#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MapPartsAppearController;

class MercatorFixParts : public LiveActor {
public:
    MercatorFixParts(const char*);

    /* 0x08 */ virtual ~MercatorFixParts();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    MapPartsAppearController* mAppearController;  // 0x8C
};