#include "revolution/vf/pf_fat16.h"
#include "revolution/usb.h"
#include "revolution/vf/pf_cache.h"
#include "revolution/vf/pf_fat.h"
#include "revolution/vf/pf_sector.h"

s32 VFiPFFAT16_ReadFATEntry(PF_VOLUME* p_vol, u32 cluster, u32* p_value) {
    u32 fat_offset;
    u32 fat_sector;
    u16 offset_in_sector;
    u8 buf[2];
    s32 err;
    u32 current_fat;
    s32 result;

    if (!p_vol) {
        *p_value = -1;
        return 10;
    }

    if ((cluster < 2 || cluster >= p_vol->bpb.num_clusters + 2) && cluster != 0 && cluster != 1) {
        *p_value = -1;
        return 14;
    }

    fat_offset = cluster << 1;
    current_fat = 1;

    fat_sector = (u16)(p_vol->bpb.active_FAT_sector + (fat_offset >> p_vol->bpb.log2_bytes_per_sector));
    offset_in_sector = fat_offset & (p_vol->bpb.bytes_per_sector - 1);

    if ((p_vol->bpb.ext_flags & 0x80) == 0) {
        current_fat = 1;
    } else {
        current_fat = p_vol->bpb.ext_flags & 7;
    }

    do {
        err = VFiPFSEC_ReadFAT(p_vol, buf, fat_sector, offset_in_sector, 2);

        if (err != 0x1000 || p_vol->p_callback == 0) {
            goto check;
        }

        result = p_vol->p_callback(p_vol->last_driver_error);
        if (result != 0) {
            if (result == 1 && p_vol->bpb.num_active_FATs >= 2 && current_fat < p_vol->bpb.num_active_FATs) {
                current_fat++;
                fat_sector += p_vol->bpb.sectors_per_FAT;
            } else {
            check:;
                if (err != 0) {
                    *p_value = -1;
                    return err;
                }
            }
        }
    } while (err != 0);

    err = 0;
    {
        u16 v = *(u16*)buf;
        *p_value = (u16)((v & 0xff00) >> 8 | (v & 0x00ff) << 8);
    }

    return err;
}

s32 VFiPFFAT16_ReadFATEntryPage(PF_VOLUME* p_vol, u32 cluster, u32* p_value, PF_CACHE_PAGE** pp_page) {
    s32 err;
    u32 offset;
    u32 sector;
    u32 current_fat;
    s32 result;

    err = 0;
    if (!p_vol) {
        return 10;
    }

    if ((cluster < 2 || cluster >= p_vol->bpb.num_clusters + 2) && cluster != 0 && cluster != 1) {
        return 14;
    }

    offset = cluster << 1;
    sector = (u16)(p_vol->bpb.active_FAT_sector + (offset >> p_vol->bpb.log2_bytes_per_sector));

    if ((p_vol->bpb.ext_flags & 0x80) == 0) {
        current_fat = 1;
    } else {
        current_fat = (p_vol->bpb.ext_flags) & 7;
    }

    if ((*pp_page)->sector > sector || (*pp_page)->sector + p_vol->cache.fat_buff_size <= sector) {
        if ((*pp_page)->option == 1) {
            err = VFiPFFAT_UpdateFATEntry(p_vol, *pp_page);
            if (err != 0) {
                return err;
            }
        }

        do {
            err = VFiPFCACHE_ReadFATPage(p_vol, sector, pp_page);
            if (err != 0x1000 || p_vol->p_callback == 0) {
                goto check;
            }

            result = p_vol->p_callback(p_vol->last_driver_error);
            if (result != 0) {
                if (result == 1 && p_vol->bpb.num_active_FATs >= 2 && current_fat < p_vol->bpb.num_active_FATs) {
                    current_fat++;
                    sector += p_vol->bpb.sectors_per_FAT;
                } else {
                check:;
                    if (err != 0) {
                        return err;
                    }
                }
            }
        } while (err != 0);

    } else {
        if (sector != (*pp_page)->sector + (((*pp_page)->p_buf - (*pp_page)->buffer) >> p_vol->bpb.log2_bytes_per_sector)) {
            (*pp_page)->p_buf = (*pp_page)->buffer + ((sector - (*pp_page)->sector) << p_vol->bpb.log2_bytes_per_sector);
        }
    }

    offset &= (p_vol->bpb.bytes_per_sector - 1);
    {
        u16 v = *(u16*)((*pp_page)->p_buf + offset);
        *p_value = (u16)((v & 0xff00) >> 8 | (v & 0x00ff) << 8);
    }

    return err;
}

