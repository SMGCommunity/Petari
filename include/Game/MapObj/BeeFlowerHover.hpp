#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class LodCtrl;
class MapPartsRailMover;
class MapPartsRailPosture;

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

    /* 0x8C */ TMtx34f _8C;
    /* 0xBC */ f32 _BC;
    /* 0xC0 */ LodCtrl* mLodCtrlPlanet;
    /* 0xC4 */ MapPartsRailMover* mRailMover;
    /* 0xC8 */ MapPartsRailPosture* mRailPosture;
    /* 0xCC */ TVec3f _CC;
};
