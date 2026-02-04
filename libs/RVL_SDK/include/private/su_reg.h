#ifndef SU_REG_H
#define SU_REG_H

#define SU_LPSIZE_RID_SIZE 8
#define SU_LPSIZE_RID_SHIFT 24
#define SU_LPSIZE_RID_MASK 0xff000000
#define SU_LPSIZE_GET_RID(su_lpsize) ((((unsigned long)(su_lpsize)) & SU_LPSIZE_RID_MASK) >> SU_LPSIZE_RID_SHIFT)

#define SU_TS0_RID_SIZE 8
#define SU_TS0_RID_SHIFT 24
#define SU_TS0_RID_MASK 0xff000000
#define SU_TS0_GET_RID(su_ts0) ((((unsigned long)(su_ts0)) & SU_TS0_RID_MASK) >> SU_TS0_RID_SHIFT)

#define SU_TS1_RID_SIZE 8
#define SU_TS1_RID_SHIFT 24
#define SU_TS1_RID_MASK 0xff000000
#define SU_TS1_GET_RID(su_ts1) ((((unsigned long)(su_ts1)) & SU_TS1_RID_MASK) >> SU_TS1_RID_SHIFT)

#define SU_TS1_TSIZE_SIZE 16
#define SU_TS1_TSIZE_SHIFT 0
#define SU_TS1_TSIZE_MASK 0x0000ffff
#define SU_TS1_GET_TSIZE(su_ts1) ((((unsigned long)(su_ts1)) & SU_TS1_TSIZE_MASK) >> SU_TS1_TSIZE_SHIFT)
#define SU_TS1_SET_TSIZE(su_ts1, tsize)                                                                                                              \
    { su_ts1 = (((unsigned long)(su_ts1)) & ~SU_TS1_TSIZE_MASK) | (((unsigned long)(tsize)) << SU_TS1_TSIZE_SHIFT); }

#define SU_SCIS0_RID_SIZE 8
#define SU_SCIS0_RID_SHIFT 24
#define SU_SCIS0_RID_MASK 0xff000000
#define SU_SCIS0_GET_RID(su_scis0) ((((unsigned long)(su_scis0)) & SU_SCIS0_RID_MASK) >> SU_SCIS0_RID_SHIFT)

#define SU_PERF_SELA_SIZE 3
#define SU_PERF_SELA_SHIFT 0
#define SU_PERF_SELA_MASK 0x00000007
#define SU_PERF_GET_SELA(su_perf) ((((unsigned long)(su_perf)) & SU_PERF_SELA_MASK) >> SU_PERF_SELA_SHIFT)

#define SU_SCIS1_RID_SIZE 8
#define SU_SCIS1_RID_SHIFT 24
#define SU_SCIS1_RID_MASK 0xff000000
#define SU_SCIS1_GET_RID(su_scis1) ((((unsigned long)(su_scis1)) & SU_SCIS1_RID_MASK) >> SU_SCIS1_RID_SHIFT)

#define SU_PERF_NTEX_SIZE 4
#define SU_PERF_NTEX_SHIFT 6
#define SU_PERF_NTEX_MASK 0x000003c0
#define SU_PERF_GET_NTEX(su_perf) ((((unsigned long)(su_perf)) & SU_PERF_NTEX_MASK) >> SU_PERF_NTEX_SHIFT)

#define SU_PERF_NCOL_SIZE 2
#define SU_PERF_NCOL_SHIFT 10
#define SU_PERF_NCOL_MASK 0x00000c00
#define SU_PERF_GET_NCOL(su_perf) ((((unsigned long)(su_perf)) & SU_PERF_NCOL_MASK) >> SU_PERF_NCOL_SHIFT)

#define SU_PERF_SELB_SIZE 3
#define SU_PERF_SELB_SHIFT 3
#define SU_PERF_SELB_MASK 0x00000038
#define SU_PERF_GET_SELB(su_perf) ((((unsigned long)(su_perf)) & SU_PERF_SELB_MASK) >> SU_PERF_SELB_SHIFT)

#define SU_PERF_REJF_SIZE 2
#define SU_PERF_REJF_SHIFT 12
#define SU_PERF_REJF_MASK 0x00003000
#define SU_PERF_GET_REJF(su_perf) ((((unsigned long)(su_perf)) & SU_PERF_REJF_MASK) >> SU_PERF_REJF_SHIFT)

