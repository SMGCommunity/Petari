#include "revolution/vf/d_vf_sys.h"
#include "macros.h"
#include "revolution/dvd.h"
#include "revolution/mem.h"
#include "revolution/nand.h"
#include "revolution/vf/pf_fopen.h"
#include "revolution/vf/pf_volume.h"
#include "revolution/vf/vf_struct.h"

struct VFSysDeviceBase {
    struct VF_HANDLE_DEVICE base;  // offset 0x0, size 0x10
};

union VFSysDeviceTableEntry {
    struct {
        struct VFSysDeviceBase device;  // offset 0x0, size 0x10
        struct DVDFileInfo file_info;   // offset 0x10, size 0x3C
    } dvd;                              // offset 0x0, size 0x4C
    struct {
        struct VFSysDeviceBase device;  // offset 0x0, size 0x10
        struct NANDFileInfo file_info;  // offset 0x10, size 0x8C
        // char file_info[0x8C];  // offset 0x10, size 0x8C
    } nand;  // offset 0x0, size 0x9C
    struct {
        struct VF_HANDLE_DEVICE base;  // offset 0x0, size 0x10
        // _SDDev is never used in this file, so we can just pad the field.
        // struct _SDDev drive;         // offset 0x20, size 0x28
        char drive[0x28];               // offset 0x20, size 0x28
        void (*eventCallback)(u32);     // offset 0x48, size 0x4
        u32 slotNo;                     // offset 0x4C, size 0x4
    } sd_device;                        // offset 0x0, size 0x60
    struct VFSysDeviceBase ram_device;  // offset 0x0, size 0x10
};

s32 VFSysSetSyncMode(s32 i_handle_idx, u32 i_mode);

static union VFSysDeviceTableEntry* l_vfsys_dev_table[26];
static struct PDM_INIT_DISK l_dev_init_info_table[26];

static struct PDM_INIT_DISK l_dev_nandflash_init_info;

static struct VF_HANDLE_TYPE* l_sys_handle_table_p;
static s32 l_vfsys_dev_table_init;
static struct MEMiHeapHead* l_vfsys_exp_heap_handle;
static s32 l_vfsys_last_err;
static u32 l_vfsys_vol_max;

extern s32 VF_nand_retry_max;
extern s32 VF_nand_sleep_msec;

static s32 (*l_check_exist_file[4])(struct VF_HANDLE_DEVICE*, const s8*, void*, u32) = {VFSysCheckExistPrfFile_nandflash, VFSysCheckExistPrfFile_ram,
                                                                                        VFSysCheckExistPrfFile_dvd, NULL};

static void VFSys_change_separater(s8* o_dst_str_p, const s8* i_src_str_p, s8 i_dst_spr, s8 i_src_spr, s32 i_str_len_max) {
    s32 i;

    VFipf_memset(o_dst_str_p, 0, (u32)i_str_len_max);
    for (i = 0; i < i_str_len_max && *i_src_str_p != '\0'; i++) {
        if (*i_src_str_p == i_src_spr) {
            *o_dst_str_p = i_dst_spr;
        } else {
            *o_dst_str_p = *i_src_str_p;
        }
        i_src_str_p++;
        o_dst_str_p++;
    }
}

void VFSysSetLastError(s32 i_err) {
    if (i_err != 0) {
        l_vfsys_last_err = i_err;
    }
}

static s32 VFSysGetLastErr(void) {
    return l_vfsys_last_err;
}

static void* VFSys_alloc_common(u32 i_size, u32 i_align, struct MEMiHeapHead* i_heap_handle) {
    if (i_heap_handle == NULL) {
        return NULL;
    }
    return MEMAllocFromExpHeapEx(i_heap_handle, i_size, i_align);
}

static void* VFSys_alloc(u32 i_size, u32 i_align) {
    return VFSys_alloc_common(i_size, i_align, l_vfsys_exp_heap_handle);
}

static void VFSys_free_common(void* i_ptr, struct MEMiHeapHead* i_heap_handle) {
    if (i_heap_handle != NULL) {
        MEMFreeToExpHeap(i_heap_handle, i_ptr);
    }
}

static void VFSys_free(void* i_ptr) {
    VFSys_free_common(i_ptr, l_vfsys_exp_heap_handle);
}

static void VFSys_clear_cache(struct VF_HANDLE_CACHE* i_cache_p) {
    if (i_cache_p != NULL) {
        i_cache_p->heap_handle = NULL;
        i_cache_p->cache_pages = 0;
        i_cache_p->pf_cache_page_p = NULL;
        i_cache_p->pf_cache_buf_p = NULL;
    }
}

