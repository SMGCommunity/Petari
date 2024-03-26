#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/MapPartsAppearController.hpp"

class MercatorRailMoveParts : public LiveActor {
public:
    MercatorRailMoveParts(const char *);

    virtual ~MercatorRailMoveParts();
    virtual void init(const JMapInfoIter &);
    virtual void initAfterPlacement();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    void startMove();
    void endMove();
    void updatePose();

    MapPartsRailMover* mRailMover;                      // _8C
    MapPartsRailRotator* mRailRotator;                  // _90
    MapPartsAppearController* mAppearController;        // _94
    TMtx34f mRotateMtx;                                 // _98
    TVec3f mLocalTrans;                                 // _C8
    TVec3f mLocalRotation;                              // _D4
    TVec3f _E0;
    bool mIsNotMoving;                                  // _EC
};