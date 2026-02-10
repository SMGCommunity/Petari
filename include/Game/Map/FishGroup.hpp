#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class FishGroup : public LiveActor {
public:
    FishGroup(const char*);
    virtual ~FishGroup();

private:
    u8 mPad[(0xB0) - sizeof(LiveActor)];
};
