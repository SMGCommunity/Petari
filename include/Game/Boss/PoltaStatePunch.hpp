#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Polta;
class HitSensor;
class PoltaArm;

class PoltaStatePunch : public ActorStateBase< Polta > {
public:
    PoltaStatePunch(Polta*);

    virtual ~PoltaStatePunch();
    virtual void appear();

    void exeStart();
    void exeToWait();
    void exeAttack();

    bool isEnableAttack(const HitSensor*) const;
    bool isEnablePunchArm() const;

    PoltaArm* getAttackActor();

    u8 padding[0xC];
    /* 0x1C */ bool mIsLeftArmActor;
    /* 0x1D */ bool mIsActionAffectBody;
};
