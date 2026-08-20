#include "revolution/vf/pf_volume.h"
#include "revolution/vf/pf_cache.h"
#include "revolution/vf/pf_clib.h"
#include "revolution/vf/pf_cp932.h"
#include "revolution/vf/pf_dir.h"
#include "revolution/vf/pf_driver.h"
#include "revolution/vf/pf_entry.h"
#include "revolution/vf/pf_fat.h"
#include "revolution/vf/pf_file.h"
#include "revolution/vf/pf_filelock.h"
#include "revolution/vf/pf_sector.h"
#include "revolution/vf/pf_system.h"

static u32 VFiPFVOL_CheckContextRegistered(s32 context_id) {
    u32 i;
    for (i = 1; i < 1; i++) {
        VFipf_vol_set.context[i].stat &= 1;
        if (VFipf_vol_set.context[i].stat != 0 && VFipf_vol_set.context[i].context_id == context_id) {
            return 1;
        }
    }
    return 0;
}

static s32 VFiPFVOL_SetUpVolumeForMount(PF_VOLUME* p_vol) {
    return 0;
}

static s32 VFiPFVOL_InitCurrentDir(PF_VOLUME* p_vol) {
    int err;
    int i;
    s32 context_id;

    if ((p_vol->flags & 2) == 0) {
        return 9;
    }

    VFiPFSYS_GetCurrentContextID(&context_id);
    p_vol->current_dir[0].stat |= 1u;

    for (i = 0; i < 1; i++) {
        err = VFiPFENT_GetRootDir(p_vol, &p_vol->current_dir[i].directory);
        if (err) {
            return err;
        }
    }

    return 0;
}

static s32 VFiPFVOL_FinalizeCurrentDir(PF_VOLUME* p_vol) {
    u32 i;

    if ((p_vol->flags & 2) == 0) {
        return 9;
    }
    for (i = 0; i < 1; i++) {
        p_vol->current_dir[i].stat = 0;
    }

    return 0;
}

s32 VFiPFVOL_DoMountVolume(PF_VOLUME* p_vol) {
    int result;
    int err;

    VFiPFVOL_SetUpVolumeForMount(p_vol);
    result = VFiPFDRV_mount(p_vol);
    if (result) {
        return result;
    }

    if (!p_vol->bpb.bytes_per_sector || (p_vol->bpb.bytes_per_sector & 0x1FF) != 0) {
        return 15;
    }

    result = VFiPFCACHE_InitCaches(p_vol);
    if (result) {
        return err;
    }

    p_vol->flags |= 2u;
    err = VFiPFVOL_InitCurrentDir(p_vol);
    if (!err && (p_vol->flags & 0x10) != 0) {
        err = VFiPFDRV_format(p_vol, p_vol->format_param);
        if (!err && (p_vol->flags & 0x20) == 0) {
            err = VFiPFFAT_InitFATRegion(p_vol);
            if (!err)
                err = VFiPFENT_MakeRootDir(p_vol);
        }
    }
    if (err)
        p_vol->flags &= ~2u;
    return err;
}

static s32 VFiPFVOL_DoUnmountVolume(PF_VOLUME* p_vol, u32 mode) {
    int err;

    err = VFiPFDRV_unmount(p_vol, mode);
    if (err) {
        return err;
    }

    VFiPFVOL_FinalizeCurrentDir(p_vol);
    p_vol->flags &= ~2u;
    return 0;
}

static void VFiPFVOL_UnmountVolumeByEject(PF_VOLUME* p_vol) {
    VFiPFFILE_FinalizeAllFiles(p_vol);
    VFiPFDIR_FinalizeAllDirs(p_vol);
    VFiPFCACHE_FreeAllCaches(p_vol);
    VFiPFVOL_DoUnmountVolume(p_vol, 1);
    VFipf_vol_set.num_mounted_volumes--;
}

