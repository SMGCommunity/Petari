#ifndef PDM_MBR_H
#define PDM_MBR_H

#include "revolution/vf/vf_struct.h"

s32 VFipdm_mbr_get_table(u8* buf, u32 sector, PDM_MBR* p_mbr_tb);

s32 VFipdm_mbr_check_master_boot_record(PDM_DISK* p_disk, u8* buf, u32* is_master_boot);

#endif  // PDM_MBR_H
