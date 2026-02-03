#ifndef PDM_DSKMNG_H
#define PDM_DSKMNG_H

#include "revolution/vf/vf_struct.h"

s32 VFipdm_init_diskmanager(u32 config, void* param);
s32 VFipdm_open_disk(PDM_INIT_DISK* p_init_disk_tbl, PDM_DISK** pp_disk);
s32 VFipdm_close_disk(PDM_DISK* p_disk);
s32 VFipdm_open_partition(PDM_DISK* p_disk, s32 part_id, PDM_PARTITION** pp_part);
s32 VFipdm_close_partition(PDM_PARTITION* p_part);

extern PDM_DISK_SET VFipdm_disk_set;

#endif  // PDM_DSKMNG_H
