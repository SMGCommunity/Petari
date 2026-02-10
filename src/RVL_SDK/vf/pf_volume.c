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
    int i;

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

s32 VFiPFVOL_p_unmount(PF_VOLUME* p_vol, u32 mode) {
    int err2;
    int err;
    int v7;

    err2 = 0;
    if ((p_vol->flags & 2) == 0) {
        return 9;
    }

    VFiPFFILE_FinalizeAllFiles(p_vol);
    VFiPFDIR_FinalizeAllDirs(p_vol);
    err = VFiPFCACHE_FlushAllCaches(p_vol);
    if (!err || (err2 = err, ((mode & 1) != 0))) {
        VFiPFCACHE_FreeAllCaches(p_vol);
        v7 = VFiPFVOL_DoUnmountVolume(p_vol, mode);
        if (v7) {
            if (!err2)
                err2 = v7;
        }
    }

    if (!err2 || (mode & 1) != 0) {
        --VFipf_vol_set.num_mounted_volumes;
    }

    return err2;
}

static s32 VFiPFVOL_p_setcode(PF_CHARCODE* p_codeset) {
    VFipf_vol_set.codeset = *p_codeset;
    return 0;
}

s32 VFiPFVOL_InitModule(u32 config, void* param) {
    int result;
    int i;
    int v4;
    int vol_idx;
    PF_CHARCODE codeset;

    if ((config & 0xFFFCFFFF) != 0) {
        result = 10;
        VFipf_vol_set.last_error = 10;
    } else if ((config & 0x30000) == 0x30000) {
        result = 10;
        VFipf_vol_set.last_error = 10;
    } else {
        if ((config & 0x10000) != 0)
            VFipf_vol_set.config |= 0x10000u;
        else
            VFipf_vol_set.config &= ~0x10000u;
        for (i = 0; !i; i = 1)
            VFipf_vol_set.current_vol[0].p_vol = VFipf_vol_set.volumes;
        VFipf_vol_set.current_vol[0].stat |= 1u;
        VFipf_vol_set.num_attached_drives = 0;
        VFipf_vol_set.num_mounted_volumes = 0;
        if ((config & 0x10000) != 0)
            VFipf_vol_set.config |= 0x10000u;
        else
            VFipf_vol_set.config &= ~0x10000u;

        VFipf_vol_set.param = param;
        VFipf_vol_set.last_error = 0;
        VFipf_vol_set.last_driver_error = 0;
        VFipf_vol_set.setting = 1;
        codeset.oem2unicode = &VFiPFCODE_CP932_OEM2Unicode;
        codeset.unicode2oem = &VFiPFCODE_CP932_Unicode2OEM;
        codeset.oem_char_width = &VFiPFCODE_CP932_OEMCharWidth;
        codeset.is_oem_mb_char = &VFiPFCODE_CP932_isOEMMBchar;
        codeset.unicode_char_width = &VFiPFCODE_CP932_UnicodeCharWidth;
        codeset.is_unicode_mb_char = &VFiPFCODE_CP932_isUnicodeMBchar;
        v4 = VFiPFVOL_p_setcode(&codeset);
        for (vol_idx = 0; vol_idx < 26; ++vol_idx)
            VFipf_memset(&VFipf_vol_set.volumes[vol_idx], 0, 6272);
        VFiPF_InitLockFile();
        return 0;
    }
    return result;
}

s32 VFiPFVOL_errnum() {
    return VFipf_vol_set.last_error;
}
