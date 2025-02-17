#ifndef SU_REG_H
#define SU_REG_H

#define SU_LPSIZE_RID_SIZE	8
#define SU_LPSIZE_RID_SHIFT	24
#define SU_LPSIZE_RID_MASK	0xff000000
#define SU_LPSIZE_GET_RID(su_lpsize) \
	((((unsigned long)(su_lpsize)) & SU_LPSIZE_RID_MASK) >> SU_LPSIZE_RID_SHIFT)


#define SU_TS0_RID_SIZE	8
#define SU_TS0_RID_SHIFT	24
#define SU_TS0_RID_MASK	0xff000000
#define SU_TS0_GET_RID(su_ts0) \
    ((((unsigned long)(su_ts0)) & SU_TS0_RID_MASK) >> SU_TS0_RID_SHIFT)

#define SU_TS1_RID_SIZE	8
#define SU_TS1_RID_SHIFT	24
#define SU_TS1_RID_MASK	0xff000000
#define SU_TS1_GET_RID(su_ts1) \
    ((((unsigned long)(su_ts1)) & SU_TS1_RID_MASK) >> SU_TS1_RID_SHIFT)

#define SU_SCIS0_RID_SIZE	8
#define SU_SCIS0_RID_SHIFT	24
#define SU_SCIS0_RID_MASK	0xff000000
#define SU_SCIS0_GET_RID(su_scis0) \
    ((((unsigned long)(su_scis0)) & SU_SCIS0_RID_MASK) >> SU_SCIS0_RID_SHIFT)

#define SU_PERF_SELA_SIZE	3
#define SU_PERF_SELA_SHIFT	0
#define SU_PERF_SELA_MASK	0x00000007
#define SU_PERF_GET_SELA(su_perf) \
    ((((unsigned long)(su_perf)) & SU_PERF_SELA_MASK) >> SU_PERF_SELA_SHIFT)

#define SU_SCIS1_RID_SIZE	8
#define SU_SCIS1_RID_SHIFT	24
#define SU_SCIS1_RID_MASK	0xff000000
#define SU_SCIS1_GET_RID(su_scis1) \
    ((((unsigned long)(su_scis1)) & SU_SCIS1_RID_MASK) >> SU_SCIS1_RID_SHIFT)

#define SU_PERF_NTEX_SIZE	4
#define SU_PERF_NTEX_SHIFT	6
#define SU_PERF_NTEX_MASK	0x000003c0
#define SU_PERF_GET_NTEX(su_perf) \
    ((((unsigned long)(su_perf)) & SU_PERF_NTEX_MASK) >> SU_PERF_NTEX_SHIFT)

#define SU_PERF_NCOL_SIZE	2
#define SU_PERF_NCOL_SHIFT	10
#define SU_PERF_NCOL_MASK	0x00000c00
#define SU_PERF_GET_NCOL(su_perf) \
    ((((unsigned long)(su_perf)) & SU_PERF_NCOL_MASK) >> SU_PERF_NCOL_SHIFT)

#define SU_PERF_SELB_SIZE	3
#define SU_PERF_SELB_SHIFT	3
#define SU_PERF_SELB_MASK	0x00000038
#define SU_PERF_GET_SELB(su_perf) \
    ((((unsigned long)(su_perf)) & SU_PERF_SELB_MASK) >> SU_PERF_SELB_SHIFT)

#define SU_PERF_REJF_SIZE	2
#define SU_PERF_REJF_SHIFT	12
#define SU_PERF_REJF_MASK	0x00003000
#define SU_PERF_GET_REJF(su_perf) \
    ((((unsigned long)(su_perf)) & SU_PERF_REJF_MASK) >> SU_PERF_REJF_SHIFT)

#define SU_PERF_REJS_SIZE	2
#define SU_PERF_REJS_SHIFT	14
#define SU_PERF_REJS_MASK	0x0000c000
#define SU_PERF_GET_REJS(su_perf) \
    ((((unsigned long)(su_perf)) & SU_PERF_REJS_MASK) >> SU_PERF_REJS_SHIFT)

#define SU_PERF_CMD_SIZE	2
#define SU_PERF_CMD_SHIFT	16
#define SU_PERF_CMD_MASK	0x00030000
#define SU_PERF_GET_CMD(su_perf) \
    ((((unsigned long)(su_perf)) & SU_PERF_CMD_MASK) >> SU_PERF_CMD_SHIFT)

#define SU_PERF_PAD0_SIZE	2
#define SU_PERF_PAD0_SHIFT	18
#define SU_PERF_PAD0_MASK	0x000c0000
#define SU_PERF_GET_PAD0(su_perf) \
    ((((unsigned long)(su_perf)) & SU_PERF_PAD0_MASK) >> SU_PERF_PAD0_SHIFT)

#define SU_PERF_EN_SIZE	2
#define SU_PERF_EN_SHIFT	20
#define SU_PERF_EN_MASK	0x00300000
#define SU_PERF_GET_EN(su_perf) \
    ((((unsigned long)(su_perf)) & SU_PERF_EN_MASK) >> SU_PERF_EN_SHIFT)

#define SU_PERF_PWR_EN_SIZE	2
#define SU_PERF_PWR_EN_SHIFT	22
#define SU_PERF_PWR_EN_MASK	0x00c00000
#define SU_PERF_GET_PWR_EN(su_perf) \
    ((((unsigned long)(su_perf)) & SU_PERF_PWR_EN_MASK) >> SU_PERF_PWR_EN_SHIFT)

#define SU_PERF_RID_SIZE	8
#define SU_PERF_RID_SHIFT	24
#define SU_PERF_RID_MASK	0xff000000
#define SU_PERF_GET_RID(su_perf) \
    ((((unsigned long)(su_perf)) & SU_PERF_RID_MASK) >> SU_PERF_RID_SHIFT)

#define SU_PERF_TOTAL_SIZE	32
#define SU_PERF(selA, selB, ntex, ncol, rejf, rejs, cmd, en, pwr_en, rid) \
    ((((unsigned long)(selA)) << SU_PERF_SELA_SHIFT) | \
        (((unsigned long)(selB)) << SU_PERF_SELB_SHIFT) | \
        (((unsigned long)(ntex)) << SU_PERF_NTEX_SHIFT) | \
        (((unsigned long)(ncol)) << SU_PERF_NCOL_SHIFT) | \
        (((unsigned long)(rejf)) << SU_PERF_REJF_SHIFT) | \
        (((unsigned long)(rejs)) << SU_PERF_REJS_SHIFT) | \
        (((unsigned long)(cmd)) << SU_PERF_CMD_SHIFT) | \
        (((unsigned long)(en)) << SU_PERF_EN_SHIFT) | \
        (((unsigned long)(pwr_en)) << SU_PERF_PWR_EN_SHIFT) | \
        (((unsigned long)(rid)) << SU_PERF_RID_SHIFT))

    #endif // SU_REG_H