s32 VFiPFFAT16_WriteFATEntry(PF_VOLUME* p_vol, u32 cluster, u32 value) {
    u16 fat_sector;
    u16 offset_in_sector;
    u8 buf[2];

    if (!p_vol)
        return 10;
    if ((cluster < 2 || cluster >= p_vol->bpb.num_clusters + 2) && cluster != 0 && cluster != 1)
        return 14;
    fat_sector = p_vol->bpb.active_FAT_sector + ((2 * cluster) >> p_vol->bpb.log2_bytes_per_sector);
    offset_in_sector = (2 * cluster) & (p_vol->bpb.bytes_per_sector - 1);
    *(u16*)buf = SWAP16((u16)value);
    return VFiPFSEC_WriteFAT(p_vol, buf, fat_sector, offset_in_sector, 2);
}

s32 VFiPFFAT16_WriteFATEntryPage(PF_VOLUME* p_vol, u32 cluster, u32 value, PF_CACHE_PAGE** pp_page) {
    s32 err;
    u32 fat_offset;
    u32 fat_sector;
    u32 current_fat;
    s32 result;

    err = 0;
    if (!p_vol) {
        return 10;
    }

    if ((cluster < 2 || cluster >= p_vol->bpb.num_clusters + 2) && cluster != 0 && cluster != 1) {
        return 14;
    }

    fat_offset = cluster << 1;
    fat_sector = (u16)(p_vol->bpb.active_FAT_sector + (fat_offset >> p_vol->bpb.log2_bytes_per_sector));
    fat_offset = (u16)(fat_offset & (p_vol->bpb.bytes_per_sector - 1));

    if ((*pp_page)->sector > fat_sector || (*pp_page)->sector + p_vol->cache.fat_buff_size <= fat_sector) {
        err = VFiPFFAT_UpdateFATEntry(p_vol, *pp_page);
        if (err != 0) {
            return err;
        }

        if ((p_vol->bpb.ext_flags & 0x80) == 0) {
            current_fat = 1;
        } else {
            current_fat = (p_vol->bpb.ext_flags) & 7;
        }

        do {
            err = VFiPFCACHE_ReadFATPage(p_vol, fat_sector, pp_page);
            if (err != 0x1000 || p_vol->p_callback == 0) {
                goto check;
            }

            result = p_vol->p_callback(p_vol->last_driver_error);
            if (result != 0) {
                if (result == 1 && p_vol->bpb.num_active_FATs >= 2 && current_fat < p_vol->bpb.num_active_FATs) {
                    current_fat++;
                    fat_sector += p_vol->bpb.sectors_per_FAT;
                } else {
                check:;
                    if (err != 0) {
                        return err;
                    }
                }
            }
        } while (err != 0);
    } else {
        if (fat_sector != (*pp_page)->sector + (((*pp_page)->p_buf - (*pp_page)->buffer) >> p_vol->bpb.log2_bytes_per_sector)) {
            (*pp_page)->p_buf = (*pp_page)->buffer + ((fat_sector - (*pp_page)->sector) << p_vol->bpb.log2_bytes_per_sector);
        }
    }

    (*pp_page)->p_buf[fat_offset] = (u8)value;
    (*pp_page)->p_buf[fat_offset + 1] = (u8)(value >> 8);

    VFiPFCACHE_UpdateModifiedSector(p_vol, *pp_page, 1);
    return err;
}