static void VFSys_free_cache(struct VF_HANDLE_CACHE* i_cache_p) {
    if (i_cache_p != NULL) {
        if (i_cache_p->pf_cache_page_p != NULL) {
            VFSys_free_common(i_cache_p->pf_cache_page_p, i_cache_p->heap_handle);
        }
        if (i_cache_p->pf_cache_buf_p != NULL) {
            VFSys_free_common(i_cache_p->pf_cache_buf_p, i_cache_p->heap_handle);
        }
        VFSys_clear_cache(i_cache_p);
    }
}

static void VFSys_create_cache(struct VF_HANDLE_TYPE* i_handle_p, u32 i_pages, struct MEMiHeapHead* i_heap_handle) {
    struct VF_HANDLE_CACHE* cache_p;
    u32 pages_2;
    u32 cache_page_size;
    u32 cache_buf_size;

    if (i_handle_p != NULL) {
        cache_p = &i_handle_p->drive.cache;
        pages_2 = i_pages * 2;
        if (cache_p->pf_cache_page_p == NULL && cache_p->pf_cache_buf_p == NULL) {
            cache_buf_size = pages_2 * 512;
            cache_page_size = pages_2 * sizeof(struct PF_CACHE_PAGE);
            cache_p->pf_cache_page_p = VFSys_alloc_common(cache_page_size, 4, i_heap_handle);
            cache_p->pf_cache_buf_p = VFSys_alloc_common(cache_buf_size, 4, i_heap_handle);

            if (cache_p->pf_cache_page_p != NULL && cache_p->pf_cache_buf_p != NULL) {
                VFipf_memset(cache_p->pf_cache_page_p, 0, cache_page_size);
                VFipf_memset(cache_p->pf_cache_buf_p, 0, cache_buf_size);
                cache_p->heap_handle = i_heap_handle;
                cache_p->cache_pages = i_pages;
            } else {
                if (cache_p != NULL) {
                    if (cache_p->pf_cache_page_p != NULL && cache_p->heap_handle != NULL) {
                        MEMFreeToExpHeap(cache_p->heap_handle, cache_p->pf_cache_page_p);
                    }
                    if (cache_p->pf_cache_buf_p != NULL && cache_p->heap_handle != NULL) {
                        MEMFreeToExpHeap(cache_p->heap_handle, cache_p->pf_cache_buf_p);
                    }
                    if (cache_p != NULL) {
                        cache_p->heap_handle = NULL;
                        cache_p->cache_pages = 0;
                        cache_p->pf_cache_page_p = NULL;
                        cache_p->pf_cache_buf_p = NULL;
                    }
                }
            }
        }
    }
}

static void VFSys_alloc_cache(struct VF_HANDLE_TYPE* i_handle_p, void* i_cache_heap_p, u32 i_cache_size) {
    u32 pages;
    struct MEMiHeapHead* heap_handle;
    struct MEMiHeapHead* cache_heap_handle;

    if (i_handle_p != NULL) {
        heap_handle = l_vfsys_exp_heap_handle;
        pages = 8;
        if (i_cache_heap_p != NULL && i_cache_size > 0x2800) {
            cache_heap_handle = MEMCreateExpHeapEx(i_cache_heap_p, i_cache_size, 0);
            if (cache_heap_handle != NULL) {
                pages = i_cache_size / 1280;
                heap_handle = cache_heap_handle;
            }
        }
        VFSys_create_cache(i_handle_p, pages, heap_handle);
    }
}

static void VFSys_destroy_cache(struct VF_HANDLE_TYPE* i_handle_p) {
    struct VF_HANDLE_CACHE* cache_p;
    struct MEMiHeapHead* heap_handle;

    if (i_handle_p != NULL) {
        cache_p = &i_handle_p->drive.cache;
        heap_handle = cache_p->heap_handle;
        VFSys_free_cache(cache_p);
        if (heap_handle != l_vfsys_exp_heap_handle && heap_handle != NULL) {
            MEMDestroyExpHeap(heap_handle);
        }
    }
}

static void VFSys_destroy_cache_all(void) {
    struct VF_HANDLE_TYPE* handle_p;
    struct VF_HANDLE_TYPE* handle_end_p;

    if (l_vfsys_vol_max != 0 && l_sys_handle_table_p != NULL) {
        handle_p = l_sys_handle_table_p;
    } else {
        handle_p = NULL;
    }
    if (handle_p != NULL) {
        handle_end_p = handle_p + l_vfsys_vol_max;
        for (; handle_p != handle_end_p; handle_p++) {
            VFSys_destroy_cache(handle_p);
        }
    }
}

