#ifndef PF_PATH_H
#define PF_PATH_H

#include "revolution/vf/vf_struct.h"

s32 VFiPFPATH_AdjustExtShortName(s8* pname, u32 position);

s32 VFiPFPATH_SplitPath(PF_STR* p_path, PF_STR* p_dir_path, PF_STR* p_filname);

PF_VOLUME* VFiPFPATH_GetVolumeFromPath(PF_STR* p_path);

#endif  // PF_PATH_H
