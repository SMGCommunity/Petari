#pragma once

#include "Game/NPC/NPCActor.hpp"

class Peach : public NPCActor {
public:
    Peach(const char*);

    virtual ~Peach();
    virtual void init(const JMapInfoIter&);

    void exeWait();
};
