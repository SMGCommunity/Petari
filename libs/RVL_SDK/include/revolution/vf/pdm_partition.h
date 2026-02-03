#ifndef PDM_PARTITION_H
#define PDM_PARTITION_H

#include "revolution/vf/vf_struct.h"

s32 VFipdm_part_logical_read(PDM_PARTITION* p_part, u8* buf, u32 lsector, u32 num_sector, u16 bps, u32* p_num_success);

s32 VFipdm_part_check_media_write_protect(PDM_PARTITION* p_part, u32* is_wprotected);

s32 VFipdm_part_check_media_insert(PDM_PARTITION* p_part, u32* is_insert);
s32 VFipdm_part_check_media_detect(PDM_PARTITION* p_part, u32* is_detect);
s32 VFipdm_part_check_data_erase(PDM_PARTITION* p_part, u32* is_erase);

s32 VFipdm_part_get_media_information(PDM_PARTITION* p_part, PDM_DISK_INFO* p_disk_info);

s32 VFipdm_part_logical_read(PDM_PARTITION* p_part, u8* buf, u32 lsector, u32 num_sector, u16 bps, u32* p_num_success);
s32 VFipdm_part_logical_write(PDM_PARTITION* p_part, const u8* buf, u32 lsector, u32 num_sector, u16 bps, u32* p_num_success);
s32 VFipdm_part_logical_erase(PDM_PARTITION* p_part, u32 lsector, u32 num_sector, u16 bps);

s32 VFipdm_part_format(PDM_PARTITION* p_part, const u8* param);

s32 VFipdm_part_get_permission(PDM_PARTITION* p_part);

s32 VFipdm_part_get_driver_error_code(PDM_PARTITION* p_part);

s32 VFipdm_part_release_permission(PDM_PARTITION* p_part, u32 mode);

s32 VFipdm_part_open_partition(PDM_DISK* p_disk, u16 part_id, PDM_PARTITION** pp_part);
s32 VFipdm_part_close_partition(PDM_PARTITION* p_part);

#endif  // PDM_PARTITION_H
