#ifndef TEV_REG_H
#define TEV_REG_H

#define TEV_COLOR_ENV_RID_SIZE	8
#define TEV_COLOR_ENV_RID_SHIFT	24
#define TEV_COLOR_ENV_RID_MASK	0xff000000
#define TEV_COLOR_ENV_GET_RID(tev_color_env) \
	((((unsigned long)(tev_color_env)) & TEV_COLOR_ENV_RID_MASK) >> TEV_COLOR_ENV_RID_SHIFT)

#define TEV_ALPHA_ENV_RID_SIZE	8
#define TEV_ALPHA_ENV_RID_SHIFT	24
#define TEV_ALPHA_ENV_RID_MASK	0xff000000
#define TEV_ALPHA_ENV_GET_RID(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_RID_MASK) >> TEV_ALPHA_ENV_RID_SHIFT)

#define TEV_KSEL_RID_SIZE	8
#define TEV_KSEL_RID_SHIFT	24
#define TEV_KSEL_RID_MASK	0xff000000
#define TEV_KSEL_GET_RID(tev_ksel) \
    ((((unsigned long)(tev_ksel)) & TEV_KSEL_RID_MASK) >> TEV_KSEL_RID_SHIFT)

#define TEV_ALPHA_ENV_SELD_SIZE	3
#define TEV_ALPHA_ENV_SELD_SHIFT	4
#define TEV_ALPHA_ENV_SELD_MASK	0x00000070
#define TEV_ALPHA_ENV_GET_SELD(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_SELD_MASK) >> TEV_ALPHA_ENV_SELD_SHIFT)

#define TEV_ALPHA_ENV_SELC_SIZE	3
#define TEV_ALPHA_ENV_SELC_SHIFT	7
#define TEV_ALPHA_ENV_SELC_MASK	0x00000380
#define TEV_ALPHA_ENV_GET_SELC(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_SELC_MASK) >> TEV_ALPHA_ENV_SELC_SHIFT)

#define TEV_ALPHA_ENV_SELB_SIZE	3
#define TEV_ALPHA_ENV_SELB_SHIFT	10
#define TEV_ALPHA_ENV_SELB_MASK	0x00001c00
#define TEV_ALPHA_ENV_GET_SELB(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_SELB_MASK) >> TEV_ALPHA_ENV_SELB_SHIFT)

#define TEV_ALPHA_ENV_SELA_SIZE	3
#define TEV_ALPHA_ENV_SELA_SHIFT	13
#define TEV_ALPHA_ENV_SELA_MASK	0x0000e000
#define TEV_ALPHA_ENV_GET_SELA(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_SELA_MASK) >> TEV_ALPHA_ENV_SELA_SHIFT)

#define TEV_COLOR_ENV_SELD_SIZE	4
#define TEV_COLOR_ENV_SELD_SHIFT	0
#define TEV_COLOR_ENV_SELD_MASK	0x0000000f
#define TEV_COLOR_ENV_GET_SELD(tev_color_env) \
    ((((unsigned long)(tev_color_env)) & TEV_COLOR_ENV_SELD_MASK) >> TEV_COLOR_ENV_SELD_SHIFT)

#define TEV_COLOR_ENV_SELC_SIZE	4
#define TEV_COLOR_ENV_SELC_SHIFT	4
#define TEV_COLOR_ENV_SELC_MASK	0x000000f0
#define TEV_COLOR_ENV_GET_SELC(tev_color_env) \
    ((((unsigned long)(tev_color_env)) & TEV_COLOR_ENV_SELC_MASK) >> TEV_COLOR_ENV_SELC_SHIFT)

#define TEV_COLOR_ENV_SELB_SIZE	4
#define TEV_COLOR_ENV_SELB_SHIFT	8
#define TEV_COLOR_ENV_SELB_MASK	0x00000f00
#define TEV_COLOR_ENV_GET_SELB(tev_color_env) \
    ((((unsigned long)(tev_color_env)) & TEV_COLOR_ENV_SELB_MASK) >> TEV_COLOR_ENV_SELB_SHIFT)

