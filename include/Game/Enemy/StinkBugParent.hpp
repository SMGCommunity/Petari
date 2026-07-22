#pragma once

#include "Game/Enemy/StinkBugBase.hpp"

class CollisionParts;

class StinkBugParent : public StinkBugBase {
public:
    StinkBugParent(const char*);

    virtual ~StinkBugParent();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeSearch();
    void exeDashSign();
    void exeDash();
    void exeDashEnd();
    void exeBack();
    void exeHipDropDown();
    void exeAttack();
    void exeSpinReaction();
    void exePanic();
    void exeRecover();
    void exeShakeStart();
    void exeDemoChildDown();
    void exeDemoAnger();
    void jumpBackPlayerFromChild() const;
    bool isShakeChance() const;
    bool isHitChild() const;
    bool isNrvEnableStarPieceAttack() const;

    /* 0xBC */ CollisionParts* mParent;
    /* 0xC0 */ bool _C0;
};
