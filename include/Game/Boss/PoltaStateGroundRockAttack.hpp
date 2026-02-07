#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Polta;

class PoltaStateGroundRockAttack : public ActorStateBase<Polta> {
public:
    PoltaStateGroundRockAttack(Polta*);
    
    virtual ~PoltaStateGroundRockAttack();
    virtual void appear();

	void exeSign();
	void exeGenerate();
	void exeEnd();

    inline Polta* getPolta() { return mOwner; }
    
    Polta* mOwner;
    s32 _10;
    bool _14;
};
