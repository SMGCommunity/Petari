#include "revolution/vf/pf_path.h"
#include "revolution/vf/pf_str.h"
#include "revolution/vf/vf_struct.h"

extern PF_VOLUME_SET VFipf_vol_set;
extern const u8 VFipf_valid_fn_char[];

s32 VFiPFPATH_DoSplitPath(struct PF_STR* p_path, struct PF_STR* p_dir_path, struct PF_STR* p_filename, u32 wildcard) {
    s8* p;
    s8* p_tail_prev;
    u32 code_mode;
    struct PF_STR token;
    struct PF_STR token_prev;
    s32 err;

    if (p_dir_path == NULL) {
        return 10;
    }

    code_mode = VFiPFSTR_GetCodeMode(p_path);
    VFiPFSTR_SetCodeMode(p_dir_path, code_mode);
    VFiPFSTR_SetCodeMode(p_filename, code_mode);

    p_dir_path->p_head = NULL;
    if (p_filename != NULL) {
        p_filename->p_head = NULL;
    }

    if (VFiPFSTR_StrNCmp(p_path, (const s8*)":", 1, 1, 1) == 0) {
        VFiPFSTR_MoveStrPos(p_path, 2);
    }

    p = (s8*)p_path->p_head;
    VFiPFPATH_InitTokenOfPath(&token, (s8*)p_path->p_head, code_mode);
    err = VFiPFPATH_GetNextTokenOfPath(&token, wildcard);

    if (err != 0) {
        return err;
    }

    if (token.p_head == NULL || (u16)VFiPFSTR_StrLen(&token) == 0) {
        return 2;
    }

    if (p_path->p_tail < token.p_tail) {
        return 2;
    }

    token_prev = token;

    while (1) {
        if (VFiPFSTR_StrNCmp(&token, (const s8*)"\\", 2, 0, 1) == 0) {
            p = (s8*)token_prev.p_tail;
            break;
        }

        err = VFiPFPATH_GetNextTokenOfPath(&token, wildcard);
        if (err != 0) {
            return err;
        }

        if ((u16)VFiPFSTR_StrLen(&token) == 0) {
            return 2;
        }

        if (p_path->p_tail < token.p_tail) {
            break;
        }

        if (token.p_head == NULL) {
            break;
        }

        p_tail_prev = (s8*)token_prev.p_tail;
        p = p_tail_prev;
        token_prev = token;
    }

    p_dir_path->p_head = p_path->p_head;
    p_dir_path->p_tail = p;
    if (p_filename != NULL) {
        *p_filename = token_prev;
    }

    return 0;
}

static u32 VFiPFPATH_UNI_ConvertFWchar(u16 src, u16* dst) {
    if (0xFF41 <= src && src <= 0xFF5A) {
        *dst = src - 0x20;
        return 1;
    } else {
        return 0;
    }
}

static u32 VFiPFPATH_OEM_ConvertFWchar(const s8* src, u16* dst) {
    VFipf_vol_set.codeset.oem2unicode(src, dst);
    if (VFiPFPATH_UNI_ConvertFWchar(*dst, dst) == 1) {
        VFipf_vol_set.codeset.unicode2oem(dst, (s8*)dst);
        *dst = (u16)(*dst);
        return 1;
    }
    return 0;
}

static u16 VFiPFPATH_GetNextCharOfShortName(struct PF_FILE_NAME_ITER* p_name) {
    u16 wc;
    s8 c;

    if (p_name->index >= 12) {
        return 0;
    }

    wc = p_name->buf[p_name->index++];
    if (VFipf_vol_set.codeset.is_oem_mb_char(wc, 1) != 0) {
        c = (u8)p_name->buf[p_name->index++];
        wc = ((u8)wc << 8) + (u8)c;
    }

    wc = (wc >= 0x61 && wc <= 0x7A) ? wc - 0x20 : wc;
    return wc;
}

static u16 VFiPFPATH_GetNextCharOfLongName(struct PF_FILE_NAME_ITER* p_name) {
    u16 wc;
    u16 wchar;

    if (p_name->index > 522) {
        return 0;
    }

    wc = *(u16*)(p_name->buf + p_name->index);
    p_name->index += 2;

    wc = (wc >= 0x61 && wc <= 0x7A) ? wc - 0x20 : wc;

    if (VFiPFPATH_UNI_ConvertFWchar(wc, &wchar) == 1) {
        wc = wchar;
    }
    return wc;
}

static u16 VFiPFPATH_GetNextCharOfFileName(struct PF_FILE_NAME_ITER* p_name) {
    if (p_name->kind != 0) {
        return VFiPFPATH_GetNextCharOfLongName(p_name);
    } else {
        return VFiPFPATH_GetNextCharOfShortName(p_name);
    }
}

