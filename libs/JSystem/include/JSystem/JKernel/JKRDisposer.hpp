#pragma once

#include "JSystem/JSupport/JSUList.hpp"

class JKRHeap;

class JKRDisposer {
public:
    JKRDisposer();
    virtual ~JKRDisposer();

    JKRHeap* mHeap;                // 0x4
    JSULink< JKRDisposer > mLink;  // 0x8
};