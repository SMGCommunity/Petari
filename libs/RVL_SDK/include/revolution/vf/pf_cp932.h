#ifndef PF_CP932_H
#define PF_CP932_H

#include "revolution/vf/vf_struct.h"

s32 VFiPFCODE_CP932_OEM2Unicode(const s8* cp932_src, u16* uc_dst);
s32 VFiPFCODE_CP932_Unicode2OEM(const u16* uc_src, s8* cp932_dst);
s32 VFiPFCODE_CP932_OEMCharWidth(const s8* buf);
u32 VFiPFCODE_CP932_isOEMMBchar(s8 cp932, u32 num);
s32 VFiPFCODE_CP932_UnicodeCharWidth(const u16* buf);
u32 VFiPFCODE_CP932_isUnicodeMBchar(u16 uc_src, u32 num);

#endif  // PF_CP932_H
