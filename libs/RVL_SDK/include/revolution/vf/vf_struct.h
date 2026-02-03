#ifndef VF_STRUCT_H
#define VF_STRUCT_H

#include "revolution/types.h"

typedef struct PF_CURSOR {
    unsigned long position;
    unsigned long sector;
    unsigned long file_sector_index;
    unsigned short offset_in_sector;
} PF_CURSOR;

typedef struct PF_BPB {
    // total size: 0x38
    unsigned short bytes_per_sector;      // offset 0x0, size 0x2
    unsigned short num_reserved_sectors;  // offset 0x2, size 0x2
    unsigned short num_root_dir_entries;  // offset 0x4, size 0x2
    unsigned char sectors_per_cluster;    // offset 0x6, size 0x1
    unsigned char num_FATs;               // offset 0x7, size 0x1
    unsigned long total_sectors;          // offset 0x8, size 0x4
    unsigned long sectors_per_FAT;        // offset 0xC, size 0x4
    unsigned long root_dir_cluster;       // offset 0x10, size 0x4
    unsigned short fs_info_sector;        // offset 0x14, size 0x2
    unsigned short backup_boot_sector;    // offset 0x16, size 0x2
    unsigned short ext_flags;             // offset 0x18, size 0x2
    unsigned char media;                  // offset 0x1A, size 0x1
    enum /* @enum$142pf_volume_c */ {
        FAT_12 = 0,
        FAT_16 = 1,
        FAT_32 = 2,
        FAT_ERR = -1,
    } fat_type;                              // offset 0x1C, size 0x4
    unsigned char log2_bytes_per_sector;     // offset 0x20, size 0x1
    unsigned char log2_sectors_per_cluster;  // offset 0x21, size 0x1
    unsigned char num_active_FATs;           // offset 0x22, size 0x1
    unsigned short num_root_dir_sectors;     // offset 0x24, size 0x2
    unsigned long active_FAT_sector;         // offset 0x28, size 0x4
    unsigned long first_root_dir_sector;     // offset 0x2C, size 0x4
    unsigned long first_data_sector;         // offset 0x30, size 0x4
    unsigned long num_clusters;              // offset 0x34, size 0x4
} PF_BPB;

typedef struct PDM_BPB {
    // total size: 0x70
    unsigned char oem_name[8];            // offset 0x0, size 0x8
    unsigned short bytes_per_sector;      // offset 0x8, size 0x2
    unsigned char sectors_per_cluster;    // offset 0xA, size 0x1
    unsigned char num_FATs;               // offset 0xB, size 0x1
    unsigned short num_reserved_sectors;  // offset 0xC, size 0x2
    unsigned short num_root_dir_entries;  // offset 0xE, size 0x2
    unsigned short total_sectors16;       // offset 0x10, size 0x2
    unsigned short sectors_per_FAT16;     // offset 0x12, size 0x2
    unsigned short sector_per_track;      // offset 0x14, size 0x2
    unsigned long num_hidden_sectors;     // offset 0x18, size 0x4
    unsigned long total_sectors32;        // offset 0x1C, size 0x4
    unsigned short num_heads;             // offset 0x20, size 0x2
    unsigned char media;                  // offset 0x22, size 0x1
    unsigned char drive;                  // offset 0x23, size 0x1
    unsigned long vol_id;                 // offset 0x24, size 0x4
    unsigned char boot_sig;               // offset 0x28, size 0x1
    unsigned char vol_label[11];          // offset 0x29, size 0xB
    unsigned char fs_type[8];             // offset 0x34, size 0x8
    unsigned long sectors_per_FAT32;      // offset 0x3C, size 0x4
    unsigned short ext_flags;             // offset 0x40, size 0x2
    unsigned short fs_version;            // offset 0x42, size 0x2
    unsigned long root_dir_cluster;       // offset 0x44, size 0x4
    unsigned short fs_info_sector;        // offset 0x48, size 0x2
    unsigned short backup_boot_sector;    // offset 0x4A, size 0x2
    unsigned char jump_boot[3];           // offset 0x4C, size 0x3
    unsigned char num_active_FATs;        // offset 0x4F, size 0x1
    enum /* @enum$52pdm_bpb_c */ {
        PDM_FAT_12 = 0,
        PDM_FAT_16 = 1,
        PDM_FAT_32 = 2,
        PDM_FAT_ERR = -1,
    } fat_type;                              // offset 0x50, size 0x4
    unsigned char log2_bytes_per_sector;     // offset 0x54, size 0x1
    unsigned char log2_sectors_per_cluster;  // offset 0x55, size 0x1
    unsigned short num_root_dir_sectors;     // offset 0x56, size 0x2
    unsigned long active_FAT_sector;         // offset 0x58, size 0x4
    unsigned long first_root_dir_sector;     // offset 0x5C, size 0x4
    unsigned long first_data_sector;         // offset 0x60, size 0x4
    unsigned long num_clusters;              // offset 0x64, size 0x4
    unsigned long total_sectors;             // offset 0x68, size 0x4
    unsigned long sectors_per_FAT;           // offset 0x6C, size 0x4
} PDM_BPB;

