#ifndef PDM_BPB_H
#define PDM_BPB_H

#include "revolution/vf/vf_struct.h"

s32 VFipdm_bpb_get_bpb_information(u8* buf, PDM_BPB* p_bpb);

s32 VFipdm_bpb_check_fsinfo_sector(u8* buf, u32* is_fsinfo);

s32 VFipdm_bpb_get_fsinfo_information(u8* buf, PDM_FSINFO* p_fsinfo);

s32 VFipdm_bpb_store_string(const u8* p_string, u32 length, u8* buf);

s32 VFipdm_bpb_check_boot_sector(u8* buf, u32* is_boot);

#endif  // PDM_BPB_H
