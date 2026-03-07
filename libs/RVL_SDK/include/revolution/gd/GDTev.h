#ifndef GDTEV_H
#define GDTEV_H

#define TEV_COLOR_ENV_0_ID 0x000000c0
#define TEV_ALPHA_ENV_0_ID 0x000000c1
#define TEV_COLOR_ENV_1_ID 0x000000c2
#define TEV_ALPHA_ENV_1_ID 0x000000c3
#define TEV_COLOR_ENV_2_ID 0x000000c4
#define TEV_ALPHA_ENV_2_ID 0x000000c5
#define TEV_COLOR_ENV_3_ID 0x000000c6
#define TEV_ALPHA_ENV_3_ID 0x000000c7
#define TEV_COLOR_ENV_4_ID 0x000000c8
#define TEV_ALPHA_ENV_4_ID 0x000000c9
#define TEV_COLOR_ENV_5_ID 0x000000ca
#define TEV_ALPHA_ENV_5_ID 0x000000cb
#define TEV_COLOR_ENV_6_ID 0x000000cc
#define TEV_ALPHA_ENV_6_ID 0x000000cd
#define TEV_COLOR_ENV_7_ID 0x000000ce
#define TEV_ALPHA_ENV_7_ID 0x000000cf
#define TEV_COLOR_ENV_8_ID 0x000000d0
#define TEV_ALPHA_ENV_8_ID 0x000000d1
#define TEV_COLOR_ENV_9_ID 0x000000d2
#define TEV_ALPHA_ENV_9_ID 0x000000d3
#define TEV_COLOR_ENV_A_ID 0x000000d4
#define TEV_ALPHA_ENV_A_ID 0x000000d5
#define TEV_COLOR_ENV_B_ID 0x000000d6
#define TEV_ALPHA_ENV_B_ID 0x000000d7
#define TEV_COLOR_ENV_C_ID 0x000000d8
#define TEV_ALPHA_ENV_C_ID 0x000000d9
#define TEV_COLOR_ENV_D_ID 0x000000da
#define TEV_ALPHA_ENV_D_ID 0x000000db
#define TEV_COLOR_ENV_E_ID 0x000000dc
#define TEV_ALPHA_ENV_E_ID 0x000000dd
#define TEV_COLOR_ENV_F_ID 0x000000de
#define TEV_ALPHA_ENV_F_ID 0x000000df
#define TEV_REGISTERL_0_ID 0x000000e0
#define TEV_REGISTERH_0_ID 0x000000e1
#define TEV_REGISTERL_1_ID 0x000000e2
#define TEV_REGISTERH_1_ID 0x000000e3
#define TEV_REGISTERL_2_ID 0x000000e4
#define TEV_REGISTERH_2_ID 0x000000e5
#define TEV_REGISTERL_3_ID 0x000000e6
#define TEV_REGISTERH_3_ID 0x000000e7

#define TEV_COLOR_REG 0
#define TEV_KONSTANT_REG 1

#define TEV_ALPHAFUNC_ID 0x000000f3

#define RAS1_CC_0 0x00000000  /* color channel 0 */
#define RAS1_CC_1 0x00000001  /* color channel 1 */
#define RAS1_CC_B 0x00000005  /* indirect texture bump alpha */
#define RAS1_CC_BN 0x00000006 /* ind tex bump alpha, normalized 0-255 */
#define RAS1_CC_Z 0x00000007  /* set color value to zero */

#define RAS1_TREF0_ID 0x00000028

#define TEV_COLOR_ENV_SELD_SHIFT 0
#define TEV_COLOR_ENV_SELC_SHIFT 4
#define TEV_COLOR_ENV_SELB_SHIFT 8
#define TEV_COLOR_ENV_SELA_SHIFT 12
#define TEV_COLOR_ENV_BIAS_SHIFT 16
#define TEV_COLOR_ENV_SUB_SHIFT 18
#define TEV_COLOR_ENV_CLAMP_SHIFT 19
#define TEV_COLOR_ENV_SHIFT_SHIFT 20
#define TEV_COLOR_ENV_DEST_SHIFT 22
#define TEV_COLOR_ENV_RID_SHIFT 24
#define TEV_COLOR_ENV(seld, selc, selb, sela, bias, sub, clamp, shift, dest, rid)                                                                    \
    ((((unsigned long)(seld)) << TEV_COLOR_ENV_SELD_SHIFT) | (((unsigned long)(selc)) << TEV_COLOR_ENV_SELC_SHIFT) |                                 \
     (((unsigned long)(selb)) << TEV_COLOR_ENV_SELB_SHIFT) | (((unsigned long)(sela)) << TEV_COLOR_ENV_SELA_SHIFT) |                                 \
     (((unsigned long)(bias)) << TEV_COLOR_ENV_BIAS_SHIFT) | (((unsigned long)(sub)) << TEV_COLOR_ENV_SUB_SHIFT) |                                   \
     (((unsigned long)(clamp)) << TEV_COLOR_ENV_CLAMP_SHIFT) | (((unsigned long)(shift)) << TEV_COLOR_ENV_SHIFT_SHIFT) |                             \
     (((unsigned long)(dest)) << TEV_COLOR_ENV_DEST_SHIFT) | (((unsigned long)(rid)) << TEV_COLOR_ENV_RID_SHIFT))