static void VFSys_clear_handle(struct VF_HANDLE_TYPE* o_handle_p) {
    VFipf_memset(o_handle_p, 0, sizeof(struct VF_HANDLE_TYPE));
    VFSys_clear_cache(&o_handle_p->drive.cache);
}

static void VFSys_clear_all_handle(void) {
    struct VF_HANDLE_TYPE* handle_p;
    struct VF_HANDLE_TYPE* handle_end_p;

    if (l_vfsys_vol_max != 0 && l_sys_handle_table_p != NULL) {
        handle_p = l_sys_handle_table_p;
    } else {
        handle_p = NULL;
    }
    handle_end_p = handle_p + l_vfsys_vol_max;
    for (; handle_p != handle_end_p; handle_p++) {
        VFSys_clear_handle(handle_p);
    }
}

static s32 VFSys_get_free_handle_idx(void) {
    struct VF_HANDLE_TYPE* handle_p;
    struct VF_HANDLE_TYPE* handle_end_p;
    s32 idx;

    if (l_vfsys_vol_max != 0 && l_sys_handle_table_p != NULL) {
        handle_p = l_sys_handle_table_p;
    } else {
        handle_p = NULL;
    }
    handle_end_p = handle_p + l_vfsys_vol_max;
    idx = 0;
    for (; handle_p != handle_end_p; handle_p++, idx++) {
        if (handle_p->device_p == NULL) {
            return idx;
        }
    }
    return -1;
}

static void VFSys_create_handle_table(u32 i_max) {
    if (l_sys_handle_table_p == NULL) {
        l_sys_handle_table_p = VFSys_alloc(i_max * sizeof(struct VF_HANDLE_TYPE), 32);
    }
}

static void VFSys_destroy_handle_table(void) {
    if (l_sys_handle_table_p != NULL) {
        VFSys_destroy_cache_all();
        VFSys_free(l_sys_handle_table_p);
        l_sys_handle_table_p = NULL;
    }
}

struct VF_HANDLE_TYPE* VFSysGetHandleP(s32 i_idx) {
    if ((u32)i_idx < l_vfsys_vol_max && l_sys_handle_table_p != NULL) {
        return &l_sys_handle_table_p[i_idx];
    }
    return NULL;
}

s32 VFSysHandleP2Idx(struct VF_HANDLE_TYPE* i_handle_p) {
    struct VF_HANDLE_TYPE* handle_p;
    struct VF_HANDLE_TYPE* handle_end_p;
    s32 idx;

    if (i_handle_p != NULL) {
        if (l_vfsys_vol_max != 0 && l_sys_handle_table_p != NULL) {
            handle_p = l_sys_handle_table_p;
        } else {
            handle_p = NULL;
        }
        handle_end_p = handle_p + l_vfsys_vol_max;
        idx = 0;
        for (; handle_p != handle_end_p; handle_p++, idx++) {
            if (i_handle_p == handle_p) {
                return idx;
            }
        }
    }
    return -1;
}

struct VF_HANDLE_TYPE* VFSysVol2HandleP(struct PF_VOLUME* i_vol_p) {
    struct VF_HANDLE_TYPE* handle_p;
    struct VF_HANDLE_TYPE* handle_end_p;

    if (i_vol_p != NULL) {
        if (l_vfsys_vol_max != 0 && l_sys_handle_table_p != NULL) {
            handle_p = l_sys_handle_table_p;
        } else {
            handle_p = NULL;
        }
        handle_end_p = handle_p + l_vfsys_vol_max;
        while (handle_p != handle_end_p) {
            if (handle_p->device_p != NULL && handle_p->drive.pf_drv.drive == i_vol_p->drv_char) {
                return handle_p;
            }
            handle_p++;
        }
    }
    return NULL;
}

static struct VF_HANDLE_TYPE* VFSys_file_p_2_handle_p(struct PF_FILE* i_file_p) {
    struct VF_HANDLE_TYPE* ret_p;

    ret_p = NULL;
    if (i_file_p != NULL && i_file_p->p_sfd != NULL) {
        ret_p = VFSysVol2HandleP(i_file_p->p_sfd->ffd.p_vol);
    }
    return ret_p;
}

static struct VF_HANDLE_TYPE* VFSys_get_current_handle_p(void) {
    struct PF_VOLUME* vol_p;

    vol_p = VFiPFVOL_GetCurrentVolume();
    return VFSysVol2HandleP(vol_p);
}

