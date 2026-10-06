#pragma once

#include "JSystem/JSupport/JSUList.hpp"

class JKRHeap;

class JKRDisposer {
public:
    JKRDisposer();
    virtual ~JKRDisposer();

    /* 0x4 */ JKRHeap* mHeap;
    /* 0x8 */ JSULink< JKRDisposer > mLink;
};