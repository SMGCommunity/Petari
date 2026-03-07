#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class NameObjArchiveListCollector;
class JMapInfoIter;

class KoopaJrShip : public LiveActor {
public:
    KoopaJrShip(const char*);
    virtual ~KoopaJrShip();
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

private:
    u8 mPad[(0x204) - sizeof(LiveActor)];
};
