#pragma once

#include "Game/System/NerveExecutor.hpp"

class JMapInfoIter;
class Tico;

class TicoDemoGetPower : public NerveExecutor {
public:
    TicoDemoGetPower(Tico*, const JMapInfoIter&);

    void startDemo();

    void exeWait();
    void exeDemo();

    /* 0x08 */ Tico* mHost;
};
