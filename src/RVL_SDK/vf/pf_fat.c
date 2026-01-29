#include "revolution/vf/pf_fat.h"
#include "revolution/vf/pf_cache.h"
#include "revolution/vf/pf_fat12.h"
#include "revolution/vf/pf_fat16.h"
#include "revolution/vf/pf_fat32.h"

static struct {
    unsigned long bad;
    unsigned long eoc1;
    unsigned long eoc2;
    unsigned long fat0_mask;
    unsigned long fat1;
} fat_special_values[3] = {
    {0xFF7, 0xFF8, 0xFFF, 0xF00, 0xFFF},                     // FAT12
    {0xFFF7, 0xFFF8, 0xFFFF, 0xFF00, 0xFFFF},                // FAT16
    {0xFFFFFF7, 0xFFFFFF8, 0xFFFFFFF, 0xFFFFF00, 0xFFFFFFF}  // FAT32
};

s32 VFiPFFAT_ReadFATSector(PF_VOLUME* p_vol, PF_CACHE_PAGE** pp_page, u32 cluster) {
    int fat_type;
    unsigned int offset;
    unsigned int sector;
    unsigned int current_fat;
    int err;
    int v10;

    fat_type = p_vol->bpb.fat_type;
    if (fat_type == FAT_16) {
        offset = 2 * cluster;
    } else if (fat_type >= FAT_16) {
        if (fat_type >= (FAT_32 | FAT_16))
            return 15;
        offset = 4 * cluster;
    } else {
        if (fat_type < FAT_12)
            return 15;
        offset = cluster + (cluster >> 1);
    }

    sector = p_vol->bpb.active_FAT_sector + (offset >> p_vol->bpb.log2_bytes_per_sector);
    current_fat = (p_vol->bpb.ext_flags & 0x80) ? (p_vol->bpb.ext_flags & 7) : 1;

    while (1) {
        err = VFiPFCACHE_ReadFATPage(p_vol, sector, pp_page);
        if (err != 4096 || !p_vol->p_callback) {
            if (err)
                return err;
            break;
        }

        v10 = p_vol->p_callback(p_vol->last_driver_error);
        if (!v10)
            continue;

        if (v10 == 1 && p_vol->bpb.num_active_FATs >= 2u && current_fat < p_vol->bpb.num_active_FATs) {
            ++current_fat;
            sector += p_vol->bpb.sectors_per_FAT;
        } else {
            if (err)
                return err;
            break;
        }
    }

    return 0;
}

s32 VFiPFFAT_SearchForNumFreeClusters(PF_VOLUME* p_vol, unsigned int start_cluster, unsigned int end_cluster, unsigned int num_cluster,
                                      unsigned int* p_start_free_cluster, unsigned int* p_last_free_cluster) {
    unsigned int v7;
    unsigned int v8;
    unsigned int success_num;
    unsigned int save_start_cluster;
    unsigned int v13;
    unsigned int upper_bound_cluster;
    int result;
    PF_CACHE_PAGE* p_page;
    unsigned int temp_start_cluster;
    int search_flg;
    u32 fat_entry[14];

    v7 = start_cluster;
    v8 = end_cluster;
    search_flg = 0;
    temp_start_cluster = 0;
    *p_start_free_cluster = -1;
    *p_last_free_cluster = -1;
    success_num = 0;
    save_start_cluster = -1;
    v13 = 0;
    upper_bound_cluster = p_vol->bpb.num_clusters + 2;
    if (start_cluster < 2 || start_cluster >= p_vol->bpb.num_clusters + 2)
        v7 = 2;
    if (end_cluster < 2 || end_cluster >= p_vol->bpb.num_clusters + 2)
        v8 = p_vol->bpb.num_clusters + 1;
    temp_start_cluster = v7;
    result = VFiPFFAT_ReadFATSector(p_vol, &p_page, v7);
    if (!result) {
        p_page->option = 0;
        while (v7 <= v8) {
            if (upper_bound_cluster <= v7)
                v7 = 2;
            result = VFiPFFAT_ReadFATEntryPage(p_vol, v7, fat_entry, &p_page);
            if (result)
                return result;
            if (fat_entry[0]) {
                if (v13 < success_num) {
                    save_start_cluster = *p_start_free_cluster;
                    v13 = success_num;
                }
                *p_start_free_cluster = -1;
                success_num = 0;
            } else {
                if (*p_start_free_cluster == -1)
                    *p_start_free_cluster = v7;
                if (++success_num >= num_cluster) {
                    *p_last_free_cluster = v7;
                    return 0;
                }
            }
            ++v7;
            if (!search_flg && !success_num && v7 > 2 && v7 == upper_bound_cluster) {
                v8 = temp_start_cluster;
                v7 = 2;
                search_flg = 1;
            }
        }
        if (v13 < success_num) {
            save_start_cluster = *p_start_free_cluster;
            v13 = success_num;
        }
        if (v13) {
            *p_start_free_cluster = save_start_cluster;
            *p_last_free_cluster = *p_start_free_cluster + v13 - 1;
        }
        return 0;
    }
    return result;
}

