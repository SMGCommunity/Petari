#include "revolution/vf/pf_fat.h"
#include "revolution/types.h"
#include "revolution/vf/pf_cache.h"
#include "revolution/vf/pf_clib.h"
#include "revolution/vf/pf_driver.h"
#include "revolution/vf/pf_fat12.h"
#include "revolution/vf/pf_fat16.h"
#include "revolution/vf/pf_fat32.h"
#include "revolution/vf/pf_sector.h"

static struct {
    u32 bad;
    u32 eoc1;
    u32 eoc2;
    u32 fat0_mask;
    u32 fat1;
} fat_special_values[3] = {
    {0xFF7, 0xFF8, 0xFFF, 0xF00, 0xFFF},                     // FAT12
    {0xFFF7, 0xFFF8, 0xFFFF, 0xFF00, 0xFFFF},                // FAT16
    {0xFFFFFF7, 0xFFFFFF8, 0xFFFFFFF, 0xFFFFF00, 0xFFFFFFF}  // FAT32
};

static s32 VFiPFFAT_ReadFATEntry(PF_VOLUME* p_vol, u32 cluster, u32* p_value) {
    u32 fat_type;

    fat_type = p_vol->bpb.fat_type;
    if (fat_type == FAT_16)
        return VFiPFFAT16_ReadFATEntry(p_vol, cluster, p_value);
    if (fat_type >= FAT_16) {
        if (fat_type < (FAT_32 | FAT_16))
            return VFiPFFAT32_ReadFATEntry(p_vol, cluster, p_value);
    } else if (fat_type >= FAT_12) {
        return VFiPFFAT12_ReadFATEntry(p_vol, cluster, p_value);
    }
    *p_value = -1;
    return 15;
}

static s32 VFiPFFAT_ReadFATEntryPage(PF_VOLUME* p_vol, u32 cluster, u32* p_value, PF_CACHE_PAGE** p_page) {
    u32 fat_type;  // r0

    fat_type = p_vol->bpb.fat_type;
    if (fat_type == FAT_16)
        return VFiPFFAT16_ReadFATEntryPage(p_vol, cluster, p_value, p_page);
    if (fat_type >= FAT_16) {
        if (fat_type >= (FAT_32 | FAT_16))
            return 15;
        return VFiPFFAT32_ReadFATEntryPage(p_vol, cluster, p_value, p_page);
    } else {
        if (fat_type < FAT_12)
            return 15;
        return VFiPFFAT12_ReadFATEntryPage(p_vol, cluster, p_value, p_page);
    }
}

