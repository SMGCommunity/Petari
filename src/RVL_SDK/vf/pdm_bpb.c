#include "revolution/vf/vf_struct.h"

void VFipdm_bpb_calculate_common_bpb_fields(struct PDM_BPB* p_bpb) {
    u32 num_data_sectors;
    u32 val;   // Should be u16
    u32 temp;  // Present in DWARF but unused here.

    p_bpb->log2_bytes_per_sector = 0;
    temp = p_bpb->bytes_per_sector;
    while ((temp = (temp >> 1) & 0x7FFF) != 0) {
        p_bpb->log2_bytes_per_sector++;
    }

    p_bpb->log2_sectors_per_cluster = 0;
    temp = p_bpb->sectors_per_cluster;
    while ((temp = (temp >> 1) & 0x7FFF) != 0) {
        p_bpb->log2_sectors_per_cluster++;
    }

    val = ((p_bpb->bytes_per_sector + (p_bpb->num_root_dir_entries * 32)) - 1) >> p_bpb->log2_bytes_per_sector;
    p_bpb->num_root_dir_sectors = val;

    val = ((u16)val + p_bpb->num_reserved_sectors) + (p_bpb->num_FATs * p_bpb->sectors_per_FAT);
    p_bpb->first_data_sector = val;

    num_data_sectors = p_bpb->total_sectors - val;
    p_bpb->num_clusters = num_data_sectors >> p_bpb->log2_sectors_per_cluster;

    if ((num_data_sectors >> p_bpb->log2_sectors_per_cluster) < 0xFF5) {
        p_bpb->fat_type = PDM_FAT_12;
    } else if ((num_data_sectors >> p_bpb->log2_sectors_per_cluster) < 0xFFF5) {
        p_bpb->fat_type = PDM_FAT_16;
    } else {
        p_bpb->fat_type = PDM_FAT_32;
    }
}

static void VFipdm_bpb_calculate_specific_bpb_fields(PDM_BPB* p_bpb) {
    switch ((s32)p_bpb->fat_type) {
    case PDM_FAT_12:
    case PDM_FAT_16: {
        p_bpb->active_FAT_sector = p_bpb->num_reserved_sectors;
        p_bpb->num_active_FATs = p_bpb->num_FATs;
        p_bpb->first_root_dir_sector = p_bpb->num_reserved_sectors + (p_bpb->num_FATs * p_bpb->sectors_per_FAT);
    } break;
    case PDM_FAT_32: {
        p_bpb->first_root_dir_sector = p_bpb->first_data_sector + ((p_bpb->root_dir_cluster - 2) << p_bpb->log2_sectors_per_cluster);
        if (p_bpb->ext_flags & 0x80) {
            p_bpb->num_active_FATs = 1;
            p_bpb->active_FAT_sector = p_bpb->num_reserved_sectors + ((p_bpb->ext_flags & 0x7) * p_bpb->sectors_per_FAT);
        } else {
            p_bpb->num_active_FATs = p_bpb->num_FATs;
            p_bpb->active_FAT_sector = p_bpb->num_reserved_sectors;
        }
    }
    }
}

static s32 VFipdm_bpb_load_string(const u8* buf, u32 length, u8* p_string) {
    u32 i;
    if (buf == NULL || p_string == NULL || length == 0) {
        return 1;
    }
    for (i = 0; i < length; ++i) {
        p_string[i] = buf[i];
    }
    return 0;
}

