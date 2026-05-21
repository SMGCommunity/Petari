#include "revolution/vf/pdm_disk.h"
#include "revolution/vf/pdm_mbr.h"
#include "revolution/vf/vf_struct.h"

extern PDM_DISK_SET VFipdm_disk_set;

static s32 VFipdm_part_search_handle(struct PDM_PARTITION* p_part, struct PDM_PARTITION* lp_part, u16* p_handle_no) {
    u16 handle_no;

    for (handle_no = 0; handle_no < 26; handle_no++) {
        if (VFipdm_disk_set.partition_handle[handle_no].handle == lp_part &&
            VFipdm_disk_set.partition_handle[handle_no].signature == ((u32)p_part >> 16)) {
            break;
        }
    }

    if (handle_no >= 26) {
        return 1;
    } else {
        *p_handle_no = handle_no;
        return 0;
    }
}

static s32 VFipdm_part_check_partition_handle(struct PDM_PARTITION* p_part) {
    s32 err;
    u32 part_no;
    u32 part_id;
    u32 part_sig;
    u16 handle_no;
    struct PDM_PARTITION* lp_part;

    part_no = (u32)p_part & 0xFF;
    part_id = (u32)p_part & 0xFF00;
    part_sig = (u32)p_part >> 16;

    if (((u32)p_part & 0xFF) >= 26 || part_id != 0x400 || part_sig > VFipdm_disk_set.partition[part_no].signature) {
        return 1;
    }

    lp_part = &VFipdm_disk_set.partition[part_no];
    return VFipdm_part_search_handle(p_part, lp_part, &handle_no);
}

s32 VFipdm_part_get_start_sector(struct PDM_PARTITION* p_part) {
    s32 err;
    u16 part_id;
    u16 part_index;
    struct PDM_MBR mbr_tbl;
    struct PDM_DISK_INFO disk_info;

    part_id = p_part->part_id;
    err = VFipdm_mbr_get_mbr_part_table(p_part->p_disk, &mbr_tbl);
    if (err != 0 && err != 5) {
        return err;
    }

    if (err != 5) {
        if (part_id < 4) {
            if (mbr_tbl.partition_table[part_id].lba_start_sector == 0) {
                return 7;
            }
            p_part->start_sector = mbr_tbl.partition_table[part_id].lba_start_sector;
            p_part->total_sector = mbr_tbl.partition_table[part_id].lba_num_sectors;
            p_part->partition_type = mbr_tbl.partition_table[part_id].partition_type;
            p_part->mbr_sector = mbr_tbl.current_sector;
        } else {
            part_index = 4;
            while (1) {
                err = VFipdm_mbr_get_epbr_part_table(p_part->p_disk, &mbr_tbl);
                if (err != 0 && err != 6) {
                    return err;
                }

                if (err != 6) {
                    if (part_id == part_index) {
                        p_part->start_sector = mbr_tbl.partition_table[0].lba_start_sector;
                        p_part->total_sector = mbr_tbl.partition_table[0].lba_num_sectors;
                        p_part->partition_type = mbr_tbl.partition_table[0].partition_type;
                        p_part->mbr_sector = mbr_tbl.current_sector;
                        break;
                    }
                } else {
                    return 7;
                }
                part_index++;
            }
        }
    } else {
        if (part_id >= 1) {
            return 7;
        }
        err = VFipdm_disk_get_media_information(p_part->p_disk, &disk_info);
        if (err != 0) {
            return err;
        }
        p_part->start_sector = 0;
        p_part->total_sector = disk_info.total_sectors;
        p_part->partition_type = 0;
        p_part->mbr_sector = 0;
    }
    return 0;
}

u32 VFipdm_part_chg_ltop(struct PDM_PARTITION* p_part, u32 lsector, u16 lbps) {
    u32 psector;
    u32 tmp_start_sector;
    u16 lba_size;
    u16 l_delta;
    u16 p_delta;

    VFipdm_disk_get_lba_size(p_part->p_disk, &lba_size);

    if (lbps == lba_size) {
        psector = lsector + p_part->start_sector + p_part->mbr_sector;
    } else {
        p_delta = lba_size / 512;
        tmp_start_sector = p_part->start_sector + p_part->mbr_sector;

        if (p_delta == 2) {
            tmp_start_sector <<= 1;
        } else if (p_delta == 4) {
            tmp_start_sector <<= 2;
        } else if (p_delta == 8) {
            tmp_start_sector <<= 3;
        }

        l_delta = lbps / 512;
        if (l_delta == 2) {
            tmp_start_sector >>= 1;
        } else if (l_delta == 4) {
            tmp_start_sector >>= 2;
        } else if (l_delta == 8) {
            tmp_start_sector >>= 3;
        }

        psector = lsector + tmp_start_sector;
    }

    return psector;
}

