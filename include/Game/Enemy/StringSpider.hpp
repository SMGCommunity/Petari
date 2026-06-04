#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class StringSpider : public LiveActor {
public:
    StringSpider(const char*);
    virtual ~StringSpider();

private:
    u8 mPad[(0xC8) - sizeof(LiveActor)];
};
