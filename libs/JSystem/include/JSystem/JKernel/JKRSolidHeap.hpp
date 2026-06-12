#pragma once

#include "JSystem/JKernel/JKRHeap.hpp"

class JKRSolidHeap : public JKRHeap {
public:
    static JKRSolidHeap* create(u32, JKRHeap*, bool);

    u32 _6C;
    u32 _70;
    u32 _74;
    u32 _78;
};

inline JKRSolidHeap* JKRCreateSolidHeap(u32 param_0, JKRHeap* heap, bool param_2) {
    return JKRSolidHeap::create(param_0, heap, param_2);
}

inline void JKRDestroySolidHeap(JKRSolidHeap* heap) {
    heap->destroy();
}
