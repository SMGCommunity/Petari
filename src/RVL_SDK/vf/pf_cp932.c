#include "revolution/vf/pf_code.h"
#include "revolution/vf/vf_struct.h"

static u16 cp932_to_unicode[45][189];

int VFiPFCODE_CP932_OEM2Unicode(const char* cp932_src, unsigned short* uc_dst) {
    int lead_index;
    int trail_index;
    unsigned char c0;
    unsigned char c1;

    c0 = *cp932_src;
    c1 = *(cp932_src + 1);

    if (c0 < 0x80u) {
        *uc_dst = c0;
        return VFiPFCODE_Combine_Width(1, 2);
    }

    if (c0 >= 0xA1u && c0 <= 0xDFu) {
        *uc_dst = c0 + 0xFEC0;
        return VFiPFCODE_Combine_Width(1, 2);
    }

    if (c0 == 128 || c0 == 133 || c0 == 134 || c0 == 160 || c0 == 235 || c0 == 236 || (c0 >= 0xEFu && c0 <= 0xF9u) || (c0 >= 0xFDu && c0 == 0xFFu)) {
        *uc_dst = 95;
        return VFiPFCODE_Combine_Width(1, 2);
    }

    if (c1 >= 0xFDu) {
        *uc_dst = 95;
        return VFiPFCODE_Combine_Width(1, 2);
    }

    if (c0 >= 0x81u && c0 <= 0x84u)
        lead_index = c0 - 129;
    else if (c0 >= 0x87u && c0 <= 0x9Fu)
        lead_index = c0 - 131;
    else if (c0 >= 0xE0u && c0 <= 0xEAu)
        lead_index = c0 - 195;
    else if (c0 >= 0xEDu && c0 <= 0xEEu)
        lead_index = c0 - 197;
    else if (c0 >= 0xFAu && c0 <= 0xFCu)
        lead_index = c0 - 208;
    else {
        *uc_dst = 95;
        return VFiPFCODE_Combine_Width(1, 2);
    }

    trail_index = c1 - 64;

    if (trail_index < 0 || trail_index >= 189) {
        *uc_dst = 95;
        return VFiPFCODE_Combine_Width(1, 2);
    }

    if ((*uc_dst = cp932_to_unicode[lead_index][trail_index]) == 0) {
        *uc_dst = 95;
        return VFiPFCODE_Combine_Width(1, 2);
    }

    return VFiPFCODE_Combine_Width(2, 2);
}

s32 VFiPFCODE_CP932_Unicode2OEM(const u16* uc_src, s8* cp932_dst) {
    s32 i;
    s32 j;
    u16* p_table;
    u8 uc_lead;
    u8 uc_trail;
    u16 uc;
    u16 cp932;

    u16 temp;  // Extra variable. Not in DWARF.

    uc_lead = *uc_src & 0xFF;
    uc_trail = *uc_src >> 8;

    if ((uc_lead < 0x80) && (uc_trail == 0)) {
        cp932_dst[0] = (u8)uc_lead;
        cp932_dst[1] = 0;
        return VFiPFCODE_Combine_Width(1, 2);
    }

    uc = (u16)((uc_trail << 8) + uc_lead);
    if ((0xFF61 <= uc) && (uc <= 0xFF9F)) {
        temp = (u16)(uc - 0xFEC0);
        cp932_dst[0] = (u8)(temp);
        cp932_dst[1] = 0;
        return VFiPFCODE_Combine_Width(1, 2);
    }

    if (uc == 0x5F) {
        cp932_dst[0] = 0x5F;
        return VFiPFCODE_Combine_Width(1, 2);
    }

    i = 0;
    while (i < 45) {
        p_table = (u16*)cp932_to_unicode[i];
        j = 0;
        while (j < 189) {
            if (*p_table == uc) {
                break;
            }
            j++;
            p_table++;
        }
        if (j < 189) {
            break;
        }
        i++;
    }

    if (j == 189) {
        cp932_dst[0] = 0x5F;
        return VFiPFCODE_Combine_Width(1, 2);
    }

    if ((i >= 0) && (i <= 3)) {
        cp932 = ((i + 0x81) << 8) | (j + 0x40);
    } else if ((i >= 4) && (i <= 0x1C)) {
        cp932 = ((i + 0x83) << 8) | (j + 0x40);
    } else if ((i >= 0x1D) && (i <= 0x27)) {
        cp932 = ((i + 0xC3) << 8) | (j + 0x40);
    } else if ((i >= 0x28) && (i <= 0x29)) {
        cp932 = ((i + 0xC5) << 8) | (j + 0x40);
    } else if ((i >= 0x2A) && (i <= 0x2C)) {
        cp932 = ((i + 0xD0) << 8) | (j + 0x40);
    } else {
        cp932_dst[0] = 0x5F;
        return VFiPFCODE_Combine_Width(1, 2);
    }

    cp932_dst[1] = (u8)(cp932 & 0xFF);
    cp932_dst[0] = (cp932 >> 8) & 0xFF;
    return VFiPFCODE_Combine_Width(2, 2);
}

int VFiPFCODE_CP932_OEMCharWidth(const char* buf) {
    unsigned short tmp;
    int v1;
    short oem_width;
    short uni_width;

    v1 = VFiPFCODE_CP932_OEM2Unicode(buf, &tmp);
    VFiPFCODE_Divide_Width(v1, &oem_width, &uni_width);
    return oem_width;
}

u32 VFiPFCODE_CP932_isOEMMBchar(s8 cp932, u32 num) {
    u8 code;
    u32 is_mb;

    code = (u8)cp932;
    is_mb = 0;

    switch (num) {
    case 1:
        is_mb = 0;
        if ((code >= 0x81 && code <= 0x9F) || (code >= 0xE0 && code <= 0xFC)) {
            is_mb = 1;
        }
        return is_mb;

    case 2:
        is_mb = 0;
        if ((code >= 0x40 && code <= 0x7E) || (code >= 0x80 && code <= 0xFC)) {
            is_mb = 1;
        }
        return is_mb;
    }
    return is_mb;
}
s32 VFiPFCODE_CP932_UnicodeCharWidth() {
    return 2;
}

u32 VFiPFCODE_CP932_isUnicodeMBchar() {
    return 0;
}
