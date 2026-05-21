#include "revolution/vf/nand_drv.h"
#include "macros.h"
#include "revolution/nand.h"
#include "revolution/vf/vf_struct.h"

extern struct PDM_DISK_SET VFipdm_disk_set;
s32 VF_nand_retry_max;
s32 VF_nand_sleep_msec;

static struct {
    s32 (*create)(const char*, u8, u8);
    s32 (*open)(const char*, struct NANDFileInfo*, u8);
    s32 (*createDir)(const char*, u8, u8);
    s32 (*delete)(const char*);
} l_nandFunc[26];

static const struct PDM_FUNCTBL l_nand_func = {nanddrv_init,
                                               nanddrv_finalize,
                                               nanddrv_mount,
                                               nanddrv_unmount,
                                               (s32(*)(struct PDM_DISK*, u8*))nanddrv_format,
                                               nanddrv_pread,
                                               (s32(*)(struct PDM_DISK*, u8*, u32, u32, u32*))nanddrv_pwrite,
                                               (s32(*)(struct PDM_DISK*, struct PDM_DISK_INFO*))nanddrv_get_disk_info};

static inline void _SleepAfewMiliSec(void) {
    OSSleepTicks((s64)VF_nand_sleep_msec * ((*(u32*)0x800000F8 / 4) / 1000));
}

s32 VFi_NandCreate(const char* path, u8 perm, u8 attr) {
    s32 challenge;
    s32 error;

    challenge = VF_nand_retry_max;
    error = 0;
    while (challenge-- > 0) {
        error = NANDPrivateCreate(path, perm, attr);
        if (error != NAND_RESULT_BUSY) {
            return error;
        } else {
            _SleepAfewMiliSec();
        }
    }
    return error;
}

s32 VFi_NandDelete(const char* path) {
    s32 challenge;
    s32 error;

    challenge = VF_nand_retry_max;
    error = 0;
    while (challenge-- > 0) {
        error = NANDPrivateDelete(path);
        if (error != NAND_RESULT_BUSY) {
            return error;
        } else {
            _SleepAfewMiliSec();
        }
    }
    return error;
}

s32 VFi_NandClose(struct NANDFileInfo* info) {
    s32 challenge;
    s32 error;

    challenge = VF_nand_retry_max;
    error = 0;
    while (challenge-- > 0) {
        error = NANDClose(info);
        if (error != NAND_RESULT_BUSY) {
            return error;
        } else {
            _SleepAfewMiliSec();
        }
    }
    return error;
}

s32 VFi_NandOpen(const char* path, struct NANDFileInfo* info, u8 accType) {
    s32 challenge;
    s32 error;

    challenge = VF_nand_retry_max;
    error = 0;
    while (challenge-- > 0) {
        error = NANDOpen(path, info, accType);
        if (error != NAND_RESULT_BUSY) {
            return error;
        } else {
            _SleepAfewMiliSec();
        }
    }
    return error;
}

s32 VFi_NANDPrivateOpen(const char* path, struct NANDFileInfo* info, u8 accType) {
    s32 challenge;
    s32 error;

    challenge = VF_nand_retry_max;
    error = 0;
    while (challenge-- > 0) {
        error = NANDPrivateOpen(path, info, accType);
        if (error != NAND_RESULT_BUSY && error != NAND_RESULT_ALLOC_FAILED) {
            return error;
        } else {
            _SleepAfewMiliSec();
        }
    }
    return error;
}

s32 VFi_NandWrite(struct NANDFileInfo* info, void* buf, u32 length) {
    s32 challenge;
    s32 error;

    challenge = VF_nand_retry_max;
    error = 0;
    while (challenge-- > 0) {
        error = NANDWrite(info, buf, length);
        if (error != NAND_RESULT_BUSY && error != NAND_RESULT_ALLOC_FAILED) {
            return error;
        } else {
            _SleepAfewMiliSec();
        }
    }
    return error;
}

