#ifndef PF_VOLUME_H
#define PF_VOLUME_H

#include "revolution/vf/vf_struct.h"
#include <revolution.h>

s32 VFiPFVOL_errnum(void);
s32 VFiPFVOL_detach(s8);
s32 VFiPFVOL_attach(PF_DRV_TBL*);
s32 VFiPFVOL_getdev(s8 drv_char, PF_DEV_INF* dev_inf);
s32 VFiPFVOL_unmount(s8 drv_char, u32 mode);

s32 VFiPFVOL_CheckForWrite(PF_VOLUME* p_vol);
s32 VFiPFVOL_CheckForRead(PF_VOLUME* p_vol);

void VFiPFVOL_LoadVolumeLabelFromBuf(const u8* buf, PF_VOLUME* p_vol);

#endif  // PF_VOLUME_H