#define TEV_COLOR_ENV_SELA_SIZE	4
#define TEV_COLOR_ENV_SELA_SHIFT	12
#define TEV_COLOR_ENV_SELA_MASK	0x0000f000
#define TEV_COLOR_ENV_GET_SELA(tev_color_env) \
    ((((unsigned long)(tev_color_env)) & TEV_COLOR_ENV_SELA_MASK) >> TEV_COLOR_ENV_SELA_SHIFT)

#define TEV_COLOR_ENV_BIAS_SIZE	2
#define TEV_COLOR_ENV_BIAS_SHIFT	16
#define TEV_COLOR_ENV_BIAS_MASK	0x00030000
#define TEV_COLOR_ENV_GET_BIAS(tev_color_env) \
    ((((unsigned long)(tev_color_env)) & TEV_COLOR_ENV_BIAS_MASK) >> TEV_COLOR_ENV_BIAS_SHIFT)

#define TEV_COLOR_ENV_SUB_SIZE	1
#define TEV_COLOR_ENV_SUB_SHIFT	18
#define TEV_COLOR_ENV_SUB_MASK	0x00040000
#define TEV_COLOR_ENV_GET_SUB(tev_color_env) \
    ((((unsigned long)(tev_color_env)) & TEV_COLOR_ENV_SUB_MASK) >> TEV_COLOR_ENV_SUB_SHIFT)

#define TEV_COLOR_ENV_CLAMP_SIZE	1
#define TEV_COLOR_ENV_CLAMP_SHIFT	19
#define TEV_COLOR_ENV_CLAMP_MASK	0x00080000
#define TEV_COLOR_ENV_GET_CLAMP(tev_color_env) \
    ((((unsigned long)(tev_color_env)) & TEV_COLOR_ENV_CLAMP_MASK) >> TEV_COLOR_ENV_CLAMP_SHIFT)

#define TEV_COLOR_ENV_SHIFT_SIZE	2
#define TEV_COLOR_ENV_SHIFT_SHIFT	20
#define TEV_COLOR_ENV_SHIFT_MASK	0x00300000
#define TEV_COLOR_ENV_GET_SHIFT(tev_color_env) \
    ((((unsigned long)(tev_color_env)) & TEV_COLOR_ENV_SHIFT_MASK) >> TEV_COLOR_ENV_SHIFT_SHIFT)

#define TEV_COLOR_ENV_DEST_SIZE	2
#define TEV_COLOR_ENV_DEST_SHIFT	22
#define TEV_COLOR_ENV_DEST_MASK	0x00c00000
#define TEV_COLOR_ENV_GET_DEST(tev_color_env) \
    ((((unsigned long)(tev_color_env)) & TEV_COLOR_ENV_DEST_MASK) >> TEV_COLOR_ENV_DEST_SHIFT)

#define TEV_COLOR_ENV_RID_SIZE	8
#define TEV_COLOR_ENV_RID_SHIFT	24
#define TEV_COLOR_ENV_RID_MASK	0xff000000
#define TEV_COLOR_ENV_GET_RID(tev_color_env) \
    ((((unsigned long)(tev_color_env)) & TEV_COLOR_ENV_RID_MASK) >> TEV_COLOR_ENV_RID_SHIFT)

#define TEV_ALPHA_ENV_MODE_SIZE	2
#define TEV_ALPHA_ENV_MODE_SHIFT	0
#define TEV_ALPHA_ENV_MODE_MASK	0x00000003
#define TEV_ALPHA_ENV_GET_MODE(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_MODE_MASK) >> TEV_ALPHA_ENV_MODE_SHIFT)

