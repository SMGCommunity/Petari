#ifndef PF_FAT32_H
#define PF_FAT32_H

#include "revolution/vf/vf_struct.h"

s32 VFiPFFAT32_ReadFATEntry(PF_VOLUME* p_vol, u32 cluster, u32* p_value);

s32 VFiPFFAT32_ReadFATEntryPage(PF_VOLUME* p_vol, u32 cluster, u32* p_value, PF_CACHE_PAGE** pp_page);

s32 VFiPFFAT32_WriteFATEntry(PF_VOLUME* p_vol, u32 cluster, u32 value);

#endif  // PF_FAT32_H
