#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class CoconutTreeLeafGroup : public LiveActor {
public:
    CoconutTreeLeafGroup(const char*);
    virtual ~CoconutTreeLeafGroup();

private:
    u8 mPad[(0xA4) - sizeof(LiveActor)];
};
