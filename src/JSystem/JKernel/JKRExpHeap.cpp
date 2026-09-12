#include "JSystem/JKernel/JKRExpHeap.hpp"
#include "JSystem/JUtility/JUTConsole.hpp"
#include "JSystem/JUtility/JUTException.hpp"
#include <new>

extern "C" void JUTWarningConsole(const char*);
extern "C" void JUTReportConsole(const char*);

static u32 DBfoundSize;
static u32 DBfoundOffset;
static JKRExpHeap::CMemBlock* DBfoundBlock;
static JKRExpHeap::CMemBlock* DBnewFreeBlock;
static JKRExpHeap::CMemBlock* DBnewUsedBlock;

JKRExpHeap* JKRExpHeap::createRoot(int heapNum, bool errorFlag) {
    JKRExpHeap* heap = nullptr;

    if (!JKRHeap::sRootHeap) {
        char* arena;
        u32 arenaSize;
        JKRHeap::initArena(&arena, &arenaSize, heapNum);
        char* area = arena + 0x90;
        u32 size = arenaSize - 0x90;
        heap = new (arena) JKRExpHeap(area, size, nullptr, errorFlag);
        JKRHeap::sRootHeap = heap;
    }

    heap->mIsExternal = 1;
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

    u8* mem = static_cast< u8* >(JKRHeap::alloc(alignedSize, 16, pParent));
    u8* data = (mem + heapSize);
    if (mem == nullptr) {
        return nullptr;
    }

    JKRExpHeap* heap = new (mem) JKRExpHeap(data, alignedSize - heapSize, pParent, errorFlag);

    if (heap == nullptr) {
        JKRHeap::free(mem, nullptr);
        return nullptr;
    }

    heap->mIsExternal = 0;
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

    void* data = static_cast< u8* >(ptr) + heapSize;
    u32 alignSize = ALIGN_PREV(reinterpret_cast< u32 >(ptr) + size - reinterpret_cast< u32 >(data), 0x10);
    if (ptr != nullptr) {
        heap = new (ptr) JKRExpHeap(data, alignSize, parent, errorFlag);
    }

    heap->mIsExternal = 1;
    heap->_70 = ptr;
    heap->_74 = size;
    return heap;
}

void JKRExpHeap::do_destroy() {
    if (!mIsExternal) {
        JKRHeap* heap = mChildTree.getParent()->getObject();

        if (heap != nullptr) {
            this->~JKRExpHeap();
            JKRHeap::free(this, heap);
        }
    } else {
        this->~JKRExpHeap();
    }
}

JKRExpHeap::JKRExpHeap(void* data, u32 size, JKRHeap* parent, bool error) : JKRHeap(data, size, parent, error) {
    CMemBlock* block = static_cast< CMemBlock* >(data);

    mAllocMode = 0;
    mCurrentGroupId = 0xFF;
    mHeadFreeList = block;
    mTailFreeList = block;
    block->initiate(nullptr, nullptr, size - sizeof(CMemBlock), 0, 0);
    mHeadUsedList = nullptr;
    mTailUsedList = nullptr;
}

JKRExpHeap::~JKRExpHeap() {
    dispose();
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
    u32 content;
    u32 blockSize;
    u32 foundOffset;
    int foundSize;

    size = ALIGN_NEXT(size, 4);
    foundSize = -1;
    foundOffset = 0;
    CMemBlock* foundBlock = nullptr;
    CMemBlock* newFreeBlock = nullptr;
    CMemBlock* newUsedBlock = nullptr;

    for (CMemBlock* block = mHeadFreeList; block; block = block->mNext) {
        content = reinterpret_cast< u32 >(block->getContent());
        blockSize = block->mSize;
        u32 offset = ALIGN_PREV(align + content - 1, align) - content;
        if (blockSize < size + offset) {
            continue;
        }

        if (foundSize <= blockSize) {
            continue;
        }

        foundSize = blockSize;
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
                newFreeBlock = nullptr;
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
                newUsedBlock = reinterpret_cast< CMemBlock* >(reinterpret_cast< u32 >(foundBlock) + foundOffset);
                newUsedBlock->mSize = foundBlock->mSize - foundOffset;
                newFreeBlock = newUsedBlock->allocFore(size, mCurrentGroupId, static_cast< u8 >(foundOffset), 0, 0);
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

    return nullptr;
}

void* JKRExpHeap::allocFromHead(u32 size) {
    size = ALIGN_NEXT(size, 4);
    s32 foundSize = -1;
    CMemBlock* foundBlock = nullptr;
    CMemBlock* newBlock = nullptr;
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
        newBlock = foundBlock->allocFore(size, mCurrentGroupId, 0, 0, 0);
        if (newBlock) {
            setFreeBlock(newBlock, foundBlock->mPrev, foundBlock->mNext);
        } else {
            removeFreeBlock(foundBlock);
        }
        appendUsedList(foundBlock);
        return foundBlock->getContent();
    }
    return nullptr;
}