#define TEV_ALPHA_ENV_SWAP_SIZE	2
#define TEV_ALPHA_ENV_SWAP_SHIFT	2
#define TEV_ALPHA_ENV_SWAP_MASK	0x0000000c
#define TEV_ALPHA_ENV_GET_SWAP(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_SWAP_MASK) >> TEV_ALPHA_ENV_SWAP_SHIFT)

#define TEV_ALPHA_ENV_SELD_SIZE	3
#define TEV_ALPHA_ENV_SELD_SHIFT	4
#define TEV_ALPHA_ENV_SELD_MASK	0x00000070
#define TEV_ALPHA_ENV_GET_SELD(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_SELD_MASK) >> TEV_ALPHA_ENV_SELD_SHIFT)

#define TEV_ALPHA_ENV_SELC_SIZE	3
#define TEV_ALPHA_ENV_SELC_SHIFT	7
#define TEV_ALPHA_ENV_SELC_MASK	0x00000380
#define TEV_ALPHA_ENV_GET_SELC(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_SELC_MASK) >> TEV_ALPHA_ENV_SELC_SHIFT)

#define TEV_ALPHA_ENV_SELB_SIZE	3
#define TEV_ALPHA_ENV_SELB_SHIFT	10
#define TEV_ALPHA_ENV_SELB_MASK	0x00001c00
#define TEV_ALPHA_ENV_GET_SELB(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_SELB_MASK) >> TEV_ALPHA_ENV_SELB_SHIFT)

#define TEV_ALPHA_ENV_SELA_SIZE	3
#define TEV_ALPHA_ENV_SELA_SHIFT	13
#define TEV_ALPHA_ENV_SELA_MASK	0x0000e000
#define TEV_ALPHA_ENV_GET_SELA(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_SELA_MASK) >> TEV_ALPHA_ENV_SELA_SHIFT)

#define TEV_ALPHA_ENV_BIAS_SIZE	2
#define TEV_ALPHA_ENV_BIAS_SHIFT	16
#define TEV_ALPHA_ENV_BIAS_MASK	0x00030000
#define TEV_ALPHA_ENV_GET_BIAS(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_BIAS_MASK) >> TEV_ALPHA_ENV_BIAS_SHIFT)

#define TEV_ALPHA_ENV_SUB_SIZE	1
#define TEV_ALPHA_ENV_SUB_SHIFT	18
#define TEV_ALPHA_ENV_SUB_MASK	0x00040000
#define TEV_ALPHA_ENV_GET_SUB(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_SUB_MASK) >> TEV_ALPHA_ENV_SUB_SHIFT)

#define TEV_ALPHA_ENV_CLAMP_SIZE	1
#define TEV_ALPHA_ENV_CLAMP_SHIFT	19
#define TEV_ALPHA_ENV_CLAMP_MASK	0x00080000
#define TEV_ALPHA_ENV_GET_CLAMP(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_CLAMP_MASK) >> TEV_ALPHA_ENV_CLAMP_SHIFT)

#define TEV_ALPHA_ENV_SHIFT_SIZE	2
#define TEV_ALPHA_ENV_SHIFT_SHIFT	20
#define TEV_ALPHA_ENV_SHIFT_MASK	0x00300000
#define TEV_ALPHA_ENV_GET_SHIFT(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_SHIFT_MASK) >> TEV_ALPHA_ENV_SHIFT_SHIFT)

#define TEV_ALPHA_ENV_DEST_SIZE	2
#define TEV_ALPHA_ENV_DEST_SHIFT	22
#define TEV_ALPHA_ENV_DEST_MASK	0x00c00000
#define TEV_ALPHA_ENV_GET_DEST(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_DEST_MASK) >> TEV_ALPHA_ENV_DEST_SHIFT)

#define TEV_ALPHA_ENV_RID_SIZE	8
#define TEV_ALPHA_ENV_RID_SHIFT	24
#define TEV_ALPHA_ENV_RID_MASK	0xff000000
#define TEV_ALPHA_ENV_GET_RID(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_RID_MASK) >> TEV_ALPHA_ENV_RID_SHIFT)

