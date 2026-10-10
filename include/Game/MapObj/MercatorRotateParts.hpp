#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/MapPartsAppearController.hpp"
#include "Game/MapObj/MapPartsRotator.hpp"

class MercatorRotateParts : public LiveActor {
public:
    MercatorRotateParts(const char*);

    virtual ~MercatorRotateParts();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    /* 0x8C */ MapPartsRotator* mRotator;
    /* 0x90 */ MapPartsAppearController* mAppearController;
    /* 0x94 */ TVec3f mLocalTrans;
    /* 0xA0 */ TVec3f mLocalRotate;
};