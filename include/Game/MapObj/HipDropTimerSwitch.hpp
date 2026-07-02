#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class SpringValue;
class MapObjConnector;
class CollisionParts;

class HipDropTimerSwitch : public LiveActor {
public:
    HipDropTimerSwitch(const char*);

    virtual ~HipDropTimerSwitch();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void calcAnim();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void updateTimerSE();
    void exeOff();
    void exeSwitchDown();
    void exeOn();
    void exeSwitchUp();

    u32 _8C;
    SpringValue* _90;
    MapObjConnector* _94;
    CollisionParts* _98;
    TPos3f _9C;
    s32 _CC;
    s32 _D0;
    u8 _D4;
    u8 _D5;
    u8 _D6;
};
