#ifndef JKRHEAP_H
#define JKRHEAP_H

#include "JSU/JSUList.h"

class JKRHeap
{
public:
    static JKRHeap* findFromRoot(void *);
    
    u8 _0[0x5C];
    JSUPtrList mPtrList; // _5C
};

#endif // JKRHEAP_H