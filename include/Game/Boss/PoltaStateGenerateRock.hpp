#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Polta;

class PoltaStateGenerateRock : public ActorStateBase<Polta> {
public:
	PoltaStateGenerateRock(Polta*);

	virtual ~PoltaStateGenerateRock();
	virtual void appear();

	void exeSign();
	void exeGenerate();
	void exeEnd();

	Polta* mOwner;				// 0x0C
	s32 mPatternIndex;			// 0x10
	s32 mIndexIntoPattern;		// 0x14
	s32 mMaxIndexIntoPattern;	// 0x18
	s32 mEndDelayStep;			// 0x1C
};