static s32 VFiPFVOL_p_attach(PF_VOLUME* p_vol, PF_DRV_TBL* p_drv, s16 vol_idx) {
    s32 err;

    VFipf_memset(p_vol, 0, sizeof(PF_VOLUME));
    p_vol->num_free_clusters = -1;
    p_vol->last_free_cluster = -1;
    p_vol->p_part = p_drv->p_part;
    p_vol->drv_char = vol_idx + 'A';
    p_vol->tail_entry.tracker_size = 1;
    p_vol->tail_entry.tracker_bits = p_vol->tail_entry.tracker_buff;

    err = VFiPFDRV_init(p_vol);
    if (err != 0) {
        return err;
    }

    VFiPFCACHE_SetCache(p_vol, p_drv->cache->pages, p_drv->cache->buffers, p_drv->cache->num_fat_pages, p_drv->cache->num_data_pages);
    VFiPFCACHE_SetFATBufferSize(p_vol, p_drv->cache->num_fat_buf_size);
    VFiPFCACHE_SetDataBufferSize(p_vol, p_drv->cache->num_data_buf_size);
    return 0;
}

static s32 VFiPFVOL_p_detach(PF_VOLUME* p_vol) {
    s32 err;
    err = VFiPFDRV_finalize(p_vol);
    if (err) {
        return err;
    }
    return 0;
}

static s32 VFiPFVOL_p_mount(PF_VOLUME* p_vol) {
    s32 err;

    if ((p_vol->flags & 2) == 0) {
        err = VFiPFVOL_DoMountVolume(p_vol);
        if (err != 0) {
            return err;
        }

        p_vol->fsi_flag &= ~7;
        VFipf_vol_set.num_mounted_volumes++;
    }
    return 0;
}

s32 VFiPFVOL_p_unmount(PF_VOLUME* p_vol, u32 mode) {
    s32 err;
    s32 err2;

    err = 0;
    if ((p_vol->flags & 2) == 0) {
        return 9;
    }

    VFiPFFILE_FinalizeAllFiles(p_vol);
    VFiPFDIR_FinalizeAllDirs(p_vol);
    err2 = VFiPFCACHE_FlushAllCaches(p_vol);
    if (err2 != 0) {
        err = err2;
    }

    if (err2 == 0 || (mode & 1) != 0) {
        VFiPFCACHE_FreeAllCaches(p_vol);
        err2 = VFiPFVOL_DoUnmountVolume(p_vol, mode);
        if (err2 != 0 && err == 0) {
            err = err2;
        }
    }

    if (err == 0 || (mode & 1) != 0) {
        VFipf_vol_set.num_mounted_volumes--;
    }

    return err;
}

static s32 VFiPFVOL_CheckMediaInsert(PF_VOLUME* p_vol) {
    s32 err;

    if (VFiPFDRV_IsInserted(p_vol) != 0) {
        if (VFiPFDRV_IsDetected(p_vol) != 0) {
            if ((p_vol->flags & 2) != 0) {
                VFiPFVOL_UnmountVolumeByEject(p_vol);
            }
            err = VFiPFVOL_p_mount(p_vol);
            if (err != 0) {
                return err;
            }
        }
    } else {
        if ((p_vol->flags & 2) != 0) {
            if (VFiPFDRV_IsDetected(p_vol) != 0) {
                VFiPFVOL_UnmountVolumeByEject(p_vol);
            }
            p_vol->flags &= ~2;
        }
    }
    return 0;
}

s32 VFiPFVOL_InitModule(u32 config, void* param) {
    s32 vol_idx;

    if ((config & 0xFFFCFFFF) != 0) {
        VFipf_vol_set.last_error = 10;
        return 10;
    }
    if ((config & 0x30000) == 0x30000) {
        VFipf_vol_set.last_error = 10;
        return 10;
    }

    if ((config & 0x10000) != 0) {
        VFipf_vol_set.config |= 0x10000;
    } else {
        VFipf_vol_set.config &= ~0x10000;
    }

    VFipf_vol_set.current_vol[0].p_vol = &VFipf_vol_set.volumes[0];
    VFipf_vol_set.current_vol[0].stat |= 1;
    VFipf_vol_set.num_attached_drives = 0;
    VFipf_vol_set.num_mounted_volumes = 0;

    if ((config & 0x10000) != 0) {
        VFipf_vol_set.config |= 0x10000;
    } else {
        VFipf_vol_set.config &= ~0x10000;
    }

    VFipf_vol_set.param = param;
    VFipf_vol_set.last_error = 0;
    VFipf_vol_set.last_driver_error = 0;
    VFipf_vol_set.setting = 1;

    VFipf_vol_set.codeset.oem2unicode = &VFiPFCODE_CP932_OEM2Unicode;
    VFipf_vol_set.codeset.unicode2oem = &VFiPFCODE_CP932_Unicode2OEM;
    VFipf_vol_set.codeset.oem_char_width = &VFiPFCODE_CP932_OEMCharWidth;
    VFipf_vol_set.codeset.is_oem_mb_char = &VFiPFCODE_CP932_isOEMMBchar;
    VFipf_vol_set.codeset.unicode_char_width = &VFiPFCODE_CP932_UnicodeCharWidth;
    VFipf_vol_set.codeset.is_unicode_mb_char = &VFiPFCODE_CP932_isUnicodeMBchar;

    for (vol_idx = 0; vol_idx < 26; vol_idx++) {
        VFipf_memset(&VFipf_vol_set.volumes[vol_idx], 0, sizeof(PF_VOLUME));
    }

    VFiPF_InitLockFile();

    return 0;
}

