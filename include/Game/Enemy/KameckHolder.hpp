#pragma once

#include "Game/LiveActor/LiveActorGroup.hpp"

class Kameck;

class KameckHolder : public DeriveActorGroup< Kameck > {
public:
    KameckHolder(s32);

    virtual ~KameckHolder();

    void startDemoAppear();
    void endDemoAppear();
    void deadForceAll();
};