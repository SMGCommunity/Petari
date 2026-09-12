#include "revolution/vf/pf_fat.h"
#include "revolution/types.h"
#include "revolution/vf/pf_cache.h"
#include "revolution/vf/pf_clib.h"
#include "revolution/vf/pf_driver.h"
#include "revolution/vf/pf_fat12.h"
#include "revolution/vf/pf_fat16.h"
#include "revolution/vf/pf_fat32.h"
#include "revolution/vf/pf_sector.h"

static const struct {
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
    switch (p_vol->bpb.fat_type) {
    case FAT_12:
        return VFiPFFAT12_ReadFATEntry(p_vol, cluster, p_value);
    case FAT_16:
        return VFiPFFAT16_ReadFATEntry(p_vol, cluster, p_value);
    case FAT_32:
        return VFiPFFAT32_ReadFATEntry(p_vol, cluster, p_value);
    default:
        *p_value = -1;
        return 15;
    }
}

static s32 VFiPFFAT_ReadFATEntryPage(PF_VOLUME* p_vol, u32 cluster, u32* p_value, PF_CACHE_PAGE** p_page) {
    s32 err;
    switch (p_vol->bpb.fat_type) {
    case FAT_12:
        err = VFiPFFAT12_ReadFATEntryPage(p_vol, cluster, p_value, p_page);
        break;
    case FAT_16:
        err = VFiPFFAT16_ReadFATEntryPage(p_vol, cluster, p_value, p_page);
        break;
    case FAT_32:
        err = VFiPFFAT32_ReadFATEntryPage(p_vol, cluster, p_value, p_page);
        break;
    default:
        return 15;
    }
    return err;
}

s32 VFiPFFAT_ReadFATSector(PF_VOLUME* p_vol, PF_CACHE_PAGE** pp_page, u32 cluster) {
    s32 err;
    u32 offset;
    u32 current_fat;
    s32 result;
    u32 sector;

    switch (p_vol->bpb.fat_type) {
    case FAT_12:
        offset = (u16)(cluster + (cluster >> 1));
        break;
    case FAT_16:
        offset = cluster << 1;
        break;
    case FAT_32:
        offset = cluster << 2;
        break;
    default:
        return 15;
    }
    sector = (u16)(p_vol->bpb.active_FAT_sector + (offset >> p_vol->bpb.log2_bytes_per_sector));
    current_fat = (p_vol->bpb.ext_flags & 0x80) ? (p_vol->bpb.ext_flags & 7) : 1;
    do {
        err = VFiPFCACHE_ReadFATPage(p_vol, sector, pp_page);
        if (err == 4096 && p_vol->p_callback) {
            result = p_vol->p_callback(p_vol->last_driver_error);
            if (result == 0)
                continue;
            if (result == 1 && p_vol->bpb.num_active_FATs >= 2u && current_fat < p_vol->bpb.num_active_FATs) {
                ++current_fat;
                sector += p_vol->bpb.sectors_per_FAT;
                continue;
            }
        }
        if (err)
            return err;
    } while (err);
    return err;
}

