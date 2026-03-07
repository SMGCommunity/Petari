#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class JellyfishElectric : public LiveActor {
public:
    JellyfishElectric(const char*);
    virtual ~JellyfishElectric();

private:
    u8 mPad[(0xC0) - sizeof(LiveActor)];
};
