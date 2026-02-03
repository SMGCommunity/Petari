#ifndef PF_STR_H
#define PF_STR_H

#include <revolution/types.h>
#include "revolution/vf/vf_struct.h"

s32 VFiPFSTR_InitStr(PF_STR *p_str, const s8 *s, u32 code_mode);
void VFiPFSTR_SetCodeMode(PF_STR *p_str, u32 code_mode);
u32 VFiPFSTR_GetCodeMode(PF_STR *p_str);
s8* VFiPFSTR_GetStrPos(PF_STR *p_str, u32 target);
void VFiPFSTR_MoveStrPos(PF_STR *p_str, s16 num_char);
u16 VFiPFSTR_StrLen(PF_STR *p_str);
u16 VFiPFSTR_StrNumChar(PF_STR *p_str, u32 target);
s32 VFiPFSTR_StrCmp(const PF_STR *p_str, const s8 *s);
s32 VFiPFSTR_StrNCmp(PF_STR *p_str, const s8 *s, u32 target, s16 offset, u16 num);
void VFiPFSTR_ToUpperNStr(PF_STR *p_str, u16 num, s8 *dest);
#endif
