#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class CoconutTree : public LiveActor {
public:
    CoconutTree(const char*);

    /* 0x08 */ virtual ~CoconutTree();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
};