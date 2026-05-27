#ifndef D_VF_SYS_H
#define D_VF_SYS_H

#include "revolution/vf/vf_struct.h"

void (*VFSysGetTimeStampCallback())(struct VFSysTime*);

s32 VFSysUnsetDevice(s32 i_handle_idx);
s32 VFSysCheckExistPrfFile_nandflash_sub(const s8* i_prf_file_name_p, u32 i_handle_idx);
s32 VFSysCheckExistPrfFile_nandflash(struct VF_HANDLE_DEVICE* i_device_p, const s8* i_prf_file_name_p, void* i_memory_p, u32 i_handle_idx);
s32 VFSysCheckExistPrfFile_ram(struct VF_HANDLE_DEVICE* i_device_p, const s8* i_prf_file_name_p, void* i_memory_p, u32 i_handle_idx);
s32 VFSysCheckExistPrfFile_dvd(struct VF_HANDLE_DEVICE* i_device_p, const s8* i_prf_file_name_p, void* i_memory_p, u32 i_handle_idx);

#endif  // D_VF_SYS_H
