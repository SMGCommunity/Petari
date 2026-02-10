#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/NPC/NPCActor.hpp"

class NameObjArchiveListCollector;
class JMapInfoIter;

class Rabbit : public NPCActor {
public:
    Rabbit(const char*);
    virtual ~Rabbit();
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

private:
    u8 mPad[(0x184) - sizeof(NPCActor)];
};
