#ifndef PF_FAT16_H
#define PF_FAT16_H

#include "revolution/vf/vf_struct.h"

s32 VFiPFFAT16_ReadFATEntry(PF_VOLUME* p_vol, u32 cluster, u32* p_value);

s32 VFiPFFAT16_ReadFATEntryPage(PF_VOLUME* p_vol, u32 cluster, u32* p_value, PF_CACHE_PAGE** pp_page);

s32 VFiPFFAT16_WriteFATEntry(PF_VOLUME* p_vol, u32 cluster, u32 value);

s32 VFiPFFAT16_WriteFATEntryPage(PF_VOLUME* p_vol, u32 cluster, u32 value, PF_CACHE_PAGE** pp_page);

#endif  // PF_FAT16_H
