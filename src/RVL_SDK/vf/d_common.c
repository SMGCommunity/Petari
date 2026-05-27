#include "revolution/vf/d_common.h"
#include "revolution/vf/vf_struct.h"

struct VF_DRIVE_INFO l_driveInfo[26];

static struct VF_DRIVE_INFO* _getDriveInfoP(struct PDM_DISK* p_disk);
static u32 dCommon_getFatTypeFromDisk(struct PDM_DISK* p_disk);

void dCommon_DevideBuff32(void* i_buf, u32 i_size, u32* i_1st_size_p, void* i_2nd_pp, u32* i_2nd_size_p, void* i_3rd_pp, u32* i_3rd_size_p) {
    u32 align;

    align = ((u32)i_buf) & 0x1f;
    *(void**)i_3rd_pp = NULL;
    *(void**)i_2nd_pp = NULL;
    *i_3rd_size_p = 0;
    *i_2nd_size_p = 0;
    *i_1st_size_p = 0;

    if (align == 0) {
        *i_1st_size_p = 0;
        *(void**)i_2nd_pp = i_buf;
        *i_2nd_size_p = i_size;
        *(void**)i_3rd_pp = NULL;
        *i_3rd_size_p = 0;
        return;
    }

    *i_1st_size_p = 0x20 - align;

    if ((i_size - 0x20) != 0) {
        *(void**)i_2nd_pp = (u8*)i_buf + *i_1st_size_p;
        *i_2nd_size_p = i_size - 0x20;
    }

    *i_3rd_size_p = align;
    *(void**)i_3rd_pp = (u8*)i_buf + i_size - align;
}

int dCommon_IsPrfFile(void* i_buff_p) {
    struct PR_BINHEADER header;
    VFipf_memcpy(&header, i_buff_p, 0x20);

    if (header.signature[0] != 'V' || header.signature[1] != 'F' || header.signature[2] != 'F' || header.signature[3] != ' ') {
        return 0;
    }
    return 1;
}

void dCommon_PrintSignature(struct PR_BINHEADER* i_header_p) {
    ;
}

s32 dCommon_MakeFsInfoSec(u8* buf, u32 fsInfoSize) {
    struct PDM_FSINFO fsinfo;
    VFipf_memset(buf, 0, fsInfoSize);
    fsinfo.free_count = -1;
    fsinfo.next_free = -1;
    return VFipdm_bpb_set_fsinfo_information(&fsinfo, buf);
}

u32 dCommon_GetPhysicalOffset(u32 startBlock, u32 BPS, u32 reservedSecNum) {
    return ((startBlock - reservedSecNum) * BPS) + 0x20;
}

enum FatType dCommon_GetNiceFatType(u32* spf, u32 SPU, u32 SPC, u32 BPS) {
    u32 SPU_div_SPC;

    // TODO: dumb hack to prevent inlining
    // This prevents dCommon_GetNiceFatType from being inlined in dCommon_MakeBootSector.
    int i;
    for (i = 0; i < 1; i++) {
    }

    SPU_div_SPC = SPU / SPC;
    if (SPU_div_SPC < 0xff5) {
        if (spf != NULL) {
            *spf = (((SPU_div_SPC * 3) / 2) + BPS - 1) / BPS;
        }
        return FAT_12;
    }

    if (SPU_div_SPC < 0xfff5) {
        if (spf != NULL) {
            *spf = ((SPU_div_SPC * 2) + BPS - 1) / BPS;
        }
        return FAT_16;
    }

    if (spf != NULL) {
        *spf = ((SPU_div_SPC * 4) + BPS - 1) / BPS;
    }

    return FAT_32;
}

u32 dCommon_GetReservedSecFromFatType(enum FatType fatType) {
    if (fatType == FAT_32) {
        return 0x20;
    }
    return 1;
}

u32 dCommon_GetRootEntNumFromFatType(enum FatType fatType) {
    return (fatType == FAT_32) ? 0 : 0x80;
}

