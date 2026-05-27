#ifndef D_COMMON_H
#define D_COMMON_H

#include "revolution/vf/vf_struct.h"

s32 dCommon_FlushFromVol(PF_VOLUME* i_vol_p, int i_setLastDeviceError);

u32 dCommon_getResvSecNumFromDisk(struct PDM_DISK* p_disk);

#endif  // D_COMMON_H
