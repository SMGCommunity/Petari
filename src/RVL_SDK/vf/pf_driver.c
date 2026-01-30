#include "revolution/vf/pf_driver.h"
#include "revolution/vf/pdm_bpb.h"
#include "revolution/vf/pdm_partition.h"
#include "revolution/vf/pf_cache.h"

int VFiPFDRV_GetBPBInformation(unsigned char* p_buf, PF_BPB* p_bpb) {
    struct PDM_BPB bpb;

    if (VFipdm_bpb_get_bpb_information(p_buf, &bpb)) {
        return 7;
    }

    p_bpb->bytes_per_sector = bpb.bytes_per_sector;
    p_bpb->num_reserved_sectors = bpb.num_reserved_sectors;
    p_bpb->num_root_dir_entries = bpb.num_root_dir_entries;
    p_bpb->sectors_per_cluster = bpb.sectors_per_cluster;
    p_bpb->num_FATs = bpb.num_FATs;
    p_bpb->total_sectors = bpb.total_sectors;
    p_bpb->sectors_per_FAT = bpb.sectors_per_FAT;
    p_bpb->root_dir_cluster = bpb.root_dir_cluster;
    p_bpb->fs_info_sector = bpb.fs_info_sector;
    p_bpb->backup_boot_sector = bpb.backup_boot_sector;
    p_bpb->ext_flags = bpb.ext_flags;
    p_bpb->media = bpb.media;
    p_bpb->fat_type = bpb.fat_type;
    p_bpb->log2_bytes_per_sector = bpb.log2_bytes_per_sector;
    p_bpb->log2_sectors_per_cluster = bpb.log2_sectors_per_cluster;
    p_bpb->num_active_FATs = bpb.num_active_FATs;
    p_bpb->num_root_dir_sectors = bpb.num_root_dir_sectors;
    p_bpb->active_FAT_sector = bpb.active_FAT_sector;
    p_bpb->first_root_dir_sector = bpb.first_root_dir_sector;
    p_bpb->first_data_sector = bpb.first_data_sector;
    p_bpb->num_clusters = bpb.num_clusters;
    return 0;
}

static void VFiPFDRV_notifyExecuteFormatAfterMount(PF_VOLUME* p_vol, const u8* format_param) {
    p_vol->flags |= 0x10u;
    p_vol->format_param = format_param;
}

static void VFiPFDRV_notifyNoExecuteLogicalFormat(PF_VOLUME* p_vol) {
    p_vol->flags |= 0x20u;
}

static s32 VFiPFDRV_LoadFSINFOInformation(unsigned char* p_buf, PF_VOLUME* p_vol) {
    PDM_FSINFO v4;

    if (VFipdm_bpb_get_fsinfo_information(p_buf, &v4))
        return 7;
    if (v4.free_count >= 2 && v4.free_count < p_vol->bpb.num_clusters + 2)
        p_vol->num_free_clusters = v4.free_count;
    if (v4.next_free >= 2 && v4.next_free < p_vol->bpb.num_clusters + 2)
        p_vol->last_free_cluster = v4.next_free;
    return 0;
}

s32 VFiPFDRV_GetFSINFOInformation(PF_VOLUME* p_vol) {
    int result;
    int v3;
    int driver_error_code;
    int v5;
    PF_CACHE_PAGE* p_page;
    u32 nSector;
    u32 is_valid;

    result = VFiPFCACHE_AllocateDataPage(p_vol, 0xFFFFFFFF, &p_page);
    if (result) {
        return result;
    }

    v3 = VFipdm_part_logical_read(p_vol->p_part, p_page->buffer, p_vol->bpb.fs_info_sector, 1, p_vol->bpb.bytes_per_sector, &nSector);
    if (v3) {
        if (v3 == 21) {
            driver_error_code = VFipdm_part_get_driver_error_code(p_vol->p_part);
            VFipf_vol_set.last_driver_error = driver_error_code;
            p_vol->last_driver_error = driver_error_code;
            v5 = 4096;
        } else {
            v5 = -1;
        }
        VFiPFCACHE_FreeDataPage(p_vol, p_page);
        VFipdm_part_release_permission(p_vol->p_part, 1);
        return v5;
    } else {
        if (VFipdm_bpb_check_fsinfo_sector(p_page->buffer, &is_valid)) {
            VFiPFCACHE_FreeDataPage(p_vol, p_page);
            VFipdm_part_release_permission(p_vol->p_part, 1);
            return -1;
        }

        if (!is_valid) {
            VFiPFCACHE_FreeDataPage(p_vol, p_page);
            VFipdm_part_release_permission(p_vol->p_part, 1);
            return 7;
        }
        if (VFiPFDRV_LoadFSINFOInformation(p_page->buffer, p_vol)) {
            VFiPFCACHE_FreeDataPage(p_vol, p_page);
            VFipdm_part_release_permission(p_vol->p_part, 1);
            return -1;
        }
        VFiPFCACHE_FreeDataPage(p_vol, p_page);
        return 0;
    }
}

