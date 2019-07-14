#pragma once

#include "types.h"
#include "actor/nameobj/NameObj.h"

class NameObjHolder
{
public:
    NameObjHolder(s32);

    void add(NameObj *);
    void suspendAllObj();
    void resumeAllObj();
    void syncWithFlags();
    void callMethodAllObj();
    void clearArray();
    NameObj* find(const char *);

    NameObj** mObjs; // _0
    s32 _4;
    u32 _8;
    u8 _C[0x4C-0x0C];
    u32 _4C;
};