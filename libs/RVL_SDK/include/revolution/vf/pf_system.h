#ifndef PF_SYSTEM_H
#define PF_SYSTEM_H

#include <revolution/types.h>
#include "revolution/vf/vf_struct.h"

void VFiPFSYS_initializeSYS(void);
s32 VFiPFSYS_GetCurrentContextID(s32 *context_id);
void VFiPFSYS_TimeStamp(PF_SYS_DATE *sdate, PF_SYS_TIME *stime); 

#endif
