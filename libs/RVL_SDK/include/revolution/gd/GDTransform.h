#ifndef GDTRANSFORM_H
#define GDTRANSFORM_H

#define CP_MATINDEX_A 0x30
#define CP_MATINDEX_B 0x40

#define XF_MATINDEX_A 0x1018

#define MATIDX_REG_A_POSIDX_SHIFT 0
#define MATIDX_REG_A_TEX0IDX_SHIFT 6
#define MATIDX_REG_A_TEX1IDX_SHIFT 12
#define MATIDX_REG_A_TEX2IDX_SHIFT 18
#define MATIDX_REG_A_TEX3IDX_SHIFT 24
#define MATIDX_REG_A(posIdx, tex0Idx, tex1Idx, tex2Idx, tex3Idx)                                                                                     \
    ((((unsigned long)(posIdx)) << MATIDX_REG_A_POSIDX_SHIFT) | (((unsigned long)(tex0Idx)) << MATIDX_REG_A_TEX0IDX_SHIFT) |                         \
     (((unsigned long)(tex1Idx)) << MATIDX_REG_A_TEX1IDX_SHIFT) | (((unsigned long)(tex2Idx)) << MATIDX_REG_A_TEX2IDX_SHIFT) |                       \
     (((unsigned long)(tex3Idx)) << MATIDX_REG_A_TEX3IDX_SHIFT))

#define MATIDX_REG_B_TEX4IDX_SHIFT 0
#define MATIDX_REG_B_TEX5IDX_SHIFT 6
#define MATIDX_REG_B_TEX6IDX_SHIFT 12
#define MATIDX_REG_B_TEX7IDX_SHIFT 18
#define MATIDX_REG_B(tex4Idx, tex5Idx, tex6Idx, tex7Idx)                                                                                             \
    ((((unsigned long)(tex4Idx)) << MATIDX_REG_B_TEX4IDX_SHIFT) | (((unsigned long)(tex5Idx)) << MATIDX_REG_B_TEX5IDX_SHIFT) |                       \
     (((unsigned long)(tex6Idx)) << MATIDX_REG_B_TEX6IDX_SHIFT) | (((unsigned long)(tex7Idx)) << MATIDX_REG_B_TEX7IDX_SHIFT))

#endif  // GDTRANSFORM_H
