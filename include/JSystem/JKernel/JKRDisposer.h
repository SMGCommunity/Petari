#pragma once

#include "JSystem/JSupport/JSUList.h"

class JKRHeap;

class JKRDisposer {
public:
    JKRDisposer();
    virtual ~JKRDisposer();

    virtual void unmount() = 0;

    JKRHeap* mHeap;                 // _4
    JSULink<JKRDisposer> mLink;     // _8
};