#ifndef PF_VOLUME_H
#define PF_VOLUME_H

#include "revolution/vf/vf_struct.h"
#include <revolution.h>


int VFiPFVOL_errnum(void);
int VFiPFVOL_detach(s8);
int VFiPFVOL_attach(PF_DRV_TBL*);
int VFiPFVOL_getdev(s8 drv_char, PF_DEV_INF* dev_inf);
int VFiPFVOL_unmount(s8 drv_char, u32 mode);

#endif  // PF_VOLUME_H