s32 VFiPFFAT_UpdateClusterLink(PF_FFD* p_ffd, unsigned int cluster, unsigned int chain_index) {
    if (p_ffd->cluster_link.max_count > p_ffd->cluster_link.position && p_ffd->cluster_link.max_count) {
        if (chain_index == p_ffd->cluster_link.position * (p_ffd->cluster_link.interval + 1)) {
            p_ffd->cluster_link.interval_offset = 0;
            p_ffd->cluster_link.buffer[p_ffd->cluster_link.position++] = cluster;
        } else {
            ++p_ffd->cluster_link.interval_offset;
        }
        p_ffd->cluster_link.save_index = chain_index;
    }
    return 0;
}

s32 VFiPFFAT_FindClusterLink(PF_FFD* p_ffd, unsigned int chain_index, unsigned int* p_cluster, unsigned int* is_found) {
    int result;
    unsigned int position;
    unsigned int offset;
    unsigned int current_cluster;
    unsigned int i;
    PF_CACHE_PAGE* p_page;
    u32 next_cluster;

    next_cluster = -1;
    *is_found = 0;
    if (!p_ffd->cluster_link.position)
        return 0;
    if (p_ffd->cluster_link.save_index < chain_index)
        return 0;
    position = chain_index / (p_ffd->cluster_link.interval + 1);
    offset = chain_index % (p_ffd->cluster_link.interval + 1);
    if (offset) {
        current_cluster = p_ffd->cluster_link.buffer[position];
        result = VFiPFFAT_ReadFATSector(p_ffd->p_vol, &p_page, current_cluster);
        if (result)
            return result;
        for (i = offset; i; --i) {
            result = VFiPFFAT_ReadFATEntryPage(p_ffd->p_vol, current_cluster, &next_cluster, &p_page);
            if (result)
                return result;
            if (!next_cluster)
                return 13;
            current_cluster = next_cluster;
        }
        if (!next_cluster)
            return 13;
        if (next_cluster == fat_special_values[p_ffd->p_vol->bpb.fat_type].eoc2)
            return 0;
        *p_cluster = next_cluster;
        *is_found = 1;
    } else {
        *p_cluster = p_ffd->cluster_link.buffer[position];
        *is_found = 1;
    }
    return 0;
}

s32 VFiPFFAT_FindClusterLinkPage(PF_FFD* p_ffd, unsigned int chain_index, u32* p_cluster, u32* is_found, PF_CACHE_PAGE* p_page) {
    int result;
    unsigned int position;
    u32 current_cluster;
    unsigned int i;
    PF_CACHE_PAGE* v12;
    u32 next_cluster;

    v12 = p_page;
    next_cluster = -1;
    *is_found = 0;
    if (!p_ffd->cluster_link.position)
        return 0;
    if (p_ffd->cluster_link.save_index < chain_index)
        return 0;
    position = chain_index / (p_ffd->cluster_link.interval + 1);
    if (chain_index % (p_ffd->cluster_link.interval + 1)) {
        current_cluster = p_ffd->cluster_link.buffer[position];
        for (i = chain_index % (p_ffd->cluster_link.interval + 1); i; --i) {
            result = VFiPFFAT_ReadFATEntryPage(p_ffd->p_vol, current_cluster, &next_cluster, &v12);
            if (result)
                return result;
            if (!next_cluster)
                return 13;
            current_cluster = next_cluster;
        }
        if (!next_cluster)
            return 13;
        if (next_cluster == fat_special_values[p_ffd->p_vol->bpb.fat_type].eoc2)
            return 0;
        *p_cluster = next_cluster;
        *is_found = 1;
    } else {
        *p_cluster = p_ffd->cluster_link.buffer[position];
        *is_found = 1;
    }
    return 0;
}

