#pragma once

#include "JSystem/JSupport/JSUList.h"

class JKRHeap;

class JKRDisposer {
public:
    JKRDisposer();
    virtual ~JKRDisposer();

    JKRHeap *mHeap;                 // _4
    JSULink<JKRDisposer> mLink;     // _8
};