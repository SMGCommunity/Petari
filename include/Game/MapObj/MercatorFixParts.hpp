#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/MapPartsAppearController.hpp"

class MercatorFixParts : public LiveActor {
public:
    MercatorFixParts(const char*);

    virtual ~MercatorFixParts();
    virtual void init(const JMapInfoIter&);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    MapPartsAppearController* mAppearController;  // 0x8C
};