#define TEV_REGISTERL_R_SIZE	11
#define TEV_REGISTERL_R_SHIFT	0
#define TEV_REGISTERL_R_MASK	0x000007ff
#define TEV_REGISTERL_GET_R(tev_registerl) \
    ((((unsigned long)(tev_registerl)) & TEV_REGISTERL_R_MASK) >> TEV_REGISTERL_R_SHIFT)

#define TEV_REGISTERL_PAD0_SIZE	1
#define TEV_REGISTERL_PAD0_SHIFT	11
#define TEV_REGISTERL_PAD0_MASK	0x00000800
#define TEV_REGISTERL_GET_PAD0(tev_registerl) \
    ((((unsigned long)(tev_registerl)) & TEV_REGISTERL_PAD0_MASK) >> TEV_REGISTERL_PAD0_SHIFT)

#define TEV_REGISTERL_A_SIZE	11
#define TEV_REGISTERL_A_SHIFT	12
#define TEV_REGISTERL_A_MASK	0x007ff000
#define TEV_REGISTERL_GET_A(tev_registerl) \
    ((((unsigned long)(tev_registerl)) & TEV_REGISTERL_A_MASK) >> TEV_REGISTERL_A_SHIFT)

#define TEV_REGISTERL_PAD1_SIZE	1
#define TEV_REGISTERL_PAD1_SHIFT	23
#define TEV_REGISTERL_PAD1_MASK	0x00800000
#define TEV_REGISTERL_GET_PAD1(tev_registerl) \
    ((((unsigned long)(tev_registerl)) & TEV_REGISTERL_PAD1_MASK) >> TEV_REGISTERL_PAD1_SHIFT)

#define TEV_REGISTERL_RID_SIZE	8
#define TEV_REGISTERL_RID_SHIFT	24
#define TEV_REGISTERL_RID_MASK	0xff000000
#define TEV_REGISTERL_GET_RID(tev_registerl) \
    ((((unsigned long)(tev_registerl)) & TEV_REGISTERL_RID_MASK) >> TEV_REGISTERL_RID_SHIFT)

#define TEV_REGISTERL_TOTAL_SIZE	32
#define TEV_REGISTERL(r, a, rid) \
    ((((unsigned long)(r)) << TEV_REGISTERL_R_SHIFT) | \
        (((unsigned long)(a)) << TEV_REGISTERL_A_SHIFT) | \
        (((unsigned long)(rid)) << TEV_REGISTERL_RID_SHIFT))

#define TEV_KREGISTERL_R_SIZE	8
#define TEV_KREGISTERL_R_SHIFT	0
#define TEV_KREGISTERL_R_MASK	0x000000ff
#define TEV_KREGISTERL_GET_R(tev_kregisterl) \
    ((((unsigned long)(tev_kregisterl)) & TEV_KREGISTERL_R_MASK) >> TEV_KREGISTERL_R_SHIFT)

#define TEV_KREGISTERL_PAD0_SIZE	4
#define TEV_KREGISTERL_PAD0_SHIFT	8
#define TEV_KREGISTERL_PAD0_MASK	0x00000f00
#define TEV_KREGISTERL_GET_PAD0(tev_kregisterl) \
    ((((unsigned long)(tev_kregisterl)) & TEV_KREGISTERL_PAD0_MASK) >> TEV_KREGISTERL_PAD0_SHIFT)

#define TEV_KREGISTERL_A_SIZE	8
#define TEV_KREGISTERL_A_SHIFT	12
#define TEV_KREGISTERL_A_MASK	0x000ff000
#define TEV_KREGISTERL_GET_A(tev_kregisterl) \
    ((((unsigned long)(tev_kregisterl)) & TEV_KREGISTERL_A_MASK) >> TEV_KREGISTERL_A_SHIFT)