static u16 VFiPFPATH_GetNextCharOfPattern(PF_STR* p_pattern, u16 name_kind) {
    u16 twc;
    u16 wc;
    u16 tmp_wc;
    s8 pattern[3];

    if (p_pattern->code_mode == 1) {
        pattern[0] = *p_pattern->p_head++;
        pattern[1] = 0;

        if (pattern[0] == 0 || p_pattern->p_tail < p_pattern->p_head) {
            return 0;
        }

        if (VFipf_vol_set.codeset.is_oem_mb_char(pattern[0], 1) != 0) {
            pattern[1] = *p_pattern->p_head++;
            twc = ((u8)pattern[0] << 8) + (u8)pattern[1];
        } else {
            twc = pattern[0];
        }

        if (name_kind != 0) {
            VFipf_vol_set.codeset.oem2unicode(pattern, &twc);
        }
    } else {
        pattern[0] = *(u16*)p_pattern->p_head;
        pattern[1] = (s8)((*(u16*)p_pattern->p_head) >> 8);
        p_pattern->p_head += 2;
        pattern[2] = 0;

        if ((pattern[0] == 0 && pattern[1] == 0) || p_pattern->p_tail < p_pattern->p_head) {
            return 0;
        }

        twc = ((u8)pattern[1] << 8) + (u8)pattern[0];

        if (name_kind == 0) {
            VFipf_vol_set.codeset.unicode2oem(&twc, (s8*)&wc);
            if (VFipf_vol_set.codeset.is_oem_mb_char(wc >> 8, 1) != 0) {
                twc = wc;
            } else {
                twc = pattern[0];
            }
        }
    }

    twc = (twc >= 0x61 && twc <= 0x7A) ? twc - 0x20 : twc;

    if (name_kind != 0) {
        if (VFiPFPATH_UNI_ConvertFWchar(twc, &tmp_wc) == 1) {
            twc = tmp_wc;
        }
    } else {
        if (VFiPFPATH_OEM_ConvertFWchar((const s8*)&twc, &tmp_wc) == 1) {
            twc = tmp_wc;
        }
    }

    return twc;
}

