#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/Color.hpp"

class TreasureBoxCracked : public LiveActor {
public:
    TreasureBoxCracked(const char*);

    virtual ~TreasureBoxCracked();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    CollisionParts* mBoxCollider;  // 0x8C
    TMtx34f _90;
    u32 _C0;
    s32 mOpenCondition;  // 0xC4
    u32 _C8;
    u32 _CC;
    TVec3f _D0;
    TVec3f _DC;
    u8 _E8;
    s32 _EC;
    s32 _F0;
};