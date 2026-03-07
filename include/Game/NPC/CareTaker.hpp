#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/NPC/NPCActor.hpp"

class NameObjArchiveListCollector;
class JMapInfoIter;

class Caretaker : public NPCActor {
public:
    Caretaker(const char*);
    virtual ~Caretaker();
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

private:
    u8 mPad[(0x17C) - sizeof(NPCActor)];
};
