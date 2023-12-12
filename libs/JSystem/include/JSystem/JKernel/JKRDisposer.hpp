#pragma once

#include "JSystem/JSupport/JSUList.hpp"

class JKRHeap;

class JKRDisposer {
public:
    JKRDisposer();
    virtual ~JKRDisposer();

    JKRHeap *mHeap;                 // _4
    JSULink<JKRDisposer> mLink;     // _8
};