s32 VFi_NandSeek(struct NANDFileInfo* info, s32 offset, s32 whence) {
    s32 challenge;
    s32 error;

    challenge = VF_nand_retry_max;
    error = 0;
    while (challenge-- > 0) {
        error = NANDSeek(info, offset, whence);
        if (error != NAND_RESULT_BUSY && error != NAND_RESULT_ALLOC_FAILED) {
            return error;
        } else {
            _SleepAfewMiliSec();
        }
    }
    return error;
}

s32 VFi_NandRead(struct NANDFileInfo* info, void* buf, u32 length) {
    s32 challenge;
    s32 error;

    challenge = VF_nand_retry_max;
    error = 0;
    while (challenge-- > 0) {
        error = NANDRead(info, buf, length);
        if (error != NAND_RESULT_BUSY && error != NAND_RESULT_ALLOC_FAILED) {
            return error;
        } else {
            _SleepAfewMiliSec();
        }
    }
    return error;
}

s32 VFi_NandCreateDir(const char* path, u8 perm, u8 attr) {
    s32 challenge;
    s32 error;

    challenge = VF_nand_retry_max;
    error = 0;
    while (challenge-- > 0) {
        error = NANDPrivateCreateDir(path, perm, attr);
        if (error != NAND_RESULT_BUSY) {
            return error;
        } else {
            _SleepAfewMiliSec();
        }
    }
    return error;
}

s32 VFi_NandGetLength(struct NANDFileInfo* info, u32* length) {
    s32 challenge;
    s32 error;

    challenge = VF_nand_retry_max;
    error = 0;
    while (challenge-- > 0) {
        error = NANDGetLength(info, length);
        if (error != NAND_RESULT_BUSY) {
            return error;
        } else {
            _SleepAfewMiliSec();
        }
    }
    return error;
}

s32 VFi_NandOpenSp(const char* path, void* info, u8 accType, u32 i_handleIdx) {
    struct NANDFileInfo* info_p;
    s32 (*tmpOpen)(const char*, struct NANDFileInfo*, u8);

    info_p = (struct NANDFileInfo*)info;

    if (i_handleIdx < 26) {
        tmpOpen = (s32(*)(const char*, struct NANDFileInfo*, u8))l_nandFunc[i_handleIdx].open;
        if (tmpOpen != NULL) {
            return tmpOpen(path, info_p, accType);
        }
        return VFi_NandOpen(path, info_p, accType);
    } else if (i_handleIdx == 0xFFFFFFF6) {
        return VFi_NandOpen(path, info_p, accType);
    } else {
        return VFi_NANDPrivateOpen(path, info_p, accType);
    }
}

void VFi_NandSetNANDFuncNormal(u32 i_handleIdx) {
    if (i_handleIdx < 26) {
        l_nandFunc[i_handleIdx].create = VFi_NandCreate;
        l_nandFunc[i_handleIdx].open = VFi_NandOpen;
        l_nandFunc[i_handleIdx].createDir = VFi_NandCreateDir;
        l_nandFunc[i_handleIdx].delete = VFi_NandDelete;
    }
}

s32 A32_NANDRead(struct NANDFileInfo* i_fileInfo_p, void* i_buf, u32 i_size) {
    u8 work[32] ATTR_ALIGN(64);
    s32 NANDError;
    void* p_2nd;
    void* p_3rd;
    u32 size_1st;
    u32 size_2nd;
    u32 size_3rd;

    if ((i_size & 0x1F) != 0) {
        return 0;
    }

    dCommon_DevideBuff32(i_buf, i_size, &size_1st, &p_2nd, &size_2nd, &p_3rd, &size_3rd);

    if (size_1st == 0) {
        NANDError = VFi_NandRead(i_fileInfo_p, i_buf, i_size);
        if (NANDError < 0) {
            return NANDError;
        }
    } else {
        NANDError = VFi_NandRead(i_fileInfo_p, work, 32);
        if (NANDError < 0) {
            return NANDError;
        }

        VFipf_memcpy(i_buf, work, size_1st);

        NANDError = VFi_NandSeek(i_fileInfo_p, -(s32)size_3rd, 1);
        if (NANDError < 0) {
            return NANDError;
        }

        if (size_2nd != 0) {
            NANDError = VFi_NandRead(i_fileInfo_p, p_2nd, size_2nd);
            if (NANDError < 0) {
                return NANDError;
            }
        }

        if (size_3rd != 0) {
            NANDError = VFi_NandRead(i_fileInfo_p, work, 32);
            if (NANDError < 0) {
                return NANDError;
            }

            VFipf_memcpy(p_3rd, work, size_3rd);
        }
    }
    return i_size;
}

