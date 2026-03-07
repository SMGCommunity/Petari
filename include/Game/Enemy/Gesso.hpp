#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class Gesso : public LiveActor {
public:
    Gesso(const char*);
    virtual ~Gesso();

private:
    u8 mPad[(0xD8) - sizeof(LiveActor)];
};
