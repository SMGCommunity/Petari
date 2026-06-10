#pragma once

#include "Game/NPC/NPCActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class NameObjArchiveListCollector;
class JMapInfoIter;

class DemoRabbit : public NPCActor {
public:
    DemoRabbit(const char*);
    virtual ~DemoRabbit();
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

private:
    u8 mPad[(0x16C) - sizeof(NPCActor)];
};
