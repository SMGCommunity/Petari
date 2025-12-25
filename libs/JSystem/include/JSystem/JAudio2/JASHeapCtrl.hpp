#pragma once

#include <revolution.h>

#include "JSystem/JAudio2/JASTaskThread.hpp"

class JASGenericMemPool {
public:
    JASGenericMemPool();
    ~JASGenericMemPool();

    void newMemPool(u32, int);
    void* alloc(u32);

    void free(void*, u32);

    u32 _0;
    u32 _4;
    u32 _8;
};

template < typename T >
class JASMemPool : public JASGenericMemPool {
public:
    JASMemPool() : JASGenericMemPool() {}

    ~JASMemPool();
};

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
    static void* operator new(u32 size) NO_INLINE { return memPool_.alloc(size); }
#endif
    static void operator delete(void* addr, u32 size) NO_INLINE { memPool_.free(addr, size); }
};

template < typename T >
JASMemPool_MultiThreaded< T > JASPoolAllocObject_MultiThreaded< T >::memPool_ = JASMemPool_MultiThreaded< T >();