#pragma once

#include "Game/NameObj/NameObj.h"
#include "JSystem/JGeometry/TVec.h"

class AreaObj;
class AreaObjMgr;

struct AreaObjEntry {
    const char* mName;                          // _0
    int _4;                                     // _4
    AreaObjMgr* (*mFuncPtr)(s32, const char *); // _8
};

namespace {
    const AreaObjEntry* cCreateTable;
};

class AreaObjContainer : public NameObj {
public:
    AreaObjContainer(const char *);

    virtual void init(const JMapInfoIter &);

    AreaObjMgr* getManager(const char *) const;
    AreaObj* getAreaObj(const char *, const TVec3f &) const;
};