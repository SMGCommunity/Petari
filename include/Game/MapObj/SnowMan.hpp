#pragma once

#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class SnowMan : public LiveActor {
public:
    SnowMan(const char*);

    virtual ~SnowMan();
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

    CollisionParts* mHeadCollisionParts; /* 0x8C */
    CollisionParts* mBodyCollisionParts; /* 0x90 */
    AnimScaleController* mAnimScaleCtrl; /* 0x94 */
    AnimScaleParam* mAnimScaleParam;     /* 0x98 */
};