void dCommon_MakeBootSector(u8* buf, enum FatType* type, u32 SPU, u32 SPC, u32 BPS, u8 jump0_1, u8 jump2, u32 secPerTrack, u32 reservedSecNum,
                            u32 rootEntNum, u8 mediaDesc) {
    u32 spf;
    *type = dCommon_GetNiceFatType(&spf, SPU, SPC, BPS);

    VFipf_memset(buf, 0, 0x200);

    buf[0] = jump0_1;
    buf[2] = jump2;
    buf[0x1fe] = 0x55;
    buf[0x1ff] = 0xaa;

    buf[0x18] = secPerTrack & 0xff;
    buf[0x19] = (secPerTrack >> 8) & 0xff;
    buf[0x1a] = 0xff;
    buf[0x1b] = 0;

    buf[0xb] = BPS & 0xff;
    buf[0xc] = (BPS >> 8) & 0xff;
    buf[0xd] = SPC;
    buf[0xe] = reservedSecNum & 0xff;
    buf[0xf] = (reservedSecNum >> 8) & 0xff;
    buf[0x10] = 2;
    buf[0x11] = rootEntNum & 0xff;
    buf[0x12] = (rootEntNum >> 8) & 0xff;
    buf[0x15] = mediaDesc;

    if (SPU < 0x10000) {
        buf[0x13] = SPU & 0xff;
        buf[0x14] = (SPU >> 8) & 0xff;
        buf[0x20] = 0;
        buf[0x21] = 0;
        buf[0x22] = 0;
        buf[0x23] = 0;
    } else {
        buf[0x13] = 0;
        buf[0x14] = 0;
        buf[0x20] = SPU & 0xff;
        buf[0x21] = (SPU >> 8) & 0xff;
        buf[0x22] = (SPU >> 16) & 0xff;
        buf[0x23] = (SPU >> 24) & 0xff;
    }

    if (*type == FAT_32) {
        buf[0xe] = 0x20;
        buf[0xf] = 0;
        buf[0x11] = 0;
        buf[0x12] = 0;

        buf[0x24] = spf & 0xff;
        buf[0x25] = (spf >> 8) & 0xff;
        buf[0x26] = (spf >> 16) & 0xff;
        buf[0x27] = (spf >> 24) & 0xff;
        buf[0x28] = 0;
        buf[0x29] = 0;
        buf[0x2a] = 0;
        buf[0x2b] = 0;
        buf[0x2c] = 2;
        buf[0x2d] = 0;
        buf[0x2e] = 0;
        buf[0x2f] = 0;
        buf[0x30] = 1;
        buf[0x31] = 0;

        buf[0x41] = 0;
        buf[0x42] = 0x29;
        buf[0x43] = 0x34;
        buf[0x44] = 0x12;
        buf[0x45] = 0;
        buf[0x46] = 0;

        buf[0x40] = 0x80;
        buf[0x32] = 6;
        buf[0x33] = 0;
    } else {
        buf[0x11] = rootEntNum;
        buf[0x12] = rootEntNum >> 8;

        buf[0x16] = spf & 0xff;
        buf[0x17] = (spf >> 8) & 0xff;
        buf[0x25] = 0;
        buf[0x26] = 0x29;
        buf[0x27] = 0x34;
        buf[0x28] = 0x12;
        buf[0x29] = 0;
        buf[0x2a] = 0;
        buf[0x24] = 0x80;
    }
}

int dCommon_ReadDummyBPB(u32 num_blocks, u8* buf, u32 block, u32* p_num_success, struct PDM_DISK* p_disk, s32* p_err,
                         s32 (*makeBS)(struct PDM_DISK*, u8*, enum FatType*), s32 (*makeFS)(u8*)) {
    enum FatType type;

    *p_err = 0;

    if (block < dCommon_getResvSecNumFromDisk(p_disk)) {
        if (block == 0) {
            *p_err = makeBS(p_disk, buf, &type);
            if (*p_err != 0) {
                return 0;
            } else {
                *p_num_success = num_blocks;
                *p_err = 0;
                return 0;
            }
        } else if (block == 1 && dCommon_getFatTypeFromDisk(p_disk) == FAT_32) {
            *p_err = makeFS(buf);
            if (*p_err != 0) {
                return 0;
            } else {
                *p_num_success = num_blocks;
                *p_err = 0;
                return 0;
            }
        } else {
            *p_err = -22;
            return 0;
        }
    } else {
        return 1;
    }
}

int dCommon_WriteDummyBPB(u32 num_blocks, u32 block, u32* p_num_success, struct PDM_DISK* p_disk, s32* p_err) {
    *p_err = 0;

    if (block < dCommon_getResvSecNumFromDisk(p_disk)) {
        if (block == 0 || (block == 1 && dCommon_getFatTypeFromDisk(p_disk) == FAT_32)) {
            *p_num_success = num_blocks;
            *p_err = 0;
            return 0;
        } else {
            *p_err = -22;
            return 0;
        }
        return 0;
    }

    return 1;
}

static struct VF_DRIVE_INFO* _getDriveInfoP(struct PDM_DISK* p_disk) {
    u32 handleIdx;
    handleIdx = (u32)VFSysPDMDisk2HandleIdx(p_disk);
    if (handleIdx < 0x1A) {
        return &l_driveInfo[handleIdx];
    }
    return NULL;
}

void dCommon_initDriveInfo() {
    struct VF_DRIVE_INFO* driveInfo_p;
    int i;

    for (i = 0, driveInfo_p = l_driveInfo; i < 26; i++, driveInfo_p++) {
        driveInfo_p->prf_file_size = 0;
        driveInfo_p->last_device_error = 0;
        driveInfo_p->fat_type = 0;
        driveInfo_p->reserved_sec_num = 0;
        driveInfo_p->root_ent_num = 0;
    }
}

