#include "JSystem/JKernel/JKRExpHeap.h"
#include <new>

JKRExpHeap* JKRExpHeap::createRoot(int heapNum, bool a2) {
    JKRExpHeap* heap = nullptr;

    if (!JKRHeap::sRootHeap) {
        char* stack_C;
        u32 arenaSize;
        JKRHeap::initArena(&stack_C, &arenaSize, heapNum);
        char* area = stack_C + 0x90;
        u32 size = arenaSize - 0x90;
        heap = new(stack_C)JKRExpHeap(area, size, nullptr, a2);
        JKRHeap::sRootHeap = heap;
    }

    heap->_6C = 1;
    return heap;
}