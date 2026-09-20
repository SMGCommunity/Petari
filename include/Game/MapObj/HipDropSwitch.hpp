#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class CollisionParts;
class MapObjConnector;
class SpringValue;

class HipDropSwitch : public LiveActor {
public:
    HipDropSwitch(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void calcAnim();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initStarPointerTarget();
    bool trySwitchDown();
    bool tryOn();

    void exeOff();
    void exeSwitchDown();
    void exeOn();

    /* 0x8C */ u32 _8C;  // useless
    /* 0x90 */ SpringValue* mSpringValue;
    /* 0x94 */ MapObjConnector* mConnector;
    /* 0x98 */ CollisionParts* mSpringModel;
    /* 0x9C */ TPos3f _9C;
    /* 0xCC */ bool _CC;
    /* 0xCD */ bool _CD;
    /* 0xCE */ bool _CE;
};
