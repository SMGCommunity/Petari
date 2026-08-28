#pragma once

#include "Game/System/OSThreadWrapper.hpp"

class NANDRequestInfo;

class NANDManagerThread : public OSThreadWrapper {
public:
    NANDManagerThread(int, int, JKRHeap*);

    /* 0x0C */ virtual void* run();

    void executeWriteSequence(NANDRequestInfo*);
    void executeReadSequence(NANDRequestInfo*);
};