typedef struct PDM_PARTITION {
    // total size: 0x2C
    unsigned long status;                    // offset 0x0, size 0x4
    struct PDM_DISK* p_disk;                 // offset 0x4, size 0x4
    unsigned long signature;                 // offset 0x8, size 0x4
    unsigned short part_id;                  // offset 0xC, size 0x2
    unsigned short open_part_cnt;            // offset 0xE, size 0x2
    struct PDM_PARTITION* part_lock_handle;  // offset 0x10, size 0x4
    unsigned long start_sector;              // offset 0x14, size 0x4
    unsigned long total_sector;              // offset 0x18, size 0x4
    unsigned long mbr_sector;                // offset 0x1C, size 0x4
    unsigned char partition_type;            // offset 0x20, size 0x1
    long driver_last_error;                  // offset 0x24, size 0x4
    void* p_vol;                             // offset 0x28, size 0x4
} PDM_PARTITION;

typedef struct PDM_FSINFO {
    // total size: 0x8
    unsigned long free_count;  // offset 0x0, size 0x4
    unsigned long next_free;   // offset 0x4, size 0x4
} PDM_FSINFO;

typedef struct PDM_DISK_INFO {
    // total size: 0x14
    unsigned long total_sectors;      // offset 0x0, size 0x4
    unsigned short cylinders;         // offset 0x4, size 0x2
    unsigned char heads;              // offset 0x6, size 0x1
    unsigned char sectors_per_track;  // offset 0x7, size 0x1
    unsigned short bytes_per_sector;  // offset 0x8, size 0x2
    unsigned long media_attr;         // offset 0xC, size 0x4
    void* format_param;               // offset 0x10, size 0x4
} PDM_DISK_INFO;

struct PF_LAST_CLUSTER {
    // total size: 0x8
    unsigned long num_last_cluster;  // offset 0x0, size 0x4
    unsigned long max_chain_index;   // offset 0x4, size 0x4
};

typedef struct PF_DIR_ENT {
    // total size: 0x240
    unsigned short long_name[261];    // offset 0x0, size 0x20A
    unsigned char num_entry_LFNs;     // offset 0x20A, size 0x1
    unsigned char ordinal;            // offset 0x20B, size 0x1
    unsigned char check_sum;          // offset 0x20C, size 0x1
    unsigned char align_pad[1];       // offset 0x20D, size 0x1
    signed char short_name[13];       // offset 0x20E, size 0xD
    unsigned char small_letter_flag;  // offset 0x21B, size 0x1
    unsigned char attr;               // offset 0x21C, size 0x1
    unsigned char create_time_ms;     // offset 0x21D, size 0x1
    unsigned short create_time;       // offset 0x21E, size 0x2
    unsigned short create_date;       // offset 0x220, size 0x2
    unsigned short access_date;       // offset 0x222, size 0x2
    unsigned short modify_time;       // offset 0x224, size 0x2
    unsigned short modify_date;       // offset 0x226, size 0x2
    unsigned long file_size;          // offset 0x228, size 0x4
    struct PF_VOLUME* p_vol;          // offset 0x22C, size 0x4
    unsigned long path_len;           // offset 0x230, size 0x4
    unsigned long start_cluster;      // offset 0x234, size 0x4
    unsigned long entry_sector;       // offset 0x238, size 0x4
    unsigned short entry_offset;      // offset 0x23C, size 0x2
} PF_DIR_ENT;