s32 VFipdm_bpb_get_bpb_information(u8* buf, struct PDM_BPB* p_bpb) {
    s32 result;
    int i;

    if (buf == 0 || p_bpb == 0) {
        return 1;
    }

    result = 0;

    VFipdm_bpb_load_string(buf, 3, p_bpb->jump_boot);
    VFipdm_bpb_load_string(buf + 3, 8, p_bpb->oem_name);

    p_bpb->bytes_per_sector = buf[0xB] | (buf[0xC] << 8);
    p_bpb->sectors_per_cluster = buf[0xD];
    p_bpb->num_reserved_sectors = buf[0xE] | (buf[0xF] << 8);
    p_bpb->num_FATs = buf[0x10];
    p_bpb->num_root_dir_entries = buf[0x11] | (buf[0x12] << 8);

    p_bpb->total_sectors16 = buf[0x13] | (buf[0x14] << 8);
    p_bpb->media = buf[0x15];
    p_bpb->sectors_per_FAT16 = buf[0x16] | (buf[0x17] << 8);
    p_bpb->sector_per_track = buf[0x18] | (buf[0x19] << 8);
    p_bpb->num_heads = buf[0x1A] | (buf[0x1B] << 8);
    p_bpb->num_hidden_sectors = buf[0x1C] | (buf[0x1D] << 8) | (buf[0x1E] << 16) | (buf[0x1F] << 24);
    p_bpb->total_sectors32 = buf[0x20] | (buf[0x21] << 8) | (buf[0x22] << 16) | (buf[0x23] << 24);

    p_bpb->total_sectors = (p_bpb->total_sectors16 == 0) ? p_bpb->total_sectors32 : (u32)p_bpb->total_sectors16;

    if (p_bpb->sectors_per_FAT16 == 0) {
        p_bpb->sectors_per_FAT = (p_bpb->sectors_per_FAT32 = buf[0x24] | (buf[0x25] << 8) | (buf[0x26] << 16) | (buf[0x27] << 24));
    } else {
        p_bpb->sectors_per_FAT = p_bpb->sectors_per_FAT16;
        p_bpb->sectors_per_FAT32 = 0;
    }

    VFipdm_bpb_calculate_common_bpb_fields(p_bpb);

    switch ((s32)p_bpb->fat_type) {
    case PDM_FAT_12:
    case PDM_FAT_16: {
        if (p_bpb->sectors_per_FAT16 == 0) {
            result = 4;
        }

        p_bpb->ext_flags = 0;
        p_bpb->fs_version = 0;
        p_bpb->root_dir_cluster = 0;
        p_bpb->fs_info_sector = 0;
        p_bpb->backup_boot_sector = 0;

        p_bpb->drive = buf[0x24];
        p_bpb->boot_sig = buf[0x26];
        p_bpb->vol_id = buf[0x27] | (buf[0x28] << 8) | (buf[0x29] << 16) | (buf[0x2A] << 24);

        VFipdm_bpb_load_string(buf + 0x2b, 11, p_bpb->vol_label);
        VFipdm_bpb_load_string(buf + 0x36, 8, p_bpb->fs_type);
    } break;
    case PDM_FAT_32: {
        if (p_bpb->total_sectors16 != 0 || p_bpb->sectors_per_FAT16 != 0) {
            result = 4;
        }

        p_bpb->ext_flags = buf[0x28] | (buf[0x29] << 8);
        p_bpb->fs_version = buf[0x2A] | (buf[0x2B] << 8);
        p_bpb->root_dir_cluster = buf[0x2C] | (buf[0x2D] << 8) | (buf[0x2E] << 16) | (buf[0x2F] << 24);
        p_bpb->fs_info_sector = (buf[0x31] << 8) | buf[0x30];
        p_bpb->backup_boot_sector = (buf[0x33] << 8) | buf[0x32];

        p_bpb->drive = buf[0x40];
        p_bpb->boot_sig = buf[0x42];
        p_bpb->vol_id = buf[0x43] | (buf[0x44] << 8) | (buf[0x45] << 16) | (buf[0x46] << 24);

        VFipdm_bpb_load_string(buf + 0x47, 11, p_bpb->vol_label);
        VFipdm_bpb_load_string(buf + 0x52, 8, p_bpb->fs_type);

        if (p_bpb->fs_version != 0) {
            result = 4;
        }
    } break;
    default:
        result = 4;
    }

    VFipdm_bpb_calculate_specific_bpb_fields(p_bpb);

    return result;
}

