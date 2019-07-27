#ifndef JKRDISPOSER_H
#define JKRDISPOSER_H

#include "jsu/JSUPtrLink.h"

class JKRHeap;

class JKRDisposer
{
public:
    JKRDisposer();

    virtual ~JKRDisposer();

    JKRHeap* mRootHeap; // _4
    JSUPtrLink mPointerLinks; // _8
};

#endif // JKRDISPOSER_H