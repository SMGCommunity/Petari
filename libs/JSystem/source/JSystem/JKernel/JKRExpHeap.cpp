#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JUtility/JUTConsole.h"
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

    heap->mAllocMode = 1;
    return heap;
}

void JKRExpHeap::do_destroy() {
    if (_6E) {
        JKRHeap* heap = mChildTree.getParent()->getObject();

        if (heap != nullptr) {
            this->~JKRExpHeap();
            JKRHeap::free(this, heap);
        }
    }
    else {
        this->~JKRExpHeap();
    }
}

void* JKRExpHeap::do_alloc(u32 size, int align) {
    void* ptr;
    OSLockMutex(&mMutex);

    if (size < 4) {
        size = 4;
    }

    if (align >= 0) {
        if (align <= 4) {
            ptr = allocFromHead(size);
        }
        else {
            ptr = allocFromHead(size, align);
        }
    }
    else {
        if (-align <= 4) {
            ptr = allocFromTail(size);
        }
        else {
            ptr = allocFromTail(size, -align);
        }
    }

    if (ptr == nullptr) {
        JUTWarningConsole_f(":::cannot alloc memory (0x%x byte).\n", size);

        if (JKRHeap::mErrorFlag == true) {
            if (JKRHeap::mErrorHandler) {
                (*JKRHeap::mErrorHandler)(this, size, align);
            }
        }
    }

    OSUnlockMutex(&mMutex);
    return ptr;
}

// JKRExpHeap::allocFromHead
// JKRExpHeap::allocFromTail



JKRExpHeap::JKRExpHeap(void* data, u32 size, JKRHeap* parent, bool error)
    : JKRHeap(data, size, parent, error) {

    CMemBlock* block = (CMemBlock*)data;

    _6A = 0;
    _6B = 0xFF;
    mHeadFreeList = block;
    mTailFreeList = block;
    block->initiate(nullptr, nullptr, size - sizeof(CMemBlock), 0, 0);
    mHeadUsedList = nullptr;
    mTailUsedList = nullptr;
}

void JKRExpHeap::CMemBlock::initiate(CMemBlock* prev, CMemBlock* next, u32 size, u8 groupID, u8 align) {
    mMagic = 'HM';
    mFlags = align;
    mGroupId = groupID;
    mSize = size;
    mPrev = prev;
    mNext = next;
}

JKRExpHeap::CMemBlock* JKRExpHeap::CMemBlock::allocFore(u32 size, u8 group_1, u8 align_1, u8 group_2, u8 align_2) {
    CMemBlock* block = nullptr;
    mGroupId = group_1;
    mFlags = align_1;

    if (mSize >= size + sizeof(CMemBlock)) {
        block = (CMemBlock*)((u32)this + size);
        block[1].mGroupId = group_2;
        block[1].mFlags = align_2;
        block[1].mSize = mSize - (size + sizeof(CMemBlock));
        mSize = size;
        block++;
    }

    return block;
}

JKRExpHeap::CMemBlock* JKRExpHeap::CMemBlock::allocBack(u32 size, u8 group_1, u8 align_1, u8 group_2, u8 align_2) {
    CMemBlock* block = nullptr;

    if (mSize >= size + sizeof(CMemBlock)) {
        block = (CMemBlock*)((u32)this + mSize - size);
        block->mGroupId = group_2;
        block->mFlags = align_2 | 0x80;
        block->mSize = size;
        mGroupId = group_1;
        mFlags = align_1;
        mSize -= size + sizeof(CMemBlock);
    }
    else {
        mGroupId = group_2;
        mFlags = 0x80;
    }

    return block;
}

JKRExpHeap::CMemBlock* JKRExpHeap::CMemBlock::getHeapBlock(void* ptr) {
    if (ptr != nullptr) {
        CMemBlock* block = (CMemBlock*)ptr - 1;

        if (block->mMagic == 'HM') {
            return block;
        }
    }

    return nullptr;
}