s32 VFipdm_part_get_handle(struct PDM_DISK* p_disk, u16 part_id, struct PDM_PARTITION** pp_part, u16* p_part_no, u16* p_handle_no) {
    u16 handle_no;
    u16 part_no;
    u16 save_part_no;
    struct PDM_PARTITION* p_save_free_part;

    *pp_part = NULL;
    p_save_free_part = NULL;
    save_part_no = 0;

    for (handle_no = 0; handle_no < 26; handle_no++) {
        if (VFipdm_disk_set.partition_handle[handle_no].handle == NULL) {
            break;
        }
    }

    if (handle_no >= 26) {
        return 10;
    }
    *p_handle_no = handle_no;

    for (part_no = 0; part_no < 26; part_no++) {
        if (!(VFipdm_disk_set.partition[part_no].status & 1)) {
            if (p_save_free_part == NULL) {
                p_save_free_part = &VFipdm_disk_set.partition[part_no];
                save_part_no = part_no;
            }
        } else {
            if (VFipdm_disk_set.partition[part_no].part_id == part_id && VFipdm_disk_set.partition[part_no].p_disk == p_disk) {
                *pp_part = &VFipdm_disk_set.partition[part_no];
                *p_part_no = part_no;
                return 0;
            }
        }
    }

    if (p_save_free_part == NULL) {
        return 10;
    }

    p_save_free_part->status |= 1;
    VFipdm_disk_set.num_partition++;
    p_save_free_part->p_disk = p_disk;
    p_save_free_part->part_id = part_id;

    *pp_part = p_save_free_part;
    *p_part_no = save_part_no;

    return 0;
}

s32 VFipdm_part_open_partition(struct PDM_DISK* p_disk, u16 part_id, struct PDM_PARTITION** pp_part) {
    s32 err;
    u16 part_no;
    u16 handle_no;
    struct PDM_PARTITION* p_part;
    u32 is_insert;

    if (p_disk == NULL || pp_part == NULL) {
        return 1;
    }

    err = VFipdm_part_get_handle(p_disk, part_id, &p_part, &part_no, &handle_no);
    if (err != 0) {
        return err;
    }

    p_part->signature++;
    p_part->open_part_cnt++;

    VFipdm_disk_set.partition_handle[handle_no].signature = p_part->signature;
    VFipdm_disk_set.partition_handle[handle_no].handle = p_part;

    err = VFipdm_disk_check_media_insert(p_disk, &is_insert);
    if (err != 0) {
        return err;
    }

    if (is_insert != 0) {
        p_part->status |= 4;
    }

    *pp_part = (struct PDM_PARTITION*)((part_no & 0xFF) | 0x400 | (VFipdm_disk_set.partition[part_no].signature << 16));

    return 0;
}

s32 VFipdm_part_close_partition(struct PDM_PARTITION* p_part) {
    s32 err;
    u16 handle_no;
    struct PDM_PARTITION* lp_part;

    if (p_part == NULL) {
        return 1;
    }

    err = VFipdm_part_check_partition_handle(p_part);
    if (err != 0) {
        return err;
    }

    lp_part = &VFipdm_disk_set.partition[(u32)p_part & 0xFF];

    if (!(lp_part->status & 1)) {
        return 12;
    }
    if ((lp_part->status & 2) == 2) {
        return 13;
    }

    err = VFipdm_part_search_handle(p_part, lp_part, &handle_no);
    if (err != 0) {
        return err;
    }

    if (lp_part->open_part_cnt == 1) {
        lp_part->status &= ~1;
        VFipdm_disk_set.num_partition--;
    }

    lp_part->open_part_cnt--;
    VFipdm_disk_set.partition_handle[handle_no].handle = NULL;

    return 0;
}

