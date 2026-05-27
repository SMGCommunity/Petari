#include "revolution/vf/d_vf.h"
#include "revolution/os.h"
#include "revolution/vf/vf_struct.h"

static OSMutex l_Mutex;
static s32 l_InitedMutex;
static s32 l_vf_init;

#define VF_ERROR_B001 0xB001
#define VF_ERR_GENERIC 0xB002
#define VF_ERR_DRIVE_NOT_FOUND 0xB003
#define VF_ERR_0xB004 0xB004
#define VF_ERR_0xB005 0xB005

static void _VFInitMutex() {
    if (l_InitedMutex == 0) {
        OSInitMutex(&l_Mutex);
        l_InitedMutex = 1;
    }
}

static void _VFLockMutex() {
    if (l_InitedMutex != 0) {
        OSLockMutex(&l_Mutex);
    }
}

static void _VFUnlockMutex() {
    if (l_InitedMutex != 0) {
        OSUnlockMutex(&l_Mutex);
    }
}

s32 VFIsAvailable() {
    return l_vf_init != 0;
}

void VFInitEx(void* i_heap_start_address_p, u32 i_size) {
    _VFInitMutex();
    _VFLockMutex();

    if (l_vf_init == 0) {
        l_vf_init = 1;
        VFSysInit(i_heap_start_address_p, i_size);
        VFipdm_init_diskmanager(0, 0);
        VFipf2_init_prfile2(0, 0);
        dHash_InitHashTable();
        VFSysSetTimeStampCallback(0);
    }

    _VFUnlockMutex();
}

static s32 VFAttachDriveNANDFlash(const s8* i_drive) {
    s32 err = VF_ERR_0xB004;
    s32 idx = -1;

    _VFLockMutex();
    if (dHash_GetArg(i_drive) == -1) {
        err = VFSysSetDeviceNANDFlash(&idx, 0, 0);
        if (err == 0) {
            dHash_SetArg(i_drive, idx & 0xFF);
        }
    }
    VFSysSetLastError(err);
    _VFUnlockMutex();
    return err;
}

static s32 VFDetachDrive_common(const s8* i_drive, u8 i_set_err) {
    s32 err;
    s32 idx;

    _VFLockMutex();
    idx = dHash_GetArg(i_drive);
    err = VFSysUnsetDevice(idx);
    if (err == 0) {
        dHash_DeleteData(i_drive);
    }
    if (i_set_err != 0) {
        VFSysSetLastError(err);
    }
    _VFUnlockMutex();
    return err;
}

static s32 VF_activate_drive_common(s32 i_handle_idx, const char* i_sys_file_name_p, void* i_memory_p) {
    s32 err;
    s32 errChk;  // Present in DWARF but unused here.
    struct VF_HANDLE_DRIVE* drive_p;
    struct PF_DRV_TBL* drv_tbl[2];

    err = VFSysCheckExistPrfFile(i_handle_idx, (const s8*)i_sys_file_name_p, i_memory_p);
    if (err != 0) {
        VFSysSetLastError(err);
        return err;
    }

    err = VFSysMountDrv(i_handle_idx, (const s8*)i_sys_file_name_p, i_memory_p);
    if (err != 0) {
        VFSysSetLastError(err);
        return err;
    }

    drive_p = (void*)VFSysGetDriveP(i_handle_idx);

    if (drive_p == 0) {
        VFSysSetLastError(0xb002);
        return 0xb002;
    }

    drv_tbl[0] = &drive_p->pf_drv;
    drv_tbl[1] = 0;

    err = VFipf2_attach(drv_tbl);

    if (err != 0) {
        err = VFipf2_errnum();
        VFSysSetLastError(err);
        VFipdm_close_partition(drive_p->pf_part_p);
        return err;
    }

    return 0;
}

static s32 VF_activate_driveEx(const s8* i_drive, const s8* i_sys_file_name_p, void* i_memory_p) {
    s32 handle_idx;

    handle_idx = dHash_GetArg(i_drive);
    VFSysSetNandFuncEx(handle_idx);
    return VF_activate_drive_common(handle_idx, (const char*)i_sys_file_name_p, i_memory_p);
}

static s32 VFActivateDriveNANDFlashEx(const s8* i_drive, const s8* i_sys_file_name_p) {
    s32 Err;

    _VFLockMutex();
    Err = VF_activate_driveEx(i_drive, i_sys_file_name_p, NULL);
    _VFUnlockMutex();
    return Err;
}

static s32 VFInactivateDrive_common(s32 i_handle_idx, u32 i_mode) {
    s32 Err;

    Err = VFSysUnmountDrv(i_handle_idx, i_mode);
    VFSysSetLastError(Err);
    return Err;
}

static s32 VFInactivateDrive(const s8* i_drive) {
    s32 err;
    s32 handle_idx;

    _VFLockMutex();
    handle_idx = dHash_GetArg(i_drive);
    err = VFInactivateDrive_common(handle_idx, 0);
    _VFUnlockMutex();
    return err;
}

s32 VFMountDriveNANDFlashEx(const char* i_drive, const char* i_sys_file_name_p) {
    s32 err;

    err = VFAttachDriveNANDFlash((const s8*)i_drive);
    if ((err == 0 || err == VF_ERR_0xB004) && (err = VFActivateDriveNANDFlashEx((const s8*)i_drive, (const s8*)i_sys_file_name_p)) != 0 &&
        err != VF_ERR_0xB005) {
        VFDetachDrive_common((const s8*)i_drive, 0);
    }
    return err;
}

