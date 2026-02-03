#ifndef PDM_DISK_H
#define PDM_DISK_H

#include "revolution/vf/vf_struct.h"

s32 VFipdm_disk_open_disk(PDM_INIT_DISK* p_init_disk_tbl, PDM_DISK** pp_disk);
s32 VFipdm_disk_close_disk(PDM_DISK* p_disk);

#endif  // PDM_DISK_H
