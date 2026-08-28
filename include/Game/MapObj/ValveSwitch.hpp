#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/MapObjConnector.hpp"

class ValveSwitch : public LiveActor {
public:
    ValveSwitch(const char*);

    /* 0x08 */ virtual ~ValveSwitch();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

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
