#include "revolution/vf/pf_file.h"
#include "revolution/vf/pf_cache.h"
#include "revolution/vf/pf_clib.h"
#include "revolution/vf/pf_cluster.h"
#include "revolution/vf/pf_driver.h"
#include "revolution/vf/pf_entry.h"
#include "revolution/vf/pf_fat.h"
#include "revolution/vf/pf_filelock.h"
#include "revolution/vf/pf_path.h"
#include "revolution/vf/pf_sector.h"
#include "revolution/vf/pf_str.h"
#include "revolution/vf/pf_volume.h"

static PF_VOLUME* VFiPFFILE_GetVolume(PF_FILE* p_file) {
    if (p_file == NULL) {
        return NULL;
    }
    return p_file->p_sfd->dir_entry.p_vol;
}

static inline void VFiPFFILE_Cursor_Recalc(PF_FILE* p_file) {
    PF_VOLUME* p_vol;

    p_vol = VFiPFFILE_GetVolume(p_file);

    p_file->cursor.file_sector_index = p_file->cursor.position >> p_vol->bpb.log2_bytes_per_sector;
    p_file->cursor.offset_in_sector = p_file->cursor.position & (p_vol->bpb.bytes_per_sector - 1);
}

static inline void VFiPFFILE_Cursor_SetPosition(PF_FILE* p_file, u32 pos) {
    PF_VOLUME* p_vol;
    u32 pre_sec_off;
    u32 post_sec_off;

    p_vol = VFiPFFILE_GetVolume(p_file);

    pre_sec_off =
        (p_file->cursor.position >> p_vol->bpb.log2_bytes_per_sector) + ((p_file->cursor.position & (p_vol->bpb.bytes_per_sector - 1)) ? 1 : 0);
    post_sec_off = (pos >> p_vol->bpb.log2_bytes_per_sector) + ((pos & (p_vol->bpb.bytes_per_sector - 1)) ? 1 : 0);
    if (pre_sec_off != post_sec_off) {
        p_file->cursor.sector = -1;
    }

    p_file->cursor.position = pos;
    VFiPFFILE_Cursor_Recalc(p_file);
}

void VFiPFFILE_Cursor_MoveToEnd(PF_FILE* p_file) {
    VFiPFFILE_Cursor_SetPosition(p_file, p_file->p_sfd->dir_entry.file_size);
}

u32 VFiPFFILE_Cursor_AdvanceToRead(PF_FILE* p_file, u32 n, u32 sector) {
    u32 res;
    PF_VOLUME* p_vol;
    u32 wk_sector;

    res = 1;
    p_vol = VFiPFFILE_GetVolume(p_file);

    wk_sector = sector + ((p_file->cursor.offset_in_sector + n) >> p_vol->bpb.log2_bytes_per_sector);
    if (p_file->cursor.position + n < p_file->p_sfd->dir_entry.file_size) {
        if (((p_vol->bpb.bytes_per_sector - 1) & (p_file->cursor.offset_in_sector + n)) != 0) {
            p_file->cursor.sector = wk_sector;
        } else {
            p_file->cursor.sector = -1;
        }
        p_file->cursor.position += n;
    } else {
        if (p_file->p_sfd->dir_entry.file_size == 0) {
            p_file->cursor.sector = -1;
            p_file->cursor.position = 0;
            if (n) {
                res = 0;
            }
        } else {
            p_file->cursor.sector = -1;
            p_file->cursor.position = p_file->p_sfd->dir_entry.file_size;
            res = 0;
        }
    }
    VFiPFFILE_Cursor_Recalc(p_file);
    VFiPFCLUSTER_UpdateLastAccessCluster(p_file, wk_sector);
    return res;
}

int VFiPFFILE_Cursor_ReadHeadSector(PF_VOLUME* p_vol, PF_FILE* p_file, u8* p_buf, u32 size, u32* p_size_read) {
    s32 result;
    u32 max_readable_size;
    u32 success_size;

    *p_size_read = 0;
    if (!p_file->cursor.offset_in_sector)
        return 0;
    max_readable_size = p_vol->bpb.bytes_per_sector - p_file->cursor.offset_in_sector;
    if (size > max_readable_size)
        size = max_readable_size;
    if (p_file->cursor.position + size > p_file->p_sfd->dir_entry.file_size) {
        size = p_file->p_sfd->dir_entry.file_size - p_file->cursor.position;
        if (size < max_readable_size)
            return 0;
    }
    if (p_file->cursor.sector != -1)
        goto LABEL_14;
    result = VFiPFFAT_GetSectorSpecified(&p_file->p_sfd->ffd, p_file->cursor.file_sector_index, 0, &p_file->cursor.sector);
    if (result)
        return result;
    if (p_file->cursor.sector == -1) {
        VFiPFFILE_Cursor_MoveToEnd(p_file);
        return 28;
    }
    if (p_file->cursor.position + size > p_file->p_sfd->dir_entry.file_size) {
        VFiPFFILE_Cursor_MoveToEnd(p_file);
        return 27;
    } else {
    LABEL_14:
        result = VFiPFSEC_ReadData(p_vol, p_buf, p_file->cursor.sector, p_file->cursor.offset_in_sector, size, &success_size, 1u);
        if (result != 0 && success_size == 0) {
            return result;
        }
        *p_size_read = success_size;
        VFiPFFILE_Cursor_AdvanceToRead(p_file, success_size, p_file->cursor.sector);
        return 0;
    }
    return result;
}

s32 VFiPFFILE_Cursor_ReadBodySectors(PF_VOLUME* p_vol, PF_FILE* p_file, u8* p_buf, u32 size, u32* p_size_read) {
    s32 err;
    u32 num_sector;
    u32 max_readable_size;
    u32 success_size;

    *p_size_read = 0;
    num_sector = 0;
    err = VFiPFFAT_GetContinuousSector(&p_file->p_sfd->ffd, p_file->cursor.file_sector_index, size, &p_file->cursor.sector, &num_sector);
    if (err)
        return err;
    if (p_file->cursor.sector == -1) {
        VFiPFFILE_Cursor_MoveToEnd(p_file);
        return 28;
    }
    max_readable_size = size;
    if (max_readable_size > num_sector << p_vol->bpb.log2_bytes_per_sector)
        max_readable_size = num_sector << p_vol->bpb.log2_bytes_per_sector;
    if (p_file->cursor.position + max_readable_size > p_file->p_sfd->dir_entry.file_size) {
        max_readable_size = p_file->p_sfd->dir_entry.file_size - p_file->cursor.position;
        max_readable_size -= max_readable_size & (p_vol->bpb.bytes_per_sector - 1);
        if (max_readable_size < p_vol->bpb.bytes_per_sector)
            return 0;
    }
    err = VFiPFSEC_ReadDataSector(p_vol, p_buf, p_file->cursor.sector, max_readable_size, &success_size, 1);
    if (err != 0 && success_size == 0)
        return err;
    *p_size_read = success_size;
    VFiPFFILE_Cursor_AdvanceToRead(p_file, success_size, p_file->cursor.sector);
    return 0;
}

