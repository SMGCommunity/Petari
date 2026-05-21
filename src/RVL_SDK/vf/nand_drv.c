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
        if (error != NAND_RESULT_BUSY && error != NAND_RESULT_ALLOC_FAILED) {
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
        if (error != NAND_RESULT_BUSY && error != NAND_RESULT_ALLOC_FAILED) {
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
        if (error != NAND_RESULT_BUSY && error != NAND_RESULT_ALLOC_FAILED) {
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
        if (error != NAND_RESULT_BUSY && error != NAND_RESULT_ALLOC_FAILED) {
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
        if (error != NAND_RESULT_BUSY && error != NAND_RESULT_ALLOC_FAILED) {
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

static s32 _MountPrfFile(struct PDM_DISK* p_disk, s8* i_fullpath_p) {
    struct PR_BINHEADER header ATTR_ALIGN(64);
    struct VF_HANDLE_DRIVE* drive_p;
    struct NANDFileInfo* fileInfo_p;
    s32 nandError;
    u32 handleIdx;
    u32 fileSize;
    u32 dataSize;
    u32 SPU;  // Present in DWARF but unused here.
    enum FatType fatType;

    drive_p = (struct VF_HANDLE_DRIVE*)VFSysPDMDisk2DriveP(p_disk);
    handleIdx = dCommon_getHandleIdxFromDisk(p_disk);

    if (drive_p == NULL) {
        return -20;
    }

    fileInfo_p = drive_p->file_p;

    nandError = VFi_NandOpenSp((const char*)i_fullpath_p, fileInfo_p, 1, handleIdx);
    if (nandError == 0) {
        VFipf_memset(&header, 0, 0x20);
        nandError = A32_NANDRead(fileInfo_p, &header, 0x20);
        if (nandError < 0) {
            VFi_NandClose(fileInfo_p);
            dCommon_setLastDeviceErrorToDisk(p_disk, nandError);
            return nandError;
        }

        dCommon_PrintSignature(&header);
        if (dCommon_IsPrfFile(&header) == 0) {
            VFi_NandClose(fileInfo_p);
            return -1;
        }

        dCommon_setFileSizeToDisk(p_disk, (header.fileSize[0] << 24) | (header.fileSize[1] << 16) | (header.fileSize[2] << 8) | header.fileSize[3]);
        fileSize = dCommon_getFileSizeFromDisk(p_disk);
        dataSize = (fileSize - 0x1F) >> 9;
        fatType = dCommon_GetNiceFatType(0, dataSize, 1, 0x200);
        dCommon_setFatTypeToDisk(p_disk, fatType);
        dCommon_setResvSecNumToDisk(p_disk, dCommon_GetReservedSecFromFatType(fatType));
        dCommon_setRootEntNumToDisk(p_disk, dCommon_GetRootEntNumFromFatType(fatType));

        VFi_NandClose(fileInfo_p);
    } else {
        dCommon_setLastDeviceErrorToDisk(p_disk, nandError);
        return nandError;
    }

    nandError = VFi_NandOpenSp((const char*)i_fullpath_p, fileInfo_p, 3, handleIdx);
    if (nandError == 0) {
        return 0;
    }

    dCommon_setLastDeviceErrorToDisk(p_disk, nandError);
    return nandError;
}

static u16 _UnmountPrfFile(struct PDM_DISK* p_disk) {
    struct VF_HANDLE_DRIVE* drive_p;
    s32 nandError;
    struct NANDFileInfo* fileInfo_p;

    drive_p = (struct VF_HANDLE_DRIVE*)VFSysPDMDisk2DriveP(p_disk);
    if (drive_p != NULL) {
        fileInfo_p = drive_p->file_p;
        nandError = VFi_NandClose(fileInfo_p);
        if (nandError == 0) {
            return 0;
        }
        dCommon_setLastDeviceErrorToDisk(p_disk, nandError);
    }
    return 0xFFFF;
}

static s32 nanddrv_BuildUpFSInfoSector(u8* buf) {
    s32 err = dCommon_MakeFsInfoSec(buf, 0x200);
    return (err == 0) ? 0 : -21;
}

static s32 nanddrv_BuildUpBootSector(struct PDM_DISK* p_disk, u8* buf, enum FatType* type) {
    struct PDM_DISK* realDisk_p;
    u32 drvSPU;

    realDisk_p = (struct PDM_DISK*)((u8*)&VFipdm_disk_set + ((u32)p_disk & 0xFF) * 0x34);
    drvSPU = *(u32*)((u8*)realDisk_p + 0x1BC);

    dCommon_MakeBootSector(buf, type, drvSPU, 1, 0x200, 0xEB, 0x90, 0x3F, dCommon_getResvSecNumFromDisk(p_disk),
                           dCommon_getRootEntNumFromDisk(p_disk), 0xF0);
    dCommon_setFatTypeToDisk(p_disk, *type);
    return 0;
}

static s32 nanddrv_init(struct PDM_DISK* p_disk) {
    if (p_disk == NULL) {
        return -20;
    }
    dCommon_setFatTypeToDisk(p_disk, 1);
    VFipdm_disk_notify_media_insert(p_disk);
    return 0;
}

static s32 nanddrv_mount(struct PDM_DISK* p_disk) {
    struct VF_HANDLE_DRIVE* drive_p;
    s32 err;
    if (p_disk == NULL) {
        return -20;
    }
    drive_p = (struct VF_HANDLE_DRIVE*)VFSysPDMDisk2DriveP(p_disk);
    if (drive_p == NULL) {
        return -20;
    }
    err = _MountPrfFile(p_disk, (s8*)drive_p->pf_filename);
    return err == 0 ? 0 : err;
}

static s32 nanddrv_format(struct PDM_DISK* p_disk, const u8* param) {
    return p_disk ? 0 : -20;
}

static s32 nanddrv_pread(struct PDM_DISK* p_disk, u8* p_buf, u32 block, u32 num_blocks, u32* p_num_success) {
    *p_num_success = 0;
    if (p_disk == NULL || p_buf == NULL || num_blocks == 0 || p_num_success == NULL) {
        return -20;
    }
    return nanddrv_physical_read(num_blocks, p_buf, block, 0x200, p_num_success, p_disk);
}

static s32 nanddrv_pwrite(struct PDM_DISK* p_disk, const u8* p_buf, u32 block, u32 num_blocks, u32* p_num_success) {
    *p_num_success = 0;
    if (p_disk == NULL || p_buf == NULL || num_blocks == 0 || p_num_success == NULL) {
        return -20;
    }
    return nanddrv_physical_write(num_blocks, p_buf, block, 0x200, p_num_success, p_disk);
}

static s32 nanddrv_unmount(struct PDM_DISK* p_disk) {
    u16 nandError;

    if (p_disk == NULL) {
        return -20;
    }

    nandError = _UnmountPrfFile(p_disk);
    if (nandError == 0) {
        return 0;
    }
    return nandError;
}

static s32 nanddrv_finalize(struct PDM_DISK* p_disk) {
    if (p_disk == NULL) {
        return -20;
    }
    dCommon_setFatTypeToDisk(p_disk, 1);
    return 0;
}

static s32 nanddrv_get_disk_info(struct PDM_DISK* p_disk, struct PDM_DISK_INFO* p_disk_info) {
    u32 fileSize;
    u32 dataSize;

    if (p_disk == NULL || p_disk_info == NULL) {
        return -20;
    }

    fileSize = dCommon_getFileSizeFromDisk(p_disk);
    dataSize = (fileSize - 0x1F) >> 9;

    p_disk_info->total_sectors = dataSize;
    p_disk_info->cylinders = dataSize / 255 / 63 / 512;
    p_disk_info->heads = 0xFF;
    p_disk_info->sectors_per_track = 0x3F;
    p_disk_info->bytes_per_sector = 0x200;
    p_disk_info->media_attr = 0;
    p_disk_info->format_param = NULL;

    return 0;
}

s32 VFi_nanddrv_init_drv_tbl(struct PDM_DISK_TBL* p_disk_tbl, u32 ui_ext) {
    p_disk_tbl->p_func = (struct PDM_FUNCTBL*)&l_nand_func;
    p_disk_tbl->ui_ext = ui_ext;
    return 0;
}

static s32 nanddrv_physical_read(u32 num_blocks, u8* buf, u32 block, u32 bps, u32* p_num_success, struct PDM_DISK* p_disk) {
    u32 fileSize;
    u32 size;
    s32 err;
    u32 offset;
    s32 nandError;
    struct VF_HANDLE_DRIVE* drive_p;
    struct NANDFileInfo* fileInfo_p;

    fileSize = dCommon_getFileSizeFromDisk(p_disk);
    drive_p = (struct VF_HANDLE_DRIVE*)VFSysPDMDisk2DriveP(p_disk);

    if (drive_p == NULL) {
        return -20;
    }

    fileInfo_p = drive_p->file_p;

    if (!dCommon_ReadDummyBPB(num_blocks, buf, block, p_num_success, p_disk, &err, nanddrv_BuildUpBootSector, nanddrv_BuildUpFSInfoSector)) {
        return err;
    }

    size = num_blocks * bps;
    offset = dCommon_GetPhysicalOffset(block, bps, dCommon_getResvSecNumFromDisk(p_disk));

    nandError = VFi_NandSeek(fileInfo_p, offset, 0);

    if (nandError == offset) {
        if (offset + size > fileSize) {
            return -22;
        }

        nandError = A32_NANDRead(fileInfo_p, buf, size);
        if (nandError == size) {
            *p_num_success = num_blocks;
            return 0;
        }
    }

    dCommon_setLastDeviceErrorToDisk(p_disk, nandError);
    return nandError;
}

static s32 nanddrv_physical_write(u32 num_blocks, const u8* buf, u32 block, u32 bps, u32* p_num_success, struct PDM_DISK* p_disk) {
    u32 fileSize;
    u32 size;
    u32 offset;
    s32 nandError;
    struct VF_HANDLE_DRIVE* drive_p;
    struct NANDFileInfo* fileInfo_p;
    s32 err;

    fileSize = dCommon_getFileSizeFromDisk(p_disk);
    drive_p = (struct VF_HANDLE_DRIVE*)VFSysPDMDisk2DriveP(p_disk);
    err = 0;

    if (drive_p == NULL) {
        return -20;
    }

    fileInfo_p = drive_p->file_p;

    if (!dCommon_WriteDummyBPB(num_blocks, block, p_num_success, p_disk, &err)) {
        return err;
    }

    size = num_blocks * bps;
    offset = dCommon_GetPhysicalOffset(block, bps, dCommon_getResvSecNumFromDisk(p_disk));

    nandError = VFi_NandSeek(fileInfo_p, offset, 0);

    if (nandError == offset) {
        if (offset + size > fileSize) {
            return -22;
        }

        nandError = A32_NANDWrite(fileInfo_p, (void*)buf, size, p_disk);
        if (nandError == size) {
            *p_num_success = num_blocks;
            return 0;
        }
    }

    dCommon_setLastDeviceErrorToDisk(p_disk, nandError);
    return nandError;
}
