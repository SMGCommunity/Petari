#include "revolution/gx/GXPerf.h"
#include "revolution/gx/GXRegs.h"
#include "revolution/gx.h"

void GXSetGPMetric(GXPerf0 perf0, GXPerf1 perf1) {
    u32 reg;

    switch (gx->perf0) {
        case GX_PERF0_VERTICES:
        case GX_PERF0_CLIP_VTX:
        case GX_PERF0_CLIP_CLKS:
        case GX_PERF0_XF_WAIT_IN:
        case GX_PERF0_XF_WAIT_OUT:
        case GX_PERF0_XF_XFRM_CLKS:
        case GX_PERF0_XF_LIT_CLKS:
        case GX_PERF0_XF_BOT_CLKS:
        case GX_PERF0_XF_REGLD_CLKS:
        case GX_PERF0_XF_REGRD_CLKS:
        case GX_PERF0_CLOCKS: 
        case GX_PERF0_CLIP_RATIO:
            GX_WRITE_XF_REG(0x1006, 0, 0);
            break;
        case GX_PERF0_TRIANGLES:
        case GX_PERF0_TRIANGLES_CULLED:
        case GX_PERF0_TRIANGLES_PASSED:
        case GX_PERF0_TRIANGLES_SCISSORED:
        case GX_PERF0_TRIANGLES_0TEX:
        case GX_PERF0_TRIANGLES_1TEX:
        case GX_PERF0_TRIANGLES_2TEX:
        case GX_PERF0_TRIANGLES_3TEX:
        case GX_PERF0_TRIANGLES_4TEX:
        case GX_PERF0_TRIANGLES_5TEX:
        case GX_PERF0_TRIANGLES_6TEX:
        case GX_PERF0_TRIANGLES_7TEX:
        case GX_PERF0_TRIANGLES_8TEX:
        case GX_PERF0_TRIANGLES_0CLR:
        case GX_PERF0_TRIANGLES_1CLR:
        case GX_PERF0_TRIANGLES_2CLR:
            GX_WRITE_RA_REG(0x23000000);
            break;
        case GX_PERF0_QUAD_0CVG:
        case GX_PERF0_QUAD_NON0CVG:
        case GX_PERF0_QUAD_1CVG:
        case GX_PERF0_QUAD_2CVG:
        case GX_PERF0_QUAD_3CVG:
        case GX_PERF0_QUAD_4CVG:
        case GX_PERF0_AVG_QUAD_CNT:
            GX_WRITE_RA_REG(0x24000000);
            break;
        case GX_PERF0_NONE:
        default:
            break;
    }

    switch (gx->perf1) {
        case GX_PERF1_TEXELS:
        case GX_PERF1_TX_IDLE:
        case GX_PERF1_TX_REGS:
        case GX_PERF1_TX_MEMSTALL:
        case GX_PERF1_TC_MISS:
        case GX_PERF1_CLOCKS:
        case GX_PERF1_TC_CHECK1_2:
        case GX_PERF1_TC_CHECK3_4:
        case GX_PERF1_TC_CHECK5_6:
        case GX_PERF1_TC_CHECK7_8:
            GX_WRITE_RA_REG(0x67000000);
            break;

        case GX_PERF1_VC_ELEMQ_FULL:
        case GX_PERF1_VC_MISSQ_FULL:
        case GX_PERF1_VC_MEMREQ_FULL:
        case GX_PERF1_VC_STATUS7:
        case GX_PERF1_VC_MISSREP_FULL:
        case GX_PERF1_VC_STREAMBUF_LOW:
        case GX_PERF1_VC_ALL_STALLS:
        case GX_PERF1_VERTICES:
            SC_CP_STAT_SEL_REG_SET_STALLPERF_SEL(gx->perfSel, STALLPERF_ZERO);
            GX_WRITE_CP_STRM_REG(CP_STAT_SEL, 0, gx->perfSel);
            break;

        case GX_PERF1_FIFO_REQ:
        case GX_PERF1_CALL_REQ:
        case GX_PERF1_VC_MISS_REQ:
        case GX_PERF1_CP_ALL_REQ:
            reg = CP_REG_MEMPERFSEL(MEMPERF_ZERO);
            GX_CP_REG_WRITE_U16(CP_MEMPERF_SEL, reg);
            break;

        case GX_PERF1_NONE:
        default:
            break;
    }

    gx->perf0 = perf0;

    switch (gx->perf0) {
        case GX_PERF0_VERTICES:
            GX_WRITE_XF_REG(0x1006, 0x273, 0);
            break;
            case GX_PERF0_CLIP_VTX:
            GX_WRITE_XF_REG(0x1006, 0x14a, 0);
            break;
        case GX_PERF0_CLIP_CLKS:
            GX_WRITE_XF_REG(0x1006, 0x16b, 0);
            break;
        case GX_PERF0_XF_WAIT_IN:
            GX_WRITE_XF_REG(0x1006, 0x84, 0);
            break;
        case GX_PERF0_XF_WAIT_OUT:
            GX_WRITE_XF_REG(0x1006, 0xc6, 0);
            break;
        case GX_PERF0_XF_XFRM_CLKS:
            GX_WRITE_XF_REG(0x1006, 0x210, 0);
            break;
        case GX_PERF0_XF_LIT_CLKS:
            GX_WRITE_XF_REG(0x1006, 0x252, 0);
            break;
        case GX_PERF0_XF_BOT_CLKS:
            GX_WRITE_XF_REG(0x1006, 0x231, 0);
            break;
        case GX_PERF0_XF_REGLD_CLKS:
            GX_WRITE_XF_REG(0x1006, 0x1AD, 0);
            break;
        case GX_PERF0_XF_REGRD_CLKS:
            GX_WRITE_XF_REG(0x1006, 0x1CE, 0);
            break;
        case GX_PERF0_CLOCKS:
            GX_WRITE_XF_REG(0x1006, 0x21, 0);
            break;
        case GX_PERF0_CLIP_RATIO:
            GX_WRITE_XF_REG(0x1006, 0x153, 0);
            break;
        case GX_PERF0_TRIANGLES:
            GX_WRITE_RA_REG(0x2300ae7f);
            break;
        case GX_PERF0_TRIANGLES_CULLED:
            GX_WRITE_RA_REG(0x23008e7f);
            break;
        case GX_PERF0_TRIANGLES_PASSED:
            GX_WRITE_RA_REG(0x23009e7f);
            break;
        case GX_PERF0_TRIANGLES_SCISSORED:
            GX_WRITE_RA_REG(0x23001e7f);
            break;
        case GX_PERF0_TRIANGLES_0TEX:
            GX_WRITE_RA_REG(0x2300ac3f);
            break;
        case GX_PERF0_TRIANGLES_1TEX:
            GX_WRITE_RA_REG(0x2300ac7f);
            break;
        case GX_PERF0_TRIANGLES_2TEX:
            GX_WRITE_RA_REG(0x2300acbf);
            break;
        case GX_PERF0_TRIANGLES_3TEX:
            GX_WRITE_RA_REG(0x2300acff);
            break;

        case GX_PERF0_TRIANGLES_4TEX:
            GX_WRITE_RA_REG(0x2300ad3f);
            break;

        case GX_PERF0_TRIANGLES_5TEX:
            GX_WRITE_RA_REG(0x2300ad7f);
            break;

        case GX_PERF0_TRIANGLES_6TEX:
            GX_WRITE_RA_REG(0x2300adbf);
            break;

        case GX_PERF0_TRIANGLES_7TEX:
            GX_WRITE_RA_REG(0x2300adff);
            break;

        case GX_PERF0_TRIANGLES_8TEX:
            GX_WRITE_RA_REG(0x2300ae3f);
            break;

        case GX_PERF0_TRIANGLES_0CLR:
            GX_WRITE_RA_REG(0x2300a27f);
            break;

        case GX_PERF0_TRIANGLES_1CLR:
            GX_WRITE_RA_REG(0x2300a67f);
            break;

        case GX_PERF0_TRIANGLES_2CLR:
            GX_WRITE_RA_REG(0x2300aa7f);
            break;

        case GX_PERF0_QUAD_0CVG:
            GX_WRITE_RA_REG(0x2402c0c6);
            break;

        case GX_PERF0_QUAD_NON0CVG:
            GX_WRITE_RA_REG(0x2402c16b);
            break;

        case GX_PERF0_QUAD_1CVG:
            GX_WRITE_RA_REG(0x2402c0e7);
            break;

        case GX_PERF0_QUAD_2CVG:
            GX_WRITE_RA_REG(0x2402c108);
            break;

        case GX_PERF0_QUAD_3CVG:
            GX_WRITE_RA_REG(0x2402c129);
            break;

        case GX_PERF0_QUAD_4CVG:
            GX_WRITE_RA_REG(0x2402c14a);
            break;

        case GX_PERF0_AVG_QUAD_CNT:
            GX_WRITE_RA_REG(0x2402c1ad);
            break;
        case GX_PERF0_NONE:
            break;

        default:
            break;
    }

    gx->perf1 = perf1;

    switch (gx->perf1) {
        case GX_PERF1_TEXELS:
            GX_WRITE_RA_REG(0x67000042);
            break;

        case GX_PERF1_TX_IDLE:
            GX_WRITE_RA_REG(0x67000084);
            break;

        case GX_PERF1_TX_REGS:
            GX_WRITE_RA_REG(0x67000063);
            break;

        case GX_PERF1_TX_MEMSTALL:
            GX_WRITE_RA_REG(0x67000129);
            break;

        case GX_PERF1_TC_MISS:
            GX_WRITE_RA_REG(0x67000252);
            break;

        case GX_PERF1_CLOCKS:
            GX_WRITE_RA_REG(0x67000021);
            break;

        case GX_PERF1_TC_CHECK1_2:
            GX_WRITE_RA_REG(0x6700014b);
            break;

        case GX_PERF1_TC_CHECK3_4:
            GX_WRITE_RA_REG(0x6700018d);
            break;

        case GX_PERF1_TC_CHECK5_6:
            GX_WRITE_RA_REG(0x670001cf);
            break;

        case GX_PERF1_TC_CHECK7_8:
            GX_WRITE_RA_REG(0x67000211);
            break;

        case GX_PERF1_VC_ELEMQ_FULL:
            SC_CP_STAT_SEL_REG_SET_STALLPERF_SEL(gx->perfSel, ELEMQ_FULL);
            GX_WRITE_CP_STRM_REG(CP_STAT_SEL, 0, gx->perfSel);
            break;

        case GX_PERF1_VC_MISSQ_FULL:
            SC_CP_STAT_SEL_REG_SET_STALLPERF_SEL(gx->perfSel, MISSQ_FULL);
            GX_WRITE_CP_STRM_REG(CP_STAT_SEL, 0, gx->perfSel);
            break;

        case GX_PERF1_VC_MEMREQ_FULL:
            SC_CP_STAT_SEL_REG_SET_STALLPERF_SEL(gx->perfSel, MEMREQ_FULL);
            GX_WRITE_CP_STRM_REG(CP_STAT_SEL, 0, gx->perfSel);
            break;

        case GX_PERF1_VC_STATUS7:
            SC_CP_STAT_SEL_REG_SET_STALLPERF_SEL(gx->perfSel, VC_STATCNT7);
            GX_WRITE_CP_STRM_REG(CP_STAT_SEL, 0, gx->perfSel);
            break;

        case GX_PERF1_VC_MISSREP_FULL:
            SC_CP_STAT_SEL_REG_SET_STALLPERF_SEL(gx->perfSel, VC_MISS_REP_FULL);
            GX_WRITE_CP_STRM_REG(CP_STAT_SEL, 0, gx->perfSel);
            break;

        case GX_PERF1_VC_STREAMBUF_LOW:
            SC_CP_STAT_SEL_REG_SET_STALLPERF_SEL(gx->perfSel, VC_STALL_STMBUFLOW);
            GX_WRITE_CP_STRM_REG(CP_STAT_SEL, 0, gx->perfSel);
            break;

        case GX_PERF1_VC_ALL_STALLS:
            SC_CP_STAT_SEL_REG_SET_STALLPERF_SEL(gx->perfSel, ALL_STALL);
            GX_WRITE_CP_STRM_REG(CP_STAT_SEL, 0, gx->perfSel);
            break;

        case GX_PERF1_VERTICES:
            SC_CP_STAT_SEL_REG_SET_STALLPERF_SEL(gx->perfSel, VTX_CNT);
            GX_WRITE_CP_STRM_REG(CP_STAT_SEL, 0, gx->perfSel);
            break;

        case GX_PERF1_FIFO_REQ:
            reg = CP_REG_MEMPERFSEL(DFIFO_REQ_CNT);
            GX_CP_REG_WRITE_U16(CP_MEMPERF_SEL, reg);
            break;

        case GX_PERF1_CALL_REQ:
            reg = CP_REG_MEMPERFSEL(OBJCALL_REQ_CNT);
            GX_CP_REG_WRITE_U16(CP_MEMPERF_SEL, reg);
            break;

        case GX_PERF1_VC_MISS_REQ:
            reg = CP_REG_MEMPERFSEL(VCMISS_REQ_CNT);
            GX_CP_REG_WRITE_U16(CP_MEMPERF_SEL, reg);
            break;

        case GX_PERF1_CP_ALL_REQ:
            reg = CP_REG_MEMPERFSEL(ALL_MEMREQ_CNT);
            GX_CP_REG_WRITE_U16(CP_MEMPERF_SEL, reg);
            break;

        case GX_PERF1_NONE:
            break;

        default:
            break;
    }

    gx->bpSentNot = GX_FALSE;
}

void GXClearGPMetric(void) {
    GX_CP_REG_WRITE_U16(2, 4);
}

void GXReadXrRasMetric(u32 *xf_wait_in, u32* xf_wait_out, u32* ras_busy, u32* clocks) {
    *ras_busy = GX_CP_COUNTER_READ_U32(CP_COUNTER0);
    *clocks = GX_CP_COUNTER_READ_U32(CP_COUNTER1);
    *xf_wait_in = GX_CP_COUNTER_READ_U32(CP_COUNTER2);
    *xf_wait_out = GX_CP_COUNTER_READ_U32(CP_COUNTER3);
}
