#ifndef PE_REG_H
#define PE_REG_H

#define PE_QUAD_OFFSET_ID 0x00000059

#define PE_REFRESH_RID_SIZE 8
#define PE_REFRESH_RID_SHIFT 24
#define PE_REFRESH_RID_MASK 0xff000000
#define PE_REFRESH_GET_RID(pe_refresh) ((((unsigned long)(pe_refresh)) & PE_REFRESH_RID_MASK) >> PE_REFRESH_RID_SHIFT)

#define PE_REFRESH_ENABLE_SIZE 1
#define PE_REFRESH_ENABLE_SHIFT 9
#define PE_REFRESH_ENABLE_MASK 0x00000200
#define PE_REFRESH_GET_ENABLE(pe_refresh) ((((unsigned long)(pe_refresh)) & PE_REFRESH_ENABLE_MASK) >> PE_REFRESH_ENABLE_SHIFT)

#define PE_REFRESH_INTERVAL_SIZE 9
#define PE_REFRESH_INTERVAL_SHIFT 0
#define PE_REFRESH_INTERVAL_MASK 0x000001ff
#define PE_REFRESH_GET_INTERVAL(pe_refresh) ((((unsigned long)(pe_refresh)) & PE_REFRESH_INTERVAL_MASK) >> PE_REFRESH_INTERVAL_SHIFT)

#define PE_REFRESH_TOTAL_SIZE 32
#define PE_REFRESH(interval, enable, rid)                                                                                                            \
    ((((unsigned long)(interval)) << PE_REFRESH_INTERVAL_SHIFT) | (((unsigned long)(enable)) << PE_REFRESH_ENABLE_SHIFT) |                           \
     (((unsigned long)(rid)) << PE_REFRESH_RID_SHIFT))

#define PE_CHICKEN_PIWR_SIZE 1
#define PE_CHICKEN_PIWR_SHIFT 0
#define PE_CHICKEN_PIWR_MASK 0x00000001
#define PE_CHICKEN_GET_PIWR(pe_chicken) ((((unsigned long)(pe_chicken)) & PE_CHICKEN_PIWR_MASK) >> PE_CHICKEN_PIWR_SHIFT)

#define PE_CHICKEN_TXCPY_FMT_SIZE 1
#define PE_CHICKEN_TXCPY_FMT_SHIFT 1
#define PE_CHICKEN_TXCPY_FMT_MASK 0x00000002
#define PE_CHICKEN_GET_TXCPY_FMT(pe_chicken) ((((unsigned long)(pe_chicken)) & PE_CHICKEN_TXCPY_FMT_MASK) >> PE_CHICKEN_TXCPY_FMT_SHIFT)

#define PE_CHICKEN_TXCPY_CCV_SIZE 1
#define PE_CHICKEN_TXCPY_CCV_SHIFT 2
#define PE_CHICKEN_TXCPY_CCV_MASK 0x00000004
#define PE_CHICKEN_GET_TXCPY_CCV(pe_chicken) ((((unsigned long)(pe_chicken)) & PE_CHICKEN_TXCPY_CCV_MASK) >> PE_CHICKEN_TXCPY_CCV_SHIFT)

#define PE_CHICKEN_BLENDOP_SIZE 1
#define PE_CHICKEN_BLENDOP_SHIFT 3
#define PE_CHICKEN_BLENDOP_MASK 0x00000008
#define PE_CHICKEN_GET_BLENDOP(pe_chicken) ((((unsigned long)(pe_chicken)) & PE_CHICKEN_BLENDOP_MASK) >> PE_CHICKEN_BLENDOP_SHIFT)

#define PE_CHICKEN_RID_SIZE 8
#define PE_CHICKEN_RID_SHIFT 24
#define PE_CHICKEN_RID_MASK 0xff000000
#define PE_CHICKEN_GET_RID(pe_chicken) ((((unsigned long)(pe_chicken)) & PE_CHICKEN_RID_MASK) >> PE_CHICKEN_RID_SHIFT)

#define PE_CMODE0_RID_SIZE 8
#define PE_CMODE0_RID_SHIFT 24
#define PE_CMODE0_RID_MASK 0xff000000
#define PE_CMODE0_GET_RID(pe_cmode0) ((((unsigned long)(pe_cmode0)) & PE_CMODE0_RID_MASK) >> PE_CMODE0_RID_SHIFT)

#define PE_CMODE1_RID_SIZE 8
#define PE_CMODE1_RID_SHIFT 24
#define PE_CMODE1_RID_MASK 0xff000000
#define PE_CMODE1_GET_RID(pe_cmode1) ((((unsigned long)(pe_cmode1)) & PE_CMODE1_RID_MASK) >> PE_CMODE1_RID_SHIFT)

#define PE_ZMODE_RID_SIZE 8
#define PE_ZMODE_RID_SHIFT 24
#define PE_ZMODE_RID_MASK 0xff000000
#define PE_ZMODE_GET_RID(pe_zmode) ((((unsigned long)(pe_zmode)) & PE_ZMODE_RID_MASK) >> PE_ZMODE_RID_SHIFT)

#define PE_CONTROL_RID_SIZE 8
#define PE_CONTROL_RID_SHIFT 24
#define PE_CONTROL_RID_MASK 0xff000000
#define PE_CONTROL_GET_RID(pe_control) ((((unsigned long)(pe_control)) & PE_CONTROL_RID_MASK) >> PE_CONTROL_RID_SHIFT)

#define PE_COPY_CMD_GAMMA_SIZE 2
#define PE_COPY_CMD_GAMMA_SHIFT 7
#define PE_COPY_CMD_GAMMA_MASK 0x00000180
#define PE_COPY_CMD_GET_GAMMA(pe_copy_cmd) ((((unsigned long)(pe_copy_cmd)) & PE_COPY_CMD_GAMMA_MASK) >> PE_COPY_CMD_GAMMA_SHIFT)

#define PE_TOKEN_TOKEN_SIZE 16
#define PE_TOKEN_TOKEN_SHIFT 0
#define PE_TOKEN_TOKEN_MASK 0x0000ffff
#define PE_TOKEN_GET_TOKEN(pe_token) ((((unsigned long)(pe_token)) & PE_TOKEN_TOKEN_MASK) >> PE_TOKEN_TOKEN_SHIFT)
#define PE_TOKEN_SET_TOKEN(pe_token, token)                                                                                                          \
    {                                                                                                                                                \
        FDL_ASSERT(!((token) & ~((1 << PE_TOKEN_TOKEN_SIZE) - 1)));                                                                                  \
        pe_token = (((unsigned long)(pe_token)) & ~PE_TOKEN_TOKEN_MASK) | (((unsigned long)(token)) << PE_TOKEN_TOKEN_SHIFT);                        \
    }

