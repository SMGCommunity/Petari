#ifndef RAS_REG
#define RAS_REG

#define RAS1_TREF_RID_SIZE 8
#define RAS1_TREF_RID_SHIFT 24
#define RAS1_TREF_RID_MASK 0xff000000
#define RAS1_TREF_GET_RID(ras1_tref) ((((unsigned long)(ras1_tref)) & RAS1_TREF_RID_MASK) >> RAS1_TREF_RID_SHIFT)

#define RAS1_IREF_RID_SIZE 8
#define RAS1_IREF_RID_SHIFT 24
#define RAS1_IREF_RID_MASK 0xff000000
#define RAS1_IREF_GET_RID(ras1_iref) ((((unsigned long)(ras1_iref)) & RAS1_IREF_RID_MASK) >> RAS1_IREF_RID_SHIFT)

#define RAS_PERF_SELA_SIZE 5
#define RAS_PERF_SELA_SHIFT 0
#define RAS_PERF_SELA_MASK 0x0000001f
#define RAS_PERF_GET_SELA(ras_perf) ((((unsigned long)(ras_perf)) & RAS_PERF_SELA_MASK) >> RAS_PERF_SELA_SHIFT)

#define RAS_PERF_SELB_SIZE 5
#define RAS_PERF_SELB_SHIFT 5
#define RAS_PERF_SELB_MASK 0x000003e0
#define RAS_PERF_GET_SELB(ras_perf) ((((unsigned long)(ras_perf)) & RAS_PERF_SELB_MASK) >> RAS_PERF_SELB_SHIFT)

#define RAS_PERF_NTEV_SIZE 5
#define RAS_PERF_NTEV_SHIFT 10
#define RAS_PERF_NTEV_MASK 0x00007c00
#define RAS_PERF_GET_NTEV(ras_perf) ((((unsigned long)(ras_perf)) & RAS_PERF_NTEV_MASK) >> RAS_PERF_NTEV_SHIFT)

#define RAS_PERF_NBMP_SIZE 3
#define RAS_PERF_NBMP_SHIFT 15
#define RAS_PERF_NBMP_MASK 0x00038000
#define RAS_PERF_GET_NBMP(ras_perf) ((((unsigned long)(ras_perf)) & RAS_PERF_NBMP_MASK) >> RAS_PERF_NBMP_SHIFT)

#define RAS_PERF_NBR_SIZE 3
#define RAS_PERF_NBR_SHIFT 18
#define RAS_PERF_NBR_MASK 0x001c0000
#define RAS_PERF_GET_NBR(ras_perf) ((((unsigned long)(ras_perf)) & RAS_PERF_NBR_MASK) >> RAS_PERF_NBR_SHIFT)

#define RAS_PERF_PAD0_SIZE 3
#define RAS_PERF_PAD0_SHIFT 21
#define RAS_PERF_PAD0_MASK 0x00e00000
#define RAS_PERF_GET_PAD0(ras_perf) ((((unsigned long)(ras_perf)) & RAS_PERF_PAD0_MASK) >> RAS_PERF_PAD0_SHIFT)
#define RAS_PERF_RID_SIZE 8
#define RAS_PERF_RID_SHIFT 24
#define RAS_PERF_RID_MASK 0xff000000
#define RAS_PERF_GET_RID(ras_perf) ((((unsigned long)(ras_perf)) & RAS_PERF_RID_MASK) >> RAS_PERF_RID_SHIFT)

#define RAS_PERF_TOTAL_SIZE 32
#define RAS_PERF(selA, selB, ntev, nbmp, nbr, rid)                                                                                                   \
    ((((unsigned long)(selA)) << RAS_PERF_SELA_SHIFT) | (((unsigned long)(selB)) << RAS_PERF_SELB_SHIFT) |                                           \
     (((unsigned long)(ntev)) << RAS_PERF_NTEV_SHIFT) | (((unsigned long)(nbmp)) << RAS_PERF_NBMP_SHIFT) |                                           \
     (((unsigned long)(nbr)) << RAS_PERF_NBR_SHIFT) | (((unsigned long)(rid)) << RAS_PERF_RID_SHIFT))

#define RAS1_TREF_TI0_SIZE 3
#define RAS1_TREF_TI0_SHIFT 0
#define RAS1_TREF_TI0_MASK 0x00000007
#define RAS1_TREF_GET_TI0(ras1_tref) ((((unsigned long)(ras1_tref)) & RAS1_TREF_TI0_MASK) >> RAS1_TREF_TI0_SHIFT)