#define SU_PERF_REJS_SIZE 2
#define SU_PERF_REJS_SHIFT 14
#define SU_PERF_REJS_MASK 0x0000c000
#define SU_PERF_GET_REJS(su_perf) ((((unsigned long)(su_perf)) & SU_PERF_REJS_MASK) >> SU_PERF_REJS_SHIFT)

#define SU_PERF_CMD_SIZE 2
#define SU_PERF_CMD_SHIFT 16
#define SU_PERF_CMD_MASK 0x00030000
#define SU_PERF_GET_CMD(su_perf) ((((unsigned long)(su_perf)) & SU_PERF_CMD_MASK) >> SU_PERF_CMD_SHIFT)

#define SU_PERF_PAD0_SIZE 2
#define SU_PERF_PAD0_SHIFT 18
#define SU_PERF_PAD0_MASK 0x000c0000
#define SU_PERF_GET_PAD0(su_perf) ((((unsigned long)(su_perf)) & SU_PERF_PAD0_MASK) >> SU_PERF_PAD0_SHIFT)

#define SU_PERF_EN_SIZE 2
#define SU_PERF_EN_SHIFT 20
#define SU_PERF_EN_MASK 0x00300000
#define SU_PERF_GET_EN(su_perf) ((((unsigned long)(su_perf)) & SU_PERF_EN_MASK) >> SU_PERF_EN_SHIFT)

#define SU_PERF_PWR_EN_SIZE 2
#define SU_PERF_PWR_EN_SHIFT 22
#define SU_PERF_PWR_EN_MASK 0x00c00000
#define SU_PERF_GET_PWR_EN(su_perf) ((((unsigned long)(su_perf)) & SU_PERF_PWR_EN_MASK) >> SU_PERF_PWR_EN_SHIFT)

#define SU_PERF_RID_SIZE 8
#define SU_PERF_RID_SHIFT 24
#define SU_PERF_RID_MASK 0xff000000
#define SU_PERF_GET_RID(su_perf) ((((unsigned long)(su_perf)) & SU_PERF_RID_MASK) >> SU_PERF_RID_SHIFT)

#define SU_PERF_TOTAL_SIZE 32
#define SU_PERF(selA, selB, ntex, ncol, rejf, rejs, cmd, en, pwr_en, rid)                                                                            \
    ((((unsigned long)(selA)) << SU_PERF_SELA_SHIFT) | (((unsigned long)(selB)) << SU_PERF_SELB_SHIFT) |                                             \
     (((unsigned long)(ntex)) << SU_PERF_NTEX_SHIFT) | (((unsigned long)(ncol)) << SU_PERF_NCOL_SHIFT) |                                             \
     (((unsigned long)(rejf)) << SU_PERF_REJF_SHIFT) | (((unsigned long)(rejs)) << SU_PERF_REJS_SHIFT) |                                             \
     (((unsigned long)(cmd)) << SU_PERF_CMD_SHIFT) | (((unsigned long)(en)) << SU_PERF_EN_SHIFT) |                                                   \
     (((unsigned long)(pwr_en)) << SU_PERF_PWR_EN_SHIFT) | (((unsigned long)(rid)) << SU_PERF_RID_SHIFT))

#define SU_LPSIZE_LSIZE_SIZE 8
#define SU_LPSIZE_LSIZE_SHIFT 0
#define SU_LPSIZE_LSIZE_MASK 0x000000ff
#define SU_LPSIZE_GET_LSIZE(su_lpsize) ((((unsigned long)(su_lpsize)) & SU_LPSIZE_LSIZE_MASK) >> SU_LPSIZE_LSIZE_SHIFT)

#define SU_LPSIZE_PSIZE_SIZE 8
#define SU_LPSIZE_PSIZE_SHIFT 8
#define SU_LPSIZE_PSIZE_MASK 0x0000ff00
#define SU_LPSIZE_GET_PSIZE(su_lpsize) ((((unsigned long)(su_lpsize)) & SU_LPSIZE_PSIZE_MASK) >> SU_LPSIZE_PSIZE_SHIFT)

#define SU_LPSIZE_LTOFF_SIZE 3
#define SU_LPSIZE_LTOFF_SHIFT 16
#define SU_LPSIZE_LTOFF_MASK 0x00070000
#define SU_LPSIZE_GET_LTOFF(su_lpsize) ((((unsigned long)(su_lpsize)) & SU_LPSIZE_LTOFF_MASK) >> SU_LPSIZE_LTOFF_SHIFT)