s32 VFiPFFAT_SearchForNumFreeClusters(PF_VOLUME* p_vol, u32 start_cluster, u32 end_cluster, u32 num_cluster, u32* p_start_free_cluster,
                                      u32* p_last_free_cluster) {
    u32 upper_bound_cluster;
    u32 fat_entry;
    u32 success_num;
    u32 save_start_cluster;
    u32 save_success_num;
    s32 result;
    u32 search_flg;
    u32 temp_start_cluster;
    PF_CACHE_PAGE* p_page;

    search_flg = 0;
    temp_start_cluster = 0;
    *p_start_free_cluster = -1;
    *p_last_free_cluster = -1;
    success_num = 0;
    save_start_cluster = -1;
    save_success_num = 0;
    upper_bound_cluster = p_vol->bpb.num_clusters + 2;
    if (start_cluster < 2 || start_cluster >= p_vol->bpb.num_clusters + 2)
        start_cluster = 2;
    if (end_cluster < 2 || end_cluster >= p_vol->bpb.num_clusters + 2)
        end_cluster = upper_bound_cluster - 1;
    temp_start_cluster = start_cluster;
    result = VFiPFFAT_ReadFATSector(p_vol, &p_page, start_cluster);
    if (result)
        return result;
    {
        p_page->option = 0;
        while (start_cluster <= end_cluster) {
            if (upper_bound_cluster <= start_cluster)
                start_cluster = 2;
            result = VFiPFFAT_ReadFATEntryPage(p_vol, start_cluster, &fat_entry, &p_page);
            if (result)
                return result;
            if (fat_entry == 0) {
                if (*p_start_free_cluster == -1)
                    *p_start_free_cluster = start_cluster;
                if (++success_num >= num_cluster) {
                    *p_last_free_cluster = start_cluster;
                    return 0;
                }
            } else {
                if (save_success_num < success_num) {
                    save_start_cluster = *p_start_free_cluster;
                    save_success_num = success_num;
                }
                *p_start_free_cluster = -1;
                success_num = 0;
            }
            ++start_cluster;
            if (!search_flg && !success_num && start_cluster > 2 && start_cluster == upper_bound_cluster) {
                end_cluster = temp_start_cluster;
                start_cluster = 2;
                search_flg = 1;
            }
        }
        if (save_success_num < success_num) {
            save_start_cluster = *p_start_free_cluster;
            save_success_num = success_num;
        }
        if (save_success_num) {
            *p_start_free_cluster = save_start_cluster;
            *p_last_free_cluster = *p_start_free_cluster + save_success_num - 1;
        }
        return 0;
    }
}

s32 VFiPFFAT_UpdateClusterLink(PF_FFD* p_ffd, u32 cluster, u32 chain_index) {
    if (p_ffd->cluster_link.max_count > p_ffd->cluster_link.position && p_ffd->cluster_link.max_count) {
        if (chain_index == p_ffd->cluster_link.position * (p_ffd->cluster_link.interval + 1)) {
            p_ffd->cluster_link.interval_offset = 0;
            p_ffd->cluster_link.buffer[p_ffd->cluster_link.position] = cluster;
            ++p_ffd->cluster_link.position;
        } else {
            ++p_ffd->cluster_link.interval_offset;
        }
        p_ffd->cluster_link.save_index = chain_index;
    }
    return 0;
}

s32 VFiPFFAT_FindClusterLink(PF_FFD* p_ffd, u32 chain_index, u32* p_cluster, u32* is_found) {
    u32 current_cluster;
    u32 next_cluster;
    u32 position;
    u32 offset;
    u32 i;
    s32 result;
    PF_CACHE_PAGE* p_page;

    next_cluster = -1;
    *is_found = 0;
    if (!p_ffd->cluster_link.position)
        return 0;
    if (p_ffd->cluster_link.save_index >= chain_index) {
        position = chain_index / (p_ffd->cluster_link.interval + 1);
        offset = chain_index % (p_ffd->cluster_link.interval + 1);
        if (offset == 0) {
            *p_cluster = p_ffd->cluster_link.buffer[position];
            *is_found = 1;
        } else {
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
        }
    } else {
        return 0;
    }
    return 0;
}

s32 VFiPFFAT_FindClusterLinkPage(PF_FFD* p_ffd, u32 chain_index, u32* p_cluster, u32* is_found, PF_CACHE_PAGE* p_page) {
    u32 current_cluster;
    u32 next_cluster;
    u32 position;
    u32 offset;
    u32 i;
    s32 result;

    next_cluster = -1;
    *is_found = 0;
    if (!p_ffd->cluster_link.position)
        return 0;
    if (p_ffd->cluster_link.save_index >= chain_index) {
        position = chain_index / (p_ffd->cluster_link.interval + 1);
        offset = chain_index % (p_ffd->cluster_link.interval + 1);
        if (offset == 0) {
            *p_cluster = p_ffd->cluster_link.buffer[position];
            *is_found = 1;
        } else {
            current_cluster = p_ffd->cluster_link.buffer[position];
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
        }
    } else {
        return 0;
    }
    return 0;
}

static s32 VFiPFFAT_WriteFATEntry(PF_VOLUME* p_vol, u32 cluster, u32 value) {
    switch (p_vol->bpb.fat_type) {
    case FAT_12:
        if (value > 0xFFF)
            return 16;
        return VFiPFFAT12_WriteFATEntry(p_vol, cluster, value);
    case FAT_16:
        if (value > 0xFFFF)
            return 16;
        return VFiPFFAT16_WriteFATEntry(p_vol, cluster, value);
    case FAT_32:
        if (value > 0xFFFFFFF)
            return 16;
        return VFiPFFAT32_WriteFATEntry(p_vol, cluster, value);
    default:
        return 15;
    }
}

