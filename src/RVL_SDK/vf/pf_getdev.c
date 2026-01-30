#include "revolution/vf/pf_getdev.h"
#include "revolution/vf/pf_api_util.h"
#include "revolution/vf/pf_volume.h"

s32 VFipf2_devinf(u8 drv_char, PF_DEV_INF* dev_inf) {
    int result = VFiPFVOL_getdev(drv_char, dev_inf);
    return VFiPFAPI_convertReturnValue(result);
}
