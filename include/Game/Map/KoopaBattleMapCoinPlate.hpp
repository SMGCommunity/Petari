#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class KoopaBattleMapCoinPlate : public LiveActor {
public:
    KoopaBattleMapCoinPlate(const char*);
    virtual ~KoopaBattleMapCoinPlate();

private:
    u8 mPad[(0xBC) - sizeof(LiveActor)];
};
