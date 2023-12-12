#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/Util/Color.h"

namespace {
    static Color8 sPointLightColor(0xFF, 0xFF, 0x96, 0xFF);
    const Vec sTopOpenStartOffset = { 0.0f, -100.0f, -250.0f };
    const Vec sTopOpenEndOffset = { 0.0f, 23.0f, -250.0f };
    const Vec sCoinOffset = { 0.0f, 130.0f, 0.0f };
    const Vec sChipOffset = { 0.0f, 170.0f, 0.0f };
    const Vec sKinokoOneUpOffset = { 0.0f, 130.0f, 0.0f };
    const Vec sKinokoLifeUpOffset = { 0.0f, 130.0f, 0.0f };
    const Vec sAirBubbleOffset = { 0.0f, 220.0f, 0.0f };
};

class TreasureBoxCracked : public LiveActor {
public:
    TreasureBoxCracked(const char *);

    virtual ~TreasureBoxCracked();
    virtual void init(const JMapInfoIter &);
    virtual void control();
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor *, HitSensor *);

    CollisionParts* mBoxCollider;   // _8C
    TMtx34f _90;
    u32 _C0;
    s32 mOpenCondition;             // _C4
    u32 _C8;
    u32 _CC;
    TVec3f _D0;
    TVec3f _DC;
    u8 _E8;
    s32 _EC;
    s32 _F0;
};