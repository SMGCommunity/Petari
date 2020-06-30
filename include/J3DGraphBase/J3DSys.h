#pragma once

#include <revolution.h>

const s32 GPU_FIFO_ADDR = 0xCC008000;

class J3DSys
{
public:
    void loadPosMtxIndx(s32, u16) const;
    void loadNrmMtxIndx(s32, u16) const;
};