#define RAS1_TREF_TC0_SIZE 3
#define RAS1_TREF_TC0_SHIFT 3
#define RAS1_TREF_TC0_MASK 0x00000038
#define RAS1_TREF_GET_TC0(ras1_tref) ((((unsigned long)(ras1_tref)) & RAS1_TREF_TC0_MASK) >> RAS1_TREF_TC0_SHIFT)

#define RAS1_TREF_TE0_SIZE 1
#define RAS1_TREF_TE0_SHIFT 6
#define RAS1_TREF_TE0_MASK 0x00000040
#define RAS1_TREF_GET_TE0(ras1_tref) ((((unsigned long)(ras1_tref)) & RAS1_TREF_TE0_MASK) >> RAS1_TREF_TE0_SHIFT)

#define RAS1_TREF_CC0_SIZE 3
#define RAS1_TREF_CC0_SHIFT 7
#define RAS1_TREF_CC0_MASK 0x00000380
#define RAS1_TREF_GET_CC0(ras1_tref) ((((unsigned long)(ras1_tref)) & RAS1_TREF_CC0_MASK) >> RAS1_TREF_CC0_SHIFT)

#define RAS1_TREF_PAD0_SIZE 2
#define RAS1_TREF_PAD0_SHIFT 10
#define RAS1_TREF_PAD0_MASK 0x00000c00
#define RAS1_TREF_GET_PAD0(ras1_tref) ((((unsigned long)(ras1_tref)) & RAS1_TREF_PAD0_MASK) >> RAS1_TREF_PAD0_SHIFT)

#define RAS1_TREF_TI1_SIZE 3
#define RAS1_TREF_TI1_SHIFT 12
#define RAS1_TREF_TI1_MASK 0x00007000
#define RAS1_TREF_GET_TI1(ras1_tref) ((((unsigned long)(ras1_tref)) & RAS1_TREF_TI1_MASK) >> RAS1_TREF_TI1_SHIFT)

#define RAS1_TREF_TC1_SIZE 3
#define RAS1_TREF_TC1_SHIFT 15
#define RAS1_TREF_TC1_MASK 0x00038000
#define RAS1_TREF_GET_TC1(ras1_tref) ((((unsigned long)(ras1_tref)) & RAS1_TREF_TC1_MASK) >> RAS1_TREF_TC1_SHIFT)

#define RAS1_TREF_TE1_SIZE 1
#define RAS1_TREF_TE1_SHIFT 18
#define RAS1_TREF_TE1_MASK 0x00040000
#define RAS1_TREF_GET_TE1(ras1_tref) ((((unsigned long)(ras1_tref)) & RAS1_TREF_TE1_MASK) >> RAS1_TREF_TE1_SHIFT)

#define RAS1_TREF_CC1_SIZE 3
#define RAS1_TREF_CC1_SHIFT 19
#define RAS1_TREF_CC1_MASK 0x00380000
#define RAS1_TREF_GET_CC1(ras1_tref) ((((unsigned long)(ras1_tref)) & RAS1_TREF_CC1_MASK) >> RAS1_TREF_CC1_SHIFT)

#define RAS1_TREF_PAD1_SIZE 2
#define RAS1_TREF_PAD1_SHIFT 22
#define RAS1_TREF_PAD1_MASK 0x00c00000
#define RAS1_TREF_GET_PAD1(ras1_tref) ((((unsigned long)(ras1_tref)) & RAS1_TREF_PAD1_MASK) >> RAS1_TREF_PAD1_SHIFT)

#define RAS1_TREF_RID_SIZE 8
#define RAS1_TREF_RID_SHIFT 24
#define RAS1_TREF_RID_MASK 0xff000000
#define RAS1_TREF_GET_RID(ras1_tref) ((((unsigned long)(ras1_tref)) & RAS1_TREF_RID_MASK) >> RAS1_TREF_RID_SHIFT)

#define RAS1_SS_SS0_SIZE 4
#define RAS1_SS_SS0_SHIFT 0
#define RAS1_SS_SS0_MASK 0x0000000f
#define RAS1_SS_GET_SS0(ras1_ss) ((((unsigned long)(ras1_ss)) & RAS1_SS_SS0_MASK) >> RAS1_SS_SS0_SHIFT)
#define RAS1_SS_SET_SS0(ras1_ss, ss0)                                                                                                                \
    { ras1_ss = (((unsigned long)(ras1_ss)) & ~RAS1_SS_SS0_MASK) | (((unsigned long)(ss0)) << RAS1_SS_SS0_SHIFT); }

