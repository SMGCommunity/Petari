#ifndef PF_PATH_H
#define PF_PATH_H

#include "revolution/vf/vf_struct.h"

s32 VFiPFPATH_putShortName(u8* pDirEntry, const s8* short_name, u8 attr);

s32 VFiPFPATH_getShortName(s8* short_name, const u8* pDirEntry, u8 attr);

s32 VFiPFPATH_AdjustExtShortName(s8* pname, u32 position);

s32 VFiPFPATH_SplitPath(PF_STR* p_path, PF_STR* p_dir_path, PF_STR* p_filname);

s32 VFiPFPATH_parseShortNameNumeric(s8* p_char, u32 count);

s32 VFiPFPATH_cmpTailSFN(const s8* sfn_name, const s8* pattern);

PF_VOLUME* VFiPFPATH_GetVolumeFromPath(PF_STR* p_path);

#endif  // PF_PATH_H
