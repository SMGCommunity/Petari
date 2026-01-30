#include "revolution/vf/pf_fwrite.h"
#include "revolution/vf/pf_file.h"

u32 VFipf2_fwrite(u8* p_buf, u32 size, u32 count, PF_FILE* p_file) {
    u32 count_written;
    VFiPFFILE_fwrite(p_buf, size, count, p_file, &count_written);
    return count_written;
}
