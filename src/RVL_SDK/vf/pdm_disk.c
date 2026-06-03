#include "revolution/vf/pdm_partition.h"
#include "revolution/vf/vf_struct.h"

extern PDM_DISK_SET VFipdm_disk_set;

s32 VFipdm_disk_convert_sector_into_block(struct PDM_DISK* p_disk, u32 sector, u32 num_sector, u16 bps, u32* p_block, u32* p_num_block) {
    u16 delta;
    s32 err;

    *p_block = sector;
    err = 0;
    *p_num_block = num_sector;

    if (bps > 512) {
        delta = bps >> 9;

        if (delta == 2) {
            *p_block <<= 1;
        } else if (delta == 4) {
            *p_block <<= 2;
        } else if (delta == 8) {
            *p_block <<= 3;
        }

        if (delta == 2) {
            *p_num_block <<= 1;
        } else if (delta == 4) {
            *p_num_block <<= 2;
        } else if (delta == 8) {
            *p_num_block <<= 3;
        }

    } else {
        if (bps < 512) {
            err = 4;
        }
    }

    return err;
}

s32 VFipdm_disk_convert_block_into_sector(struct PDM_DISK* p_disk, u32 block, u32 num_block, u16 bps, u32* p_sector, u32* p_num_sector) {
    u16 delta;
    s32 err;

    *p_sector = block;
    err = 0;
    *p_num_sector = num_block;

    if (bps > 512) {
        delta = bps >> 9;

        if (delta == 2) {
            *p_sector >>= 1;
        } else if (delta == 4) {
            *p_sector >>= 2;
        } else if (delta == 8) {
            *p_sector >>= 3;
        }

        if (delta == 2) {
            *p_num_sector >>= 1;
        } else if (delta == 4) {
            *p_num_sector >>= 2;
        } else if (delta == 8) {
            *p_num_sector >>= 3;
        }

    } else {
        if (bps < 512) {
            err = 4;
        }
    }

    return err;
}

s32 VFipdm_disk_get_handle(struct PDM_INIT_DISK* p_init_disk_tbl, struct PDM_DISK** pp_disk, u16* p_disk_no, u16* p_handle_no) {
    u16 handle_no;
    u16 disk_no;
    u16 save_disk_no;
    struct PDM_DISK* p_save_free_disk;

    *pp_disk = NULL;
    p_save_free_disk = NULL;
    save_disk_no = 0;

    for (handle_no = 0; handle_no < 26; handle_no++) {
        if (VFipdm_disk_set.disk_handle[handle_no].handle == NULL) {
            break;
        }
    }

    if (handle_no >= 26) {
        return 8;
    }
    *p_handle_no = handle_no;

    for (disk_no = 0; disk_no < 26; disk_no++) {
        if (!(VFipdm_disk_set.disk[disk_no].status & 1)) {
            if (p_save_free_disk == NULL) {
                p_save_free_disk = &VFipdm_disk_set.disk[disk_no];
                save_disk_no = disk_no;
            }
        } else {
            if (VFipdm_disk_set.disk[disk_no].p_init_disk_tbl->p_func == p_init_disk_tbl->p_func &&
                VFipdm_disk_set.disk[disk_no].p_init_disk_tbl->ui_ext == p_init_disk_tbl->ui_ext) {
                *pp_disk = &VFipdm_disk_set.disk[disk_no];
                *p_disk_no = disk_no;
                return 0;
            }
        }
    }

    if (p_save_free_disk == NULL) {
        return 8;
    }

    p_init_disk_tbl->p_func(&p_save_free_disk->disk_tbl, p_init_disk_tbl->ui_ext);
    p_save_free_disk->p_init_disk_tbl = p_init_disk_tbl;
    p_save_free_disk->status |= 1;
    VFipdm_disk_set.num_allocated_disk++;

    *pp_disk = p_save_free_disk;
    *p_disk_no = save_disk_no;

    return 0;
}

