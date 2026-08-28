#pragma once

#include "Game/LiveActor/LiveActor.hpp"

#define JETTURTLETYPE_RED (s16)1
#define JETTURTLETYPE_GOLD (s16)2

class JetTurtle : public LiveActor {
public:
    JetTurtle(const char*);

    /* 0x08 */ virtual ~JetTurtle();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x24 */ virtual void appear();
    /* 0x48 */ virtual void control();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    /* 0x6C */ virtual bool receiveMsgThrow(HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    /* 0x78 */ virtual void resetPosition();
    /* 0x7C */ virtual void reset(u32);

    void exeWait();
    void endWait();
    void exeWait2();
    void endWait2();
    void exeThrowWait();
    /* 0x80 */ virtual void exeThrowing();
    void exeTakenReserve();
    void exeTakenStart();
    void exeRestart();
    void exeDrop();

    void init2(const JMapInfoIter&, s32);
    void becomeSlowType();
    void becomeFastType();
    void appearAndTryTaken();
    void resetPositionAndVanish();
    void boundDrop();
    void bound();
    bool isWait() const;
    bool isRestart() const;

    f32 _8C;
    u16 mShellType;  // 0x90 (0 = green, 1 = red, 2 = gold)
    u16 _92;
    HitSensor* _94;
    HitSensor* _98;
    TVec3f _9C;
    TVec3f _A8;
    TVec3f _B4;
    TVec3f _C0;
    TVec3f _CC;
    u16 _D8;
    u16 _DA;
    u32 _DC;
    u8 _E0;
    u8 _E1;
    u8 _E2;
    u8 _E3;
};

class GoldenTurtle : public JetTurtle {
public:
    GoldenTurtle(const char* pName) : JetTurtle(pName) {
    }

    /* 0x08 */ virtual ~GoldenTurtle();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x78 */ virtual void resetPosition();
    /* 0x7C */ virtual void reset(u32);
    /* 0x80 */ virtual void exeThrowing();
};
