#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class CocoSambo : public LiveActor {
public:
    CocoSambo(const char*);
    virtual ~CocoSambo();

private:
    u8 mPad[(0xC8) - sizeof(LiveActor)];
};
