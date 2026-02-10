#include "revolution/vf/pf_str.h"
#include "revolution/vf/pf_clib.h"
#include "revolution/vf/pf_w_clib.h"

void VFiPFSTR_SetCodeMode(PF_STR* p_str, u32 code_mode) {
    p_str->code_mode = code_mode;
}

u32 VFiPFSTR_GetCodeMode(PF_STR* p_str) {
    return p_str->code_mode;
}

s8* VFiPFSTR_GetStrPos(PF_STR* p_str, u32 target) {
    s8* p_pos;
    if (target == 1) {
        p_pos = (s8*)p_str->p_head;
    } else {
        p_pos = (s8*)p_str->p_tail;
    }

    return p_pos;
}

void VFiPFSTR_MoveStrPos(PF_STR* p_str, s16 num_char) {
    s16 cnt;
    s16 offset;
    s32 width;
    s8* p;

    offset = 0;

    if (p_str->code_mode == 1) {
        p = (s8*)p_str->p_head;

        while (num_char != 0) {
            if (VFipf_vol_set.codeset.is_oem_mb_char(*p, 1)) {
                offset++;
            }
            offset++;
            num_char--;
        }
    } else {
        p = (s8*)p_str->p_head;
        for (cnt = 0; cnt < num_char; cnt++) {
            width = VFipf_vol_set.codeset.unicode_char_width((u16*)p);
            p += width;
            offset += (s16)width;
        }
    }

    p_str->p_head += offset;
}

s32 VFiPFSTR_InitStr(PF_STR* p_str, const s8* s, u32 code_mode) {
    if (p_str == NULL || s == NULL) {
        return 10;
    }

    if (code_mode == 1) {
        p_str->p_head = s;
        p_str->p_tail = s + VFipf_strlen(s);
    } else if (code_mode == 2) {
        p_str->p_head = s;
        p_str->p_tail = s + (2 * VFipf_w_strlen((const u16*)s));
    } else {
        return 10;
    }

    p_str->code_mode = code_mode;
    return 0;
}

u16 VFiPFSTR_StrLen(PF_STR* p_str) {
    return p_str->p_tail - p_str->p_head;
}

u16 VFiPFSTR_StrNumChar(PF_STR* p_str, u32 target) {
    s8* p;
    u16 cnt;
    s32 width;

    if (target == 1) {
        p = (s8*)p_str->p_head;
    } else {
        p = (s8*)p_str->p_tail;
    }

    if (p_str->code_mode == 1) {
        cnt = 0;
        while (*p != '\0') {
            if (VFipf_vol_set.codeset.is_oem_mb_char(*p, 1)) {
                p++;
            }
            p++;
            cnt++;
        }
    } else {
        cnt = 0;
        while (*p != '\0' || p[1] != '\0') {
            width = VFipf_vol_set.codeset.unicode_char_width((u16*)p);
            p += width;
            cnt++;
        }
    }

    return cnt;
}

s32 VFiPFSTR_StrCmp(const PF_STR* p_str, const s8* s) {
    u16 wc;
    const u16* wp;
    const s8* p1;
    const s8* p2;
    s32 ret;

    p2 = s;
    if (p_str->code_mode == 1) {
        return VFipf_strcmp(p_str->p_head, s);
    }

    p1 = p_str->p_head;
    do {
        VFipf_vol_set.codeset.oem2unicode((s8*)p2, &wc);

        wp = (const u16*)p1;
        ret = (s32)*wp;

        p2++;
        p1 += 2;

    } while ((u16)ret == wc && *(u16*)(p1 - 2) != 0 && wc != 0);

    return *(u16*)(p1 - 2) - wc;
}

s32 VFiPFSTR_StrNCmp(PF_STR* p_str, const s8* s, u32 target, s16 offset, u16 num) {
    u16 wc;
    const u16* wp;
    const s8* p1;
    const s8* p2;
    s32 ret;

    p2 = s;
    if (p_str->code_mode == 1) {
        if (target == 1) {
            p1 = p_str->p_head + offset;
        } else {
            p1 = p_str->p_tail + offset;
        }
        return VFipf_strncmp(p1, p2, num);
    }

    p1 = (target == 1) ? p_str->p_head + (2 * offset) : p_str->p_tail + (2 * offset);

    do {
        VFipf_vol_set.codeset.oem2unicode((s8*)s, &wc);

        wp = (const u16*)p1;
        ret = (s32)*wp;

        s++;
        num--;
        p1 += 2;

    } while ((u16)ret == wc && num != 0 && *(u16*)(p1 - 2) != 0 && wc != 0);

    return *(u16*)(p1 - 2) - wc;
}

void VFiPFSTR_ToUpperNStr(PF_STR* p_str, u16 num, s8* dest) {
    u16 wc;
    const u16* wp;
    const s8* p;

    if (p_str->code_mode == 1) {
        for (p = p_str->p_head; num != 0 && *p != '\0'; p++, num--) {
            *dest++ = (s8)VFipf_toupper(*p);
        }
    } else {
        for (wp = (const u16*)p_str->p_head; num != 0 && *wp != 0; num--, wp++, dest += 2) {
            wc = *wp;

            wc = wc >= 0x61 && wc <= 0x7A ? wc - 32 : wc;

            *dest = (s8)wc;
            dest[1] = (s8)(wc >> 8);
        }
        *dest++ = 0;
    }
    *dest = 0;
}
