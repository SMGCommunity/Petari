#pragma once

#include <revolution.h>

class NameObj;

typedef void (NameObj::*func)(const void *, void *);

class NameObjHolder {
public:
    NameObjHolder(int);

    void add(NameObj *);
    void suspendAllObj();
    void resumeAllObj();
    void syncWithFlags();
    void callMethodAllObj(func);
    void clearArray();

    NameObj** mObjs; // _0
    u32 _4;
    u32 mObjCount; // _8
    u8 _C[0x4C-0xC];
    u32 _4C;
};