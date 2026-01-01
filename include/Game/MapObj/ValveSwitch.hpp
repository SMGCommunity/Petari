#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/MapObjConnector.hpp"

class ValveSwitch : public LiveActor {
public:
    ValveSwitch(const char*);

    virtual ~ValveSwitch();
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

    LiveActor* _8C;
    TPos3f _90;
    MapObjConnector* mObjConnector;  // 0xC0
    bool _C4;
};
