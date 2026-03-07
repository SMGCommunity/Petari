#ifndef GDINDIRECT_H
#define GDINDIRECT_H

#define IND_CMD0_ID 0x00000010

#define IND_CMD_BT_SHIFT 0
#define IND_CMD_FMT_SHIFT 2
#define IND_CMD_BIAS_SHIFT 4
#define IND_CMD_BS_SHIFT 7
#define IND_CMD_M_SHIFT 9
#define IND_CMD_SW_SHIFT 13
#define IND_CMD_TW_SHIFT 16
#define IND_CMD_LB_SHIFT 19
#define IND_CMD_FB_SHIFT 20
#define IND_CMD_PAD0_SHIFT 21
#define IND_CMD_RID_SHIFT 24
#define IND_CMD(bt, fmt, bias, bs, m, sw, tw, lb, fb, rid)                                                                                           \
    ((((unsigned long)(bt)) << IND_CMD_BT_SHIFT) | (((unsigned long)(fmt)) << IND_CMD_FMT_SHIFT) | (((unsigned long)(bias)) << IND_CMD_BIAS_SHIFT) | \
     (((unsigned long)(bs)) << IND_CMD_BS_SHIFT) | (((unsigned long)(m)) << IND_CMD_M_SHIFT) | (((unsigned long)(sw)) << IND_CMD_SW_SHIFT) |         \
     (((unsigned long)(tw)) << IND_CMD_TW_SHIFT) | (((unsigned long)(lb)) << IND_CMD_LB_SHIFT) | (((unsigned long)(fb)) << IND_CMD_FB_SHIFT) |       \
     (((unsigned long)(rid)) << IND_CMD_RID_SHIFT))

#define BP_IND_MTX(m0, m1, scale_exp, id) ((u32)(m0) << 0 | (u32)(m1) << 11 | (u32)(scale_exp) << 22 | (u32)(id) << 24)

#define BP_IND_TEXCOORD_SCALE(scaleS0, scaleT0, scaleS1, scaleT1, id)                                                                                \
    ((u32)(scaleS0) << 0 | (u32)(scaleT0) << 4 | (u32)(scaleS1) << 8 | (u32)(scaleT1) << 12 | (u32)(id) << 24)

#define BP_IND_TEX_ORDER(map0, coord0, map1, coord1, map2, coord2, map3, coord3, id)                                                                 \
    ((u32)(map0) << 0 | (u32)(coord0) << 3 | (u32)(map1) << 6 | (u32)(coord1) << 9 | (u32)(map2) << 12 | (u32)(coord2) << 15 | (u32)(map3) << 18 |   \
     (u32)(coord3) << 21 | (u32)(id) << 24)

#define BP_IND_MASK(mask, id) ((u32)(mask) << 0 | (u32)(id) << 24)

#endif  // GDINDIRECT_H
