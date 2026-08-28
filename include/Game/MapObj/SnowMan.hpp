#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;
class AnimScaleParam;

class SnowMan : public LiveActor {
public:
    SnowMan(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

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
