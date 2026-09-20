#include "JSystem/JAudio2/JASHeapCtrl.hpp"
#include "JSystem/JAudio2/JASMutex.hpp"
#include "JSystem/JAudio2/JASWaveArcLoader.hpp"
#include "JSystem/JKernel/JKRExpHeap.hpp"
#include "JSystem/JKernel/JKRSolidHeap.hpp"
#include <revolution/aralt.h>
#include <revolution/os/OSMutex.h>

JASHeap::JASHeap(JASDisposer* pDisposer) : mTree(this), mDisposer(pDisposer), mBase(), mSize(), mHeap() {
    OSInitMutex(&mMutex);
}

void JASHeap::initRootHeap(void* pBase, u32 size) {
    OSLockMutex(&mMutex);
    mBase = (u8*)OSRoundUp32B(pBase);
    mHeap = nullptr;
    mSize = size - (u32(mBase) - u32(pBase));
    OSUnlockMutex(&mMutex);
}

bool JASHeap::alloc(JASHeap* pParent, u32 size) {
    JASMutexLock lock(&mMutex);
    if (isAllocated()) {
        return false;
    }

    if (!pParent->isAllocated()) {
        return false;
    }

    size = OSRoundUp32B(size);
    u32 curOffset = pParent->getCurOffset();
    u32 tailOffset = pParent->getTailOffset();
    if (curOffset + size <= tailOffset) {
        pParent->insertChild(this, pParent->getTailHeap(), pParent->mBase + curOffset, size, false);
        return true;
    }

    s32 smallestGap = -1;
    u8* pCurrent = pParent->mBase;
    bool found = false;
    JASHeap* pNext = nullptr;
    void* pAddress;
    JSUTreeIterator< JASHeap > it(nullptr);
    for (it = pParent->mTree.getFirstChild(); it != pParent->mTree.getEndChild(); ++it) {
        if (pCurrent >= pParent->mBase + tailOffset) {
            break;
        }

        u32 gapSize = u32(it->mBase) - u32(pCurrent);
        if (gapSize >= size && gapSize < smallestGap) {
            pNext = &*it;
            pAddress = pCurrent;
            smallestGap = gapSize;
            found = true;
        }

        u32 childSize = it->mSize;
        pCurrent = (u8*)it->mBase + childSize;
    }

    if (pCurrent != pParent->mBase && pCurrent < pParent->mBase + tailOffset) {
        u32 gapSize = pParent->mBase + pParent->mSize - pCurrent;
        if (gapSize >= size && gapSize < smallestGap) {
            pNext = nullptr;
            pAddress = pCurrent;
            smallestGap = gapSize;
            found = true;
        }
    }

    if (!found) {
        return false;
    }

    pParent->insertChild(this, pNext, pAddress, size, false);
    return true;
}

bool JASHeap::allocTail(JASHeap* pParent, u32 size) {
    JASMutexLock lock(&mMutex);
    if (isAllocated()) {
        return false;
    }

    if (!pParent->isAllocated()) {
        return false;
    }

    u32 alignedSize = (size + 0x1f) & ~0x1f;
    u32 curOffset = pParent->getCurOffset();
    u32 tailOffset = pParent->getTailOffset();
    if (curOffset + alignedSize > tailOffset) {
        return false;
    }

    pParent->insertChild(this, pParent->getTailHeap(), pParent->mBase + tailOffset - alignedSize, alignedSize, true);
    return true;
}

bool JASHeap::free() {
    JASMutexLock lock(&mMutex);
    if (!isAllocated()) {
        return false;
    }

    JSUTreeIterator< JASHeap > next(nullptr);
    for (JSUTreeIterator< JASHeap > it(mTree.getFirstChild()); it != mTree.getEndChild(); it = next) {
        next = it;
        ++next;
        it->free();
    }

    JSUTree< JASHeap >* pParentTree = mTree.getParent();
    if (pParentTree != nullptr) {
        JASHeap* pParent = pParentTree->getObject();
        if (pParent->mHeap == this) {
            JSUTreeIterator< JASHeap > prev(mTree.getPrevChild());
            if (prev != mTree.getEndChild()) {
                pParent->mHeap = &*prev;
            } else {
                pParent->mHeap = nullptr;
            }
        }

        pParentTree->removeChild(&mTree);
    }

    mBase = nullptr;
    mHeap = nullptr;
    mSize = 0;
    if (mDisposer != nullptr) {
        mDisposer->onDispose();
    }

    return true;
}

