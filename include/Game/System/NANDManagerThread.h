#pragma once

#include "Game/System/OSThreadWrapper.h"

class NANDManagerThread : public OSThreadWrapper {
public:
    NANDManagerThread(int, int, JKRHeap *);

    virtual ~NANDManagerThread();
    virtual s32 run();
};