s32 VFiPFVOL_CheckForRead(PF_VOLUME* p_vol) {
    s32 err;

    if (p_vol == 0) {
        return 10;
    }

    err = VFiPFVOL_CheckMediaInsert(p_vol);
    if (err != 0) {
        return err;
    }

    if (!(p_vol->flags & 2)) {
        return 9;
    }

    return 0;
}

s32 VFiPFVOL_CheckForWrite(PF_VOLUME* p_vol) {
    s32 err;

    if (p_vol == 0) {
        return 10;
    }

    err = VFiPFVOL_CheckMediaInsert(p_vol);
    if (err != 0) {
        return err;
    }

    if ((p_vol->flags & 2) == 0) {
        return 9;
    }

    if (VFiPFDRV_IsWProtected(p_vol)) {
        return 11;
    }

    return 0;
}

s32 VFiPFVOL_GetCurrentDir(PF_VOLUME* p_vol, PF_DIR_ENT* p_current_dir) {
    s32 err;
    s32 context_id;
    u32 i;

    if (p_vol == 0) {
        err = 10;
        return err;
    }
    if ((p_vol->flags & 2) == 0) {
        err = 9;
        return err;
    }

    VFiPFSYS_GetCurrentContextID(&context_id);

    for (i = 1; i < 1; i++) {
        if ((p_vol->current_dir[i].stat & 1) != 0 && p_vol->current_dir[i].context_id == context_id) {
            *p_current_dir = p_vol->current_dir[i].directory;
            break;
        }
    }

    if (i == 1) {
        if (VFiPFVOL_CheckContextRegistered(context_id)) {
            for (i = 1; i < 1; i++) {
                if ((p_vol->current_dir[i].stat & 1) == 0) {
                    p_vol->current_dir[i].stat |= 1;
                    p_vol->current_dir[i].context_id = context_id;
                    *p_current_dir = p_vol->current_dir[i].directory;
                    break;
                }
            }
        } else {
            *p_current_dir = p_vol->current_dir[0].directory;
        }
    }

    return 0;
}

void VFiPFVOL_SetCurrentVolume(PF_VOLUME* p_vol) {
    s32 context_id;
    u32 i;

    VFiPFSYS_GetCurrentContextID(&context_id);
    for (i = 1; i < 1; i++) {
        if ((VFipf_vol_set.current_vol[i].stat & 1) != 0 && VFipf_vol_set.current_vol[i].context_id == context_id) {
            VFipf_vol_set.current_vol[i].p_vol = p_vol;
            break;
        }
    }

    if (i == 1) {
        if (VFiPFVOL_CheckContextRegistered(context_id)) {
            for (i = 1; i < 1; i++) {
                if ((VFipf_vol_set.current_vol[i].stat & 1) == 0) {
                    VFipf_vol_set.current_vol[i].stat |= 1;
                    VFipf_vol_set.current_vol[i].context_id = context_id;
                    VFipf_vol_set.current_vol[i].p_vol = p_vol;
                    break;
                }
            }
        }
    }
    VFipf_vol_set.current_vol[0].p_vol = p_vol;
}

PF_VOLUME* VFiPFVOL_GetCurrentVolume() {
    PF_VOLUME* p_vol;
    s32 context_id;
    u32 i;

    p_vol = 0;
    VFiPFSYS_GetCurrentContextID(&context_id);

    for (i = 1; i < 1; i++) {
        if ((VFipf_vol_set.current_vol[i].stat & 1) != 0 && VFipf_vol_set.current_vol[i].context_id == context_id) {
            p_vol = VFipf_vol_set.current_vol[i].p_vol;
            break;
        }
    }

    if (i == 1) {
        if (VFiPFVOL_CheckContextRegistered(context_id)) {
            for (i = 1; i < 1; i++) {
                if ((VFipf_vol_set.current_vol[i].stat & 1) == 0) {
                    VFipf_vol_set.current_vol[i].stat |= 1;
                    VFipf_vol_set.current_vol[i].context_id = context_id;
                    p_vol = VFipf_vol_set.current_vol[i].p_vol;
                    break;
                }
            }
        } else {
            p_vol = VFipf_vol_set.current_vol[0].p_vol;
        }
    }
    return p_vol;
}

