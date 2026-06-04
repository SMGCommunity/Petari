#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class QuestionCoin : public LiveActor {
public:
    QuestionCoin(const char*);
    virtual ~QuestionCoin();

private:
    u8 mPad[(0x124) - sizeof(LiveActor)];
};
