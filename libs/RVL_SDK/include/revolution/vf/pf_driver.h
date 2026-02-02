#ifndef PF_DRIVER_H
#define PF_DRIVER_H

#include "revolution/vf/vf_struct.h"

s32 VFiPFDRV_lwrite(PF_VOLUME* p_vol, const u8* buf, u32 sector, u32 num_sectors, u32* p_num_success);

s32 VFiPFDRV_GetFSINFOInformation(PF_VOLUME* p_vol);

s32 VFiPFDRV_StoreFreeCountToFSINFO(PF_VOLUME* p_vol);

s32 VFiPFDRV_lread(PF_VOLUME* p_vol, u8* buf, u32 sector, u32 num_sectors, u32* p_num_success);

s32 VFiPFDRV_lerase(PF_VOLUME* p_vol, u32 sector, u32 num_sectors);

u32 VFiPFDRV_IsWProtected(PF_VOLUME* p_vol);

u32 VFiPFDRV_IsInserted(PF_VOLUME* p_vol);

s32 VFiPFDRV_mount(PF_VOLUME* p_vol);

s32 VFiPFDRV_unmount(PF_VOLUME* p_vol, u32 mode);

s32 VFiPFDRV_format(PF_VOLUME* p_vol, const u8* param);

#endif  // PF_DRIVER_H
