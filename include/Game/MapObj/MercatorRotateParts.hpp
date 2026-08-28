#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/MapPartsAppearController.hpp"
#include "Game/MapObj/MapPartsRotator.hpp"

class MercatorRotateParts : public LiveActor {
public:
    MercatorRotateParts(const char*);

    /* 0x08 */ virtual ~MercatorRotateParts();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    MapPartsRotator* mRotator;                    // 0x8C
    MapPartsAppearController* mAppearController;  // 0x90
    TVec3f mLocalTrans;                           // 0x94
    TVec3f mLocalRotate;                          // 0xA0
};