typedef struct PF_FAT_HINT {
    // total size: 0x8
    unsigned long chain_index;  // offset 0x0, size 0x4
    unsigned long cluster;      // offset 0x4, size 0x4
} PF_FAT_HINT;

struct PF_CLUSTER_LINK {
    // total size: 0x14
    unsigned long* buffer;           // offset 0x0, size 0x4
    unsigned short interval;         // offset 0x4, size 0x2
    unsigned short interval_offset;  // offset 0x6, size 0x2
    unsigned long position;          // offset 0x8, size 0x4
    unsigned long max_count;         // offset 0xC, size 0x4
    unsigned long save_index;        // offset 0x10, size 0x4
};

typedef struct PF_FFD {
    // total size: 0x34
    unsigned long start_cluster;          // offset 0x0, size 0x4
    unsigned long* p_start_cluster;       // offset 0x4, size 0x4
    struct PF_LAST_CLUSTER last_cluster;  // offset 0x8, size 0x8
    struct PF_FAT_HINT last_access;       // offset 0x10, size 0x8
    struct PF_CLUSTER_LINK cluster_link;  // offset 0x18, size 0x14
    struct PF_FAT_HINT* p_hint;           // offset 0x2C, size 0x4
    struct PF_VOLUME* p_vol;              // offset 0x30, size 0x4
} PF_FFD;

typedef struct PF_SDD {
    // total size: 0x27C
    unsigned long stat;           // offset 0x0, size 0x4
    unsigned short num_handlers;  // offset 0x4, size 0x2
    struct PF_FFD ffd;            // offset 0x8, size 0x34
    struct PF_DIR_ENT dir_entry;  // offset 0x3C, size 0x240
} PF_SDD;

struct PF_DIR_CURSOR {
    // total size: 0xC
    unsigned long physical_entry_index;  // offset 0x0, size 0x4
    unsigned long logical_entry_index;   // offset 0x4, size 0x4
    unsigned long logical_seek_index;    // offset 0x8, size 0x4
};

typedef struct PF_DIR {
    // total size: 0x1C
    unsigned long stat;           // offset 0x0, size 0x4
    struct PF_SDD* p_sdd;         // offset 0x4, size 0x4
    struct PF_FAT_HINT hint;      // offset 0x8, size 0x8
    struct PF_DIR_CURSOR cursor;  // offset 0x10, size 0xC
} PF_DIR;

typedef struct PF_CACHE_PAGE {
    // total size: 0x28
    unsigned short stat;           // offset 0x0, size 0x2
    unsigned short option;         // offset 0x2, size 0x2
    unsigned char* buffer;         // offset 0x4, size 0x4
    unsigned char* p_buf;          // offset 0x8, size 0x4
    unsigned char* p_mod_sbuf;     // offset 0xC, size 0x4
    unsigned char* p_mod_ebuf;     // offset 0x10, size 0x4
    unsigned long size;            // offset 0x14, size 0x4
    unsigned long sector;          // offset 0x18, size 0x4
    void* signature;               // offset 0x1C, size 0x4
    struct PF_CACHE_PAGE* p_next;  // offset 0x20, size 0x4
    struct PF_CACHE_PAGE* p_prev;  // offset 0x24, size 0x4
} PF_CACHE_PAGE;

