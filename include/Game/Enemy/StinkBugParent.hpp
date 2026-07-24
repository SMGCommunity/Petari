#pragma once

#include "Game/Enemy/StinkBugBase.hpp"

class CollisionParts;

namespace {
    struct Param {
        f32 mDashVelocity;  // 0x0
        f32 mDashDistance;  // 0x4
        s32 mPanicStep;     // 0x8
    };
};  // namespace

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
    void exeDashSignEnd();
    void exeDash();
    void exeDashEnd();
    void exeBack();
    void exeHipDropDown();
    void exeAttack();
    void exeSpinReaction();
    void exePanic();
    void exeRecover();
    void exeShakeStart();
    void exeShake();
    void exeDemoPrepare();
    void exeDemoChildDown();
    void exeDemoAnger();
    void jumpBackPlayerFromChild() const;
    bool isShakeChance() const;
    bool isHitChild() const;
    bool isNrvEnableStarPieceAttack() const;

    inline bool isNrvMoving() const;
    inline bool isNrvDemo() const;

    /* 0xBC */ CollisionParts* mParent;
    /* 0xC0 */ bool mIsChildAlive;
};
