#ifndef PF_FILELOCK_H
#define PF_FILELOCK_H

#include "revolution/vf/vf_struct.h"

void VFiPF_InitLockFile();
s32 VFiPF_LockFile(PF_FILE* p_file);
s32 VFiPF_UnLockFile(PF_FILE* p_file);

#endif  // PF_FILELOCK_H
