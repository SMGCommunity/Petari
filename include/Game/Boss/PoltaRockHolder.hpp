#pragma once

#include "Game/LiveActor/LiveActorGroup.hpp"

class PoltaRock;

class PoltaRockHolder : public DeriveActorGroup< PoltaRock > {
public:
    PoltaRockHolder();

    /* 0x08 */ virtual ~PoltaRockHolder() {};
};
