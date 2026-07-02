#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class WhirlPoolAccelerator : public LiveActor {
public:
    WhirlPoolAccelerator(const char*);
    virtual ~WhirlPoolAccelerator();

    bool calcInfo(const TVec3f&, TVec3f*) const;

private:
    u8 mPad[(0xC4) - sizeof(LiveActor)];
};
