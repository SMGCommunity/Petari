#include "JSystem/JKernel/JKRExpHeap.hpp"
#include "JSystem/JSupport/JSupport.hpp"
#include "JSystem/JUtility/JUTConsole.hpp"
#include "JSystem/JUtility/JUTException.hpp"
#include <new>

static u32 DBfoundSize;
static u32 DBfoundOffset;
static JKRExpHeap::CMemBlock* DBfoundBlock;
static JKRExpHeap::CMemBlock* DBnewFreeBlock;
static JKRExpHeap::CMemBlock* DBnewUsedBlock;

JKRExpHeap* JKRExpHeap::createRoot(int heapNum, bool a2) {
    JKRExpHeap* heap = nullptr;

    if (!JKRHeap::sRootHeap) {
        char* stack_C;
        u32 arenaSize;
        JKRHeap::initArena(&stack_C, &arenaSize, heapNum);
        char* area = stack_C + 0x90;
        u32 size = arenaSize - 0x90;
        heap = new (stack_C) JKRExpHeap(area, size, nullptr, a2);
        JKRHeap::sRootHeap = heap;
    }

    heap->mAllocMode = 1;
    return heap;
}

JKRExpHeap* JKRExpHeap::create(u32 size, JKRHeap* pParent, bool errorFlag) {
    if (!pParent) {
        pParent = JKRHeap::sRootHeap;
    }

    if (size == 0xFFFFFFFF) {
        size = pParent->getMaxAllocatableSize(0x10);
    }

    u32 alignedSize = ALIGN_PREV(size, 0x10);
    u32 heapSize = ALIGN_NEXT(sizeof(JKRExpHeap), 0x10);

    if (alignedSize < 0xA0) {
        return nullptr;
    }

    u8* mem = (u8*)JKRHeap::alloc(alignedSize, 16, pParent);
    u8* data = (mem + heapSize);
    if (mem == nullptr) {
        return nullptr;
    }

    JKRExpHeap* heap = new (mem) JKRExpHeap(data, alignedSize - heapSize, pParent, errorFlag);

    if (heap == nullptr) {
        JKRHeap::free(mem, nullptr);
        return nullptr;
    }

    heap->mAllocMode = 0;
    return heap;
}

JKRExpHeap* JKRExpHeap::create(void* ptr, u32 size, JKRHeap* pParent, bool errorFlag) {
    JKRHeap* parent;

    if (pParent == nullptr) {
        parent = sRootHeap->find(ptr);

        if (parent == nullptr) {
            return nullptr;
        }
    } else {
        parent = pParent;
    }

    JKRExpHeap* heap = nullptr;
    u32 heapSize = ALIGN_NEXT(sizeof(JKRExpHeap), 0x10);

    if (size < heapSize) {
        return nullptr;
    }

    void* data = (u8*)ptr + heapSize;
    u32 alignSize = ALIGN_PREV((u32)ptr + size - (u32)data, 0x10);
    if (ptr != nullptr) {
        heap = new (ptr) JKRExpHeap(data, alignSize, parent, errorFlag);
    }

    heap->mAllocMode = 1;
    heap->_70 = ptr;
    heap->_74 = size;
    return heap;
}

