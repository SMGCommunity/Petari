#pragma once

#include<revolution.h>

// this inherits JKRDisposer
class JKRHeap {
public:

    void* alloc(u32, int);
    void becomeCurrentHeap();

    void free(void *);

    static JKRHeap* sCurrentHeap;
    static JKRHeap* sRootHeap;
};

void* operator new(u32, JKRHeap *, int);