s32 VFipdm_part_get_permission(struct PDM_PARTITION* p_part) {
    s32 err;
    struct PDM_PARTITION* lp_part;
    struct PDM_DISK_INFO disk_info;

    if (p_part == NULL) {
        return 1;
    }

    err = VFipdm_part_check_partition_handle(p_part);
    if (err != 0) {
        return err;
    }

    lp_part = &VFipdm_disk_set.partition[(u32)p_part & 0xFF];

    if (!(lp_part->status & 2)) {
        err = VFipdm_disk_set_disk(lp_part->p_disk, p_part);
        if (err != 0) {
            return err;
        }

        err = VFipdm_disk_get_part_permission(lp_part->p_disk);
        if (err != 0) {
            return err;
        }

        err = VFipdm_disk_get_media_information(lp_part->p_disk, &disk_info);
        if (err != 0) {
            VFipdm_disk_release_part_permission(lp_part->p_disk, 1);
            return err;
        }

        if (disk_info.media_attr & 1) {
            lp_part->status |= 0x10;
        } else {
            lp_part->status &= ~0x10;
        }

        err = VFipdm_part_get_start_sector(lp_part);
        if (err != 0) {
            VFipdm_disk_release_part_permission(lp_part->p_disk, 1);
            return err;
        }
    } else {
        return 13;
    }

    lp_part->status = (lp_part->status | 2) & ~0x8;
    return 0;
}

s32 VFipdm_part_release_permission(struct PDM_PARTITION* p_part, u32 mode) {
    s32 err;
    s32 ret;
    struct PDM_PARTITION* lp_part;

    ret = 0;

    if (p_part == NULL) {
        return 1;
    }

    err = VFipdm_part_check_partition_handle(p_part);
    if (err != 0) {
        return err;
    }

    lp_part = &VFipdm_disk_set.partition[(u32)p_part & 0xFF];

    if (!(lp_part->status & 2)) {
        return 14;
    }

    err = VFipdm_disk_set_disk(lp_part->p_disk, p_part);
    if (err != 0) {
        return err;
    }

    err = VFipdm_disk_release_part_permission(lp_part->p_disk, mode);
    if (err != 0) {
        ret = err;
    }

    if (ret == 0 || mode == 1) {
        lp_part->status = lp_part->status & ~0x2;
        lp_part->status = lp_part->status & ~0x18;
    }

    return ret;
}

s32 VFipdm_part_format(struct PDM_PARTITION* p_part, const u8* param) {
    s32 err;
    struct PDM_PARTITION* lp_part;

    if (p_part == NULL) {
        return 1;
    }

    err = VFipdm_part_check_partition_handle(p_part);
    if (err != 0) {
        return err;
    }

    lp_part = &VFipdm_disk_set.partition[(u32)p_part & 0xFF];

    err = VFipdm_disk_set_disk(lp_part->p_disk, p_part);
    if (err != 0) {
        return err;
    }

    err = VFipdm_disk_format(lp_part->p_disk, param);
    {
        s32 mask = (-err | err) >> 31;
        return err & mask;
    }
}

s32 VFipdm_part_logical_read(struct PDM_PARTITION* p_part, u8* buf, u32 lsector, u32 num_sector, u16 bps, u32* p_num_success) {
    s32 err;
    u32 over_sec;
    u32 psector;
    struct PDM_PARTITION* lp_part;

    if (p_part == NULL || buf == NULL || num_sector == 0 || bps == 0 || p_num_success == NULL) {
        return 1;
    }

    err = VFipdm_part_check_partition_handle(p_part);
    if (err != 0) {
        return err;
    }

    lp_part = &VFipdm_disk_set.partition[(u32)p_part & 0xFF];

    err = VFipdm_disk_set_disk(lp_part->p_disk, p_part);
    if (err != 0) {
        return err;
    }

    if (lp_part->total_sector <= lsector) {
        return 18;
    }

    over_sec = lsector + num_sector;
    if (lp_part->total_sector < over_sec) {
        num_sector -= (over_sec - lp_part->total_sector);
    }

    psector = VFipdm_part_chg_ltop(lp_part, lsector, bps);
    err = VFipdm_disk_physical_read(lp_part->p_disk, buf, psector, num_sector, bps, p_num_success);
    if (err != 0) {
        return err;
    }

    return 0;
}

