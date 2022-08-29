#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>

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

#ifdef __cplusplus
}
#endif

#endif // ALLOCATOR_H