int VFSysPDMDisk2HandleIdx(const struct PDM_DISK* i_disk_p) {
    int ret_handle_idx;
    struct VF_HANDLE_TYPE* handle_p;
    int idx;

    ret_handle_idx = -1;
    if (i_disk_p != NULL) {
        if (l_vfsys_vol_max != 0 && l_sys_handle_table_p != NULL) {
            handle_p = l_sys_handle_table_p;
        } else {
            handle_p = NULL;
        }
        idx = 0;
        for (; (u32)idx < l_vfsys_vol_max; handle_p++, idx++) {
            if (i_disk_p == handle_p->drive.pf_disk_p) {
                ret_handle_idx = idx;
                break;
            }
        }
    }
    return ret_handle_idx;
}

static void VFSys_clear_device_info(struct VF_HANDLE_DEVICE* o_dev_p) {
    VFipf_memset(o_dev_p, 0, sizeof(struct VF_HANDLE_DEVICE));
    o_dev_p->status = 0;
    o_dev_p->sync_mode = 0;
}

static void VFSys_clear_device_nandflash(struct VFSysDeviceBase* o_nandflash_p) {
    VFSys_clear_device_info(&o_nandflash_p->base);
}

struct VF_HANDLE_DRIVE* VFSysGetDriveP(s32 i_idx) {
    struct VF_HANDLE_TYPE* handle_p;

    handle_p = VFSysGetHandleP(i_idx);
    if (handle_p != NULL) {
        return &handle_p->drive;
    }
    return NULL;
}

struct VF_HANDLE_DRIVE* VFSysPDMDisk2DriveP(struct PDM_DISK* disk_p) {
    if (disk_p != NULL) {
        return VFSysGetDriveP(dCommon_getHandleIdxFromDisk(disk_p));
    }
    return NULL;
}

inline void VFSys_set_cache(struct VF_HANDLE_DRIVE* i_drive_p) {
    struct PF_CACHE_SETTING* cache_p;

    cache_p = &i_drive_p->pf_cache_set;
    cache_p->pages = i_drive_p->cache.pf_cache_page_p;
    cache_p->buffers = i_drive_p->cache.pf_cache_buf_p;
    cache_p->num_fat_pages = (u16)i_drive_p->cache.cache_pages;
    cache_p->num_data_pages = (u16)i_drive_p->cache.cache_pages;
    i_drive_p->pf_cache_set.num_fat_buf_size = 1;
}

static void VFSys_create_heap(void* i_start_address_p, u32 i_size) {
    if (i_start_address_p != NULL && i_size != 0 && l_vfsys_exp_heap_handle == NULL) {
        l_vfsys_exp_heap_handle = MEMCreateExpHeapEx(i_start_address_p, i_size, 0);
    }
}

static void VFSys_create_device_table(u32 i_max) {
    int i;

    if (l_vfsys_dev_table_init == 0) {
        for (i = 0; i < i_max; i++) {
            // l_vfsys_dev_table[i] = VFSys_alloc(sizeof(union VFSysDeviceTableEntry), 32);
            l_vfsys_dev_table[i] = VFSys_alloc(160, 32);
        }
        l_vfsys_dev_table_init = 1;
    }
}

static void VFSys_destroy_device_table(u32 i_max) {
    int i;

    if (l_vfsys_dev_table_init != 0) {
        for (i = 0; i < i_max; i++) {
            if (l_vfsys_dev_table[i] != NULL) {
                VFSys_free(l_vfsys_dev_table[i]);
                l_vfsys_dev_table[i] = NULL;
            }
        }
        l_vfsys_dev_table_init = 0;
    }
}

void VFSysInit(void* i_heap_start_address_p, u32 i_size) {
    u32 vol_max;

    vol_max = i_size >> 14;
    VFSys_create_heap(i_heap_start_address_p, i_size);
    vol_max = (vol_max > 26) ? 26 : vol_max;
    l_vfsys_vol_max = vol_max;

    VFSys_create_handle_table(vol_max);
    VFSys_clear_all_handle();
    VFSys_create_device_table(l_vfsys_vol_max);

    dCommon_initDriveInfo();
    l_vfsys_last_err = 0;
    VF_nand_sleep_msec = 2;
    VF_nand_retry_max = 8;
    VFi_InitSDWrok();
}

static void VFSys_set_device_err_info(struct VF_HANDLE_TYPE* o_handle_p, s32 i_err) {
    if (o_handle_p != NULL && o_handle_p->drive.pf_disk_p != NULL) {
        dCommon_setLastDeviceErrorToDisk(o_handle_p->drive.pf_disk_p, i_err);
    }
}

static void VFSysSetDevErrInfo(s32 i_handle_idx, s32 i_err) {
    struct VF_HANDLE_TYPE* handle_p;

    handle_p = VFSysGetHandleP(i_handle_idx);
    if (handle_p != NULL) {
        VFSys_set_device_err_info(handle_p, i_err);
    }
}

