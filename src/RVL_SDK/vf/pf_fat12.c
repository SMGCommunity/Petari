#include "revolution/vf/pf_fat12.h"
#include "revolution/vf/pf_cache.h"
#include "revolution/vf/pf_fat.h"
#include "revolution/vf/pf_sector.h"

s32 VFiPFFAT12_ReadFATEntry(PF_VOLUME* p_vol, u16 cluster, u32* p_value) {
    s32 err;
    u16 fat_offset;
    u16 fat_sector;
    u16 offset_in_sector;
    u16 raw_entry;
    u8 buf[2];
    u32 current_fat;
    s32 result;

    if (!p_vol) {
        *p_value = -1;
        return 10;
    }

    if (!((cluster >= 2) && (cluster < p_vol->bpb.num_clusters + 2)) && cluster != 0 && cluster != 1) {
        *p_value = -1;
        return 14;
    }

    fat_offset = cluster + (cluster >> 1);
    fat_sector = p_vol->bpb.active_FAT_sector + (fat_offset >> p_vol->bpb.log2_bytes_per_sector);
    offset_in_sector = fat_offset & (p_vol->bpb.bytes_per_sector - 1);

    if ((p_vol->bpb.ext_flags & 0x80) != 0) {
        current_fat = p_vol->bpb.ext_flags & 7;
    } else {
        current_fat = 1;
    }

    do {
        if (offset_in_sector < p_vol->bpb.bytes_per_sector - 1) {
            err = VFiPFSEC_ReadFAT(p_vol, buf, fat_sector, offset_in_sector, 2);
        } else {
            err = VFiPFSEC_ReadFAT(p_vol, buf, fat_sector, offset_in_sector, 1);
            if (err == 0) {
                err = VFiPFSEC_ReadFAT(p_vol, &buf[1], fat_sector + 1, 0, 1);
            }
        }

        if (err == 0x1000 && p_vol->p_callback) {
            result = p_vol->p_callback(p_vol->last_driver_error);
            if (result == 0) {
                continue;
            }
            if (result == 1 && p_vol->bpb.num_active_FATs >= 2 && current_fat < p_vol->bpb.num_active_FATs) {
                current_fat++;
                fat_sector += (u16)p_vol->bpb.sectors_per_FAT;
                continue;
            }
        }

        if (err != 0) {
            *p_value = -1;
            return err;
        }
    } while (err != 0);

    {
        u16 v = *(u16*)buf;
        raw_entry = (u16)((v & 0xff00) >> 8 | (v & 0x00ff) << 8);
    }
    if ((cluster & 1) != 0) {
        *p_value = (u32)(raw_entry >> 4);
    } else {
        *p_value = (u32)(raw_entry & 0xFFF);
    }

    return 0;
}

