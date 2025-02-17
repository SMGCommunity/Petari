#ifndef GEN_REG_H
#define GEN_REG_H

#define GEN_MODE_RID_SIZE	8
#define GEN_MODE_RID_SHIFT	24
#define GEN_MODE_RID_MASK	0xff000000
#define GEN_MODE_GET_RID(gen_mode) \
	((((unsigned long)(gen_mode)) & GEN_MODE_RID_MASK) >> GEN_MODE_RID_SHIFT)

#define GEN_MODE_NTEX_SIZE	4
#define GEN_MODE_NTEX_SHIFT	0
#define GEN_MODE_NTEX_MASK	0x0000000f
#define GEN_MODE_GET_NTEX(gen_mode) \
	((((unsigned long)(gen_mode)) & GEN_MODE_NTEX_MASK) >> GEN_MODE_NTEX_SHIFT)

#define GEN_MODE_NTEX_SIZE	4
#define GEN_MODE_NTEX_SHIFT	0
#define GEN_MODE_NTEX_MASK	0x0000000f
#define GEN_MODE_GET_NTEX(gen_mode) \
	((((unsigned long)(gen_mode)) & GEN_MODE_NTEX_MASK) >> GEN_MODE_NTEX_SHIFT)

#define GEN_MODE_NCOL_SIZE	3
#define GEN_MODE_NCOL_SHIFT	4
#define GEN_MODE_NCOL_MASK	0x00000070
#define GEN_MODE_GET_NCOL(gen_mode) \
	((((unsigned long)(gen_mode)) & GEN_MODE_NCOL_MASK) >> GEN_MODE_NCOL_SHIFT)

#define GEN_MODE_PAD0_SIZE	1
#define GEN_MODE_PAD0_SHIFT	7
#define GEN_MODE_PAD0_MASK	0x00000080
#define GEN_MODE_GET_PAD0(gen_mode) \
	((((unsigned long)(gen_mode)) & GEN_MODE_PAD0_MASK) >> GEN_MODE_PAD0_SHIFT)

#define GEN_MODE_FLAT_EN_SIZE	1
#define GEN_MODE_FLAT_EN_SHIFT	8
#define GEN_MODE_FLAT_EN_MASK	0x00000100
#define GEN_MODE_GET_FLAT_EN(gen_mode) \
	((((unsigned long)(gen_mode)) & GEN_MODE_FLAT_EN_MASK) >> GEN_MODE_FLAT_EN_SHIFT)

#define GEN_MODE_MS_EN_SIZE	1
#define GEN_MODE_MS_EN_SHIFT	9
#define GEN_MODE_MS_EN_MASK	0x00000200
#define GEN_MODE_GET_MS_EN(gen_mode) \
	((((unsigned long)(gen_mode)) & GEN_MODE_MS_EN_MASK) >> GEN_MODE_MS_EN_SHIFT)

#define GEN_MODE_NTEV_SIZE	4
#define GEN_MODE_NTEV_SHIFT	10
#define GEN_MODE_NTEV_MASK	0x00003c00
#define GEN_MODE_GET_NTEV(gen_mode) \
	((((unsigned long)(gen_mode)) & GEN_MODE_NTEV_MASK) >> GEN_MODE_NTEV_SHIFT)

#define GEN_MODE_REJECT_EN_SIZE	2
#define GEN_MODE_REJECT_EN_SHIFT	14
#define GEN_MODE_REJECT_EN_MASK	0x0000c000
#define GEN_MODE_GET_REJECT_EN(gen_mode) \
	((((unsigned long)(gen_mode)) & GEN_MODE_REJECT_EN_MASK) >> GEN_MODE_REJECT_EN_SHIFT)

#define GEN_MODE_NBMP_SIZE	3
#define GEN_MODE_NBMP_SHIFT	16
#define GEN_MODE_NBMP_MASK	0x00070000
#define GEN_MODE_GET_NBMP(gen_mode) \
	((((unsigned long)(gen_mode)) & GEN_MODE_NBMP_MASK) >> GEN_MODE_NBMP_SHIFT)

#define GEN_MODE_ZFREEZE_SIZE	1
#define GEN_MODE_ZFREEZE_SHIFT	19
#define GEN_MODE_ZFREEZE_MASK	0x00080000
#define GEN_MODE_GET_ZFREEZE(gen_mode) \
	((((unsigned long)(gen_mode)) & GEN_MODE_ZFREEZE_MASK) >> GEN_MODE_ZFREEZE_SHIFT)

#define GEN_MODE_PAD1_SIZE	4
#define GEN_MODE_PAD1_SHIFT	20
#define GEN_MODE_PAD1_MASK	0x00f00000
#define GEN_MODE_GET_PAD1(gen_mode) \
	((((unsigned long)(gen_mode)) & GEN_MODE_PAD1_MASK) >> GEN_MODE_PAD1_SHIFT)

#define GEN_MODE_RID_SIZE	8
#define GEN_MODE_RID_SHIFT	24
#define GEN_MODE_RID_MASK	0xff000000
#define GEN_MODE_GET_RID(gen_mode) \
	((((unsigned long)(gen_mode)) & GEN_MODE_RID_MASK) >> GEN_MODE_RID_SHIFT)

#endif // GEN_REG_H
