#pragma once

#include "Game/LiveActor/LiveActorGroup.hpp"

class PoltaRock;

class PoltaRockHolder : public DeriveActorGroup<PoltaRock> {
public:
	PoltaRockHolder();

	virtual ~PoltaRockHolder();
};
