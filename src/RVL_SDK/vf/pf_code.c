#include "revolution/vf/pf_code.h"

int VFiPFCODE_Combine_Width(s16 oem_width, s16 uni_width) {
    return (oem_width << 16) + uni_width;
}

void VFiPFCODE_Divide_Width(u32 width, s16* oem_width, s16* uni_width) {
    *oem_width = width >> 16;
    *uni_width = width;
}