PF_VOLUME* VFiPFVOL_GetVolumeFromDrvChar(s8 drv_char) {
    s16 vol_idx;

    vol_idx = VFipf_toupper(drv_char) - 'A';
    if (vol_idx < 0 || vol_idx >= 26) {
        return 0;
    }

    return &VFipf_vol_set.volumes[vol_idx];
}

void VFiPFVOL_LoadVolumeLabelFromBuf(const u8* buf, PF_VOLUME* p_vol) {
    VFipf_memcpy(p_vol->label, (void*)buf, 11);
    p_vol->label[11] = '\0';
}

s32 VFiPFVOL_errnum() {
    return VFipf_vol_set.last_error;
}

s32 VFiPFVOL_getdev(s8 drv_char, PF_DEV_INF* dev_inf) {
    PF_VOLUME* p_vol;
    s32 err;

    p_vol = VFiPFVOL_GetVolumeFromDrvChar(drv_char);
    if (p_vol == 0) {
        VFipf_vol_set.last_error = 10;
        return 10;
    }

    if (dev_inf == 0) {
        VFipf_vol_set.last_error = 10;
        p_vol->last_error = 10;
        return 10;
    }

    err = VFiPFVOL_CheckForRead(p_vol);
    if (err != 0) {
        VFipf_vol_set.last_error = err;
        p_vol->last_error = err;
        return err;
    }

    dev_inf->bps = p_vol->bpb.bytes_per_sector;
    dev_inf->spc = p_vol->bpb.sectors_per_cluster;
    dev_inf->cls = p_vol->bpb.num_clusters;

    err = VFiPFFAT_CountFreeClusters(p_vol, &dev_inf->ecl);
    if (err != 0) {
        VFipf_vol_set.last_error = err;
        p_vol->last_error = err;
        return err;
    }

    return 0;
}

s32 VFiPFVOL_attach(PF_DRV_TBL* p_drv) {
    s16 vol_idx;
    PF_VOLUME* p_vol;
    s8 drv_char;
    s32 err;

    vol_idx = 0;

    if (p_drv == 0) {
        VFipf_vol_set.last_error = 10;
        return 10;
    }

    drv_char = p_drv->drive;
    p_drv->stat = 0;
    p_drv->drive = 0;

    if (VFipf_vol_set.num_attached_drives < 0 || VFipf_vol_set.num_attached_drives >= 26) {
        VFipf_vol_set.last_error = 4;
        return 4;
    }

    if (p_drv == 0 || p_drv->p_part == 0 || p_drv->cache == 0 || p_drv->cache->num_fat_buf_size > 0xFFFF ||
        p_drv->cache->num_data_buf_size > 0x7FFF || p_drv->cache->num_fat_pages < 1 || p_drv->cache->num_data_pages < 2 || p_drv->cache->pages == 0 ||
        p_drv->cache->buffers == 0 || ((u32)p_drv->cache->pages & 3) != 0 || ((u32)p_drv->cache->buffers & 3) != 0) {
        VFipf_vol_set.last_error = 10;
        return 10;
    }

    if (p_drv->cache->num_fat_buf_size == 0) {
        p_drv->cache->num_fat_buf_size = 1;
    }
    if (p_drv->cache->num_data_buf_size == 0) {
        p_drv->cache->num_data_buf_size = 1;
    }

    if (p_drv->cache->num_fat_pages / p_drv->cache->num_fat_buf_size < 1) {
        VFipf_vol_set.last_error = 10;
        return 10;
    }

    if (p_drv->cache->num_data_pages / p_drv->cache->num_data_buf_size < 2) {
        VFipf_vol_set.last_error = 10;
        return 10;
    }

    if (drv_char != 0) {
        if ((drv_char < 'a' || drv_char > 'z') && (drv_char < 'A' || drv_char > 'Z')) {
            VFipf_vol_set.last_error = 10;
            return 10;
        }
        vol_idx = VFipf_toupper(drv_char) - 'A';

        if (vol_idx < 0 || vol_idx >= 26) {
            VFipf_vol_set.last_error = 18;
            return 18;
        }
        p_vol = &VFipf_vol_set.volumes[vol_idx];
        if ((p_vol->flags & 1) != 0) {
            VFipf_vol_set.last_error = 18;
            return 18;
        }
    } else {
        for (; vol_idx < 26; vol_idx++) {
            p_vol = &VFipf_vol_set.volumes[vol_idx];
            if ((p_vol->flags & 1) == 0) {
                break;
            }
        }
        if (vol_idx < 0 || vol_idx >= 26) {
            VFipf_vol_set.last_error = 4;
            return 4;
        }
    }

    err = VFiPFVOL_p_attach(p_vol, p_drv, vol_idx);
    if (err != 0) {
        VFipf_vol_set.last_error = err;
        return err;
    }

    p_drv->stat |= 1;
    p_drv->drive = p_vol->drv_char;
    VFipf_vol_set.num_attached_drives++;

    err = VFiPFVOL_CheckMediaInsert(p_vol);
    if (err != 0) {
        if (VFiPFDRV_IsInserted(p_vol)) {
            p_drv->stat |= 4;
        }
        VFipf_vol_set.last_error = err;
        p_vol->last_error = err;
        return 0;
    }

    if (VFiPFDRV_IsInserted(p_vol)) {
        p_drv->stat |= 4;

        if ((p_vol->flags & 2) == 0) {
            err = VFiPFVOL_DoMountVolume(p_vol);
            if (err == 0) {
                p_vol->fsi_flag &= ~7;
                VFipf_vol_set.num_mounted_volumes++;
            }
        } else {
            err = 0;
        }

        if (err != 0) {
            VFipf_vol_set.last_error = err;
            p_vol->last_error = err;
            return 0;
        }

        p_drv->stat |= 2;
    }

    return 0;
}