static void VFSys_set_current_device_err_info(s32 i_err) {
    struct VF_HANDLE_TYPE* handle_p;

    handle_p = VFSys_get_current_handle_p();
    VFSys_set_device_err_info(handle_p, i_err);
}

static void VFSys_file_p_2_set_device_err_info(struct PF_FILE* i_file_p, s32 i_err) {
    struct VF_HANDLE_TYPE* handle_p;

    handle_p = VFSys_file_p_2_handle_p(i_file_p);
    VFSys_set_device_err_info(handle_p, i_err);
}

s32 VFSysSetDeviceNANDFlash(s32* o_idx_p, void* i_cache_heap_p, u32 i_cache_size) {
    s32 idx;
    struct VF_HANDLE_TYPE* handle_p;
    s32 err;
    struct PDM_INIT_DISK* init_info_p;
    union VFSysDeviceTableEntry* uni_p;
    struct VFSysDeviceBase* nandflash_p;

    idx = VFSys_get_free_handle_idx();
    handle_p = VFSysGetHandleP(idx);
    *o_idx_p = -1;

    if (handle_p == NULL) {
        err = 0xB002;
    } else {
        init_info_p = &l_dev_init_info_table[idx];
        VFipf_memcpy(init_info_p, &l_dev_nandflash_init_info, sizeof(struct PDM_INIT_DISK));
        init_info_p->ui_ext = idx;

        err = VFipdm_open_disk(init_info_p, &handle_p->drive.pf_disk_p);
        if (err == 0) {
            uni_p = l_vfsys_dev_table[idx];
            nandflash_p = &uni_p->nand.device;
            VFSys_clear_device_nandflash(nandflash_p);
            nandflash_p->base.type = 0;
            handle_p->device_p = &nandflash_p->base;
            *o_idx_p = idx;
            VFSys_set_device_err_info(handle_p, 0);
            VFSys_alloc_cache(handle_p, i_cache_heap_p, i_cache_size);
            err = 0;
        } else {
            VFSys_clear_handle(handle_p);
            err = VFipf2_errnum();
        }
    }
    return err;
}

s32 VFSysUnsetDevice(s32 i_handle_idx) {
    struct VF_HANDLE_TYPE* handle_p;
    s32 err;

    handle_p = VFSysGetHandleP(i_handle_idx);
    if (handle_p == NULL || handle_p->drive.pf_disk_p == NULL) {
        err = 0xB003;
    } else {
        VFSys_set_device_err_info(handle_p, 0);
        err = VFipdm_close_disk(handle_p->drive.pf_disk_p);
        if (err != 0) {
            err = VFipf2_errnum();
        } else {
            VFSys_destroy_cache(handle_p);
            VFSys_clear_handle(handle_p);
            err = 0;
        }
    }
    return err;
}

s32 VFSysCheckExistPrfFile_nandflash_sub(const s8* i_prf_file_name_p, u32 i_handle_idx) {
    s32 NANDError;
    s32 NANDErrorLen;
    struct NANDFileInfo FileInfo ATTR_ALIGN(32);
    struct PR_BINHEADER header ATTR_ALIGN(32);
    s32 ret;
    s8 file_name[255];
    u32 length;

    ret = 0xB001;
    VFSys_change_separater(file_name, i_prf_file_name_p, '/', '\\', 255);
    NANDError = VFi_NandOpenSp((const char*)file_name, &FileInfo, 1, i_handle_idx);
    if (NANDError == 0) {
        length = 0;
        NANDErrorLen = VFi_NandGetLength(&FileInfo, &length);
        NANDError = VFi_NandRead(&FileInfo, &header, sizeof(struct PR_BINHEADER));
        if (NANDError >= 0 && NANDErrorLen >= 0 && dCommon_IsPrfFile(&header) != 0 &&
            length ==
                (((u32)header.fileSize[3] | ((u32)header.fileSize[2] << 8)) | (((u32)header.fileSize[0] << 24) | ((u32)header.fileSize[1] << 16)))) {
            ret = 0;
        } else {
            ret = 0xB006;
        }
        VFi_NandClose(&FileInfo);
    }
    return ret;
}

static s32 VFSysCheckExistPrfFile_nandflash(struct VF_HANDLE_DEVICE* i_device_p, const s8* i_prf_file_name_p, void* i_memory_p, u32 i_handle_idx) {
    return VFSysCheckExistPrfFile_nandflash_sub(i_prf_file_name_p, i_handle_idx);
}

static s32 VFSysCheckExistPrfFile_ram_sub(void* i_memory_p) {
    s32 ret;
    if (dCommon_IsPrfFile(i_memory_p)) {
        return 0;
    }
    return 0xB006;
}

