#pragma once

#include "Game/LiveActor/LiveActor.hpp"

#define JETTURTLETYPE_RED (s16)1
#define JETTURTLETYPE_GOLD (s16)2

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
    virtual void exeThrowing();

    inline void endWait() {
        if (mShellType == JETTURTLETYPE_GOLD) {
            MR::deleteEffect(this, "Glow");
        }
    }

    void init2(const JMapInfoIter&, s32);
    void becomeSlowType();
    void becomeFastType();
    void appearAndTryTaken();
    void resetPositionAndVanish();
    void exeWait();
    void exeThrowWait();
    void boundDrop();
    void bound();
    void exeTakenReserve();
    void exeTakenStart();
    void exeRestart();
    void exeDrop();
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
    GoldenTurtle(const char* pName) : JetTurtle(pName) {}

    virtual ~GoldenTurtle();
    virtual void init(const JMapInfoIter&);
    virtual void resetPosition();
    virtual void reset(u32);
    virtual void exeThrowing();
};
