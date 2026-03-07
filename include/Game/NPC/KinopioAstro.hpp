#pragma once

#include "Game/NameObj/NameObj.hpp"

class NameObjArchiveListCollector;
class JMapInfoIter;

class KinopioAstro : public NameObj {
public:
    KinopioAstro(const char*);
    virtual ~KinopioAstro();
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

private:
    u8 mPad[(0x1A0) - sizeof(NameObj)];
};
