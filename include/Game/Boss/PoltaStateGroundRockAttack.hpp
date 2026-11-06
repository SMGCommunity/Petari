#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Polta;

class PoltaStateGroundRockAttack : public ActorStateBase< Polta > {
public:
    PoltaStateGroundRockAttack(Polta*);

    virtual ~PoltaStateGroundRockAttack();
    virtual void appear();

    void exeSign();
    void exeGenerate();
    void exeEnd();

    /* 0x10 */ s32 mRockCount; ///< The amount of Ground Rocks Bouldgeist spawns in the attack.
    /* 0x14 */ bool mIsAppearClockwise;
};
