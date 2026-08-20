#include "revolution/vf/pf_cluster.h"
#include "revolution/vf/pf_fat.h"
#include "revolution/vf/pf_file.h"

void VFiPFCLUSTER_InitLastAccessCluster(PF_FILE* p_file) {
    p_file->last_access.chain_index = 0;
    p_file->last_access.cluster = 0;
}

void VFiPFCLUSTER_UpdateLastAccessCluster(PF_FILE* p_file, u32 sector) {
    PF_VOLUME* p_vol;

    if (!p_file->cursor.position) {
        p_file->last_access.chain_index = 0;
        p_file->last_access.cluster = 0;
    } else {
        if (!p_file)
            p_vol = 0;
        else
            p_vol = p_file->p_sfd->dir_entry.p_vol;

        if ((p_file->cursor.position & (p_vol->bpb.bytes_per_sector - 1)) == 0 &&
            (p_file->cursor.file_sector_index & (p_vol->bpb.sectors_per_cluster - 1)) == 0) {
            if (p_file->cursor.file_sector_index) {
                p_file->last_access.chain_index = (p_file->cursor.file_sector_index - 1) >> p_vol->bpb.log2_sectors_per_cluster;
                p_file->last_access.cluster = ((sector - 1 - p_vol->bpb.first_data_sector) >> p_vol->bpb.log2_sectors_per_cluster) + 2;
            }
        } else {
            p_file->last_access.chain_index = p_file->cursor.file_sector_index >> p_vol->bpb.log2_sectors_per_cluster;
            p_file->last_access.cluster = ((sector - p_vol->bpb.first_data_sector) >> p_vol->bpb.log2_sectors_per_cluster) + 2;
        }
    }
}

void VFiPFCLUSTER_SetLastAccessCluster(PF_FILE* p_file) {
    VFiPFFAT_SetLastAccess(&p_file->p_sfd->ffd, &p_file->last_access);
}

s32 VFiPFCLUSTER_AppendCluster(PF_FILE* p_file, u32 byte, u32* p_success, u32* sector) {
    s32 result;
    PF_VOLUME* p_vol;
    u32 max_appendable_size;
    u32 num_sector;
    PF_CURSOR save_cursor;
    PF_FAT_HINT save_hint;

    *p_success = 0;
    if (&VFipf_vol_set > (PF_VOLUME_SET*)p_file || (PF_VOLUME_SET*)((u8*)&VFipf_vol_set + sizeof(VFipf_vol_set)) < (PF_VOLUME_SET*)p_file)
        return 10;
    if (!p_file || !p_file->p_sfd || (p_file->stat & 1) == 0 || (p_file->p_sfd->stat & 1) == 0 || (p_file->p_sfd->stat & 2) == 0) {
        return 38;
    }
    if ((p_file->p_sfd->stat & 1) == 0 || (p_file->p_sfd->stat & 2) == 0)
        return 38;
    if (!p_file)
        p_vol = 0;
    else
        p_vol = p_file->p_sfd->dir_entry.p_vol;
    if (!p_vol)
        return 38;

    save_cursor = p_file->cursor;
    save_hint = p_file->hint;

    *sector = -1;
    if ((p_vol->fsi_flag & 4) != 0 && p_vol->num_free_clusters != 0xFFFFFFFF && p_vol->num_free_clusters == 0)
        return 6;

    VFiPFFILE_Cursor_MoveToClusterEnd(p_file, p_file->p_sfd->dir_entry.file_size + byte);
    if (p_file->cursor.position == (u32)-1) {
        *p_success = 0;
        return 37;
    }

    result = VFiPFFAT_GetSectorAllocated(&p_file->p_sfd->ffd, p_file->cursor.file_sector_index, byte, sector, &num_sector);
    if (result) {
        p_file->cursor = save_cursor;
        p_file->hint = save_hint;
        return result;
    }

    p_file->cursor = save_cursor;
    p_file->hint = save_hint;
    max_appendable_size = num_sector << p_vol->bpb.log2_bytes_per_sector;
    if (byte < max_appendable_size)
        max_appendable_size = byte;
    *p_success = max_appendable_size;
    return 0;
}

s32 VFiPFCLUSTER_GetAppendSize(PF_FILE* p_file, u32* p_size) {
    int result;
    u32 cluster_size;
    int v5;
    u32 v6;
    PF_VOLUME* p_vol;
    int ValueOfEOC2;
    u32 next_cluster;
    u32 unused_cluster;
    u32 file_end_cluster;
    u32 total_allocated_size;
    u32 file_cluster_size;

    *p_size = 0;
    if (&VFipf_vol_set > (PF_VOLUME_SET*)p_file || (PF_VOLUME_SET*)((u8*)&VFipf_vol_set + sizeof(VFipf_vol_set)) < (PF_VOLUME_SET*)p_file)
        return 10;
    if (!p_file || !p_file->p_sfd || (p_file->stat & 1) == 0 || (p_file->p_sfd->stat & 1) == 0 || (p_file->p_sfd->stat & 2) == 0) {
        return 38;
    }
    if ((p_file->p_sfd->stat & 1) == 0 || (p_file->p_sfd->stat & 2) == 0)
        return 38;
    if (!p_file)
        p_vol = 0;
    else
        p_vol = p_file->p_sfd->dir_entry.p_vol;
    if (!p_vol)
        return 38;
    cluster_size = p_vol->bpb.bytes_per_sector << p_vol->bpb.log2_sectors_per_cluster;
    if (p_file->p_sfd->dir_entry.start_cluster) {
        if (p_file->p_sfd->dir_entry.file_size % cluster_size)
            v6 = cluster_size - p_file->p_sfd->dir_entry.file_size % cluster_size;
        else
            v6 = 0;
        file_cluster_size = p_file->p_sfd->dir_entry.file_size + v6;
        if (p_file->p_sfd->ffd.last_cluster.max_chain_index) {
            total_allocated_size = (p_file->p_sfd->ffd.last_cluster.max_chain_index + 1) * cluster_size;
        } else {
            if (p_file->p_sfd->dir_entry.file_size) {
                result = VFiPFFAT_TraceClustersChain(&p_file->p_sfd->ffd, p_file->p_sfd->dir_entry.start_cluster, p_file->p_sfd->dir_entry.file_size,
                                                     &file_end_cluster, &unused_cluster);
                if (result)
                    return result;
            } else {
                unused_cluster = p_file->p_sfd->dir_entry.start_cluster;
            }
            ValueOfEOC2 = VFiPFFAT_GetValueOfEOC2(p_vol);
            v5 = 0;
            while (unused_cluster != ValueOfEOC2) {
                ++v5;
                result = VFiPFFAT_ReadValueToSpecifiedCluster(p_vol, unused_cluster, &next_cluster);
                if (result)
                    return result;
                unused_cluster = next_cluster;
            }
            total_allocated_size = file_cluster_size + v5 * cluster_size;
        }
        *p_size = total_allocated_size - file_cluster_size;
    }
    return 0;
}
