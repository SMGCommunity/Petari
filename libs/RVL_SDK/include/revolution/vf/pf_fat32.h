#ifndef PF_FAT32_H
#define PF_FAT32_H

#include "revolution/vf/vf_struct.h"

s32 VFiPFFAT32_WriteFATEntry(PF_VOLUME* p_vol, u32 cluster, u32 value);

#endif  // PF_FAT32_H