#define PE_TOKEN_RID_SIZE 8
#define PE_TOKEN_RID_SHIFT 24
#define PE_TOKEN_RID_MASK 0xff000000
#define PE_TOKEN_GET_RID(pe_token) ((((unsigned long)(pe_token)) & PE_TOKEN_RID_MASK) >> PE_TOKEN_RID_SHIFT)
#define PE_TOKEN_SET_RID(pe_token, rid)                                                                                                              \
    {                                                                                                                                                \
        FDL_ASSERT(!((rid) & ~((1 << PE_TOKEN_RID_SIZE) - 1)));                                                                                      \
        pe_token = (((unsigned long)(pe_token)) & ~PE_TOKEN_RID_MASK) | (((unsigned long)(rid)) << PE_TOKEN_RID_SHIFT);                              \
    }

#define PE_TOKEN_INT_TOKEN_SIZE 16
#define PE_TOKEN_INT_TOKEN_SHIFT 0
#define PE_TOKEN_INT_TOKEN_MASK 0x0000ffff
#define PE_TOKEN_INT_GET_TOKEN(pe_token_int) ((((unsigned long)(pe_token_int)) & PE_TOKEN_INT_TOKEN_MASK) >> PE_TOKEN_INT_TOKEN_SHIFT)
#define PE_TOKEN_INT_SET_TOKEN(pe_token_int, token)                                                                                                  \
    {                                                                                                                                                \
        FDL_ASSERT(!((token) & ~((1 << PE_TOKEN_INT_TOKEN_SIZE) - 1)));                                                                              \
        pe_token_int = (((unsigned long)(pe_token_int)) & ~PE_TOKEN_INT_TOKEN_MASK) | (((unsigned long)(token)) << PE_TOKEN_INT_TOKEN_SHIFT);        \
    }

#define PE_TOKEN_INT_RID_SIZE 8
#define PE_TOKEN_INT_RID_SHIFT 24
#define PE_TOKEN_INT_RID_MASK 0xff000000
#define PE_TOKEN_INT_GET_RID(pe_token_int) ((((unsigned long)(pe_token_int)) & PE_TOKEN_INT_RID_MASK) >> PE_TOKEN_INT_RID_SHIFT)
#define PE_TOKEN_INT_SET_RID(pe_token_int, rid)                                                                                                      \
    {                                                                                                                                                \
        FDL_ASSERT(!((rid) & ~((1 << PE_TOKEN_INT_RID_SIZE) - 1)));                                                                                  \
        pe_token_int = (((unsigned long)(pe_token_int)) & ~PE_TOKEN_INT_RID_MASK) | (((unsigned long)(rid)) << PE_TOKEN_INT_RID_SHIFT);              \
    }
#define PE_TOKEN_INT_TOTAL_SIZE 32
#define PE_TOKEN_INT(token, rid) ((((unsigned long)(token)) << PE_TOKEN_INT_TOKEN_SHIFT) | (((unsigned long)(rid)) << PE_TOKEN_INT_RID_SHIFT))

#define PE_FINISH_DST_SIZE 2
#define PE_FINISH_DST_SHIFT 0
#define PE_FINISH_DST_MASK 0x00000003
#define PE_FINISH_GET_DST(pe_finish) ((((unsigned long)(pe_finish)) & PE_FINISH_DST_MASK) >> PE_FINISH_DST_SHIFT)
#define PE_FINISH_SET_DST(pe_finish, dst)                                                                                                            \
    {                                                                                                                                                \
        FDL_ASSERT(!((dst) & ~((1 << PE_FINISH_DST_SIZE) - 1)));                                                                                     \
        pe_finish = (((unsigned long)(pe_finish)) & ~PE_FINISH_DST_MASK) | (((unsigned long)(dst)) << PE_FINISH_DST_SHIFT);                          \
    }
#define PE_FINISH_PAD0_SIZE 22
#define PE_FINISH_PAD0_SHIFT 2
#define PE_FINISH_PAD0_MASK 0x00fffffc
#define PE_FINISH_GET_PAD0(pe_finish) ((((unsigned long)(pe_finish)) & PE_FINISH_PAD0_MASK) >> PE_FINISH_PAD0_SHIFT)
#define PE_FINISH_SET_PAD0(pe_finish, pad0)                                                                                                          \
    {                                                                                                                                                \
        FDL_ASSERT(!((pad0) & ~((1 << PE_FINISH_PAD0_SIZE) - 1)));                                                                                   \
        pe_finish = (((unsigned long)(pe_finish)) & ~PE_FINISH_PAD0_MASK) | (((unsigned long)(pad0)) << PE_FINISH_PAD0_SHIFT);                       \
    }
#define PE_FINISH_RID_SIZE 8
#define PE_FINISH_RID_SHIFT 24
#define PE_FINISH_RID_MASK 0xff000000
#define PE_FINISH_GET_RID(pe_finish) ((((unsigned long)(pe_finish)) & PE_FINISH_RID_MASK) >> PE_FINISH_RID_SHIFT)
#define PE_FINISH_SET_RID(pe_finish, rid)                                                                                                            \
    {                                                                                                                                                \
        FDL_ASSERT(!((rid) & ~((1 << PE_FINISH_RID_SIZE) - 1)));                                                                                     \
        pe_finish = (((unsigned long)(pe_finish)) & ~PE_FINISH_RID_MASK) | (((unsigned long)(rid)) << PE_FINISH_RID_SHIFT);                          \
    }
#define PE_FINISH_TOTAL_SIZE 32
#define PE_FINISH(dst, rid) ((((unsigned long)(dst)) << PE_FINISH_DST_SHIFT) | (((unsigned long)(rid)) << PE_FINISH_RID_SHIFT))

#define PE_PI_CTL_AFMT_SIZE 2
#define PE_PI_CTL_AFMT_SHIFT 0
#define PE_PI_CTL_AFMT_MASK 0x00000003
#define PE_PI_CTL_GET_AFMT(pe_pi_ctl) ((((unsigned long)(pe_pi_ctl)) & PE_PI_CTL_AFMT_MASK) >> PE_PI_CTL_AFMT_SHIFT)
#define PE_PI_CTL_SET_AFMT(pe_pi_ctl, afmt)                                                                                                          \
    {                                                                                                                                                \
        FDL_ASSERT(!((afmt) & ~((1 << PE_PI_CTL_AFMT_SIZE) - 1)));                                                                                   \
        pe_pi_ctl = (((unsigned long)(pe_pi_ctl)) & ~PE_PI_CTL_AFMT_MASK) | (((unsigned long)(afmt)) << PE_PI_CTL_AFMT_SHIFT);                       \
    }
#define PE_PI_CTL_ZFMT_SIZE 1
#define PE_PI_CTL_ZFMT_SHIFT 2
#define PE_PI_CTL_ZFMT_MASK 0x00000004
#define PE_PI_CTL_GET_ZFMT(pe_pi_ctl) ((((unsigned long)(pe_pi_ctl)) & PE_PI_CTL_ZFMT_MASK) >> PE_PI_CTL_ZFMT_SHIFT)
#define PE_PI_CTL_SET_ZFMT(pe_pi_ctl, zfmt)                                                                                                          \
    {                                                                                                                                                \
        FDL_ASSERT(!((zfmt) & ~((1 << PE_PI_CTL_ZFMT_SIZE) - 1)));                                                                                   \
        pe_pi_ctl = (((unsigned long)(pe_pi_ctl)) & ~PE_PI_CTL_ZFMT_MASK) | (((unsigned long)(zfmt)) << PE_PI_CTL_ZFMT_SHIFT);                       \
    }
