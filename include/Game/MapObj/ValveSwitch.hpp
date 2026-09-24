#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class MapObjConnector;

class ValveSwitch : public LiveActor {
public:
    ValveSwitch(const char*);

    virtual ~ValveSwitch() {
    }
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeAdjust();
    void exeValve();
    void exeEnd();
    void updateBindActorMtx();

    /* 0x8C */ LiveActor* _8C;
    /* 0x90 */ TPos3f _90;
    /* 0xC0 */ MapObjConnector* mObjConnector;
    /* 0xC4 */ bool _C4;
};
