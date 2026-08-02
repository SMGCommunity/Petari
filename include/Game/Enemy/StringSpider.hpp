#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class StringSpider : public LiveActor {
public:
    StringSpider(const char*);

    virtual ~StringSpider();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void calcAnim();
    virtual void kill();
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    s32 getSearchToAttackTime();
    void doSwingAction(f32, bool);
    void reactDPD();
    void reactStringForce();
    bool isOverStringLength(TVec3f*, f32*);
    void calcToRootVector();
    void calcPosture(f32, bool);
    void exeWait();
    void exeDownStart();
    void exeDownEnd();
    void exeDown();
    void exeUp();
    void exeSearch();
    void exeAttack();
    void exeHit();
    void exeSpinned();
    void exeTrampleFall();
    void exeWallHitDeath();
    void exeBlow();
    void exeDpdGripped();
    void tearDownDpdGripped();
    void extendString();
    void shortenString();
    void tearDownDpdGrreactSpinHitipped(HitSensor*, HitSensor*);
    void startSwingLevelSound();
};

class StringSpiderString : public LiveActor {
public:
    StringSpiderString(const char*);

    virtual ~StringSpiderString();
    virtual void calcAndSetBaseMtx();
};
