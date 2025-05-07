#pragma once

#include "Game/System/NerveExecutor.hpp"
#include "Game/Util.hpp"

class Tico;

class TicoDemoGetPower : public NerveExecutor {
public:
    TicoDemoGetPower(Tico *, const JMapInfoIter &);

    virtual ~TicoDemoGetPower();

    Tico* mTico;            // 0x08
};
