#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class MapPartsAppearController;
class MapPartsRailMover;
class MapPartsRailRotator;

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

    /* 0x8C */ MapPartsRailMover* mRailMover;
    /* 0x90 */ MapPartsRailRotator* mRailRotator;
    /* 0x94 */ MapPartsAppearController* mAppearController;
    /* 0x98 */ TMtx34f mRotateMtx;
    /* 0xC8 */ TVec3f mLocalTrans;
    /* 0xD4 */ TVec3f mLocalRotation;
    /* 0xE0 */ TVec3f _E0;
    /* 0xEC */ bool mIsNotMoving;
};