#define PE_PI_CTL_PAD0_SIZE 21
#define PE_PI_CTL_PAD0_SHIFT 3
#define PE_PI_CTL_PAD0_MASK 0x00fffff8
#define PE_PI_CTL_GET_PAD0(pe_pi_ctl) ((((unsigned long)(pe_pi_ctl)) & PE_PI_CTL_PAD0_MASK) >> PE_PI_CTL_PAD0_SHIFT)
#define PE_PI_CTL_SET_PAD0(pe_pi_ctl, pad0)                                                                                                          \
    {                                                                                                                                                \
        FDL_ASSERT(!((pad0) & ~((1 << PE_PI_CTL_PAD0_SIZE) - 1)));                                                                                   \
        pe_pi_ctl = (((unsigned long)(pe_pi_ctl)) & ~PE_PI_CTL_PAD0_MASK) | (((unsigned long)(pad0)) << PE_PI_CTL_PAD0_SHIFT);                       \
    }
#define PE_PI_CTL_RID_SIZE 8
#define PE_PI_CTL_RID_SHIFT 24
#define PE_PI_CTL_RID_MASK 0xff000000
#define PE_PI_CTL_GET_RID(pe_pi_ctl) ((((unsigned long)(pe_pi_ctl)) & PE_PI_CTL_RID_MASK) >> PE_PI_CTL_RID_SHIFT)
#define PE_PI_CTL_SET_RID(pe_pi_ctl, rid)                                                                                                            \
    {                                                                                                                                                \
        FDL_ASSERT(!((rid) & ~((1 << PE_PI_CTL_RID_SIZE) - 1)));                                                                                     \
        pe_pi_ctl = (((unsigned long)(pe_pi_ctl)) & ~PE_PI_CTL_RID_MASK) | (((unsigned long)(rid)) << PE_PI_CTL_RID_SHIFT);                          \
    }
#define PE_PI_CTL_TOTAL_SIZE 32
#define PE_PI_CTL(afmt, zfmt, rid)                                                                                                                   \
    ((((unsigned long)(afmt)) << PE_PI_CTL_AFMT_SHIFT) | (((unsigned long)(zfmt)) << PE_PI_CTL_ZFMT_SHIFT) |                                         \
     (((unsigned long)(rid)) << PE_PI_CTL_RID_SHIFT))

#define PE_CMODE0_BLEND_ENABLE_SIZE 1
#define PE_CMODE0_BLEND_ENABLE_SHIFT 0
#define PE_CMODE0_BLEND_ENABLE_MASK 0x00000001
#define PE_CMODE0_GET_BLEND_ENABLE(pe_cmode0) ((((unsigned long)(pe_cmode0)) & PE_CMODE0_BLEND_ENABLE_MASK) >> PE_CMODE0_BLEND_ENABLE_SHIFT)
#define PE_CMODE0_SET_BLEND_ENABLE(pe_cmode0, blend_enable)                                                                                          \
    {                                                                                                                                                \
        FDL_ASSERT(!((blend_enable) & ~((1 << PE_CMODE0_BLEND_ENABLE_SIZE) - 1)));                                                                   \
        pe_cmode0 =                                                                                                                                  \
            (((unsigned long)(pe_cmode0)) & ~PE_CMODE0_BLEND_ENABLE_MASK) | (((unsigned long)(blend_enable)) << PE_CMODE0_BLEND_ENABLE_SHIFT);       \
    }
#define PE_CMODE0_LOGICOP_ENABLE_SIZE 1
#define PE_CMODE0_LOGICOP_ENABLE_SHIFT 1
#define PE_CMODE0_LOGICOP_ENABLE_MASK 0x00000002
#define PE_CMODE0_GET_LOGICOP_ENABLE(pe_cmode0) ((((unsigned long)(pe_cmode0)) & PE_CMODE0_LOGICOP_ENABLE_MASK) >> PE_CMODE0_LOGICOP_ENABLE_SHIFT)
#define PE_CMODE0_SET_LOGICOP_ENABLE(pe_cmode0, logicop_enable)                                                                                      \
    {                                                                                                                                                \
        FDL_ASSERT(!((logicop_enable) & ~((1 << PE_CMODE0_LOGICOP_ENABLE_SIZE) - 1)));                                                               \
        pe_cmode0 =                                                                                                                                  \
            (((unsigned long)(pe_cmode0)) & ~PE_CMODE0_LOGICOP_ENABLE_MASK) | (((unsigned long)(logicop_enable)) << PE_CMODE0_LOGICOP_ENABLE_SHIFT); \
    }
#define PE_CMODE0_DITHER_ENABLE_SIZE 1
#define PE_CMODE0_DITHER_ENABLE_SHIFT 2
#define PE_CMODE0_DITHER_ENABLE_MASK 0x00000004
#define PE_CMODE0_GET_DITHER_ENABLE(pe_cmode0) ((((unsigned long)(pe_cmode0)) & PE_CMODE0_DITHER_ENABLE_MASK) >> PE_CMODE0_DITHER_ENABLE_SHIFT)
#define PE_CMODE0_SET_DITHER_ENABLE(pe_cmode0, dither_enable)                                                                                        \
    {                                                                                                                                                \
        FDL_ASSERT(!((dither_enable) & ~((1 << PE_CMODE0_DITHER_ENABLE_SIZE) - 1)));                                                                 \
        pe_cmode0 =                                                                                                                                  \
            (((unsigned long)(pe_cmode0)) & ~PE_CMODE0_DITHER_ENABLE_MASK) | (((unsigned long)(dither_enable)) << PE_CMODE0_DITHER_ENABLE_SHIFT);    \
    }
#define PE_CMODE0_COLOR_MASK_SIZE 1
#define PE_CMODE0_COLOR_MASK_SHIFT 3
#define PE_CMODE0_COLOR_MASK_MASK 0x00000008
#define PE_CMODE0_GET_COLOR_MASK(pe_cmode0) ((((unsigned long)(pe_cmode0)) & PE_CMODE0_COLOR_MASK_MASK) >> PE_CMODE0_COLOR_MASK_SHIFT)
#define PE_CMODE0_SET_COLOR_MASK(pe_cmode0, color_mask)                                                                                              \
    {                                                                                                                                                \
        FDL_ASSERT(!((color_mask) & ~((1 << PE_CMODE0_COLOR_MASK_SIZE) - 1)));                                                                       \
        pe_cmode0 = (((unsigned long)(pe_cmode0)) & ~PE_CMODE0_COLOR_MASK_MASK) | (((unsigned long)(color_mask)) << PE_CMODE0_COLOR_MASK_SHIFT);     \
    }
