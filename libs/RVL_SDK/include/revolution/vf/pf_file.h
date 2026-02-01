#ifndef PF_FILE_H
#define PF_FILE_H

#include "revolution/types.h"
#include "revolution/vf/vf_struct.h"

s32 VFiPFFILE_RemoveFile(PF_VOLUME* p_vol, PF_DIR_ENT* p_ent, PF_ENT_ITER* p_iter);

s32 VFiPFFILE_fclose(PF_FILE* p_file);
s32 VFiPFFILE_fread(u8* p_buf, u32 size, u32 count, PF_FILE* p_file, u32* p_count_read);
s32 VFiPFFILE_fwrite(u8* p_buf, u32 size, u32 count, PF_FILE* p_file, u32* p_count_written);
s32 VFiPFFILE_fseek(PF_FILE* p_file, s32 lOffset, s32 nOrigin);
s32 VFiPFFILE_finfo(PF_FILE* p_file, PF_INFO* p_info);
s32 VFiPFFILE_remove(PF_STR* p_path_str);
s32 VFiPFFILE_fopen(PF_STR* p_path_str, u32 mode, PF_FILE** pp_file);
s32 VFiPFFILE_fgetpos(PF_FILE* p_file, u32* pos);
s32 VFiPFFILE_fsetpos(PF_FILE* p_file, const u32* pos);

void VFiPFFILE_Cursor_MoveToClusterEnd(PF_FILE* p_file, u32 size);

void VFiPFFILE_FinalizeAllFiles(PF_VOLUME* p_vol);

#endif  // PF_FILE_H
