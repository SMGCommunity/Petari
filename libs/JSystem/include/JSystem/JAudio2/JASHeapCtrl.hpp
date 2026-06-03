#pragma once

#include "JSystem/JKernel/JKRHeap.hpp"
#include "JSystem/JSupport/JSUList.hpp"
#include <revolution/os.h>
#include <revolution/types.h>

class JASDisposer;
class JKRHeap;
class JKRSolidHeap;

extern JKRSolidHeap* JASDram;

class JASHeap {
public:
    JASHeap(JASDisposer* disposer = nullptr);

    void initRootHeap(void*, u32);
    bool alloc(JASHeap*, u32);
    bool allocTail(JASHeap*, u32);
    bool free();
    void insertChild(JASHeap*, JASHeap*, void*, u32, bool);
    JASHeap* getTailHeap();
    u32 getTailOffset();
    u32 getCurOffset();

    /* 0x00 */ JSUTree< JASHeap > mTree;
    /* 0x1C */ OSMutex mMutex;
    /* 0x34 */ JASDisposer* mDisposer;
    /* 0x38 */ u8* mBase;
    /* 0x3C */ u32 mSize;
    /* 0x40 */ JASHeap* mHeap;
};

class JASGenericMemPool {
public:
    JASGenericMemPool();
    ~JASGenericMemPool();

    void newMemPool(u32, int);
    void* alloc(u32);

    void free(void*, u32);

    /* 0x0 */ void* _0;
    /* 0x4 */ u32 mFreeMemCount;
    /* 0x8 */ u32 mTotalMemCount;
    /* 0xC */ u32 mUsedMemCount;
};

namespace JASThreadingModel {
    template < typename T >
    class InterruptsDisable {
    public:
        struct Lock {
            Lock(const T&) NO_INLINE {
                mInterruptState = OSDisableInterrupts();
            }
            ~Lock() NO_INLINE;

            /* 0x00 */ u32 mInterruptState;
        };
    };

    template < typename T >
    struct ObjectLevelLockable : public OSMutex {
        ObjectLevelLockable() {
            OSInitMutex(this);
        }

        struct Lock {
            Lock(T const& mutex) {
                mMutex = (T*)&mutex;
                OSLockMutex(mMutex);
            }

            ~Lock() {
                OSUnlockMutex(mMutex);
            }

            /* 0x00 */ T* mMutex;
        };
    };

    template < typename T >
    struct SingleThreaded {
        struct Lock {
            Lock(const T&) {
            }
        };
    };
}  // namespace JASThreadingModel

template < typename T >
class JASMemPool : public JASGenericMemPool {
public:
    JASMemPool() : JASGenericMemPool() {
    }

    ~JASMemPool();
};

namespace JASKernel {
    JKRHeap* getSystemHeap();
};  // namespace JASKernel

template < u32 ChunkSize, template < typename > class T >
class JASMemChunkPool : public T< JASMemChunkPool< ChunkSize, T > >::ObjectLevelLockable {
public:
    struct MemoryChunk {
        MemoryChunk(MemoryChunk* nextChunk) {
            mNextChunk = nextChunk;
            mUsedSize = 0;
            mChunks = 0;
        }

        bool checkArea(const void* ptr) const {
            return (u8*)this + 0xc <= (u8*)ptr && (u8*)ptr < (u8*)this + (0xc + ChunkSize);
        }

        MemoryChunk* getNextChunk() {
            return mNextChunk;
        }

        void* alloc(u32 size) {
            u8* rv = mBuffer + mUsedSize;
            mUsedSize += size;
            mChunks++;
            return rv;
        }

        void free(void* mem) {
            mChunks--;
        }

        bool isEmpty() const {
            return mChunks == 0;
        }

        void setNextChunk(MemoryChunk* chunk) {
            mNextChunk = chunk;
        }

        u32 getFreeSize() const {
            return ChunkSize - mUsedSize;
        }

        void revive() {
            mUsedSize = 0;
        }

