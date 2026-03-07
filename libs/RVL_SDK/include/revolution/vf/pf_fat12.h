#ifndef PF_FAT12_H
#define PF_FAT12_H

#include "revolution/vf/vf_struct.h"

s32 VFiPFFAT12_ReadFATEntry(PF_VOLUME* p_vol, u16 cluster, u32* p_value);
s32 VFiPFFAT12_ReadFATEntryPage(PF_VOLUME* p_vol, u16 cluster, u32* p_value, PF_CACHE_PAGE** pp_page);
s32 VFiPFFAT12_WriteFATEntry(PF_VOLUME* p_vol, u16 cluster, u16 value);
s32 VFiPFFAT12_WriteFATEntryPage(PF_VOLUME* p_vol, u16 cluster, u16 value, PF_CACHE_PAGE** pp_page);

#endif  // PF_FAT12_H
