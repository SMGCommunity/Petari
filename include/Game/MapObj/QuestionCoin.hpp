#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class QuestionCoin : public LiveActor {
public:
    QuestionCoin(const char*);
    /* 0x08 */ virtual ~QuestionCoin();

private:
    u8 mPad[(0x124) - sizeof(LiveActor)];
};
