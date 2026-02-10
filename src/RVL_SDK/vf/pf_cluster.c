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

        if ((p_file->cursor.position & (p_vol->bpb.bytes_per_sector - 1)) != 0 ||
            (p_file->cursor.file_sector_index & (p_vol->bpb.sectors_per_cluster - 1)) != 0) {
            p_file->last_access.chain_index = p_file->cursor.file_sector_index >> p_vol->bpb.log2_sectors_per_cluster;
            p_file->last_access.cluster = ((sector - p_vol->bpb.first_data_sector) >> p_vol->bpb.log2_sectors_per_cluster) + 2;
        } else if (p_file->cursor.file_sector_index) {
            p_file->last_access.chain_index = (p_file->cursor.file_sector_index - 1) >> p_vol->bpb.log2_sectors_per_cluster;
            p_file->last_access.cluster = ((sector - 1 - p_vol->bpb.first_data_sector) >> p_vol->bpb.log2_sectors_per_cluster) + 2;
        }
    }
}

void VFiPFCLUSTER_SetLastAccessCluster(PF_FILE* p_file) {
    VFiPFFAT_SetLastAccess(&p_file->p_sfd->ffd, &p_file->last_access);
}

s32 VFiPFCLUSTER_AppendCluster(PF_FILE* p_file, u32 byte, u32* p_success, u32* sector) {
    int result;
    PF_VOLUME* p_vol;
    u32 v10;
    int v11;
    u32 cluster;
    u32 v13;
    int v14;
    u32 v15;
    u32 v16;
    int v17;
    u32 v18;
    u32 max_appendable_size;
    u32 num_sector;
    PF_FAT_HINT v21;
    PF_CURSOR save_cursor;

    *p_success = 0;
    if (&VFipf_vol_set > (PF_VOLUME_SET*)p_file || (PF_FILE*)0x295B4 < p_file)
        return 10;
    if (!p_file || !p_file->p_sfd || (p_file->stat & 1) == 0 || (p_file->p_sfd->stat & 1) == 0 || (p_file->p_sfd->stat & 2) == 0) {
        return 38;
    }
    if ((p_file->p_sfd->stat & 1) == 0 || (p_file->p_sfd->stat & 2) == 0)
        return 38;
    p_vol = p_file->p_sfd->dir_entry.p_vol;
    if (!p_vol)
        return 38;
    v10 = p_file->cursor.sector;
    save_cursor.position = p_file->cursor.position;
    save_cursor.sector = v10;
    v11 = *&p_file->cursor.offset_in_sector;
    save_cursor.file_sector_index = p_file->cursor.file_sector_index;
    *&save_cursor.offset_in_sector = v11;
    cluster = p_file->hint.cluster;
    v21.chain_index = p_file->hint.chain_index;
    v21.cluster = cluster;
    *sector = -1;
    if ((p_vol->fsi_flag & 4) != 0 && !p_vol->num_free_clusters)
        return 6;
    VFiPFFILE_Cursor_MoveToClusterEnd(p_file, p_file->p_sfd->dir_entry.file_size + byte);
    if (p_file->cursor.position == -1) {
        *p_success = 0;
        return 37;
    } else {
        result = VFiPFFAT_GetSectorAllocated(&p_file->p_sfd->ffd, p_file->cursor.file_sector_index, byte, sector, &num_sector);
        if (result) {
            v13 = save_cursor.sector;
            p_file->cursor.position = save_cursor.position;
            p_file->cursor.sector = v13;
            v14 = *&save_cursor.offset_in_sector;
            p_file->cursor.file_sector_index = save_cursor.file_sector_index;
            *&p_file->cursor.offset_in_sector = v14;
            v15 = v21.cluster;
            p_file->hint.chain_index = v21.chain_index;
            p_file->hint.cluster = v15;
        } else {
            v16 = save_cursor.sector;
            p_file->cursor.position = save_cursor.position;
            p_file->cursor.sector = v16;
            v17 = *&save_cursor.offset_in_sector;
            p_file->cursor.file_sector_index = save_cursor.file_sector_index;
            *&p_file->cursor.offset_in_sector = v17;
            v18 = v21.cluster;
            p_file->hint.chain_index = v21.chain_index;
            p_file->hint.cluster = v18;
            max_appendable_size = num_sector << p_vol->bpb.log2_bytes_per_sector;
            if (byte < max_appendable_size)
                max_appendable_size = byte;
            *p_success = max_appendable_size;
            return 0;
        }
    }
    return result;
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
    if (&VFipf_vol_set > (PF_VOLUME_SET*)p_file || (PF_FILE*)0x295B4 < p_file)
        return 10;
    if (!p_file || !p_file->p_sfd || (p_file->stat & 1) == 0 || (p_file->p_sfd->stat & 1) == 0 || (p_file->p_sfd->stat & 2) == 0) {
        return 38;
    }
    if ((p_file->p_sfd->stat & 1) == 0 || (p_file->p_sfd->stat & 2) == 0)
        return 38;
    p_vol = p_file->p_sfd->dir_entry.p_vol;
    if (!p_vol)
        return 38;
    cluster_size = p_vol->bpb.bytes_per_sector << p_vol->bpb.log2_sectors_per_cluster;
    if (!p_file->p_sfd->dir_entry.start_cluster)
        return 0;
    if (p_file->p_sfd->dir_entry.file_size % cluster_size)
        v6 = cluster_size - p_file->p_sfd->dir_entry.file_size % cluster_size;
    else
        v6 = 0;
    file_cluster_size = p_file->p_sfd->dir_entry.file_size + v6;
    if (p_file->p_sfd->ffd.last_cluster.max_chain_index) {
        total_allocated_size = (p_file->p_sfd->ffd.last_cluster.max_chain_index + 1) * cluster_size;
    LABEL_30:
        *p_size = total_allocated_size - file_cluster_size;
        return 0;
    }
    if (!p_file->p_sfd->dir_entry.file_size) {
        unused_cluster = p_file->p_sfd->dir_entry.start_cluster;
    LABEL_25:
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
        goto LABEL_30;
    }
    result = VFiPFFAT_TraceClustersChain(&p_file->p_sfd->ffd, p_file->p_sfd->dir_entry.start_cluster, p_file->p_sfd->dir_entry.file_size,
                                         &file_end_cluster, &unused_cluster);
    if (!result)
        goto LABEL_25;
    return result;
}