#define PE_CMODE0_ALPHA_MASK_SIZE 1
#define PE_CMODE0_ALPHA_MASK_SHIFT 4
#define PE_CMODE0_ALPHA_MASK_MASK 0x00000010
#define PE_CMODE0_GET_ALPHA_MASK(pe_cmode0) ((((unsigned long)(pe_cmode0)) & PE_CMODE0_ALPHA_MASK_MASK) >> PE_CMODE0_ALPHA_MASK_SHIFT)
#define PE_CMODE0_SET_ALPHA_MASK(pe_cmode0, alpha_mask)                                                                                              \
    {                                                                                                                                                \
        FDL_ASSERT(!((alpha_mask) & ~((1 << PE_CMODE0_ALPHA_MASK_SIZE) - 1)));                                                                       \
        pe_cmode0 = (((unsigned long)(pe_cmode0)) & ~PE_CMODE0_ALPHA_MASK_MASK) | (((unsigned long)(alpha_mask)) << PE_CMODE0_ALPHA_MASK_SHIFT);     \
    }
#define PE_CMODE0_DFACTOR_SIZE 3
#define PE_CMODE0_DFACTOR_SHIFT 5
#define PE_CMODE0_DFACTOR_MASK 0x000000e0
#define PE_CMODE0_GET_DFACTOR(pe_cmode0) ((((unsigned long)(pe_cmode0)) & PE_CMODE0_DFACTOR_MASK) >> PE_CMODE0_DFACTOR_SHIFT)
#define PE_CMODE0_SET_DFACTOR(pe_cmode0, dfactor)                                                                                                    \
    {                                                                                                                                                \
        FDL_ASSERT(!((dfactor) & ~((1 << PE_CMODE0_DFACTOR_SIZE) - 1)));                                                                             \
        pe_cmode0 = (((unsigned long)(pe_cmode0)) & ~PE_CMODE0_DFACTOR_MASK) | (((unsigned long)(dfactor)) << PE_CMODE0_DFACTOR_SHIFT);              \
    }
#define PE_CMODE0_SFACTOR_SIZE 3
#define PE_CMODE0_SFACTOR_SHIFT 8
#define PE_CMODE0_SFACTOR_MASK 0x00000700
#define PE_CMODE0_GET_SFACTOR(pe_cmode0) ((((unsigned long)(pe_cmode0)) & PE_CMODE0_SFACTOR_MASK) >> PE_CMODE0_SFACTOR_SHIFT)
#define PE_CMODE0_SET_SFACTOR(pe_cmode0, sfactor)                                                                                                    \
    {                                                                                                                                                \
        FDL_ASSERT(!((sfactor) & ~((1 << PE_CMODE0_SFACTOR_SIZE) - 1)));                                                                             \
        pe_cmode0 = (((unsigned long)(pe_cmode0)) & ~PE_CMODE0_SFACTOR_MASK) | (((unsigned long)(sfactor)) << PE_CMODE0_SFACTOR_SHIFT);              \
    }
#define PE_CMODE0_BLENDOP_SIZE 1
#define PE_CMODE0_BLENDOP_SHIFT 11
#define PE_CMODE0_BLENDOP_MASK 0x00000800
#define PE_CMODE0_GET_BLENDOP(pe_cmode0) ((((unsigned long)(pe_cmode0)) & PE_CMODE0_BLENDOP_MASK) >> PE_CMODE0_BLENDOP_SHIFT)
#define PE_CMODE0_SET_BLENDOP(pe_cmode0, blendop)                                                                                                    \
    {                                                                                                                                                \
        FDL_ASSERT(!((blendop) & ~((1 << PE_CMODE0_BLENDOP_SIZE) - 1)));                                                                             \
        pe_cmode0 = (((unsigned long)(pe_cmode0)) & ~PE_CMODE0_BLENDOP_MASK) | (((unsigned long)(blendop)) << PE_CMODE0_BLENDOP_SHIFT);              \
    }
#define PE_CMODE0_LOGICOP_SIZE 4
#define PE_CMODE0_LOGICOP_SHIFT 12
#define PE_CMODE0_LOGICOP_MASK 0x0000f000
#define PE_CMODE0_GET_LOGICOP(pe_cmode0) ((((unsigned long)(pe_cmode0)) & PE_CMODE0_LOGICOP_MASK) >> PE_CMODE0_LOGICOP_SHIFT)
#define PE_CMODE0_SET_LOGICOP(pe_cmode0, logicop)                                                                                                    \
    {                                                                                                                                                \
        FDL_ASSERT(!((logicop) & ~((1 << PE_CMODE0_LOGICOP_SIZE) - 1)));                                                                             \
        pe_cmode0 = (((unsigned long)(pe_cmode0)) & ~PE_CMODE0_LOGICOP_MASK) | (((unsigned long)(logicop)) << PE_CMODE0_LOGICOP_SHIFT);              \
    }
#define PE_CMODE0_PAD0_SIZE 8
#define PE_CMODE0_PAD0_SHIFT 16
#define PE_CMODE0_PAD0_MASK 0x00ff0000
#define PE_CMODE0_GET_PAD0(pe_cmode0) ((((unsigned long)(pe_cmode0)) & PE_CMODE0_PAD0_MASK) >> PE_CMODE0_PAD0_SHIFT)
#define PE_CMODE0_SET_PAD0(pe_cmode0, pad0)                                                                                                          \
    {                                                                                                                                                \
        FDL_ASSERT(!((pad0) & ~((1 << PE_CMODE0_PAD0_SIZE) - 1)));                                                                                   \
        pe_cmode0 = (((unsigned long)(pe_cmode0)) & ~PE_CMODE0_PAD0_MASK) | (((unsigned long)(pad0)) << PE_CMODE0_PAD0_SHIFT);                       \
    }
#define PE_CMODE0_RID_SIZE 8
#define PE_CMODE0_RID_SHIFT 24
#define PE_CMODE0_RID_MASK 0xff000000
#define PE_CMODE0_GET_RID(pe_cmode0) ((((unsigned long)(pe_cmode0)) & PE_CMODE0_RID_MASK) >> PE_CMODE0_RID_SHIFT)
#define PE_CMODE0_SET_RID(pe_cmode0, rid)                                                                                                            \
    {                                                                                                                                                \
        FDL_ASSERT(!((rid) & ~((1 << PE_CMODE0_RID_SIZE) - 1)));                                                                                     \
        pe_cmode0 = (((unsigned long)(pe_cmode0)) & ~PE_CMODE0_RID_MASK) | (((unsigned long)(rid)) << PE_CMODE0_RID_SHIFT);                          \
    }