#define TEV_KREGISTERL_PAD1_SIZE	4
#define TEV_KREGISTERL_PAD1_SHIFT	20
#define TEV_KREGISTERL_PAD1_MASK	0x00f00000
#define TEV_KREGISTERL_GET_PAD1(tev_kregisterl) \
    ((((unsigned long)(tev_kregisterl)) & TEV_KREGISTERL_PAD1_MASK) >> TEV_KREGISTERL_PAD1_SHIFT)

#define TEV_KREGISTERL_RID_SIZE	8
#define TEV_KREGISTERL_RID_SHIFT	24
#define TEV_KREGISTERL_RID_MASK	0xff000000
#define TEV_KREGISTERL_GET_RID(tev_kregisterl) \
    ((((unsigned long)(tev_kregisterl)) & TEV_KREGISTERL_RID_MASK) >> TEV_KREGISTERL_RID_SHIFT)

#define TEV_KREGISTERL_TOTAL_SIZE	32
#define TEV_KREGISTERL(r, a, rid) \
    ((((unsigned long)(r)) << TEV_KREGISTERL_R_SHIFT) | \
        (((unsigned long)(a)) << TEV_KREGISTERL_A_SHIFT) | \
        (((unsigned long)(rid)) << TEV_KREGISTERL_RID_SHIFT))

#define TEV_REGISTERH_B_SIZE	11
#define TEV_REGISTERH_B_SHIFT	0
#define TEV_REGISTERH_B_MASK	0x000007ff
#define TEV_REGISTERH_GET_B(tev_registerh) \
    ((((unsigned long)(tev_registerh)) & TEV_REGISTERH_B_MASK) >> TEV_REGISTERH_B_SHIFT)

#define TEV_REGISTERH_PAD0_SIZE	1
#define TEV_REGISTERH_PAD0_SHIFT	11
#define TEV_REGISTERH_PAD0_MASK	0x00000800
#define TEV_REGISTERH_GET_PAD0(tev_registerh) \
    ((((unsigned long)(tev_registerh)) & TEV_REGISTERH_PAD0_MASK) >> TEV_REGISTERH_PAD0_SHIFT)

#define TEV_REGISTERH_G_SIZE	11
#define TEV_REGISTERH_G_SHIFT	12
#define TEV_REGISTERH_G_MASK	0x007ff000
#define TEV_REGISTERH_GET_G(tev_registerh) \
    ((((unsigned long)(tev_registerh)) & TEV_REGISTERH_G_MASK) >> TEV_REGISTERH_G_SHIFT)

#define TEV_REGISTERH_PAD1_SIZE	1
#define TEV_REGISTERH_PAD1_SHIFT	23
#define TEV_REGISTERH_PAD1_MASK	0x00800000
#define TEV_REGISTERH_GET_PAD1(tev_registerh) \
    ((((unsigned long)(tev_registerh)) & TEV_REGISTERH_PAD1_MASK) >> TEV_REGISTERH_PAD1_SHIFT)

#define TEV_REGISTERH_RID_SIZE	8
#define TEV_REGISTERH_RID_SHIFT	24
#define TEV_REGISTERH_RID_MASK	0xff000000
#define TEV_REGISTERH_GET_RID(tev_registerh) \
    ((((unsigned long)(tev_registerh)) & TEV_REGISTERH_RID_MASK) >> TEV_REGISTERH_RID_SHIFT)

#define TEV_REGISTERH_TOTAL_SIZE	32
#define TEV_REGISTERH(b, g, rid) \
    ((((unsigned long)(b)) << TEV_REGISTERH_B_SHIFT) | \
        (((unsigned long)(g)) << TEV_REGISTERH_G_SHIFT) | \
        (((unsigned long)(rid)) << TEV_REGISTERH_RID_SHIFT))

#define TEV_KREGISTERH_B_SIZE	8
#define TEV_KREGISTERH_B_SHIFT	0
#define TEV_KREGISTERH_B_MASK	0x000000ff
#define TEV_KREGISTERH_GET_B(tev_kregisterh) \
    ((((unsigned long)(tev_kregisterh)) & TEV_KREGISTERH_B_MASK) >> TEV_KREGISTERH_B_SHIFT)

