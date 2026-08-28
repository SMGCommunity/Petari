#pragma once

#include "Game/NPC/NPCActor.hpp"

class Rabbit : public NPCActor {
public:
    Rabbit(const char*);
    /* 0x08 */ virtual ~Rabbit();
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

private:
    u8 mPad[(0x184) - sizeof(NPCActor)];
};
