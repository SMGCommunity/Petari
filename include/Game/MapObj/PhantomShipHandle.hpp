#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MapPartsSeesaw1AxisRotator;

class PhantomShipHandle : public LiveActor {
public:
    PhantomShipHandle(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void calcAndSetBaseMtx();

    /* 0x8C */ MapPartsSeesaw1AxisRotator* mRotator;
};
