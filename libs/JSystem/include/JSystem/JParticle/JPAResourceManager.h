#pragma once

#include <revolution.h>

class JKRHeap;

class JPAResourceManager {
public:
    JPAResourceManager(const void *, JKRHeap *);

    JKRHeap* mHeap; // _0
    u32 _4;
    u32 _8;
    u16 _C;
    u16 _E;
    u16 _10;
    u16 _12;
};