#pragma once

#include "JSystem/JKernel/JKRHeap.h"

class JKRUnitHeap : public JKRHeap {
public:

    static JKRUnitHeap* create(u32, u32, u32, JKRHeap *, bool);
};