static s32 VFSysCheckExistPrfFile_ram(struct VF_HANDLE_DEVICE* i_device_p, const s8* i_prf_file_name_p, void* i_memory_p, u32 i_handle_idx) {
    return VFSysCheckExistPrfFile_ram_sub(i_memory_p);
}

static s32 VFSysCheckExistPrfFile_dvd_sub(const s8* i_prf_file_name_p) {
    s32 DVDError;
    struct DVDFileInfo FileInfo;
    struct PR_BINHEADER header ATTR_ALIGN(32);
    s32 ret;
    s8 file_name[255];
    s32 readBytes;
    u32 dvdFileSize;

    ret = 0xB001;
    VFSys_change_separater(file_name, i_prf_file_name_p, '\\', '/', 255);
    DVDError = DVDOpen((const char*)file_name, &FileInfo);
    if (DVDError != 0) {
        readBytes = DVDReadPrio(&FileInfo, &header, 0x20, 0, 2);
        dvdFileSize = FileInfo.length;
        if (readBytes != 0 && dCommon_IsPrfFile(&header) != 0 &&
            dvdFileSize ==
                (((u32)header.fileSize[3] | ((u32)header.fileSize[2] << 8)) | (((u32)header.fileSize[0] << 24) | ((u32)header.fileSize[1] << 16)))) {
            ret = 0;
        } else {
            ret = 0xB006;
        }
        DVDClose(&FileInfo);
    }
    return ret;
}

static s32 VFSysCheckExistPrfFile_dvd(struct VF_HANDLE_DEVICE* i_device_p, const s8* i_prf_file_name_p, void* i_memory_p, u32 i_handle_idx) {
    return VFSysCheckExistPrfFile_dvd_sub(i_prf_file_name_p);
}

s32 VFSysCheckExistPrfFile(s32 i_handle_idx, const s8* i_prf_file_name_p, void* i_memory_p) {
    struct VF_HANDLE_TYPE* handle_p;
    u32 type;
    s32 (*check_proc)(struct VF_HANDLE_DEVICE*, const s8*, void*, u32);

    handle_p = VFSysGetHandleP(i_handle_idx);
    if (handle_p == NULL || handle_p->device_p == NULL) {
        return 0xB003;
    }
    type = handle_p->device_p->type;
    if (type < 4) {
        check_proc = l_check_exist_file[type];
        if (check_proc != NULL) {
            return check_proc(handle_p->device_p, i_prf_file_name_p, i_memory_p, i_handle_idx);
        }
    } else {
        return -1;
    }
    return 0;
}

s32 VFSysMountDrv(s32 i_handle_idx, const s8* i_prf_file_name_p, void* i_memory_p) {
    struct VF_HANDLE_TYPE* handle_p;
    struct VF_HANDLE_DRIVE* drive_p;
    s32 err;
    union VFSysDeviceTableEntry* uni_p;

    handle_p = VFSysGetHandleP(i_handle_idx);
    if (handle_p == NULL || handle_p->device_p == NULL) {
        return 0xB003;
    }
    drive_p = &handle_p->drive;
    if (drive_p->pf_disk_p == NULL) {
        return 0xB003;
    }
    if (drive_p->pf_drv.cache != NULL) {
        return 0xB005;
    }

    VFSys_set_device_err_info(handle_p, 0);
    VFSys_set_cache(drive_p);

    err = VFipdm_open_partition(drive_p->pf_disk_p, 0, &drive_p->pf_part_p);
    if (err == 0) {
        drive_p->pf_drv.p_part = drive_p->pf_part_p;
        drive_p->pf_drv.cache = &drive_p->pf_cache_set;

        switch (handle_p->device_p->type) {
        case 1:
            drive_p->file_p = i_memory_p;
            break;
        case 0:
            uni_p = l_vfsys_dev_table[i_handle_idx];
            drive_p->file_p = &uni_p->dvd.file_info;
            VFSys_change_separater((s8*)drive_p->pf_filename, i_prf_file_name_p, '/', '\\', 255);
            break;
        case 2:
            uni_p = l_vfsys_dev_table[i_handle_idx];
            drive_p->file_p = &uni_p->nand.file_info;
            VFSys_change_separater((s8*)drive_p->pf_filename, i_prf_file_name_p, '\\', '/', 255);
            break;

        case 3:
            break;
        default:
            VFSys_change_separater((s8*)drive_p->pf_filename, i_prf_file_name_p, '\\', '/', 255);
            break;
        }

        dCommon_setLastDeviceErrorToDisk(drive_p->pf_disk_p, 0);
        dCommon_setFileSizeToDisk(drive_p->pf_disk_p, 0x19000);

        return 0;
    }
    return VFipf2_errnum();
}