s32 A32_NANDWrite(struct NANDFileInfo* i_fileInfo_p, void* i_buf, u32 i_size, struct PDM_DISK* p_disk) {
    u8 work[32] ATTR_ALIGN(64);
    s32 NANDError;
    void* p_2nd;
    void* p_3rd;
    u32 size_1st;
    u32 size_2nd;
    u32 size_3rd;
    u32 handleIdx;
    struct VF_HANDLE_TYPE* handle_p;

    handleIdx = dCommon_getHandleIdxFromDisk(p_disk);
    handle_p = (struct VF_HANDLE_TYPE*)VFSysGetHandleP(handleIdx);

    if ((i_size & 0x1F) != 0) {
        return 0;
    }

    dCommon_DevideBuff32(i_buf, i_size, &size_1st, &p_2nd, &size_2nd, &p_3rd, &size_3rd);

    if (size_1st == 0) {
        NANDError = VFi_NandWrite(i_fileInfo_p, i_buf, i_size);
        if (NANDError < 0) {
            return NANDError;
        }
    } else {
        VFipf_memcpy(work, i_buf, size_1st);

        NANDError = VFi_NandWrite(i_fileInfo_p, work, size_1st);
        if (NANDError < 0) {
            return NANDError;
        }

        NANDError = VFi_NandWrite(i_fileInfo_p, p_2nd, size_2nd + size_3rd);
        if (NANDError < 0) {
            return NANDError;
        }
    }

    return i_size;
}

s32 VFi_NandFlushNANDFromHandleIdx(s32 i_handleIdx, int i_setLastDeviceError) {
    s32 NANDError;
    struct VF_HANDLE_DRIVE* drive_p;
    struct VF_HANDLE_TYPE* handle_p;
    const char* sys_name_p;
    struct NANDFileInfo* fileInfo_p;

    drive_p = (struct VF_HANDLE_DRIVE*)VFSysGetDriveP(i_handleIdx);
    handle_p = (struct VF_HANDLE_TYPE*)VFSysGetHandleP(i_handleIdx);

    if (handle_p != NULL && handle_p->device_p != NULL && handle_p->device_p->sync_mode == 1) {
        return 0;
    }

    if (drive_p != NULL) {
        sys_name_p = (const char*)drive_p->pf_filename;
        fileInfo_p = drive_p->file_p;
        NANDError = VFi_NandClose(fileInfo_p);

        if (NANDError < 0) {
            if (i_setLastDeviceError != 0) {
                dCommon_setLastDeviceErrorToDisk2(i_handleIdx, NANDError);
            }
            return NANDError;
        }

        if ((u32)i_handleIdx < 26) {
            s32 (*tmpOpen)(const char*, struct NANDFileInfo*, u8) = l_nandFunc[i_handleIdx].open;
            if (tmpOpen != NULL) {
                NANDError = tmpOpen(sys_name_p, fileInfo_p, 3);
            } else {
                NANDError = VFi_NandOpen(sys_name_p, fileInfo_p, 3);
            }
        } else if (i_handleIdx == 0xFFFFFFF6) {
            NANDError = VFi_NandOpen(sys_name_p, fileInfo_p, 3);
        } else {
            NANDError = VFi_NANDPrivateOpen(sys_name_p, fileInfo_p, 3);
        }

        if (NANDError < 0) {
            if (i_setLastDeviceError != 0) {
                dCommon_setLastDeviceErrorToDisk2(i_handleIdx, NANDError);
            }
            return NANDError;
        }
    }
    return 0;
}