#define TEV_ALPHA_ENV_RSWAP_SHIFT 0
#define TEV_ALPHA_ENV_TSWAP_SHIFT 2
#define TEV_ALPHA_ENV_SELD_SHIFT 4
#define TEV_ALPHA_ENV_SELC_SHIFT 7
#define TEV_ALPHA_ENV_SELB_SHIFT 10
#define TEV_ALPHA_ENV_SELA_SHIFT 13
#define TEV_ALPHA_ENV_BIAS_SHIFT 16
#define TEV_ALPHA_ENV_SUB_SHIFT 18
#define TEV_ALPHA_ENV_CLAMP_SHIFT 19
#define TEV_ALPHA_ENV_SHIFT_SHIFT 20
#define TEV_ALPHA_ENV_DEST_SHIFT 22
#define TEV_ALPHA_ENV_RID_SHIFT 24
#define TEV_ALPHA_ENV(rswap, tswap, seld, selc, selb, sela, bias, sub, clamp, shift, dest, rid)                                                      \
    ((((unsigned long)(rswap)) << TEV_ALPHA_ENV_RSWAP_SHIFT) | (((unsigned long)(tswap)) << TEV_ALPHA_ENV_TSWAP_SHIFT) |                             \
     (((unsigned long)(seld)) << TEV_ALPHA_ENV_SELD_SHIFT) | (((unsigned long)(selc)) << TEV_ALPHA_ENV_SELC_SHIFT) |                                 \
     (((unsigned long)(selb)) << TEV_ALPHA_ENV_SELB_SHIFT) | (((unsigned long)(sela)) << TEV_ALPHA_ENV_SELA_SHIFT) |                                 \
     (((unsigned long)(bias)) << TEV_ALPHA_ENV_BIAS_SHIFT) | (((unsigned long)(sub)) << TEV_ALPHA_ENV_SUB_SHIFT) |                                   \
     (((unsigned long)(clamp)) << TEV_ALPHA_ENV_CLAMP_SHIFT) | (((unsigned long)(shift)) << TEV_ALPHA_ENV_SHIFT_SHIFT) |                             \
     (((unsigned long)(dest)) << TEV_ALPHA_ENV_DEST_SHIFT) | (((unsigned long)(rid)) << TEV_ALPHA_ENV_RID_SHIFT))

#define TEV_REGISTERL_R_SHIFT 0
#define TEV_REGISTERL_A_SHIFT 12
#define TEV_REGISTERL_TYPE_SHIFT 23
#define TEV_REGISTERL_RID_SHIFT 24
#define TEV_REGISTERL(r, a, t, rid)                                                                                                                  \
    ((((unsigned long)(r)) << TEV_REGISTERL_R_SHIFT) | (((unsigned long)(a)) << TEV_REGISTERL_A_SHIFT) |                                             \
     (((unsigned long)(t)) << TEV_REGISTERL_TYPE_SHIFT) | (((unsigned long)(rid)) << TEV_REGISTERL_RID_SHIFT))

#define TEV_REGISTERH_B_SHIFT 0
#define TEV_REGISTERH_G_SHIFT 12
#define TEV_REGISTERH_TYPE_SHIFT 23
#define TEV_REGISTERH_RID_SHIFT 24
#define TEV_REGISTERH(b, g, t, rid)                                                                                                                  \
    ((((unsigned long)(b)) << TEV_REGISTERH_B_SHIFT) | (((unsigned long)(g)) << TEV_REGISTERH_G_SHIFT) |                                             \
     (((unsigned long)(t)) << TEV_REGISTERH_TYPE_SHIFT) | (((unsigned long)(rid)) << TEV_REGISTERH_RID_SHIFT))

#define TEV_ALPHAFUNC_A0_SHIFT 0
#define TEV_ALPHAFUNC_A1_SHIFT 8
#define TEV_ALPHAFUNC_OP0_SHIFT 16
#define TEV_ALPHAFUNC_OP1_SHIFT 19
#define TEV_ALPHAFUNC_LOGIC_SHIFT 22
#define TEV_ALPHAFUNC_RID_SHIFT 24
#define TEV_ALPHAFUNC(a0, a1, op0, op1, logic, rid)                                                                                                  \
    ((((unsigned long)(a0)) << TEV_ALPHAFUNC_A0_SHIFT) | (((unsigned long)(a1)) << TEV_ALPHAFUNC_A1_SHIFT) |                                         \
     (((unsigned long)(op0)) << TEV_ALPHAFUNC_OP0_SHIFT) | (((unsigned long)(op1)) << TEV_ALPHAFUNC_OP1_SHIFT) |                                     \
     (((unsigned long)(logic)) << TEV_ALPHAFUNC_LOGIC_SHIFT) | (((unsigned long)(rid)) << TEV_ALPHAFUNC_RID_SHIFT))