void* JKRExpHeap::allocFromTail(u32 size, int align) {
    u32 offset = 0;
    CMemBlock* foundBlock = nullptr;
    CMemBlock* newBlock = nullptr;
    u32 usedSize;
    u32 start;

    for (CMemBlock* block = mTailFreeList; block; block = block->mPrev) {
        start = ALIGN_PREV(reinterpret_cast< u32 >(block->getContent()) + block->mSize - size, align);
        usedSize = reinterpret_cast< u32 >(block->getContent()) + block->mSize - start;
        if (block->mSize >= usedSize) {
            foundBlock = block;
            offset = block->mSize - usedSize;
            newBlock = reinterpret_cast< CMemBlock* >(start) - 1;
            break;
        }
    }

    if (foundBlock != nullptr) {
        if (offset >= sizeof(CMemBlock)) {
            newBlock->initiate(nullptr, nullptr, usedSize, mCurrentGroupId, -0x80);
            foundBlock->mSize = foundBlock->mSize - usedSize - sizeof(CMemBlock);
            appendUsedList(newBlock);
            return newBlock->getContent();
        } else {
            if (offset != 0) {
                removeFreeBlock(foundBlock);
                newBlock->initiate(nullptr, nullptr, usedSize, mCurrentGroupId, offset | 0x80);
                appendUsedList(newBlock);
                return newBlock->getContent();
            } else {
                removeFreeBlock(foundBlock);
                newBlock->initiate(nullptr, nullptr, usedSize, mCurrentGroupId, -0x80);
                appendUsedList(newBlock);
                return newBlock->getContent();
            }
        }
    }

    return nullptr;
}

void* JKRExpHeap::allocFromTail(u32 size) {
    size = ALIGN_NEXT(size, 4);
    CMemBlock* foundBlock = nullptr;
    CMemBlock* freeBlock = nullptr;
    CMemBlock* usedBlock = nullptr;
    for (CMemBlock* block = mTailFreeList; block; block = block->mPrev) {
        if (block->mSize >= size) {
            foundBlock = block;
            break;
        }
    }

    if (foundBlock != nullptr) {
        usedBlock = foundBlock->allocBack(size, 0, 0, mCurrentGroupId, 0);
        if (usedBlock) {
            freeBlock = foundBlock;
        } else {
            removeFreeBlock(foundBlock);
            usedBlock = foundBlock;
            freeBlock = nullptr;
        }

        if (freeBlock) {
            setFreeBlock(freeBlock, foundBlock->mPrev, foundBlock->mNext);
        }
        appendUsedList(usedBlock);
        return usedBlock->getContent();
    }
    return nullptr;
}

void JKRExpHeap::do_free(void* ptr) {
    lock();
    if (mStart <= ptr && ptr <= mEnd) {
        CMemBlock* block = CMemBlock::getHeapBlock(ptr);
        if (block) {
            block->free(this);
        }
    }
    unlock();
}

void JKRExpHeap::do_freeAll() {
    lock();
    JKRHeap::callAllDisposer();
    mHeadFreeList = reinterpret_cast< CMemBlock* >(mStart);
    mTailFreeList = mHeadFreeList;
    mHeadFreeList->initiate(nullptr, nullptr, mSize - sizeof(CMemBlock), 0, 0);
    mHeadUsedList = nullptr;
    mTailUsedList = nullptr;
    unlock();
}

