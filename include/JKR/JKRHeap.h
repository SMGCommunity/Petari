#ifndef JKRHEAP_H
#define JKRHEAP_H

#include "JSU/JSUList.h"

class JKRHeap : public JKRDisposer
{
public:
    JKRHeap(void *, u32, JKRHeap *, bool);

    static JKRHeap* findFromRoot(void *);
    static void* setErrorHandler(void (*)(void *, u32, s32));
    
    u8 _C[0x5C-0xC];
    JSUPtrList mPtrList; // _5C

    static JKRHeap* sSystemHeap;
    static JKRHeap* sCurrentHeap;
    static JKRHeap* sRootHeap;
    static void* sErrorHandler;
};

void JKRDefaultMemoryErrorRoutine(void *, u32, s32);

#endif // JKRHEAP_H