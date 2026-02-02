#ifndef PF_ENTRY_ITERATOR_H
#define PF_ENTRY_ITERATOR_H

#include "revolution/vf/vf_struct.h"

s32 VFiPFENT_ITER_IteratorInitialize(PF_ENT_ITER *p_iter, u32 index_start_from);
u32 VFiPFENT_ITER_IsAtLogicalEnd(PF_ENT_ITER *p_iter);
s32 VFiPFENT_ITER_MoveTo(PF_ENT_ITER *p_iter, u32 index, u32 may_allocate);
s32 VFiPFENT_ITER_Advance(PF_ENT_ITER *p_iter, u32 may_allocate);
s32 VFiPFENT_ITER_Retreat(PF_ENT_ITER *p_iter, u32 may_allocate);

s32 VFiPFENT_ITER_LoadEntry(PF_ENT_ITER *p_iter);
s32 VFiPFENT_ITER_FindEntry(PF_ENT_ITER *p_iter, PF_DIR_ENT *p_ent, PF_STR *p_pattern, u8 attr_required, u32 *p_is_found, u32 *p_ppos, u32 *p_lpos, u32 is_skip);
s32 VFiPFENT_ITER_AllocateEntry(PF_DIR_ENT *p_ent, u8 num_entries, PF_FFD *p_ffd, u32 *p_prev_chain, PF_STR *p_filename, u8 attr_required, u32 *p_pos);
s32 VFiPFENT_ITER_FindCluster(PF_ENT_ITER *p_iter, PF_DIR_ENT *p_ent, u32 cluster, u32 *p_is_found);

s32 VFiPFENT_ITER_GetEntryOfPath(PF_ENT_ITER *p_iter, PF_DIR_ENT *p_ent, PF_VOLUME *p_vol, PF_STR *p_path, u32 is_parent);
s32 VFiPFENT_ITER_GetEntryOfPattern(PF_ENT_ITER *p_iter, PF_DIR_ENT *p_ent, PF_VOLUME *p_vol, PF_STR *p_path);
s32 VFiPFENT_ITER_GetEntryOfIter(PF_ENT_ITER *p_iter, PF_DIR_ENT *p_ent);
s32 VFiPFENT_ITER_GetLFNEntryName(PF_ENT_ITER *p_iter, PF_DIR_ENT *p_ent);

static s32 VFiPFENT_ITER_RecalcEntryIterator(PF_ENT_ITER *p_iter, u32 may_allocate);
static s32 VFiPFENT_ITER_DoMoveTo(PF_ENT_ITER *p_iter, u32 index, u32 may_allocate);
static u32 VFiPFENT_ITER_IsAtPhysicalEnd(PF_ENT_ITER *p_iter);
static void VFiPFENT_ITER_MakeLongFileName(PF_ENT_ITER *p_iter, PF_DIR_ENT *p_ent);
static s32 VFiPFENT_ITER_GetEntry(PF_DIR_ENT *p_ent, PF_ENT_ITER *p_iter, PF_STR *p_pattern, u8 attr_required, u32 *p_lpos, u32 is_reverse);
static s32 VFiPFENT_ITER_DoFindEntry(PF_ENT_ITER *p_iter, PF_DIR_ENT *p_ent, PF_STR *p_pattern, u8 attr_required, u32 *p_is_found, u32 *p_ppos, u32 *p_lpos, u32 is_skip);
static s32 VFiPFENT_ITER_DoAllocateEntry(PF_DIR_ENT *p_ent, u8 num_entries, PF_FFD *p_ffd, u32 *p_prev_chain, PF_STR *p_filename, u8 attr_required, u32 *p_pos);
static s32 VFiPFENT_ITER_DoGetEntryOfPath(PF_ENT_ITER *p_iter, PF_DIR_ENT *p_ent, PF_VOLUME *p_vol, PF_STR *p_path, u32 wildcard, u32 is_parent);

#endif  // PF_ENTRY_ITERATOR
