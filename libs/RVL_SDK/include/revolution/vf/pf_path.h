#ifndef PF_PATH_H
#define PF_PATH_H

#include "revolution/vf/vf_struct.h"

PF_VOLUME* VFiPFPATH_GetVolumeFromPath(PF_STR* p_path);
s32 VFiPFPATH_SplitPath(PF_STR* p_path, PF_STR* p_dir_path, PF_STR* p_filename);
s32 VFiPFPATH_SplitPathPattern(PF_STR* p_path, PF_STR* p_dir_path, PF_STR* p_pattern);
void VFiPFPATH_InitTokenOfPath(PF_STR* p_str, s8* path, u32 code_mode);
s32 VFiPFPATH_GetNextTokenOfPath(PF_STR* p_str, u32 wildcard);

s32 VFiPFPATH_putShortName(u8* pDirEntry, const s8* short_name, u8 attr);
s32 VFiPFPATH_getShortName(s8* short_name, const u8* pDirEntry, u8 attr);
void VFiPFPATH_getLongNameformShortName(s8* short_name, s8* long_name, u8 flag);
u32 VFiPFPATH_GetLengthFromShortname(const s8* sSrc);
u32 VFiPFPATH_parseShortName(s8* pDest, PF_STR* p_pattern);
s32 VFiPFPATH_parseShortNameNumeric(s8* p_char, u32 count);
s32 VFiPFPATH_AdjustExtShortName(s8* pName, u32 position);

u32 VFiPFPATH_GetLengthFromUnicode(const u16* sSrc);
s32 VFiPFPATH_transformFromUnicodeToNormal(s8* sDest, const u16* sSrc);
s32 VFiPFPATH_transformInUnicode(u16* sDestStr, const s8* sSrcStr);

u32 VFiPFPATH_MatchFileNameWithPattern(const s8* file_name, PF_STR* p_pattern, u32 is_short_search, u32 is_long_name);
s32 VFiPFPATH_cmpTailSFN(const s8* sfn_name, const s8* pattern);
void VFiPFPATH_SetSearchPattern(s8* p_buf_local, u16* p_buf_unicode, PF_STR* p_pattern);

u32 VFiPFPATH_CheckExtShortNameSignature(PF_STR* p_str);
u32 VFiPFPATH_CheckExtShortName(PF_STR* p_str, u32 target, u32 wildcard);
u32 VFiPFPATH_GetExtShortNameIndex(PF_STR* p_str, u32* p_index);

static s32 VFiPFPATH_DoSplitPath(PF_STR* p_path, PF_STR* p_dir_path, PF_STR* p_filename, u32 wildcard);
static u32 VFiPFPATH_UNI_ConvertFWchar(u16 src, u16* dst);
static u32 VFiPFPATH_OEM_ConvertFWchar(const s8* src, u16* dst);
static u16 VFiPFPATH_GetNextCharOfShortName(PF_FILE_NAME_ITER* p_name);
static u16 VFiPFPATH_GetNextCharOfLongName(PF_FILE_NAME_ITER* p_name);
static u16 VFiPFPATH_GetNextCharOfFileName(PF_FILE_NAME_ITER* p_name);
static u16 VFiPFPATH_GetNextCharOfPattern(PF_STR* p_pattern, u16 name_kind);
static u32 VFiPFPATH_DoMatchFileNameWithPattern(u16 c_name, PF_FILE_NAME_ITER* p_name, u16 c_pat, PF_STR* p_pattern);

#endif  // PF_PATH_H
