#pragma once

#include "JKernel/JKRHeap.h"

class JKRSolidHeap : public JKRHeap
{
public:
    static JKRSolidHeap* create(u32, JKRHeap *, bool);
};