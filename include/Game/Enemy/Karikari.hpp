#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class Karikari : public LiveActor {
public:
    Karikari(const char*);
    virtual ~Karikari();

private:
    u8 mPad[(0xA8) - sizeof(LiveActor)];
};
