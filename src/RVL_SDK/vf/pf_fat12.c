#include "revolution/vf/pf_fat12.h"
#include "revolution/vf/pf_cache.h"
#include "revolution/vf/pf_fat.h"
#include "revolution/vf/pf_sector.h"

#define ROR32(x, n) ((u32)(x >> n) | (x << (32 - n)))

s32 VFiPFFAT12_ReadFATEntry(PF_VOLUME* p_vol, u16 cluster, u32* p_value) {
    char v4;
    int result;
    u16 ext_flags;
    int v8;
    unsigned int current_fat;
    u16 fat_sector;
    int offset_in_sector;
    int err;
    int (*p_callback)();
    int (*v14)();
    int v15;
    unsigned int num_active_FATs;
    u16 v17;
    unsigned int v18;
    u8 v19[2];

    v4 = cluster;
    if (!p_vol) {
        result = 10;
        *p_value = -1;
        return result;
    }
    if ((cluster < 2u || cluster >= p_vol->bpb.num_clusters + 2) && cluster >= 2u) {
        result = 14;
        *p_value = -1;
        return result;
    }
    ext_flags = p_vol->bpb.ext_flags;
    v8 = (cluster + (cluster >> 1));
    current_fat = 1;
    fat_sector = p_vol->bpb.active_FAT_sector + (v8 >> p_vol->bpb.log2_bytes_per_sector);
    if ((ext_flags & 0x80) != 0) {
        current_fat = ext_flags & 7;
    }
    offset_in_sector = (v8 & (p_vol->bpb.bytes_per_sector - 1));
    do {
        if (offset_in_sector < p_vol->bpb.bytes_per_sector - 1) {
            err = VFiPFSEC_ReadFAT(p_vol, v19, fat_sector, offset_in_sector, 2u);
        } else {
            err = VFiPFSEC_ReadFAT(p_vol, v19, fat_sector, offset_in_sector, 1u);
            if (!err) {
                err = VFiPFSEC_ReadFAT(p_vol, &v19[1], fat_sector + 1, 0, 1u);
            }
        }
        if (err == 0x1000) {
            p_callback = p_vol->p_callback;
            if (p_callback) {
                v14 = p_vol->p_callback;
                v15 = (p_callback)(p_vol->last_driver_error);
                if (v15) {
                    if (v15 == 1 && (num_active_FATs = p_vol->bpb.num_active_FATs, num_active_FATs >= 2) && current_fat < num_active_FATs) {
                        ++current_fat;
                        fat_sector += p_vol->bpb.sectors_per_FAT;
                    }
                }
            }
        } else {
            if (err) {
                result = err;
                *p_value = -1;
                return result;
            }
        }
    } while (err);

    v17 = ROR32(*v19, 8) | (*v19 << 8);
    if ((v4 & 1) != 0) {
        v18 = v17 >> 4;
    } else {
        v18 = v17 & 0xFFF;
    }
    *p_value = v18;
    return 0;
}