s32 VFiPFFAT_ReadFATSector(PF_VOLUME* p_vol, PF_CACHE_PAGE** pp_page, u32 cluster) {
    int fat_type;
    u32 offset;
    u32 sector;
    u32 current_fat;
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

s32 VFiPFFAT_SearchForNumFreeClusters(PF_VOLUME* p_vol, u32 start_cluster, u32 end_cluster, u32 num_cluster, u32* p_start_free_cluster,
                                      u32* p_last_free_cluster) {
    u32 v7;
    u32 v8;
    u32 success_num;
    u32 save_start_cluster;
    u32 v13;
    u32 upper_bound_cluster;
    int result;
    PF_CACHE_PAGE* p_page;
    u32 temp_start_cluster;
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

s32 VFiPFFAT_UpdateClusterLink(PF_FFD* p_ffd, u32 cluster, u32 chain_index) {
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

s32 VFiPFFAT_FindClusterLink(PF_FFD* p_ffd, u32 chain_index, u32* p_cluster, u32* is_found) {
    int result;
    u32 position;
    u32 offset;
    u32 current_cluster;
    u32 i;
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

s32 VFiPFFAT_FindClusterLinkPage(PF_FFD* p_ffd, u32 chain_index, u32* p_cluster, u32* is_found, PF_CACHE_PAGE* p_page) {
    int result;
    u32 position;
    u32 current_cluster;
    u32 i;
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

s32 VFiPFFAT_ReadClusterPage(PF_FFD* p_ffd, u32 cluster, u32 chain_index, u32* next_cluster, u32* chk_clstlnk, PF_CACHE_PAGE** p_page) {
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
    u32 v2;

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

s32 VFiPFFAT_WriteCluster(PF_FFD* p_ffd, u32 cluster, u32 chain_index, u32 next_cluster, u32 use_clstlnk) {
    int err;
    u32 v10;
    u32 access_cluster;

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

s32 VFiPFFAT_WriteClusterPage(PF_FFD* p_ffd, u32 cluster, u32 chain_index, u32 next_cluster, u32 use_clstlnk, PF_CACHE_PAGE** p_page) {
    int err;
    u32 v12;
    u32 v13;

    v12 = use_clstlnk;
    err = VFiPFFAT_WriteFATEntryPage(p_ffd->p_vol, cluster, next_cluster, p_page);
    if (!err) {
        if (!p_ffd->cluster_link.buffer || !v12)
            return 0;
        if (!next_cluster) {
            VFiPFFAT_ClearClusterLink(p_ffd, chain_index);
            return 0;
        }
        if (cluster == fat_special_values[p_ffd->p_vol->bpb.fat_type].eoc2)
            return 0;
        err = VFiPFFAT_FindClusterLinkPage(p_ffd, chain_index, &v13, &v12, *p_page);
        if (!err) {
            if (v12)
                return 0;
            err = VFiPFFAT_UpdateClusterLink(p_ffd, cluster, chain_index);
            if (!err)
                return 0;
        }
    }
    return err;
}
s32 VFiPFFAT_DoAllocateChain(PF_FFD* p_ffd, u32 chain_len, u32 chain_index, u32* p_chain_start, u32* p_last_allocated) {
    PF_VOLUME* p_vol;
    int result;
    u32 i;
    PF_CACHE_PAGE* v12;
    u32 last_allocated_cluster;
    u32 first_allocated_cluster;
    u32 fat_type;
    u32 eoc2;

    p_vol = p_ffd->p_vol;
    fat_type = p_vol->bpb.fat_type;
    eoc2 = fat_special_values[fat_type].eoc2;
    *p_chain_start = -1;
    *p_last_allocated = -1;
    last_allocated_cluster = -1;
    result =
        VFiPFFAT_SearchForNumFreeClusters(p_vol, p_vol->last_free_cluster, 0xFFFFFFFF, chain_len, &first_allocated_cluster, &last_allocated_cluster);
    if (!result) {
        if (first_allocated_cluster == -1)
            return 0;
        result = VFiPFFAT_ReadFATSector(p_vol, &v12, first_allocated_cluster);
        if (!result) {
            v12->option = 0;
            for (i = first_allocated_cluster; i < last_allocated_cluster; ++i) {
                result = VFiPFFAT_WriteClusterPage(p_ffd, i, chain_index, i + 1, 1u, &v12);
                if (result)
                    return result;
                if ((p_vol->fsi_flag & 4) != 0 && p_vol->num_free_clusters != -1 && p_vol->num_free_clusters &&
                    (p_vol->bpb.fat_type != FAT_32 || last_allocated_cluster != p_vol->bpb.root_dir_cluster)) {
                    --p_vol->num_free_clusters;
                }
                if (*p_chain_start == -1)
                    *p_chain_start = i;
                *p_last_allocated = i;
                ++chain_index;
            }
            if (last_allocated_cluster != -1) {
                result = VFiPFFAT_WriteClusterPage(p_ffd, last_allocated_cluster, chain_index, eoc2, 1u, &v12);
                if (result)
                    return result;
                if (*p_chain_start == -1)
                    *p_chain_start = i;
                *p_last_allocated = i;
                if (p_vol->num_free_clusters != -1 && p_vol->num_free_clusters &&
                    (p_vol->bpb.fat_type != FAT_32 || last_allocated_cluster != p_vol->bpb.root_dir_cluster)) {
                    --p_vol->num_free_clusters;
                }
            }
            p_vol->last_free_cluster = last_allocated_cluster + 1;
            p_ffd->last_cluster.num_last_cluster = last_allocated_cluster;
            p_ffd->last_cluster.max_chain_index = chain_index;
            *p_last_allocated = last_allocated_cluster;
            result = VFiPFFAT_UpdateFATEntry(p_vol, v12);
            if (!result) {
                if (p_vol->bpb.fat_type == FAT_32 && (p_vol->fsi_flag & 2) != 0) {
                    if (VFiPFFAT_RefreshFSINFO(p_vol)) {
                        p_vol->num_free_clusters = -1;
                        p_vol->fsi_flag &= ~4u;
                    }
                }
                return 0;
            }
        }
    }
    return result;
}

static s32 VFiPFFAT_AllocateChain(PF_FFD* p_ffd, u32 chain_len, u32 chain_index, u32* p_chain_start, u32* p_last_allocated) {
    int err;

    err = VFiPFFAT_DoAllocateChain(p_ffd, chain_len, chain_index, p_chain_start, p_last_allocated);
    if (!err)
        return 0;
    if (*p_chain_start != -1)
        VFiPFFAT_FreeChain(p_ffd, *p_chain_start, chain_index, -1);
    *p_chain_start = -1;
    *p_last_allocated = -1;
    return err;
}

int VFiPFFAT_GetClusterInChain(PF_FFD* p_ffd, u32 chain_index, u32 mode, u32 num_cluster, u32* locate_start, u32* locate_end) {
    u32* p_chain_start;
    u32 trace_cnt;
    u32 search_index;
    u32 current_cluster;
    int result;
    int err;
    int v16;
    PF_CACHE_PAGE* p_page;
    u32 check_use;
    u32 chain_start;
    u32 append_cnt;
    u32 next_cluster;
    u32 eoc1;
    u32 bad;

    p_page = 0;
    bad = fat_special_values[p_ffd->p_vol->bpb.fat_type].bad;
    eoc1 = fat_special_values[p_ffd->p_vol->bpb.fat_type].eoc1;
    *locate_end = -1;
    *locate_start = -1;
    if (p_ffd->p_hint->chain_index && p_ffd->p_hint->chain_index <= chain_index) {
        chain_start = p_ffd->p_hint->cluster;
        p_chain_start = &chain_start;
        trace_cnt = chain_index - p_ffd->p_hint->chain_index;
        search_index = p_ffd->p_hint->chain_index + 1;
    } else {
        p_chain_start = p_ffd->p_start_cluster;
        trace_cnt = chain_index;
        search_index = 1;
    }
    if (p_ffd->last_access.chain_index && p_ffd->last_access.chain_index <= chain_index && p_ffd->last_access.chain_index > search_index - 1) {
        chain_start = p_ffd->last_access.cluster;
        p_chain_start = &chain_start;
        trace_cnt = chain_index - p_ffd->last_access.chain_index;
        search_index = p_ffd->last_access.chain_index + 1;
    }
    current_cluster = *p_chain_start;
    next_cluster = *p_chain_start;
    if (mode == 2) {
        if (trace_cnt && num_cluster)
            trace_cnt = num_cluster + trace_cnt - 1;
        else
            trace_cnt = num_cluster;
        append_cnt = trace_cnt;
        if (p_ffd->last_cluster.num_last_cluster) {
            trace_cnt = 0;
            append_cnt = p_ffd->last_cluster.max_chain_index + num_cluster - chain_index;
            current_cluster = p_ffd->last_cluster.num_last_cluster;
            next_cluster = fat_special_values[p_ffd->p_vol->bpb.fat_type].eoc2;
            search_index = p_ffd->last_cluster.max_chain_index + 1;
        }
    } else {
        if (mode == 1 && !current_cluster)
            ++trace_cnt;
        append_cnt = trace_cnt;
    }
    if (trace_cnt) {
        result = VFiPFFAT_ReadFATSector(p_ffd->p_vol, &p_page, current_cluster);
        if (result)
            return result;
        p_page->option = 0;
        check_use = 1;
        while (1) {
            v16 = trace_cnt-- == 0;
            if (v16 || current_cluster >= bad)
                break;
            if (current_cluster) {
                err = VFiPFFAT_ReadClusterPage(p_ffd, current_cluster, search_index, &next_cluster, &check_use, &p_page);
                if (err && next_cluster != -1)
                    return err;
            } else {
                next_cluster = fat_special_values[p_ffd->p_vol->bpb.fat_type].eoc2;
                --search_index;
            }
            if ((next_cluster < 2 || next_cluster >= p_ffd->p_vol->bpb.num_clusters + 2) && next_cluster < eoc1)
                return 14;
            if (mode && next_cluster >= eoc1)
                break;
            current_cluster = next_cluster;
            ++search_index;
        }
        append_cnt = trace_cnt;
    }
    if (mode && next_cluster >= eoc1) {
        *locate_start = 0;
        result = VFiPFFAT_AllocateChain(p_ffd, ++append_cnt, search_index, locate_start, &next_cluster);
        if (result)
            return result;
        if (next_cluster == -1)
            return 6;
        if (*p_chain_start) {
            result = VFiPFFAT_WriteCluster(p_ffd, current_cluster, search_index - 1, *locate_start, 1u);
            if (result)
                return result;
        } else {
            *p_chain_start = *locate_start;
        }
        current_cluster = next_cluster;
    }
    *locate_end = current_cluster;
    return 0;
}

s32 VFiPFFAT_GetClusterContinuousSectorInChain(PF_FFD* p_ffd, u32 initial_cluster, u32 chain_index, u32 size, u32* p_num_sector) {
    PF_VOLUME* p_vol;
    u32 cluster;
    int err;
    u32 next_cluster;
    u32 check_use;
    PF_CACHE_PAGE* p_page;

    p_vol = p_ffd->p_vol;
    next_cluster = -1;
    cluster = initial_cluster;
    check_use = 1;
    err = VFiPFFAT_ReadFATSector(p_vol, &p_page, initial_cluster);
    if (err) {
        return err;
    }

    while (next_cluster) {
        err = VFiPFFAT_ReadClusterPage(p_ffd, cluster, chain_index, &next_cluster, &check_use, &p_page);
        if (err)
            return err;
        if (cluster + 1 != next_cluster) {
            if (next_cluster == fat_special_values[p_vol->bpb.fat_type].eoc2) {
                p_ffd->last_cluster.num_last_cluster = cluster;
                p_ffd->last_cluster.max_chain_index = chain_index - 1;
            }
            break;
        }
        ++p_ffd->p_hint->chain_index;
        p_ffd->p_hint->cluster = next_cluster;
        *p_num_sector += p_vol->bpb.sectors_per_cluster;
        if (*p_num_sector << p_vol->bpb.log2_bytes_per_sector >= size)
            break;
        cluster = next_cluster;
        ++chain_index;
    }
    return 0;
}

s32 VFiPFFAT_GetClusterAllocatedInChain(PF_FFD* p_ffd, u32 initial_cluster, u32 chain_index, u32 size, u32* p_num_clusters) {
    PF_VOLUME* p_vol;
    u32 cluster;
    u32 v10;
    int err;
    u32 next_cluster;
    u32 check_use;

    PF_CACHE_PAGE* p_page;

    p_vol = p_ffd->p_vol;
    next_cluster = -1;
    cluster = initial_cluster;
    check_use = 1;
    v10 = 0;
    err = VFiPFFAT_ReadFATSector(p_vol, &p_page, initial_cluster);
    if (err) {
        return err;
    }

    while (next_cluster) {
        v10 += p_vol->bpb.bytes_per_sector << p_vol->bpb.log2_sectors_per_cluster;
        ++*p_num_clusters;
        if (v10 >= size)
            break;
        err = VFiPFFAT_ReadClusterPage(p_ffd, cluster, chain_index, &next_cluster, &check_use, &p_page);
        if (err)
            return err;
        if (next_cluster == fat_special_values[p_vol->bpb.fat_type].eoc2) {
            p_ffd->last_cluster.num_last_cluster = cluster;
            p_ffd->last_cluster.max_chain_index = chain_index - 1;
            break;
        }
        cluster = next_cluster;
        ++chain_index;
    }

    return 0;
}

s32 VFiPFFAT_GetClusterSpecified(PF_FFD* p_ffd, u32 chain_index, u32 may_allocate, u32* p_cluster) {
    PF_VOLUME* p_vol;
    PF_FAT_HINT* p_hint;
    s32 err;
    u32 is_found;

    p_vol = p_ffd->p_vol;
    p_hint = p_ffd->p_hint;

    if (!*p_ffd->p_start_cluster && !may_allocate) {
        *p_cluster = -1;
        return 0;
    }

    if (*p_ffd->p_start_cluster == 1) {
        if (p_vol->bpb.fat_type <= FAT_16) {
            *p_cluster = -1;
            return 0;
        }
        *p_ffd->p_start_cluster = p_vol->bpb.root_dir_cluster;
    }

    if (p_ffd->cluster_link.buffer && !may_allocate) {
        err = VFiPFFAT_FindClusterLink(p_ffd, chain_index, p_cluster, &is_found);
        if (err)
            return err;

        if (is_found) {
            p_hint->chain_index = chain_index;
            p_hint->cluster = *p_cluster;
            return 0;
        }
    }

    err = VFiPFFAT_GetNumberOfCluster(p_ffd, chain_index, may_allocate, p_cluster);
    if (!err && *p_cluster != -1) {
        p_hint->chain_index = chain_index;
        p_hint->cluster = *p_cluster;
    }

    return err;
}

s32 VFiPFFAT_GetNumberOfCluster(PF_FFD* p_ffd, u32 chain_index, u32 may_allocate, u32* p_cluster) {
    int err;
    u32 locate_start;
    u32 locate_end;

    *p_cluster = -1;
    err = VFiPFFAT_GetClusterInChain(p_ffd, chain_index, may_allocate != 0, 0, &locate_start, &locate_end);
    if (err) {
        return err;
    }

    if (locate_end >= 2 && locate_end < p_ffd->p_vol->bpb.num_clusters + 2) {
        *p_cluster = locate_end;
    }

    return 0;
}

static s32 VFiPFFAT_GetNumberOfAllocatedCluster(PF_FFD* p_ffd, u32 chain_index, u32 num_cluster, u32* p_cluster, u32* p_num_cluster) {
    int err;
    u32 locate_start;
    u32 locate_end;

    *p_cluster = -1;
    err = VFiPFFAT_GetClusterInChain(p_ffd, chain_index, 2u, num_cluster, &locate_start, &locate_end);
    if (err) {
        return err;
    }

    *p_num_cluster = locate_end - locate_start + 1;
    if (locate_start >= 2 && locate_start < p_ffd->p_vol->bpb.num_clusters + 2) {
        *p_cluster = locate_start;
    }

    return 0;
}

static s32 VFiPFFAT_GetClusterAllocated(PF_FFD* p_ffd, u32 chain_index, u32 num_cluster, u32* p_cluster, u32* p_num_cluster) {
    PF_VOLUME* p_vol;
    PF_FAT_HINT* p_hint;
    int err;

    p_vol = p_ffd->p_vol;
    p_hint = p_ffd->p_hint;
    if (*p_ffd->p_start_cluster == 1) {
        if (p_vol->bpb.fat_type <= FAT_16) {
            *p_cluster = -1;
            return 0;
        }
        *p_ffd->p_start_cluster = p_vol->bpb.root_dir_cluster;
    }
    err = VFiPFFAT_GetNumberOfAllocatedCluster(p_ffd, chain_index, num_cluster, p_cluster, p_num_cluster);
    if (err) {
        return err;
    }

    if (*p_cluster != -1) {
        p_hint->chain_index = chain_index;
        p_hint->cluster = *p_cluster;
    }
    return 0;
}

static s32 VFiPFFAT_GetSectorInRootDirRegion(PF_VOLUME* p_vol, u32 sector_offset, u32* p_sector) {
    if (p_vol->bpb.fat_type >= FAT_32)
        return 12;
    if (sector_offset < p_vol->bpb.num_root_dir_sectors)
        *p_sector = p_vol->bpb.first_root_dir_sector + sector_offset;
    else
        *p_sector = -1;
    return 0;
}

s32 VFiPFFAT_GetSector(PF_FFD* p_ffd, u32 file_sector_index, u32 mode, u32 size, u32* p_sector, u32* p_num_sector) {
    PF_VOLUME* p_vol;
    int err;
    int v11;
    u32 v12;
    u32 v13;
    int v14;
    u32 num_divide;
    u32 cluster;
    u32 chain_index;
    u32 fat_type;

    v12 = 0;
    p_vol = p_ffd->p_vol;
    fat_type = p_vol->bpb.fat_type;
    if (*p_ffd->p_start_cluster == 1 && fat_type <= 1) {
        err = VFiPFFAT_GetSectorInRootDirRegion(p_vol, file_sector_index, p_sector);
        if (!err)
            return 0;
    } else {
        chain_index = file_sector_index >> p_vol->bpb.log2_sectors_per_cluster;
        if (mode == 2) {
            num_divide = size >> p_vol->bpb.log2_bytes_per_sector;
            v11 = (size & (p_vol->bpb.bytes_per_sector - 1)) != 0;
            v14 = v11;
            v13 = (num_divide + v11) >> p_vol->bpb.log2_sectors_per_cluster;
            v13 += ((p_vol->bpb.sectors_per_cluster - 1) & (num_divide + v11)) != 0;
            err = VFiPFFAT_GetClusterAllocated(p_ffd, chain_index, v13, &cluster, &v12);
            if (err)
                return err;
            *p_num_sector = v12 << p_vol->bpb.log2_sectors_per_cluster;
        } else {
            err = VFiPFFAT_GetClusterSpecified(p_ffd, chain_index, mode, &cluster);
            if (err)
                return err;
            *p_num_sector = 1 << p_vol->bpb.log2_sectors_per_cluster;
        }
        if (cluster == -1)
            *p_sector = -1;
        else
            *p_sector = (file_sector_index & (p_vol->bpb.sectors_per_cluster - 1)) + p_vol->bpb.first_data_sector +
                        ((cluster - 2) << p_vol->bpb.log2_sectors_per_cluster);
        return 0;
    }
    return err;
}

s32 VFiPFFAT_UpdateFATEntry(PF_VOLUME* p_vol, PF_CACHE_PAGE* p_page) {
    int err;

    err = 0;
    if ((p_page->stat & 2) == 2) {
        err = VFiPFCACHE_WriteFATPage(p_vol, p_page);
    }

    return err;
}

s32 VFiPFFAT_UpdateAlternateFATEntry(PF_VOLUME* p_vol, u8* p_buf, u32 sector, u32 size) {
    u32 offset;
    int err;
    int err2;
    u32 num_success;
    u32 fat_num;
    u32 fat_no;

    err = 0;
    offset = sector - p_vol->bpb.num_reserved_sectors;
    if (offset / p_vol->bpb.sectors_per_FAT) {
        fat_no = offset % p_vol->bpb.sectors_per_FAT;
        fat_num = 0;
    } else {
        fat_no = sector + p_vol->bpb.sectors_per_FAT;
        fat_num = 1;
    }
    while (fat_num < p_vol->bpb.num_active_FATs) {
        err2 = VFiPFDRV_lwrite(p_vol, p_buf, fat_no, size, &num_success);
        if (err2 && !err) {
            err = err2;
        }
        if (num_success != size) {
            err = 17;
        }
        ++fat_num;
        fat_no += p_vol->bpb.sectors_per_FAT;
    }
    return err;
}

s32 VFiPFFAT_GetSectorSpecified(PF_FFD* p_ffd, u32 file_sector_index, u32 may_allocate, u32* p_sector) {
    int err;
    u32 num_sector;

    if (!p_ffd)
        return 10;
    if (!p_ffd->p_hint)
        return 10;
    if (p_ffd->p_vol->drv_char < 65 || p_ffd->p_vol->drv_char > 90)
        return 35;
    if (file_sector_index >= (0xFFFFFFFF >> p_ffd->p_vol->bpb.log2_bytes_per_sector) + 1)
        return 37;

    err = VFiPFFAT_GetSector(p_ffd, file_sector_index, may_allocate != 0, 0, p_sector, &num_sector);
    if (err) {
        return err;
    }

    return 0;
}

s32 VFiPFFAT_GetSectorAllocated(PF_FFD* p_ffd, u32 file_sector_index, u32 size, u32* p_sector, u32* p_num_sector) {
    int err;

    if (!p_ffd)
        return 10;
    if (!p_ffd->p_hint)
        return 10;
    if (p_ffd->p_vol->drv_char < 65 || p_ffd->p_vol->drv_char > 90)
        return 35;
    if (file_sector_index >= (0xFFFFFFFF >> p_ffd->p_vol->bpb.log2_bytes_per_sector) + 1)
        return 37;
    err = VFiPFFAT_GetSector(p_ffd, file_sector_index, 2u, size, p_sector, p_num_sector);
    if (!err)
        return 0;
    return err;
}

s32 VFiPFFAT_GetContinuousSector(PF_FFD* p_ffd, u32 file_sector_index, u32 size, u32* p_sector, u32* p_num_sector) {
    u32 cluster;
    s32 err;
    PF_VOLUME* p_vol;

    cluster = -1;
    if (!p_ffd)
        return 10;
    if (!p_ffd->p_hint)
        return 10;
    if (p_ffd->p_vol->drv_char < 65 || p_ffd->p_vol->drv_char > 90)
        return 35;
    if (file_sector_index >= (0xFFFFFFFF >> p_ffd->p_vol->bpb.log2_bytes_per_sector) + 1)
        return 37;
    p_vol = p_ffd->p_vol;
    if (*p_sector == -1) {
        err = VFiPFFAT_GetSectorSpecified(p_ffd, file_sector_index, 0, p_sector);
        if (err)
            goto error;
        if (*p_sector != -1)
            cluster = ((*p_sector - p_vol->bpb.first_data_sector) >> p_vol->bpb.log2_sectors_per_cluster) + 2;
    } else {
        cluster = ((*p_sector - p_vol->bpb.first_data_sector) >> p_vol->bpb.log2_sectors_per_cluster) + 2;
    }
    if (*p_sector == -1)
        *p_num_sector = 0;
    else
        *p_num_sector = p_vol->bpb.first_data_sector + ((cluster - 1) << p_vol->bpb.log2_sectors_per_cluster) - *p_sector;
    if (*p_sector == -1)
        return 0;
    if (*p_num_sector << p_vol->bpb.log2_bytes_per_sector >= size)
        return 0;
    err = VFiPFFAT_GetClusterContinuousSectorInChain(p_ffd, cluster, (file_sector_index >> p_vol->bpb.log2_sectors_per_cluster) + 1, size,
                                                     p_num_sector);
    if (!err)
        return 0;

error:
    return err;
}

s32 VFiPFFAT_CountAllocatedClusters(PF_FFD* p_ffd, u32 size, u32* p_num_alloc_clusters) {
    u32 total_size;
    int err;
    u32 cluster;
    u32 chain_index;

    total_size = size;
    *p_num_alloc_clusters = 0;
    if (!p_ffd)
        return 10;
    if (!p_ffd->p_hint)
        return 10;
    if (p_ffd->p_vol->drv_char < 65 || p_ffd->p_vol->drv_char > 90)
        return 35;
    if (*p_ffd->p_start_cluster < 2)
        return 0;
    if (p_ffd->last_cluster.num_last_cluster) {
        *p_num_alloc_clusters = p_ffd->last_cluster.max_chain_index + 1;
        return 0;
    }
    if (p_ffd->p_hint->cluster) {
        if (size <= (p_ffd->p_hint->chain_index + 1) << (p_ffd->p_vol->bpb.log2_bytes_per_sector + p_ffd->p_vol->bpb.log2_sectors_per_cluster)) {
            *p_num_alloc_clusters = p_ffd->p_hint->chain_index + 1;
            return 0;
        }
        *p_num_alloc_clusters = p_ffd->p_hint->chain_index;
        cluster = p_ffd->p_hint->cluster;
        chain_index = p_ffd->p_hint->chain_index + 1;
        total_size = size - (p_ffd->p_hint->chain_index << (p_ffd->p_vol->bpb.log2_bytes_per_sector + p_ffd->p_vol->bpb.log2_sectors_per_cluster));
    } else {
        cluster = *p_ffd->p_start_cluster;
        chain_index = 1;
    }
    err = VFiPFFAT_GetClusterAllocatedInChain(p_ffd, cluster, chain_index, total_size, p_num_alloc_clusters);
    if (err) {
        return err;
    }
    return 0;
}

int VFiPFFAT_CountFreeClusters(PF_VOLUME* p_vol, u32* p_num_free_clusters) {
    int err;
    u32 free_cluster;
    PF_CACHE_PAGE* p_page;
    u32 next_cluster;

    if (!p_vol)
        return 10;
    if (p_vol->bpb.fat_type > FAT_32)
        return 15;
    if ((p_vol->fsi_flag & 4) != 0 && p_vol->num_free_clusters != -1) {
        *p_num_free_clusters = p_vol->num_free_clusters;
        return 0;
    }
    *p_num_free_clusters = -1;
    free_cluster = 2;
    if ((p_vol->fsi_flag & 1) != 0 && p_vol->bpb.fat_type == FAT_32) {
        err = VFiPFDRV_GetFSINFOInformation(p_vol);
        if (err)
            return err;
        *p_num_free_clusters = p_vol->num_free_clusters;
    }
    if (*p_num_free_clusters == -1) {
        err = VFiPFFAT_ReadFATSector(p_vol, &p_page, 2u);
        if (err)
            return err;
        p_page->option = 0;
        *p_num_free_clusters = 0;
        while (free_cluster >= 2 && free_cluster < p_vol->bpb.num_clusters + 2) {
            err = VFiPFFAT_ReadFATEntryPage(p_vol, free_cluster, &next_cluster, &p_page);
            if (err)
                return err;
            if (!next_cluster)
                ++*p_num_free_clusters;
            ++free_cluster;
        }
        p_vol->num_free_clusters = *p_num_free_clusters;
        if (p_vol->bpb.fat_type == FAT_32 && (p_vol->fsi_flag & 2) != 0) {
            if (VFiPFFAT_RefreshFSINFO(p_vol)) {
                p_vol->num_free_clusters = -1;
                p_vol->fsi_flag &= ~4u;
            }
        }
    }
    p_vol->fsi_flag |= 4u;
    return 0;
}

s32 VFiPFFAT_RefreshFSINFO(PF_VOLUME* p_vol) {
    int err;

    err = 0;
    if (p_vol->bpb.fat_type != FAT_32)
        return 12;
    if ((p_vol->fsi_flag & 2) == 0)
        return 36;
    if ((p_vol->fsi_flag & 4) != 0)
        return VFiPFDRV_StoreFreeCountToFSINFO(p_vol);
    return err;
}

s32 VFiPFFAT_FreeChain(PF_FFD* p_ffd, u32 start_cluster, u32 chain_index, u32 size) {
    u32 v5;
    PF_VOLUME* p_vol;
    int err;
    int v10;
    int save_num_clusters;
    u32 save_start_cluster;
    u32 sector;
    PF_CACHE_PAGE* p_page;
    u32 clst_size;
    u32 file_size;
    u32 next_cluster;
    u32 eoc1;

    v5 = start_cluster;
    p_vol = p_ffd->p_vol;
    if (!p_vol)
        return 10;
    if ((start_cluster < 2 || start_cluster >= p_vol->bpb.num_clusters + 2) && start_cluster)
        return 14;
    if (!start_cluster)
        return 0;
    eoc1 = fat_special_values[p_vol->bpb.fat_type].eoc1;
    if (!size)
        return 0;
    file_size = size;
    clst_size = p_vol->bpb.bytes_per_sector << p_vol->bpb.log2_sectors_per_cluster;
    err = VFiPFFAT_ReadFATSector(p_vol, &p_page, start_cluster);
    if (!err) {
        p_page->option = 1;
        next_cluster = v5;
        save_start_cluster = v5;
        save_num_clusters = 1;
        while (v5 < eoc1 && (!size || file_size)) {
            sector = p_page->sector;
            err = VFiPFFAT_ReadFATEntryPage(p_vol, v5, &next_cluster, &p_page);
            if (err)
                return err;
            if (!next_cluster) {
                err = VFiPFFAT_UpdateFATEntry(p_vol, p_page);
                if (!err)
                    return 0;
                return err;
            }
            if (sector != p_page->sector) {
                err = VFiPFFAT_ReadFATSector(p_vol, &p_page, v5);
                if (err)
                    return err;
            }
            if (chain_index == -1)
                v10 = VFiPFFAT_WriteFATEntryPage(p_vol, v5, 0, &p_page);
            else
                v10 = VFiPFFAT_WriteClusterPage(p_ffd, v5, chain_index++, 0, 1u, &p_page);
            if (v10)
                return v10;
            if ((p_vol->flags & 0x40) != 0) {
                if (next_cluster == v5 + 1) {
                    ++save_num_clusters;
                } else {
                    err = VFiPFDRV_lerase(p_vol, p_vol->bpb.first_data_sector + ((save_start_cluster - 2) << p_vol->bpb.log2_sectors_per_cluster),
                                          save_num_clusters << p_vol->bpb.log2_sectors_per_cluster);
                    if (err)
                        return err;
                    save_start_cluster = next_cluster;
                    save_num_clusters = 1;
                }
            }
            if (size && file_size) {
                if (file_size > clst_size)
                    file_size -= clst_size;
                else
                    file_size = 0;
            }
            if (p_vol->num_free_clusters != -1)
                ++p_vol->num_free_clusters;
            v5 = next_cluster;
        }
        err = VFiPFFAT_UpdateFATEntry(p_vol, p_page);
        if (!err) {
            if (p_vol->bpb.fat_type == FAT_32 && (p_vol->fsi_flag & 2) != 0) {
                if (VFiPFFAT_RefreshFSINFO(p_vol)) {
                    p_vol->num_free_clusters = -1;
                    p_vol->fsi_flag &= ~4u;
                }
            }
            return 0;
        }
    }
    return err;
}

s32 VFiPFFAT_GetBeforeChain(struct PF_VOLUME* p_vol, u32 start_cluster, u32 lActive, u32* p_cluster) {
    u32 v5;
    int err;
    u32 eoc1;
    PF_CACHE_PAGE* p_page;
    u32 next_cluster;

    v5 = start_cluster;
    if (p_vol) {
        if (start_cluster >= 2 && start_cluster < p_vol->bpb.num_clusters + 2) {
            eoc1 = fat_special_values[p_vol->bpb.fat_type].eoc1;
            *p_cluster = -1;
            err = VFiPFFAT_ReadFATSector(p_vol, &p_page, start_cluster);
            if (!err) {
                p_page->option = 0;
                while (v5 < eoc1) {
                    err = VFiPFFAT_ReadFATEntryPage(p_vol, v5, &next_cluster, &p_page);
                    if (err)
                        return err;
                    if (lActive == next_cluster) {
                        *p_cluster = v5;
                        return 0;
                    }
                    if ((--v5 < 2 || v5 >= p_vol->bpb.num_clusters + 2) && v5 < eoc1) {
                        *p_cluster = lActive;
                        return 0;
                    }
                }
                return 13;
            }
        } else {
            *p_cluster = -1;
            return 10;
        }
    } else {
        *p_cluster = -1;
        return 10;
    }
    return err;
}

s32 VFiPFFAT_GetBeforeSector(u32* p_befor_sector, PF_VOLUME* p_vol, u32 current_sector) {
    u32 v5;
    int err;
    u32 next_cluster;

    v5 = current_sector;
    err = 0;
    if (!p_befor_sector || !p_vol)
        return 10;
    if (current_sector < p_vol->bpb.first_root_dir_sector)
        return 10;
    if (p_vol->bpb.fat_type == FAT_32 || current_sector >= p_vol->bpb.first_data_sector) {
        if (((p_vol->bpb.sectors_per_cluster - 1) & (current_sector - p_vol->bpb.first_data_sector)) != 0) {
            v5 = current_sector - 1;
        } else {
            err =
                VFiPFFAT_GetBeforeChain(p_vol, ((current_sector - p_vol->bpb.first_data_sector) >> p_vol->bpb.log2_sectors_per_cluster) + 2,
                                        ((current_sector - p_vol->bpb.first_data_sector) >> p_vol->bpb.log2_sectors_per_cluster) + 2, &next_cluster);
            if (!err) {
                if (next_cluster == -1)
                    v5 = -1;
                else
                    v5 = ((next_cluster - 2) << p_vol->bpb.log2_sectors_per_cluster) + p_vol->bpb.sectors_per_cluster + p_vol->bpb.first_data_sector -
                         1;
            }
        }
    } else {
        v5 = current_sector - 1;
        if (current_sector - 1 < p_vol->bpb.first_root_dir_sector)
            err = 34;
    }
    if (err)
        *p_befor_sector = -1;
    else
        *p_befor_sector = v5;
    return err;
}

s32 VFiPFFAT_InitFATRegion(PF_VOLUME* p_vol) {
    int err;
    u32 num_cache_buff;
    u32 size;
    u32 root_dir_sector;
    u32 sector;
    u32 success_size;
    PF_CACHE_PAGE* p_page;

    if (!p_vol)
        return 10;
    if (p_vol->bpb.fat_type > FAT_32)
        return 15;
    err = VFiPFCACHE_AllocateDataPage(p_vol, 0xFFFFFFFF, &p_page);
    if (!err) {
        num_cache_buff = p_vol->cache.data_buff_size;
        size = num_cache_buff << p_vol->bpb.log2_bytes_per_sector;
        root_dir_sector = p_vol->bpb.first_root_dir_sector;
        VFipf_memset(p_page->buffer, 0, size);
        for (sector = p_vol->bpb.num_reserved_sectors; sector < root_dir_sector; sector += num_cache_buff) {
            if (sector + num_cache_buff > root_dir_sector) {
                num_cache_buff = root_dir_sector - sector;
                size = (root_dir_sector - sector) << p_vol->bpb.log2_bytes_per_sector;
            }
            if (VFiPFSEC_WriteDataSector(p_vol, p_page->buffer, sector, size, &success_size, 1u, 0)) {
                sector = root_dir_sector;
            } else if (success_size != size) {
                sector = root_dir_sector;
            }
        }
        VFiPFCACHE_FreeDataPage(p_vol, p_page);
        err = VFiPFFAT_WriteFATEntry(p_vol, 0, p_vol->bpb.media | fat_special_values[p_vol->bpb.fat_type].fat0_mask);
        if (!err) {
            err = VFiPFFAT_WriteFATEntry(p_vol, 1u, fat_special_values[p_vol->bpb.fat_type].fat0_mask);
            if (!err) {
                err = VFiPFCACHE_FlushFATCache(p_vol);
                if (!err)
                    return 0;
            }
        }
    }
    return err;
}

s32 VFiPFFAT_MakeRootDir(PF_VOLUME* p_vol) {
    int err;

    err = VFiPFFAT_WriteFATEntry(p_vol, p_vol->bpb.root_dir_cluster, fat_special_values[p_vol->bpb.fat_type].eoc2);
    if (!err) {
        err = VFiPFCACHE_FlushFATCache(p_vol);
        if (!err)
            return 0;
    }
    return err;
}

s32 VFiPFFAT_TraceClustersChain(PF_FFD* p_ffd, u32 start_clst, u32 size, u32* p_target_clst, u32* p_next_clst) {
    u32 v6;
    int err;
    PF_VOLUME* p_vol;
    u32 clst_cnt;
    u32 chain_index;
    u32 cnt;
    PF_CACHE_PAGE* p_page;
    u32 check_use;
    u32 clst_size;
    u32 save_cluster;
    u32 next_cluster;

    v6 = start_clst;
    save_cluster = start_clst;
    *p_target_clst = 0;
    *p_next_clst = 0;
    if (!p_ffd)
        return 10;
    if (!p_ffd->p_hint)
        return 10;
    if (p_ffd->p_vol->drv_char < 65 || p_ffd->p_vol->drv_char > 90)
        return 35;
    if (!p_ffd->p_vol)
        return 10;
    if (*p_ffd->p_start_cluster < 2)
        return 0;
    if (start_clst < 2)
        return 0;
    if (!size)
        return 0;
    p_vol = p_ffd->p_vol;
    if (size == -1 && p_ffd->last_cluster.num_last_cluster) {
        *p_target_clst = p_ffd->last_cluster.num_last_cluster;
        *p_next_clst = fat_special_values[p_vol->bpb.fat_type].eoc2;
        return 0;
    } else {
        clst_size = p_vol->bpb.bytes_per_sector << p_vol->bpb.log2_sectors_per_cluster;
        clst_cnt = size / clst_size;
        if (size % clst_size)
            ++clst_cnt;
        if (*p_ffd->p_start_cluster == start_clst)
            chain_index = 1;
        else
            chain_index = p_ffd->p_hint->chain_index + 1;
        next_cluster = -1;
        check_use = 1;
        err = VFiPFFAT_ReadFATSector(p_vol, &p_page, start_clst);
        if (!err) {
            for (cnt = 0; cnt < clst_cnt; ++cnt) {
                err = VFiPFFAT_ReadClusterPage(p_ffd, v6, chain_index, &next_cluster, &check_use, &p_page);
                if (err)
                    return err;
                save_cluster = v6;
                if (next_cluster == fat_special_values[p_vol->bpb.fat_type].eoc2) {
                    p_ffd->last_cluster.num_last_cluster = v6;
                    p_ffd->last_cluster.max_chain_index = chain_index - 1;
                    break;
                }
                v6 = next_cluster;
                ++chain_index;
            }
            p_ffd->p_hint->chain_index = chain_index - 1;
            *p_target_clst = save_cluster;
            *p_next_clst = next_cluster;
            return 0;
        }
    }
    return err;
}

s32 VFiPFFAT_ReadValueToSpecifiedCluster(PF_VOLUME* p_vol, u32 cluster, u32* value) {
    int err;

    err = VFiPFFAT_ReadFATEntry(p_vol, cluster, value);
    if (err)
        return err;

    return 0;
}

void VFiPFFAT_InitHint(PF_FAT_HINT* p_hint) {
    p_hint->chain_index = 0;
    p_hint->cluster = 0;
}

void VFiPFFAT_SetHint(PF_FFD* p_ffd, PF_FAT_HINT* p_hint) {
    p_ffd->p_hint = p_hint;
}

s32 VFiPFFAT_ResetFFD(PF_FFD* p_ffd, u32* p_start_cluster) {
    if (!p_ffd)
        return 10;
    if (!p_start_cluster)
        return 10;
    p_ffd->p_start_cluster = p_start_cluster;
    if (p_ffd->p_hint)
        VFiPFFAT_InitHint(p_ffd->p_hint);
    p_ffd->last_access.cluster = 0;
    p_ffd->last_access.chain_index = 0;
    p_ffd->last_cluster.num_last_cluster = 0;
    p_ffd->last_cluster.max_chain_index = 0;
    p_ffd->cluster_link.buffer = 0;
    return 0;
}

s32 VFiPFFAT_InitFFD(PF_FFD* p_ffd, PF_FAT_HINT* p_hint, PF_VOLUME* p_vol, u32* p_start_cluster) {
    if (!p_ffd)
        return 10;
    if (!p_start_cluster)
        return 10;
    p_ffd->p_hint = p_hint;
    p_ffd->p_vol = p_vol;
    VFiPFFAT_ResetFFD(p_ffd, p_start_cluster);
    return 0;
}

s32 VFiPFFAT_FinalizeFFD(PF_FFD* p_ffd) {
    if (!p_ffd)
        return 10;
    p_ffd->cluster_link.buffer = 0;
    return 0;
}

void VFiPFFAT_SetLastAccess(PF_FFD* p_ffd, PF_FAT_HINT* last_access) {
    p_ffd->last_access = *last_access;
}

u32 VFiPFFAT_GetValueOfEOC2(PF_VOLUME* p_vol) {
    return fat_special_values[p_vol->bpb.fat_type].eoc2;
}