s32 VFiPFFAT12_ReadFATEntryPage(PF_VOLUME* p_vol, u16 cluster, u32* p_value, PF_CACHE_PAGE** pp_page) {
    s32 err;
    u32 offset;
    u32 sector;
    u32 current_fat;
    s32 result;

    if (!p_vol) {
        return 10;
    }
    if ((cluster < 2 || cluster >= p_vol->bpb.num_clusters + 2) && cluster != 0 && cluster != 1) {
        return 14;
    }

    offset = (u16)(cluster + (cluster >> 1));
    sector = (u16)(p_vol->bpb.active_FAT_sector + (offset >> p_vol->bpb.log2_bytes_per_sector));
    current_fat = 1;
    if ((p_vol->bpb.ext_flags & 0x80) != 0) {
        current_fat = p_vol->bpb.ext_flags & 7;
    }

    err = 0;
    while (1) {
        if (err == 0x1000 && p_vol->p_callback) {
            result = p_vol->p_callback(p_vol->last_driver_error);
            if (result == 0) {
                err = 0;
                continue;
            }
            if (result == 1 && p_vol->bpb.num_active_FATs >= 2 && current_fat < p_vol->bpb.num_active_FATs) {
                current_fat++;
                sector += p_vol->bpb.sectors_per_FAT;
                err = 0;
                continue;
            }
            break;
        }

        if ((*pp_page)->sector > sector || (*pp_page)->sector + p_vol->cache.fat_buff_size <= sector) {
            if ((*pp_page)->option == 1) {
                err = VFiPFFAT_UpdateFATEntry(p_vol, *pp_page);
                if (err != 0) {
                    return err;
                }
            }
            err = VFiPFCACHE_ReadFATPage(p_vol, sector, pp_page);
            if (err != 0) {
                continue;
            }
        } else {
            if (sector != (*pp_page)->sector + (((*pp_page)->p_buf - (*pp_page)->buffer) >> p_vol->bpb.log2_bytes_per_sector)) {
                (*pp_page)->p_buf = (*pp_page)->buffer + ((sector - (*pp_page)->sector) << p_vol->bpb.log2_bytes_per_sector);
            }
        }

        offset &= p_vol->bpb.bytes_per_sector - 1;
        if (offset == p_vol->bpb.bytes_per_sector - 1) {
            *p_value = (*pp_page)->p_buf[offset];
            if ((*pp_page)->sector > sector + 1 || (*pp_page)->sector + p_vol->cache.fat_buff_size <= sector + 1) {
                if ((*pp_page)->option == 1) {
                    err = VFiPFFAT_UpdateFATEntry(p_vol, *pp_page);
                    if (err != 0) {
                        return err;
                    }
                }
                err = VFiPFCACHE_ReadFATPage(p_vol, sector + 1, pp_page);
                if (err != 0) {
                    return err;
                }
            } else {
                if (sector + 1 != (*pp_page)->sector + (((*pp_page)->p_buf - (*pp_page)->buffer) >> p_vol->bpb.log2_bytes_per_sector)) {
                    (*pp_page)->p_buf = (*pp_page)->buffer + ((sector + 1 - (*pp_page)->sector) << p_vol->bpb.log2_bytes_per_sector);
                }
            }
            *p_value += *(*pp_page)->p_buf << 8;
        } else {
            *p_value = ((*pp_page)->p_buf[offset + 1] << 8) + (*pp_page)->p_buf[offset];
        }
        break;
    }

    if (err != 0) {
        *p_value = -1;
        return err;
    }
    if ((cluster & 1) != 0) {
        *p_value >>= 4;
    } else {
        *p_value &= 0xFFF;
    }
    return 0;
}

s32 VFiPFFAT12_WriteFATEntry(PF_VOLUME* p_vol, u16 cluster, u16 value) {
    s32 result;
    u16 fat_offset;
    u16 fat_sector;
    u16 offset_in_sector;
    u16 word;
    s32 err;
    u8 buf[2];
    u16 other_sector;
    u16 other_offset;

    if (!p_vol) {
        return 10;
    }

    if (!((cluster >= 2) && (cluster < p_vol->bpb.num_clusters + 2)) && cluster != 0 && cluster != 1) {
        return 14;
    }

    fat_offset = cluster + (cluster >> 1);
    fat_sector = p_vol->bpb.active_FAT_sector + (fat_offset >> p_vol->bpb.log2_bytes_per_sector);
    offset_in_sector = fat_offset & (p_vol->bpb.bytes_per_sector - 1);

    if ((cluster & 1) != 0) {
        result = VFiPFSEC_ReadFAT(p_vol, buf, fat_sector, offset_in_sector, 1);
        if (result != 0) {
            return result;
        }
        word = (value << 4) + (u16)(buf[0] & 0xF);
    } else {
        other_sector = p_vol->bpb.active_FAT_sector + ((fat_offset + 1) >> p_vol->bpb.log2_bytes_per_sector);
        other_offset = (fat_offset + 1) & (p_vol->bpb.bytes_per_sector - 1);
        result = VFiPFSEC_ReadFAT(p_vol, buf, other_sector, other_offset, 1);
        if (result != 0) {
            return result;
        }
        word = ((buf[0] << 8) & 0xF000) + (value & 0xFFF);
    }

    {
        u16 v = (u16)word;
        *(u16*)buf = (u16)((v & 0xFF00) >> 8 | (v & 0x00FF) << 8);
    }

    if (offset_in_sector < p_vol->bpb.bytes_per_sector - 1) {
        return VFiPFSEC_WriteFAT(p_vol, buf, fat_sector, offset_in_sector, 2);
    }

    err = VFiPFSEC_WriteFAT(p_vol, buf, fat_sector, offset_in_sector, 1);
    if (err == 0) {
        return VFiPFSEC_WriteFAT(p_vol, &buf[1], fat_sector + 1, 0, 1);
    }
    return err;
}