s32 VFiPFFAT_WriteFATEntryPage(PF_VOLUME* p_vol, u32 cluster, u32 value, PF_CACHE_PAGE** p_page) {
    switch (p_vol->bpb.fat_type) {
    case FAT_12:
        if (value > 0xFFF)
            return 16;
        return VFiPFFAT12_WriteFATEntryPage(p_vol, cluster, value, p_page);
    case FAT_16:
        if (value > 0xFFFF)
            return 16;
        return VFiPFFAT16_WriteFATEntryPage(p_vol, cluster, value, p_page);
    case FAT_32:
        if (value > 0xFFFFFFF)
            return 16;
        return VFiPFFAT32_WriteFATEntryPage(p_vol, cluster, value, p_page);
    default:
        return 15;
    }
}

s32 VFiPFFAT_ReadClusterPage(PF_FFD* p_ffd, u32 cluster, u32 chain_index, u32* next_cluster, u32* chk_clstlnk, PF_CACHE_PAGE** p_page) {
    s32 err;

    *next_cluster = -1;
    if (p_ffd->cluster_link.buffer && *chk_clstlnk) {
        err = VFiPFFAT_FindClusterLinkPage(p_ffd, chain_index, next_cluster, chk_clstlnk, *p_page);
        if (err)
            return err;
    }
    if (!p_ffd->cluster_link.buffer || !*chk_clstlnk) {
        err = VFiPFFAT_ReadFATEntryPage(p_ffd->p_vol, cluster, next_cluster, p_page);
        if (err)
            return err;
        if (!*next_cluster)
            return 13;
        if (*next_cluster == -1)
            return 6;
        if (p_ffd->cluster_link.buffer && *next_cluster != fat_special_values[p_ffd->p_vol->bpb.fat_type].eoc2) {
            err = VFiPFFAT_UpdateClusterLink(p_ffd, *next_cluster, chain_index);
            if (err)
                return err;
        }
    }
    return 0;
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
    s32 err;
    u32 access_cluster;

    err = VFiPFFAT_WriteFATEntry(p_ffd->p_vol, cluster, next_cluster);
    if (err)
        return err;
    if (p_ffd->cluster_link.buffer && use_clstlnk) {
        if (!next_cluster) {
            VFiPFFAT_ClearClusterLink(p_ffd, chain_index);
        } else if (cluster != fat_special_values[p_ffd->p_vol->bpb.fat_type].eoc2) {
            err = VFiPFFAT_FindClusterLink(p_ffd, chain_index, &access_cluster, &use_clstlnk);
            if (err)
                return err;
            if (!use_clstlnk) {
                err = VFiPFFAT_UpdateClusterLink(p_ffd, cluster, chain_index);
                if (err)
                    return err;
            }
        }
    }
    return 0;
}

s32 VFiPFFAT_WriteClusterPage(PF_FFD* p_ffd, u32 cluster, u32 chain_index, u32 next_cluster, u32 use_clstlnk, PF_CACHE_PAGE** p_page) {
    s32 err;
    u32 access_cluster;

    err = VFiPFFAT_WriteFATEntryPage(p_ffd->p_vol, cluster, next_cluster, p_page);
    if (err)
        return err;
    if (p_ffd->cluster_link.buffer && use_clstlnk) {
        if (!next_cluster) {
            VFiPFFAT_ClearClusterLink(p_ffd, chain_index);
        } else if (cluster != fat_special_values[p_ffd->p_vol->bpb.fat_type].eoc2) {
            err = VFiPFFAT_FindClusterLinkPage(p_ffd, chain_index, &access_cluster, &use_clstlnk, *p_page);
            if (err)
                return err;
            if (!use_clstlnk) {
                err = VFiPFFAT_UpdateClusterLink(p_ffd, cluster, chain_index);
                if (err)
                    return err;
            }
        }
    }
    return 0;
}

