#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Polta;
class HitSensor;
class PoltaArm;

class PoltaStatePunch : public ActorStateBase<Polta> {
public:
    PoltaStatePunch(Polta*);

    virtual ~PoltaStatePunch();
    virtual void appear();

    void exeStart();
    void exeToWait();
    void exeAttack();

    bool isEnableAttack(const HitSensor*) const;
    bool isEnablePunchArm() const;

    inline Polta* getPolta() { return mPoltaPtr; }
    PoltaArm* getAttackActor();

    /* 0xC */ Polta* mPoltaPtr;
    u8 padding[0xC];
    bool mIsLeftArmActor;
    bool mIsActionAffectBody;
};