s32 VFiPFFILE_Cursor_ReadTailSector(struct PF_VOLUME* p_vol, struct PF_FILE* p_file, u8* p_buf, u32 size, u32* p_size_read) {
    int err;
    u32 success_size;

    *p_size_read = 0;
    if (!size)
        return 0;
    if (p_file->cursor.position + size > p_file->p_sfd->dir_entry.file_size) {
        size = p_file->p_sfd->dir_entry.file_size - p_file->cursor.position;
        if (size == 0)
            return 0;
    }
    if (p_file->cursor.sector != -1)
        goto LABEL_12;
    err = VFiPFFAT_GetSectorSpecified(&p_file->p_sfd->ffd, p_file->cursor.file_sector_index, 0, &p_file->cursor.sector);
    if (err)
        return err;
    if (p_file->cursor.sector == -1) {
        VFiPFFILE_Cursor_MoveToEnd(p_file);
        return 28;
    }
    if (p_file->cursor.position + size > p_file->p_sfd->dir_entry.file_size) {
        VFiPFFILE_Cursor_MoveToEnd(p_file);
        return 27;
    } else {
    LABEL_12:
        err = VFiPFSEC_ReadData(p_vol, p_buf, p_file->cursor.sector, p_file->cursor.offset_in_sector, size, &success_size, 1u);
        if (err != 0 && success_size == 0) {
            return err;
        }
        *p_size_read = success_size;
        VFiPFFILE_Cursor_AdvanceToRead(p_file, success_size, p_file->cursor.sector);
        return 0;
    }
    return err;
}

s32 VFiPFFILE_Cursor_Read(PF_FILE* p_file, u8* p_buf, u32 size, u32* p_size_read) {
    s32 err;
    PF_VOLUME* p_vol;
    u32 size_read;

    *p_size_read = 0;
    p_vol = VFiPFFILE_GetVolume(p_file);
    VFiPFCLUSTER_SetLastAccessCluster(p_file);
    err = VFiPFFILE_Cursor_ReadHeadSector(p_vol, p_file, p_buf, size, &size_read);
    *p_size_read += size_read;
    if (err)
        return err;
    if (size_read >= size)
        return 0;
    size -= size_read;
    while (size >= p_vol->bpb.bytes_per_sector) {
        err = VFiPFFILE_Cursor_ReadBodySectors(p_vol, p_file, &p_buf[*p_size_read], size, &size_read);
        *p_size_read += size_read;
        if (err)
            return err;
        if (!size_read)
            break;
        size -= size_read;
    }
    err = VFiPFFILE_Cursor_ReadTailSector(p_vol, p_file, &p_buf[*p_size_read], size, &size_read);
    *p_size_read += size_read;
    if (err)
        return err;
    else
        return 0;
}

u32 VFiPFFILE_Cursor_AdvanceToWrite(PF_FILE* p_file, u32 n, u32 sector) {
    PF_VOLUME* p_vol;
    u32 v5;

    p_vol = VFiPFFILE_GetVolume(p_file);
    v5 = sector + ((p_file->cursor.offset_in_sector + n) >> p_vol->bpb.log2_bytes_per_sector);
    if (((p_vol->bpb.bytes_per_sector - 1) & (p_file->cursor.offset_in_sector + n)) != 0)
        p_file->cursor.sector = sector + ((p_file->cursor.offset_in_sector + n) >> p_vol->bpb.log2_bytes_per_sector);
    else
        p_file->cursor.sector = -1;
    p_file->cursor.position += n;
    if (p_file->cursor.position > p_file->p_sfd->dir_entry.file_size)
        p_file->p_sfd->dir_entry.file_size = p_file->cursor.position;

    VFiPFFILE_Cursor_Recalc(p_file);
    VFiPFCLUSTER_UpdateLastAccessCluster(p_file, v5);
    return 1;
}

s32 VFiPFFILE_Cursor_WriteHeadSector(PF_VOLUME* p_vol, PF_FILE* p_file, u8* p_buf, u32 size, u32* p_size_write) {
    s32 result;
    u32 success_size;

    *p_size_write = 0;
    if (!p_file->cursor.offset_in_sector)
        return 0;
    if (size > p_vol->bpb.bytes_per_sector - p_file->cursor.offset_in_sector)
        size = p_vol->bpb.bytes_per_sector - p_file->cursor.offset_in_sector;
    if (p_file->cursor.sector != -1)
        goto LABEL_9;
    result = VFiPFFAT_GetSectorSpecified(&p_file->p_sfd->ffd, p_file->cursor.file_sector_index, 0, &p_file->cursor.sector);
    if (result)
        return result;
    if (p_file->cursor.sector == -1) {
        VFiPFFILE_Cursor_MoveToEnd(p_file);
        return 28;
    } else {
    LABEL_9:
        result = VFiPFSEC_WriteData(p_vol, p_buf, p_file->cursor.sector, p_file->cursor.offset_in_sector, size, &success_size, 0, 1u);
        if (result != 0 && success_size == 0) {
            return result;
        }
        *p_size_write = success_size;
        VFiPFFILE_Cursor_AdvanceToWrite(p_file, success_size, p_file->cursor.sector);
        return 0;
    }
    return result;
}

s32 VFiPFFILE_Cursor_WriteTailSector(PF_VOLUME* p_vol, PF_FILE* p_file, u8* p_buf, u32 size, u32 append_size, u32* p_size_write) {
    int err;
    u32 sector;
    u32 success_size;

    *p_size_write = 0;
    if (!size)
        return 0;
    if (size > p_vol->bpb.bytes_per_sector - p_file->cursor.offset_in_sector || size > p_vol->bpb.bytes_per_sector)
        return 26;
    if (append_size) {
        err = VFiPFCLUSTER_AppendCluster(p_file, append_size, &size, &sector);
        if (err)
            return err;
        if (!size)
            return 0;
        p_file->cursor.sector = sector;
    } else if (p_file->cursor.sector == -1) {
        err = VFiPFFAT_GetSectorSpecified(&p_file->p_sfd->ffd, p_file->cursor.file_sector_index, 0, &p_file->cursor.sector);
        if (err)
            return err;
        if (p_file->cursor.sector == -1) {
            VFiPFFILE_Cursor_MoveToEnd(p_file);
            return 28;
        }
    }
    err = VFiPFSEC_WriteData(p_vol, p_buf, p_file->cursor.sector, p_file->cursor.offset_in_sector, size, &success_size,
                             p_file->cursor.position == p_file->p_sfd->dir_entry.file_size, 1u);
    if (err != 0 && success_size == 0) {
        return err;
    }
    {
        *p_size_write = success_size;
        VFiPFFILE_Cursor_AdvanceToWrite(p_file, success_size, p_file->cursor.sector);
        return 0;
    }
}