s32 VFiPFDRV_StoreFreeCountToFSINFO(PF_VOLUME* p_vol) {
    int result;
    int v3;
    u32 num_success;
    struct PF_CACHE_PAGE* p_page;

    result = VFiPFCACHE_AllocateDataPage(p_vol, 0xFFFFFFFF, &p_page);
    if (!result) {
        v3 = VFiPFDRV_lread(p_vol, p_page->p_buf, p_vol->bpb.fs_info_sector, 1, &num_success);
        if (v3 || num_success != 1)
            v3 = 17;
        if (!v3) {
            p_page->p_buf[488] = (u8)(p_vol->num_free_clusters & 0xFF);
            p_page->p_buf[489] = (u8)((p_vol->num_free_clusters >> 8) & 0xFF);
            p_page->p_buf[490] = (u8)((p_vol->num_free_clusters >> 16) & 0xFF);
            p_page->p_buf[491] = (u8)((p_vol->num_free_clusters >> 24) & 0xFF);
            v3 = VFiPFDRV_lwrite(p_vol, p_page->p_buf, p_vol->bpb.fs_info_sector, 1u, &num_success);
            if (v3 || num_success != 1)
                v3 = 17;
        }
        VFiPFCACHE_FreeDataPage(p_vol, p_page);
        return v3;
    }
    return result;
}

u32 VFiPFDRV_IsInserted(PF_VOLUME* p_vol) {
    u32 is_insert;
    VFipdm_part_check_media_insert(p_vol->p_part, &is_insert);
    return is_insert;
}

u32 VFiPFDRV_IsDetected(PF_VOLUME* p_vol) {
    u32 is_detect;
    VFipdm_part_check_media_detect(p_vol->p_part, &is_detect);
    return is_detect;
}

unsigned int VFiPFDRV_IsWProtected(struct PF_VOLUME* p_vol) {
    u32 is_wprotect;
    VFipdm_part_check_media_write_protect(p_vol->p_part, &is_wprotect);
    return is_wprotect;
}

u32 VFiPFDRV_init(PF_VOLUME* p_vol) {
    u32 is_erase;

    if (!p_vol) {
        return 10;
    }

    if (VFipdm_part_check_data_erase(p_vol->p_part, &is_erase)) {
        return -1;
    }

    if (is_erase) {
        p_vol->flags |= 0x40;
    } else {
        p_vol->flags &= ~0x40u;
    }

    return 0;
}

s32 VFiPFDRV_finalize(PF_VOLUME* p_vol) {
    if (p_vol) {
        return 0;
    } else {
        return 10;
    }
}

s32 VFiPFDRV_mount(PF_VOLUME* p_vol) {
    int permission;
    int driver_error_code;
    int v5;
    int v6;
    int v7;
    int BPBInformation;
    u32 is_valid;
    u32 nSector;
    PF_CACHE_PAGE* p_page;
    PDM_DISK_INFO disk_inf;

    if (!p_vol)
        return 10;
    permission = VFipdm_part_get_permission(p_vol->p_part);
    if (permission) {
        if (permission == 21) {
            driver_error_code = VFipdm_part_get_driver_error_code(p_vol->p_part);
            VFipf_vol_set.last_driver_error = driver_error_code;
            p_vol->last_driver_error = driver_error_code;
            return 4096;
        } else {
            return -1;
        }
    } else if (VFipdm_part_get_media_information(p_vol->p_part, &disk_inf)) {
        VFipdm_part_release_permission(p_vol->p_part, 1);
        return -1;
    } else {
        if ((disk_inf.media_attr & 2) != 0)
            VFiPFDRV_notifyExecuteFormatAfterMount(p_vol, disk_inf.format_param);
        if ((disk_inf.media_attr & 8) != 0)
            VFiPFDRV_notifyNoExecuteLogicalFormat(p_vol);
        VFiPFCACHE_AllocateDataPage(p_vol, 0xFFFFFFFF, &p_page);
        v5 = VFipdm_part_logical_read(p_vol->p_part, p_page->buffer, 0, 1, 512, &nSector);
        if (v5) {
            if (v5 == 21) {
                v6 = VFipdm_part_get_driver_error_code(p_vol->p_part);
                VFipf_vol_set.last_driver_error = v6;
                p_vol->last_driver_error = v6;
                v7 = 4096;
            } else {
                v7 = -1;
            }
            VFiPFCACHE_FreeDataPage(p_vol, p_page);
            VFipdm_part_release_permission(p_vol->p_part, 1);
            return v7;
        } else if (VFipdm_bpb_check_boot_sector(p_page->buffer, &is_valid)) {
            VFiPFCACHE_FreeDataPage(p_vol, p_page);
            VFipdm_part_release_permission(p_vol->p_part, 1);
            return -1;
        } else if (is_valid) {
            BPBInformation = VFiPFDRV_GetBPBInformation(p_page->buffer, &p_vol->bpb);
            if (BPBInformation) {
                VFiPFCACHE_FreeDataPage(p_vol, p_page);
                VFipdm_part_release_permission(p_vol->p_part, 1);
                return BPBInformation;
            } else {
                VFiPFCACHE_FreeDataPage(p_vol, p_page);
                p_vol->num_free_clusters = -1;
                p_vol->last_free_cluster = -1;
                if (p_vol->bpb.fat_type == FAT_32)
                    return VFiPFDRV_GetFSINFOInformation(p_vol);
                return BPBInformation;
            }
        } else {
            VFiPFCACHE_FreeDataPage(p_vol, p_page);
            VFipdm_part_release_permission(p_vol->p_part, 1);
            return 7;
        }
    }
}

