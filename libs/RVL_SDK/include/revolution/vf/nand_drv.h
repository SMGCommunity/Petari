#ifndef NAND_DRV_H
#define NAND_DRV_H

#include "revolution/vf/vf_struct.h"

s32 nanddrv_init(struct PDM_DISK* p_disk);
s32 nanddrv_mount(struct PDM_DISK* p_disk);
s32 nanddrv_format(struct PDM_DISK* p_disk, const u8* param);
s32 nanddrv_pread(struct PDM_DISK* p_disk, u8* p_buf, u32 block, u32 num_blocks, u32* p_num_success);
s32 nanddrv_pwrite(struct PDM_DISK* p_disk, const u8* p_buf, u32 block, u32 num_blocks, u32* p_num_success);
s32 nanddrv_unmount(struct PDM_DISK* p_disk);
s32 nanddrv_finalize(struct PDM_DISK* p_disk);
s32 nanddrv_get_disk_info(struct PDM_DISK* p_disk, struct PDM_DISK_INFO* p_disk_info);

#endif  // NAND_DRV_H
