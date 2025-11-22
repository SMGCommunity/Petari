#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"

class NeedlePlant : public MapObjActor {
public:
    NeedlePlant(const char*);

    virtual ~NeedlePlant();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual void connectToScene(const MapObjActorInitInfo&);

    void exeShake();
    void exeWait();

    s32 _C4;  // 0xC4
};
