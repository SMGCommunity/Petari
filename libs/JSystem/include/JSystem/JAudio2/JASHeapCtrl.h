#pragma once

#include <revolution.h>

class JASGenericMemPool {
public:
    JASGenericMemPool();
    ~JASGenericMemPool();

    void newMemPool(u32, int);
    void* alloc(u32);

    void free(void *, u32);
    
    u32 _0;
    u32 _4;
    u32 _8;
};

template<typename T>
class JASMemPool : public JASGenericMemPool {
public:
    JASMemPool() : JASGenericMemPool() {

    }

    ~JASMemPool();
};

template <typename T>
class JASPoolAllocObject : public JASMemPool<T> {
public:
    static JASPoolAllocObject<T> memPool_;
};