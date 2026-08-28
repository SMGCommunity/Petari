#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Polta;
class HitSensor;
class PoltaArm;

class PoltaStatePunch : public ActorStateBase< Polta > {
public:
    PoltaStatePunch(Polta*);

    /* 0x08 */ virtual ~PoltaStatePunch();
    /* 0x10 */ virtual void appear();

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