#define SU_LPSIZE_PTOFF_SIZE 3
#define SU_LPSIZE_PTOFF_SHIFT 19
#define SU_LPSIZE_PTOFF_MASK 0x00380000
#define SU_LPSIZE_GET_PTOFF(su_lpsize) ((((unsigned long)(su_lpsize)) & SU_LPSIZE_PTOFF_MASK) >> SU_LPSIZE_PTOFF_SHIFT)

#define SU_LPSIZE_FIELDMODE_SIZE 1
#define SU_LPSIZE_FIELDMODE_SHIFT 22
#define SU_LPSIZE_FIELDMODE_MASK 0x00400000
#define SU_LPSIZE_GET_FIELDMODE(su_lpsize) ((((unsigned long)(su_lpsize)) & SU_LPSIZE_FIELDMODE_MASK) >> SU_LPSIZE_FIELDMODE_SHIFT)

#define SU_LPSIZE_PAD0_SIZE 1
#define SU_LPSIZE_PAD0_SHIFT 23
#define SU_LPSIZE_PAD0_MASK 0x00800000
#define SU_LPSIZE_GET_PAD0(su_lpsize) ((((unsigned long)(su_lpsize)) & SU_LPSIZE_PAD0_MASK) >> SU_LPSIZE_PAD0_SHIFT)

#define SU_LPSIZE_RID_SIZE 8
#define SU_LPSIZE_RID_SHIFT 24
#define SU_LPSIZE_RID_MASK 0xff000000
#define SU_LPSIZE_GET_RID(su_lpsize) ((((unsigned long)(su_lpsize)) & SU_LPSIZE_RID_MASK) >> SU_LPSIZE_RID_SHIFT)

#define SU_TS0_SSIZE_SIZE 16
#define SU_TS0_SSIZE_SHIFT 0
#define SU_TS0_SSIZE_MASK 0x0000ffff
#define SU_TS0_GET_SSIZE(su_ts0) ((((unsigned long)(su_ts0)) & SU_TS0_SSIZE_MASK) >> SU_TS0_SSIZE_SHIFT)

#define SU_TS0_BS_SIZE 1
#define SU_TS0_BS_SHIFT 16
#define SU_TS0_BS_MASK 0x00010000
#define SU_TS0_GET_BS(su_ts0) ((((unsigned long)(su_ts0)) & SU_TS0_BS_MASK) >> SU_TS0_BS_SHIFT)

#define SU_TS0_WS_SIZE 1
#define SU_TS0_WS_SHIFT 17
#define SU_TS0_WS_MASK 0x00020000
#define SU_TS0_GET_WS(su_ts0) ((((unsigned long)(su_ts0)) & SU_TS0_WS_MASK) >> SU_TS0_WS_SHIFT)

#define SU_TS0_LF_SIZE 1
#define SU_TS0_LF_SHIFT 18
#define SU_TS0_LF_MASK 0x00040000
#define SU_TS0_GET_LF(su_ts0) ((((unsigned long)(su_ts0)) & SU_TS0_LF_MASK) >> SU_TS0_LF_SHIFT)

#define SU_TS0_PF_SIZE 1
#define SU_TS0_PF_SHIFT 19
#define SU_TS0_PF_MASK 0x00080000
#define SU_TS0_GET_PF(su_ts0) ((((unsigned long)(su_ts0)) & SU_TS0_PF_MASK) >> SU_TS0_PF_SHIFT)

#define SU_TS0_PAD0_SIZE 4
#define SU_TS0_PAD0_SHIFT 20
#define SU_TS0_PAD0_MASK 0x00f00000
#define SU_TS0_GET_PAD0(su_ts0) ((((unsigned long)(su_ts0)) & SU_TS0_PAD0_MASK) >> SU_TS0_PAD0_SHIFT)

#define SU_TS0_RID_SIZE 8
#define SU_TS0_RID_SHIFT 24
#define SU_TS0_RID_MASK 0xff000000
#define SU_TS0_GET_RID(su_ts0) ((((unsigned long)(su_ts0)) & SU_TS0_RID_MASK) >> SU_TS0_RID_SHIFT)

