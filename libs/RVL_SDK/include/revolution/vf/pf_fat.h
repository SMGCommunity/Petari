#ifndef PF_FAT_H
#define PF_FAT_H

#include "revolution/vf/vf_struct.h"

s32 VFiPFFAT_ReadFATSector(PF_VOLUME* p_vol, PF_CACHE_PAGE** pp_page, u32 cluster);
s32 VFiPFFAT_GetContinuousSector(PF_FFD* p_ffd, u32 file_sector_index, u32 size, u32* p_sector, u32* p_num_sector);

s32 VFiPFFAT_UpdateFATEntry(PF_VOLUME* p_vol, PF_CACHE_PAGE* p_page);

s32 VFiPFFAT_ClearClusterLink(PF_FFD* p_ffd, u32 chain_index);

s32 VFiPFFAT_WriteFATEntryPage(PF_VOLUME* p_vol, u32 cluster, u32 value, PF_CACHE_PAGE** p_page);

s32 VFiPFFAT_RefreshFSINFO(PF_VOLUME* p_vol);

s32 VFiPFFAT_FreeChain(PF_FFD* p_ffd, u32 start_cluster, u32 chain_index, u32 size);

s32 VFiPFFAT_GetNumberOfCluster(PF_FFD* p_ffd, u32 chain_index, u32 may_allocate, u32*);

s32 VFiPFFAT_GetSectorSpecified(PF_FFD* p_ffd, u32 file_sector_index, u32 may_allocate, u32* p_sector);

s32 VFiPFFAT_CountAllocatedClusters(PF_FFD* p_ffd, u32 size, u32* p_num_alloc_clusters);

void VFiPFFAT_InitHint(PF_FAT_HINT* p_hint);

s32 VFiPFFAT_InitFFD(PF_FFD* p_ffd, PF_FAT_HINT* p_hint, PF_VOLUME* p_vol, u32* p_start_cluster);

void VFiPFFAT_SetHint(PF_FFD* p_ffd, PF_FAT_HINT* p_hint);

void VFiPFFAT_SetLastAccess(PF_FFD* p_ffd, PF_FAT_HINT* last_access);

s32 VFiPFFAT_FinalizeFFD(PF_FFD* p_ffd);

s32 VFiPFFAT_GetSectorAllocated(PF_FFD* p_ffd, u32 file_sector_index, u32 size, u32* p_sector, u32* p_num_sector);

u32 VFiPFFAT_GetValueOfEOC2(PF_VOLUME* p_vol);

s32 VFiPFFAT_ReadValueToSpecifiedCluster(PF_VOLUME* p_vol, u32 cluster, u32* value);

s32 VFiPFFAT_TraceClustersChain(PF_FFD* p_ffd, u32 start_clst, u32 size, u32* p_target_clst, u32* p_next_clst);

s32 VFiPFFAT_UpdateAlternateFATEntry(PF_VOLUME* p_vol, u8* p_buf, u32 sector, u32 size);

s32 VFiPFFAT_InitFATRegion(PF_VOLUME* p_vol);

#endif  // PF_FAT_H
