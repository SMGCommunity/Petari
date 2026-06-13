#include "JSystem/JAudio2/JASHeapCtrl.hpp"
#include "JSystem/JAudio2/JASMutex.hpp"
#include "JSystem/JAudio2/JASWaveArcLoader.hpp"
#include "JSystem/JKernel/JKRExpHeap.hpp"
#include "JSystem/JKernel/JKRSolidHeap.hpp"
#include "revolution/aralt.h"
#include "revolution/os/OSMutex.h"

// NOTE TODO: there is a LOT wrong with this file.

JASHeap::JASHeap(JASDisposer* disposer) : mTree(this) {
    mDisposer = disposer;
    mBase = nullptr;
    mSize = 0;
    mHeap = 0;
    OSInitMutex(&mMutex);
}

void JASHeap::initRootHeap(void* param_0, u32 param_1) {
    OSLockMutex(&mMutex);
    mBase = (u8*)OSRoundUp32B(param_0);
    mHeap = nullptr;
    mSize = param_1 - (u32(mBase) - u32(param_0));
    OSUnlockMutex(&mMutex);
}

bool JASHeap::alloc(JASHeap* mother, u32 param_1) {
    JASMutexLock lock(&mMutex);
    if (isAllocated()) {
        return 0;
    }
    if (!mother->isAllocated()) {
        return 0;
    }
    param_1 = OSRoundUp32B(param_1);
    u32 local_28 = mother->getCurOffset();
    u32 local_2c = mother->getTailOffset();
    if (local_28 + param_1 <= local_2c) {
        mother->insertChild(this, mother->getTailHeap(), mother->mBase + local_28, param_1, false);
        return true;
    }
    s32 r27 = -1;
    u8* r29 = mother->mBase;
    bool local_43 = false;
    JASHeap* local_30 = nullptr;
    void* local_34;
    JSUTreeIterator< JASHeap > it;
    for (it = mother->mTree.getFirstChild(); it != mother->mTree.getEndChild(); ++it) {
        if (r29 >= mother->mBase + local_2c) {
            break;
        }
        u32 local_3c = u32(it->mBase) - u32(r29);
        if (local_3c >= param_1 && local_3c < r27) {
            local_30 = &*it;
            local_34 = r29;
            r27 = local_3c;
            local_43 = true;
        }
        u32 r25 = it->mSize;
        r29 = (u8*)it->mBase + r25;
    }
    if (r29 != mother->mBase && r29 < mother->mBase + local_2c) {
        u32 local_40 = mother->mBase + mother->mSize - r29;
        if (local_40 >= param_1 && local_40 < r27) {
            local_30 = nullptr;
            local_34 = r29;
            r27 = local_40;
            local_43 = true;
        }
    }
    if (!local_43) {
        return 0;
    }
    mother->insertChild(this, local_30, local_34, param_1, false);
    return 1;
}

bool JASHeap::allocTail(JASHeap* mother, u32 size) {
    JASMutexLock lock(&mMutex);
    if (isAllocated()) {
        return false;
    }
    if (!mother->isAllocated()) {
        return false;
    }
    u32 aligned_size = (size + 0x1f) & ~0x1f;
    u32 cur_offset = mother->getCurOffset();
    u32 tail_offset = mother->getTailOffset();
    if (cur_offset + aligned_size > tail_offset) {
        return false;
    }
    mother->insertChild(this, mother->getTailHeap(), mother->mBase + tail_offset - aligned_size, aligned_size, true);
    return true;
}

bool JASHeap::free() {
    JASMutexLock lock(&mMutex);
    if (!isAllocated()) {
        return false;
    }
    JSUTreeIterator< JASHeap > stack_20(nullptr);
    for (JSUTreeIterator< JASHeap > it(mTree.getFirstChild()); it != mTree.getEndChild(); it = stack_20) {
        stack_20 = it;
        ++stack_20;
        it->free();
    }
    JSUTree< JASHeap >* parentTree = mTree.getParent();
    if (parentTree != nullptr) {
        JASHeap* parentHeap = parentTree->getObject();
        if (parentHeap->mHeap == this) {
            JSUTreeIterator< JASHeap > stack_28(mTree.getPrevChild());
            if (stack_28 != mTree.getEndChild()) {
                parentHeap->mHeap = &*stack_28;
            } else {
                parentHeap->mHeap = nullptr;
            }
        }
        parentTree->removeChild(&mTree);
    }
    mBase = nullptr;
    mHeap = nullptr;
    mSize = 0;
    if (mDisposer) {
        mDisposer->onDispose();
    }
    return true;
}

