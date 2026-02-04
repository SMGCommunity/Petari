#ifndef PF_ENTRY_H
#define PF_ENTRY_H

#include "revolution/vf/vf_struct.h"

s32 VFiPFENT_InitENT(PF_DIR_ENT* p_end, PF_STR* p_filename, u8 attr, u32 is_set_time, PF_DIR_ENT* p_dir_ent, PF_VOLUME* p_vol);

s32 VFiPFENT_allocateEntry(PF_DIR_ENT* p_ent, u8 num_entries, PF_FFD* p_ffd, u32* p_prev_chain, PF_STR* p_filename, u8 attr_required, u32* p_pos);

s32 VFiPFENT_UpdateEntry(PF_DIR_ENT* p_ent, u32* p_prev_chain, u32 is_set_ARCH);

u8 VFiPFENT_CalcCheckSum(PF_DIR_ENT* p_ent);

u8 VFiPFENT_getcurrentDateTimeForEnt(u16* p_date, u16* p_time);

s32 VFiPFENT_GetParentEntryOfPath(PF_DIR_ENT* p_ent, PF_VOLUME* p_vol, PF_STR* p_path);

s32 VFiPFENT_findEntry(PF_FFD* p_ffd, PF_DIR_ENT* p_ent, u32 index_search_from, PF_STR* p_pattern, u8 attr_required, u32* p_lpos, u32* p_ppos);

s32 VFiPFENT_RemoveEntry(PF_DIR_ENT* p_ent, PF_ENT_ITER* p_iter);

s32 VFiPFENT_ITER_GetEntryOfPath(PF_ENT_ITER* p_iter, PF_DIR_ENT* p_ent, PF_VOLUME* p_vol, PF_STR* p_path, u32 is_parent);

s32 VFiPFENT_UpdateSFNEntry(PF_DIR_ENT* p_ent, u32 flag);

s32 VFiPFENT_GetRootDir(PF_VOLUME* p_vol, PF_DIR_ENT* p_ent);

s32 VFiPFENT_MakeRootDir(PF_VOLUME* p_vol);

s32 VFiPFENT_FillVoidEntryToSectors(PF_VOLUME* p_vol, u32 start_sector, u32 num_sectors, u32 is_make_new_directory, PF_DIR_ENT* p_ent,
                                    PF_DIR_ENT* p_parent_ent);

void VFiPFENT_StoreEntryNumericFieldsToBuf(u8* buf, const PF_DIR_ENT* p_ent);

#endif  // PF_ENTRY_H
