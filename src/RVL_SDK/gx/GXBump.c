#include <revolution/gx.h>
#include <revolution/gx/GXRegs.h>

void GXSetTevIndirect(GXTevStageID tev_stage, GXIndTexStageID ind_stage, GXIndTexFormat format, 
    GXIndTexBiasSel bias_sel, GXIndTexMtxID matrix_sel, GXIndTexWrap wrap_s, 
    GXIndTexWrap wrap_t, GXBool add_prev, GXBool utc_lod, GXIndTexAlphaSel alpha_sel){
        u32 reg = 0;
        SC_BP_CMD_SET_BT(reg, ind_stage);
        SC_BP_CMD_SET_FMT(reg, format);
        SC_BP_CMD_SET_BIAS(reg, bias_sel);
        SC_BP_CMD_SET_BS(reg, alpha_sel);
        SC_BP_CMD_SET_M(reg, matrix_sel);
        SC_BP_CMD_SET_SW(reg, wrap_s);
        SC_BP_CMD_SET_TW(reg, wrap_t);
        SC_BP_CMD_SET_LB(reg, utc_lod);
        SC_BP_CMD_SET_FB(reg, add_prev);
        SC_BP_CMD_SET_RID(reg, (0x10 + tev_stage));
        GX_WRITE_RA_REG(reg);
        gx->bpSentNot = GX_FALSE;
}

void GXSetIndTexMtx(GXIndTexMtxID mtx_id, const f32 offset[2][3], s8 scale_exp)
{
    s32 mtx[6];
    u32 reg, id;

    switch (mtx_id) {
        case GX_ITM_0:
        case GX_ITM_1:
        case GX_ITM_2:
            id = (u32)(mtx_id - GX_ITM_0);
            break;
        case GX_ITM_S0:
        case GX_ITM_S1:
        case GX_ITM_S2:
            id = (u32)(mtx_id - GX_ITM_S0);
            break;
        case GX_ITM_T0:
        case GX_ITM_T1:
        case GX_ITM_T2:
            id = (u32)(mtx_id - GX_ITM_T0);
            break;
        default:
            id = 0;
            break;
    }
    
    mtx[0] = ((s32)(offset[0][0] * (f32)(1 << 10))) & 0x7FF;
    mtx[1] = ((s32)(offset[1][0] * (f32)(1 << 10))) & 0x7FF;
    scale_exp += 0x11;
    
    reg = 0;
    SC_BP_MTXA_SET_MA(reg, mtx[0]);
    SC_BP_MTXA_SET_MB(reg, mtx[1]);
    SC_BP_MTXA_SET_S(reg, (scale_exp & 0x3));
    SC_BP_MTXA_SET_RID(reg, 0x6 + id * 3);
    GX_WRITE_RA_REG(reg);
    
    mtx[2] = ((s32)(offset[0][1] * (f32)(1 << 10))) & 0x7FF;
    mtx[3] = ((s32)(offset[1][1] * (f32)(1 << 10))) & 0x7FF;
    reg = 0;
    SC_BP_MTXB_SET_MC(reg, mtx[2]);
    SC_BP_MTXB_SET_MD(reg, mtx[3]);
    SC_BP_MTXB_SET_S(reg, ((scale_exp >> 2) & 0x3));
    SC_BP_MTXB_SET_RID(reg, 0x7 + id * 3);
    GX_WRITE_RA_REG(reg);

    mtx[4] = ((s32)(offset[0][2] * (f32)(1 << 10))) & 0x7FF;
    mtx[5] = ((s32)(offset[1][2] * (f32)(1 << 10))) & 0x7FF;
    reg = 0;
    SC_BP_MTXC_SET_ME(reg, mtx[4]);
    SC_BP_MTXC_SET_MF(reg, mtx[5]);
    SC_BP_MTXC_SET_S(reg, ((scale_exp >> 4) & 0x3));
    SC_BP_MTXC_SET_RID(reg, 0x8 + id * 3);
    GX_WRITE_RA_REG(reg);
    gx->bpSentNot = GX_FALSE;
}

void __GXUpdateBPMask(void) {
    return;
}
