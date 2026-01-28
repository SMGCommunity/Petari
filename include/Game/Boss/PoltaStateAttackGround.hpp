#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Polta;
class HitSensor;

class PoltaStateAttackGround : public ActorStateBase<Polta> {
public:
    PoltaStateAttackGround(Polta*);

    virtual ~PoltaStateAttackGround();
    virtual void appear();

    void exeStart();
    void exeAttack();
    void exeToWait();

    bool isEnableAttack(const HitSensor*) const;

    /* 0xC */ Polta* mPoltaPtr;
    /* 0x10 */ bool mIsAffectBody;
    /* 0x14 */ s32 mAttackStartLength;
    /* 0x18 */ s32 mAttackLength;
    /* 0x1C */ s32 mAttackToWaitLength;
};
