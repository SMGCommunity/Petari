#ifndef EXPHEAP_H
#define EXPHEAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/mem/heapCommon.h>

typedef struct MEMiExpHeapMBlockHead MEMiExpHeapMBlockHead;

typedef struct MEMiExpHeapMBlockHead {
    u16 signature;

    union {
        u16 val;

        struct {
            u16 allocDir : 1;
            u16 alignment : 7;
            u16 groupID : 8;
        } fields;
    } attribute;

    u32 blockSize;
    MEMiExpHeapMBlockHead* prev;
    MEMiExpHeapMBlockHead* next;
} MEMiExPheapMBlockHead;

typedef struct MEMiExpMBlockList MEMiExpMBlockList;

struct MEMiExpMBlockList {
    MEMiExpHeapMBlockHead* head;
    MEMiExpHeapMBlockHead* tail;
};

typedef struct MEMiExpHeapHead MEMiExpHeapHead;

struct MEMiExpHeapHead {
    MEMiExpMBlockList mbFreeList;
    MEMiExpMBlockList mbUsedList;

    u16 groupID;

    union {
        u16 val;
        struct {
            u16 _reserved : 14;
            u16 useMarginOfAlign : 1;
            u16 allocMode : 1;
        } fields;
    } feature;
};

MEMHeapHandle MEMCreateExpHeapEx(void*, u32, u16);
void* MEMDestroyExpHeap(MEMHeapHandle);
void* MEMAllocFromExpHeapEx(MEMHeapHandle, u32, int);
void MEMFreeToExpHeap(MEMHeapHandle, void*);
u32 MEMGetAllocatableSizeForExpHeapEx(MEMHeapHandle, int);

#ifdef __cplusplus
}
#endif

#endif  // EXPHEAP_H
