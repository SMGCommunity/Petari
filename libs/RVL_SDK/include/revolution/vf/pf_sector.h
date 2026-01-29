#ifndef PF_SECTOR_H
#define PF_SECTOR_H

#include "revolution/vf/vf_struct.h"

s32 VFiPFSEC_ReadFAT(PF_VOLUME* p_vol, u8* p_buf, u32 sector, u16 offset, u16 size);

s32 VFiPFSEC_WriteFAT(PF_VOLUME* p_vol, const u8* p_buf, u32 sector, u16 offset, u16 size);

#endif  // PF_SECTOR_H