static inline s32 VFiPFFILE_Cursor_WriteBodySectors(PF_VOLUME* p_vol, PF_FILE* p_file, u8* p_buf, u32 size, u32* p_size_write) {
    int result;
    u32 success_size;

    *p_size_write = 0;
    result = VFiPFSEC_WriteDataSector(p_vol, p_buf, p_file->cursor.sector, size, &success_size,
                                      p_file->cursor.position == p_file->p_sfd->dir_entry.file_size, 1);
    if (result && !success_size) {
        return result;
    }

    *p_size_write = success_size;
    VFiPFFILE_Cursor_AdvanceToWrite(p_file, success_size, p_file->cursor.sector);
    return 0;
}

s32 VFiPFFILE_Cursor_Write_Overwrite(struct PF_VOLUME* p_vol, struct PF_FILE* p_file, u8* p_buf, u32 size, u32* p_size_write) {
    u32 max_writable_size;
    int err;
    u8 log2_bytes_per_sector;
    u32 num_sector;
    u32 success_size;

    max_writable_size = size;
    *p_size_write = 0;
    err = VFiPFFAT_GetContinuousSector(&p_file->p_sfd->ffd, p_file->cursor.file_sector_index, size, &p_file->cursor.sector, &num_sector);
    if (err) {
        return err;
    }

    if (!num_sector || p_file->cursor.sector == -1) {
        VFiPFFILE_Cursor_MoveToEnd(p_file);
        return 28;
    } else {
        log2_bytes_per_sector = p_vol->bpb.log2_bytes_per_sector;
        if (max_writable_size > num_sector << log2_bytes_per_sector)
            max_writable_size = num_sector << log2_bytes_per_sector;
        err = VFiPFFILE_Cursor_WriteBodySectors(p_vol, p_file, &p_buf[*p_size_write], max_writable_size, &success_size);
        *p_size_write = success_size;
        if (err)
            return err;
    }

    return 0;
}

static s32 VFiPFFILE_Cursor_Write_Append(PF_VOLUME* p_vol, PF_FILE* p_file, u8* p_buf, u32 size, u32* p_size_write, u32* p_append_size) {
    s32 err;
    u32 sector;
    u32 append_size;
    u32 success_size;

    *p_size_write = 0;
    *p_append_size = 0;
    err = VFiPFCLUSTER_AppendCluster(p_file, size, &append_size, &sector);
    if (err)
        return err;
    p_file->cursor.sector = sector;
    if (!append_size)
        return 0;
    *p_append_size = append_size;
    err = VFiPFFILE_Cursor_WriteBodySectors(p_vol, p_file, &p_buf[*p_size_write], append_size, &success_size);
    *p_size_write += success_size;
    if (err)
        return err;
    return 0;
}

s32 VFiPFFILE_Cursor_Write(PF_FILE* p_file, u8* p_buf, u32 size, u32* p_size_write) {
    s32 err;
    PF_VOLUME* p_vol;
    u32 size_write;
    u32 size_request;
    u32 num_cluster;
    u32 append_size;

    *p_size_write = 0;
    if (-1 - p_file->cursor.position < size) {
        size = -1 - p_file->cursor.position;
        VFipf_vol_set.last_error = 37;
        p_file->p_sfd->ffd.p_vol->last_error = 37;
        p_file->last_error = 37;
    }
    p_vol = VFiPFFILE_GetVolume(p_file);
    VFiPFCLUSTER_SetLastAccessCluster(p_file);
    err = VFiPFFILE_Cursor_WriteHeadSector(p_vol, p_file, p_buf, size, &size_write);
    *p_size_write += size_write;
    if (err)
        return err;
    if (size_write >= size)
        return 0;
    size = size - size_write;
    err = VFiPFFAT_CountAllocatedClusters(&p_file->p_sfd->ffd, p_file->cursor.position + size, &num_cluster);
    if (err)
        return err;
    {
        if (p_file->cursor.position + size > num_cluster << (p_vol->bpb.log2_bytes_per_sector + p_vol->bpb.log2_sectors_per_cluster)) {
            size_request = p_file->cursor.position + size - (num_cluster << (p_vol->bpb.log2_bytes_per_sector + p_vol->bpb.log2_sectors_per_cluster));
            append_size = size - size_request;
        } else {
            size_request = 0;
            append_size = size;
        }
        while (append_size && append_size >= p_vol->bpb.bytes_per_sector) {
            err = VFiPFFILE_Cursor_Write_Overwrite(p_vol, p_file, &p_buf[*p_size_write], append_size, &size_write);
            *p_size_write += size_write;
            if (err)
                return err;
            append_size -= size_write;
            size -= size_write;
        }
        while (size_request && size >= p_vol->bpb.bytes_per_sector) {
            err = VFiPFFILE_Cursor_Write_Append(p_vol, p_file, &p_buf[*p_size_write], size_request, &size_write, &append_size);
            *p_size_write += size_write;
            if (err)
                return err;
            if (!append_size)
                return 0;
            size_request -= append_size;
            size -= size_write;
        }
        err = VFiPFFILE_Cursor_WriteTailSector(p_vol, p_file, &p_buf[*p_size_write], size, size_request, &size_write);
        *p_size_write += size_write;
        if (err)
            return err;
        else
            return 0;
    }
}

void VFiPFFILE_Cursor_MoveToClusterEnd(PF_FILE* p_file, u32 size) {
    PF_VOLUME* p_vol;
    u32 cluster;

    p_vol = VFiPFFILE_GetVolume(p_file);

    VFiPFFAT_CountAllocatedClusters(&p_file->p_sfd->ffd, size, &cluster);

    if (cluster > (0xFFFFFFFF >> (p_vol->bpb.log2_bytes_per_sector + p_vol->bpb.log2_sectors_per_cluster)))
        VFiPFFILE_Cursor_SetPosition(p_file, 0xFFFFFFFF);
    else
        VFiPFFILE_Cursor_SetPosition(p_file, cluster << (p_vol->bpb.log2_bytes_per_sector + p_vol->bpb.log2_sectors_per_cluster));
}

static inline u32 VFiPFFILE_Cursor_IsOutOfFile(PF_FILE* p_file) {
    return p_file->cursor.position >= p_file->p_sfd->dir_entry.file_size;
}

static void VFiPFFILE_InitSFD(PF_SFD* p_sfd, PF_DIR_ENT* p_dir_entry) {
    p_sfd->stat = 268435459;
    p_sfd->num_handlers = 1;
    p_sfd->dir_entry = *p_dir_entry;
    p_sfd->lock.mode = 0;
    p_sfd->lock.count = 0;
    p_sfd->lock.wcount = 0;
    p_sfd->lock.owner = 0;
    p_sfd->lock.resource = 0;
    VFiPFFAT_InitFFD(&p_sfd->ffd, 0, p_dir_entry->p_vol, &p_sfd->dir_entry.start_cluster);
}

