#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;
class AnimScaleParam;

class SnowMan : public LiveActor {
public:
    SnowMan(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void damageHead();
    void damageBody();
    void exeMeltHead();
    void exeMeltBody();
    void exeDownHead();
    void exeDownBody();
    void exeWait();
    void exeWaitBody();
    void exeWaitHead();

    /* 0x8C */ CollisionParts* mHeadCollisionParts;
    /* 0x90 */ CollisionParts* mBodyCollisionParts;
    /* 0x94 */ AnimScaleController* mAnimScaleCtrl;
    /* 0x98 */ AnimScaleParam* mAnimScaleParam;
};