s32 VFUnmountDrive(const char* i_drive) {
    s32 err;

    err = VFInactivateDrive((const s8*)i_drive);
    if (err == 0) {
        err = VFDetachDrive_common((const s8*)i_drive, 1);
    }
    return err;
}

static s8* VF_path2handleidx(long* o_handle_idx_p, const char* i_path_p) {
    s8 drive[8];
    const s8* str_p;
    s32 idx;
    const s8* ret_p;

    *o_handle_idx_p = -1;
    str_p = (const s8*)i_path_p;
    idx = 0;
    ret_p = (const s8*)i_path_p;

    VFipf_memset(drive, 0, 8);

    while (*str_p != '\0') {
        if (*str_p == '\\' || *str_p == '/') {
            break;
        }

        if (*str_p == ':') {
            *o_handle_idx_p = dHash_GetArg(drive);
            if (*o_handle_idx_p == -1 || idx > 7) {
                ret_p = 0;
            } else {
                ret_p = str_p + 1;
            }
            break;
        }

        if (idx < 7) {
            drive[idx] = *str_p;
        }

        str_p++;
        idx++;
    }

    return (s8*)ret_p;
}

void* VFOpenFile(const char* i_path_p, const char* i_mode, u32 i_attr) {
    s32 handle_idx;
    const s8* path_p;
    void* pFile;

    handle_idx = -1;

    _VFLockMutex();

    path_p = VF_path2handleidx(&handle_idx, i_path_p);

    if (path_p == 0) {
        VFSysSetLastError(VF_ERR_DRIVE_NOT_FOUND);
        _VFUnlockMutex();
        return 0;
    }

    if (handle_idx != -1) {
        pFile = (VFFile)VFSysOpenFile(handle_idx, (const s8*)path_p, (const s8*)i_mode);
    } else {
        pFile = (VFFile)VFSysOpenFile_current((const s8*)path_p, (const s8*)i_mode);
    }

    _VFUnlockMutex();

    return pFile;
}

s32 VFCloseFile(void* i_file_p) {
    s32 Err;

    _VFLockMutex();
    Err = VFSysCloseFile(i_file_p);
    VFSysSetLastError(Err);
    _VFUnlockMutex();
    return Err;
}

s32 VFSeekFile(void* i_file_p, s32 i_offset, s32 i_origin) {
    s32 Err;

    _VFLockMutex();
    Err = VFSysSeekFile(i_file_p, i_offset, i_origin);
    VFSysSetLastError(Err);
    _VFUnlockMutex();
    return Err;
}

s32 VFReadFile(void* i_file_p, void* o_buf_p, u32 i_size, u32* o_read_size_p) {
    s32 Err;

    _VFLockMutex();
    Err = VFSysReadFile(o_read_size_p, o_buf_p, i_size, i_file_p);
    VFSysSetLastError(Err);
    _VFUnlockMutex();
    return Err;
}

s32 VFWriteFile(void* i_file_p, void* i_buf_p, u32 i_size) {
    s32 Err;

    _VFLockMutex();
    Err = VFSysWriteFile(i_buf_p, i_size, i_file_p);
    VFSysSetLastError(Err);
    _VFUnlockMutex();
    return Err;
}

s32 VFDeleteFile(const char* i_path_p) {
    s32 handle_idx;
    const s8* path_p;
    s32 Err;

    handle_idx = -1;

    _VFLockMutex();

    path_p = VF_path2handleidx(&handle_idx, i_path_p);

    if (path_p == 0) {
        Err = VF_ERR_DRIVE_NOT_FOUND;
        VFSysSetLastError(Err);
        _VFUnlockMutex();
        return VF_ERR_DRIVE_NOT_FOUND;
    }

    if (handle_idx != -1) {
        Err = VFSysDeleteFile(handle_idx, path_p);
    } else {
        Err = VFSysDeleteFile_current(path_p);
    }

    VFSysSetLastError(Err);
    _VFUnlockMutex();
    return Err;
}

s32 VFGetFileSizeByFd(void* i_file_p) {
    s32 size;
    s32 Err;

    size = -1;
    Err = VFSysGetFileSizeByFd(&size, i_file_p);
    if (Err != 0) {
        VFSysSetLastError(Err);
    }

    return size;
}

s32 VFGetLastError() {
    return VFSysGetLastError();
}

s32 VFGetLastDeviceError(const char* i_drive) {
    s32 err;
    s32 handle_idx;

    _VFLockMutex();

    if (i_drive != 0) {
        handle_idx = dHash_GetArg((const s8*)i_drive);
        err = VFSysGetLastDeviceError(handle_idx);
    } else {
        err = VFSysGetLastDeviceError_current();
    }

    VFSysSetLastError(err);

    _VFUnlockMutex();
    return err;
}

s32 VFGetDriveFreeSize(const char* i_drive) {
    s32 size;
    s32 handle_idx;

    _VFLockMutex();

    if (i_drive != 0) {
        handle_idx = dHash_GetArg((const s8*)i_drive);
        size = VFSysGetDriveFreeSize(handle_idx);
    } else {
        VFSysSetLastError(-1);
        size = -1;
    }

    _VFUnlockMutex();

    return size;
}

s32 VFSetSyncMode(const char* i_drive, u32 i_mode) {
    s32 Err;
    s32 handle_idx;

    _VFLockMutex();

    if (i_drive != 0) {
        handle_idx = dHash_GetArg((const s8*)i_drive);
        Err = VFSysSetSyncMode(handle_idx, i_mode);
    } else {
        Err = -1;
    }

    VFSysSetLastError(Err);
    _VFUnlockMutex();

    return Err;
}