PF_SFD* VFiPFFILE_GetSFD(PF_VOLUME* p_vol, PF_DIR_ENT* p_ent) {
    u32 sfd_num;
    PF_SFD* p_first_free_SFD;
    u32 i;

    sfd_num = 0;
    p_first_free_SFD = 0;
    for (i = 0; i < 5; ++i) {
        if ((p_vol->sfds[i].stat & 1) == 0 || ((p_vol->sfds[i].stat & 1) != 0 && ((p_vol->sfds[i].stat == 0) & 2) != 0)) {
            if (!p_first_free_SFD) {
                p_first_free_SFD = &p_vol->sfds[i];
                sfd_num = i;
            }
        } else {
            if (p_ent->p_vol == p_vol->sfds[i].dir_entry.p_vol && p_ent->entry_sector == p_vol->sfds[i].dir_entry.entry_sector &&
                p_ent->entry_offset == p_vol->sfds[i].dir_entry.entry_offset) {
                ++p_vol->sfds[i].num_handlers;
                return &p_vol->sfds[i];
            }
        }
    }
    if (!p_first_free_SFD)
        return 0;
    VFiPFFILE_InitSFD(p_first_free_SFD, p_ent);
    if ((p_vol->cluster_link.flag & 1) != 0) {
        p_first_free_SFD->ffd.cluster_link.buffer = &p_vol->cluster_link.buffer[sfd_num * p_vol->cluster_link.link_max];
        VFipf_memset(p_first_free_SFD->ffd.cluster_link.buffer, 0, 4 * p_vol->cluster_link.link_max);
        p_first_free_SFD->ffd.cluster_link.max_count = p_vol->cluster_link.link_max;
        p_first_free_SFD->ffd.cluster_link.interval = p_vol->cluster_link.interval;
        p_first_free_SFD->ffd.cluster_link.interval_offset = 0;
        p_first_free_SFD->ffd.cluster_link.position = 0;
        p_first_free_SFD->ffd.cluster_link.save_index = 0;
        if (p_ent->file_size) {
            if (p_first_free_SFD->ffd.cluster_link.max_count) {
                *p_first_free_SFD->ffd.cluster_link.buffer = p_ent->start_cluster;
                ++p_first_free_SFD->ffd.cluster_link.position;
            }
        }
    }
    return p_first_free_SFD;
}

void VFiPFFILE_Cursor_Initialize(PF_FILE* a1) {
    a1->cursor.sector = -1;
    VFiPFFILE_Cursor_SetPosition(a1, 0);
}

void VFiPFFILE_InitUFD(PF_FILE* p_file, u32 open_mode) {
    p_file->stat = 536870913;
    p_file->open_mode = open_mode;
    p_file->last_error = 0;
    p_file->lock_count = 0;
    VFiPFFAT_InitHint(&p_file->hint);
    VFiPFFILE_Cursor_Initialize(p_file);
    VFiPFCLUSTER_InitLastAccessCluster(p_file);
}

static PF_FILE* VFiPFFILE_GetFreeUFD(PF_VOLUME* p_vol) {
    int i;

    for (i = 0; i < 5; ++i) {
        if ((p_vol->ufds[i].stat & 1) == 0)
            return &p_vol->ufds[i];
    }
    return 0;
}

static int VFiPFFILE_ReleaseSFD(PF_SFD* p_sfd) {
    if (!--p_sfd->num_handlers) {
        p_sfd->stat &= ~1u;
        p_sfd->ffd.cluster_link.buffer = 0;
    }
    return 0;
}

static inline u32 VFiPFFILE_CheckUFD(PF_FILE* p_file) {
    u32 is_valid;

    is_valid = 1;
    if (&VFipf_vol_set > (PF_VOLUME_SET*)p_file || &VFipf_vol_set + 1 < (PF_VOLUME_SET*)p_file || (p_file->stat & 0x20000000) != 0x20000000)
        is_valid = 0;
    return is_valid;
}

s32 VFiPFFILE_createEmptyFile(PF_VOLUME* p_vol, PF_DIR_ENT* p_ent, PF_STR* p_fname, PF_DIR_ENT* p_parent_ent, u32 namelength) {
    int err;
    PF_FFD ffd;
    PF_FAT_HINT hint;
    u32 pos;
    u32 prev_chain[2];

    if (p_vol->num_opened_files >= 5) {
        return 21;
    }

    err = VFiPFENT_InitENT(p_ent, p_fname, 32, 1, p_parent_ent, p_vol);
    do {
        if (err)
            break;
        VFiPFFAT_InitFFD(&ffd, &hint, p_vol, &p_parent_ent->start_cluster);
        err = VFiPFENT_allocateEntry(p_ent, p_ent->num_entry_LFNs + 1, &ffd, prev_chain, p_fname, 0x77u, &pos);
        if (err)
            break;
        {
            p_ent->start_cluster = 0;
            if (p_ent->long_name[0]) {
                if ((VFipf_vol_set.setting & 2) == 2) {
                    VFiPFPATH_AdjustExtShortName(p_ent->short_name, pos);
                    p_ent->check_sum = VFiPFENT_CalcCheckSum(p_ent);
                }
            }
            err = VFiPFENT_UpdateEntry(p_ent, prev_chain, 0);
        }
    } while (0);
    return err;
}

static inline void VFiPFFILE_EmptyFile(PF_FFD* p_ffd, PF_DIR_ENT* p_ent) {
    if (p_ent->start_cluster >= 2 && p_ent->start_cluster != -1 && p_ent->file_size)
        VFiPFFAT_FreeChain(p_ffd, p_ent->start_cluster, -1, p_ent->file_size);
    p_ent->start_cluster = 0;
    p_ent->file_size = 0;
    VFiPFENT_getcurrentDateTimeForEnt(&p_ent->modify_date, &p_ent->modify_time);
    p_ent->access_date = p_ent->modify_date;
}

s32 VFiPFFILE_GetOpenedFile(PF_DIR_ENT* p_ent, PF_DIR_ENT** pp_open_ent);

u32 VFiPFFILE_IsOpened(PF_DIR_ENT* p_ent);

