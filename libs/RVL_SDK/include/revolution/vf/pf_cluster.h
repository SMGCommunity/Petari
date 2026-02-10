#ifndef PF_CLUSTER_H
#define PF_CLUSTER_H

#include "revolution/vf/vf_struct.h"

void VFiPFCLUSTER_UpdateLastAccessCluster(PF_FILE* p_file, u32 sector);

void VFiPFCLUSTER_SetLastAccessCluster(PF_FILE*);

s32 VFiPFCLUSTER_AppendCluster(PF_FILE* p_file, u32 byte, u32* p_success, u32* sector);

void VFiPFCLUSTER_InitLastAccessCluster(PF_FILE* p_file);

s32 VFiPFCLUSTER_GetAppendSize(PF_FILE* p_file, u32* p_size);

#endif  // PF_CLUSTER_H
