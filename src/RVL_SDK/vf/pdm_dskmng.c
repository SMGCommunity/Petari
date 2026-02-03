#include "revolution/vf/pdm_dskmng.h"
#include "revolution/vf/pdm_disk.h"
#include "revolution/vf/pdm_partition.h"
#include "revolution/vf/pf_clib.h"

PDM_DISK_SET VFipdm_disk_set;

s32 VFipdm_init_diskmanager(u32 config, void* param) {
    u32 i;

    VFipf_memset(&VFipdm_disk_set, 0, 3020);
    for (i = 0; i < 0x1A; i++) {
        VFipdm_disk_set.disk[i].disk_lock_handle = 0;
    }

    for (i = 0; i < 0x1A; i++) {
        VFipdm_disk_set.partition[i].part_lock_handle = 0;
    }

    return 0;
}

s32 VFipdm_open_disk(PDM_INIT_DISK* p_init_disk_tbl, PDM_DISK** pp_disk) {
    s32 err;

    if (!pp_disk) {
        return 1;
    }

    *pp_disk = 0;
    err = VFipdm_disk_open_disk(p_init_disk_tbl, pp_disk);

    if (!err) {
        return 0;
    }

    return err;
}

s32 VFipdm_close_disk(PDM_DISK* p_disk) {
    s32 result;

    if (!p_disk) {
        return 1;
    }

    result = VFipdm_disk_close_disk(p_disk);
    return result;
}

s32 VFipdm_open_partition(PDM_DISK* p_disk, s32 part_id, PDM_PARTITION** pp_part) {
    int result;

    if (!p_disk) {
        return 1;
    }

    if (!pp_part) {
        return 1;
    }

    *pp_part = 0;
    result = VFipdm_part_open_partition(p_disk, part_id, pp_part);
    if (!result) {
        return 0;
    }

    return result;
}

s32 VFipdm_close_partition(struct PDM_PARTITION* p_part) {
    int result;

    if (!p_part) {
        return 1;
    }

    result = VFipdm_part_close_partition(p_part);
    if (!result) {
        return 0;
    }

    return result;
}
