#ifndef PF_SECTOR_H
#define PF_SECTOR_H

#include "revolution/vf/vf_struct.h"

s32 VFiPFSEC_ReadFAT(PF_VOLUME* p_vol, u8* p_buf, u32 sector, u16 offset, u16 size);

s32 VFiPFSEC_ReadData(PF_VOLUME* p_vol, u8* p_buf, u32 sector, u16 offset, u32 size, u32* p_success_size, u32 set_sig);

s32 VFiPFSEC_ReadDataSector(PF_VOLUME* p_vol, u8* p_buf, u32 sector, u32 size, u32* p_success_size, u32 set_sig);

s32 VFiPFSEC_WriteFAT(PF_VOLUME* p_vol, const u8* p_buf, u32 sector, u16 offset, u16 size);

s32 VFiPFSEC_WriteData(PF_VOLUME* p_vol, const u8* p_buf, u32 sector, u16 offset, u32 size, u32* p_success_size, u32 is_direct, u32 set_sig);

s32 VFiPFSEC_WriteDataSector(PF_VOLUME* p_vol, const u8* p_buf, u32 sector, u32 size, u32* p_success_size, u32 is_direct, u32 set_sig);

#endif  // PF_SECTOR_H
