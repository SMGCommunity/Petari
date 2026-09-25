#pragma once

#include "Game/LiveActor/LiveActor.hpp"

#define JETTURTLETYPE_RED 1
#define JETTURTLETYPE_GOLD 2

class JetTurtle : public LiveActor {
public:
    JetTurtle(const char*);

    virtual ~JetTurtle();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgThrow(HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    virtual void resetPosition();
    virtual void reset(u32);

    void exeWait();
    void endWait();
    void exeWait2();
    void endWait2();
    void exeThrowWait();
    virtual void exeThrowing();
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

    inline bool isNerveWait();

    /* 0x8C */ f32 _8C;
    /* 0x90 */ u16 mShellType;  // (0 = green, 1 = red, 2 = gold)
    /* 0x92 */ u16 _92;
    /* 0x94 */ HitSensor* _94;
    /* 0x98 */ HitSensor* _98;
    /* 0x9C */ TVec3f _9C;
    /* 0xA8 */ TVec3f _A8;
    /* 0xB4 */ TVec3f _B4;
    /* 0xC0 */ TVec3f _C0;
    /* 0xCC */ TVec3f _CC;
    /* 0xD8 */ u16 _D8;
    /* 0xDA */ u16 _DA;
    /* 0xDC */ u32 _DC;
    /* 0xE0 */ u8 _E0;
    /* 0xE1 */ u8 _E1;
    /* 0xE2 */ u8 _E2;
    /* 0xE3 */ u8 _E3;
};

class GoldenTurtle : public JetTurtle {
public:
    GoldenTurtle(const char* pName) : JetTurtle(pName) {
    }

    virtual ~GoldenTurtle();
    virtual void init(const JMapInfoIter&);
    virtual void resetPosition();
    virtual void reset(u32);
    virtual void exeThrowing();
};
