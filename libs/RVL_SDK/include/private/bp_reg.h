#ifndef BP_REG_H
#define BP_REG_H

#define BP_IMASK_RID_SIZE	8
#define BP_IMASK_RID_SHIFT	24
#define BP_IMASK_RID_MASK	0xff000000
#define BP_IMASK_GET_RID(bp_imask) \
	((((unsigned long)(bp_imask)) & BP_IMASK_RID_MASK) >> BP_IMASK_RID_SHIFT)

#endif // BP_REG_H
