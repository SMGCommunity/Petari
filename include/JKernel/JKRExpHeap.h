#pragma once

#include "JKernel/JKRHeap.h"

class JKRExpHeap : public JKRHeap
{
public:
    void adjustSize();

    static JKRExpHeap* create(void *, u32, JKRHeap *, bool);
    static JKRExpHeap* create(u32, JKRHeap *, bool);
    static JKRHeap* createRoot(int, bool);
};