#define PE_CMODE0_TOTAL_SIZE 32
#define PE_CMODE0(blend_enable, logicop_enable, dither_enable, color_mask, alpha_mask, dfactor, sfactor, blendop, logicop, rid)                      \
    ((((unsigned long)(blend_enable)) << PE_CMODE0_BLEND_ENABLE_SHIFT) | (((unsigned long)(logicop_enable)) << PE_CMODE0_LOGICOP_ENABLE_SHIFT) |     \
     (((unsigned long)(dither_enable)) << PE_CMODE0_DITHER_ENABLE_SHIFT) | (((unsigned long)(color_mask)) << PE_CMODE0_COLOR_MASK_SHIFT) |           \
     (((unsigned long)(alpha_mask)) << PE_CMODE0_ALPHA_MASK_SHIFT) | (((unsigned long)(dfactor)) << PE_CMODE0_DFACTOR_SHIFT) |                       \
     (((unsigned long)(sfactor)) << PE_CMODE0_SFACTOR_SHIFT) | (((unsigned long)(blendop)) << PE_CMODE0_BLENDOP_SHIFT) |                             \
     (((unsigned long)(logicop)) << PE_CMODE0_LOGICOP_SHIFT) | (((unsigned long)(rid)) << PE_CMODE0_RID_SHIFT))

#define PE_CMODE1_CONSTANT_ALPHA_SIZE 8
#define PE_CMODE1_CONSTANT_ALPHA_SHIFT 0
#define PE_CMODE1_CONSTANT_ALPHA_MASK 0x000000ff
#define PE_CMODE1_GET_CONSTANT_ALPHA(pe_cmode1) ((((unsigned long)(pe_cmode1)) & PE_CMODE1_CONSTANT_ALPHA_MASK) >> PE_CMODE1_CONSTANT_ALPHA_SHIFT)
#define PE_CMODE1_SET_CONSTANT_ALPHA(pe_cmode1, constant_alpha)                                                                                      \
    {                                                                                                                                                \
        FDL_ASSERT(!((constant_alpha) & ~((1 << PE_CMODE1_CONSTANT_ALPHA_SIZE) - 1)));                                                               \
        pe_cmode1 =                                                                                                                                  \
            (((unsigned long)(pe_cmode1)) & ~PE_CMODE1_CONSTANT_ALPHA_MASK) | (((unsigned long)(constant_alpha)) << PE_CMODE1_CONSTANT_ALPHA_SHIFT); \
    }
#define PE_CMODE1_CONSTANT_ALPHA_ENABLE_SIZE 1
#define PE_CMODE1_CONSTANT_ALPHA_ENABLE_SHIFT 8
#define PE_CMODE1_CONSTANT_ALPHA_ENABLE_MASK 0x00000100
#define PE_CMODE1_GET_CONSTANT_ALPHA_ENABLE(pe_cmode1)                                                                                               \
    ((((unsigned long)(pe_cmode1)) & PE_CMODE1_CONSTANT_ALPHA_ENABLE_MASK) >> PE_CMODE1_CONSTANT_ALPHA_ENABLE_SHIFT)
#define PE_CMODE1_SET_CONSTANT_ALPHA_ENABLE(pe_cmode1, constant_alpha_enable)                                                                        \
    {                                                                                                                                                \
        FDL_ASSERT(!((constant_alpha_enable) & ~((1 << PE_CMODE1_CONSTANT_ALPHA_ENABLE_SIZE) - 1)));                                                 \
        pe_cmode1 = (((unsigned long)(pe_cmode1)) & ~PE_CMODE1_CONSTANT_ALPHA_ENABLE_MASK) |                                                         \
                    (((unsigned long)(constant_alpha_enable)) << PE_CMODE1_CONSTANT_ALPHA_ENABLE_SHIFT);                                             \
    }
#define PE_CMODE1_YUV_SIZE 2
#define PE_CMODE1_YUV_SHIFT 9
#define PE_CMODE1_YUV_MASK 0x00000600
#define PE_CMODE1_GET_YUV(pe_cmode1) ((((unsigned long)(pe_cmode1)) & PE_CMODE1_YUV_MASK) >> PE_CMODE1_YUV_SHIFT)
#define PE_CMODE1_SET_YUV(pe_cmode1, yuv)                                                                                                            \
    {                                                                                                                                                \
        FDL_ASSERT(!((yuv) & ~((1 << PE_CMODE1_YUV_SIZE) - 1)));                                                                                     \
        pe_cmode1 = (((unsigned long)(pe_cmode1)) & ~PE_CMODE1_YUV_MASK) | (((unsigned long)(yuv)) << PE_CMODE1_YUV_SHIFT);                          \
    }
#define PE_CMODE1_PAD0_SIZE 13
#define PE_CMODE1_PAD0_SHIFT 11
#define PE_CMODE1_PAD0_MASK 0x00fff800
#define PE_CMODE1_GET_PAD0(pe_cmode1) ((((unsigned long)(pe_cmode1)) & PE_CMODE1_PAD0_MASK) >> PE_CMODE1_PAD0_SHIFT)
#define PE_CMODE1_SET_PAD0(pe_cmode1, pad0)                                                                                                          \
    {                                                                                                                                                \
        FDL_ASSERT(!((pad0) & ~((1 << PE_CMODE1_PAD0_SIZE) - 1)));                                                                                   \
        pe_cmode1 = (((unsigned long)(pe_cmode1)) & ~PE_CMODE1_PAD0_MASK) | (((unsigned long)(pad0)) << PE_CMODE1_PAD0_SHIFT);                       \
    }
#define PE_CMODE1_RID_SIZE 8
#define PE_CMODE1_RID_SHIFT 24
#define PE_CMODE1_RID_MASK 0xff000000
#define PE_CMODE1_GET_RID(pe_cmode1) ((((unsigned long)(pe_cmode1)) & PE_CMODE1_RID_MASK) >> PE_CMODE1_RID_SHIFT)
#define PE_CMODE1_SET_RID(pe_cmode1, rid)                                                                                                            \
    {                                                                                                                                                \
        FDL_ASSERT(!((rid) & ~((1 << PE_CMODE1_RID_SIZE) - 1)));                                                                                     \
        pe_cmode1 = (((unsigned long)(pe_cmode1)) & ~PE_CMODE1_RID_MASK) | (((unsigned long)(rid)) << PE_CMODE1_RID_SHIFT);                          \
    }
#define PE_CMODE1_TOTAL_SIZE 32
#define PE_CMODE1(constant_alpha, constant_alpha_enable, yuv, rid)                                                                                   \
    ((((unsigned long)(constant_alpha)) << PE_CMODE1_CONSTANT_ALPHA_SHIFT) |                                                                         \
     (((unsigned long)(constant_alpha_enable)) << PE_CMODE1_CONSTANT_ALPHA_ENABLE_SHIFT) | (((unsigned long)(yuv)) << PE_CMODE1_YUV_SHIFT) |         \
     (((unsigned long)(rid)) << PE_CMODE1_RID_SHIFT))

