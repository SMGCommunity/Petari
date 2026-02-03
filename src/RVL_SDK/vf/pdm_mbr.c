#include "revolution/vf/pdm_mbr.h"
#include "revolution/vf/pdm_bpb.h"
#include "revolution/vf/pdm_disk.h"
#include "revolution/vf/pdm_dskmng.h"

s32 VFipdm_mbr_get_table(u8* buf, u32 sector, PDM_MBR* p_mbr_tbl) {
    u8* p_buf;
    u32 i;
    u8 cs_val;

    if (!buf || !p_mbr_tbl)
        return 1;
    if (sector) {
        if (!p_mbr_tbl->epbr_base_sector)
            p_mbr_tbl->epbr_base_sector = sector;
        p_mbr_tbl->current_sector = sector;
    } else {
        p_mbr_tbl->epbr_base_sector = 0;
        p_mbr_tbl->current_sector = 0;
    }
    p_buf = buf + 446;
    for (i = 0; i < 4; ++i) {
        p_mbr_tbl->partition_table[i].boot_flag = *p_buf;
        p_mbr_tbl->partition_table[i].partition_type = p_buf[4];
        p_mbr_tbl->partition_table[i].s_head = p_buf[1];
        cs_val = p_buf[2];
        p_mbr_tbl->partition_table[i].s_cylinder = 4 * (cs_val & 0xC0) + ((cs_val | (p_buf[3] << 8)) >> 8);
        p_mbr_tbl->partition_table[i].s_sector = cs_val & 0x3F;
        p_mbr_tbl->partition_table[i].e_head = p_buf[5];
        cs_val = p_buf[6];
        p_mbr_tbl->partition_table[i].e_cylinder = 4 * (cs_val & 0xC0) + ((cs_val | (p_buf[7] << 8)) >> 8);
        p_mbr_tbl->partition_table[i].e_sector = cs_val & 0x3F;
        p_mbr_tbl->partition_table[i].lba_start_sector = (p_buf[11] << 24) | (p_buf[10] << 16) | p_buf[8] | (p_buf[9] << 8);
        p_mbr_tbl->partition_table[i].lba_num_sectors = (p_buf[15] << 24) | (p_buf[14] << 16) | p_buf[12] | (p_buf[13] << 8);
        p_buf += 16;
    }
    return 0;
}

s32 VFipdm_mbr_get_mbr_part_table(PDM_DISK* p_disk, PDM_MBR* p_mbr_tbl) {
    s32 err;
    u32 num_success;
    u32 is_master_boot;
    u8 buf[512];

    if (!p_disk || !p_mbr_tbl)
        return 1;
    err = VFipdm_disk_check_disk_handle(p_disk);
    if (err) {
        return err;
    }

    err = VFipdm_disk_physical_read(p_disk, buf, 0, 1, &num_success);
    if (err) {
        return err;
    }

    VFipdm_mbr_check_master_boot_record(p_disk, buf, &is_master_boot);
    if (is_master_boot) {
        VFipdm_mbr_get_table(buf, 0, p_mbr_tbl);
    } else {
        return 5;
    }

    return 0;
}

s32 VFipdm_mbr_get_epbr_part_table(PDM_DISK* p_disk, PDM_MBR* p_mbr_tbl) {
    int err;
    u32 extend_start_sector;
    u16 tbl_index;
    u16 lba_size;
    u32 num_success;
    u32 is_master_boot;
    u8 buf[512];

    if (!p_disk || !p_mbr_tbl)
        return 1;
    err = VFipdm_disk_check_disk_handle(p_disk);
    if (err) {
        return err;
    }

    extend_start_sector = 0;
    for (tbl_index = 0; tbl_index < 4u; ++tbl_index) {
        if (p_mbr_tbl->partition_table[tbl_index].partition_type == 5 || p_mbr_tbl->partition_table[tbl_index].partition_type == 15) {
            extend_start_sector = p_mbr_tbl->epbr_base_sector + p_mbr_tbl->partition_table[tbl_index].lba_start_sector;
            break;
        }
    }
    if (tbl_index == 4) {
        return 7;
    } else {
        err = VFipdm_disk_get_lba_size(p_disk, &lba_size);
        if (err) {
            return err;
        }

        err = VFipdm_disk_physical_read(p_disk, buf, extend_start_sector * (lba_size >> 9), 1, &num_success);
        if (err) {
            return err;
        }

        VFipdm_mbr_check_master_boot_record(p_disk, buf, &is_master_boot);
        if (is_master_boot) {
            VFipdm_mbr_get_table(buf, extend_start_sector, p_mbr_tbl);
        } else {
            return 6;
        }

        return 0;
    }
}

s32 VFipdm_mbr_check_master_boot_record(PDM_DISK* p_disk, u8* buf, u32* is_master_boot) {
    u8 v3;  // I don't think this type is correct
    int err;
    s16 part_cnt;
    u32 is_boot;
    PDM_MBR mbr_tbl;

    v3 = p_disk->status;
    if (!p_disk || !buf || !is_master_boot)
        return 1;
    err = VFipdm_disk_check_disk_handle(p_disk);
    if (!err) {
        *is_master_boot = 0;
        if (buf[510] == 85 && buf[511] == 170) {
            VFipdm_mbr_get_table(buf, 0, &mbr_tbl);
            for (part_cnt = 0; part_cnt < 4; ++part_cnt) {
                if (mbr_tbl.partition_table[part_cnt].lba_num_sectors > -1 - mbr_tbl.partition_table[part_cnt].lba_start_sector ||
                    mbr_tbl.partition_table[part_cnt].lba_start_sector + mbr_tbl.partition_table[part_cnt].lba_num_sectors >
                        VFipdm_disk_set.disk[v3].disk_info.total_sectors) {
                    *is_master_boot = 0;
                    break;
                }
                if (mbr_tbl.partition_table[part_cnt].lba_start_sector)
                    *is_master_boot = 1;
            }
            if (!*is_master_boot && !mbr_tbl.partition_table[0].lba_start_sector && !mbr_tbl.partition_table[1].lba_start_sector &&
                !mbr_tbl.partition_table[2].lba_start_sector && !mbr_tbl.partition_table[3].lba_start_sector) {
                VFipdm_bpb_check_boot_sector(buf, &is_boot);
                if (is_boot)
                    *is_master_boot = 0;
                else
                    *is_master_boot = 1;
            }
            return 0;
        } else {
            return 0;
        }
    }
    return err;
}