s32 VFiPFFAT_DoAllocateChain(PF_FFD* p_ffd, u32 chain_len, u32 chain_index, u32* p_chain_start, u32* p_last_allocated) {
    PF_VOLUME* p_vol;
    u32 eoc2;
    enum FatType fat_type;
    u32 first_allocated_cluster;
    u32 last_allocated_cluster;
    u32 start_cluster;
    s32 result;
    PF_CACHE_PAGE* p_page;

    p_vol = p_ffd->p_vol;
    fat_type = p_vol->bpb.fat_type;
    eoc2 = fat_special_values[fat_type].eoc2;
    *p_chain_start = -1;
    *p_last_allocated = -1;
    last_allocated_cluster = -1;
    result =
        VFiPFFAT_SearchForNumFreeClusters(p_vol, p_vol->last_free_cluster, 0xFFFFFFFF, chain_len, &first_allocated_cluster, &last_allocated_cluster);
    if (result)
        return result;
    {
        if (first_allocated_cluster == -1)
            return 0;
        result = VFiPFFAT_ReadFATSector(p_vol, &p_page, first_allocated_cluster);
        if (result)
            return result;
        {
            p_page->option = 0;
            for (start_cluster = first_allocated_cluster; start_cluster < last_allocated_cluster; ++start_cluster) {
                result = VFiPFFAT_WriteClusterPage(p_ffd, start_cluster, chain_index, start_cluster + 1, 1u, &p_page);
                if (result)
                    return result;
                if ((p_vol->fsi_flag & 4) != 0 && p_vol->num_free_clusters != -1 && p_vol->num_free_clusters &&
                    (p_vol->bpb.fat_type != FAT_32 || last_allocated_cluster != p_vol->bpb.root_dir_cluster)) {
                    --p_vol->num_free_clusters;
                }
                if (*p_chain_start == -1)
                    *p_chain_start = start_cluster;
                *p_last_allocated = start_cluster;
                ++chain_index;
            }
            if (last_allocated_cluster != -1) {
                result = VFiPFFAT_WriteClusterPage(p_ffd, last_allocated_cluster, chain_index, eoc2, 1u, &p_page);
                if (result)
                    return result;
                if (*p_chain_start == -1)
                    *p_chain_start = start_cluster;
                *p_last_allocated = start_cluster;
                if (p_vol->num_free_clusters != -1 && p_vol->num_free_clusters &&
                    (p_vol->bpb.fat_type != FAT_32 || last_allocated_cluster != p_vol->bpb.root_dir_cluster)) {
                    --p_vol->num_free_clusters;
                }
            }
            p_vol->last_free_cluster = last_allocated_cluster + 1;
            p_ffd->last_cluster.num_last_cluster = last_allocated_cluster;
            p_ffd->last_cluster.max_chain_index = chain_index;
            *p_last_allocated = last_allocated_cluster;
            result = VFiPFFAT_UpdateFATEntry(p_vol, p_page);
            if (result)
                return result;
            {
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
}

static inline s32 VFiPFFAT_AllocateChain(PF_FFD* p_ffd, u32 chain_len, u32 chain_index, u32* p_chain_start, u32* p_last_allocated) {
    int err;

    err = VFiPFFAT_DoAllocateChain(p_ffd, chain_len, chain_index, p_chain_start, p_last_allocated);
    if (err) {
        if (*p_chain_start != -1)
            VFiPFFAT_FreeChain(p_ffd, *p_chain_start, chain_index, -1);
        *p_chain_start = -1;
        *p_last_allocated = -1;
        return err;
    }
    return 0;
}

int VFiPFFAT_GetClusterInChain(PF_FFD* p_ffd, u32 chain_index, u32 mode, u32 num_cluster, u32* locate_start, u32* locate_end) {
    s32 err;
    u32 bad;
    u32 eoc1;
    u32 current_cluster;
    u32 next_cluster;
    u32 trace_cnt;
    u32 append_cnt;
    u32 search_index;
    u32* p_chain_start;
    u32 chain_start;
    u32 check_use;
    PF_CACHE_PAGE* p_page;

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
            trace_cnt = trace_cnt + num_cluster - 1;
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
        err = VFiPFFAT_ReadFATSector(p_ffd->p_vol, &p_page, current_cluster);
        if (err)
            return err;
        p_page->option = 0;
        check_use = 1;
        while (trace_cnt-- && current_cluster < bad) {
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
        err = VFiPFFAT_AllocateChain(p_ffd, ++append_cnt, search_index, locate_start, &next_cluster);
        if (err)
            return err;
        if (next_cluster == -1)
            return 6;
        if (!*p_chain_start) {
            *p_chain_start = *locate_start;
        } else {
            err = VFiPFFAT_WriteCluster(p_ffd, current_cluster, search_index - 1, *locate_start, 1u);
            if (err)
                return err;
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
    u32 fat_type;
    s32 err;
    u32 is_found;

    p_vol = p_ffd->p_vol;
    fat_type = p_vol->bpb.fat_type;
    p_hint = p_ffd->p_hint;

    if (!*p_ffd->p_start_cluster && !may_allocate) {
        *p_cluster = -1;
        return 0;
    }

    if (*p_ffd->p_start_cluster == 1) {
        if (fat_type <= FAT_16) {
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
    if (err)
        return err;
    if (*p_cluster == -1)
        return 0;
    p_hint->chain_index = chain_index;
    p_hint->cluster = *p_cluster;
    return 0;
}

s32 VFiPFFAT_GetNumberOfCluster(PF_FFD* p_ffd, u32 chain_index, u32 may_allocate, u32* p_cluster) {
    int err;
    u32 locate_start;
    u32 locate_end;

    *p_cluster = -1;
    err = VFiPFFAT_GetClusterInChain(p_ffd, chain_index, may_allocate ? 1 : 0, 0, &locate_start, &locate_end);
    if (err) {
        return err;
    }

    if (locate_end >= 2 && locate_end < p_ffd->p_vol->bpb.num_clusters + 2) {
        *p_cluster = locate_end;
    }

    return 0;
}

static inline s32 VFiPFFAT_GetNumberOfAllocatedCluster(PF_FFD* p_ffd, u32 chain_index, u32 num_cluster, u32* p_cluster, u32* p_num_cluster) {
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

s32 VFiPFFAT_GetClusterAllocated(PF_FFD* p_ffd, u32 chain_index, u32 num_cluster, u32* p_cluster, u32* p_num_cluster) {
    PF_VOLUME* p_vol;
    PF_FAT_HINT* p_hint;
    enum FatType fat_type;
    int err;

    p_vol = p_ffd->p_vol;
    fat_type = p_vol->bpb.fat_type;
    p_hint = p_ffd->p_hint;
    if (*p_ffd->p_start_cluster == 1) {
        if (fat_type == FAT_12 || fat_type == FAT_16) {
            *p_cluster = -1;
            return 0;
        }
        *p_ffd->p_start_cluster = p_vol->bpb.root_dir_cluster;
    }
    err = VFiPFFAT_GetNumberOfAllocatedCluster(p_ffd, chain_index, num_cluster, p_cluster, p_num_cluster);
    if (err) {
        return err;
    }

    if (*p_cluster == -1)
        return 0;
    p_hint->chain_index = chain_index;
    p_hint->cluster = *p_cluster;
    return 0;
}

static s32 VFiPFFAT_GetSectorInRootDirRegion(PF_VOLUME* p_vol, u32 sector_offset, u32* p_sector) {
    if (p_vol->bpb.fat_type != FAT_12 && p_vol->bpb.fat_type != FAT_16)
        return 12;
    if (sector_offset >= p_vol->bpb.num_root_dir_sectors) {
        *p_sector = -1;
        return 0;
    }
    *p_sector = p_vol->bpb.first_root_dir_sector + sector_offset;
    return 0;
}

s32 VFiPFFAT_GetSector(PF_FFD* p_ffd, u32 file_sector_index, u32 mode, u32 size, u32* p_sector, u32* p_num_sector) {
    s32 err;
    PF_VOLUME* p_vol;
    u32 fat_type;
    u32 chain_index;
    u32 cluster;
    u32 num_divide;
    u32 num_remaind;
    u32 num_cluster;
    u32 num_success;

    num_success = 0;
    p_vol = p_ffd->p_vol;
    fat_type = p_vol->bpb.fat_type;
    if (*p_ffd->p_start_cluster == 1 && fat_type <= 1) {
        err = VFiPFFAT_GetSectorInRootDirRegion(p_vol, file_sector_index, p_sector);
        if (err)
            return err;
        return 0;
    } else {
        chain_index = file_sector_index >> p_vol->bpb.log2_sectors_per_cluster;
        if (mode == 2) {
            num_divide = size >> p_vol->bpb.log2_bytes_per_sector;
            num_remaind = (size & (p_vol->bpb.bytes_per_sector - 1)) != 0;
            num_cluster = (num_divide + num_remaind) >> p_vol->bpb.log2_sectors_per_cluster;
            num_cluster += ((p_vol->bpb.sectors_per_cluster - 1) & (num_divide + num_remaind)) != 0;
            err = VFiPFFAT_GetClusterAllocated(p_ffd, chain_index, num_cluster, &cluster, &num_success);
            if (err)
                return err;
            *p_num_sector = num_success << p_vol->bpb.log2_sectors_per_cluster;
        } else {
            err = VFiPFFAT_GetClusterSpecified(p_ffd, chain_index, mode, &cluster);
            if (err)
                return err;
            *p_num_sector = 1 << p_vol->bpb.log2_sectors_per_cluster;
        }
        if (cluster == -1) {
            *p_sector = -1;
            return 0;
        }
        *p_sector = (((cluster - 2) << p_vol->bpb.log2_sectors_per_cluster) + p_vol->bpb.first_data_sector) +
                    (file_sector_index & (p_vol->bpb.sectors_per_cluster - 1));
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
    s32 err;
    s32 err2;
    u32 fat_num;
    u32 fat_no;
    u32 offset;
    u32 num_success;

    err = 0;
    offset = sector - p_vol->bpb.num_reserved_sectors;
    fat_no = offset / p_vol->bpb.sectors_per_FAT;
    if (fat_no) {
        sector = offset % p_vol->bpb.sectors_per_FAT;
        fat_num = 0;
    } else {
        sector = sector + p_vol->bpb.sectors_per_FAT;
        fat_num = 1;
    }
    while (fat_num < p_vol->bpb.num_active_FATs) {
        err2 = VFiPFDRV_lwrite(p_vol, p_buf, sector, size, &num_success);
        if (err2 && !err) {
            err = err2;
        }
        if (num_success != size) {
            err = 17;
        }
        ++fat_num;
        sector += p_vol->bpb.sectors_per_FAT;
    }
    return err;
}

s32 VFiPFFAT_GetSectorSpecified(PF_FFD* p_ffd, u32 file_sector_index, u32 may_allocate, u32* p_sector) {
    int err;
    u32 mode;
    u32 num_sector;

    if (!p_ffd)
        return 10;
    if (!p_ffd->p_hint)
        return 10;
    if (p_ffd->p_vol->drv_char < 65 || p_ffd->p_vol->drv_char > 90)
        return 35;
    if (file_sector_index >= (0xFFFFFFFF >> p_ffd->p_vol->bpb.log2_bytes_per_sector) + 1)
        return 37;

    mode = may_allocate ? 1 : 0;
    err = VFiPFFAT_GetSector(p_ffd, file_sector_index, mode, 0, p_sector, &num_sector);
    if (err) {
        return err;
    } else {
        return 0;
    }
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
    if (*p_sector != -1) {
        cluster = ((*p_sector - p_vol->bpb.first_data_sector) >> p_vol->bpb.log2_sectors_per_cluster) + 2;
    } else {
        err = VFiPFFAT_GetSectorSpecified(p_ffd, file_sector_index, 0, p_sector);
        if (err)
            return err;
        if (*p_sector != -1)
            cluster = ((*p_sector - p_vol->bpb.first_data_sector) >> p_vol->bpb.log2_sectors_per_cluster) + 2;
    }
    if (*p_sector != -1)
        *p_num_sector = p_vol->bpb.first_data_sector + ((cluster - 1) << p_vol->bpb.log2_sectors_per_cluster) - *p_sector;
    else
        *p_num_sector = 0;
    if (*p_sector != -1 && (*p_num_sector << p_vol->bpb.log2_bytes_per_sector) < size) {
        err = VFiPFFAT_GetClusterContinuousSectorInChain(p_ffd, cluster, (file_sector_index >> p_vol->bpb.log2_sectors_per_cluster) + 1, size,
                                                         p_num_sector);
        if (err)
            return err;
    }
    return 0;
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
    u32 free_cluster;
    s32 err;
    u32 next_cluster;
    PF_CACHE_PAGE* p_page;

    if (!p_vol)
        return 10;
    if (p_vol->bpb.fat_type != FAT_12 && p_vol->bpb.fat_type != FAT_16 && p_vol->bpb.fat_type != FAT_32)
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
        err = VFiPFDRV_StoreFreeCountToFSINFO(p_vol);
    return err;
}

s32 VFiPFFAT_FreeChain(PF_FFD* p_ffd, u32 start_cluster, u32 chain_index, u32 size) {
    s32 err;
    PF_VOLUME* p_vol;
    u32 eoc1;
    u32 next_cluster;
    u32 file_size;
    u32 clst_size;
    PF_CACHE_PAGE* p_page;
    u32 sector;
    u32 save_start_cluster;
    u32 save_num_clusters;

    p_vol = p_ffd->p_vol;
    if (!p_vol)
        return 10;
    if ((start_cluster < 2 || start_cluster >= p_vol->bpb.num_clusters + 2) && start_cluster)
        return 14;
    if (!start_cluster)
        return 0;
    eoc1 = fat_special_values[p_vol->bpb.fat_type].eoc1;
    if (size) {
        file_size = size;
        clst_size = p_vol->bpb.bytes_per_sector << p_vol->bpb.log2_sectors_per_cluster;
    } else {
        return 0;
    }
    err = VFiPFFAT_ReadFATSector(p_vol, &p_page, start_cluster);
    if (err)
        return err;
    {
        p_page->option = 1;
        next_cluster = start_cluster;
        save_start_cluster = start_cluster;
        save_num_clusters = 1;
        while (start_cluster < eoc1) {
            if (size && !file_size)
                break;
            sector = p_page->sector;
            err = VFiPFFAT_ReadFATEntryPage(p_vol, start_cluster, &next_cluster, &p_page);
            if (err)
                return err;
            if (!next_cluster) {
                err = VFiPFFAT_UpdateFATEntry(p_vol, p_page);
                if (err)
                    return err;
                return 0;
            }
            if (sector != p_page->sector) {
                err = VFiPFFAT_ReadFATSector(p_vol, &p_page, start_cluster);
                if (err)
                    return err;
            }
            if (chain_index != -1) {
                err = VFiPFFAT_WriteClusterPage(p_ffd, start_cluster, chain_index, 0, 1u, &p_page);
                ++chain_index;
            } else {
                err = VFiPFFAT_WriteFATEntryPage(p_vol, start_cluster, 0, &p_page);
            }
            if (err)
                return err;
            if ((p_vol->flags & 0x40) != 0) {
                if (next_cluster == start_cluster + 1) {
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
                file_size = file_size <= clst_size ? 0 : file_size - clst_size;
            }
            if (p_vol->num_free_clusters != -1)
                ++p_vol->num_free_clusters;
            start_cluster = next_cluster;
        }
        err = VFiPFFAT_UpdateFATEntry(p_vol, p_page);
        if (err)
            return err;
        {
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

s32 VFiPFFAT_GetBeforeChain(PF_VOLUME* p_vol, u32 start_cluster, u32 lActive, u32* p_cluster) {
    s32 err;
    u32 eoc1;
    u32 next_cluster;
    PF_CACHE_PAGE* p_page;

    if (!p_vol) {
        *p_cluster = -1;
        return 10;
    }
    if (start_cluster < 2 || start_cluster >= p_vol->bpb.num_clusters + 2) {
        *p_cluster = -1;
        return 10;
    }
    eoc1 = fat_special_values[p_vol->bpb.fat_type].eoc1;
    *p_cluster = -1;
    err = VFiPFFAT_ReadFATSector(p_vol, &p_page, start_cluster);
    if (err)
        return err;
    {
        p_page->option = 0;
        while (start_cluster < eoc1) {
            err = VFiPFFAT_ReadFATEntryPage(p_vol, start_cluster, &next_cluster, &p_page);
            if (err)
                return err;
            if (lActive == next_cluster) {
                *p_cluster = start_cluster;
                return 0;
            }
            if ((--start_cluster < 2 || start_cluster >= p_vol->bpb.num_clusters + 2) && start_cluster < eoc1) {
                *p_cluster = lActive;
                return 0;
            }
        }
        return 13;
    }
}

s32 VFiPFFAT_GetBeforeSector(u32* p_befor_sector, PF_VOLUME* p_vol, u32 current_sector) {
    s32 err;
    u32 sector_index;
    u32 current_cluster;
    u32 befor_cluster;

    err = 0;
    if (!p_befor_sector || !p_vol)
        return 10;
    if (current_sector < p_vol->bpb.first_root_dir_sector)
        return 10;
    if (p_vol->bpb.fat_type != FAT_32 && current_sector < p_vol->bpb.first_data_sector) {
        --current_sector;
        if (current_sector < p_vol->bpb.first_root_dir_sector)
            err = 34;
    } else {
        sector_index = current_sector - p_vol->bpb.first_data_sector;
        if (((p_vol->bpb.sectors_per_cluster - 1) & sector_index) == 0) {
            current_cluster = (sector_index >> p_vol->bpb.log2_sectors_per_cluster) + 2;
            err = VFiPFFAT_GetBeforeChain(p_vol, current_cluster, current_cluster, &befor_cluster);
            if (!err) {
                if (befor_cluster == -1)
                    current_sector = -1;
                else
                    current_sector = ((befor_cluster - 2) << p_vol->bpb.log2_sectors_per_cluster) +
                                     (p_vol->bpb.sectors_per_cluster + p_vol->bpb.first_data_sector) - 1;
            }
        } else {
            --current_sector;
        }
    }
    if (!err)
        *p_befor_sector = current_sector;
    else
        *p_befor_sector = -1;
    return err;
}

s32 VFiPFFAT_InitFATRegion(PF_VOLUME* p_vol) {
    s32 err;
    PF_CACHE_PAGE* p_page;
    u32 sector;
    u32 size;
    u32 success_size;
    u32 num_cache_buff;
    u32 root_dir_sector;

    if (!p_vol)
        return 10;
    if (p_vol->bpb.fat_type != FAT_12 && p_vol->bpb.fat_type != FAT_16 && p_vol->bpb.fat_type != FAT_32)
        return 15;
    err = VFiPFCACHE_AllocateDataPage(p_vol, 0xFFFFFFFF, &p_page);
    if (err)
        return err;
    {
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
        if (err)
            return err;
        {
            err = VFiPFFAT_WriteFATEntry(p_vol, 1u, fat_special_values[p_vol->bpb.fat_type].fat1);
            if (err)
                return err;
            {
                err = VFiPFCACHE_FlushFATCache(p_vol);
                if (err)
                    return err;
                return 0;
            }
        }
    }
}

s32 VFiPFFAT_MakeRootDir(PF_VOLUME* p_vol) {
    int err;

    err = VFiPFFAT_WriteFATEntry(p_vol, p_vol->bpb.root_dir_cluster, fat_special_values[p_vol->bpb.fat_type].eoc2);
    if (err)
        return err;
    {
        err = VFiPFCACHE_FlushFATCache(p_vol);
        if (err)
            return err;
        return 0;
    }
}

s32 VFiPFFAT_TraceClustersChain(PF_FFD* p_ffd, u32 start_clst, u32 size, u32* p_target_clst, u32* p_next_clst) {
    s32 err;
    PF_VOLUME* p_vol;
    u32 next_cluster;
    u32 save_cluster;
    u32 chain_index;
    u32 clst_size;
    u32 clst_cnt;
    u32 cnt;
    u32 check_use;
    PF_CACHE_PAGE* p_page;

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
        if (err)
            return err;
        {
            for (cnt = 0; cnt < clst_cnt; ++cnt) {
                err = VFiPFFAT_ReadClusterPage(p_ffd, start_clst, chain_index, &next_cluster, &check_use, &p_page);
                if (err)
                    return err;
                save_cluster = start_clst;
                if (next_cluster == fat_special_values[p_vol->bpb.fat_type].eoc2) {
                    p_ffd->last_cluster.num_last_cluster = start_clst;
                    p_ffd->last_cluster.max_chain_index = chain_index - 1;
                    break;
                }
                start_clst = next_cluster;
                ++chain_index;
            }
            p_ffd->p_hint->chain_index = chain_index - 1;
            *p_target_clst = save_cluster;
            *p_next_clst = next_cluster;
            return 0;
        }
    }
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
    p_ffd->last_access.cluster = last_access->cluster;
    p_ffd->last_access.chain_index = last_access->chain_index;
}

u32 VFiPFFAT_GetValueOfEOC2(PF_VOLUME* p_vol) {
    return fat_special_values[p_vol->bpb.fat_type].eoc2;
}