struct PF_SECTOR_CACHE {
    // total size: 0x24
    unsigned long mode;                    // offset 0x0, size 0x4
    unsigned short num_fat_pages;          // offset 0x4, size 0x2
    unsigned short num_data_pages;         // offset 0x6, size 0x2
    struct PF_CACHE_PAGE* pages;           // offset 0x8, size 0x4
    struct PF_CACHE_PAGE* p_current_fat;   // offset 0xC, size 0x4
    struct PF_CACHE_PAGE* p_current_data;  // offset 0x10, size 0x4
    unsigned char (*buffers)[512];         // offset 0x14, size 0x4
    unsigned long fat_buff_size;           // offset 0x18, size 0x4
    unsigned long data_buff_size;          // offset 0x1C, size 0x4
    void* signature;                       // offset 0x20, size 0x4
};

typedef struct PF_CUR_DIR {
    // total size: 0x248
    unsigned long stat;    // offset 0x0, size 0x4
    long context_id;       // offset 0x4, size 0x4
    PF_DIR_ENT directory;  // offset 0x8, size 0x240
} PF_CUR_DIR;

struct PF_DIR_TAIL {
    // total size: 0xC
    unsigned long tracker_size;     // offset 0x0, size 0x4
    unsigned long tracker_buff[1];  // offset 0x4, size 0x4
    unsigned long* tracker_bits;    // offset 0x8, size 0x4
};

typedef struct PF_CLUSTER_LINK_VOL {
    // total size: 0xC
    unsigned short flag;      // offset 0x0, size 0x2
    unsigned short interval;  // offset 0x2, size 0x2
    unsigned long* buffer;    // offset 0x4, size 0x4
    unsigned long link_max;   // offset 0x8, size 0x4
} PF_CLUSTER_LINK_VOL;

typedef struct PF_FILE {
    // total size: 0x34
    unsigned long stat;              // offset 0x0, size 0x4
    unsigned long open_mode;         // offset 0x4, size 0x4
    struct PF_SFD* p_sfd;            // offset 0x8, size 0x4
    struct PF_FAT_HINT hint;         // offset 0xC, size 0x8
    struct PF_FAT_HINT last_access;  // offset 0x14, size 0x8
    long last_error;                 // offset 0x1C, size 0x4
    PF_CURSOR cursor;                // offset 0x20, size 0x10
    unsigned short lock_count;       // offset 0x30, size 0x2
} PF_FILE;

typedef struct PF_INFO {
    // total size: 0x20
    unsigned long file_size;       // offset 0x0, size 0x4
    unsigned long io_pointer;      // offset 0x4, size 0x4
    unsigned long empty_size;      // offset 0x8, size 0x4
    unsigned long allocated_size;  // offset 0xC, size 0x4
    unsigned long lock_mode;       // offset 0x10, size 0x4
    struct PF_FILE* lock_owner;    // offset 0x14, size 0x4
    unsigned long lock_count;      // offset 0x18, size 0x4
    unsigned long lock_tcount;     // offset 0x1C, size 0x4
} PF_INFO;

struct PF_LOCK {
    // total size: 0x10
    unsigned short mode;    // offset 0x0, size 0x2
    unsigned short count;   // offset 0x2, size 0x2
    unsigned long wcount;   // offset 0x4, size 0x4
    struct PF_FILE* owner;  // offset 0x8, size 0x4
    long resource;          // offset 0xC, size 0x4
};

typedef struct PF_SFD {
    // total size: 0x28C
    unsigned long stat;           // offset 0x0, size 0x4
    struct PF_FFD ffd;            // offset 0x4, size 0x34
    struct PF_DIR_ENT dir_entry;  // offset 0x38, size 0x240
    struct PF_LOCK lock;          // offset 0x278, size 0x10
    unsigned short num_handlers;  // offset 0x288, size 0x2
} PF_SFD;