s32 VFiPFVOL_detach(s8 drv_char) {
    PF_VOLUME* p_vol;
    s32 err;

    p_vol = VFiPFVOL_GetVolumeFromDrvChar(drv_char);
    if (p_vol == 0) {
        VFipf_vol_set.last_error = 10;
        return 10;
    }

    if ((p_vol->flags & 2) != 0) {
        VFipf_vol_set.last_error = 10;
        p_vol->last_error = 10;
        return 10;
    }

    if ((p_vol->flags & 1) == 0) {
        VFipf_vol_set.last_error = 10;
        p_vol->last_error = 10;
        return 10;
    }

    err = VFiPFVOL_p_detach(p_vol);
    if (err) {
        VFipf_vol_set.last_error = err;
        p_vol->last_error = err;
        return err;
    }

    p_vol->flags &= ~1;
    VFipf_vol_set.num_attached_drives--;

    return 0;
}

s32 VFiPFVOL_unmount(s8 drv_char, u32 mode) {
    PF_VOLUME* p_vol;
    s32 err;
    s32 err2;

    err2 = 0;
    p_vol = VFiPFVOL_GetVolumeFromDrvChar(drv_char);
    if (p_vol == 0) {
        VFipf_vol_set.last_error = 10;
        return 10;
    } else if ((p_vol->flags & 1) == 0) {
        VFipf_vol_set.last_error = 10;
        return 10;
    } else if (mode != 0 && (mode & 1) == 0) {
        VFipf_vol_set.last_error = 10;
        p_vol->last_error = 10;
        return 10;
    } else {
        if (((p_vol->flags >> 1) & 1) & VFiPFDRV_IsDetected(p_vol)) {
            VFiPFVOL_UnmountVolumeByEject(p_vol);
        }

        if ((p_vol->flags & 2) == 0) {
            return 0;
        } else {
            if (p_vol->num_opened_files != 0 || p_vol->num_opened_directories != 0) {
                err2 = 1;
                VFipf_vol_set.last_error = 19;
                p_vol->last_error = 19;
                if ((mode & 1) == 0) {
                    return 19;
                }
            }

            err = VFiPFVOL_p_unmount(p_vol, mode);
            if (err != 0 && err2 == 0) {
                VFipf_vol_set.last_error = err;
                p_vol->last_error = err;
                err2 = 1;
                if ((mode & 1) == 0) {
                    err2 = err;
                }
            }
        }
    }

    return err2;
}
