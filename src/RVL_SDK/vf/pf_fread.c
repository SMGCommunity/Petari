#include "revolution/vf/pf_file.h"

int VFipf2_fread(u8* p_buf, u32 size, u32 count, PF_FILE* p_file) {
    u32 count_read;
    VFiPFFILE_fread(p_buf, size, count, p_file, &count_read);
    return count_read;
}
