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

#endif  // PF_CACHE_H
