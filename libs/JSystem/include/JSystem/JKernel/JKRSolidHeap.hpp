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