#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class NameObjArchiveListCollector;
class JMapInfoIter;

class StarReturnDemoStarter : public LiveActor {
public:
    StarReturnDemoStarter(const char*);
    virtual ~StarReturnDemoStarter();
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

private:
    u8 mPad[(0xE0) - sizeof(LiveActor)];
};