void JKRExpHeap::do_freeTail() {
    lock();
    for (CMemBlock* block = mHeadUsedList; block != nullptr;) {
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

s32 JKRExpHeap::do_changeGroupID(u8 groupId) {
    lock();
    u8 prev = mCurrentGroupId;
    mCurrentGroupId = groupId;
    unlock();
    return prev;
}

s32 JKRExpHeap::do_resize(void* ptr, u32 size) {
    u32 end;
    CMemBlock* foundBlock;

    lock();
    CMemBlock* block = CMemBlock::getHeapBlock(ptr);
    if (block == nullptr || ptr < mStart || mEnd < ptr) {
        unlock();
        return -1;
    }

    size = ALIGN_NEXT(size, 4);
    if (size == block->mSize) {
        unlock();
        return size;
    }

    if (size > block->mSize) {
        end = reinterpret_cast< u32 >(block + 1);
        end += block->mSize;
        foundBlock = nullptr;
        for (CMemBlock* freeBlock = mHeadFreeList; freeBlock; freeBlock = freeBlock->mNext) {
            if (freeBlock == reinterpret_cast< CMemBlock* >(end)) {
                foundBlock = freeBlock;
                break;
            }
        }

        if (foundBlock == nullptr) {
            unlock();
            return -1;
        }

        if (size > block->mSize + sizeof(CMemBlock) + foundBlock->mSize) {
            unlock();
            return -1;
        }

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
        if (size < static_cast< s32 >(block->mSize)) {
            size = block->mSize;
        }
    }
    unlock();
    return size;
}

void* JKRExpHeap::do_getMaxFreeBlock() {
    lock();
    s32 size = 0;
    CMemBlock* res = nullptr;
    for (CMemBlock* block = mHeadFreeList; block; block = block->mNext) {
        if (size < static_cast< s32 >(block->mSize)) {
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
    bool result = !mHeadUsedList;
    unlock();
    return result;
}

void JKRExpHeap::appendUsedList(JKRExpHeap::CMemBlock* newBlock) {
    if (!newBlock) {
        JUTException::panic(__FILE__, 1568, "bad appendUsedList\n");
    }

    CMemBlock* block = mTailUsedList;
    newBlock->mMagic = 'HM';

    if (block) {
        block->mNext = newBlock;
        newBlock->mPrev = block;
    } else {
        newBlock->mPrev = nullptr;
    }

    mTailUsedList = newBlock;
    if (!mHeadUsedList) {
        mHeadUsedList = newBlock;
    }
    newBlock->mNext = nullptr;
}

void JKRExpHeap::setFreeBlock(CMemBlock* block, CMemBlock* prev, CMemBlock* next) {
    if (prev == nullptr) {
        mHeadFreeList = block;
        block->mPrev = nullptr;
    } else {
        prev->mNext = block;
        block->mPrev = prev;
    }

    if (next == nullptr) {
        mTailFreeList = block;
        block->mNext = nullptr;
    } else {
        next->mPrev = block;
        block->mNext = next;
    }

    block->mMagic = 0;
}

void JKRExpHeap::removeFreeBlock(CMemBlock* block) {
    CMemBlock* prev = block->mPrev;
    CMemBlock* next = block->mNext;

    if (prev == nullptr) {
        mHeadFreeList = next;
    } else {
        prev->mNext = next;
    }

    if (next == nullptr) {
        mTailFreeList = prev;
    } else {
        next->mPrev = prev;
    }
}

void JKRExpHeap::removeUsedBlock(JKRExpHeap::CMemBlock* block) {
    CMemBlock* prev = block->mPrev;
    CMemBlock* next = block->mNext;

    if (prev == nullptr) {
        mHeadUsedList = next;
    } else {
        prev->mNext = next;
    }

    if (next == nullptr) {
        mTailUsedList = prev;
    } else {
        next->mPrev = prev;
    }
}

void JKRExpHeap::recycleFreeBlock(JKRExpHeap::CMemBlock* block) {
    JKRExpHeap::CMemBlock* newBlock = block;
    int size = block->mSize;
    void* blockEnd = reinterpret_cast< u8* >(newBlock) + size;
    block->mMagic = 0;

    if ((block->mFlags & 0x7f) != 0) {
        newBlock = reinterpret_cast< CMemBlock* >(reinterpret_cast< u8* >(newBlock) - (block->mFlags & 0x7f));
        size += (block->mFlags & 0x7f);
        blockEnd = reinterpret_cast< u8* >(newBlock) + size;
        newBlock->mGroupId = 0;
        newBlock->mFlags = 0;
        newBlock->mSize = size;
    }
    if (!mHeadFreeList) {
        newBlock->initiate(nullptr, nullptr, size, 0, 0);
        mHeadFreeList = newBlock;
        mTailFreeList = newBlock;
        setFreeBlock(newBlock, nullptr, nullptr);
        return;
    }

    if (mHeadFreeList >= blockEnd) {
        newBlock->initiate(nullptr, nullptr, size, 0, 0);
        setFreeBlock(newBlock, nullptr, mHeadFreeList);
        joinTwoBlocks(newBlock);
        return;
    }

    if (mTailFreeList <= newBlock) {
        newBlock->initiate(nullptr, nullptr, size, 0, 0);
        setFreeBlock(newBlock, mTailFreeList, nullptr);
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
    u32 endAddr = reinterpret_cast< u32 >(block + 1) + block->mSize;
    CMemBlock* next = block->mNext;
    u32 nextAddr = reinterpret_cast< u32 >(next) - (next->mFlags & 0x7f);
    if (endAddr > nextAddr) {
        JUTWarningConsole_f(":::Heap may be broken. (block = %x)", block);

        JKRHeap* heap = JKRGetCurrentHeap();
        heap->dump();
        JUTException::panic(__FILE__, 1820, "Bad Block\n");
    }

    if (endAddr == nextAddr) {
        block->mSize = next->mSize + sizeof(CMemBlock) + (next->mFlags & 0x7f) + block->mSize;
        CMemBlock* nextFree = next->mNext;
        setFreeBlock(block, block->mPrev, nextFree);
    }
}

bool JKRExpHeap::check() {
    lock();
    int totalBytes = 0;
    bool ok = true;
    for (CMemBlock* block = mHeadUsedList; block; block = block->mNext) {
        if (block->mMagic != 'HM') {
            ok = false;
            JUTWarningConsole_f(":::addr %08x: bad heap signature. (%c%c)\n", block, (block->mMagic >> 8), (block->mMagic & 0xff));
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

            if (reinterpret_cast< u32 >(block) + block->mSize + sizeof(CMemBlock) > reinterpret_cast< u32 >(block->mNext)) {
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

    JUTReportConsole_f("%d / %d bytes (%6.2f%%) used (U:%d F:%d)\n", usedBytes, mSize,
                       (static_cast< f32 >(usedBytes) / static_cast< f32 >(mSize)) * 100.0f, usedCount, freeCount);
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

    if (mHeadUsedList == nullptr) {
        JUTReportConsole(" NONE\n");
    } else {
        CMemBlock* previousBlock = nullptr;
        while (true) {
            CMemBlock* block = reinterpret_cast< CMemBlock* >(0xffffffff);
            for (CMemBlock* iterBlock = mHeadUsedList; iterBlock; iterBlock = iterBlock->mNext) {
                if (previousBlock < iterBlock && iterBlock < block) {
                    block = iterBlock;
                }
            }

            if (reinterpret_cast< u32 >(block) == 0xffffffff) {
                break;
            }

            if (block->mMagic != 'HM') {
                JUTReportConsole_f("xxxxx %08x: --------  --- ---  (-------- --------)\nabort\n", previousBlock);
                break;
            }

            JUTReportConsole_f("%s %08x: %08x  %3d %3d  (%08x %08x)\n", block->isTempMemBlock() ? " temp" : "alloc", block->getContent(),
                               block->mSize, block->mGroupId, block->getAlignment(), block->mPrev, block->mNext);
            usedBytes += sizeof(CMemBlock) + block->mSize + block->getAlignment();
            usedCount++;
            previousBlock = block;
        }
    }

    JUTReportConsole("(Free Blocks)\n");
    if (mHeadFreeList == nullptr) {
        JUTReportConsole(" NONE\n");
    }

    for (CMemBlock* block = mHeadFreeList; block; block = block->mNext) {
        JUTReportConsole_f("%s %08x: %08x  %3d %3d  (%08x %08x)\n", " free", block->getContent(), block->mSize, block->mGroupId,
                           block->getAlignment(), block->mPrev, block->mNext);
        freeCount++;
    }

    JUTReportConsole_f("%d / %d bytes (%6.2f%%) used (U:%d F:%d)\n", usedBytes, mSize,
                       (static_cast< f32 >(usedBytes) / static_cast< f32 >(mSize)) * 100.0f, usedCount, freeCount);
    unlock();
    return result;
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
        block = reinterpret_cast< CMemBlock* >(reinterpret_cast< u32 >(this) + size);
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
        block = reinterpret_cast< CMemBlock* >(reinterpret_cast< u32 >(this) + mSize - size);
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
        CMemBlock* block = static_cast< CMemBlock* >(ptr) - 1;

        if (block->mMagic == 'HM') {
            return block;
        }
    }

    return nullptr;
}

static void dumpDebugInfo() {
    JUTReportConsole("+---------------JKRExpHeap\n");
    JUTReportConsole("|         Align Group  size    ( prev , next )\n");
    JUTReportConsole("| ---- FreeFirst\n");
    JUTReportConsole("| %08x  ");
    JUTReportConsole("%2x  %3d  %6x  (%08x %08x)\n");
    JUTReportConsole("| ---- FreeLast\n");
    JUTReportConsole("| ---- UsedFirst\n");
    JUTReportConsole("| ---- UsedLast\n");
    JUTReportConsole("+---------------End\n");
}

void JKRExpHeap::state_register(JKRHeap::TState* p, u32 id) const {
    setState_u32ID_(p, id);
    if (id <= 0xff) {
        setState_uUsedSize_(p, getUsedSize(id));
    } else {
        setState_uUsedSize_(p, (mSize - const_cast< JKRExpHeap* >(this)->getTotalFreeSize()));
    }

    u32 checkCode = 0;
    for (CMemBlock* block = mHeadUsedList; block; block = block->mNext) {
        if (id <= 0xff) {
            if (block->mGroupId == id) {
                checkCode += reinterpret_cast< u32 >(block) * 3;
            }
        } else {
            checkCode += reinterpret_cast< u32 >(block) * 3;
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

u8 JKRExpHeap::do_getCurrentGroupId() {
    return mCurrentGroupId;
}

u32 JKRExpHeap::getHeapType() {
    return 'EXPH';
}