typedef struct PF_VOLUME {
    // total size: 0x1880
    struct PF_BPB bpb;                        // offset 0x0, size 0x38
    unsigned long num_free_clusters;          // offset 0x38, size 0x4
    unsigned long last_free_cluster;          // offset 0x3C, size 0x4
    struct PF_SFD sfds[5];                    // offset 0x40, size 0xCBC
    struct PF_FILE ufds[5];                   // offset 0xCFC, size 0x104
    struct PF_SDD sdds[3];                    // offset 0xE00, size 0x774
    struct PF_DIR udds[3];                    // offset 0x1574, size 0x54
    long num_opened_files;                    // offset 0x15C8, size 0x4
    long num_opened_directories;              // offset 0x15CC, size 0x4
    struct PF_SECTOR_CACHE cache;             // offset 0x15D0, size 0x24
    signed char label[12];                    // offset 0x15F4, size 0xC
    struct PF_CUR_DIR current_dir[1];         // offset 0x1600, size 0x248
    struct PF_DIR_TAIL tail_entry;            // offset 0x1848, size 0xC
    long last_error;                          // offset 0x1854, size 0x4
    long last_driver_error;                   // offset 0x1858, size 0x4
    unsigned long file_config;                // offset 0x185C, size 0x4
    unsigned short flags;                     // offset 0x1860, size 0x2
    signed char drv_char;                     // offset 0x1862, size 0x1
    unsigned short fsi_flag;                  // offset 0x1864, size 0x2
    struct PF_CLUSTER_LINK_VOL cluster_link;  // offset 0x1868, size 0xC
    void* p_part;                             // offset 0x1874, size 0x4
    int (*p_callback)();                      // offset 0x1878, size 0x4
    const unsigned char* format_param;        // offset 0x187C, size 0x4
} PF_VOLUME;

struct PF_CUR_VOLUME {
    unsigned long stat;
    long context_id;
    struct PF_VOLUME* p_vol;
};

struct PF_CONTEXT {
    unsigned long stat;
    long context_id;
};

typedef struct PF_CHARCODE {
    // total size: 0x18
    s32 (*oem2unicode)(const s8*, u16*);    // offset 0x0, size 0x4
    s32 (*unicode2oem)(const u16*, s8*);    // offset 0x4, size 0x4
    s32 (*oem_char_width)(const s8*);       // offset 0x8, size 0x4
    u32 (*is_oem_mb_char)(const s8, u32);   // offset 0xC, size 0x4
    s32 (*unicode_char_width)(const u16*);  // offset 0x10, size 0x4
    u32 (*is_unicode_mb_char)(u16, u32);    // offset 0x14, size 0x4
} PF_CHARCODE;

typedef struct PF_VOLUME_SET {
    // total size: 0x27D48
    struct PF_CUR_VOLUME current_vol[1];  // offset 0x0, size 0xC
    long num_attached_drives;             // offset 0xC, size 0x4
    long num_mounted_volumes;             // offset 0x10, size 0x4
    unsigned long config;                 // offset 0x14, size 0x4
    void* param;                          // offset 0x18, size 0x4
    long last_error;                      // offset 0x1C, size 0x4
    long last_driver_error;               // offset 0x20, size 0x4
    struct PF_CHARCODE codeset;           // offset 0x24, size 0x18
    unsigned long setting;                // offset 0x3C, size 0x4
    struct PF_CONTEXT context[1];         // offset 0x40, size 0x8
    struct PF_VOLUME volumes[26];         // offset 0x48, size 0x27D00
} PF_VOLUME_SET;

