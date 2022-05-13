#pragma once

#include<revolution.h>
#include "JSystem/JSupport/JSUList.h"
#include "JSystem/JKernel/JKRDisposer.h"

// this inherits JKRDisposer
class JKRHeap {
public:

    void* alloc(u32, int);
    void becomeCurrentHeap();

    void becomeSystemHeap();

    void free(void *);

    static JKRHeap* findFromRoot(void *);

    static void* alloc(u32, int, JKRHeap *);
    static void free(void *, JKRHeap *);

    static void destroy(JKRHeap *);

    static void setAltAramStartAdr(u32);

    static void setErrorHandler(void (*)(void *, u32, int));

    static JKRHeap *sGameHeap; // _806B70A8
    static JKRHeap *sCurrentHeap; // _806B70AC
    static JKRHeap *sRootHeap; // _806B70B0
    static JKRHeap *sSystemHeap;

    u8 _0[0x5C];
    JSUList<JKRDisposer> mDisposerList; // _5C
    u8 _68;
    u8 _69;
    u8 _6A;
    u8 _6B;
};

void* operator new(u32, JKRHeap *, int);
void* operator new[](u32, int);

void* operator new[](u32, JKRHeap *, int);