#include "JSystem/JKernel/JKRAramHeap.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"

JSUList< JKRAramBlock > JKRAramHeap::sAramList;

JKRAramHeap::JKRAramHeap(u32 startAddress, u32 size) {
    OSInitMutex(&mMutex);

    mHeap = JKRHeap::findFromRoot(this);
    mSize = ALIGN_PREV(size, 0x20);
    mHeadAddress = ALIGN_NEXT(startAddress, 0x20);
    mTailAddress = mHeadAddress + mSize;
    mGroupId = -1;

    JKRAramBlock* block = new (mHeap, 0) JKRAramBlock(mHeadAddress, 0, mSize, -1, false);
    sAramList.append(&block->mBlockLink);
}

JKRAramHeap::~JKRAramHeap() {
    for (JSUListIterator< JKRAramBlock > iterator = sAramList.getFirst(); iterator != sAramList.getEnd(); delete (iterator++).getObject()) {
    }
}

JKRAramBlock* JKRAramHeap::alloc(u32 size, JKRAramHeap::EAllocMode allocationMode) {
    lock();

    JKRAramBlock* block;
    if (allocationMode == JKRAramHeap::HEAD) {
        block = allocFromHead(size);
    } else {
        block = allocFromTail(size);
    }

    unlock();
    return block;
}

JKRAramBlock* JKRAramHeap::allocFromHead(u32 size) {
    u32 alignedSize = ALIGN_NEXT(size, 0x20);
    u32 bestFreeSize = -1;
    JKRAramBlock* bestBlock = NULL;

    for (JSUListIterator< JKRAramBlock > iterator = sAramList.getFirst(); iterator != sAramList.getEnd(); ++iterator) {
        JKRAramBlock* block = iterator.getObject();
        if (block->mFreeSize < alignedSize) {
            continue;
        }
        if (bestFreeSize <= block->mFreeSize) {
            continue;
        }

        bestFreeSize = block->mFreeSize;
        bestBlock = block;

        if (bestFreeSize == alignedSize) {
            break;
        }
    }

    if (bestBlock) {
        return bestBlock->allocHead(alignedSize, mGroupId, this);
    }

    return NULL;
}

JKRAramBlock* JKRAramHeap::allocFromTail(u32 size) {
    u32 alignedSize = ALIGN_NEXT(size, 0x20);
    JKRAramBlock* tailBlock = NULL;

    for (JSUListIterator< JKRAramBlock > iterator = sAramList.getLast(); iterator != sAramList.getEnd(); --iterator) {
        JKRAramBlock* block = iterator.getObject();
        if (block->mFreeSize >= alignedSize) {
            tailBlock = block;
            break;
        }
    }

    if (tailBlock) {
        return tailBlock->allocTail(alignedSize, mGroupId, this);
    }

    return NULL;
}