s32 VFiPFFAT12_ReadFATEntryPage(PF_VOLUME* p_vol, u16 cluster, u32* p_value, PF_CACHE_PAGE** pp_page) {
    char v5;
    int result;
    unsigned int v9;
    unsigned int sector;
    unsigned int current_fat;
    int err;
    unsigned int offset;
    unsigned int v14;
    int result_1;

    v5 = cluster;

    if (!p_vol)
        return 10;

    if ((cluster < 2u || cluster >= p_vol->bpb.num_clusters + 2) && cluster >= 2u)
        return 14;

    v9 = cluster + (cluster >> 1);
    sector = p_vol->bpb.active_FAT_sector + (v9 >> p_vol->bpb.log2_bytes_per_sector);

    if ((p_vol->bpb.ext_flags & 0x80) != 0)
        current_fat = p_vol->bpb.ext_flags & 7;
    else
        current_fat = 1;

    err = 0;

    while (1) {
        while (err == 4096 && p_vol->p_callback) {
            result_1 = (p_vol->p_callback)(p_vol->last_driver_error);
            if (result_1) {
                if (result_1 != 1 || p_vol->bpb.num_active_FATs < 2u || current_fat >= p_vol->bpb.num_active_FATs) {
                    *p_value = -1;
                    return err;
                }
                ++current_fat;
                sector += p_vol->bpb.sectors_per_FAT;
                err = 0;
            } else {
                err = 0;
            }
        }

        if ((*pp_page)->sector <= sector && sector < (*pp_page)->sector + p_vol->cache.fat_buff_size)
            break;

        if ((*pp_page)->option == 1) {
            result = VFiPFFAT_UpdateFATEntry(p_vol, *pp_page);
            if (result)
                return result;
        }

        err = VFiPFCACHE_ReadFATPage(p_vol, sector, pp_page);
        if (err) {
            *p_value = -1;
            return err;
        }
    }

    if (sector != (*pp_page)->sector + (((*pp_page)->p_buf - (*pp_page)->buffer) >> p_vol->bpb.log2_bytes_per_sector))
        (*pp_page)->p_buf = &(*pp_page)->buffer[(sector - (*pp_page)->sector) << p_vol->bpb.log2_bytes_per_sector];

    offset = v9 & (p_vol->bpb.bytes_per_sector - 1);

    if (offset != p_vol->bpb.bytes_per_sector - 1) {
        *p_value = ((*pp_page)->p_buf[offset + 1] << 8) + (*pp_page)->p_buf[offset];
    } else {
        *p_value = (*pp_page)->p_buf[offset];

        if (!((*pp_page)->sector <= sector + 1 && sector + 1 < (*pp_page)->sector + p_vol->cache.fat_buff_size)) {
            if ((*pp_page)->option == 1) {
                result = VFiPFFAT_UpdateFATEntry(p_vol, *pp_page);
                if (result)
                    return result;
            }
            err = VFiPFCACHE_ReadFATPage(p_vol, sector + 1, pp_page);
            if (err) {
                *p_value = -1;
                return err;
            }
        }

        if (sector + 1 != (*pp_page)->sector + (((*pp_page)->p_buf - (*pp_page)->buffer) >> p_vol->bpb.log2_bytes_per_sector))
            (*pp_page)->p_buf = &(*pp_page)->buffer[(sector + 1 - (*pp_page)->sector) << p_vol->bpb.log2_bytes_per_sector];

        *p_value += *(*pp_page)->p_buf << 8;
    }

    if ((v5 & 1) != 0)
        v14 = *p_value >> 4;
    else
        v14 = *p_value & 0xFFF;

    *p_value = v14;
    return 0;
}

s32 VFiPFFAT12_WriteFATEntry(PF_VOLUME* p_vol, u16 cluster, u16 value) {
    int result;
    unsigned short fat_offset;
    unsigned short fat_sector;
    unsigned short offset_in_sector;
    short word;
    int err;
    u8 v11[4];
    unsigned short v12;
    unsigned short v13;
    if (!p_vol)
        return 10;
    if ((cluster < 2u || cluster >= p_vol->bpb.num_clusters + 2) && cluster >= 2u)
        return 14;
    fat_offset = cluster + (cluster >> 1);
    fat_sector = p_vol->bpb.active_FAT_sector + (fat_offset >> p_vol->bpb.log2_bytes_per_sector);
    offset_in_sector = fat_offset & (p_vol->bpb.bytes_per_sector - 1);
    if ((cluster & 1) != 0) {
        result = VFiPFSEC_ReadFAT(p_vol, v11, (p_vol->bpb.active_FAT_sector + ((cluster + (cluster >> 1)) >> p_vol->bpb.log2_bytes_per_sector)),
                                  offset_in_sector, 1u);
        if (result)
            return result;
        word = 16 * value + (v11[0] & 0xF);
    } else {
        v13 = p_vol->bpb.active_FAT_sector + (((cluster + (cluster >> 1)) + 1) >> p_vol->bpb.log2_bytes_per_sector);
        v12 = (fat_offset + 1) & (p_vol->bpb.bytes_per_sector - 1);
        result = VFiPFSEC_ReadFAT(p_vol, v11, v13, v12, 1u);
        if (result)
            return result;
        word = ((v11[0] << 8) & 0xF000) + (value & 0xFFF);
    }
    *v11 = ((word & 0xFF00) >> 8) | ((word & 0x00FF) << 8);
    if (offset_in_sector < p_vol->bpb.bytes_per_sector - 1)
        return VFiPFSEC_WriteFAT(p_vol, v11, fat_sector, offset_in_sector, 2u);
    err = VFiPFSEC_WriteFAT(p_vol, v11, fat_sector, offset_in_sector, 1u);
    if (!err)
        return VFiPFSEC_WriteFAT(p_vol, &v11[1], fat_sector + 1, 0, 1u);
    return err;
}

