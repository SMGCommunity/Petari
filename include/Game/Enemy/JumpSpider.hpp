#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class JumpSpider : public LiveActor {
public:
    JumpSpider(const char*);
    virtual ~JumpSpider();

private:
    u8 mPad[(0xAC) - sizeof(LiveActor)];
};