#define RAS1_SS_SS1_SIZE 4
#define RAS1_SS_SS1_SHIFT 8
#define RAS1_SS_SS1_MASK 0x00000f00
#define RAS1_SS_GET_SS1(ras1_ss) ((((unsigned long)(ras1_ss)) & RAS1_SS_SS1_MASK) >> RAS1_SS_SS1_SHIFT)
#define RAS1_SS_SET_SS1(ras1_ss, ss1)                                                                                                                \
    { ras1_ss = (((unsigned long)(ras1_ss)) & ~RAS1_SS_SS1_MASK) | (((unsigned long)(ss1)) << RAS1_SS_SS1_SHIFT); }

#define RAS1_SS_TS0_SIZE 4
#define RAS1_SS_TS0_SHIFT 4
#define RAS1_SS_TS0_MASK 0x000000f0
#define RAS1_SS_GET_TS0(ras1_ss) ((((unsigned long)(ras1_ss)) & RAS1_SS_TS0_MASK) >> RAS1_SS_TS0_SHIFT)
#define RAS1_SS_SET_TS0(ras1_ss, ts0)                                                                                                                \
    { ras1_ss = (((unsigned long)(ras1_ss)) & ~RAS1_SS_TS0_MASK) | (((unsigned long)(ts0)) << RAS1_SS_TS0_SHIFT); }

#define RAS1_SS_TS1_SIZE 4
#define RAS1_SS_TS1_SHIFT 12
#define RAS1_SS_TS1_MASK 0x0000f000
#define RAS1_SS_GET_TS1(ras1_ss) ((((unsigned long)(ras1_ss)) & RAS1_SS_TS1_MASK) >> RAS1_SS_TS1_SHIFT)
#define RAS1_SS_SET_TS1(ras1_ss, ts1)                                                                                                                \
    { ras1_ss = (((unsigned long)(ras1_ss)) & ~RAS1_SS_TS1_MASK) | (((unsigned long)(ts1)) << RAS1_SS_TS1_SHIFT); }

#define RAS1_SS_RID_SIZE 8
#define RAS1_SS_RID_SHIFT 24
#define RAS1_SS_RID_MASK 0xff000000
#define RAS1_SS_GET_RID(ras1_ss) ((((unsigned long)(ras1_ss)) & RAS1_SS_RID_MASK) >> RAS1_SS_RID_SHIFT)
#define RAS1_SS_SET_RID(ras1_ss, rid)                                                                                                                \
    { ras1_ss = (((unsigned long)(ras1_ss)) & ~RAS1_SS_RID_MASK) | (((unsigned long)(rid)) << RAS1_SS_RID_SHIFT); }

#define RAS1_IREF_BI0_SIZE 3
#define RAS1_IREF_BI0_SHIFT 0
#define RAS1_IREF_BI0_MASK 0x00000007
#define RAS1_IREF_GET_BI0(ras1_iref) ((((unsigned long)(ras1_iref)) & RAS1_IREF_BI0_MASK) >> RAS1_IREF_BI0_SHIFT)
#define RAS1_IREF_SET_BI0(ras1_iref, bi0)                                                                                                            \
    {                                                                                                                                                \ ras1_iref = (((unsigned long)(ras1_iref)) & ~RAS1_IREF_BI0_MASK) | (((unsigned long)(bi0)) << RAS1_IREF_BI0_SHIFT); }
#define RAS1_IREF_BC0_SIZE 3
#define RAS1_IREF_BC0_SHIFT 3
#define RAS1_IREF_BC0_MASK 0x00000038
#define RAS1_IREF_GET_BC0(ras1_iref) ((((unsigned long)(ras1_iref)) & RAS1_IREF_BC0_MASK) >> RAS1_IREF_BC0_SHIFT)
#define RAS1_IREF_SET_BC0(ras1_iref, bc0)                                                                                                            \
    {                                                                                                                                                \ ras1_iref = (((unsigned long)(ras1_iref)) & ~RAS1_IREF_BC0_MASK) | (((unsigned long)(bc0)) << RAS1_IREF_BC0_SHIFT); }
