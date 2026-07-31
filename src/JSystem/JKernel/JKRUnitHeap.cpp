#include "JSystem/JKernel/JKRUnitHeap.hpp"

JKRUnitHeap::~JKRUnitHeap() {
    dispose();
}

s32 JKRUnitHeap::findFreeBlock(int idx, u32 a2) {
    if (a2 == 1) {
        return find1FreeBlock(idx);
    }

    if (idx >= 0) {
        return findFreeBlock_fromHead(a2);
    }

    return findFreeBlock_fromTail(a2);
}

void JKRUnitHeap::do_freeAll() {
    clearBatArea();
    mTotalFreeSize = _6C * _70;
}

void JKRUnitHeap::do_freeTail() {
    return;
}

void JKRUnitHeap::do_fillFreeArea() {
    return;
}

s32 JKRUnitHeap::do_resize(void*, u32) {
    return -1;
}

s32 JKRUnitHeap::do_getSize(void* pAddr) {
    s32 idx = addressToIndex(pAddr);

    if (isUnitUsed(idx)) {
        return _6C;
    }

    return 0;
}

s32 JKRUnitHeap::do_getFreeSize() {
    return _6C * findFreeBlock_fromHead(0);
}

void* JKRUnitHeap::do_getMaxFreeBlock() {
    return indexToAddress(findFreeBlock_fromHead(0));
}

s32 JKRUnitHeap::do_getTotalFreeSize() {
    return mTotalFreeSize;
}

void* JKRUnitHeap::indexToAddress(int idx) {
    if ((idx & 0x80000000) != 0 || idx >= _70) {
        return nullptr;
    }

    return &_7C[_6C * idx];
}

u32 JKRUnitHeap::getHeapType() {
    return 0x554E4954;
}