void JASHeap::insertChild(JASHeap* heap, JASHeap* next, void* param_2, u32 param_3, bool param_4) {
    JASMutexLock lock(&mMutex);
    if (!param_4) {
        JSUTreeIterator< JASHeap > it;
        if (!next) {
            it = mTree.getLastChild();
        } else {
            it = next->mTree.getPrevChild();
        }
        JASHeap* r24 = it != mTree.getEndChild() ? it.getObject() : nullptr;
        if (mHeap == r24) {
            mHeap = heap;
        }
    }
    heap->mBase = (u8*)param_2;
    heap->mSize = param_3;
    heap->mHeap = nullptr;
    mTree.insertChild(&next->mTree, &heap->mTree);
}

JASHeap* JASHeap::getTailHeap() {
    JASMutexLock lock(&getMutex());
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
    JASHeap* heap = getTailHeap();
    if (heap == nullptr) {
        offset = mSize;
    } else {
        offset = heap->mBase - mBase;
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

JASGenericMemPool::JASGenericMemPool() {
    _0 = nullptr;
    mFreeMemCount = 0;
    mTotalMemCount = 0;
    mUsedMemCount = 0;
}

JASGenericMemPool::~JASGenericMemPool() {
    void* chunk = _0;
    while (chunk != nullptr) {
        void* next_chunk = *(void**)chunk;
        delete[] chunk;
        chunk = next_chunk;
    }
}

JKRSolidHeap* JASDram;

void JASGenericMemPool::newMemPool(u32 size, int n) {
    void* runner;
    for (int i = 0; i < n; i++) {
        runner = new (JASDram, 0) u8[size];
        *(void**)runner = _0;
        _0 = runner;
    }
    mFreeMemCount += n;
    mTotalMemCount += n;
}

void* JASGenericMemPool::alloc(u32 size) {
    if (_0 == nullptr) {
        return nullptr;
    }
    void* chunk = _0;
    _0 = *(void**)chunk;
    mFreeMemCount--;
    if (mUsedMemCount < mTotalMemCount - mFreeMemCount) {
        mUsedMemCount = mTotalMemCount - mFreeMemCount;
    }
    return chunk;
}

void JASGenericMemPool::free(void* ptr, u32 param_1) {
    if (!ptr) {
        return;
    }
    void* chunk = ptr;
    *(void**)chunk = _0;
    _0 = chunk;
    mFreeMemCount++;
}

u32 JASKernel::sAramBase;

JKRHeap* JASKernel::sSystemHeap;

JASMemChunkPool< 1024, JASThreadingModel::ObjectLevelLockable >* JASKernel::sCommandHeap;

void JASKernel::setupRootHeap(JKRSolidHeap* heap, u32 size) {
    sSystemHeap = JKRExpHeap::create(size, heap, false);
    sCommandHeap = new (heap, 0) JASMemChunkPool< 1024, JASThreadingModel::ObjectLevelLockable >();
    // sCommandHeap->createNewChunk();
    JASDram = heap;
}

JKRHeap* JASKernel::getSystemHeap() {
    return JASKernel::sSystemHeap;
}

JASMemChunkPool< 1024, JASThreadingModel::ObjectLevelLockable >* JASKernel::getCommandHeap() {
    return JASKernel::sCommandHeap;
}

JASHeap JASKernel::audioAramHeap;

void JASKernel::setupAramHeap(u32 param_0, u32 param_1) {
    // FIXME: condition reg not cleared
    OSReport("setupAramHeap %x, %x, %x\n", param_0, ARGetBaseAddress(), param_1);
    param_0 = ARGetBaseAddress();
    sAramBase = param_0;
    audioAramHeap.initRootHeap((void*)sAramBase, param_1);
}

JASHeap* JASKernel::getAramHeap() {
    return &audioAramHeap;
}
