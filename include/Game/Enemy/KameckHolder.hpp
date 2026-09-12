#pragma once

#include "Game/LiveActor/LiveActorGroup.hpp"

class Kameck;

class KameckHolder : public DeriveActorGroup<Kameck> {
public:
    KameckHolder(s32);

    void startDemoAppear();
    void endDemoAppear();
    void deadForceAll();
};
