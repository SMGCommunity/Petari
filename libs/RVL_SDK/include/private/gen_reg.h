#ifndef GEN_REG_H
#define GEN_REG_H

#define GEN_MODE_RID_SIZE 8
#define GEN_MODE_RID_SHIFT 24
#define GEN_MODE_RID_MASK 0xff000000
#define GEN_MODE_GET_RID(gen_mode) ((((unsigned long)(gen_mode)) & GEN_MODE_RID_MASK) >> GEN_MODE_RID_SHIFT)

#define GEN_MODE_NTEX_SIZE 4
#define GEN_MODE_NTEX_SHIFT 0
#define GEN_MODE_NTEX_MASK 0x0000000f
#define GEN_MODE_GET_NTEX(gen_mode) ((((unsigned long)(gen_mode)) & GEN_MODE_NTEX_MASK) >> GEN_MODE_NTEX_SHIFT)

#define GEN_MODE_NTEX_SIZE 4
#define GEN_MODE_NTEX_SHIFT 0
#define GEN_MODE_NTEX_MASK 0x0000000f
#define GEN_MODE_GET_NTEX(gen_mode) ((((unsigned long)(gen_mode)) & GEN_MODE_NTEX_MASK) >> GEN_MODE_NTEX_SHIFT)

#define GEN_MODE_NCOL_SIZE 3
#define GEN_MODE_NCOL_SHIFT 4
#define GEN_MODE_NCOL_MASK 0x00000070
#define GEN_MODE_GET_NCOL(gen_mode) ((((unsigned long)(gen_mode)) & GEN_MODE_NCOL_MASK) >> GEN_MODE_NCOL_SHIFT)

#define GEN_MODE_PAD0_SIZE 1
#define GEN_MODE_PAD0_SHIFT 7
#define GEN_MODE_PAD0_MASK 0x00000080
#define GEN_MODE_GET_PAD0(gen_mode) ((((unsigned long)(gen_mode)) & GEN_MODE_PAD0_MASK) >> GEN_MODE_PAD0_SHIFT)

#define GEN_MODE_FLAT_EN_SIZE 1
#define GEN_MODE_FLAT_EN_SHIFT 8
#define GEN_MODE_FLAT_EN_MASK 0x00000100
#define GEN_MODE_GET_FLAT_EN(gen_mode) ((((unsigned long)(gen_mode)) & GEN_MODE_FLAT_EN_MASK) >> GEN_MODE_FLAT_EN_SHIFT)

#define GEN_MODE_MS_EN_SIZE 1
#define GEN_MODE_MS_EN_SHIFT 9
#define GEN_MODE_MS_EN_MASK 0x00000200
#define GEN_MODE_GET_MS_EN(gen_mode) ((((unsigned long)(gen_mode)) & GEN_MODE_MS_EN_MASK) >> GEN_MODE_MS_EN_SHIFT)

#define GEN_MODE_NTEV_SIZE 4
#define GEN_MODE_NTEV_SHIFT 10
#define GEN_MODE_NTEV_MASK 0x00003c00
#define GEN_MODE_GET_NTEV(gen_mode) ((((unsigned long)(gen_mode)) & GEN_MODE_NTEV_MASK) >> GEN_MODE_NTEV_SHIFT)

#define GEN_MODE_REJECT_EN_SIZE 2
#define GEN_MODE_REJECT_EN_SHIFT 14
#define GEN_MODE_REJECT_EN_MASK 0x0000c000
#define GEN_MODE_GET_REJECT_EN(gen_mode) ((((unsigned long)(gen_mode)) & GEN_MODE_REJECT_EN_MASK) >> GEN_MODE_REJECT_EN_SHIFT)

#define GEN_MODE_NBMP_SIZE 3
#define GEN_MODE_NBMP_SHIFT 16
#define GEN_MODE_NBMP_MASK 0x00070000
#define GEN_MODE_GET_NBMP(gen_mode) ((((unsigned long)(gen_mode)) & GEN_MODE_NBMP_MASK) >> GEN_MODE_NBMP_SHIFT)

#define GEN_MODE_ZFREEZE_SIZE 1
#define GEN_MODE_ZFREEZE_SHIFT 19
#define GEN_MODE_ZFREEZE_MASK 0x00080000
#define GEN_MODE_GET_ZFREEZE(gen_mode) ((((unsigned long)(gen_mode)) & GEN_MODE_ZFREEZE_MASK) >> GEN_MODE_ZFREEZE_SHIFT)

#define GEN_MODE_PAD1_SIZE 4
#define GEN_MODE_PAD1_SHIFT 20
#define GEN_MODE_PAD1_MASK 0x00f00000
#define GEN_MODE_GET_PAD1(gen_mode) ((((unsigned long)(gen_mode)) & GEN_MODE_PAD1_MASK) >> GEN_MODE_PAD1_SHIFT)

#define GEN_MODE_RID_SIZE 8
#define GEN_MODE_RID_SHIFT 24
#define GEN_MODE_RID_MASK 0xff000000
#define GEN_MODE_GET_RID(gen_mode) ((((unsigned long)(gen_mode)) & GEN_MODE_RID_MASK) >> GEN_MODE_RID_SHIFT)