s32 VFiPFFILE_p_fopen(PF_VOLUME* p_vol, PF_STR* p_path_str, u32 mode, PF_FILE** pp_file) {
    s32 err;
    PF_DIR_ENT ent;
    PF_DIR_ENT parent_ent;
    PF_SFD* p_sfd;
    PF_FILE* p_file;
    u16 access_time;
    PF_STR file_str;
    PF_STR dir_str;
    PF_FFD ffd;
    PF_FAT_HINT hint;
    s32 namelength;

    *pp_file = 0;
    err = VFiPFENT_GetParentEntryOfPath(&parent_ent, p_vol, p_path_str);
    if (err)
        return err;
    if ((parent_ent.attr & 0x10) == 0)
        return 1;
    if (VFiPFPATH_SplitPath(p_path_str, &dir_str, &file_str))
        return 1;
    namelength = VFiPFSTR_StrNumChar(&file_str, 1);
    if (namelength > 0xFF)
        return 1;
    if ((mode & 2) != 0) {
        VFiPFFAT_InitFFD(&ffd, &hint, p_vol, &parent_ent.start_cluster);
        if (VFiPFENT_findEntry(&ffd, &ent, 0, &file_str, 103, 0, 0))
            return 3;
        if ((mode & 8) != 0 && (ent.attr & 1) != 0)
            return 10;
        VFiPFENT_getcurrentDateTimeForEnt(&ent.access_date, &access_time);
    } else {
        if (namelength + parent_ent.path_len > 0x103)
            return 1;
        err = VFiPFFILE_createEmptyFile(p_vol, &ent, &file_str, &parent_ent, namelength);
        if (err == 8) {
            if ((mode & 0x10) != 0)
                return 8;
            if ((mode & 1) != 0 && VFiPFFILE_IsOpened(&ent))
                return 8;
            if ((ent.attr & 1) != 0 && ((mode & 1) != 0 || (mode & 4) != 0 || (mode & 8) != 0))
                return 10;
            if ((ent.attr & 0x10) != 0)
                return 23;
            if ((mode & 1) != 0) {
                VFiPFFAT_InitFFD(&ffd, &hint, ent.p_vol, &ent.start_cluster);
                VFiPFFILE_EmptyFile(&ffd, &ent);
            } else {
                VFiPFENT_getcurrentDateTimeForEnt(&ent.access_date, &access_time);
            }
        } else if (err) {
            return err;
        }
    }
    p_vol = ent.p_vol;
    p_sfd = VFiPFFILE_GetSFD(p_vol, &ent);
    if (!p_sfd)
        return 21;
    p_file = VFiPFFILE_GetFreeUFD(p_vol);
    if (!p_file) {
        VFiPFFILE_ReleaseSFD(p_sfd);
        return 22;
    }
    {
        p_file->p_sfd = p_sfd;
        VFiPFFILE_InitUFD(p_file, mode);
        if ((mode & 1) != 0 && (p_vol->cache.mode & 1) == 0)
            p_file->p_sfd->stat |= 4u;
        if ((p_file->open_mode & 4) != 0)
            VFiPFFILE_Cursor_MoveToEnd(p_file);
        *pp_file = p_file;
        return 0;
    }
}

s32 VFiPFFILE_p_fread(PF_VOLUME* p_vol, u8* p_buf, u32 size, u32 count, PF_FILE* p_file, u32* p_count_read) {
    u32 base_pos;
    int err;
    u32 size_read;

    size_read = 0;
    *p_count_read = 0;
    if ((p_file->open_mode & 8) == 0 && ((p_file->open_mode & 1) != 0 || (p_file->open_mode & 4) != 0))
        return 10;
    if ((p_file->p_sfd->lock.mode & 3) != 0 && !p_file->lock_count)
        return 25;
    if (VFiPFFILE_Cursor_IsOutOfFile(p_file))
        return 28;
    VFiPFFAT_SetHint(&p_file->p_sfd->ffd, &p_file->hint);
    base_pos = p_file->cursor.position;
    err = VFiPFFILE_Cursor_Read(p_file, p_buf, size * count, &size_read);
    *p_count_read = size_read / size;
    if (*p_count_read != count)
        VFiPFFILE_Cursor_SetPosition(p_file, base_pos + *p_count_read * size);
    if (err)
        return err;
    else
        return 0;
}

int VFiPFFILE_p_fwrite(PF_VOLUME* p_vol, u8* p_buf, u32 size, u32 count, PF_FILE* p_file, u32* p_count_written) {
    s32 err;
    PF_CACHE_PAGE* p_page;
    u32 append_size;
    u32 size_written;
    u32 base_pos;
    u32 base_size;

    size_written = 0;
    *p_count_written = 0;
    if ((p_file->open_mode & 8) == 0 && (p_file->open_mode & 2) != 0)
        return 10;
    if ((p_file->p_sfd->lock.mode & 3) != 0 && ((p_file->p_sfd->lock.mode & 1) != 0 || (p_file->p_sfd->lock.mode & 2) != 0 && !p_file->lock_count)) {
        return 25;
    }
    if ((p_file->open_mode & 4) != 0)
        VFiPFFILE_Cursor_MoveToEnd(p_file);
    VFiPFFAT_SetHint(&p_file->p_sfd->ffd, &p_file->hint);
    if (p_file->cursor.position > p_file->p_sfd->dir_entry.file_size) {
        append_size = p_file->cursor.position - p_file->p_sfd->dir_entry.file_size;
        p_file->cursor.position = p_file->p_sfd->dir_entry.file_size;
        p_file->cursor.file_sector_index = p_file->cursor.position >> p_vol->bpb.log2_bytes_per_sector;
        p_file->cursor.offset_in_sector = p_file->cursor.position & (p_vol->bpb.bytes_per_sector - 1);
        err = VFiPFCACHE_AllocateDataPage(p_vol, -1, &p_page);
        if (err)
            return err;
        VFipf_memset(p_page->p_buf, 0, p_vol->bpb.bytes_per_sector);
        while (append_size) {
            if (append_size > p_vol->bpb.bytes_per_sector) {
                err = VFiPFFILE_Cursor_Write(p_file, p_page->p_buf, p_vol->bpb.bytes_per_sector, &size_written);
                if (err)
                    return err;
            } else {
                err = VFiPFFILE_Cursor_Write(p_file, p_page->p_buf, append_size, &size_written);
                if (err)
                    return err;
                break;
            }
            append_size -= p_vol->bpb.bytes_per_sector;
        }
        VFiPFCACHE_FreeDataPage(p_vol, p_page);
    }
    base_size = p_file->p_sfd->dir_entry.file_size;
    base_pos = p_file->cursor.position;
    err = VFiPFFILE_Cursor_Write(p_file, p_buf, size * count, &size_written);
    *p_count_written = size_written / size;
    if (*p_count_written != count) {
        if (p_file->p_sfd->dir_entry.file_size > base_size) {
            if (base_pos + *p_count_written * size > base_size)
                p_file->p_sfd->dir_entry.file_size = base_pos + *p_count_written * size;
            else
                p_file->p_sfd->dir_entry.file_size = base_size;
        }
        VFiPFFILE_Cursor_SetPosition(p_file, base_pos + *p_count_written * size);
    }
    if (err)
        return err;
    else
        return 0;
}