s32 VFipdm_bpb_get_fsinfo_information(u8* buf, struct PDM_FSINFO* p_fsinfo) {
    if ((buf == NULL) || (p_fsinfo == NULL)) {
        return 1;
    }

    p_fsinfo->free_count = buf[0x1E8] | (buf[0x1E9] << 8) | (buf[0x1EA] << 16) | (buf[0x1EB] << 24);
    p_fsinfo->next_free = buf[0x1EC] | (buf[0x1ED] << 8) | (buf[0x1EE] << 16) | (buf[0x1EF] << 24);

    return 0;
}

s32 VFipdm_bpb_set_fsinfo_information(struct PDM_FSINFO* p_fsinfo, u8* buf) {
    if (buf == NULL || p_fsinfo == NULL) {
        return 1;
    }

    buf[0] = 0x52;
    buf[1] = 0x52;
    buf[2] = 0x61;
    buf[3] = 0x41;
    buf[0x1E4] = 0x72;
    buf[0x1E5] = 0x72;
    buf[0x1E6] = 0x41;
    buf[0x1E7] = 0x61;

    buf[0x1E8] = p_fsinfo->free_count;
    buf[0x1E9] = p_fsinfo->free_count >> 8;
    buf[0x1EA] = p_fsinfo->free_count >> 16;
    buf[0x1EB] = p_fsinfo->free_count >> 24;

    buf[0x1EC] = p_fsinfo->next_free;
    buf[0x1ED] = p_fsinfo->next_free >> 8;
    buf[0x1EE] = p_fsinfo->next_free >> 16;
    buf[0x1EF] = p_fsinfo->next_free >> 24;

    buf[0x1FC] = 0x00;
    buf[0x1FD] = 0x00;
    buf[0x1FE] = 0x55;
    buf[0x1FF] = 0xAA;

    return 0;
}

s32 VFipdm_bpb_check_boot_sector(u8* buf, u32* is_boot) {
    u16 byte_per_sector;
    u16 sector_per_cluster;
    u8 media;

    if (buf == NULL || is_boot == NULL) {
        return 1;
    }

    *is_boot = 1;

    if (buf[0] != 0xEB || buf[2] != 0x90) {
        if (buf[0] != 0xE9) {
            *is_boot = 0;
        }
    }

    if (buf[0x1FE] != 0x55 || buf[0x1FF] != 0xAA) {
        *is_boot = 0;
    }

    byte_per_sector = (buf[0xC] << 8) | buf[0xB];
    if (byte_per_sector != 512 && byte_per_sector != 1024 && byte_per_sector != 2048 && byte_per_sector != 4096) {
        *is_boot = 0;
    }

    sector_per_cluster = buf[0xD];
    if (sector_per_cluster != 1 && sector_per_cluster != 2 && sector_per_cluster != 4 && sector_per_cluster != 8 && sector_per_cluster != 16 &&
        sector_per_cluster != 32 && sector_per_cluster != 64 && sector_per_cluster != 128) {
        *is_boot = 0;
    }

    media = buf[0x15];
    if (media != 0xF0 && media != 0xF8 && media != 0xF9 && media != 0xFA && media != 0xFB && media != 0xFC && media != 0xFD && media != 0xFE &&
        media != 0xFF) {
        *is_boot = 0;
    }

    return 0;
}

s32 VFipdm_bpb_check_fsinfo_sector(u8* buf, u32* is_fsinfo) {
    u32 lead_sig;
    u32 struct_sig;
    u32 trail_sig;

    if (buf == NULL || is_fsinfo == NULL) {
        return 1;
    }

    lead_sig = buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
    struct_sig = buf[0x1E4] | (buf[0x1E5] << 8) | (buf[0x1E6] << 16) | (buf[0x1E7] << 24);
    trail_sig = buf[0x1FC] | (buf[0x1FD] << 8) | (buf[0x1FE] << 16) | (buf[0x1FF] << 24);

    if (lead_sig == 0x41615252 && struct_sig == 0x61417272 && trail_sig == 0xAA550000) {
        *is_fsinfo = 1;
    } else {
        *is_fsinfo = 0;
    }

    return 0;
}