s32 VFiPFFAT12_WriteFATEntryPage(PF_VOLUME* p_vol, u16 cluster, u16 value, PF_CACHE_PAGE** pp_page) {
    s32 err;
    u32 fat_offset;
    u32 fat_sector;
    u16 offset_in_sector;
    u32 current_fat;
    s32 result;

    err = 0;
    if (!p_vol) {
        return 10;
    }
    if ((cluster < 2 || cluster >= p_vol->bpb.num_clusters + 2) && cluster != 0 && cluster != 1) {
        return 14;
    }
    fat_offset = (u16)(cluster + (cluster >> 1));
    fat_sector = (u16)(p_vol->bpb.active_FAT_sector + (fat_offset >> p_vol->bpb.log2_bytes_per_sector));
    offset_in_sector = fat_offset & (p_vol->bpb.bytes_per_sector - 1);

    if ((*pp_page)->sector > fat_sector || (*pp_page)->sector + p_vol->cache.fat_buff_size <= fat_sector) {
        err = VFiPFFAT_UpdateFATEntry(p_vol, *pp_page);
        if (err != 0) {
            return err;
        }
        current_fat = 1;
        if ((p_vol->bpb.ext_flags & 0x80) != 0) {
            current_fat = p_vol->bpb.ext_flags & 7;
        }
        do {
            err = VFiPFCACHE_ReadFATPage(p_vol, fat_sector, pp_page);
            if (err == 0x1000 && p_vol->p_callback) {
                result = p_vol->p_callback(p_vol->last_driver_error);
                if (result == 0) {
                    continue;
                }
                if (result == 1 && p_vol->bpb.num_active_FATs >= 2 && current_fat < p_vol->bpb.num_active_FATs) {
                    current_fat++;
                    fat_sector += p_vol->bpb.sectors_per_FAT;
                    continue;
                }
            }
            if (err != 0) {
                return err;
            }
        } while (err != 0);
    } else {
        if (fat_sector != (*pp_page)->sector + (((*pp_page)->p_buf - (*pp_page)->buffer) >> p_vol->bpb.log2_bytes_per_sector)) {
            (*pp_page)->p_buf = (*pp_page)->buffer + ((fat_sector - (*pp_page)->sector) << p_vol->bpb.log2_bytes_per_sector);
        }
    }

    if ((cluster & 1) != 0) {
        (*pp_page)->p_buf[offset_in_sector] = ((*pp_page)->p_buf[offset_in_sector] & 0xF) | ((value & 0xF) << 4);
        VFiPFCACHE_UpdateModifiedSector(p_vol, *pp_page, 1);
        if (offset_in_sector == p_vol->bpb.bytes_per_sector - 1u) {
            if ((*pp_page)->sector > fat_sector + 1 || (*pp_page)->sector + p_vol->cache.fat_buff_size <= fat_sector + 1) {
                err = VFiPFFAT_UpdateFATEntry(p_vol, *pp_page);
                if (err != 0) {
                    return err;
                }
                current_fat = 1;
                if ((p_vol->bpb.ext_flags & 0x80) != 0) {
                    current_fat = p_vol->bpb.ext_flags & 7;
                }
                do {
                    err = VFiPFCACHE_ReadFATPage(p_vol, fat_sector + 1, pp_page);
                    if (err == 0x1000 && p_vol->p_callback) {
                        result = p_vol->p_callback(p_vol->last_driver_error);
                        if (result == 0) {
                            continue;
                        }
                        if (result == 1 && p_vol->bpb.num_active_FATs >= 2 && current_fat < p_vol->bpb.num_active_FATs) {
                            current_fat++;
                            fat_sector += p_vol->bpb.sectors_per_FAT;
                            continue;
                        }
                    }
                    if (err != 0) {
                        return err;
                    }
                } while (err != 0);
                VFiPFCACHE_UpdateModifiedSector(p_vol, *pp_page, 1);
            } else {
                if (fat_sector + 1 != (*pp_page)->sector + (((*pp_page)->p_buf - (*pp_page)->buffer) >> p_vol->bpb.log2_bytes_per_sector)) {
                    (*pp_page)->p_buf = (*pp_page)->buffer + ((fat_sector + 1 - (*pp_page)->sector) << p_vol->bpb.log2_bytes_per_sector);
                    VFiPFCACHE_UpdateModifiedSector(p_vol, *pp_page, 1);
                }
            }
            *(*pp_page)->p_buf = value >> 4;
        } else {
            (*pp_page)->p_buf[offset_in_sector + 1] = value >> 4;
        }
    } else {
        (*pp_page)->p_buf[offset_in_sector] = value;
        VFiPFCACHE_UpdateModifiedSector(p_vol, *pp_page, 1);
        if (offset_in_sector == p_vol->bpb.bytes_per_sector - 1u) {
            if ((*pp_page)->sector > fat_sector + 1 || (*pp_page)->sector + p_vol->cache.fat_buff_size <= fat_sector + 1) {
                err = VFiPFFAT_UpdateFATEntry(p_vol, *pp_page);
                if (err != 0) {
                    return err;
                }
                current_fat = 1;
                if ((p_vol->bpb.ext_flags & 0x80) != 0) {
                    current_fat = p_vol->bpb.ext_flags & 7;
                }
                do {
                    err = VFiPFCACHE_ReadFATPage(p_vol, fat_sector + 1, pp_page);
                    if (err == 0x1000 && p_vol->p_callback) {
                        result = p_vol->p_callback(p_vol->last_driver_error);
                        if (result == 0) {
                            continue;
                        }
                        if (result == 1 && p_vol->bpb.num_active_FATs >= 2 && current_fat < p_vol->bpb.num_active_FATs) {
                            current_fat++;
                            fat_sector += p_vol->bpb.sectors_per_FAT;
                            continue;
                        }
                    }
                    if (err != 0) {
                        return err;
                    }
                } while (err != 0);
                VFiPFCACHE_UpdateModifiedSector(p_vol, *pp_page, 1);
            } else {
                if (fat_sector + 1 != (*pp_page)->sector + (((*pp_page)->p_buf - (*pp_page)->buffer) >> p_vol->bpb.log2_bytes_per_sector)) {
                    (*pp_page)->p_buf = (*pp_page)->buffer + ((fat_sector + 1 - (*pp_page)->sector) << p_vol->bpb.log2_bytes_per_sector);
                    VFiPFCACHE_UpdateModifiedSector(p_vol, *pp_page, 1);
                }
            }
            *(*pp_page)->p_buf = (*(*pp_page)->p_buf & 0xF0) | (value >> 8);
        } else {
            (*pp_page)->p_buf[offset_in_sector + 1] = (value >> 8) | ((*pp_page)->p_buf[offset_in_sector + 1] & 0xF0);
        }
    }
    return err;
}