        /* 0x0 */ MemoryChunk* mNextChunk;
        /* 0x4 */ u32 mUsedSize;
        /* 0x8 */ u32 mChunks;
        /* 0xC */ u8 mBuffer[ChunkSize];
    };

    JASMemChunkPool() {
        mChunk = nullptr;
        createNewChunk();
    }

    bool createNewChunk() {
        bool b = 0;
        if (mChunk != nullptr && mChunk->isEmpty()) {
            mChunk->revive();
            return true;
        }
        MemoryChunk* pMVar4 = mChunk;
        mChunk = new (JASKernel::getSystemHeap(), 0) MemoryChunk(pMVar4);
        if (mChunk != nullptr) {
            return true;
        }

        mChunk = new (JKRHeap::getSystemHeap(), 0) MemoryChunk(pMVar4);
        if (mChunk != nullptr) {
            return true;
        }
        mChunk = pMVar4;
        return false;
    }

    void* alloc(u32 size) {
        typename T< JASMemChunkPool< ChunkSize, T > >::Lock lock(*this);
        u32 freeSize = mChunk->getFreeSize();
        if (freeSize < size) {
            if (ChunkSize < size) {
                return nullptr;
            }
            if (createNewChunk() == 0) {
                return nullptr;
            }
        }
        return mChunk->alloc(size);
    }

    void free(void* ptr) {
        typename T< JASMemChunkPool< ChunkSize, T > >::Lock lock(*this);
        MemoryChunk* chunk = mChunk;
        MemoryChunk* prevChunk = nullptr;
        while (chunk != nullptr) {
            if (chunk->checkArea(ptr)) {
                chunk->free(ptr);
                bool r26 = false;
                if (chunk != mChunk && chunk->isEmpty()) {
                    MemoryChunk* nextChunk = chunk->getNextChunk();
                    delete chunk;
                    prevChunk->setNextChunk(nextChunk);
                }
                return;
            }
            prevChunk = chunk;
            chunk = chunk->getNextChunk();
        }
    }

    /* 0x18 */ MemoryChunk* mChunk;
};

namespace JASKernel {
    void setupRootHeap(JKRSolidHeap*, u32);
    JKRHeap* getSystemHeap();
    JASMemChunkPool< 1024, JASThreadingModel::ObjectLevelLockable >* getCommandHeap();
    void setupAramHeap(u32, u32);
    JASHeap* getAramHeap();
    u32 getAramFreeSize();
    u32 getAramSize();

    extern JASHeap audioAramHeap;
    extern u32 sAramBase;
    extern JKRHeap* sSystemHeap;
    extern JASMemChunkPool< 1024, JASThreadingModel::ObjectLevelLockable >* sCommandHeap;
};  // namespace JASKernel

template < typename T >
class JASPoolAllocObject : public JASMemPool< T > {
public:
    static JASPoolAllocObject< T > memPool_;
};

template < typename T >
class JASMemPool_MultiThreaded : public JASGenericMemPool {
public:
    void* alloc(u32 size) NO_INLINE {
        {
            JASThreadingModel::InterruptsDisable< JASMemPool_MultiThreaded >::Lock lock(*this);
            return JASGenericMemPool::alloc(size);
        }
    }
    void free(void* addr, u32 size) NO_INLINE {
        {
            JASThreadingModel::InterruptsDisable< JASMemPool_MultiThreaded >::Lock lock(*this);
            JASGenericMemPool::free(addr, size);
        }
    }
};

template < typename T >
class JASPoolAllocObject_MultiThreaded {
public:
    static JASMemPool_MultiThreaded< T > memPool_;

#ifdef __MWERKS__
    static void* operator new(u32 size) NO_INLINE {
        return memPool_.alloc(size);
    }
#endif
    static void operator delete(void* addr, u32 size) NO_INLINE {
        memPool_.free(addr, size);
    }
};

template < typename T >
JASMemPool_MultiThreaded< T > JASPoolAllocObject_MultiThreaded< T >::memPool_ = JASMemPool_MultiThreaded< T >();