#define PE_ZMODE_ENABLE_SIZE 1
#define PE_ZMODE_ENABLE_SHIFT 0
#define PE_ZMODE_ENABLE_MASK 0x00000001
#define PE_ZMODE_GET_ENABLE(pe_zmode) ((((unsigned long)(pe_zmode)) & PE_ZMODE_ENABLE_MASK) >> PE_ZMODE_ENABLE_SHIFT)
#define PE_ZMODE_SET_ENABLE(pe_zmode, enable)                                                                                                        \
    {                                                                                                                                                \
        FDL_ASSERT(!((enable) & ~((1 << PE_ZMODE_ENABLE_SIZE) - 1)));                                                                                \
        pe_zmode = (((unsigned long)(pe_zmode)) & ~PE_ZMODE_ENABLE_MASK) | (((unsigned long)(enable)) << PE_ZMODE_ENABLE_SHIFT);                     \
    }
#define PE_ZMODE_FUNC_SIZE 3
#define PE_ZMODE_FUNC_SHIFT 1
#define PE_ZMODE_FUNC_MASK 0x0000000e
#define PE_ZMODE_GET_FUNC(pe_zmode) ((((unsigned long)(pe_zmode)) & PE_ZMODE_FUNC_MASK) >> PE_ZMODE_FUNC_SHIFT)
#define PE_ZMODE_SET_FUNC(pe_zmode, func)                                                                                                            \
    {                                                                                                                                                \
        FDL_ASSERT(!((func) & ~((1 << PE_ZMODE_FUNC_SIZE) - 1)));                                                                                    \
        pe_zmode = (((unsigned long)(pe_zmode)) & ~PE_ZMODE_FUNC_MASK) | (((unsigned long)(func)) << PE_ZMODE_FUNC_SHIFT);                           \
    }
#define PE_ZMODE_MASK_SIZE 1
#define PE_ZMODE_MASK_SHIFT 4
#define PE_ZMODE_MASK_MASK 0x00000010
#define PE_ZMODE_GET_MASK(pe_zmode) ((((unsigned long)(pe_zmode)) & PE_ZMODE_MASK_MASK) >> PE_ZMODE_MASK_SHIFT)
#define PE_ZMODE_SET_MASK(pe_zmode, mask)                                                                                                            \
    {                                                                                                                                                \
        FDL_ASSERT(!((mask) & ~((1 << PE_ZMODE_MASK_SIZE) - 1)));                                                                                    \
        pe_zmode = (((unsigned long)(pe_zmode)) & ~PE_ZMODE_MASK_MASK) | (((unsigned long)(mask)) << PE_ZMODE_MASK_SHIFT);                           \
    }
#define PE_ZMODE_PAD0_SIZE 19
#define PE_ZMODE_PAD0_SHIFT 5
#define PE_ZMODE_PAD0_MASK 0x00ffffe0
#define PE_ZMODE_GET_PAD0(pe_zmode) ((((unsigned long)(pe_zmode)) & PE_ZMODE_PAD0_MASK) >> PE_ZMODE_PAD0_SHIFT)
#define PE_ZMODE_SET_PAD0(pe_zmode, pad0)                                                                                                            \
    {                                                                                                                                                \
        FDL_ASSERT(!((pad0) & ~((1 << PE_ZMODE_PAD0_SIZE) - 1)));                                                                                    \
        pe_zmode = (((unsigned long)(pe_zmode)) & ~PE_ZMODE_PAD0_MASK) | (((unsigned long)(pad0)) << PE_ZMODE_PAD0_SHIFT);                           \
    }
#define PE_ZMODE_RID_SIZE 8
#define PE_ZMODE_RID_SHIFT 24
#define PE_ZMODE_RID_MASK 0xff000000
#define PE_ZMODE_GET_RID(pe_zmode) ((((unsigned long)(pe_zmode)) & PE_ZMODE_RID_MASK) >> PE_ZMODE_RID_SHIFT)
#define PE_ZMODE_SET_RID(pe_zmode, rid)                                                                                                              \
    {                                                                                                                                                \
        FDL_ASSERT(!((rid) & ~((1 << PE_ZMODE_RID_SIZE) - 1)));                                                                                      \
        pe_zmode = (((unsigned long)(pe_zmode)) & ~PE_ZMODE_RID_MASK) | (((unsigned long)(rid)) << PE_ZMODE_RID_SHIFT);                              \
    }
#define PE_ZMODE_TOTAL_SIZE 32
#define PE_ZMODE(enable, func, mask, rid)                                                                                                            \
    ((((unsigned long)(enable)) << PE_ZMODE_ENABLE_SHIFT) | (((unsigned long)(func)) << PE_ZMODE_FUNC_SHIFT) |                                       \
     (((unsigned long)(mask)) << PE_ZMODE_MASK_SHIFT) | (((unsigned long)(rid)) << PE_ZMODE_RID_SHIFT))

#define PE_PI_EFB_ADDR_PAD0_SIZE 2
#define PE_PI_EFB_ADDR_PAD0_SHIFT 0
#define PE_PI_EFB_ADDR_PAD0_MASK 0x00000003
#define PE_PI_EFB_ADDR_GET_PAD0(pe_pi_efb_addr) ((((unsigned long)(pe_pi_efb_addr)) & PE_PI_EFB_ADDR_PAD0_MASK) >> PE_PI_EFB_ADDR_PAD0_SHIFT)
#define PE_PI_EFB_ADDR_SET_PAD0(pe_pi_efb_addr, pad0)                                                                                                \
    {                                                                                                                                                \
        FDL_ASSERT(!((pad0) & ~((1 << PE_PI_EFB_ADDR_PAD0_SIZE) - 1)));                                                                              \
        pe_pi_efb_addr = (((unsigned long)(pe_pi_efb_addr)) & ~PE_PI_EFB_ADDR_PAD0_MASK) | (((unsigned long)(pad0)) << PE_PI_EFB_ADDR_PAD0_SHIFT);   \
    }
#define PE_PI_EFB_ADDR_X_SIZE 10
#define PE_PI_EFB_ADDR_X_SHIFT 2
#define PE_PI_EFB_ADDR_X_MASK 0x00000ffc
#define PE_PI_EFB_ADDR_GET_X(pe_pi_efb_addr) ((((unsigned long)(pe_pi_efb_addr)) & PE_PI_EFB_ADDR_X_MASK) >> PE_PI_EFB_ADDR_X_SHIFT)
#define PE_PI_EFB_ADDR_SET_X(pe_pi_efb_addr, x)                                                                                                      \
    {                                                                                                                                                \
        FDL_ASSERT(!((x) & ~((1 << PE_PI_EFB_ADDR_X_SIZE) - 1)));                                                                                    \
        pe_pi_efb_addr = (((unsigned long)(pe_pi_efb_addr)) & ~PE_PI_EFB_ADDR_X_MASK) | (((unsigned long)(x)) << PE_PI_EFB_ADDR_X_SHIFT);            \
    }
