#ifndef PF_CACHE_H
#define PF_CACHE_H

#include "revolution/vf/vf_struct.h"

s32 VFiPFCACHE_UpdateModifiedSector(PF_VOLUME* p_vol, PF_CACHE_PAGE* p_page, u32 num_sector);

s32 VFiPFCACHE_ReadFATPage(PF_VOLUME* p_vol, u32 sector, PF_CACHE_PAGE** pp_page);

s32 VFiPFCACHE_WriteFATPage(PF_VOLUME* p_vol, PF_CACHE_PAGE* p_page);

s32 VFiPFCACHE_AllocateDataPage(PF_VOLUME* p_vol, u32 sector, PF_CACHE_PAGE** pp_page);

s32 VFiPFCACHE_FlushFATCache(PF_VOLUME* p_vol);

void VFiPFCACHE_FreeDataPage(PF_VOLUME* p_vol, PF_CACHE_PAGE* p_page);

s32 VFiPFCACHE_FlushDataCacheSpecific(PF_VOLUME* p_vol, void* signature);

s32 VFiPFCACHE_ReadDataPage(PF_VOLUME* p_vol, u32 sector, PF_CACHE_PAGE** pp_page, u32 set_sig);

s32 VFiPFCACHE_ReadDataNumSector(PF_VOLUME* p_vol, u8* p_buf, u32 sector, u32 num_sector, u32* p_num_success);

PF_CACHE_PAGE* VFiPFCACHE_SearchDataCache(PF_VOLUME* p_vol, u32 sector);

s32 VFiPFCACHE_ReadDataPageAndFlushIfNeeded(PF_VOLUME* p_vol, u32 sector, PF_CACHE_PAGE** pp_page, u32 set_sig);

s32 VFiPFCACHE_WriteDataPage(PF_VOLUME* p_vol, PF_CACHE_PAGE* p_page, u32 set_sig);

s32 VFiPFCACHE_WriteDataNumSectorAndFreeIfNeeded(PF_VOLUME* p_vol, const u8* p_buf, u32 sector, u32 num_sector, u32* p_num_success);

s32 VFiPFCACHE_InitCaches(PF_VOLUME* p_vol);

void VFiPFCACHE_FreeAllCaches(PF_VOLUME* p_vol);

s32 VFiPFCACHE_FlushAllCaches(PF_VOLUME* p_vol);

#endif  // PF_CACHE_H