#define TEV_KREGISTERH_PAD0_SIZE	4
#define TEV_KREGISTERH_PAD0_SHIFT	8
#define TEV_KREGISTERH_PAD0_MASK	0x00000f00
#define TEV_KREGISTERH_GET_PAD0(tev_kregisterh) \
    ((((unsigned long)(tev_kregisterh)) & TEV_KREGISTERH_PAD0_MASK) >> TEV_KREGISTERH_PAD0_SHIFT)

#define TEV_KREGISTERH_G_SIZE	8
#define TEV_KREGISTERH_G_SHIFT	12
#define TEV_KREGISTERH_G_MASK	0x000ff000
#define TEV_KREGISTERH_GET_G(tev_kregisterh) \
    ((((unsigned long)(tev_kregisterh)) & TEV_KREGISTERH_G_MASK) >> TEV_KREGISTERH_G_SHIFT)

#define TEV_KREGISTERH_PAD1_SIZE	4
#define TEV_KREGISTERH_PAD1_SHIFT	20
#define TEV_KREGISTERH_PAD1_MASK	0x00f00000
#define TEV_KREGISTERH_GET_PAD1(tev_kregisterh) \
    ((((unsigned long)(tev_kregisterh)) & TEV_KREGISTERH_PAD1_MASK) >> TEV_KREGISTERH_PAD1_SHIFT)

#define TEV_KREGISTERH_RID_SIZE	8
#define TEV_KREGISTERH_RID_SHIFT	24
#define TEV_KREGISTERH_RID_MASK	0xff000000
#define TEV_KREGISTERH_GET_RID(tev_kregisterh) \
    ((((unsigned long)(tev_kregisterh)) & TEV_KREGISTERH_RID_MASK) >> TEV_KREGISTERH_RID_SHIFT)

#define TEV_KREGISTERH_TOTAL_SIZE	32
#define TEV_KREGISTERH(b, g, rid) \
    ((((unsigned long)(b)) << TEV_KREGISTERH_B_SHIFT) | \
        (((unsigned long)(g)) << TEV_KREGISTERH_G_SHIFT) | \
        (((unsigned long)(rid)) << TEV_KREGISTERH_RID_SHIFT))

#define TEV_KSEL_XRB_SIZE	2
#define TEV_KSEL_XRB_SHIFT	0
#define TEV_KSEL_XRB_MASK	0x00000003
#define TEV_KSEL_GET_XRB(tev_ksel) \
    ((((unsigned long)(tev_ksel)) & TEV_KSEL_XRB_MASK) >> TEV_KSEL_XRB_SHIFT)

#define TEV_KSEL_XGA_SIZE	2
#define TEV_KSEL_XGA_SHIFT	2
#define TEV_KSEL_XGA_MASK	0x0000000c
#define TEV_KSEL_GET_XGA(tev_ksel) \
    ((((unsigned long)(tev_ksel)) & TEV_KSEL_XGA_MASK) >> TEV_KSEL_XGA_SHIFT)

#define TEV_KSEL_KCSEL0_SIZE	5
#define TEV_KSEL_KCSEL0_SHIFT	4
#define TEV_KSEL_KCSEL0_MASK	0x000001f0
#define TEV_KSEL_GET_KCSEL0(tev_ksel) \
    ((((unsigned long)(tev_ksel)) & TEV_KSEL_KCSEL0_MASK) >> TEV_KSEL_KCSEL0_SHIFT)

#define TEV_KSEL_KASEL0_SIZE	5
#define TEV_KSEL_KASEL0_SHIFT	9
#define TEV_KSEL_KASEL0_MASK	0x00003e00
#define TEV_KSEL_GET_KASEL0(tev_ksel) \
    ((((unsigned long)(tev_ksel)) & TEV_KSEL_KASEL0_MASK) >> TEV_KSEL_KASEL0_SHIFT)

