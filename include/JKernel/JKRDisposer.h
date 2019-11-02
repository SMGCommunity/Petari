#ifndef JKRDISPOSER_H
#define JKRDISPOSER_H

#include "JSupport/JSUList.h"

class JKRHeap;

class JKRDisposer
{
public:
    JKRDisposer();

    virtual ~JKRDisposer();

    JKRHeap* mRootHeap; // _4
    JSULink<JKRDisposer> mPointerLinks; // _8
};

#endif // JKRDISPOSER_H