s32 VFipdm_part_logical_write(struct PDM_PARTITION* p_part, const u8* buf, u32 lsector, u32 num_sector, u16 bps, u32* p_num_success) {
    s32 err;
    u32 over_sec;
    u32 psector;
    struct PDM_PARTITION* lp_part;

    if (p_part == NULL || buf == NULL || num_sector == 0 || bps == 0 || p_num_success == NULL) {
        return 1;
    }

    err = VFipdm_part_check_partition_handle(p_part);
    if (err != 0) {
        return err;
    }

    lp_part = &VFipdm_disk_set.partition[(u32)p_part & 0xFF];

    err = VFipdm_disk_set_disk(lp_part->p_disk, p_part);
    if (err != 0) {
        return err;
    }

    if (lp_part->total_sector <= lsector) {
        return 18;
    }

    over_sec = lsector + num_sector;
    if (lp_part->total_sector < over_sec) {
        num_sector -= (over_sec - lp_part->total_sector);
    }

    psector = VFipdm_part_chg_ltop(lp_part, lsector, bps);
    err = VFipdm_disk_physical_write(lp_part->p_disk, buf, psector, num_sector, bps, p_num_success);
    if (err != 0) {
        return err;
    }

    return 0;
}

s32 VFipdm_part_get_media_information(struct PDM_PARTITION* p_part, struct PDM_DISK_INFO* p_disk_info) {
    s32 err;
    struct PDM_PARTITION* lp_part;

    if (p_part == NULL || p_disk_info == NULL) {
        return 1;
    }

    err = VFipdm_part_check_partition_handle(p_part);
    if (err != 0) {
        return err;
    }

    lp_part = &VFipdm_disk_set.partition[(u32)p_part & 0xFF];
    err = VFipdm_disk_get_media_information(lp_part->p_disk, p_disk_info);
    {
        s32 mask = (-err | err) >> 31;
        return err & mask;
    }
}

s32 VFipdm_part_check_media_write_protect(struct PDM_PARTITION* p_part, u32* is_wprotect) {
    struct PDM_PARTITION* lp_part;
    s32 err;

    if (p_part == NULL || is_wprotect == NULL) {
        return 1;
    }

    err = VFipdm_part_check_partition_handle(p_part);
    if (err != 0) {
        return err;
    }

    lp_part = &VFipdm_disk_set.partition[(u32)p_part & 0xFF];

    if (lp_part->status & 0x10) {
        *is_wprotect = 1;
    } else {
        *is_wprotect = 0;
    }

    return 0;
}

s32 VFipdm_part_check_media_insert(struct PDM_PARTITION* p_part, u32* is_insert) {
    struct PDM_PARTITION* lp_part;
    s32 err;

    if (p_part == NULL || is_insert == NULL) {
        return 1;
    }

    err = VFipdm_part_check_partition_handle(p_part);
    if (err != 0) {
        return err;
    }

    lp_part = &VFipdm_disk_set.partition[(u32)p_part & 0xFF];

    if (lp_part->status & 4) {
        *is_insert = 1;
    } else {
        *is_insert = 0;
    }

    return 0;
}

s32 VFipdm_part_check_media_detect(struct PDM_PARTITION* p_part, u32* is_detect) {
    struct PDM_PARTITION* lp_part;
    s32 err;

    if (p_part == NULL || is_detect == NULL) {
        return 1;
    }

    err = VFipdm_part_check_partition_handle(p_part);
    if (err != 0) {
        return err;
    }

    lp_part = &VFipdm_disk_set.partition[(u32)p_part & 0xFF];

    if (lp_part->status & 8) {
        *is_detect = 1;
    } else {
        *is_detect = 0;
    }

    return 0;
}

void VFipdm_part_set_change_media_state(struct PDM_DISK* p_disk, u32 event) {
    u16 part_cnt;
    struct PDM_PARTITION* p_part;

    for (part_cnt = 0; part_cnt < 26; part_cnt++) {
        if (((&VFipdm_disk_set.partition[part_cnt])->status & 1) && ((&VFipdm_disk_set.partition[part_cnt])->p_disk == p_disk)) {
            if (event == 0) {
                (&VFipdm_disk_set.partition[part_cnt])->status &= ~4;
            } else {
                (&VFipdm_disk_set.partition[part_cnt])->status |= 4;
            }
            (&VFipdm_disk_set.partition[part_cnt])->status |= 8;
        }
    }
}

void VFipdm_part_set_driver_error_code(struct PDM_PARTITION* p_part, s32 error_code) {
    struct PDM_PARTITION* lp_part;
    lp_part = &VFipdm_disk_set.partition[(u32)p_part & 0xFF];
    lp_part->driver_last_error = error_code;
}

s32 VFipdm_part_get_driver_error_code(struct PDM_PARTITION* p_part) {
    struct PDM_PARTITION* lp_part;
    lp_part = &VFipdm_disk_set.partition[(u32)p_part & 0xFF];
    return lp_part->driver_last_error;
}
