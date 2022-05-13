#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/MapObj/MapPartsRailMover.h"
#include "Game/MapObj/MapPartsRailPosture.h"

class BeeFlowerHover : public LiveActor {
public:
    BeeFlowerHover(const char *);

    virtual ~BeeFlowerHover();
    virtual void init(const JMapInfoIter &);
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual u32 receiveOtherMsg(u32, HitSensor *, HitSensor *);

    void exeSoftTouch();
    void exeSoftTouchWait();
    void exeHardTouch();
    void exeRecover();

    TMtx34f _8C;
    f32 _BC;
    LodCtrl* mLodCtrlPlanet;            // _C0
    MapPartsRailMover* mRailMover;      // _C4
    MapPartsRailPosture* mRailPosture;  // _C8
    TVec3f _CC;
};

namespace NrvBeeFlowerHover {
    NERVE(BeeFlowerHoverNrvWait);
    NERVE(BeeFlowerHoverNrvSoftTouch);
    NERVE(BeeFlowerHoverNrvSoftTouchWait);
    NERVE(BeeFlowerHoverNrvHardTouch);
    NERVE(BeeFlowerHoverNrvRecover);
};