#define RAS1_TREF_TI0_SHIFT 0
#define RAS1_TREF_TC0_SHIFT 3
#define RAS1_TREF_TE0_SHIFT 6
#define RAS1_TREF_CC0_SHIFT 7
#define RAS1_TREF_PAD0_SHIFT 10
#define RAS1_TREF_TI1_SHIFT 12
#define RAS1_TREF_TC1_SHIFT 15
#define RAS1_TREF_TE1_SHIFT 18
#define RAS1_TREF_CC1_SHIFT 19
#define RAS1_TREF_PAD1_SHIFT 22
#define RAS1_TREF_RID_SHIFT 24
#define RAS1_TREF(ti0, tc0, te0, cc0, ti1, tc1, te1, cc1, rid)                                                                                       \
    ((((unsigned long)(ti0)) << RAS1_TREF_TI0_SHIFT) | (((unsigned long)(tc0)) << RAS1_TREF_TC0_SHIFT) |                                             \
     (((unsigned long)(te0)) << RAS1_TREF_TE0_SHIFT) | (((unsigned long)(cc0)) << RAS1_TREF_CC0_SHIFT) |                                             \
     (((unsigned long)(ti1)) << RAS1_TREF_TI1_SHIFT) | (((unsigned long)(tc1)) << RAS1_TREF_TC1_SHIFT) |                                             \
     (((unsigned long)(te1)) << RAS1_TREF_TE1_SHIFT) | (((unsigned long)(cc1)) << RAS1_TREF_CC1_SHIFT) |                                             \
     (((unsigned long)(rid)) << RAS1_TREF_RID_SHIFT))

#define BP_TEV_COLOR(d, c, b, a, bias, op, clamp, scale, out, id)                                                                                    \
    ((u32)(d) << 0 | (u32)(c) << 4 | (u32)(b) << 8 | (u32)(a) << 12 | (u32)(bias) << 16 | (u32)(op) << 18 | (u32)(clamp) << 19 |                     \
     (u32)(scale) << 20 | (u32)(out) << 22 | (u32)(id) << 24)

#define BP_TEV_ALPHA(ras_sel, tex_sel, d, c, b, a, bias, op, clamp, scale, out, id)                                                                  \
    ((u32)(ras_sel) << 0 | (u32)(tex_sel) << 2 | (u32)(d) << 4 | (u32)(c) << 7 | (u32)(b) << 10 | (u32)(a) << 13 | (u32)(bias) << 16 |               \
     (u32)(op) << 18 | (u32)(clamp) << 19 | (u32)(scale) << 20 | (u32)(out) << 22 | (u32)(id) << 24)

#define BP_TEV_COLOR_REG_RA(r, a, reg, id) ((u32)(r) << 0 | (u32)(a) << 12 | (u32)(reg) << 23 | (u32)(id) << 24)

#define BP_TEV_COLOR_REG_BG(b, g, reg, id) ((u32)(b) << 0 | (u32)(g) << 12 | (u32)(reg) << 23 | (u32)(id) << 24)

#define BP_TEV_KSEL(rb, ga, kcsel0, kasel0, kcsel1, kasel1, id)                                                                                      \
    ((u32)(rb) << 0 | (u32)(ga) << 2 | (u32)(kcsel0) << 4 | (u32)(kasel0) << 9 | (u32)(kcsel1) << 14 | (u32)(kasel1) << 19 | (u32)(id) << 24)

#define BP_ALPHA_COMPARE(ref0, ref1, comp0, comp1, op, id)                                                                                           \
    ((u32)(ref0) << 0 | (u32)(ref1) << 8 | (u32)(comp0) << 16 | (u32)(comp1) << 19 | (u32)(op) << 22 | (u32)(id) << 24)

#define BP_ZTEX_PARAMS_0(bias, id) ((u32)(bias) << 0 | (u32)(id) << 24)

#define BP_ZTEX_PARAMS_1(zfmt, op, id) ((u32)(zfmt) << 0 | (u32)(op) << 2 | (u32)(id) << 24)

#define BP_TEV_ORDER(map0, coord0, enable0, color0, map1, coord1, enable1, color1, id)                                                               \
    ((u32)(map0) << 0 | (u32)(coord0) << 3 | (u32)(enable0) << 6 | (u32)(color0) << 7 | (u32)(map1) << 12 | (u32)(coord1) << 15 |                    \
     (u32)(enable1) << 18 | (u32)(color1) << 19 | (u32)(id) << 24)

#endif  // GDTEV_H