s32 VFSysUnmountDrv(s32 i_handle_idx, u32 i_mode) {
    struct VF_HANDLE_TYPE* handle_p;
    struct VF_HANDLE_DRIVE* drive_p;
    s32 err;

    handle_p = VFSysGetHandleP(i_handle_idx);
    if (handle_p == NULL || handle_p->device_p == NULL) {
        return 0xB003;
    }
    drive_p = &handle_p->drive;
    if (drive_p->pf_disk_p == NULL) {
        return 0xB003;
    }

    VFSys_set_device_err_info(handle_p, 0);
    err = VFipf2_unmount(drive_p->pf_drv.drive, i_mode);

    if ((i_mode == 0 && err != 0) || (i_mode == 1 && err != 0 && err != 1)) {
        return VFipf2_errnum();
    }

    err = VFipf2_detach(drive_p->pf_drv.drive);
    if (err != 0) {
        return VFipf2_errnum();
    }

    err = VFipdm_close_partition(drive_p->pf_part_p);
    if (err == 0) {
        dCommon_setFileSizeToDisk(drive_p->pf_disk_p, 0);
        dCommon_setLastDeviceErrorToDisk(drive_p->pf_disk_p, 0);
        drive_p->pf_drv.p_part = NULL;
        drive_p->pf_drv.cache = NULL;
        drive_p->file_p = NULL;
        VFipf_memset(drive_p->pf_filename, 0, 255);
        return 0;
    }

    return VFipf2_errnum();
}

static s32 VFSys_set_current_vol(s32 i_handle_idx) {
    struct VF_HANDLE_TYPE* handle_p;
    struct PF_VOLUME* vol_p;
    s32 err;

    handle_p = VFSysGetHandleP(i_handle_idx);
    if (handle_p != NULL) {
        vol_p = VFiPFVOL_GetVolumeFromDrvChar(handle_p->drive.pf_drv.drive);
        if (vol_p != NULL) {
            VFiPFVOL_SetCurrentVolume(vol_p);
            err = 0;
        } else {
            err = VFipf2_errnum();
            if (err == 0) {
                return -1;
            }
            return err;
        }
    } else {
        err = 0xB003;
    }
    return err;
}

static struct PF_FILE* VFSysOpenFile_common(const s8* i_path_p, const s8* i_mode) {
    struct PF_FILE* ret_p;

    ret_p = VFipf2_fopen(i_path_p, i_mode);
    if (ret_p == NULL) {
        VFSysSetLastError(VFipf2_errnum());
    }
    return ret_p;
}

struct PF_FILE* VFSysOpenFile_current(const s8* i_path_p, const s8* i_mode) {
    VFSys_set_current_device_err_info(0);
    return VFSysOpenFile_common(i_path_p, i_mode);
}

struct PF_FILE* VFSysOpenFile(s32 i_handle_idx, const s8* i_path_p, const s8* i_mode) {
    s32 err;

    err = VFSys_set_current_vol(i_handle_idx);
    if (err == 0) {
        VFSysSetDevErrInfo(i_handle_idx, 0);
        return VFSysOpenFile_common(i_path_p, i_mode);
    } else {
        VFSysSetLastError(err);
        return NULL;
    }
}

s32 VFSysCloseFile(struct PF_FILE* i_file_p) {
    s32 err;

    VFSys_file_p_2_set_device_err_info(i_file_p, 0);
    err = VFipf2_fclose(i_file_p);
    if (err == 0) {
        return 0;
    }
    return VFipf2_errnum();
}

s32 VFSysSeekFile(struct PF_FILE* i_file_p, s32 i_offset, s32 i_origin) {
    s32 err;

    VFSys_file_p_2_set_device_err_info(i_file_p, 0);
    err = VFipf2_fseek(i_file_p, i_offset, i_origin);
    if (err == 0) {
        return 0;
    }
    return VFipf2_errnum();
}

s32 VFSysReadFile(u32* o_read_size_p, void* o_buf_p, u32 i_size, struct PF_FILE* i_file_p) {
    struct PF_INFO info;
    s32 rest;
    s32 err;

    if (o_read_size_p != NULL) {
        *o_read_size_p = 0;
    }

    err = VFipf2_finfo(i_file_p, &info);
    if (err == 0) {
        rest = info.file_size - i_file_p->cursor.position;
        if (i_size > rest) {
            VFipf_memset(o_buf_p, 0, i_size);
            i_size = rest;
        }
        VFSys_file_p_2_set_device_err_info(i_file_p, 0);
        if (i_size != 0) {
            err = VFipf2_fread(o_buf_p, i_size, 1, i_file_p);
            if ((u32)err == 1) {
                if (o_read_size_p != NULL) {
                    *o_read_size_p = i_size;
                }
                return 0;
            } else {
                return VFipf2_errnum();
            }
        }
        return 0;
    } else {
        return VFipf2_errnum();
    }
}

