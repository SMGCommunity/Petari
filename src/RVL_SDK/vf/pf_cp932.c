#include "revolution/vf/pf_code.h"
#include "revolution/vf/vf_struct.h"

static u16 cp932_to_unicode[45][189];

int VFiPFCODE_CP932_OEM2Unicode(const char* cp932_src, unsigned short* uc_dst) {
    int lead_index;
    unsigned int trail_index;
    unsigned char c0;
    unsigned char c1;

    c0 = *cp932_src;
    c1 = *(cp932_src + 1);

    if (c0 < 0x80u) {
        *uc_dst = c0;
        return VFiPFCODE_Combine_Width(1, 2);
    }

    if (c0 >= 0xA1u && c0 <= 0xDFu) {
        *uc_dst = c0 - 320;
        return VFiPFCODE_Combine_Width(1, 2);
    }

    if (c0 == 128 || c0 == 133 || c0 == 134 || c0 == 160 || c0 == 235 || c0 == 236 || (c0 >= 0xEFu && c0 <= 0xF9u) || c0 == 255 || c1 >= 0xFDu) {
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

    if (trail_index > 0xBC || (*uc_dst = cp932_to_unicode[lead_index][trail_index]) == 0) {
        *uc_dst = 95;
        return VFiPFCODE_Combine_Width(1, 2);
    }

    return VFiPFCODE_Combine_Width(2, 2);
}

int VFiPFCODE_CP932_Unicode2OEM(const unsigned short* uc_src, signed char* cp932_dst) {
    int v2;
    int i;
    s16 v5;
    unsigned char v6;
    unsigned char v7;
    unsigned short* j;

    v2 = 0;
    v7 = (*uc_src >> 8) & 0xFF;
    v6 = *uc_src;
    if (v6 < 0x80u && !v7) {
        *cp932_dst = v6;
        cp932_dst[1] = 0;
        return VFiPFCODE_Combine_Width(1, 2);
    }
    if (((v7 << 8) + v6) >= 0xFF61u && ((v7 << 8) + v6) <= 0xFF9Fu) {
        *cp932_dst = v6 + 64;
        cp932_dst[1] = 0;
        return VFiPFCODE_Combine_Width(1, 2);
    }
    if (((v7 << 8) + v6) == 95)
        goto LABEL_8;
    for (i = 0; i < 45; ++i) {
        v2 = 0;
        for (j = cp932_to_unicode[i]; v2 < 189 && *j != ((v7 << 8) + v6); ++j)
            ++v2;
        if (v2 < 189)
            break;
    }
    if (v2 == 189)
        goto LABEL_8;
    if (i >= 4) {
        if (i >= 4 && i <= 28) {
            v5 = ((i + 131) << 8) | (v2 + 64);
            goto LABEL_33;
        }
        if (i >= 29 && i <= 39) {
            v5 = ((i + 195) << 8) | (v2 + 64);
            goto LABEL_33;
        }
        if (i >= 40 && i <= 41) {
            v5 = ((i + 197) << 8) | (v2 + 64);
            goto LABEL_33;
        }
        if (i >= 42 && i <= 44) {
            v5 = ((i + 208) << 8) | (v2 + 64);
            goto LABEL_33;
        }
    LABEL_8:
        *cp932_dst = 95;
        return VFiPFCODE_Combine_Width(1, 2);
    }
    v5 = ((i + 129) << 8) | (v2 + 64);
LABEL_33:
    *cp932_dst = v5;
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

u16 VFiPFCODE_CP932_isOEMMBchar(short cp932, unsigned int num) {
    unsigned int v2;
    char v3;
    char v4;
    unsigned int v6;
    char v7;
    char v8;

    if (num == 2) {
        v6 = 1;
        v7 = 0;
        if (cp932 >= 0x40u) {
            if (cp932 <= 0x7F) {
                v7 = 1;
            } else {
                v7 = 0;
            }
        }

        if (!v7) {
            v8 = 0;
            if (cp932 >= 0x80u) {
                if (cp932 <= 0xFC) {
                    v8 = 1;
                } else {
                    v8 = 0;
                }
            }

            if (!v8)
                return 0;
        }
        return v6;
    } else if (num == 1) {
        v2 = 1;
        v3 = 0;
        if (cp932 >= 0x81u) {
            if (cp932 <= 0x9F) {
                v3 = 1;
            } else {
                v3 = 0;
            }
        }
        if (!v3) {
            v4 = 0;
            if (cp932 >= 0xE0u) {
                if (cp932 <= 0xFC) {
                    v4 = 1;
                } else {
                    v4 = 0;
                }
            }

            if (!v4)
                return 0;
        }
        return v2;
    } else {
        return 0;
    }
}

s32 VFiPFCODE_CP932_UnicodeCharWidth() {
    return 2;
}

u32 VFiPFCODE_CP932_isUnicodeMBchar() {
    return 0;
}