#define TEV_KSEL_KCSEL1_SIZE	5
#define TEV_KSEL_KCSEL1_SHIFT	14
#define TEV_KSEL_KCSEL1_MASK	0x0007c000
#define TEV_KSEL_GET_KCSEL1(tev_ksel) \
    ((((unsigned long)(tev_ksel)) & TEV_KSEL_KCSEL1_MASK) >> TEV_KSEL_KCSEL1_SHIFT)

#define TEV_KSEL_KASEL1_SIZE	5
#define TEV_KSEL_KASEL1_SHIFT	19
#define TEV_KSEL_KASEL1_MASK	0x00f80000
#define TEV_KSEL_GET_KASEL1(tev_ksel) \
    ((((unsigned long)(tev_ksel)) & TEV_KSEL_KASEL1_MASK) >> TEV_KSEL_KASEL1_SHIFT)

#define TEV_KSEL_RID_SIZE	8
#define TEV_KSEL_RID_SHIFT	24
#define TEV_KSEL_RID_MASK	0xff000000
#define TEV_KSEL_GET_RID(tev_ksel) \
    ((((unsigned long)(tev_ksel)) & TEV_KSEL_RID_MASK) >> TEV_KSEL_RID_SHIFT)

#define TEV_ALPHAFUNC_A0_SIZE	8
#define TEV_ALPHAFUNC_A0_SHIFT	0
#define TEV_ALPHAFUNC_A0_MASK	0x000000ff
#define TEV_ALPHAFUNC_GET_A0(tev_alphafunc) \
    ((((unsigned long)(tev_alphafunc)) & TEV_ALPHAFUNC_A0_MASK) >> TEV_ALPHAFUNC_A0_SHIFT)

#define TEV_ALPHAFUNC_A1_SIZE	8
#define TEV_ALPHAFUNC_A1_SHIFT	8
#define TEV_ALPHAFUNC_A1_MASK	0x0000ff00
#define TEV_ALPHAFUNC_GET_A1(tev_alphafunc) \
    ((((unsigned long)(tev_alphafunc)) & TEV_ALPHAFUNC_A1_MASK) >> TEV_ALPHAFUNC_A1_SHIFT)

#define TEV_ALPHAFUNC_OP0_SIZE	3
#define TEV_ALPHAFUNC_OP0_SHIFT	16
#define TEV_ALPHAFUNC_OP0_MASK	0x00070000
#define TEV_ALPHAFUNC_GET_OP0(tev_alphafunc) \
    ((((unsigned long)(tev_alphafunc)) & TEV_ALPHAFUNC_OP0_MASK) >> TEV_ALPHAFUNC_OP0_SHIFT)

#define TEV_ALPHAFUNC_OP1_SIZE	3
#define TEV_ALPHAFUNC_OP1_SHIFT	19
#define TEV_ALPHAFUNC_OP1_MASK	0x00380000
#define TEV_ALPHAFUNC_GET_OP1(tev_alphafunc) \
    ((((unsigned long)(tev_alphafunc)) & TEV_ALPHAFUNC_OP1_MASK) >> TEV_ALPHAFUNC_OP1_SHIFT)

#define TEV_ALPHAFUNC_LOGIC_SIZE	2
#define TEV_ALPHAFUNC_LOGIC_SHIFT	22
#define TEV_ALPHAFUNC_LOGIC_MASK	0x00c00000
#define TEV_ALPHAFUNC_GET_LOGIC(tev_alphafunc) \
    ((((unsigned long)(tev_alphafunc)) & TEV_ALPHAFUNC_LOGIC_MASK) >> TEV_ALPHAFUNC_LOGIC_SHIFT)