s32 VFiPFFAT12_WriteFATEntryPage(PF_VOLUME* p_vol, u16 cluster, u16 value, PF_CACHE_PAGE** pp_page) {
    char v5;
    int result;
    unsigned int fat_sector;
    u16 offset_in_sector;
    unsigned int v11;
    int FATPage;
    int v13;
    unsigned int current_fat;
    int err;
    int v19;

    v5 = cluster;

    if (!p_vol)
        return 10;
    if ((cluster < 2u || cluster >= p_vol->bpb.num_clusters + 2) && cluster >= 2u)
        return 14;

    fat_sector = p_vol->bpb.active_FAT_sector + ((cluster + (cluster >> 1)) >> p_vol->bpb.log2_bytes_per_sector);
    offset_in_sector = (cluster + (cluster >> 1)) & (p_vol->bpb.bytes_per_sector - 1);

    if ((*pp_page)->sector > fat_sector || (*pp_page)->sector + p_vol->cache.fat_buff_size <= fat_sector) {
        result = VFiPFFAT_UpdateFATEntry(p_vol, *pp_page);
        if (result)
            return result;

        v11 = (p_vol->bpb.ext_flags & 0x80) ? (p_vol->bpb.ext_flags & 7) : 1;

        while (1) {
            FATPage = VFiPFCACHE_ReadFATPage(p_vol, fat_sector, pp_page);
            if (FATPage == 4096 && p_vol->p_callback) {
                v13 = p_vol->p_callback(p_vol->last_driver_error);
                if (!v13)
                    break;
                if (v13 == 1 && p_vol->bpb.num_active_FATs >= 2u && v11 < p_vol->bpb.num_active_FATs) {
                    ++v11;
                    fat_sector += p_vol->bpb.sectors_per_FAT;
                    continue;
                }
            }
            if (FATPage)
                return FATPage;
            break;
        }
    }

    if (fat_sector != (*pp_page)->sector + (((*pp_page)->p_buf - (*pp_page)->buffer) >> p_vol->bpb.log2_bytes_per_sector))
        (*pp_page)->p_buf = &(*pp_page)->buffer[(fat_sector - (*pp_page)->sector) << p_vol->bpb.log2_bytes_per_sector];

    if ((v5 & 1) == 0) {
        (*pp_page)->p_buf[offset_in_sector] = value & 0xFF;
        VFiPFCACHE_UpdateModifiedSector(p_vol, *pp_page, 1);

        if (offset_in_sector != p_vol->bpb.bytes_per_sector - 1) {
            (*pp_page)->p_buf[offset_in_sector + 1] = (value >> 8) | ((*pp_page)->p_buf[offset_in_sector + 1] & 0xF0);
            return 0;
        }
    } else {
        (*pp_page)->p_buf[offset_in_sector] = ((*pp_page)->p_buf[offset_in_sector] & 0x0F) | ((value & 0xF) << 4);
        VFiPFCACHE_UpdateModifiedSector(p_vol, *pp_page, 1);

        if (offset_in_sector != p_vol->bpb.bytes_per_sector - 1) {
            (*pp_page)->p_buf[offset_in_sector + 1] = value >> 4;
            return 0;
        }
    }

    if ((*pp_page)->sector <= fat_sector + 1 && (*pp_page)->sector + p_vol->cache.fat_buff_size > fat_sector + 1) {
        if (fat_sector + 1 != (*pp_page)->sector + (((*pp_page)->p_buf - (*pp_page)->buffer) >> p_vol->bpb.log2_bytes_per_sector)) {
            (*pp_page)->p_buf = &(*pp_page)->buffer[(fat_sector + 1 - (*pp_page)->sector) << p_vol->bpb.log2_bytes_per_sector];
            VFiPFCACHE_UpdateModifiedSector(p_vol, *pp_page, 1);
        }

        if ((v5 & 1) == 0) {
            *(*pp_page)->p_buf = (*(*pp_page)->p_buf & 0xF0) | (value >> 8);
        } else {
            *(*pp_page)->p_buf = value >> 4;
        }

        return 0;
    }

    result = VFiPFFAT_UpdateFATEntry(p_vol, *pp_page);
    if (result)
        return result;

    current_fat = (p_vol->bpb.ext_flags & 0x80) ? (p_vol->bpb.ext_flags & 7) : 1;

    while (1) {
        err = VFiPFCACHE_ReadFATPage(p_vol, fat_sector + 1, pp_page);
        if (err != 4096 || !p_vol->p_callback)
            break;

        v19 = p_vol->p_callback(p_vol->last_driver_error);
        if (!v19 || v19 != 1 || p_vol->bpb.num_active_FATs < 2u || current_fat >= p_vol->bpb.num_active_FATs)
            break;

        ++current_fat;
        fat_sector += p_vol->bpb.sectors_per_FAT;
    }

    if (err)
        return err;

    VFiPFCACHE_UpdateModifiedSector(p_vol, *pp_page, 1);

    if ((v5 & 1) == 0)
        *(*pp_page)->p_buf = (*(*pp_page)->p_buf & 0xF0) | (value >> 8);
    else
        *(*pp_page)->p_buf = value >> 4;

    return 0;
}
