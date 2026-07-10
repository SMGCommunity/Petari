#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class HammerHeadPackun : public LiveActor {
public:
    HammerHeadPackun(const char*);

    virtual ~HammerHeadPackun();
    virtual void init(const JMapInfoIter&);
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeSearch();
    void exeAttack();
    void exeRest();
    void exeRecover();
    void exeDamage();
    void exeRise();
    void exeCrow();
    void exeSmashKill();
    void exeBlowKill();
    void exeNumb();
    void initPosture();
    void initSensor();
    void initShadow();
    void initLeaf(const JMapInfoIter&);
    bool isTargetInRange() const;
    bool isInFieldOfView(const TVec3f&) const;
    bool isAttackSensorValid() const;
    bool isChance() const;
    void calcPlayerDir(TVec3f*) const;
    void turnTo(const TVec3f&, f32);
    void turnToAttackVec(s32);
    void verticalizeFrontVec();
    bool tryShiftNumb();
    void receiveMsgPlayerAttackChance(u32, HitSensor*, HitSensor*);

    /* 0x8C */ s32 _8C;
    /* 0x90 */ s32 _90;
    /* 0x94 */ f32 _94;
    /* 0x98 */ f32 _98;
    /* 0x9C */ f32 _9C;
    /* 0xA0 */ s32 _A0;
    /* 0xA4 */ s32 _A4;
    /* 0xA8 */ f32 _A8;
    /* 0xAC */ f32 _AC;
    /* 0xB0 */ f32 _B0;
    /* 0xB4 */ f32 _B4;
    /* 0xB8 */ s32 _B8;
    /* 0xBC */ s32 _BC;

private:
    u8 mPad[(0xC0) - sizeof(LiveActor)];
};
