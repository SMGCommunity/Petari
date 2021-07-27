#pragma once

#include "JSystem/JSupport/JSUList.h"

class JKRHeap;

class JKRDisposer
{
public:
    JKRDisposer();

    virtual ~JKRDisposer();

    JKRHeap* mRootHeap; // _4
    JSULink<JKRDisposer> mPointerLinks; // _8
};