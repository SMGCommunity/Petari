#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class NameObjArchiveListCollector;
class JMapInfoIter;

class Sandstorm : public LiveActor {
public:
    Sandstorm(const char*);
    virtual ~Sandstorm();
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

private:
    u8 mPad[(0xF4) - sizeof(LiveActor)];
};
