#ifndef TX_REG_H
#define TX_REG_H

#define TX_REFRESH_INTERVAL_SIZE        10
#define TX_REFRESH_INTERVAL_SHIFT       0
#define TX_REFRESH_INTERVAL_MASK        0x000003ff
#define TX_REFRESH_GET_INTERVAL(tx_refresh) \
        ((((unsigned long)(tx_refresh)) & TX_REFRESH_INTERVAL_MASK) >> TX_REFRESH_INTERVAL_SHIFT)

#define TX_REFRESH_ENABLE_SIZE  1
#define TX_REFRESH_ENABLE_SHIFT 10
#define TX_REFRESH_ENABLE_MASK  0x00000400
#define TX_REFRESH_GET_ENABLE(tx_refresh) \
        ((((unsigned long)(tx_refresh)) & TX_REFRESH_ENABLE_MASK) >> TX_REFRESH_ENABLE_SHIFT)

#define TX_REFRESH_PAD0_SIZE    13
#define TX_REFRESH_PAD0_SHIFT   11
#define TX_REFRESH_PAD0_MASK    0x00fff800
#define TX_REFRESH_GET_PAD0(tx_refresh) \
        ((((unsigned long)(tx_refresh)) & TX_REFRESH_PAD0_MASK) >> TX_REFRESH_PAD0_SHIFT)
#define TX_REFRESH_SET_PAD0(tx_refresh, pad0) { \
        FDL_ASSERT(!((pad0) & ~((1 << TX_REFRESH_PAD0_SIZE)-1))); \
        tx_refresh = (((unsigned long)(tx_refresh)) & ~TX_REFRESH_PAD0_MASK) | (((unsigned long)(pad0)) << TX_REFRESH_PAD0_SHIFT);\
}
#define TX_REFRESH_RID_SIZE     8
#define TX_REFRESH_RID_SHIFT    24
#define TX_REFRESH_RID_MASK     0xff000000
#define TX_REFRESH_GET_RID(tx_refresh) \
        ((((unsigned long)(tx_refresh)) & TX_REFRESH_RID_MASK) >> TX_REFRESH_RID_SHIFT)
#define TX_REFRESH_SET_RID(tx_refresh, rid) { \
        FDL_ASSERT(!((rid) & ~((1 << TX_REFRESH_RID_SIZE)-1))); \
        tx_refresh = (((unsigned long)(tx_refresh)) & ~TX_REFRESH_RID_MASK) | (((unsigned long)(rid)) << TX_REFRESH_RID_SHIFT);\
}
#define TX_REFRESH_TOTAL_SIZE   32
#define TX_REFRESH(interval, enable, rid) \
        ((((unsigned long)(interval)) << TX_REFRESH_INTERVAL_SHIFT) | \
         (((unsigned long)(enable)) << TX_REFRESH_ENABLE_SHIFT) | \
         (((unsigned long)(rid)) << TX_REFRESH_RID_SHIFT))

#define TX_PERFMODE_SEL3_SIZE   5
#define TX_PERFMODE_SEL3_SHIFT  0
#define TX_PERFMODE_SEL3_MASK   0x0000001f
#define TX_PERFMODE_GET_SEL3(tx_perfmode) \
        ((((unsigned long)(tx_perfmode)) & TX_PERFMODE_SEL3_MASK) >> TX_PERFMODE_SEL3_SHIFT)

#define TX_PERFMODE_SEL2_SIZE   5
#define TX_PERFMODE_SEL2_SHIFT  5
#define TX_PERFMODE_SEL2_MASK   0x000003e0
#define TX_PERFMODE_GET_SEL2(tx_perfmode) \
        ((((unsigned long)(tx_perfmode)) & TX_PERFMODE_SEL2_MASK) >> TX_PERFMODE_SEL2_SHIFT)

#define TX_PERFMODE_SEL1_SIZE   5
#define TX_PERFMODE_SEL1_SHIFT  10
#define TX_PERFMODE_SEL1_MASK   0x00007c00
#define TX_PERFMODE_GET_SEL1(tx_perfmode) \
        ((((unsigned long)(tx_perfmode)) & TX_PERFMODE_SEL1_MASK) >> TX_PERFMODE_SEL1_SHIFT)

#define TX_PERFMODE_SEL0_SIZE   5
#define TX_PERFMODE_SEL0_SHIFT  15
#define TX_PERFMODE_SEL0_MASK   0x000f8000
#define TX_PERFMODE_GET_SEL0(tx_perfmode) \
        ((((unsigned long)(tx_perfmode)) & TX_PERFMODE_SEL0_MASK) >> TX_PERFMODE_SEL0_SHIFT)

#define TX_PERFMODE_PAD0_SIZE   4
#define TX_PERFMODE_PAD0_SHIFT  20
#define TX_PERFMODE_PAD0_MASK   0x00f00000
#define TX_PERFMODE_GET_PAD0(tx_perfmode) \
        ((((unsigned long)(tx_perfmode)) & TX_PERFMODE_PAD0_MASK) >> TX_PERFMODE_PAD0_SHIFT)

#define TX_PERFMODE_RID_SIZE    8
#define TX_PERFMODE_RID_SHIFT   24
#define TX_PERFMODE_RID_MASK    0xff000000
#define TX_PERFMODE_GET_RID(tx_perfmode) \
        ((((unsigned long)(tx_perfmode)) & TX_PERFMODE_RID_MASK) >> TX_PERFMODE_RID_SHIFT)

#define TX_PERFMODE_TOTAL_SIZE  32
#define TX_PERFMODE(sel3, sel2, sel1, sel0, rid) \
        ((((unsigned long)(sel3)) << TX_PERFMODE_SEL3_SHIFT) | \
        (((unsigned long)(sel2)) << TX_PERFMODE_SEL2_SHIFT) | \
        (((unsigned long)(sel1)) << TX_PERFMODE_SEL1_SHIFT) | \
        (((unsigned long)(sel0)) << TX_PERFMODE_SEL0_SHIFT) | \
        (((unsigned long)(rid)) << TX_PERFMODE_RID_SHIFT))

#endif // TX_REG_H
