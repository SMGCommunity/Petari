#ifndef PF_SERVICE_H
#define PF_SERVICE_H

#include "revolution/types.h"

u16 VFiPF_GET_LE_U16(const u8* buf);
void VFiPF_LE16_TO_U16_STR(u8* sSrc, u32 num);

#endif  // PF_SERVICE_H
