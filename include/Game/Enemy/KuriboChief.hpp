#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class NameObjArchiveListCollector;
class JMapInfoIter;

class KuriboChief : public LiveActor {
public:
    KuriboChief(const char*);
    virtual ~KuriboChief();
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

private:
    u8 mPad[(0xC8) - sizeof(LiveActor)];
};