/*
static u32 VFiPFPATH_DoMatchFileNameWithPattern(u16 c_name, PF_FILE_NAME_ITER* p_name, u16 c_pat, PF_STR* p_pattern) {
    struct PF_FILE_NAME_ITER name;
    struct PF_STR pattern;

    while (c_pat != '\0') {
        switch (c_pat) {
        case '?': {
            if (c_name == '\0') {
                return 0;
            }
            break;
        }
        case '*': {
            c_pat = VFiPFPATH_GetNextCharOfPattern(p_pattern, is_long_name);
            while (c_pat == '*' || c_pat == '?') {
                c_pat = VFiPFPATH_GetNextCharOfPattern(p_pattern, is_long_name);
            }
            if (c_pat == '\0') {
                return 1;
            }

            while (c_name != '\0') {
                if (c_name == c_pat) {
                    name = *p_name;
                    pattern = *p_pattern;

                    c_name = VFiPFPATH_GetNextCharOfFileName(&name);
                    c_pat = VFiPFPATH_GetNextCharOfPattern(&pattern, is_long_name);

                    if (VFiPFPATH_DoMatchFileNameWithPattern(c_name, &name, c_pat, &pattern, is_long_name)) {
                        return 1;
                    }
                }
                c_name = VFiPFPATH_GetNextCharOfFileName(p_name);
            }
            return 0;
        }
        default: {
            if (c_name != c_pat) {
                return 0;
            }
            break;
        }
        }
        c_name = VFiPFPATH_GetNextCharOfFileName(p_name);
        c_pat = VFiPFPATH_GetNextCharOfPattern(p_pattern, is_long_name);
    }
    return c_name == '\0';
}

s32 VFiPFPATH_cmpNameImpl(const s8* sName, const s8* sPattern, u32* p_is_end) {
    s32 pw;
    s32 nw;
    s32 ret;
    u16 p;
    u16 n;
    u16 tp;
    u16 tn;

    while (*sPattern != '\0') {
        pw = VFipf_vol_set.codeset.oem_char_width(sPattern);
        nw = VFipf_vol_set.codeset.oem_char_width(sName);

        p = (pw == 1) ? (VFipf_toupper(*sPattern)) : ((u16)VFiPF_GET_LE_U16((const u8*)sPattern));
        n = (nw == 1) ? (VFipf_toupper(*sName)) : ((u16)VFiPF_GET_LE_U16((const u8*)sName));

        if (pw == 2 && VFiPFPATH_OEM_ConvertFWchar(sPattern, &tp) == 1) {
            p = tp;
        }

        if (nw == 2 && VFiPFPATH_OEM_ConvertFWchar(sName, &tn) == 1) {
            n = tn;
        }

        sPattern += pw;

        switch (p) {
        case '?': {
            if (n == 0) {
                return 1;
            }
            break;
        }
        case '*': {
            do {
                pw = VFipf_vol_set.codeset.oem_char_width(sPattern);
                p = (pw == 1) ? (VFipf_toupper(*sPattern)) : ((u16)VFiPF_GET_LE_U16((const u8*)sPattern));

                sPattern += pw;

                if (p == '?') {
                    if (n == 0) {
                        return 1;
                    }
                    sName += nw;
                    nw = VFipf_vol_set.codeset.oem_char_width(sName);
                    n = (nw == 1) ? (VFipf_toupper(*sName)) : ((u16)VFiPF_GET_LE_U16((const u8*)sName));
                }
            } while (p == '?' || p == '*');

            if (p == 0) {
                return 0;
            }

            while (n != 0) {
                sName += nw;
                if (n == p) {
                    ret = VFiPFPATH_cmpNameImpl(sName, sPattern, p_is_end);
                    if (ret == 0) {
                        return 0;
                    }
                    if (*p_is_end != 0) {
                        return ret;
                    }
                }

                nw = VFipf_vol_set.codeset.oem_char_width(sName);
                n = (nw == 1) ? (VFipf_toupper(*sName)) : ((u16)VFiPF_GET_LE_U16((const u8*)sName));
            }

            if (*sName == '\0' || *sPattern == '\0') {
                *p_is_end = 1;
            }
        }

        default: {
            if (n != p) {
                return 1;
            }
            break;
        }
        }

        sName += nw;
    }

    return *sName != '\0';
}

s32 VFiPFPATH_cmpNameUni(const u16* p_name, struct PF_STR* sPattern) {
    return VFiPFPATH_MatchFileNameWithPattern((const s8*)p_name, sPattern, 1) == 0;
}

s32 VFiPFPATH_cmpName(const s8* sShort, struct PF_STR* p_pattern, u32 is_short_search) {
    u32 is_end;
    s8 tmpBuf[13];
    s8* p_tmpBuf;
    const s8* sPattern;

    is_end = 0;
    p_tmpBuf = tmpBuf;
    sPattern = VFiPFSTR_GetStrPos(p_pattern, 3);
    VFipf_strcpy(tmpBuf, sShort);

    if (((VFipf_vol_set.setting & 2) == 2) && VFiPFSTR_GetCodeMode(p_pattern) == 2 && VFipf_strcmp(tmpBuf, (const s8*)".") != 0 &&
        VFipf_strcmp(tmpBuf, (const s8*)"..") != 0 && VFiPFPATH_CheckExtShortName(p_pattern, 3, 1) == 0 && is_short_search == 0) {
        return 1;
    }

    if (VFipf_strcmp(sPattern, (const s8*)"*.*") == 0) {
        for (; *p_tmpBuf != '\0' && *p_tmpBuf != '.'; p_tmpBuf++) {
        }
        if (*p_tmpBuf == '\0') {
            *p_tmpBuf = '.';
            p_tmpBuf[1] = '\0';
        }
    } else {
        if (*sPattern == '\0') {
            return 1;
        }
        if (tmpBuf[0] == '\0') {
            return 1;
        }
    }

    return VFiPFPATH_cmpNameImpl(tmpBuf, sPattern, &is_end);
}

s32 VFiPFPATH_cmpTailSFN(const s8* sfn_name, const s8* pattern) {
    return VFipf_strcmp(sfn_name, pattern) != 0;
}

void VFiPFPATH_InitTokenOfPath(struct PF_STR* p_str, s8* path, u32 code_mode) {
    p_str->p_head = path;
    p_str->p_tail = path;
    p_str->code_mode = code_mode;
}

s32 VFiPFPATH_GetNextTokenOfPath(struct PF_STR* p_str, u32 wildcard) {
    u32 extsfn_len;
    u32 code_mode;

    p_str->p_head = p_str->p_tail;
    extsfn_len = 0;

    if (VFiPFSTR_StrNCmp(p_str, (const s8*)"\\", 1, 0, 1) == 0) {
        p_str->p_tail = 0;
        p_str->p_head = 0;
        return 0;
    }

    if (VFiPFSTR_StrNCmp(p_str, (const s8*)"\\", 1, 0, 1) == 0 || VFiPFSTR_StrNCmp(p_str, (const s8*)"/", 1, 0, 1) == 0) {
        VFiPFSTR_MoveStrPos(p_str, 1);
    }

    if (VFiPFSTR_StrNCmp(p_str, (const s8*)"\\", 1, 0, 1) == 0 || VFiPFSTR_StrNCmp(p_str, (const s8*)"/", 1, 0, 1) == 0) {
        return 2;
    }

    code_mode = VFiPFSTR_GetCodeMode(p_str);
    p_str->p_tail = p_str->p_head;

    if ((VFipf_vol_set.setting & 2) == 2) {
        extsfn_len = VFiPFPATH_CheckExtShortName(p_str, 2, wildcard);
        if (extsfn_len != 0) {
            if (code_mode == 1) {
                p_str->p_tail += extsfn_len;
            } else {
                p_str->p_tail += extsfn_len * 2;
            }
        }
    }

    if (extsfn_len == 0) {
        while (VFiPFSTR_StrNCmp(p_str, (const s8*)"\\", 2, 0, 1) != 0) {
            if (code_mode == 1 && VFipf_vol_set.codeset.is_oem_mb_char(*p_str->p_tail, 1) != 0) {
                p_str->p_tail++;
                if (VFipf_vol_set.codeset.is_oem_mb_char(*p_str->p_tail, 2) == 0 || *p_str->p_tail == '\0') {
                    return 2;
                }
            } else {
                if (VFiPFSTR_StrNCmp(p_str, (const s8*)"\\", 2, 0, 1) == 0 || VFiPFSTR_StrNCmp(p_str, (const s8*)"/", 2, 0, 1) == 0) {
                    break;
                }

                if ((VFipf_vol_set.config & 0x10000) == 0) {
                    if (code_mode == 1 && (u8)*p_str->p_tail < 0x80 &&
                        ((u8)*p_str->p_tail < 0x20 || (VFipf_valid_fn_char[(u8)*p_str->p_tail - 0x20] & 1) == 0)) {
                        goto check_wildcard;
                    } else if (code_mode == 2) {
                        u16 bVar1 = (u64)(((u8)p_str->p_tail[0] << 8) + (u8)p_str->p_tail[1]);
                        u32 bVar3 = 1;

                        if (bVar1 < 0x80) {
                            s32 bVar2 = 0;

                            if ((u8)bVar1 >= 0x20 && ((u8)bVar1 & 0xFF00) == 0) {
                                if ((VFipf_valid_fn_char[(u8)((u8)bVar1 - 0x20)] & 1) != 0) {
                                    bVar2 = 1;
                                }
                            }
                            if (bVar2 == 0) {
                                bVar3 = 0;
                            }
                        }

                        if (bVar3 == 0) {
                        check_wildcard:
                            if (wildcard == 0 ||
                                (VFiPFSTR_StrNCmp(p_str, (const s8*)"*", 2, 0, 1) != 0 && VFiPFSTR_StrNCmp(p_str, (const s8*)"?", 2, 0, 1) != 0)) {
                                return 2;
                            }
                        }
                    }
                }
            }

            if (code_mode == 1) {
                p_str->p_tail += 1;
            } else {
                p_str->p_tail += 2;
            }
        }
    }
    return 0;
}

s32 VFiPFPATH_SplitPath(struct PF_STR* p_path, struct PF_STR* p_dir_path, struct PF_STR* p_filename) {
    VFiPFPATH_DoSplitPath(p_path, p_dir_path, p_filename, 0);
}

struct PF_VOLUME* VFiPFPATH_GetVolumeFromPath(struct PF_STR* p_path) {
    struct PF_VOLUME* p_vol;
    s8 drv_char[2];

    if (VFiPFSTR_StrLen(p_path) == 0) {
        p_vol = 0;
    } else {
        if (VFiPFSTR_StrNumChar(p_path, 1) >= 2 && VFiPFSTR_StrNCmp(p_path, (const s8*)":", 1, 1, 1) == 0) {
            VFiPFSTR_ToUpperNStr(p_path, 1, drv_char);
            p_vol = VFiPFVOL_GetVolumeFromDrvChar(drv_char[0]);
        } else {
            p_vol = VFiPFVOL_GetCurrentVolume();
        }
    }
    return p_vol;
}

u32 VFiPFPATH_MatchFileNameWithPattern(const s8* file_name, struct PF_STR* p_pattern, u32 is_long_name) {
    u16 c_name;
    u16 c_pat;
    struct PF_FILE_NAME_ITER name;
    struct PF_STR pattern;
    u32 is_match;
    s8 sig[2];

    is_match = 1;
    sig[0] = *(s8*)"~";
    sig[1] = *(s8*)"1";

    name.buf = file_name;
    name.dot_inserted = 0;
    name.is_long_name = is_long_name;
    name.index = 0;

    pattern = *p_pattern;

    if ((u32)VFiPFSTR_GetCodeMode(p_pattern) == 1) {
        if (is_long_name == 0 && (VFipf_vol_set.setting & 2) == 2 && (u32)VFiPFPATH_CheckExtShortNameSignature(&pattern) == 1 &&
            VFipf_strncmp(file_name, sig, 2) == 0) {
            name.index += 2;
            pattern.p_head += 2;
        }
    } else {
        if ((VFipf_vol_set.setting & 2) == 2 && is_long_name == 0 && VFiPFSTR_StrNCmp(p_pattern, (const s8*)".", 1, 0, 1) != 0 &&
            VFiPFSTR_StrNCmp(p_pattern, (const s8*)"..", 1, 0, 2) != 0 && VFiPFPATH_CheckExtShortName(p_pattern, 1, 0) == 0) {
            is_match = 0;
        }
    }

    if (is_match == 1) {
        c_name = VFiPFPATH_GetNextCharOfFileName(&name);
        c_pat = VFiPFPATH_GetNextCharOfPattern(&pattern, is_long_name);
        is_match = VFiPFPATH_DoMatchFileNameWithPattern(c_name, &name, c_pat, &pattern, is_long_name);
    }

    return is_match;
}

s32 VFiPFPATH_putShortName(u8* pDirEntry, const s8* short_name, u8 attr) {
    s32 i;

    i = 0;
    while (i < 8 && *short_name != '\0' && *short_name != '.') {
        *pDirEntry++ = *short_name++;
        i++;
    }

    if (i == 0) {
        i = 0;
        while (i < 2 && *short_name != '\0') {
            *pDirEntry++ = *short_name++;
            i++;
        }
    }

    if (i > 8) {
        return 10;
    }

    if ((attr & 8) == 0) {
        while (i < 8) {
            *pDirEntry++ = ' ';
            i++;
        }
    }

    if (*short_name != '\0') {
        if ((attr & 8) == 0) {
            short_name++;
        }
        while (*short_name != '\0') {
            *pDirEntry++ = *short_name++;
            i++;
        }
    }

    while (i < 11) {
        *pDirEntry++ = ' ';
        i++;
    }

    return 0;
}

s32 VFiPFPATH_getShortName(s8* short_name, const u8* pDirEntry, u8 attr) {
    s32 i;
    s32 nLen;

    i = 0;
    nLen = -1;
    for (i = 0; i < 8; i++) {
        short_name[i] = pDirEntry[i];
        if ((s32)short_name[i] != ' ') {
            nLen = i;
        }
    }

    short_name += (nLen += 1);
    if (nLen == 0) {
        return 10;
    }

    nLen = 7;
    for (i = 8; i < 11; i++) {
        if (pDirEntry[i] != ' ') {
            nLen = i;
        }
    }

    if (nLen > 7) {
        if ((attr & 8) == 0) {
            *short_name++ = '.';
        }
        for (i = 8; i <= nLen; i++) {
            *short_name++ = pDirEntry[i];
        }
    }
    *short_name = '\0';
    return 0;
}

void VFiPFPATH_getLongNameformShortName(s8* short_name, s8* long_name, u8 flag) {
    s32 i;
    s32 j;

    for (i = 0; i < 8; i++) {
        if (short_name[i] == '\0' || short_name[i] == '.') {
            break;
        }

        if ((flag & 8) != 0 && short_name[i] >= 'A' && short_name[i] <= 'Z') {
            long_name[i] = short_name[i] + 0x20;
        } else {
            long_name[i] = short_name[i];
        }
    }

    if (short_name[i] == '.') {
        long_name[i] = '.';
        i++;
    }

    j = i + 3;
    while (i < j) {
        if (short_name[i] == '\0') {
            break;
        }

        if ((flag & 0x10) != 0 && short_name[i] >= 'A' && short_name[i] <= 'Z') {
            long_name[i] = short_name[i] + 0x20;
        } else {
            long_name[i] = short_name[i];
        }
        i++;
    }

    long_name[i] = '\0';
}

u32 VFiPFPATH_GetLengthFromShortname(const s8* sSrc) {
    s32 i;
    u32 szStr;

    for (szStr = i = 0; sSrc[i]; i++) {
        if (i == 8 && (sSrc[i] != ' ' || sSrc[i + 1] != ' ' || sSrc[i + 2] != ' ')) {
            szStr++;
        }
        if (sSrc[i] != ' ') {
            szStr++;
        }
    }
    return szStr;
}

u32 VFiPFPATH_GetLengthFromUnicode(const u16* sSrc) {
    s32 i;
    s32 width;
    u32 szStr;
    s16 oem_width;
    s16 uni_width;
    s8 Dest[2];

    szStr = 0;
    width = 0;
    while (sSrc[width] != 0) {
        VFiPFCODE_Divide_Width(VFipf_vol_set.codeset.unicode2oem(sSrc + width, Dest), &oem_width, &uni_width);
        szStr += oem_width;
        width += uni_width >> 1;
    }
    return szStr;
}

s32 VFiPFPATH_transformFromUnicodeToNormal(s8* sDest, const u16* sSrc) {
    s32 i;
    s32 width;
    u32 szStr;
    s16 oem_width;
    s16 uni_width;
    u16 dot_buf[2];
    u16 space_buf[2];

    if ((VFipf_vol_set.setting & 2) == 2) {
        dot_buf[0] = '.';
        dot_buf[1] = 0;
        space_buf[0] = ' ';
        space_buf[1] = 0;
        for (i = 0; sSrc[i] != 0; i++) {
            if (VFipf_w_strncmp(sSrc + i, dot_buf, 1) == 0) {
                *sDest = '.';
            } else if (VFipf_w_strncmp(sSrc + i, space_buf, 1) == 0) {
                *sDest = ' ';
            } else {
                *sDest = '_';
            }
            sDest++;
        }
    } else {
        for (i = 0; sSrc[i] != 0; i += uni_width >> 1) {
            VFiPFCODE_Divide_Width(VFipf_vol_set.codeset.unicode2oem(sSrc + i, sDest), &oem_width, &uni_width);
            sDest += oem_width;
        }
    }
    *sDest = '\0';
    return 0;
}

s32 VFiPFPATH_transformInUnicode(u16* sDestStr, const s8* sSrcStr) {
    s32 i;
    s32 width;
    s32 szStr;
    s16 oem_width;
    s16 uni_width;

    szStr = 0;
    width = 0;
    while (sSrcStr[width] != '\0') {
        VFiPFCODE_Divide_Width(VFipf_vol_set.codeset.oem2unicode(sSrcStr + width, sDestStr), &oem_width, &uni_width);
        sDestStr += uni_width >> 1;
        width += oem_width;
        szStr++;
    }
    *sDestStr = 0;
    return szStr;
}

u32 VFiPFPATH_parseShortName(s8* pDest, struct PF_STR* p_pattern) {
    s32 width;
    const s8* p_cur_src;
    u32 is_create_long;
    u32 is_create_tail;
    u16* p_name_cnt;
    u16 num_base;
    u16 num_ext;
    u16 last_width;
    u16 prev_last_width;
    u16 src_dot;
    u16 prev_src_dot;
    u16 src_pos;
    u16 dst_pos;
    u16 ext_pos;
    u16 move_cnt;
    s16 i;
    u16 wchar;
    u16 t_wchar;

    is_create_long = 0;
    is_create_tail = 0;
    p_cur_src = VFiPFSTR_GetStrPos(p_pattern, 3);

    if ((VFipf_vol_set.setting & 2) == 0) {
        while (*p_cur_src == ' ' || *p_cur_src == '.') {
            is_create_tail = 1;
            p_cur_src++;
        }

        src_dot = 0;
        prev_src_dot = 0;
        src_pos = 0;
        while (p_cur_src[src_pos] != '\0') {
            if (p_cur_src[src_pos] == '.') {
                if (src_dot != 0) {
                    prev_src_dot = src_dot;
                }
                src_dot = src_pos;
            }
            src_pos++;
        }

        if (src_dot != 0 && p_cur_src[src_dot + 1] == '\0') {
            if (prev_src_dot != 0) {
                src_dot = prev_src_dot;
            }
            is_create_tail = 1;
        }

        dst_pos = 0;
        src_pos = 0;
        num_base = 0;
        num_ext = 0;
        p_name_cnt = &num_base;
        last_width = 1;
        prev_last_width = 1;

        while (num_ext < 3 && p_cur_src[src_pos] != '\0') {
            if (num_base != 8 && (src_dot == 0 || src_pos != src_dot) || p_name_cnt == &num_ext) {
                if ((p_cur_src[src_pos] != ' ') && (p_cur_src[src_pos] != '.')) {
                    width = VFipf_vol_set.codeset.oem_char_width(p_cur_src + src_pos);

                    if (width != 1) {
                        if (src_pos < src_dot || src_dot == 0) {
                            if ((num_base + width) > 8) {
                                is_create_tail = 1;
                                goto jump;  // remove me!
                            }
                        }

                        if (src_dot != 0 && src_pos > src_dot) {
                            if ((num_ext + width) > 3) {
                                is_create_tail = 1;
                                break;
                            }
                        }

                        if (p_name_cnt == &num_base) {
                            prev_last_width = last_width;
                            last_width = width;
                        }

                        while (width != 0) {
                            wchar = ((((u8)p_cur_src[src_pos]) << 8)) + ((u8)p_cur_src[src_pos + 1]);
                            if (VFiPFPATH_OEM_ConvertFWchar(p_cur_src + src_pos, &t_wchar)) {
                                wchar = t_wchar;
                                is_create_long = 1;
                            }

                            pDest[dst_pos] = (s8)(wchar >> 8);
                            pDest[dst_pos + 1] = (s8)wchar;
                            dst_pos += 2;
                            width -= 2;
                            src_pos += 2;
                            *p_name_cnt += 2;
                        }
                        src_pos -= (u8)1;
                    } else {
                        if (p_name_cnt == &num_base) {
                            prev_last_width = last_width;
                            last_width = 1;
                        }

                        if ((VFipf_vol_set.config & 0x10000) || (u8)p_cur_src[src_pos] >= 0x80 ||
                            ((u8)p_cur_src[src_pos] >= 0x20 && (VFipf_valid_fn_char[(u8)p_cur_src[src_pos] - 0x20] & 2))) {
                            pDest[dst_pos] = VFipf_toupper(p_cur_src[src_pos]);
                            dst_pos++;
                            (*p_name_cnt)++;

                            if (p_cur_src[src_pos] >= 'a' && p_cur_src[src_pos] <= 'z') {
                                is_create_long = 1;
                            }
                        } else {
                            is_create_tail = 1;

                            pDest[dst_pos] = '_';
                            dst_pos++;
                            (*p_name_cnt)++;
                        }
                    }
                } else {
                    is_create_tail = 1;
                }
            } else {
                p_name_cnt = &num_ext;
                if (p_cur_src[src_pos] != '\0') {
                    if (p_cur_src[src_pos] != '.') {
                        is_create_tail = 1;
                    }
                }
                if ((src_dot)) {
                    pDest[dst_pos++] = '.';
                    src_pos = src_dot;
                } else {
                    break;
                }
            }
        jump:
            src_pos++;
        }

        if ((num_ext == 3) && (p_cur_src[src_pos] != '\0')) {
            is_create_tail = 1;
        }
        pDest[dst_pos] = '\0';

        if (is_create_tail != 0) {
            dst_pos = num_base;
            if (num_base == 8) {
                if (last_width != 1) {
                    dst_pos = 8 - last_width;
                } else {
                    dst_pos = 8 - ((prev_last_width == 1) ? 2 : (prev_last_width + 1));
                }
            } else if (num_base == 7) {
                dst_pos = dst_pos - prev_last_width;
            }

            ext_pos = dst_pos + 2;
            if (ext_pos < num_base) {
                move_cnt = num_base - ext_pos;
                for (i = -1; i < ((num_ext + 1)); i++) {
                    pDest[num_base + i] = pDest[(num_base + move_cnt) + i];
                }
            } else if (ext_pos > num_base) {
                move_cnt = ext_pos - num_base;
                for (i = num_ext + 1; i >= 0; i--) {
                    pDest[(num_base + move_cnt) + i] = pDest[num_base + i];
                }
            }

            if (num_base != 0) {
                if ((VFipf_vol_set.setting & 2) != 2) {
                    pDest[dst_pos++] = '~';
                    pDest[dst_pos++] = '1';
                } else {
                    pDest[(dst_pos++)] = '_';
                    pDest[(dst_pos++)] = '_';
                }
            }
            is_create_long = 1;
        }

        if ((u32)VFiPFSTR_GetCodeMode(p_pattern) == 2) {
            is_create_long = 1;
        }
    } else {
        while ((*p_cur_src) != '\0' && !is_create_tail) {
            if (*p_cur_src != '.' && *p_cur_src != ' ') {
                is_create_tail = 1;
            }
            p_cur_src++;
        }

        if (is_create_tail != 0) {
            pDest[0] = 1;
            pDest[1] = 2;
            pDest[2] = '0';
            pDest[3] = '0';
            pDest[4] = '0';
            pDest[5] = '0';
            pDest[6] = '0';
            pDest[7] = '0';
            pDest[8] = '\0';
        } else {
            pDest[0] = '\0';
        }
        is_create_long = 1;
    }

    return is_create_long;
}

s32 VFiPFPATH_parseShortNameNumeric(s8* p_char, u32 count) {
    u32 numeric_cnt;
    u32 pos_tail;
    u32 pos_dot;
    u32 pos_ext;
    u32 pos_slide;
    u32 pos_end;
    s8 numeric[6];

    if (count == 0) {
        return 0;
    }

    for (pos_tail = 1; p_char[pos_tail] != '~'; pos_tail++) {
    }

    for (pos_dot = pos_tail + 1; p_char[pos_dot] != '.' && p_char[pos_dot] != '\0'; pos_dot++) {
    }

    pos_ext = pos_dot + 1;

    if (p_char[pos_dot] == '.' && p_char[pos_ext] != '\0') {
        for (pos_end = pos_ext + 1; p_char[pos_end] != '\0'; pos_end++) {
        }
    } else {
        pos_end = pos_ext;
    }

    numeric_cnt = 0;
    while (count != 0) {
        numeric[numeric_cnt] = (count % 10) + '0';
        numeric_cnt++;
        count /= 10;
    }

    if (pos_tail + numeric_cnt >= pos_ext) {
        pos_slide = pos_tail + numeric_cnt + 1;
        if (pos_slide > 8) {
            pos_slide = 8;
        }
        p_char[pos_slide + 4] = p_char[pos_end];
        p_char[pos_slide + 3] = p_char[pos_ext + 2];
        p_char[pos_slide + 2] = p_char[pos_ext + 1];
        p_char[pos_slide + 1] = p_char[pos_ext];
        p_char[pos_slide] = p_char[pos_dot];
    }

    if (pos_tail + numeric_cnt >= 8) {
        pos_tail = 7 - numeric_cnt;
    }

    p_char[pos_tail] = '~';
    pos_tail++;
    while (numeric_cnt != 0) {
        p_char[pos_tail] = numeric[numeric_cnt - 1];
        numeric_cnt--;
        pos_tail++;
    }

    return 0;
}

u32 VFiPFPATH_CheckExtShortNameSignature(struct PF_STR* p_str) {
    u32 result;
    s8 sig[2];

    sig[0] = *(s8*)"~";
    sig[1] = *(s8*)"1";

    result = 0;

    if (p_str == NULL) {
        return 10;
    }
    if (VFiPFSTR_StrNCmp(p_str, sig, 1, 0, 2) == 0) {
        result = 1;
    }
    return result;
}

u32 VFiPFPATH_CheckExtShortName(struct PF_STR* p_str, u32 target, u32 wildcard) {
    u32 result;
    s16 i;
    s16 num;
    u32 is_wildcard;
    s8 sig[2];
    s8* p_c;
    u16* p_wc;

    result = 0;
    is_wildcard = 0;
    sig[0] = *(s8*)"~";
    sig[1] = *(s8*)"1";

    if (p_str == NULL) {
        return 10;
    }

    if (VFiPFSTR_StrNCmp(p_str, sig, target, 0, 2) == 0 ||
        (VFiPFSTR_StrNCmp(p_str, (const s8*)".", target, 0, 1) == 0 &&
         (VFiPFSTR_StrNCmp(p_str, (const s8*)".", target, 1, 1) == 0 || VFiPFSTR_StrNCmp(p_str, (const s8*)"\\", target, 0, 1) == 0)) ||
        VFiPFSTR_StrNCmp(p_str, (const s8*)"\\", target, 0, 1) == 0) {
        i = 2;
        goto jump;
        while (1) {
            if ((u32)VFiPFSTR_GetCodeMode(p_str) == 1) {
                p_c = (s8*)VFiPFSTR_GetStrPos(p_str, target);
                num = p_c[i] - '0';
            } else {
                p_wc = (u16*)VFiPFSTR_GetStrPos(p_str, target);
                num = p_wc[i] - '0';
            }

            if (num < 0 || num > 9) {
                if (wildcard == 0) {
                    break;
                }

                if (VFiPFSTR_StrNCmp(p_str, (const s8*)"\\", target, i, 1) == 0 || VFiPFSTR_StrNCmp(p_str, (const s8*)".", target, i, 1) == 0) {
                    if (wildcard == 1 && VFiPFSTR_StrNCmp(p_str, (const s8*)"\\", target, i, 1) == 0) {
                        is_wildcard = 1;
                    }
                } else {
                    break;
                }
            }

            i++;
        jump:;
            if (i < 8) {
                if ((VFiPFSTR_StrNCmp(p_str, (const s8*)"\\", target, i, 1) == 0 || VFiPFSTR_StrNCmp(p_str, (const s8*)"/", target, i, 1) == 0) ==
                        0 &&
                    (VFiPFSTR_StrNCmp(p_str, (const s8*)"\0", target, i, 1) != 0 && VFiPFSTR_StrNCmp(p_str, (const s8*)" ", target, i, 1) != 0)) {
                    continue;
                }
            }
            break;
        }

        if (i == 8 || is_wildcard == 1) {
            if (((VFiPFSTR_StrNCmp(p_str, (const s8*)"\0", target, i, 1) == 0) ||
                 (VFiPFSTR_StrNCmp(p_str, (const s8*)"\\", target, i, 1) == 0 || VFiPFSTR_StrNCmp(p_str, (const s8*)"/", target, i, 1) == 0) == 0) ||
                (VFiPFSTR_StrNCmp(p_str, (const s8*)" ", target, i, 1) == 0)) {
                result = i;
            }
        }
    }
    return result;
}

u32 VFiPFPATH_GetExtShortNameIndex(struct PF_STR* p_str, u32* p_index) {
    u32 result;
    s16 i;
    s16 num;
    u32 index;
    s8 sig[2];
    s8* p_c;
    u16* p_wc;

    result = 0;
    sig[0] = *(s8*)"~";
    sig[1] = *(s8*)"1";

    if (p_str == NULL || p_index == NULL) {
        return 10;
    }

    if (VFiPFSTR_StrNCmp(p_str, sig, 1, 0, 2) == 0) {
        index = 0;
        i = 2;

        while (i < 8 || VFiPFSTR_StrNCmp(p_str, (const s8*)"", 1, i, 1) != 0) {
            if ((u32)VFiPFSTR_GetCodeMode(p_str) == 1) {
                p_c = (s8*)VFiPFSTR_GetStrPos(p_str, 1);
                num = p_c[i] - '0';
            } else {
                p_wc = (u16*)VFiPFSTR_GetStrPos(p_str, 1);
                num = p_wc[i] - '0';
            }

            if (num < 0 || num > 9) {
                break;
            }
            index = index * 10;
            index = index + num;
            i++;
        }

        if (i == 8) {
            u32 is_valid = 0;
            if (VFiPFSTR_StrNCmp(p_str, (const s8*)"\\", 1, i, 1) == 0 || VFiPFSTR_StrNCmp(p_str, (const s8*)"/", 1, i, 1) == 0) {
                is_valid = 1;
            }

            if (is_valid == 0 || VFiPFSTR_StrNCmp(p_str, 0, 1, i, 1) == 0) {
                *p_index = index;
                result = 1;
            }
        }
    }
    return result;
}

s32 VFiPFPATH_AdjustExtShortName(s8* pName, u32 position) {
    u32 num;
    u32 i;
    u32 div1;
    u32 div2;

    s8** pName_copy;

    if (pName == 0) {
        return 10;
    }

    pName += 7;
    i = 7;
    div1 = 10;
    div2 = 1;

    while ((i > 1) && (position != 0)) {
        num = position / div1;
        num = position - (num * div1);
        pName_copy = &pName;

        if (num != 0) {
            position = position - num;
            num = num / div2;
            // TODO: Fakematch
            *(*pName_copy) = (s8)((*pName) + ((s8)num));
        }

        div1 *= 10;
        i -= 1;
        pName -= 1;
        div2 *= 10;
    }

    return 0;
}

*/
