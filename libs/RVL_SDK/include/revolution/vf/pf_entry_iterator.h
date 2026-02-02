#ifndef PF_ENTRY_ITERATOR_H
#define PF_ENTRY_ITERATOR_H

#include "revolution/vf/vf_struct.h"

s32 VFiPFENT_ITER_IteratorInitialize(PF_ENT_ITER* p_iter, u32 index_start_from);

s32 VFiPFENT_ITER_Advance(PF_ENT_ITER* p_iter, u32 may_allocate);

u32 VFiPFENT_ITER_IsAtLogicalEnd(PF_ENT_ITER* p_iter);

s32 VFiPFENT_ITER_GetEntryOfPath(PF_ENT_ITER* p_iter, PF_DIR_ENT* p_ent, PF_VOLUME* p_vol, PF_STR* p_path, u32 is_parent);

#endif  // PF_ENTRY_ITERATOR
