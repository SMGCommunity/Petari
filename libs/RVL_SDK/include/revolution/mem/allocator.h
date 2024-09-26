#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>
#include <revolution/mem/heapCommon.h>

typedef struct MEMAllocator MEMAllocator;
typedef void* (*MEMFuncAllocatorAlloc)( MEMAllocator* pAllocator, u32 size );
typedef void  (*MEMFuncAllocatorFree) ( MEMAllocator* pAllocator, void* memBlock );
typedef struct MEMAllocatorFunc MEMAllocatorFunc;

struct MEMAllocatorFunc {
    MEMFuncAllocatorAlloc pfAlloc;
    MEMFuncAllocatorFree pfFree;
};

struct MEMAllocator {
    const MEMAllocatorFunc* pFunc;
    void* pHeap;
    u32 heapParam1;
    u32 heapParam2;
};

void* MEMAllocFromAllocator(MEMAllocator *, u32);
void MEMFreeToAllocator(MEMAllocator *, void *);
void MEMInitAllocatorForExpHeap(MEMAllocator *, MEMHeapHandle, int);


#ifdef __cplusplus
}
#endif

#endif // ALLOCATOR_H