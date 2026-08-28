#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class CollisionParts;

class TreasureBoxCracked : public LiveActor {
public:
    TreasureBoxCracked(const char*);

    /* 0x08 */ virtual ~TreasureBoxCracked();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

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