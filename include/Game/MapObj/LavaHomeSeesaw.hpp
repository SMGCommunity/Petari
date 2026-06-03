#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class LavaHomeSeesaw : public LiveActor {
public:
    LavaHomeSeesaw(const char*);
    virtual ~LavaHomeSeesaw();

private:
    u8 mPad[(0xA4) - sizeof(LiveActor)];
};
