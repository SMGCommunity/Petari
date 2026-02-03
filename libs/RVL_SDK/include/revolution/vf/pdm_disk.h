#ifndef PDM_DISK_H
#define PDM_DISK_H

#include "revolution/vf/vf_struct.h"

s32 VFipdm_disk_open_disk(PDM_INIT_DISK* p_init_disk_tbl, PDM_DISK** pp_disk);
s32 VFipdm_disk_close_disk(PDM_DISK* p_disk);

s32 VFipdm_disk_check_disk_handle(PDM_DISK* p_disk);

s32 VFipdm_disk_physical_read(PDM_DISK* p_disk, u8* buf, u32 block, u32 num_block, u32* p_num_success);

s32 VFipdm_disk_get_lba_size(PDM_DISK* p_disk, u16* p_lba_size);

#endif  // PDM_DISK_H
