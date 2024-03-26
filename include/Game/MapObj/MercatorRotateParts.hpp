#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/MapPartsAppearController.hpp"
#include "Game/MapObj/MapPartsRotator.hpp"

class MercatorRotateParts : public LiveActor {
public:
    MercatorRotateParts(const char *);

    virtual ~MercatorRotateParts();
    virtual void init(const JMapInfoIter &);
    virtual void initAfterPlacement();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    MapPartsRotator* mRotator;                          // _8C
    MapPartsAppearController* mAppearController;        // _90
    TVec3f mLocalTrans;                                 // _94
    TVec3f mLocalRotate;                                // _A0
};