#define GEN_MSLOC_XS0_SIZE 4
#define GEN_MSLOC_XS0_SHIFT 0
#define GEN_MSLOC_XS0_MASK 0x0000000f
#define GEN_MSLOC_GET_XS0(gen_msloc) ((((unsigned long)(gen_msloc)) & GEN_MSLOC_XS0_MASK) >> GEN_MSLOC_XS0_SHIFT)
#define GEN_MSLOC_SET_XS0(gen_msloc, xs0)                                                                                                            \
    { gen_msloc = (((unsigned long)(gen_msloc)) & ~GEN_MSLOC_XS0_MASK) | (((unsigned long)(xs0)) << GEN_MSLOC_XS0_SHIFT); }

#define GEN_MSLOC_YS0_SIZE 4
#define GEN_MSLOC_YS0_SHIFT 4
#define GEN_MSLOC_YS0_MASK 0x000000f0
#define GEN_MSLOC_GET_YS0(gen_msloc) ((((unsigned long)(gen_msloc)) & GEN_MSLOC_YS0_MASK) >> GEN_MSLOC_YS0_SHIFT)
#define GEN_MSLOC_SET_YS0(gen_msloc, ys0)                                                                                                            \
    { gen_msloc = (((unsigned long)(gen_msloc)) & ~GEN_MSLOC_YS0_MASK) | (((unsigned long)(ys0)) << GEN_MSLOC_YS0_SHIFT); }

#define GEN_MSLOC_XS1_SIZE 4
#define GEN_MSLOC_XS1_SHIFT 8
#define GEN_MSLOC_XS1_MASK 0x00000f00
#define GEN_MSLOC_GET_XS1(gen_msloc) ((((unsigned long)(gen_msloc)) & GEN_MSLOC_XS1_MASK) >> GEN_MSLOC_XS1_SHIFT)
#define GEN_MSLOC_SET_XS1(gen_msloc, xs1)                                                                                                            \
    { gen_msloc = (((unsigned long)(gen_msloc)) & ~GEN_MSLOC_XS1_MASK) | (((unsigned long)(xs1)) << GEN_MSLOC_XS1_SHIFT); }

#define GEN_MSLOC_YS1_SIZE 4
#define GEN_MSLOC_YS1_SHIFT 12
#define GEN_MSLOC_YS1_MASK 0x0000f000
#define GEN_MSLOC_GET_YS1(gen_msloc) ((((unsigned long)(gen_msloc)) & GEN_MSLOC_YS1_MASK) >> GEN_MSLOC_YS1_SHIFT)
#define GEN_MSLOC_SET_YS1(gen_msloc, ys1)                                                                                                            \
    { gen_msloc = (((unsigned long)(gen_msloc)) & ~GEN_MSLOC_YS1_MASK) | (((unsigned long)(ys1)) << GEN_MSLOC_YS1_SHIFT); }
#define GEN_MSLOC_XS2_SIZE 4
#define GEN_MSLOC_XS2_SHIFT 16
#define GEN_MSLOC_XS2_MASK 0x000f0000
#define GEN_MSLOC_GET_XS2(gen_msloc) ((((unsigned long)(gen_msloc)) & GEN_MSLOC_XS2_MASK) >> GEN_MSLOC_XS2_SHIFT)
#define GEN_MSLOC_SET_XS2(gen_msloc, xs2)                                                                                                            \
    { gen_msloc = (((unsigned long)(gen_msloc)) & ~GEN_MSLOC_XS2_MASK) | (((unsigned long)(xs2)) << GEN_MSLOC_XS2_SHIFT); }
#define GEN_MSLOC_YS2_SIZE 4
#define GEN_MSLOC_YS2_SHIFT 20
#define GEN_MSLOC_YS2_MASK 0x00f00000
#define GEN_MSLOC_GET_YS2(gen_msloc) ((((unsigned long)(gen_msloc)) & GEN_MSLOC_YS2_MASK) >> GEN_MSLOC_YS2_SHIFT)
#define GEN_MSLOC_SET_YS2(gen_msloc, ys2)                                                                                                            \
    { gen_msloc = (((unsigned long)(gen_msloc)) & ~GEN_MSLOC_YS2_MASK) | (((unsigned long)(ys2)) << GEN_MSLOC_YS2_SHIFT); }

#define GEN_MSLOC_RID_SIZE 8
#define GEN_MSLOC_RID_SHIFT 24
#define GEN_MSLOC_RID_MASK 0xff000000
#define GEN_MSLOC_GET_RID(gen_msloc) ((((unsigned long)(gen_msloc)) & GEN_MSLOC_RID_MASK) >> GEN_MSLOC_RID_SHIFT)
#define GEN_MSLOC_SET_RID(gen_msloc, rid)                                                                                                            \
    { gen_msloc = (((unsigned long)(gen_msloc)) & ~GEN_MSLOC_RID_MASK) | (((unsigned long)(rid)) << GEN_MSLOC_RID_SHIFT); }

#define GEN_MSLOC_TOTAL_SIZE 32
#define GEN_MSLOC(xs0, ys0, xs1, ys1, xs2, ys2, rid)                                                                                                 \
    ((((unsigned long)(xs0)) << GEN_MSLOC_XS0_SHIFT) | (((unsigned long)(ys0)) << GEN_MSLOC_YS0_SHIFT) |                                             \
     (((unsigned long)(xs1)) << GEN_MSLOC_XS1_SHIFT) | (((unsigned long)(ys1)) << GEN_MSLOC_YS1_SHIFT) |                                             \
     (((unsigned long)(xs2)) << GEN_MSLOC_XS2_SHIFT) | (((unsigned long)(ys2)) << GEN_MSLOC_YS2_SHIFT) |                                             \
     (((unsigned long)(rid)) << GEN_MSLOC_RID_SHIFT))

#endif  // GEN_REG_H
