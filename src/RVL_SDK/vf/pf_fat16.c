#include "revolution/vf/pf_fat16.h"
#include "revolution/vf/pf_cache.h"
#include "revolution/vf/pf_fat.h"
#include "revolution/vf/pf_sector.h"

s32 VFiPFFAT16_ReadFATEntry(PF_VOLUME* p_vol, u32 cluster, u32* p_value) {
    int fat_sector;
    u16 offset_in_sector;
    u32 v8;
    int current_fat;
    int v10;
    u8 buf[2];

    if (!p_vol) {
        *p_value = -1;
        return 10;
    }
    if ((cluster < 2 || cluster >= p_vol->bpb.num_clusters + 2) && cluster >= 2) {
        *p_value = -1;
        return 14;
    }

    fat_sector = p_vol->bpb.active_FAT_sector + ((2 * cluster) >> p_vol->bpb.log2_bytes_per_sector);
    offset_in_sector = (2 * cluster) & (p_vol->bpb.bytes_per_sector - 1);

    if ((p_vol->bpb.ext_flags & 0x80) != 0)
        v8 = p_vol->bpb.ext_flags & 7;
    else
        v8 = 1;

    for (;;) {
        current_fat = VFiPFSEC_ReadFAT(p_vol, buf, fat_sector, offset_in_sector, 2);

        if (current_fat == 4096 && p_vol->p_callback) {
            v10 = p_vol->p_callback(p_vol->last_driver_error);
            if (!v10)
                continue;
            if (v10 == 1 && p_vol->bpb.num_active_FATs >= 2u && v8 < p_vol->bpb.num_active_FATs) {
                ++v8;
                fat_sector += p_vol->bpb.sectors_per_FAT;
                continue;
            }
        }

        if (current_fat) {
            *p_value = -1;
            return current_fat;
        }

        break;
    }

    *p_value = buf[0] | (buf[1] << 8);
    return 0;
}

s32 VFiPFFAT16_ReadFATEntryPage(PF_VOLUME* p_vol, u32 cluster, u32* p_value, PF_CACHE_PAGE** pp_page) {
    int result;
    u32 sector;
    u32 current_fat;
    int err;
    int v12;
    u32 offset;

    if (!p_vol)
        return 10;
    if ((cluster < 2 || cluster >= p_vol->bpb.num_clusters + 2) && cluster >= 2)
        return 14;

    sector = p_vol->bpb.active_FAT_sector + ((2 * cluster) >> p_vol->bpb.log2_bytes_per_sector);

    if ((p_vol->bpb.ext_flags & 0x80) != 0)
        current_fat = p_vol->bpb.ext_flags & 7;
    else
        current_fat = 1;

    for (;;) {
        if ((*pp_page)->sector <= sector && (*pp_page)->sector + p_vol->cache.fat_buff_size > sector) {
            if (sector != (*pp_page)->sector + (((*pp_page)->p_buf - (*pp_page)->buffer) >> p_vol->bpb.log2_bytes_per_sector)) {
                (*pp_page)->p_buf = &(*pp_page)->buffer[(sector - (*pp_page)->sector) << p_vol->bpb.log2_bytes_per_sector];
            }

            offset = (2 * cluster) & (p_vol->bpb.bytes_per_sector - 1);
            *p_value = (*pp_page)->p_buf[offset] | (((u16)(*pp_page)->p_buf[offset + 1]) << 8);
            return 0;
        }

        if ((*pp_page)->option == 1) {
            result = VFiPFFAT_UpdateFATEntry(p_vol, *pp_page);
            if (result)
                return result;
        }

        err = VFiPFCACHE_ReadFATPage(p_vol, sector, pp_page);

        if (err == 4096 && p_vol->p_callback) {
            v12 = p_vol->p_callback(p_vol->last_driver_error);
            if (!v12)
                continue;
            if (v12 == 1 && p_vol->bpb.num_active_FATs >= 2u && current_fat < p_vol->bpb.num_active_FATs) {
                ++current_fat;
                sector += p_vol->bpb.sectors_per_FAT;
                continue;
            }
        }

        if (err)
            return err;
    }
}

s32 VFiPFFAT16_WriteFATEntry(PF_VOLUME* p_vol, u32 cluster, u32 value) {
    u16 fat_sector;
    u16 offset_in_sector;
    u8 buf[2];

    if (!p_vol)
        return 10;
    if ((cluster < 2 || cluster >= p_vol->bpb.num_clusters + 2) && cluster >= 2)
        return 14;
    fat_sector = p_vol->bpb.active_FAT_sector + ((2 * cluster) >> p_vol->bpb.log2_bytes_per_sector);
    offset_in_sector = (2 * cluster) & (p_vol->bpb.bytes_per_sector - 1);
    *buf = (value >> 8) | (value << 8);
    return VFiPFSEC_WriteFAT(p_vol, buf, fat_sector, offset_in_sector, 2);
}

s32 VFiPFFAT16_WriteFATEntryPage(PF_VOLUME* p_vol, u32 cluster, u32 value, PF_CACHE_PAGE** pp_page) {
    s16 v5;
    int err;
    u32 fat_sector;
    u32 offset_in_sector;
    u32 current_fat;
    int FATPage;
    int v12;

    v5 = value;

    if (!p_vol)
        return 10;
    if ((cluster < 2 || cluster >= p_vol->bpb.num_clusters + 2) && cluster >= 2)
        return 14;

    fat_sector = p_vol->bpb.active_FAT_sector + ((2 * cluster) >> p_vol->bpb.log2_bytes_per_sector);
    offset_in_sector = (2 * cluster) & (p_vol->bpb.bytes_per_sector - 1);

    if ((*pp_page)->sector > fat_sector || (*pp_page)->sector + p_vol->cache.fat_buff_size <= fat_sector) {
        err = VFiPFFAT_UpdateFATEntry(p_vol, *pp_page);
        if (err)
            return err;

        if ((p_vol->bpb.ext_flags & 0x80) != 0)
            current_fat = p_vol->bpb.ext_flags & 7;
        else
            current_fat = 1;

        for (;;) {
            FATPage = VFiPFCACHE_ReadFATPage(p_vol, fat_sector, pp_page);

            if (FATPage == 4096 && p_vol->p_callback) {
                v12 = p_vol->p_callback(p_vol->last_driver_error);
                if (!v12)
                    continue;
                if (v12 == 1 && p_vol->bpb.num_active_FATs >= 2u && current_fat < p_vol->bpb.num_active_FATs) {
                    ++current_fat;
                    fat_sector += p_vol->bpb.sectors_per_FAT;
                    continue;
                }
            }

            if (FATPage)
                return FATPage;

            break;
        }
    }

    if (fat_sector != (*pp_page)->sector + (((*pp_page)->p_buf - (*pp_page)->buffer) >> p_vol->bpb.log2_bytes_per_sector)) {
        (*pp_page)->p_buf = &(*pp_page)->buffer[(fat_sector - (*pp_page)->sector) << p_vol->bpb.log2_bytes_per_sector];
    }

    (*pp_page)->p_buf[offset_in_sector] = (u8)v5;
    (*pp_page)->p_buf[offset_in_sector + 1] = (u8)(v5 >> 8);

    VFiPFCACHE_UpdateModifiedSector(p_vol, *pp_page, 1);
    return 0;
}