s32 VFiPFFILE_p_finfo(PF_FILE* p_file, PF_INFO* p_info) {
    PF_VOLUME* p_vol;
    u32 cluster_size;
    int result;
    u32 v7;

    p_vol = VFiPFFILE_GetVolume(p_file);
    VFiPFFAT_SetHint(&p_file->p_sfd->ffd, &p_file->hint);
    p_info->file_size = p_file->p_sfd->dir_entry.file_size;
    p_info->io_pointer = p_file->cursor.position;
    cluster_size = p_vol->bpb.bytes_per_sector << p_vol->bpb.log2_sectors_per_cluster;
    if (p_file->p_sfd->dir_entry.start_cluster) {
        result = VFiPFCLUSTER_GetAppendSize(p_file, &p_info->allocated_size);
        if (result)
            return result;
        if (p_file->p_sfd->dir_entry.file_size % cluster_size)
            v7 = cluster_size - p_file->p_sfd->dir_entry.file_size % cluster_size;
        else
            v7 = 0;
        p_info->empty_size = p_info->allocated_size + v7;
    } else {
        p_info->allocated_size = 0;
        p_info->empty_size = 0;
    }
    p_info->lock_mode = p_file->p_sfd->lock.mode & 3;
    p_info->lock_owner = p_file->p_sfd->lock.owner;
    p_info->lock_count = p_file->lock_count;
    p_info->lock_tcount = p_file->p_sfd->lock.count;
    return 0;
}

s32 VFiPFFILE_GetOpenedFile(PF_DIR_ENT* p_ent, PF_DIR_ENT** pp_open_ent) {
    int i;

    if (!p_ent || !pp_open_ent || !p_ent->p_vol)
        return 10;
    *pp_open_ent = 0;
    for (i = 0; i < 5; ++i) {
        if ((p_ent->p_vol->sfds[i].stat & 1) != 0 && (p_ent->p_vol->sfds[i].stat & 2) != 0 && p_ent->p_vol == p_ent->p_vol->sfds[i].dir_entry.p_vol &&
            p_ent->entry_sector == p_ent->p_vol->sfds[i].dir_entry.entry_sector &&
            p_ent->entry_offset == p_ent->p_vol->sfds[i].dir_entry.entry_offset) {
            *pp_open_ent = &p_ent->p_vol->sfds[i].dir_entry;
        }
    }
    return 0;
}

u32 VFiPFFILE_IsOpened(PF_DIR_ENT* p_ent) {
    PF_DIR_ENT* p_open_ent;
    u32 is_open;

    p_open_ent = NULL;
    if (p_ent == NULL)
        return 0;
    VFiPFFILE_GetOpenedFile(p_ent, &p_open_ent);
    is_open = p_open_ent != NULL;
    return is_open;
}

void VFiPFFILE_FinalizeSFD(PF_SFD* p_sfd) {
    p_sfd->stat &= 0xFFFFFFF8;
    VFiPFFAT_FinalizeFFD(&p_sfd->ffd);
}

void VFiPFFILE_FinalizeUFD(PF_FILE* p_file) {
    p_file->stat &= ~1u;
}

s32 VFiPFFILE_ReleaseUFD(PF_FILE* p_file) {
    p_file->stat &= ~1u;
    return 0;
}

void VFiPFFILE_FinalizeAllFiles(PF_VOLUME* p_vol) {
    u16 i;
    u16 j;

    for (i = 0; i < 5u; ++i)
        VFiPFFILE_FinalizeSFD(&p_vol->sfds[i]);
    for (j = 0; j < 5u; ++j)
        VFiPFFILE_FinalizeUFD(&p_vol->ufds[j]);
    p_vol->num_opened_files = 0;
}

static s32 VFiPFFILE_p_remove(PF_VOLUME* p_vol, PF_STR* p_path_str) {
    s32 err;
    PF_DIR_ENT ent;
    PF_ENT_ITER iter;
    PF_FAT_HINT hint;
    u32 start_cluster;

    err = VFiPFENT_ITER_GetEntryOfPath(&iter, &ent, p_vol, p_path_str, 0);
    if (err)
        return err;
    VFiPFFAT_InitFFD(&iter.ffd, &hint, p_vol, iter.ffd.p_start_cluster);
    if ((ent.attr & 0x19) != 0)
        return 11;
    if (VFiPFFILE_IsOpened(&ent))
        return 19;
    start_cluster = ent.start_cluster;
    err = VFiPFENT_RemoveEntry(&ent, &iter);
    if (err)
        return err;
    return VFiPFFAT_FreeChain(&iter.ffd, start_cluster, -1, ent.file_size);
}

s32 VFiPFFILE_remove(PF_STR* p_path_str) {
    int err;
    PF_VOLUME* VolumeFromPath;
    PF_VOLUME* p_vol;
    int v5;

    if (!p_path_str) {
        VFipf_vol_set.last_error = 10;
        return 10;
    } else {
        VolumeFromPath = VFiPFPATH_GetVolumeFromPath(p_path_str);
        p_vol = VolumeFromPath;
        if (!VolumeFromPath) {
            VFipf_vol_set.last_error = 10;
            return 10;
        } else {
            err = VFiPFVOL_CheckForWrite(VolumeFromPath);
            if (err) {
                VFipf_vol_set.last_error = err;
                p_vol->last_error = err;
            } else {
                p_vol->cache.signature = 0;
                err = VFiPFFILE_p_remove(p_vol, p_path_str);
                if (err) {
                    VFipf_vol_set.last_error = err;
                    p_vol->last_error = err;
                } else {
                    v5 = VFiPFCACHE_FlushDataCacheSpecific(p_vol, 0);
                    if (v5) {
                        VFipf_vol_set.last_error = v5;
                        p_vol->last_error = v5;
                    } else {
                        v5 = VFiPFCACHE_FlushFATCache(p_vol);
                        if (v5) {
                            VFipf_vol_set.last_error = v5;
                            p_vol->last_error = v5;
                        }
                    }
                    return v5;
                }
            }
        }
    }
    return err;
}

s32 VFiPFFILE_fopen(PF_STR* p_path_str, u32 mode, PF_FILE** pp_file) {
    int err;
    PF_VOLUME* VolumeFromPath;
    PF_VOLUME* p_vol;
    int v9;

    if (!pp_file) {
        VFipf_vol_set.last_error = 10;
        return 10;
    } else {
        *pp_file = 0;
        if (!p_path_str) {
            VFipf_vol_set.last_error = 10;
            return 10;
        } else {
            VolumeFromPath = VFiPFPATH_GetVolumeFromPath(p_path_str);
            p_vol = VolumeFromPath;
            if (!VolumeFromPath) {
                VFipf_vol_set.last_error = 10;
                return 10;
            } else {
                err = VFiPFVOL_CheckForRead(VolumeFromPath);
                if (err) {
                    VFipf_vol_set.last_error = err;
                    p_vol->last_error = err;
                } else if (VFiPFDRV_IsWProtected(p_vol) && mode != 2) {
                    VFipf_vol_set.last_error = 11;
                    p_vol->last_error = 11;
                    return 11;
                } else {
                    v9 = VFiPFFILE_p_fopen(p_vol, p_path_str, mode, pp_file);
                    if (v9) {
                        VFipf_vol_set.last_error = v9;
                        p_vol->last_error = v9;
                    } else {
                        ++p_vol->num_opened_files;
                    }
                    return v9;
                }
            }
        }
    }
    return err;
}

