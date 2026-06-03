#ifndef PDM_DISK_H
#define PDM_DISK_H

#include "revolution/vf/vf_struct.h"

s32 VFipdm_disk_open_disk(PDM_INIT_DISK* p_init_disk_tbl, PDM_DISK** pp_disk);
s32 VFipdm_disk_close_disk(PDM_DISK* p_disk);

s32 VFipdm_disk_check_disk_handle(PDM_DISK* p_disk);

s32 VFipdm_disk_physical_read(struct PDM_DISK* p_disk, u8* buf, u32 psector, u32 num_sector, u16 bps, u32* p_num_success);

s32 VFipdm_disk_get_lba_size(PDM_DISK* p_disk, u16* p_lba_size);

s32 VFipdm_disk_get_media_information(PDM_DISK* p_disk, PDM_DISK_INFO* p_disk_info);

s32 VFipdm_disk_check_media_insert(PDM_DISK* p_disk, u32* is_insert);

s32 VFipdm_disk_set_disk(struct PDM_DISK* p_disk, struct PDM_PARTITION* p_part);

s32 VFipdm_disk_get_part_permission(struct PDM_DISK* p_disk);

s32 VFipdm_disk_release_part_permission(struct PDM_DISK* p_disk, u32 mode);

s32 VFipdm_disk_format(struct PDM_DISK* p_disk, const u8* param);

s32 VFipdm_disk_physical_write(struct PDM_DISK* p_disk, const u8* buf, u32 psector, u32 num_sector, u16 bps, u32* p_num_success);

#endif  // PDM_DISK_H
