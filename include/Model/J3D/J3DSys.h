#ifndef J3DSYS_H
#define J3DSYS_H

#include "types.h"

const s32 GPU_FIFO_ADDR = 0xCC008000;

class J3DSys
{
public:
    void loadPosMtxIndx(s32, u16) const;
    void loadNrmMtxIndx(s32, u16) const;
};

#endif // J3DSYS_H