static s32 VFipdm_disk_search_handle(struct PDM_DISK* p_disk, struct PDM_DISK* lp_disk, u16* p_handle_no) {
    u16 handle_no;

    for (handle_no = 0; handle_no < 26; handle_no++) {
        if (VFipdm_disk_set.disk_handle[handle_no].handle == lp_disk && VFipdm_disk_set.disk_handle[handle_no].signature == (u32)p_disk >> 16) {
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

s32 VFipdm_disk_do_get_permission(struct PDM_DISK* p_disk, struct PDM_DISK* lp_disk) {
    s32 err;

    if (lp_disk->disk_lock_cnt == 0 || (lp_disk->status & 0x20)) {
        err = lp_disk->disk_tbl.p_func->mount(p_disk);
        if (err != 0) {
            if (lp_disk->p_cur_part != NULL) {
                VFipdm_part_set_driver_error_code(lp_disk->p_cur_part, err);
            }
            return 0x15;
        }

        err = lp_disk->disk_tbl.p_func->get_disk_info(p_disk, &lp_disk->disk_info);
        if (err != 0) {
            if (lp_disk->p_cur_part != NULL) {
                VFipdm_part_set_driver_error_code(lp_disk->p_cur_part, err);
            }

            lp_disk->disk_tbl.p_func->unmount(p_disk);
            return 0x15;
        }

        if (lp_disk->disk_info.bytes_per_sector & 0x1FF) {
            lp_disk->disk_tbl.p_func->unmount(p_disk);
            return 0x16;
        }

        if (lp_disk->disk_info.media_attr & 1) {
            lp_disk->status |= 0x10;
        } else {
            lp_disk->status &= ~0x10;
        }

        lp_disk->status = (lp_disk->status | 2) & ~0x20;
    }

    lp_disk->disk_lock_cnt++;
    return 0;
}

s32 VFipdm_disk_check_disk_handle(struct PDM_DISK* p_disk) {
    s32 err;
    u32 disk_no;
    u32 disk_id;
    u32 disk_sig;
    u16 handle_no;
    struct PDM_DISK* lp_disk;

    disk_no = (u32)p_disk & 0xFF;
    disk_id = (u32)p_disk & 0xFF00;
    disk_sig = (u32)p_disk >> 16;

    if (((u32)p_disk & 0xFF) >= 26 || disk_id != 0x300 || disk_sig > VFipdm_disk_set.disk[disk_no].signature) {
        return 1;
    }

    lp_disk = &VFipdm_disk_set.disk[disk_no];
    return VFipdm_disk_search_handle(p_disk, lp_disk, &handle_no);
}

s32 VFipdm_disk_open_disk(struct PDM_INIT_DISK* p_init_disk_tbl, struct PDM_DISK** pp_disk) {
    s32 err;
    u16 disk_no;
    u16 handle_no;
    struct PDM_DISK* p_disk;

    if (pp_disk == NULL || p_init_disk_tbl == NULL || p_init_disk_tbl->p_func == NULL) {
        return 1;
    }

    err = VFipdm_disk_get_handle(p_init_disk_tbl, &p_disk, &disk_no, &handle_no);
    if (err != 0) {
        return err;
    }

    p_disk->signature++;
    p_disk->open_disk_cnt++;

    VFipdm_disk_set.disk_handle[handle_no].signature = p_disk->signature;
    VFipdm_disk_set.disk_handle[handle_no].handle = p_disk;

    *pp_disk = (struct PDM_DISK*)((disk_no & 0xFF) | 0x300 | (VFipdm_disk_set.disk[disk_no].signature << 16));

    if (p_disk->open_disk_cnt == 1) {
        err = p_disk->disk_tbl.p_func->init(*pp_disk);
        if (err != 0) {
            if (p_disk->open_disk_cnt == 1) {
                p_disk->status &= ~1;
                VFipdm_disk_set.num_allocated_disk--;
            }
            p_disk->open_disk_cnt--;
            VFipdm_disk_set.disk_handle[handle_no].handle = NULL;
            return 0x15;
        }
    }

    return 0;
}

s32 VFipdm_disk_close_disk(struct PDM_DISK* p_disk) {
    s32 err;
    u16 handle_no;
    struct PDM_DISK* lp_disk;

    if (p_disk == NULL) {
        return 1;
    }

    err = VFipdm_disk_check_disk_handle(p_disk);
    if (err != 0) {
        return err;
    }

    lp_disk = &VFipdm_disk_set.disk[(u32)p_disk & 0xFF];

    if (!(lp_disk->status & 1)) {
        return 13;
    }
    if ((lp_disk->status & 2) == 2) {
        return 13;
    }

    err = VFipdm_disk_search_handle(p_disk, lp_disk, &handle_no);

    if (err != 0) {
        return err;
    }

    if (lp_disk->open_disk_cnt == 1) {
        err = lp_disk->disk_tbl.p_func->finalize(p_disk);
        if (err != 0) {
            if (lp_disk->p_cur_part != NULL) {
                VFipdm_part_set_driver_error_code(lp_disk->p_cur_part, err);
            }
            return 0x15;
        }
    }

    if (lp_disk->open_disk_cnt == 1) {
        lp_disk->status &= ~1;
        VFipdm_disk_set.num_allocated_disk--;
    }

    lp_disk->open_disk_cnt--;
    VFipdm_disk_set.disk_handle[handle_no].handle = NULL;

    return 0;
}

s32 VFipdm_disk_get_part_permission(struct PDM_DISK* p_disk) {
    s32 err;
    struct PDM_DISK* lp_disk;

    if (p_disk == NULL) {
        return 1;
    }

    err = VFipdm_disk_check_disk_handle(p_disk);
    if (err != 0) {
        return err;
    }

    lp_disk = &VFipdm_disk_set.disk[(u32)p_disk & 0xFF];
    err = VFipdm_disk_do_get_permission(p_disk, lp_disk);

    return err;
}

s32 VFipdm_disk_release_part_permission(struct PDM_DISK* p_disk, u32 mode) {
    s32 err;
    struct PDM_DISK* lp_disk;

    if (p_disk == NULL) {
        return 1;
    }

    err = VFipdm_disk_check_disk_handle(p_disk);
    if (err != 0) {
        return err;
    }

    lp_disk = &VFipdm_disk_set.disk[(u32)p_disk & 0xFF];

    if (lp_disk->disk_lock_cnt == 0) {
        return 14;
    }

    err = 0;
    if (lp_disk->disk_lock_cnt == 1) {
        err = lp_disk->disk_tbl.p_func->unmount(p_disk);
        if (err != 0) {
            if (lp_disk->p_cur_part != NULL) {
                VFipdm_part_set_driver_error_code(lp_disk->p_cur_part, err);
            }
            err = 0x15;
        }
    }

    if (err == 0 || mode == 1) {
        lp_disk->disk_lock_cnt--;
        if (lp_disk->disk_lock_cnt == 0) {
            lp_disk->status &= ~2;
        }
    }

    return err;
}

s32 VFipdm_disk_physical_read(struct PDM_DISK* p_disk, u8* buf, u32 psector, u32 num_sector, u16 bps, u32* p_num_success) {
    s32 err;
    u32 block;
    u32 num_block;
    u32 tmp_sector;
    struct PDM_DISK* lp_disk;

    if (p_disk == NULL || buf == NULL || num_sector == 0 || bps == 0 || p_num_success == NULL) {
        return 1;
    }

    err = VFipdm_disk_check_disk_handle(p_disk);
    if (err != 0) {
        return err;
    }

    lp_disk = &VFipdm_disk_set.disk[(u32)p_disk & 0xFF];

    err = VFipdm_disk_convert_sector_into_block(lp_disk, psector, num_sector, bps, &block, &num_block);
    if (err != 0) {
        return err;
    }

    err = lp_disk->disk_tbl.p_func->physical_read(p_disk, buf, block, num_block, p_num_success);

    VFipdm_disk_convert_block_into_sector(lp_disk, block, *p_num_success, bps, &tmp_sector, p_num_success);

    if (err != 0) {
        if (lp_disk->p_cur_part != NULL) {
            VFipdm_part_set_driver_error_code(lp_disk->p_cur_part, err);
        }
        return 0x15;
    }

    return 0;
}

s32 VFipdm_disk_physical_write(struct PDM_DISK* p_disk, const u8* buf, u32 psector, u32 num_sector, u16 bps, u32* p_num_success) {
    s32 err;
    u32 block;
    u32 num_block;
    u32 tmp_sector;
    struct PDM_DISK* lp_disk;

    if (p_disk == NULL || buf == NULL || num_sector == 0 || bps == 0 || p_num_success == NULL) {
        return 1;
    }

    err = VFipdm_disk_check_disk_handle(p_disk);
    if (err != 0) {
        return err;
    }

    lp_disk = &VFipdm_disk_set.disk[(u32)p_disk & 0xFF];

    err = VFipdm_disk_convert_sector_into_block(lp_disk, psector, num_sector, bps, &block, &num_block);
    if (err != 0) {
        return err;
    }

    err = lp_disk->disk_tbl.p_func->physical_write(p_disk, (u8*)buf, block, num_block, p_num_success);

    VFipdm_disk_convert_block_into_sector(lp_disk, block, *p_num_success, bps, &tmp_sector, p_num_success);

    if (err != 0) {
        if (lp_disk->p_cur_part != NULL) {
            VFipdm_part_set_driver_error_code(lp_disk->p_cur_part, err);
        }
        return 0x15;
    }

    return 0;
}

s32 VFipdm_disk_format(struct PDM_DISK* p_disk, const u8* param) {
    s32 err;
    struct PDM_DISK* lp_disk;

    if (p_disk == NULL) {
        return 1;
    }

    err = VFipdm_disk_check_disk_handle(p_disk);
    if (err != 0) {
        return err;
    }

    lp_disk = &VFipdm_disk_set.disk[(u32)p_disk & 0xFF];

    err = lp_disk->disk_tbl.p_func->format(p_disk, (u8*)param);
    if (err != 0) {
        if (lp_disk->p_cur_part != NULL) {
            VFipdm_part_set_driver_error_code(lp_disk->p_cur_part, err);
        }
        return 0x15;
    }

    return 0;
}

s32 VFipdm_disk_get_lba_size(struct PDM_DISK* p_disk, u16* p_lba_size) {
    struct PDM_DISK* lp_disk;
    s32 err;

    if (p_disk == NULL || p_lba_size == NULL) {
        return 1;
    }

    err = VFipdm_disk_check_disk_handle(p_disk);
    if (err != 0) {
        return err;
    }

    lp_disk = &VFipdm_disk_set.disk[(u32)p_disk & 0xFF];
    *p_lba_size = lp_disk->disk_info.bytes_per_sector;

    return 0;
}

s32 VFipdm_disk_get_media_information(struct PDM_DISK* p_disk, struct PDM_DISK_INFO* p_disk_info) {
    struct PDM_DISK* lp_disk;
    s32 err;

    if (p_disk == NULL || p_disk_info == NULL) {
        return 1;
    }

    err = VFipdm_disk_check_disk_handle(p_disk);
    if (err != 0) {
        return err;
    }

    lp_disk = &VFipdm_disk_set.disk[(u32)p_disk & 0xFF];
    *p_disk_info = lp_disk->disk_info;

    return 0;
}

s32 VFipdm_disk_check_media_insert(struct PDM_DISK* p_disk, u32* is_insert) {
    struct PDM_DISK* lp_disk;
    s32 err;

    if (p_disk == NULL || is_insert == NULL) {
        return 1;
    }

    err = VFipdm_disk_check_disk_handle(p_disk);
    if (err != 0) {
        return err;
    }

    lp_disk = &VFipdm_disk_set.disk[(u32)p_disk & 0xFF];

    if (lp_disk->status & 4) {
        *is_insert = 1;
    } else {
        *is_insert = 0;
    }

    return 0;
}

s32 VFipdm_disk_set_disk(struct PDM_DISK* p_disk, struct PDM_PARTITION* p_part) {
    struct PDM_DISK* lp_disk;
    s32 err;

    if (p_disk == NULL || p_part == NULL) {
        return 1;
    }

    err = VFipdm_disk_check_disk_handle(p_disk);
    if (err != 0) {
        return err;
    }

    lp_disk = &VFipdm_disk_set.disk[(u32)p_disk & 0xFF];
    lp_disk->p_cur_part = p_part;

    return 0;
}

void VFipdm_disk_notify_media_insert(struct PDM_DISK* p_disk) {
    struct PDM_DISK* lp_disk;

    if (VFipdm_disk_check_disk_handle(p_disk) != 0) {
        return;
    }

    lp_disk = &VFipdm_disk_set.disk[(u32)p_disk & 0xFF];
    lp_disk->status |= 0x2C;
    VFipdm_part_set_change_media_state(p_disk, 1);
}
