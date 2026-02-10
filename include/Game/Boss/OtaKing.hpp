#pragma once

#include "Game/NameObj/NameObj.hpp"

class NameObjArchiveListCollector;
class JMapInfoIter;

class OtaKing : public NameObj {
public:
    OtaKing(const char*);
    virtual ~OtaKing();
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

private:
    u8 mPad[(0x158) - sizeof(NameObj)];
};
