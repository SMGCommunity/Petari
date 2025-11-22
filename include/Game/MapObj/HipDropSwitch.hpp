#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class SpringValue;
class MapObjConnector;
class CollisionParts;

class HipDropSwitch : public LiveActor {
public:
    HipDropSwitch(const char*);

    virtual ~HipDropSwitch();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void calcAnim();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    bool trySwitchDown();
    bool tryOn();
    void exeOff();
    void exeSwitchDown();
    void exeOn();

    u32 _8C;
    SpringValue* mSpringValue;    // 0x90
    MapObjConnector* mConnector;  // 0x94
    CollisionParts* _98;
    TPos3f _9C;
    u8 _CC;
    u8 _CD;
    u8 _CE;
};
