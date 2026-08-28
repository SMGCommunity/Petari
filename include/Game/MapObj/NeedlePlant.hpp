#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"

class NeedlePlant : public MapObjActor {
public:
    NeedlePlant(const char*);

    /* 0x08 */ virtual ~NeedlePlant();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x2C */ virtual void kill();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x58 */ virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    /* 0x78 */ virtual void connectToScene(const MapObjActorInitInfo&);

    void exeShake();
    void exeWait();

    s32 _C4;  // 0xC4
};
