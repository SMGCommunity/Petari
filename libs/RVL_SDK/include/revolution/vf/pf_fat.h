#ifndef PF_FAT_H
#define PF_FAT_H

#include "revolution/vf/vf_struct.h"

s32 VFiPFFAT_ReadFATSector(PF_VOLUME* p_vol, PF_CACHE_PAGE** pp_page, u32 cluster);

s32 VFiPFFAT_ReadFATEntryPage(PF_VOLUME* p_vol, u32 cluster, u32* p_value, PF_CACHE_PAGE** pp_page);

s32 VFiPFFAT_UpdateFATEntry(PF_VOLUME* p_vol, PF_CACHE_PAGE* p_page);

s32 VFiPFFAT_ClearClusterLink(PF_FFD* p_ffd, u32 chain_index);

#endif  // PF_FAT_H
