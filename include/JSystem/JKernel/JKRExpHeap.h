#pragma once

#include "JSystem/JKernel/JKRHeap.h"

class JKRExpHeap : public JKRHeap {
public:

    static JKRExpHeap* create(void *, u32, JKRHeap *, bool);
    static JKRExpHeap* create(u32, JKRHeap *, bool);

    static void createRoot(int, bool);

    u32 _6C;
    u32 _70;
    u32 _74;
    u32 _78;
    u32 _7C;
    u32 _80;
};