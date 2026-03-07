#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/MapObj/MapPartsRailPosture.hpp"

class BeeFlowerHover : public LiveActor {
public:
    BeeFlowerHover(const char*);

    virtual ~BeeFlowerHover();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeSoftTouch();
    void exeSoftTouchWait();
    void exeHardTouch();
    void exeRecover();

    TMtx34f _8C;
    f32 _BC;
    LodCtrl* mLodCtrlPlanet;            // 0xC0
    MapPartsRailMover* mRailMover;      // 0xC4
    MapPartsRailPosture* mRailPosture;  // 0xC8
    TVec3f _CC;
};
