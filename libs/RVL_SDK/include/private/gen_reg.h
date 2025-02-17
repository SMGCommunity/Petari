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

#endif // GEN_REG_H