s32 VFiPFFILE_fclose(PF_FILE* p_file) {
    int err;
    int updated;
    int v4;
    int v5;
    PF_VOLUME* p_vol;

    if (!VFiPFFILE_CheckUFD(p_file)) {
        VFipf_vol_set.last_error = 10;
        return 10;
    } else {
        p_vol = VFiPFFILE_GetVolume(p_file);
        if (!p_vol) {
            VFipf_vol_set.last_error = 10;
            return 10;
        } else {
            err = VFiPFVOL_CheckForRead(p_vol);
            updated = err;
            if (err) {
                VFipf_vol_set.last_error = err;
                p_vol->last_error = err;
            } else if (VFiPFDRV_IsWProtected(p_vol) && p_file->open_mode != 2) {
                VFipf_vol_set.last_error = 11;
                p_vol->last_error = 11;
                return 11;
            } else if (!(p_file && p_file->p_sfd && (p_file->stat & 1) != 0 && (p_file->p_sfd->stat & 1) != 0 && (p_file->p_sfd->stat & 2) != 0)) {
                VFipf_vol_set.last_error = 10;
                p_vol->last_error = 10;
                return 10;
            } else if (!((p_file->p_sfd->stat & 1) != 0 && (p_file->p_sfd->stat & 2) != 0)) {
                VFipf_vol_set.last_error = 10;
                p_vol->last_error = 10;
                return 10;
            } else {
                p_vol->cache.signature = p_file;
                if (p_file->p_sfd->num_handlers - 1 <= 0 && (p_file->p_sfd->dir_entry.attr & 0x19) == 0 && !VFiPFDRV_IsWProtected(p_vol) &&
                    ((p_file->p_sfd->stat & 4) != 0 || (p_vol->file_config & 1) != 1)) {
                    updated = VFiPFENT_UpdateSFNEntry(&p_file->p_sfd->dir_entry, 1);
                }
                if (p_file->lock_count) {
                    if ((p_file->p_sfd->lock.mode & 1) != 0) {
                        p_file->p_sfd->lock.count -= p_file->lock_count;
                        p_file->lock_count = 0;
                        if (!p_file->p_sfd->lock.count)
                            VFiPF_UnLockFile(p_file);
                        p_file->p_sfd->lock.mode &= 0xFFFCu;
                    } else if (p_file->p_sfd->lock.owner != p_file) {
                        updated = 25;
                    } else {
                        p_file->p_sfd->lock.count = 0;
                        p_file->lock_count = 0;
                        p_file->p_sfd->lock.owner = 0;
                        VFiPF_UnLockFile(p_file);
                        p_file->p_sfd->lock.mode &= 0xFFFCu;
                    }
                }
                if (updated) {
                    VFipf_vol_set.last_error = updated;
                    p_file->p_sfd->ffd.p_vol->last_error = updated;
                    p_file->last_error = updated;
                } else {
                    if ((p_vol->cache.mode & 2) != 0) {
                        v4 = VFiPFCACHE_FlushFATCache(p_vol);
                        updated = v4;
                        if (v4) {
                            VFipf_vol_set.last_error = v4;
                            p_file->p_sfd->ffd.p_vol->last_error = v4;
                            p_file->last_error = v4;
                        } else {
                            v5 = VFiPFCACHE_FlushDataCacheSpecific(p_vol, p_file);
                            updated = v5;
                            if (v5) {
                                VFipf_vol_set.last_error = v5;
                                p_file->p_sfd->ffd.p_vol->last_error = v5;
                                p_file->last_error = v5;
                            }
                        }
                    }
                    if (!updated) {
                        VFiPFFILE_ReleaseSFD(p_file->p_sfd);
                        VFiPFFILE_ReleaseUFD(p_file);
                        --p_vol->num_opened_files;
                    }
                }
                p_vol->cache.signature = 0;
                return updated;
            }
        }
    }
    return err;
}

s32 VFiPFFILE_fread(u8* p_buf, u32 size, u32 count, struct PF_FILE* p_file, u32* p_count_read) {
    int err;
    PF_VOLUME* p_vol;
    u32 count_read;

    if (!p_count_read) {
        VFipf_vol_set.last_error = 10;
        return 10;
    } else {
        *p_count_read = 0;
        if (!VFiPFFILE_CheckUFD(p_file)) {
            VFipf_vol_set.last_error = 10;
            return 10;
        } else {
            p_vol = VFiPFFILE_GetVolume(p_file);
            if (!p_vol) {
                VFipf_vol_set.last_error = 38;
                p_file->p_sfd->ffd.p_vol->last_error = 38;
                p_file->last_error = 38;
                return 38;
            } else {
                err = VFiPFVOL_CheckForRead(p_vol);
                if (err) {
                    VFipf_vol_set.last_error = err;
                    p_vol->last_error = err;
                } else if (!(p_file && p_file->p_sfd && (p_file->stat & 1) != 0 && (p_file->p_sfd->stat & 1) != 0 &&
                             (p_file->p_sfd->stat & 2) != 0)) {
                    VFipf_vol_set.last_error = 38;
                    p_vol->last_error = 38;
                    return 38;
                } else if (!((p_file->p_sfd->stat & 1) != 0 && (p_file->p_sfd->stat & 2) != 0)) {
                    VFipf_vol_set.last_error = 38;
                    p_vol->last_error = 38;
                    return 38;
                } else if (!(p_buf && count && size)) {
                    VFipf_vol_set.last_error = 10;
                    p_file->p_sfd->ffd.p_vol->last_error = 10;
                    p_file->last_error = 10;
                    return 10;
                } else {
                    p_vol->cache.signature = p_file;
                    err = VFiPFFILE_p_fread(p_vol, p_buf, size, count, p_file, &count_read);
                    *p_count_read = count_read;
                    p_vol->cache.signature = 0;
                    if (err) {
                        VFipf_vol_set.last_error = err;
                        p_file->p_sfd->ffd.p_vol->last_error = err;
                        p_file->last_error = err;
                    }
                }
            }
        }
    }
    return err;
}

