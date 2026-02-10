#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class NameObjArchiveListCollector;
class JMapInfoIter;

class GhostPlayer : public LiveActor {
public:
    GhostPlayer(const char*);
    virtual ~GhostPlayer();
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

private:
    u8 mPad[(0x134) - sizeof(LiveActor)];
};
