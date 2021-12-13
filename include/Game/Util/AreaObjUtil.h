#pragma once

#include "JSystem/JGeometry/TVec.h"

class AreaObj;
class AreaObjMgr;

namespace MR {
    AreaObjMgr* getAreaObjManager(const char *);
    AreaObj* getAreaObj(const char *, const TVec3f &);
    bool isInAreaObj(const char *, const TVec3f &);
    s32 getAreaObjArg(const AreaObj *, s32);
};