static s32 VFiPFFAT_WriteFATEntry(PF_VOLUME* p_vol, u32 cluster, u32 value) {
    s32 fat_type;

    fat_type = p_vol->bpb.fat_type;
    if (fat_type == FAT_16) {
        if (value <= 0xFFFF)
            return VFiPFFAT16_WriteFATEntry(p_vol, cluster, value);
        else
            return 16;
    } else if (fat_type >= FAT_16) {
        if (fat_type >= (FAT_32 | FAT_16))
            return 15;
        if (value <= 0xFFFFFFF)
            return VFiPFFAT32_WriteFATEntry(p_vol, cluster, value);
        else
            return 16;
    } else {
        if (fat_type < FAT_12)
            return 15;
        if (value <= 0xFFF)
            return VFiPFFAT12_WriteFATEntry(p_vol, cluster, value);
        else
            return 16;
    }
}

s32 VFiPFFAT_ReadClusterPage(PF_FFD* p_ffd, unsigned int cluster, unsigned int chain_index, u32* next_cluster, u32* chk_clstlnk,
                             PF_CACHE_PAGE** p_page) {
    int err;

    *next_cluster = -1;
    if (!p_ffd->cluster_link.buffer || !*chk_clstlnk ||
        (err = VFiPFFAT_FindClusterLinkPage(p_ffd, chain_index, next_cluster, chk_clstlnk, *p_page)) == 0) {
        if (p_ffd->cluster_link.buffer && *chk_clstlnk)
            return 0;
        err = VFiPFFAT_ReadFATEntryPage(p_ffd->p_vol, cluster, next_cluster, p_page);
        if (err)
            return err;
        if (!*next_cluster)
            return 13;
        if (*next_cluster == -1)
            return 6;
        if (!p_ffd->cluster_link.buffer)
            return 0;
        if (*next_cluster == fat_special_values[p_ffd->p_vol->bpb.fat_type].eoc2)
            return 0;
        err = VFiPFFAT_UpdateClusterLink(p_ffd, *next_cluster, chain_index);
        if (!err)
            return 0;
    }
    return err;
}

s32 VFiPFFAT_ClearClusterLink(PF_FFD* p_ffd, u32 chain_index) {
    unsigned int v2;

    v2 = chain_index / (p_ffd->cluster_link.interval + 1);
    if (p_ffd->cluster_link.max_count >= v2) {
        if (!(chain_index % (p_ffd->cluster_link.interval + 1)))
            p_ffd->cluster_link.buffer[v2] = 0;
        if (p_ffd->cluster_link.save_index >= chain_index) {
            p_ffd->cluster_link.position = (chain_index - 1) / (p_ffd->cluster_link.interval + 1);
            p_ffd->cluster_link.interval_offset = (chain_index - 1) % (p_ffd->cluster_link.interval + 1);
            p_ffd->cluster_link.save_index = chain_index - 1;
        }
    }
    return 0;
}

s32 VFiPFFAT_WriteCluster(PF_FFD* p_ffd, unsigned int cluster, unsigned int chain_index, unsigned int next_cluster, unsigned int use_clstlnk) {
    int err;
    unsigned int v10;
    unsigned int access_cluster;

    v10 = use_clstlnk;
    err = VFiPFFAT_WriteFATEntry(p_ffd->p_vol, cluster, next_cluster);
    if (!err) {
        if (!p_ffd->cluster_link.buffer || !v10)
            return 0;
        if (!next_cluster) {
            VFiPFFAT_ClearClusterLink(p_ffd, chain_index);
            return 0;
        }
        if (cluster == fat_special_values[p_ffd->p_vol->bpb.fat_type].eoc2)
            return 0;
        err = VFiPFFAT_FindClusterLink(p_ffd, chain_index, &access_cluster, &v10);
        if (!err) {
            if (v10)
                return 0;
            err = VFiPFFAT_UpdateClusterLink(p_ffd, cluster, chain_index);
            if (!err)
                return 0;
        }
    }
    return err;
}
