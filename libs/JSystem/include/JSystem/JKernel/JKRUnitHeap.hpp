#pragma once

#include "JSystem/JKernel/JKRHeap.hpp"

class JKRUnitHeap : public JKRHeap {
public:
    static JKRUnitHeap* create(u32, u32, u32, JKRHeap*, bool);
};