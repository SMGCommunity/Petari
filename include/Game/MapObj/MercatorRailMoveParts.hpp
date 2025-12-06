#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/MapPartsAppearController.hpp"

class MercatorRailMoveParts : public LiveActor {
public:
    MercatorRailMoveParts(const char*);

    virtual ~MercatorRailMoveParts();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void startMove();
    void endMove();
    void updatePose();

    MapPartsRailMover* mRailMover;                // 0x8C
    MapPartsRailRotator* mRailRotator;            // 0x90
    MapPartsAppearController* mAppearController;  // 0x94
    TMtx34f mRotateMtx;                           // 0x98
    TVec3f mLocalTrans;                           // 0xC8
    TVec3f mLocalRotation;                        // 0xD4
    TVec3f _E0;
    bool mIsNotMoving;  // 0xEC
};