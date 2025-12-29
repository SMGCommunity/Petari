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

#endif  // GDINDIRECT_H