s32 VFSysWriteFile(void* i_buf_p, u32 i_size, struct PF_FILE* i_file_p) {
    VFSys_file_p_2_set_device_err_info(i_file_p, 0);
    if (VFipf2_fwrite(i_buf_p, i_size, 1, i_file_p) == 0x1) {
        return 0;
    }
    return VFipf2_errnum();
}

static s32 VFSysDeleteFile_common(const s8* i_path_p) {
    s32 err;

    err = VFipf2_remove(i_path_p);
    if (err == 0) {
        return 0;
    }
    return VFipf2_errnum();
}

s32 VFSysDeleteFile_current(const s8* i_path_p) {
    VFSys_set_current_device_err_info(0);
    return VFSysDeleteFile_common(i_path_p);
}

s32 VFSysDeleteFile(s32 i_handle_idx, const s8* i_path_p) {
    s32 err;

    err = VFSys_set_current_vol(i_handle_idx);
    if (err == 0) {
        VFSysSetDevErrInfo(i_handle_idx, 0);
        err = VFSysDeleteFile_common(i_path_p);
    }
    return err;
}

static s32 VFSysCreateDir_common(const s8* i_dir_name_p) {
    s32 err;

    err = VFipf2_mkdir(i_dir_name_p);
    if (err == 0) {
        return 0;
    }
    return VFipf2_errnum();
}

s32 VFSysGetFileSizeByFd(s32* o_size_p, struct PF_FILE* i_file_p) {
    struct PF_INFO info;
    s32 err;

    *o_size_p = -1;
    if (i_file_p != NULL) {
        err = VFipf2_finfo(i_file_p, &info);
        if (err == 0) {
            *o_size_p = info.file_size;
            return 0;
        }
        return VFipf2_errnum();
    }
    return -1;
}

s32 VFSysGetDriveFreeSize(s32 i_handle_idx) {
    struct VF_HANDLE_TYPE* handle_p;
    struct PF_DEV_INF device_info;
    s32 err;

    handle_p = VFSysGetHandleP(i_handle_idx);
    if (handle_p == NULL || handle_p->device_p == NULL) {
        VFSysSetLastError(0xB003);
        return -1;
    }
    if (handle_p->drive.pf_disk_p == NULL) {
        VFSysSetLastError(0xB003);
        return -1;
    }

    VFSys_set_device_err_info(handle_p, 0);
    err = VFipf2_devinf(handle_p->drive.pf_drv.drive, &device_info);
    if (err != 0) {
        err = VFipf2_errnum();
        VFSysSetLastError(err);
        return -1;
    } else {
        return device_info.ecl * device_info.bps * device_info.spc;
    }
}

s32 VFSysGetLastError(void) {
    return VFSysGetLastErr();
}

static s32 VFSysGetLastDeviceError_common(struct VF_HANDLE_TYPE* i_handle_p) {
    if (i_handle_p != NULL && i_handle_p->drive.pf_disk_p != NULL) {
        return dCommon_getLastDeviceErrorFromDisk(i_handle_p->drive.pf_disk_p);
    }
    return -1;
}

s32 VFSysGetLastDeviceError_current(void) {
    struct VF_HANDLE_TYPE* handle_p;

    handle_p = VFSys_get_current_handle_p();
    return VFSysGetLastDeviceError_common(handle_p);
}

s32 VFSysGetLastDeviceError(s32 i_handle_idx) {
    struct VF_HANDLE_TYPE* handle_p;

    handle_p = VFSysGetHandleP(i_handle_idx);
    return VFSysGetLastDeviceError_common(handle_p);
}

void VFSysSetNandFuncEx(u32 i_handle_idx) {
    VFi_NandSetNANDFuncNormal(i_handle_idx);
}

s32 VFSysSetSyncMode(s32 i_handle_idx, u32 i_mode) {
    struct VF_HANDLE_TYPE* handle_p;

    handle_p = VFSysGetHandleP(i_handle_idx);
    if (handle_p == NULL || handle_p->drive.pf_disk_p == NULL || handle_p->device_p == NULL) {
        return 0xB003;
    }
    if (i_mode <= 1) {
        handle_p->device_p->sync_mode = i_mode;
        return 0;
    }
    return -1;
}