u32 dCommon_getFileSizeFromDisk(struct PDM_DISK* p_disk) {
    struct VF_DRIVE_INFO* driveInfo_p;
    driveInfo_p = _getDriveInfoP(p_disk);
    return driveInfo_p ? driveInfo_p->prf_file_size : 0;
}

void dCommon_setFileSizeToDisk(struct PDM_DISK* p_disk, u32 i_fileSize) {
    struct VF_DRIVE_INFO* driveInfo_p;
    driveInfo_p = _getDriveInfoP(p_disk);
    if (driveInfo_p) {
        driveInfo_p->prf_file_size = i_fileSize;
    }
}

s32 dCommon_getLastDeviceErrorFromDisk(struct PDM_DISK* p_disk) {
    struct VF_DRIVE_INFO* driveInfo_p;
    driveInfo_p = _getDriveInfoP(p_disk);
    return driveInfo_p ? driveInfo_p->last_device_error : 0;
}

void dCommon_setLastDeviceErrorToDisk(struct PDM_DISK* p_disk, s32 i_lastDevErr) {
    struct VF_DRIVE_INFO* driveInfo_p;
    driveInfo_p = _getDriveInfoP(p_disk);
    if (driveInfo_p) {
        driveInfo_p->last_device_error = i_lastDevErr;
    }
}

void dCommon_setLastDeviceErrorToDisk2(s32 i_handleIdx, s32 i_lastDevErr) {
    if (i_handleIdx >= 0 && i_handleIdx < 0x1a) {
        l_driveInfo[i_handleIdx].last_device_error = i_lastDevErr;
    }
}

s32 dCommon_getHandleIdxFromDisk(struct PDM_DISK* p_disk) {
    return VFSysPDMDisk2HandleIdx(p_disk);
}

static u32 dCommon_getFatTypeFromDisk(struct PDM_DISK* p_disk) {
    struct VF_DRIVE_INFO* driveInfo_p = _getDriveInfoP(p_disk);
    return driveInfo_p ? driveInfo_p->fat_type : 1;
}

void dCommon_setFatTypeToDisk(struct PDM_DISK* p_disk, u32 i_fatType) {
    struct VF_DRIVE_INFO* driveInfo_p;
    driveInfo_p = _getDriveInfoP(p_disk);
    if (driveInfo_p) {
        driveInfo_p->fat_type = i_fatType;
    }
}

u32 dCommon_getResvSecNumFromDisk(struct PDM_DISK* p_disk) {
    struct VF_DRIVE_INFO* driveInfo_p;
    driveInfo_p = _getDriveInfoP(p_disk);
    return driveInfo_p ? driveInfo_p->reserved_sec_num : 0;
}

void dCommon_setResvSecNumToDisk(struct PDM_DISK* p_disk, u32 i_reservedSecNum) {
    struct VF_DRIVE_INFO* driveInfo_p;
    driveInfo_p = _getDriveInfoP(p_disk);
    if (driveInfo_p) {
        driveInfo_p->reserved_sec_num = i_reservedSecNum;
    }
}

u32 dCommon_getRootEntNumFromDisk(struct PDM_DISK* p_disk) {
    struct VF_DRIVE_INFO* driveInfo_p;
    driveInfo_p = _getDriveInfoP(p_disk);
    return driveInfo_p ? driveInfo_p->root_ent_num : 0;
}

void dCommon_setRootEntNumToDisk(struct PDM_DISK* p_disk, u32 i_rootEntNum) {
    struct VF_DRIVE_INFO* driveInfo_p;
    driveInfo_p = _getDriveInfoP(p_disk);
    if (driveInfo_p) {
        driveInfo_p->root_ent_num = i_rootEntNum;
    }
}

s32 dCommon_flush_from_handle_p(struct VF_HANDLE_TYPE* i_handle_p, int i_setLastDeviceError) {
    struct VF_HANDLE_DRIVE* vol;  // Extra variable. Not in DWARF.
    s32 err;
    s32 handle_idx;

    vol = (struct VF_HANDLE_DRIVE*)VFSysVol2HandleP((struct PF_VOLUME*)i_handle_p);
    err = -1;
    if (vol != NULL && vol->file_p != NULL) {
        if (((u32*)vol->file_p)[2] == 0) {
            handle_idx = VFSysHandleP2Idx((struct VF_HANDLE_TYPE*)vol);
            if (handle_idx != -1) {
                err = VFi_NandFlushNANDFromHandleIdx(handle_idx, i_setLastDeviceError);
            }
        } else {
            err = 0;
        }
    }
    return err;
}
