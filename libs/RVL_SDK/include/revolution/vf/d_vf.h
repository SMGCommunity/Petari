#ifndef D_VF_H
#define D_VF_H

#include "revolution.h"

typedef void* VFFile;

typedef enum {
    VF_OK = 0x0000,
    VF_ERROR_0002 = 0x0002,
    VF_ERROR_0005 = 0x0005,
    VF_ERROR_B001 = 0xB001,
} VFError;

s32 VFIsAvailable();

#ifdef __cplusplus
extern "C" {
#endif

void VFInitEx(void* i_heap_start_address_p, u32 i_size);
void VFInit(void);
s32 VFMountDriveNANDFlashEx(const char* i_drive, const char* i_sys_file_name_p);
s32 VFUnmountDrive(const char* i_drive);
void* VFOpenFile(const char* i_path_p, const char* i_mode, u32 i_attr);
s32 VFCloseFile(void* i_file_p);
s32 VFSeekFile(void* i_file_p, s32 i_offset, s32 i_origin);
s32 VFReadFile(void* i_file_p, void* o_buf_p, u32 i_size, u32* o_read_size_p);
s32 VFWriteFile(void* i_file_p, void* i_buf_p, u32 i_size);
s32 VFDeleteFile(const char* i_path_p);
s32 VFGetFileSizeByFd(void* i_file_p);
s32 VFGetLastError();
s32 VFGetLastDeviceError(const char* i_drive);
s32 VFGetDriveFreeSize(const char* i_drive);
s32 VFSetSyncMode(const char* i_drive, u32 i_mode);

#ifdef __cplusplus
}
#endif

#endif  // D_VF_H
