#pragma once

#include "Game/NPC/NPCActor.hpp"

class Peach : public NPCActor {
public:
    Peach(const char*);

    /* 0x08 */ virtual ~Peach();
    /* 0x0C */ virtual void init(const JMapInfoIter&);

    void exeWait();
};