#define RAS1_IREF_BI1_SIZE 3
#define RAS1_IREF_BI1_SHIFT 6
#define RAS1_IREF_BI1_MASK 0x000001c0
#define RAS1_IREF_GET_BI1(ras1_iref) ((((unsigned long)(ras1_iref)) & RAS1_IREF_BI1_MASK) >> RAS1_IREF_BI1_SHIFT)
#define RAS1_IREF_SET_BI1(ras1_iref, bi1)                                                                                                            \
    {                                                                                                                                                \ ras1_iref = (((unsigned long)(ras1_iref)) & ~RAS1_IREF_BI1_MASK) | (((unsigned long)(bi1)) << RAS1_IREF_BI1_SHIFT); }
#define RAS1_IREF_BC1_SIZE 3
#define RAS1_IREF_BC1_SHIFT 9
#define RAS1_IREF_BC1_MASK 0x00000e00
#define RAS1_IREF_GET_BC1(ras1_iref) ((((unsigned long)(ras1_iref)) & RAS1_IREF_BC1_MASK) >> RAS1_IREF_BC1_SHIFT)
#define RAS1_IREF_SET_BC1(ras1_iref, bc1)                                                                                                            \
    {                                                                                                                                                \ ras1_iref = (((unsigned long)(ras1_iref)) & ~RAS1_IREF_BC1_MASK) | (((unsigned long)(bc1)) << RAS1_IREF_BC1_SHIFT); }
#define RAS1_IREF_BI2_SIZE 3
#define RAS1_IREF_BI2_SHIFT 12
#define RAS1_IREF_BI2_MASK 0x00007000
#define RAS1_IREF_GET_BI2(ras1_iref) ((((unsigned long)(ras1_iref)) & RAS1_IREF_BI2_MASK) >> RAS1_IREF_BI2_SHIFT)
#define RAS1_IREF_SET_BI2(ras1_iref, bi2)                                                                                                            \
    {                                                                                                                                                \ ras1_iref = (((unsigned long)(ras1_iref)) & ~RAS1_IREF_BI2_MASK) | (((unsigned long)(bi2)) << RAS1_IREF_BI2_SHIFT); }
#define RAS1_IREF_BC2_SIZE 3
#define RAS1_IREF_BC2_SHIFT 15
#define RAS1_IREF_BC2_MASK 0x00038000
#define RAS1_IREF_GET_BC2(ras1_iref) ((((unsigned long)(ras1_iref)) & RAS1_IREF_BC2_MASK) >> RAS1_IREF_BC2_SHIFT)
#define RAS1_IREF_SET_BC2(ras1_iref, bc2)                                                                                                            \
    {                                                                                                                                                \ ras1_iref = (((unsigned long)(ras1_iref)) & ~RAS1_IREF_BC2_MASK) | (((unsigned long)(bc2)) << RAS1_IREF_BC2_SHIFT); }
#define RAS1_IREF_BI3_SIZE 3
#define RAS1_IREF_BI3_SHIFT 18
#define RAS1_IREF_BI3_MASK 0x001c0000
#define RAS1_IREF_GET_BI3(ras1_iref) ((((unsigned long)(ras1_iref)) & RAS1_IREF_BI3_MASK) >> RAS1_IREF_BI3_SHIFT)
#define RAS1_IREF_SET_BI3(ras1_iref, bi3)                                                                                                            \
    {                                                                                                                                                \ ras1_iref = (((unsigned long)(ras1_iref)) & ~RAS1_IREF_BI3_MASK) | (((unsigned long)(bi3)) << RAS1_IREF_BI3_SHIFT); }
#define RAS1_IREF_BC3_SIZE 3
#define RAS1_IREF_BC3_SHIFT 21
#define RAS1_IREF_BC3_MASK 0x00e00000
#define RAS1_IREF_GET_BC3(ras1_iref) ((((unsigned long)(ras1_iref)) & RAS1_IREF_BC3_MASK) >> RAS1_IREF_BC3_SHIFT)
#define RAS1_IREF_SET_BC3(ras1_iref, bc3)                                                                                                            \
    {                                                                                                                                                \ ras1_iref = (((unsigned long)(ras1_iref)) & ~RAS1_IREF_BC3_MASK) | (((unsigned long)(bc3)) << RAS1_IREF_BC3_SHIFT); }

#endif  // RAS_REG
