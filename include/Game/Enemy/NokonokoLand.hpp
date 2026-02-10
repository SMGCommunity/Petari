#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class NokonokoLand : public LiveActor {
public:
    NokonokoLand(const char*);
    virtual ~NokonokoLand();

private:
    u8 mPad[(0xC8) - sizeof(LiveActor)];
};