typedef struct {
    // total size: 0x14
    PF_CACHE_PAGE* pages;             // offset 0x0, size 0x4
    unsigned char (*buffers)[512];    // offset 0x4, size 0x4
    unsigned short num_fat_pages;     // offset 0x8, size 0x2
    unsigned short num_data_pages;    // offset 0xA, size 0x2
    unsigned long num_fat_buf_size;   // offset 0xC, size 0x4
    unsigned long num_data_buf_size;  // offset 0x10, size 0x4
} PF_CACHE_SETTING;
typedef struct {
    // total size: 0xC
    void* p_part;             // offset 0x0, size 0x4
    PF_CACHE_SETTING* cache;  // offset 0x4, size 0x4
    signed char drive;        // offset 0x8, size 0x1
    unsigned char stat;       // offset 0x9, size 0x1
} PF_DRV_TBL;

struct PF_VOLUME_SET VFipf_vol_set;

typedef struct {
    // total size: 0x10
    unsigned long cls;  // offset 0x0, size 0x4
    unsigned long ecl;  // offset 0x4, size 0x4
    unsigned long bps;  // offset 0x8, size 0x4
    unsigned long spc;  // offset 0xC, size 0x4
} PF_DEV_INF;

typedef struct {
    // total size: 0xC
    const signed char* p_head;  // offset 0x0, size 0x4
    const signed char* p_tail;  // offset 0x4, size 0x4
    unsigned long code_mode;    // offset 0x8, size 0x4
} PF_STR;

typedef struct {
    // total size: 0x6
    unsigned short sys_year;   // offset 0x0, size 0x2
    unsigned short sys_month;  // offset 0x2, size 0x2
    unsigned short sys_day;    // offset 0x4, size 0x2
} PF_SYS_DATE;

typedef struct {
    // total size: 0x8
    unsigned short sys_hour;  // offset 0x0, size 0x2
    unsigned short sys_min;   // offset 0x2, size 0x2
    unsigned short sys_sec;   // offset 0x4, size 0x2
    unsigned short sys_ms;    // offset 0x6, size 0x2
} PF_SYS_TIME;

typedef struct PF_ENT_ITER {
    // total size: 0x6C
    unsigned long index;                    // offset 0x0, size 0x4
    struct PF_VOLUME* p_vol;                // offset 0x4, size 0x4
    struct PF_FFD ffd;                      // offset 0x8, size 0x34
    unsigned long file_sector_index;        // offset 0x3C, size 0x4
    unsigned long sector;                   // offset 0x40, size 0x4
    unsigned short offset;                  // offset 0x44, size 0x2
    unsigned short offset_mask;             // offset 0x46, size 0x2
    unsigned char buf[32];                  // offset 0x48, size 0x20
    unsigned char log2_entries_per_sector;  // offset 0x68, size 0x1
} PF_ENT_ITER;

typedef struct PF_SYSTEM_SET {
    // total size: 0x4
    unsigned long flock_count;  // offset 0x0, size 0x4
} PF_SYSTEM_SET;

struct PF_SYSTEM_SET VFipf_sys_set;

typedef struct VFSysTime {
    // total size: 0x18
    long sec;    // offset 0x0, size 0x4
    long min;    // offset 0x4, size 0x4
    long hour;   // offset 0x8, size 0x4
    long day;    // offset 0xC, size 0x4
    long month;  // offset 0x10, size 0x4
    long year;   // offset 0x14, size 0x4
} VFSysTime;

struct PDM_FUNCTBL {
    // total size: 0x20
    long (*init)(struct PDM_DISK*);                                                                          // offset 0x0, size 0x4
    long (*finalize)(struct PDM_DISK*);                                                                      // offset 0x4, size 0x4
    long (*mount)(struct PDM_DISK*);                                                                         // offset 0x8, size 0x4
    long (*unmount)(struct PDM_DISK*);                                                                       // offset 0xC, size 0x4
    long (*format)(struct PDM_DISK*, unsigned char*);                                                        // offset 0x10, size 0x4
    long (*physical_read)(struct PDM_DISK*, unsigned char*, unsigned long, unsigned long, unsigned long*);   // offset 0x14, size 0x4
    long (*physical_write)(struct PDM_DISK*, unsigned char*, unsigned long, unsigned long, unsigned long*);  // offset 0x18, size 0x4
    long (*get_disk_info)(struct PDM_DISK*, struct PDM_DISK_INFO*);                                          // offset 0x1C, size 0x4
};
typedef struct PDM_DISK_TBL {
    // total size: 0x8
    struct PDM_FUNCTBL* p_func;  // offset 0x0, size 0x4
    unsigned long ui_ext;        // offset 0x4, size 0x4
} PDM_DISK_TBL;

