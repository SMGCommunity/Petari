#pragma once

#include "Game/LiveActor/LiveActorGroup.hpp"

class PoltaGroundRock;

class PoltaGroundRockHolder : public DeriveActorGroup< PoltaGroundRock > {
public:
    PoltaGroundRockHolder();

    /* 0x08 */ virtual ~PoltaGroundRockHolder() {};

    void breakAll();
};