#define PE_PI_EFB_ADDR_Y_SIZE 10
#define PE_PI_EFB_ADDR_Y_SHIFT 12
#define PE_PI_EFB_ADDR_Y_MASK 0x003ff000
#define PE_PI_EFB_ADDR_GET_Y(pe_pi_efb_addr) ((((unsigned long)(pe_pi_efb_addr)) & PE_PI_EFB_ADDR_Y_MASK) >> PE_PI_EFB_ADDR_Y_SHIFT)
#define PE_PI_EFB_ADDR_SET_Y(pe_pi_efb_addr, y)                                                                                                      \
    {                                                                                                                                                \
        FDL_ASSERT(!((y) & ~((1 << PE_PI_EFB_ADDR_Y_SIZE) - 1)));                                                                                    \
        pe_pi_efb_addr = (((unsigned long)(pe_pi_efb_addr)) & ~PE_PI_EFB_ADDR_Y_MASK) | (((unsigned long)(y)) << PE_PI_EFB_ADDR_Y_SHIFT);            \
    }
#define PE_PI_EFB_ADDR_TYPE_SIZE 2
#define PE_PI_EFB_ADDR_TYPE_SHIFT 22
#define PE_PI_EFB_ADDR_TYPE_MASK 0x00c00000
#define PE_PI_EFB_ADDR_GET_TYPE(pe_pi_efb_addr) ((((unsigned long)(pe_pi_efb_addr)) & PE_PI_EFB_ADDR_TYPE_MASK) >> PE_PI_EFB_ADDR_TYPE_SHIFT)
#define PE_PI_EFB_ADDR_SET_TYPE(pe_pi_efb_addr, type)                                                                                                \
    {                                                                                                                                                \
        FDL_ASSERT(!((type) & ~((1 << PE_PI_EFB_ADDR_TYPE_SIZE) - 1)));                                                                              \
        pe_pi_efb_addr = (((unsigned long)(pe_pi_efb_addr)) & ~PE_PI_EFB_ADDR_TYPE_MASK) | (((unsigned long)(type)) << PE_PI_EFB_ADDR_TYPE_SHIFT);   \
    }
#define PE_PI_EFB_ADDR_TOTAL_SIZE 24
#define PE_PI_EFB_ADDR(x, y, type)                                                                                                                   \
    ((((unsigned long)(x)) << PE_PI_EFB_ADDR_X_SHIFT) | (((unsigned long)(y)) << PE_PI_EFB_ADDR_Y_SHIFT) |                                           \
     (((unsigned long)(type)) << PE_PI_EFB_ADDR_TYPE_SHIFT))

#define PE_INTRCTL_INT0EN_SIZE 1
#define PE_INTRCTL_INT0EN_SHIFT 0
#define PE_INTRCTL_INT0EN_MASK 0x00000001
#define PE_INTRCTL_GET_INT0EN(pe_intrctl) ((((unsigned long)(pe_intrctl)) & PE_INTRCTL_INT0EN_MASK) >> PE_INTRCTL_INT0EN_SHIFT)
#define PE_INTRCTL_SET_INT0EN(pe_intrctl, int0en)                                                                                                    \
    {                                                                                                                                                \
        FDL_ASSERT(!((int0en) & ~((1 << PE_INTRCTL_INT0EN_SIZE) - 1)));                                                                              \
        pe_intrctl = (((unsigned long)(pe_intrctl)) & ~PE_INTRCTL_INT0EN_MASK) | (((unsigned long)(int0en)) << PE_INTRCTL_INT0EN_SHIFT);             \
    }
#define PE_INTRCTL_INT1EN_SIZE 1
#define PE_INTRCTL_INT1EN_SHIFT 1
#define PE_INTRCTL_INT1EN_MASK 0x00000002
#define PE_INTRCTL_GET_INT1EN(pe_intrctl) ((((unsigned long)(pe_intrctl)) & PE_INTRCTL_INT1EN_MASK) >> PE_INTRCTL_INT1EN_SHIFT)
#define PE_INTRCTL_SET_INT1EN(pe_intrctl, int1en)                                                                                                    \
    {                                                                                                                                                \
        FDL_ASSERT(!((int1en) & ~((1 << PE_INTRCTL_INT1EN_SIZE) - 1)));                                                                              \
        pe_intrctl = (((unsigned long)(pe_intrctl)) & ~PE_INTRCTL_INT1EN_MASK) | (((unsigned long)(int1en)) << PE_INTRCTL_INT1EN_SHIFT);             \
    }
#define PE_INTRCTL_INT0CLR_SIZE 1
#define PE_INTRCTL_INT0CLR_SHIFT 2
#define PE_INTRCTL_INT0CLR_MASK 0x00000004
#define PE_INTRCTL_GET_INT0CLR(pe_intrctl) ((((unsigned long)(pe_intrctl)) & PE_INTRCTL_INT0CLR_MASK) >> PE_INTRCTL_INT0CLR_SHIFT)
#define PE_INTRCTL_SET_INT0CLR(pe_intrctl, int0clr)                                                                                                  \
    {                                                                                                                                                \
        FDL_ASSERT(!((int0clr) & ~((1 << PE_INTRCTL_INT0CLR_SIZE) - 1)));                                                                            \
        pe_intrctl = (((unsigned long)(pe_intrctl)) & ~PE_INTRCTL_INT0CLR_MASK) | (((unsigned long)(int0clr)) << PE_INTRCTL_INT0CLR_SHIFT);          \
    }
#define PE_INTRCTL_INT1CLR_SIZE 1
#define PE_INTRCTL_INT1CLR_SHIFT 3
#define PE_INTRCTL_INT1CLR_MASK 0x00000008
#define PE_INTRCTL_GET_INT1CLR(pe_intrctl) ((((unsigned long)(pe_intrctl)) & PE_INTRCTL_INT1CLR_MASK) >> PE_INTRCTL_INT1CLR_SHIFT)
#define PE_INTRCTL_SET_INT1CLR(pe_intrctl, int1clr)                                                                                                  \
    {                                                                                                                                                \
        FDL_ASSERT(!((int1clr) & ~((1 << PE_INTRCTL_INT1CLR_SIZE) - 1)));                                                                            \
        pe_intrctl = (((unsigned long)(pe_intrctl)) & ~PE_INTRCTL_INT1CLR_MASK) | (((unsigned long)(int1clr)) << PE_INTRCTL_INT1CLR_SHIFT);          \
    }
#define PE_INTRCTL_TOTAL_SIZE 4
#define PE_INTRCTL(int0en, int1en, int0clr, int1clr)                                                                                                 \
    ((((unsigned long)(int0en)) << PE_INTRCTL_INT0EN_SHIFT) | (((unsigned long)(int1en)) << PE_INTRCTL_INT1EN_SHIFT) |                               \
     (((unsigned long)(int0clr)) << PE_INTRCTL_INT0CLR_SHIFT) | (((unsigned long)(int1clr)) << PE_INTRCTL_INT1CLR_SHIFT))

