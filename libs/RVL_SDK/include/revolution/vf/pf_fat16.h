#ifndef PF_FAT16_H
#define PF_FAT16_H

#include "revolution/vf/vf_struct.h"

s32 VFiPFFAT16_WriteFATEntry(PF_VOLUME* p_vol, u32 cluster, u32 value);

#endif  // PF_FAT16_H
