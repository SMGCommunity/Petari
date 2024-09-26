#pragma once

#include <revolution.h>

class NameObj;

typedef void (NameObj::*func)(void);
typedef void (NameObj::*func_const)(void) const;

class NameObjHolder {
public:
    NameObjHolder(int);

    void add(NameObj *);
    void suspendAllObj();
    void resumeAllObj();
    void syncWithFlags();
    void callMethodAllObj(func);
    void clearArray();

    NameObj* find(const char *);

    NameObj** mObjs; // 0x0
    u32 _4;
    u32 mObjCount; // 0x8
    u8 _C[0x4C-0xC];
    u32 _4C;
};