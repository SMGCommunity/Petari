#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class LavaHomeSeesaw : public LiveActor {
public:
    LavaHomeSeesaw(const char*);
    virtual ~LavaHomeSeesaw();

private:
    u8 mPad[(0xA4) - sizeof(LiveActor)];
};
