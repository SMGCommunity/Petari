#pragma once

#include "types.h"

class JSUPtrList;

class JSUPtrLink
{
public:
    JSUPtrLink(void *);
    ~JSUPtrLink();

    void* mPtr; // _0
    JSUPtrList* mPtrList; // _4
    u32 _8;
    u32 _C;
};