typedef struct PDM_INIT_DISK {
    // total size: 0x8
    long (*p_func)(struct PDM_DISK_TBL*, unsigned long);  // offset 0x0, size 0x4
    unsigned long ui_ext;                                 // offset 0x4, size 0x4
} PDM_INIT_DISK;

typedef struct PDM_DISK {
    // total size: 0x38
    unsigned long status;                   // offset 0x0, size 0x4
    struct PDM_DISK_TBL disk_tbl;           // offset 0x4, size 0x8
    unsigned long signature;                // offset 0xC, size 0x4
    unsigned short open_disk_cnt;           // offset 0x10, size 0x2
    unsigned short disk_lock_cnt;           // offset 0x12, size 0x2
    struct PDM_DISK* disk_lock_handle;      // offset 0x14, size 0x4
    struct PDM_DISK_INFO disk_info;         // offset 0x18, size 0x14
    struct PDM_INIT_DISK* p_init_disk_tbl;  // offset 0x2C, size 0x4
    struct PDM_PARTITION* p_cur_part;       // offset 0x30, size 0x4
    void (*p_erase_func)();                 // offset 0x34, size 0x4
} PDM_DISK;

struct PDM_DISK_HANDLE {
    // total size: 0x8
    unsigned long signature;  // offset 0x0, size 0x4
    struct PDM_DISK* handle;  // offset 0x4, size 0x4
};
struct PDM_PARTITION_HANDLE {
    // total size: 0x8
    unsigned long signature;       // offset 0x0, size 0x4
    struct PDM_PARTITION* handle;  // offset 0x4, size 0x4
};
typedef struct PDM_DISK_SET {
    // total size: 0xBCC
    unsigned short num_partition;                      // offset 0x0, size 0x2
    unsigned short num_allocated_disk;                 // offset 0x2, size 0x2
    struct PDM_DISK_HANDLE disk_handle[26];            // offset 0x4, size 0xD0
    struct PDM_PARTITION_HANDLE partition_handle[26];  // offset 0xD4, size 0xD0
    struct PDM_DISK disk[26];                          // offset 0x1A4, size 0x5B0
    struct PDM_PARTITION partition[26];                // offset 0x754, size 0x478
} PDM_DISK_SET;

typedef struct PDM_PART_TBL {
    // total size: 0x14
    unsigned char boot_flag;         // offset 0x0, size 0x1
    unsigned char partition_type;    // offset 0x1, size 0x1
    unsigned short s_cylinder;       // offset 0x2, size 0x2
    unsigned char s_head;            // offset 0x4, size 0x1
    unsigned char s_sector;          // offset 0x5, size 0x1
    unsigned short e_cylinder;       // offset 0x6, size 0x2
    unsigned char e_head;            // offset 0x8, size 0x1
    unsigned char e_sector;          // offset 0x9, size 0x1
    unsigned char pad[2];            // offset 0xA, size 0x2
    unsigned long lba_start_sector;  // offset 0xC, size 0x4
    unsigned long lba_num_sectors;   // offset 0x10, size 0x4
} PDM_PART_TBL;

typedef struct PDM_MBR {
    // total size: 0x58
    unsigned long current_sector;            // offset 0x0, size 0x4
    unsigned long epbr_base_sector;          // offset 0x4, size 0x4
    struct PDM_PART_TBL partition_table[4];  // offset 0x8, size 0x50
} PDM_MBR;

#endif  // VF_STRUCT_H