#define SU_SSMASK_SSMASK_SIZE 24
#define SU_SSMASK_SSMASK_SHIFT 0
#define SU_SSMASK_SSMASK_MASK 0x00ffffff
#define SU_SSMASK_GET_SSMASK(su_ssmask) ((((unsigned long)(su_ssmask)) & SU_SSMASK_SSMASK_MASK) >> SU_SSMASK_SSMASK_SHIFT)

#define SU_SSMASK_RID_SIZE 8
#define SU_SSMASK_RID_SHIFT 24
#define SU_SSMASK_RID_MASK 0xff000000
#define SU_SSMASK_GET_RID(su_ssmask) ((((unsigned long)(su_ssmask)) & SU_SSMASK_RID_MASK) >> SU_SSMASK_RID_SHIFT)

#define SU_SSMASK_TOTAL_SIZE 32
#define SU_SSMASK(ssmask, rid) ((((unsigned long)(ssmask)) << SU_SSMASK_SSMASK_SHIFT) | (((unsigned long)(rid)) << SU_SSMASK_RID_SHIFT))

#define SU_SCIS0_SY0_SIZE 11
#define SU_SCIS0_SY0_SHIFT 0
#define SU_SCIS0_SY0_MASK 0x000007ff
#define SU_SCIS0_GET_SY0(su_scis0) ((((unsigned long)(su_scis0)) & SU_SCIS0_SY0_MASK) >> SU_SCIS0_SY0_SHIFT)
#define SU_SCIS0_SET_SY0(su_scis0, sy0)                                                                                                              \
    { su_scis0 = (((unsigned long)(su_scis0)) & ~SU_SCIS0_SY0_MASK) | (((unsigned long)(sy0)) << SU_SCIS0_SY0_SHIFT); }

#define SU_SCIS0_SX0_SIZE 11
#define SU_SCIS0_SX0_SHIFT 12
#define SU_SCIS0_SX0_MASK 0x007ff000
#define SU_SCIS0_GET_SX0(su_scis0) ((((unsigned long)(su_scis0)) & SU_SCIS0_SX0_MASK) >> SU_SCIS0_SX0_SHIFT)
#define SU_SCIS0_SET_SX0(su_scis0, sx0)                                                                                                              \
    { su_scis0 = (((unsigned long)(su_scis0)) & ~SU_SCIS0_SX0_MASK) | (((unsigned long)(sx0)) << SU_SCIS0_SX0_SHIFT); }

#define SU_SCIS0_PAD1_SIZE 1
#define SU_SCIS0_PAD1_SHIFT 23
#define SU_SCIS0_PAD1_MASK 0x00800000
#define SU_SCIS0_GET_PAD1(su_scis0) ((((unsigned long)(su_scis0)) & SU_SCIS0_PAD1_MASK) >> SU_SCIS0_PAD1_SHIFT)
#define SU_SCIS0_SET_PAD1(su_scis0, pad1)                                                                                                            \
    { su_scis0 = (((unsigned long)(su_scis0)) & ~SU_SCIS0_PAD1_MASK) | (((unsigned long)(pad1)) << SU_SCIS0_PAD1_SHIFT); }

#define SU_SCIS1_SY1_SIZE 11
#define SU_SCIS1_SY1_SHIFT 0
#define SU_SCIS1_SY1_MASK 0x000007ff
#define SU_SCIS1_GET_SY1(su_scis1) ((((unsigned long)(su_scis1)) & SU_SCIS1_SY1_MASK) >> SU_SCIS1_SY1_SHIFT)
#define SU_SCIS1_SET_SY1(su_scis1, sy1)                                                                                                              \
    { su_scis1 = (((unsigned long)(su_scis1)) & ~SU_SCIS1_SY1_MASK) | (((unsigned long)(sy1)) << SU_SCIS1_SY1_SHIFT); }

#define SU_SCIS1_SX1_SIZE 11
#define SU_SCIS1_SX1_SHIFT 12
#define SU_SCIS1_SX1_MASK 0x007ff000
#define SU_SCIS1_GET_SX1(su_scis1) ((((unsigned long)(su_scis1)) & SU_SCIS1_SX1_MASK) >> SU_SCIS1_SX1_SHIFT)
#define SU_SCIS1_SET_SX1(su_scis1, sx1)                                                                                                              \
    { su_scis1 = (((unsigned long)(su_scis1)) & ~SU_SCIS1_SX1_MASK) | (((unsigned long)(sx1)) << SU_SCIS1_SX1_SHIFT); }

#endif  // SU_REG_H