s32 VFiPFFILE_fwrite(u8* p_buf, u32 size, u32 count, PF_FILE* p_file, u32* p_count_written) {
    int result;
    PF_VOLUME* p_vol;
    int v12;
    int err;
    u32 count_written;

    if (!p_count_written) {
        VFipf_vol_set.last_error = 10;
        return 10;
    } else {
        *p_count_written = 0;
        if (!VFiPFFILE_CheckUFD(p_file)) {
            VFipf_vol_set.last_error = 10;
            return 10;
        } else {
            p_vol = VFiPFFILE_GetVolume(p_file);
            if (!p_vol) {
                VFipf_vol_set.last_error = 38;
                p_file->p_sfd->ffd.p_vol->last_error = 38;
                p_file->last_error = 38;
                return 38;
            } else {
                result = VFiPFVOL_CheckForWrite(p_vol);
                if (result) {
                    VFipf_vol_set.last_error = result;
                    p_vol->last_error = result;
                } else if (!(p_file && p_file->p_sfd && (p_file->stat & 1) != 0 && (p_file->p_sfd->stat & 1) != 0 &&
                             (p_file->p_sfd->stat & 2) != 0)) {
                    VFipf_vol_set.last_error = 38;
                    p_vol->last_error = 38;
                    return 38;
                } else if (!((p_file->p_sfd->stat & 1) != 0 && (p_file->p_sfd->stat & 2) != 0)) {
                    VFipf_vol_set.last_error = 38;
                    p_vol->last_error = 38;
                    return 38;
                } else if (!(p_buf && count && size)) {
                    VFipf_vol_set.last_error = 10;
                    p_file->p_sfd->ffd.p_vol->last_error = 10;
                    p_file->last_error = 10;
                    return 10;
                } else {
                    p_vol->cache.signature = p_file;
                    v12 = VFiPFFILE_p_fwrite(p_vol, p_buf, size, count, p_file, &count_written);
                    err = v12;
                    *p_count_written = count_written;
                    p_vol->cache.signature = 0;
                    if (v12) {
                        VFipf_vol_set.last_error = v12;
                        p_file->p_sfd->ffd.p_vol->last_error = v12;
                        p_file->last_error = v12;
                    }
                    if (count_written) {
                        VFiPFENT_getcurrentDateTimeForEnt(&p_file->p_sfd->dir_entry.modify_date, &p_file->p_sfd->dir_entry.modify_time);
                        p_file->p_sfd->dir_entry.access_date = p_file->p_sfd->dir_entry.modify_date;
                        if ((p_vol->cache.mode & 1) != 0)
                            err = VFiPFENT_UpdateSFNEntry(&p_file->p_sfd->dir_entry, 1);
                        else
                            p_file->p_sfd->stat |= 4u;
                    }
                    return err;
                }
            }
        }
    }
    return result;
}

s32 VFiPFFILE_fseek(PF_FILE* p_file, s32 lOffset, s32 nOrigin) {
    int err;
    PF_VOLUME* p_vol;
    u32 wk_offset;
    u32 file_io;

    if (!VFiPFFILE_CheckUFD(p_file)) {
        VFipf_vol_set.last_error = 10;
        return 10;
    }
    p_vol = VFiPFFILE_GetVolume(p_file);
    if (!p_vol) {
        VFipf_vol_set.last_error = 38;
        return 38;
    }
    err = VFiPFVOL_CheckForRead(p_vol);
    if (err) {
        VFipf_vol_set.last_error = err;
        p_vol->last_error = err;
        return err;
    }
    if (!p_file || !p_file->p_sfd || (p_file->stat & 1) == 0 || (p_file->p_sfd->stat & 1) == 0 || (p_file->p_sfd->stat & 2) == 0) {
        VFipf_vol_set.last_error = 38;
        p_vol->last_error = 38;
        return 38;
    }
    if ((p_file->p_sfd->stat & 1) == 0 || (p_file->p_sfd->stat & 2) == 0) {
        VFipf_vol_set.last_error = 38;
        p_vol->last_error = 38;
        return 38;
    }
    switch (nOrigin) {
    case 1:
        file_io = p_file->cursor.position;
        break;
    case 0:
        file_io = 0;
        break;
    case 2:
        file_io = p_file->p_sfd->dir_entry.file_size;
        break;
    default:
        VFipf_vol_set.last_error = 10;
        p_file->p_sfd->ffd.p_vol->last_error = 10;
        p_file->last_error = 10;
        return 10;
    }
    if ((lOffset & 0x80000000) != 0) {
        wk_offset = (~lOffset & 0x7FFFFFFF) + 1;
        if (file_io < wk_offset) {
            VFipf_vol_set.last_error = 10;
            p_file->p_sfd->ffd.p_vol->last_error = 10;
            p_file->last_error = 10;
            return 10;
        }
        file_io = file_io - wk_offset;
    } else {
        if (lOffset > -1 - file_io) {
            VFipf_vol_set.last_error = 37;
            p_file->p_sfd->ffd.p_vol->last_error = 37;
            p_file->last_error = 37;
            return 37;
        }
        file_io = file_io + lOffset;
    }
    VFiPFFILE_Cursor_Initialize(p_file);
    VFiPFFILE_Cursor_SetPosition(p_file, file_io);
    return err;
}

s32 VFiPFFILE_finfo(PF_FILE* p_file, PF_INFO* p_info) {
    int result;
    PF_VOLUME* p_vol;
    int err;
    PF_FAT_HINT hint;
    PF_CURSOR save_cursor;

    if (!VFiPFFILE_CheckUFD(p_file)) {
        VFipf_vol_set.last_error = 10;
        return 10;
    } else {
        p_vol = VFiPFFILE_GetVolume(p_file);
        if (!p_vol) {
            VFipf_vol_set.last_error = 38;
            return 38;
        } else {
            result = VFiPFVOL_CheckForRead(p_vol);
            if (result) {
                VFipf_vol_set.last_error = result;
                p_file->p_sfd->ffd.p_vol->last_error = result;
                p_file->last_error = result;
            } else if (!(p_file && p_file->p_sfd && (p_file->stat & 1) != 0 && (p_file->p_sfd->stat & 1) != 0 && (p_file->p_sfd->stat & 2) != 0)) {
                VFipf_vol_set.last_error = 38;
                p_vol->last_error = 38;
                return 38;
            } else if (!((p_file->p_sfd->stat & 1) != 0 && (p_file->p_sfd->stat & 2) != 0)) {
                VFipf_vol_set.last_error = 38;
                p_vol->last_error = 38;
                return 38;
            } else if (!p_info) {
                VFipf_vol_set.last_error = 10;
                p_file->p_sfd->ffd.p_vol->last_error = 10;
                p_file->last_error = 10;
                return 10;
            } else {
                p_vol->cache.signature = p_file;
                save_cursor = p_file->cursor;
                hint = p_file->hint;
                err = VFiPFFILE_p_finfo(p_file, p_info);
                p_file->cursor = save_cursor;
                p_file->hint = hint;
                p_vol->cache.signature = 0;
                if (err) {
                    VFipf_vol_set.last_error = err;
                    p_file->p_sfd->ffd.p_vol->last_error = err;
                    p_file->last_error = err;
                    return err;
                } else {
                    return 0;
                }
            }
        }
    }
    return result;
}