s32 VFiPFDRV_unmount(PF_VOLUME* p_vol, unsigned int mode) {
    int v4;
    int driver_error_code;

    if (!p_vol)
        return 10;
    v4 = VFipdm_part_release_permission(p_vol->p_part, mode == 1);

    if (v4) {
        if (v4 == 21) {
            driver_error_code = VFipdm_part_get_driver_error_code(p_vol->p_part);
            VFipf_vol_set.last_driver_error = driver_error_code;
            p_vol->last_driver_error = driver_error_code;
            return 4096;
        }

        return -1;
    }

    return 0;
}

s32 VFiPFDRV_format(PF_VOLUME* p_vol, const u8* param) {
    int permission;
    int v5;
    int v6;
    int driver_error_code;

    if (!p_vol)
        return 10;
    if ((p_vol->flags & 2) == 0) {
        permission = VFipdm_part_get_permission(p_vol->p_part);
        if (permission) {
            if (permission != 21)
                return -1;
            goto LABEL_14;
        }
    }
    v5 = VFipdm_part_format(p_vol->p_part, param);
    if (v5) {
        if (v5 != 21)
            return -1;
        goto LABEL_14;
    }
    if ((p_vol->flags & 2) != 0)
        return 0;
    v6 = VFipdm_part_release_permission(p_vol->p_part, 1);
    if (!v6)
        return 0;
    if (v6 == 21) {
    LABEL_14:
        driver_error_code = VFipdm_part_get_driver_error_code(p_vol->p_part);
        VFipf_vol_set.last_driver_error = driver_error_code;
        p_vol->last_driver_error = driver_error_code;
        return 4096;
    }
    return -1;
}

s32 VFiPFDRV_lread(PF_VOLUME* p_vol, u8* buf, u32 sector, u32 num_sectors, u32* p_num_success) {
    int v7;
    int driver_error_code;

    if (!p_vol)
        return 10;
    v7 = VFipdm_part_logical_read(p_vol->p_part, buf, sector, num_sectors, p_vol->bpb.bytes_per_sector, p_num_success);

    if (v7) {
        if (v7 == 21) {
            driver_error_code = VFipdm_part_get_driver_error_code(p_vol->p_part);
            VFipf_vol_set.last_driver_error = driver_error_code;
            p_vol->last_driver_error = driver_error_code;
            return 4096;
        }

        return -1;
    }

    return 0;
}

s32 VFiPFDRV_lwrite(PF_VOLUME* p_vol, const u8* buf, u32 sector, u32 num_sectors, u32* p_num_success) {
    int v7;
    int driver_error_code;

    if (!p_vol)
        return 10;
    v7 = VFipdm_part_logical_write(p_vol->p_part, buf, sector, num_sectors, p_vol->bpb.bytes_per_sector, p_num_success);

    if (v7) {
        if (v7 == 21) {
            driver_error_code = VFipdm_part_get_driver_error_code(p_vol->p_part);
            VFipf_vol_set.last_driver_error = driver_error_code;
            p_vol->last_driver_error = driver_error_code;
            return 4096;
        }

        return -1;
    }

    return 0;
}

s32 VFiPFDRV_lerase(PF_VOLUME* p_vol, u32 sector, u32 num_sectors) {
    int v5;
    int driver_error_code;

    if (!p_vol)
        return 10;
    v5 = VFipdm_part_logical_erase(p_vol->p_part, sector, num_sectors, p_vol->bpb.bytes_per_sector);

    if (v5) {
        if (v5 == 21) {
            driver_error_code = VFipdm_part_get_driver_error_code(p_vol->p_part);
            VFipf_vol_set.last_driver_error = driver_error_code;
            p_vol->last_driver_error = driver_error_code;
            return 4096;
        }

        return -1;
    }

    return 0;
}