void JKRExpHeap::do_destroy() {
    if (_6E) {
        JKRHeap* heap = mChildTree.getParent()->getObject();

        if (heap != nullptr) {
            this->~JKRExpHeap();
            JKRHeap::free(this, heap);
        }
    } else {
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
        } else {
            ptr = allocFromHead(size, align);
        }
    } else {
        if (-align <= 4) {
            ptr = allocFromTail(size);
        } else {
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

void* JKRExpHeap::allocFromHead(u32 size, int align) {
    u32 foundOffset;
    int foundSize;

    size = ALIGN_NEXT(size, 4);
    foundSize = -1;
    foundOffset = 0;
    CMemBlock* foundBlock = NULL;
    CMemBlock* newFreeBlock = NULL;
    CMemBlock* newUsedBlock = NULL;

    for (CMemBlock* block = mHeadFreeList; block; block = block->mNext) {
        u32 offset = ALIGN_PREV(align - 1 + (uintptr_t)block->getContent(), align) - (uintptr_t)block->getContent();
        if (block->mSize < size + offset) {
            continue;
        }

        if (foundSize <= (u32)block->mSize) {
            continue;
        }

        foundSize = block->mSize;
        foundBlock = block;
        foundOffset = offset;
        if (mAllocMode != 0) {
            break;
        }

        if (foundSize == size) {
            break;
        }
    }

    DBfoundSize = foundSize;
    DBfoundOffset = foundOffset;
    DBfoundBlock = foundBlock;

    if (foundBlock) {
        if (foundOffset >= sizeof(CMemBlock)) {
            CMemBlock* prev = foundBlock->mPrev;
            CMemBlock* next = foundBlock->mNext;
            newUsedBlock = foundBlock->allocFore(foundOffset - sizeof(CMemBlock), 0, 0, 0, 0);

            if (newUsedBlock) {
                newFreeBlock = newUsedBlock->allocFore(size, mCurrentGroupId, 0, 0, 0);
            } else {
                newFreeBlock = NULL;
            }

            if (newFreeBlock) {
                setFreeBlock(foundBlock, prev, newFreeBlock);
            } else {
                setFreeBlock(foundBlock, prev, next);
            }

            if (newFreeBlock) {
                setFreeBlock(newFreeBlock, foundBlock, next);
            }

            appendUsedList(newUsedBlock);
            DBnewFreeBlock = newFreeBlock;
            DBnewUsedBlock = newUsedBlock;
            return newUsedBlock->getContent();
        } else {
            if (foundOffset != 0) {
                CMemBlock* prev = foundBlock->mPrev;
                CMemBlock* next = foundBlock->mNext;
                removeFreeBlock(foundBlock);
                newUsedBlock = (CMemBlock*)((uintptr_t)foundBlock + foundOffset);
                newUsedBlock->mSize = foundBlock->mSize - foundOffset;
                newFreeBlock = newUsedBlock->allocFore(size, mCurrentGroupId, (u8)foundOffset, 0, 0);
                if (newFreeBlock) {
                    setFreeBlock(newFreeBlock, prev, next);
                }
                appendUsedList(newUsedBlock);
                return newUsedBlock->getContent();
            } else {
                CMemBlock* prev = foundBlock->mPrev;
                CMemBlock* next = foundBlock->mNext;

                newFreeBlock = foundBlock->allocFore(size, mCurrentGroupId, 0, 0, 0);
                removeFreeBlock(foundBlock);
                if (newFreeBlock) {
                    setFreeBlock(newFreeBlock, prev, next);
                }
                appendUsedList(foundBlock);
                return foundBlock->getContent();
            }
        }
    }

    return NULL;
}

void* JKRExpHeap::allocFromHead(u32 size) {
    size = ALIGN_NEXT(size, 4);
    s32 foundSize = -1;
    CMemBlock* foundBlock = NULL;
    CMemBlock* newblock = NULL;
    for (CMemBlock* block = mHeadFreeList; block; block = block->mNext) {
        if (block->mSize < size) {
            continue;
        }

        if (foundSize <= block->mSize) {
            continue;
        }

        foundSize = block->mSize;
        foundBlock = block;
        if (mAllocMode != 0) {
            break;
        }

        if (foundSize == size) {
            break;
        }
    }

    if (foundBlock) {
        newblock = foundBlock->allocFore(size, mCurrentGroupId, 0, 0, 0);
        if (newblock) {
            setFreeBlock(newblock, foundBlock->mPrev, foundBlock->mNext);
        } else {
            removeFreeBlock(foundBlock);
        }
        appendUsedList(foundBlock);
        return foundBlock->getContent();
    }
    return NULL;
}

void* JKRExpHeap::allocFromTail(u32 size, int align) {
    u32 local_2c = 0;
    u32 offset = 0;
    CMemBlock* foundBlock = NULL;
    CMemBlock* newBlock = NULL;
    u32 usedSize;
    u32 start;

    for (CMemBlock* block = mTailFreeList; block; block = block->mPrev) {
        start = ALIGN_PREV((uintptr_t)block->getContent() + block->mSize - size, align);
        usedSize = (uintptr_t)block->getContent() + block->mSize - start;
        if (block->mSize >= usedSize) {
            local_2c = usedSize;
            foundBlock = block;
            offset = block->mSize - usedSize;
            newBlock = (CMemBlock*)start - 1;
            break;
        }
    }

    if (foundBlock != NULL) {
        if (offset >= sizeof(CMemBlock)) {
            newBlock->initiate(NULL, NULL, usedSize, mCurrentGroupId, -0x80);
            foundBlock->mSize = foundBlock->mSize - usedSize - sizeof(CMemBlock);
            appendUsedList(newBlock);
            return newBlock->getContent();
        } else {
            if (offset != 0) {
                removeFreeBlock(foundBlock);
                newBlock->initiate(NULL, NULL, usedSize, mCurrentGroupId, offset | 0x80);
                appendUsedList(newBlock);
                return newBlock->getContent();
            } else {
                removeFreeBlock(foundBlock);
                newBlock->initiate(NULL, NULL, usedSize, mCurrentGroupId, -0x80);
                appendUsedList(newBlock);
                return newBlock->getContent();
            }
        }
    }

    return NULL;
}

void* JKRExpHeap::allocFromTail(u32 size) {
    size = ALIGN_NEXT(size, 4);
    CMemBlock* foundBlock = NULL;
    CMemBlock* freeBlock = NULL;
    CMemBlock* usedBlock = NULL;
    for (CMemBlock* block = mTailFreeList; block; block = block->mPrev) {
        if (block->mSize >= size) {
            foundBlock = block;
            break;
        }
    }

    if (foundBlock != NULL) {
        usedBlock = foundBlock->allocBack(size, 0, 0, mCurrentGroupId, 0);
        if (usedBlock) {
            freeBlock = foundBlock;
        } else {
            removeFreeBlock(foundBlock);
            usedBlock = foundBlock;
            freeBlock = NULL;
        }

        if (freeBlock) {
            setFreeBlock(freeBlock, foundBlock->mPrev, foundBlock->mNext);
        }
        appendUsedList(usedBlock);
        return usedBlock->getContent();
    }
    return NULL;
}

int JKRExpHeap::CMemBlock::free(JKRExpHeap* heap) {
    heap->removeUsedBlock(this);
    heap->recycleFreeBlock(this);
    return 0;
}

void JKRExpHeap::do_free(void* ptr) {
    lock();
    if (mStart <= ptr && ptr <= mEnd) {
        CMemBlock* block = CMemBlock::getHeapBlock(ptr);
        if (block) {
            block->free(this);
        }
    } else {
    }
    unlock();
}

void JKRExpHeap::do_freeAll() {
    lock();
    JKRHeap::callAllDisposer();
    mHeadFreeList = (CMemBlock*)mStart;
    mTailFreeList = mHeadFreeList;
    mHeadFreeList->initiate(NULL, NULL, mSize - sizeof(CMemBlock), 0, 0);
    mHeadUsedList = NULL;
    mTailUsedList = NULL;
    unlock();
}

void JKRExpHeap::do_freeTail() {
    lock();
    for (CMemBlock* block = mHeadUsedList; block != NULL;) {
        if (block->isTempMemBlock()) {
            dispose(block->getContent(), block->mSize);
            CMemBlock* temp = block->mNext;
            block->free(this);
            block = temp;
        } else {
            block = block->mNext;
        }
    }
    unlock();
}
void JKRExpHeap::do_fillFreeArea() {
}

s32 JKRExpHeap::do_changeGroupID(u8 param_0) {
    lock();
    u8 prev = mCurrentGroupId;
    mCurrentGroupId = param_0;
    unlock();
    return prev;
}

s32 JKRExpHeap::do_resize(void* ptr, u32 size) {
    lock();
    CMemBlock* block = CMemBlock::getHeapBlock(ptr);
    if (block == NULL || ptr < mStart || mEnd < ptr) {
        unlock();
        return -1;
    }

    size = ALIGN_NEXT(size, 4);
    if (size == block->mSize) {
        unlock();
        return size;
    }

    if (size > block->mSize) {
        CMemBlock* foundBlock = NULL;
        for (CMemBlock* freeBlock = mHeadFreeList; freeBlock; freeBlock = freeBlock->mNext) {
            if (freeBlock == (CMemBlock*)((uintptr_t)(block + 1) + block->mSize)) {
                foundBlock = freeBlock;
                break;
            }
        }

        if (foundBlock == NULL) {
            unlock();
            return -1;
        }

        if (size > block->mSize + sizeof(CMemBlock) + foundBlock->mSize) {
            unlock();
            return -1;
        }

        u32 local_24 = block->mSize;
        removeFreeBlock(foundBlock);
        block->mSize += foundBlock->mSize + sizeof(CMemBlock);
        if (block->mSize - size > sizeof(CMemBlock)) {
            CMemBlock* newBlock = block->allocFore(size, block->mGroupId, block->mFlags, 0, 0);
            if (newBlock) {
                recycleFreeBlock(newBlock);
            }
        }
    } else {
        if (block->mSize - size > sizeof(CMemBlock)) {
            CMemBlock* freeBlock = block->allocFore(size, block->mGroupId, block->mFlags, 0, 0);
            if (freeBlock) {
                recycleFreeBlock(freeBlock);
            }
        }
    }

    unlock();
    return block->mSize;
}

s32 JKRExpHeap::do_getSize(void* ptr) {
    lock();
    CMemBlock* block = CMemBlock::getHeapBlock(ptr);
    if (!block || ptr < mStart || mEnd < ptr) {
        unlock();
        return -1;
    }
    unlock();
    return block->mSize;
}

s32 JKRExpHeap::do_getFreeSize() {
    lock();
    s32 size = 0;
    for (CMemBlock* block = mHeadFreeList; block; block = block->mNext) {
        if (size < (s32)block->mSize) {
            size = block->mSize;
        }
    }
    unlock();
    return size;
}

void* JKRExpHeap::do_getMaxFreeBlock() {
    lock();
    s32 size = 0;
    CMemBlock* res = NULL;
    for (CMemBlock* block = mHeadFreeList; block; block = block->mNext) {
        if (size < (s32)block->mSize) {
            size = block->mSize;
            res = block;
        }
    }
    unlock();
    return res;
}

s32 JKRExpHeap::do_getTotalFreeSize() {
    u32 size = 0;
    lock();
    for (CMemBlock* block = mHeadFreeList; block; block = block->mNext) {
        size += block->mSize;
    }
    unlock();
    return size;
}

s32 JKRExpHeap::getUsedSize(u8 groupId) const {
    lock();
    u32 size = 0;

    for (CMemBlock* block = mHeadUsedList; block; block = block->mNext) {
        if (block->mGroupId == groupId) {
            size += block->mSize + sizeof(CMemBlock);
        }
    }

    unlock();
    return size;
}

bool JKRExpHeap::isEmpty() {
    lock();
    bool result = !mHeadUsedList ? true : false;
    unlock();
    return result;
}

void JKRExpHeap::appendUsedList(JKRExpHeap::CMemBlock* newblock) {
    if (!newblock) {
        JUTException::panic(__FILE__, 1568, "bad appendUsedList\n");
    }

    CMemBlock* block = mTailUsedList;
    newblock->mMagic = 'HM';

    if (block) {
        block->mNext = newblock;
        newblock->mPrev = block;
    } else {
        newblock->mPrev = NULL;
    }

    mTailUsedList = newblock;
    if (!mHeadUsedList) {
        mHeadUsedList = newblock;
    }
    newblock->mNext = NULL;
}

void JKRExpHeap::setFreeBlock(CMemBlock* block, CMemBlock* prev, CMemBlock* next) {
    if (prev == NULL) {
        mHeadFreeList = block;
        block->mPrev = NULL;
    } else {
        prev->mNext = block;
        block->mPrev = prev;
    }

    if (next == NULL) {
        mTailFreeList = block;
        block->mNext = NULL;
    } else {
        next->mPrev = block;
        block->mNext = next;
    }

    block->mMagic = 0;
}

void JKRExpHeap::removeFreeBlock(CMemBlock* block) {
    CMemBlock* prev = block->mPrev;
    CMemBlock* next = block->mNext;

    if (prev == NULL) {
        mHeadFreeList = next;
    } else {
        prev->mNext = next;
    }

    if (next == NULL) {
        mTailFreeList = prev;
    } else {
        next->mPrev = prev;
    }
}

void JKRExpHeap::removeUsedBlock(JKRExpHeap::CMemBlock* block) {
    CMemBlock* prev = block->mPrev;
    CMemBlock* next = block->mNext;

    if (prev == NULL) {
        mHeadUsedList = next;
    } else {
        prev->mNext = next;
    }

    if (next == NULL) {
        mTailUsedList = prev;
    } else {
        next->mPrev = prev;
    }
}

void JKRExpHeap::recycleFreeBlock(JKRExpHeap::CMemBlock* block) {
    JKRExpHeap::CMemBlock* newBlock = block;
    int size = block->mSize;
    void* blockEnd = (u8*)newBlock + size;
    block->mMagic = 0;

    if ((block->mFlags & 0x7f) != 0) {
        newBlock = (CMemBlock*)((u8*)newBlock - (block->mFlags & 0x7f));
        size += (block->mFlags & 0x7f);
        blockEnd = (u8*)newBlock + size;
        newBlock->mGroupId = 0;
        newBlock->mFlags = 0;
        newBlock->mSize = size;
    }

    if (!mHeadFreeList) {
        newBlock->initiate(NULL, NULL, size, 0, 0);
        mHeadFreeList = newBlock;
        mTailFreeList = newBlock;
        setFreeBlock(newBlock, NULL, NULL);
        return;
    }

    if (mHeadFreeList >= blockEnd) {
        newBlock->initiate(NULL, NULL, size, 0, 0);
        setFreeBlock(newBlock, NULL, mHeadFreeList);
        joinTwoBlocks(newBlock);
        return;
    }

    if (mTailFreeList <= newBlock) {
        newBlock->initiate(NULL, NULL, size, 0, 0);
        setFreeBlock(newBlock, mTailFreeList, NULL);
        joinTwoBlocks(newBlock->mPrev);
        return;
    }

    for (CMemBlock* freeBlock = mHeadFreeList; freeBlock; freeBlock = freeBlock->mNext) {
        if (freeBlock >= newBlock || newBlock >= freeBlock->mNext) {
            continue;
        }
        newBlock->mNext = freeBlock->mNext;
        newBlock->mPrev = freeBlock;
        freeBlock->mNext = newBlock;
        newBlock->mNext->mPrev = newBlock;
        newBlock->mGroupId = 0;
        joinTwoBlocks(newBlock);
        joinTwoBlocks(freeBlock);
        return;
    }
}

void JKRExpHeap::joinTwoBlocks(CMemBlock* block) {
    u32 endAddr = (uintptr_t)(block + 1) + block->mSize;
    CMemBlock* next = block->mNext;
    u32 nextAddr = (uintptr_t)next - (next->mFlags & 0x7f);
    if (endAddr > nextAddr) {
        JUTWarningConsole_f(":::Heap may be broken. (block = %x)", block);
        JKRHeap* heap = JKRGetCurrentHeap();
        heap->dump();
        JUTException::panic(__FILE__, 1820, "Bad Block\n");
    }

    if (endAddr == nextAddr) {
        block->mSize = next->mSize + sizeof(CMemBlock) + (next->mFlags & 0x7f) + block->mSize;
        CMemBlock* local_30 = next->mNext;
        setFreeBlock(block, block->mPrev, local_30);
    }
}

bool JKRExpHeap::check() {
    lock();
    int totalBytes = 0;
    bool ok = true;
    for (CMemBlock* block = mHeadUsedList; block; block = block->mNext) {
        if (block->mMagic != 'HM') {
            ok = false;
            JUTWarningConsole_f(":::addr %08x: bad heap signature. (%c%c)\n", block, JSUHiByte(block->mMagic), JSULoByte(block->mMagic));
        }

        if (block->mNext) {
            if (block->mNext->mMagic != 'HM') {
                ok = false;
                JUTWarningConsole_f(":::addr %08x: bad next pointer (%08x)\nabort\n", block, block->mNext);
                break;
            }
            if (block->mNext->mPrev != block) {
                ok = false;
                JUTWarningConsole_f(":::addr %08x: bad previous pointer (%08x)\n", block->mNext, block->mNext->mPrev);
            }
        } else {
            if (mTailUsedList != block) {
                ok = false;
                JUTWarningConsole_f(":::addr %08x: bad used list(REV) (%08x)\n", block, mTailUsedList);
            }
        }
        totalBytes += sizeof(CMemBlock) + block->mSize + block->getAlignment();
    }

    for (CMemBlock* block = mHeadFreeList; block; block = block->mNext) {
        totalBytes += block->mSize + sizeof(CMemBlock);
        if (block->mNext) {
            if (block->mNext->mPrev != block) {
                ok = false;
                JUTWarningConsole_f(":::addr %08x: bad previous pointer (%08x)\n", block->mNext, block->mNext->mPrev);
            }

            if ((uintptr_t)block + block->mSize + sizeof(CMemBlock) > (uintptr_t)block->mNext) {
                ok = false;
                JUTWarningConsole_f(":::addr %08x: bad block size (%08x)\n", block, block->mSize);
            }
        } else {
            if (mTailFreeList != block) {
                ok = false;
                JUTWarningConsole_f(":::addr %08x: bad used list(REV) (%08x)\n", block, mTailFreeList);
            }
        }
    }

    if (totalBytes != mSize) {
        ok = false;
        JUTWarningConsole_f(":::bad total memory block size (%08X, %08X)\n", mSize, totalBytes);
    }

    if (!ok) {
        JUTWarningConsole(":::there is some error in this heap!\n");
    }

    unlock();
    return ok;
}

bool JKRExpHeap::dump() {
    lock();
    bool result = check();
    u32 usedBytes = 0;
    u32 usedCount = 0;
    u32 freeCount = 0;

    JUTReportConsole(" attr  address:   size    gid aln   prev_ptr next_ptr\n");
    JUTReportConsole("(Used Blocks)\n");
    if (!mHeadUsedList) {
        JUTReportConsole(" NONE\n");
    }

    for (CMemBlock* block = mHeadUsedList; block; block = block->mNext) {
        if (block->mMagic != 'HM') {
            JUTReportConsole_f("xxxxx %08x: --------  --- ---  (-------- --------)\nabort\n", block);
            break;
        }

        JUTReportConsole_f("%s %08x: %08x  %3d %3d  (%08x %08x)\n", block->isTempMemBlock() ? " temp" : "alloc", block->getContent(), block->mSize,
                           block->mGroupId, block->getAlignment(), block->mPrev, block->mNext);
        usedBytes += sizeof(CMemBlock) + block->mSize + block->getAlignment();
        usedCount++;
    }

    JUTReportConsole("(Free Blocks)\n");
    if (!mHeadFreeList) {
        JUTReportConsole(" NONE\n");
    }

    for (CMemBlock* block = mHeadFreeList; block; block = block->mNext) {
        JUTReportConsole_f("%s %08x: %08x  %3d %3d  (%08x %08x)\n", " free", block->getContent(), block->mSize, block->mGroupId,
                           block->getAlignment(), block->mPrev, block->mNext);
        freeCount++;
    }

    JUTReportConsole_f("%d / %d bytes (%6.2f%%) used (U:%d F:%d)\n", usedBytes, mSize, (f32(usedBytes) / f32(mSize)) * 100.0f, usedCount, freeCount);
    unlock();
    return result;
}

bool JKRExpHeap::dump_sort() {
    lock();
    bool result = check();
    u32 usedBytes = 0;
    u32 usedCount = 0;
    u32 freeCount = 0;
    JUTReportConsole(" attr  address:   size    gid aln   prev_ptr next_ptr\n");
    JUTReportConsole("(Used Blocks)\n");

    if (mHeadUsedList == NULL) {
        JUTReportConsole(" NONE\n");
    } else {
        CMemBlock* var1 = NULL;
        while (true) {
            CMemBlock* block = (CMemBlock*)0xffffffff;
            for (CMemBlock* iterBlock = mHeadUsedList; iterBlock; iterBlock = iterBlock->mNext) {
                if (var1 < iterBlock && iterBlock < block) {
                    block = iterBlock;
                }
            }

            if (uintptr_t(block) == 0xffffffff) {
                break;
            }

            if (block->mMagic != 'HM') {
                JUTReportConsole_f("xxxxx %08x: --------  --- ---  (-------- --------)\nabort\n", var1);
                break;
            }

            JUTReportConsole_f("%s %08x: %08x  %3d %3d  (%08x %08x)\n", block->isTempMemBlock() ? " temp" : "alloc", block->getContent(),
                               block->mSize, block->mGroupId, block->getAlignment(), block->mPrev, block->mNext);
            usedBytes += sizeof(CMemBlock) + block->mSize + block->getAlignment();
            usedCount++;
            var1 = block;
        }
    }

    JUTReportConsole("(Free Blocks)\n");
    if (mHeadFreeList == NULL) {
        JUTReportConsole(" NONE\n");
    }

    for (CMemBlock* block = mHeadFreeList; block; block = block->mNext) {
        JUTReportConsole_f("%s %08x: %08x  %3d %3d  (%08x %08x)\n", " free", block->getContent(), block->mSize, block->mGroupId,
                           block->getAlignment(), block->mPrev, block->mNext);
        freeCount++;
    }

    JUTReportConsole_f("%d / %d bytes (%6.2f%%) used (U:%d F:%d)\n", usedBytes, mSize, (f32(usedBytes) / f32(mSize)) * 100.0f, usedCount, freeCount);
    unlock();
    return result;
}

JKRExpHeap::JKRExpHeap(void* data, u32 size, JKRHeap* parent, bool error) : JKRHeap(data, size, parent, error) {
    CMemBlock* block = (CMemBlock*)data;

    _6A = 0;
    _6B = 0xFF;
    mHeadFreeList = block;
    mTailFreeList = block;
    block->initiate(nullptr, nullptr, size - sizeof(CMemBlock), 0, 0);
    mHeadUsedList = nullptr;
    mTailUsedList = nullptr;
}

JKRExpHeap::~JKRExpHeap() {
    dispose();
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
    } else {
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

void JKRExpHeap::state_register(JKRHeap::TState* p, u32 param_1) const {
    void* r24 = getState_(p);
    u32 r25 = param_1;
    setState_u32ID_(p, param_1);
    if (param_1 <= 0xff) {
        setState_uUsedSize_(p, getUsedSize(r25));
    } else {
        setState_uUsedSize_(p, getUsedSize_(const_cast< JKRExpHeap* >(this)));
    }

    u32 checkCode = 0;
    for (CMemBlock* block = mHeadUsedList; block; block = block->mNext) {
        if (param_1 <= 0xff) {
            if (block->mGroupId == param_1) {
                checkCode += (uintptr_t)block * 3;
            }
        } else {
            checkCode += (uintptr_t)block * 3;
        }
    }
    setState_u32CheckCode_(p, checkCode);
}

bool JKRExpHeap::state_compare(JKRHeap::TState const& r1, JKRHeap::TState const& r2) const {
    bool result = true;
    if (r1.getCheckCode() != r2.getCheckCode()) {
        result = false;
    }

    if (r1.getUsedSize() != r2.getUsedSize()) {
        result = false;
    }

    return result;
}

u32 JKRExpHeap::getHeapType() {
    return 'EXPH';
}

u8 JKRExpHeap::do_getCurrentGroupId() {
    return mCurrentGroupId;
}