#define TEV_ALPHAFUNC_RID_SIZE	8
#define TEV_ALPHAFUNC_RID_SHIFT	24
#define TEV_ALPHAFUNC_RID_MASK	0xff000000
#define TEV_ALPHAFUNC_GET_RID(tev_alphafunc) \
    ((((unsigned long)(tev_alphafunc)) & TEV_ALPHAFUNC_RID_MASK) >> TEV_ALPHAFUNC_RID_SHIFT)

#define TEV_Z_ENV_0_ZOFF_SIZE	24
#define TEV_Z_ENV_0_ZOFF_SHIFT	0
#define TEV_Z_ENV_0_ZOFF_MASK	0x00ffffff
#define TEV_Z_ENV_0_GET_ZOFF(tev_z_env_0) \
    ((((unsigned long)(tev_z_env_0)) & TEV_Z_ENV_0_ZOFF_MASK) >> TEV_Z_ENV_0_ZOFF_SHIFT)

#define TEV_Z_ENV_0_RID_SIZE	8
#define TEV_Z_ENV_0_RID_SHIFT	24
#define TEV_Z_ENV_0_RID_MASK	0xff000000
#define TEV_Z_ENV_0_GET_RID(tev_z_env_0) \
    ((((unsigned long)(tev_z_env_0)) & TEV_Z_ENV_0_RID_MASK) >> TEV_Z_ENV_0_RID_SHIFT)

#define TEV_Z_ENV_1_TYPE_SIZE	2
#define TEV_Z_ENV_1_TYPE_SHIFT	0
#define TEV_Z_ENV_1_TYPE_MASK	0x00000003
#define TEV_Z_ENV_1_GET_TYPE(tev_z_env_1) \
    ((((unsigned long)(tev_z_env_1)) & TEV_Z_ENV_1_TYPE_MASK) >> TEV_Z_ENV_1_TYPE_SHIFT)

#define TEV_Z_ENV_1_OP_SIZE	2
#define TEV_Z_ENV_1_OP_SHIFT	2
#define TEV_Z_ENV_1_OP_MASK	0x0000000c
#define TEV_Z_ENV_1_GET_OP(tev_z_env_1) \
    ((((unsigned long)(tev_z_env_1)) & TEV_Z_ENV_1_OP_MASK) >> TEV_Z_ENV_1_OP_SHIFT)

#define TEV_Z_ENV_1_PAD0_SIZE	20
#define TEV_Z_ENV_1_PAD0_SHIFT	4
#define TEV_Z_ENV_1_PAD0_MASK	0x00fffff0
#define TEV_Z_ENV_1_GET_PAD0(tev_z_env_1) \
    ((((unsigned long)(tev_z_env_1)) & TEV_Z_ENV_1_PAD0_MASK) >> TEV_Z_ENV_1_PAD0_SHIFT)

#define TEV_Z_ENV_1_RID_SIZE	8
#define TEV_Z_ENV_1_RID_SHIFT	24
#define TEV_Z_ENV_1_RID_MASK	0xff000000
#define TEV_Z_ENV_1_GET_RID(tev_z_env_1) \
    ((((unsigned long)(tev_z_env_1)) & TEV_Z_ENV_1_RID_MASK) >> TEV_Z_ENV_1_RID_SHIFT)

typedef struct {
    unsigned long rid : 8;
    unsigned long dest : 2;
    unsigned long shift : 2;
    unsigned long clamp : 1;
    unsigned long sub : 1;
    unsigned long bias : 2;
    unsigned long sela : 4;
    unsigned long selb : 4;
    unsigned long selc : 4;
    unsigned long seld : 4;
} tev_color_env_t;

typedef struct {
    unsigned long rid : 8;
    unsigned long dest : 2;
    unsigned long shift : 2;
    unsigned long clamp : 1;
    unsigned long sub : 1;
    unsigned long bias : 2;
    unsigned long sela : 3;
    unsigned long selb : 3;
    unsigned long selc : 3;
    unsigned long seld : 3;
    unsigned long swap : 2;
    unsigned long mode : 2;
} tev_alpha_env_t;

#endif // TEV_REG_H
