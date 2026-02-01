#include "revolution/vf/pf_service.h"

void VFiPF_LE16_TO_U16_STR(u8* sSrc, u32 num) {
    u32 cnt;
    s8 tmp_src;

    for (cnt = 0; cnt < num; cnt += 2) {
        tmp_src = sSrc[cnt];
        sSrc[cnt] = sSrc[cnt + 1];
        sSrc[cnt + 1] = tmp_src;
    }
}