void JASHeap::insertChild(JASHeap* pHeap, JASHeap* pNext, void* pBase, u32 size, bool fromTail) {
    JASMutexLock lock(&mMutex);
    if (!fromTail) {
        JSUTreeIterator< JASHeap > it;
        if (pNext == nullptr) {
            it = mTree.getLastChild();
        } else {
            it = pNext->mTree.getPrevChild();
        }

        JASHeap* pPrev = it != mTree.getEndChild() ? it.getObject() : nullptr;
        if (mHeap == pPrev) {
            mHeap = pHeap;
        }
    }

    pHeap->mBase = (u8*)pBase;
    pHeap->mSize = size;
    pHeap->mHeap = nullptr;
    mTree.insertChild(&pNext->mTree, &pHeap->mTree);
}

JASHeap* JASHeap::getTailHeap() {
    JASMutexLock lock(&mMutex);
    JSUTreeIterator< JASHeap > it;
    if (mHeap == nullptr) {
        it = mTree.getFirstChild();
    } else {
        it = mHeap->mTree.getNextChild();
    }

    if (it == mTree.getEndChild()) {
        return nullptr;
    }

    return it.getObject();
}

u32 JASHeap::getTailOffset() {
    u32 offset = 0;
    JASMutexLock lock(&mMutex);
    JASHeap* pHeap = getTailHeap();
    if (pHeap == nullptr) {
        offset = mSize;
    } else {
        offset = pHeap->mBase - mBase;
    }

    return offset;
}

u32 JASHeap::getCurOffset() {
    u32 offset = 0;
    JASMutexLock lock(&mMutex);
    if (mHeap == nullptr) {
        offset = 0;
    } else {
        offset = mHeap->mBase + mHeap->mSize - mBase;
    }

    return offset;
}

JASGenericMemPool::JASGenericMemPool() : _0(), mFreeMemCount(), mTotalMemCount(), mUsedMemCount() {
}

JASGenericMemPool::~JASGenericMemPool() {
    void* pChunk = _0;
    while (pChunk != nullptr) {
        void* pNextChunk = *(void**)pChunk;
        delete[] pChunk;
        pChunk = pNextChunk;
    }
}

JKRSolidHeap* JASDram;

void JASGenericMemPool::newMemPool(u32 size, int n) {
    void* pChunk;
    for (int i = 0; i < n; i++) {
        pChunk = new (JASDram, 0) u8[size];
        *(void**)pChunk = _0;
        _0 = pChunk;
    }

    mFreeMemCount += n;
    mTotalMemCount += n;
}

void* JASGenericMemPool::alloc(u32 size) {
    if (_0 == nullptr) {
        return nullptr;
    }

    void* pChunk = _0;
    _0 = *(void**)pChunk;
    mFreeMemCount--;
    if (mUsedMemCount < mTotalMemCount - mFreeMemCount) {
        mUsedMemCount = mTotalMemCount - mFreeMemCount;
    }

    return pChunk;
}

void JASGenericMemPool::free(void* pMemory, u32 size) {
    if (pMemory == nullptr) {
        return;
    }

    void* pChunk = pMemory;
    *(void**)pChunk = _0;
    _0 = pChunk;
    mFreeMemCount++;
}

u32 JASKernel::sAramBase;

JKRHeap* JASKernel::sSystemHeap;

JASMemChunkPool< 1024, JASThreadingModel::ObjectLevelLockable >* JASKernel::sCommandHeap;

void JASKernel::setupRootHeap(JKRSolidHeap* pHeap, u32 size) {
    sSystemHeap = JKRExpHeap::create(size, pHeap, false);
    sCommandHeap = new (pHeap, 0) JASMemChunkPool< 1024, JASThreadingModel::ObjectLevelLockable >();
    JASDram = pHeap;
}

JKRHeap* JASKernel::getSystemHeap() {
    return JASKernel::sSystemHeap;
}

JASMemChunkPool< 1024, JASThreadingModel::ObjectLevelLockable >* JASKernel::getCommandHeap() {
    return JASKernel::sCommandHeap;
}

JASHeap JASKernel::audioAramHeap;

void JASKernel::setupAramHeap(u32 base, u32 size) {
    OSReport("setupAramHeap %x, %x, %x\n", base, ARGetBaseAddress(), size);
    base = ARGetBaseAddress();
    sAramBase = base;
    audioAramHeap.initRootHeap((void*)sAramBase, size);
}

JASHeap* JASKernel::getAramHeap() {
    return &audioAramHeap;
}