#define PE_QUAD_OFFSET_X_SIZE 10
#define PE_QUAD_OFFSET_X_SHIFT 0
#define PE_QUAD_OFFSET_X_MASK 0x000003ff
#define PE_QUAD_OFFSET_GET_X(pe_quad_offset) ((((unsigned long)(pe_quad_offset)) & PE_QUAD_OFFSET_X_MASK) >> PE_QUAD_OFFSET_X_SHIFT)
#define PE_QUAD_OFFSET_SET_X(pe_quad_offset, x)                                                                                                      \
    {                                                                                                                                                \
        FDL_ASSERT(!((x) & ~((1 << PE_QUAD_OFFSET_X_SIZE) - 1)));                                                                                    \
        pe_quad_offset = (((unsigned long)(pe_quad_offset)) & ~PE_QUAD_OFFSET_X_MASK) | (((unsigned long)(x)) << PE_QUAD_OFFSET_X_SHIFT);            \
    }

#define PE_QUAD_OFFSET_Y_SIZE 10
#define PE_QUAD_OFFSET_Y_SHIFT 10
#define PE_QUAD_OFFSET_Y_MASK 0x000ffc00
#define PE_QUAD_OFFSET_GET_Y(pe_quad_offset) ((((unsigned long)(pe_quad_offset)) & PE_QUAD_OFFSET_Y_MASK) >> PE_QUAD_OFFSET_Y_SHIFT)
#define PE_QUAD_OFFSET_SET_Y(pe_quad_offset, y)                                                                                                      \
    { pe_quad_offset = (((unsigned long)(pe_quad_offset)) & ~PE_QUAD_OFFSET_Y_MASK) | (((unsigned long)(y)) << PE_QUAD_OFFSET_Y_SHIFT); }

#define PE_QUAD_OFFSET_RID_SIZE 8
#define PE_QUAD_OFFSET_RID_SHIFT 24
#define PE_QUAD_OFFSET_RID_MASK 0xff000000
#define PE_QUAD_OFFSET_GET_RID(pe_quad_offset) ((((unsigned long)(pe_quad_offset)) & PE_QUAD_OFFSET_RID_MASK) >> PE_QUAD_OFFSET_RID_SHIFT)
#define PE_QUAD_OFFSET_SET_RID(pe_quad_offset, rid)                                                                                                  \
    { pe_quad_offset = (((unsigned long)(pe_quad_offset)) & ~PE_QUAD_OFFSET_RID_MASK) | (((unsigned long)(rid)) << PE_QUAD_OFFSET_RID_SHIFT); }
#define PE_QUAD_OFFSET_TOTAL_SIZE 32
#define PE_QUAD_OFFSET(x, y, rid)                                                                                                                    \
    ((((unsigned long)(x)) << PE_QUAD_OFFSET_X_SHIFT) | (((unsigned long)(y)) << PE_QUAD_OFFSET_Y_SHIFT) |                                           \
     (((unsigned long)(rid)) << PE_QUAD_OFFSET_RID_SHIFT))

#define PE_CONTROL_ZTOP_SIZE 1
#define PE_CONTROL_ZTOP_SHIFT 6
#define PE_CONTROL_ZTOP_MASK 0x00000040
#define PE_CONTROL_GET_ZTOP(pe_control) ((((unsigned long)(pe_control)) & PE_CONTROL_ZTOP_MASK) >> PE_CONTROL_ZTOP_SHIFT)
#define PE_CONTROL_SET_ZTOP(pe_control, ztop)                                                                                                        \
    { pe_control = (((unsigned long)(pe_control)) & ~PE_CONTROL_ZTOP_MASK) | (((unsigned long)(ztop)) << PE_CONTROL_ZTOP_SHIFT); }
#define PE_CONTROL_PIXTYPE_SIZE 3
#define PE_CONTROL_PIXTYPE_SHIFT 0
#define PE_CONTROL_PIXTYPE_MASK 0x00000007
#define PE_CONTROL_GET_PIXTYPE(pe_control) ((((unsigned long)(pe_control)) & PE_CONTROL_PIXTYPE_MASK) >> PE_CONTROL_PIXTYPE_SHIFT)
#define PE_CONTROL_SET_PIXTYPE(pe_control, pixtype)                                                                                                  \
    { pe_control = (((unsigned long)(pe_control)) & ~PE_CONTROL_PIXTYPE_MASK) | (((unsigned long)(pixtype)) << PE_CONTROL_PIXTYPE_SHIFT); }
#define PE_CONTROL_ZCMODE_SIZE 3
#define PE_CONTROL_ZCMODE_SHIFT 3
#define PE_CONTROL_ZCMODE_MASK 0x00000038
#define PE_CONTROL_GET_ZCMODE(pe_control) ((((unsigned long)(pe_control)) & PE_CONTROL_ZCMODE_MASK) >> PE_CONTROL_ZCMODE_SHIFT)
#define PE_CONTROL_SET_ZCMODE(pe_control, zcmode)                                                                                                    \
    { pe_control = (((unsigned long)(pe_control)) & ~PE_CONTROL_ZCMODE_MASK) | (((unsigned long)(zcmode)) << PE_CONTROL_ZCMODE_SHIFT); }

#define PE_FIELD_MASK_EVEN_SIZE 1
#define PE_FIELD_MASK_EVEN_SHIFT 0
#define PE_FIELD_MASK_EVEN_MASK 0x00000001
#define PE_FIELD_MASK_GET_EVEN(pe_field_mask) ((((unsigned long)(pe_field_mask)) & PE_FIELD_MASK_EVEN_MASK) >> PE_FIELD_MASK_EVEN_SHIFT)
#define PE_FIELD_MASK_SET_EVEN(pe_field_mask, even)                                                                                                  \
    { pe_field_mask = (((unsigned long)(pe_field_mask)) & ~PE_FIELD_MASK_EVEN_MASK) | (((unsigned long)(even)) << PE_FIELD_MASK_EVEN_SHIFT); }

#define PE_FIELD_MASK_ODD_SIZE 1
#define PE_FIELD_MASK_ODD_SHIFT 1
#define PE_FIELD_MASK_ODD_MASK 0x00000002
#define PE_FIELD_MASK_GET_ODD(pe_field_mask) ((((unsigned long)(pe_field_mask)) & PE_FIELD_MASK_ODD_MASK) >> PE_FIELD_MASK_ODD_SHIFT)
#define PE_FIELD_MASK_SET_ODD(pe_field_mask, odd)                                                                                                    \
    { pe_field_mask = (((unsigned long)(pe_field_mask)) & ~PE_FIELD_MASK_ODD_MASK) | (((unsigned long)(odd)) << PE_FIELD_MASK_ODD_SHIFT); }

#define PE_FIELD_MASK_RID_SIZE 8
#define PE_FIELD_MASK_RID_SHIFT 24
#define PE_FIELD_MASK_RID_MASK 0xff000000
#define PE_FIELD_MASK_GET_RID(pe_field_mask) ((((unsigned long)(pe_field_mask)) & PE_FIELD_MASK_RID_MASK) >> PE_FIELD_MASK_RID_SHIFT)
#define PE_FIELD_MASK_SET_RID(pe_field_mask, rid)                                                                                                    \
    { pe_field_mask = (((unsigned long)(pe_field_mask)) & ~PE_FIELD_MASK_RID_MASK) | (((unsigned long)(rid)) << PE_FIELD_MASK_RID_SHIFT); }

#endif  // PE_REG_H
