#include "revolution/gd/GDIndirect.h"
#include "revolution/gd.h"

void GDSetTevIndirect(GXTevStageID tev_stage, GXIndTexStageID ind_stage, GXIndTexFormat format, GXIndTexBiasSel bias_sel, GXIndTexMtxID matrix_sel,
                      GXIndTexWrap wrap_s, GXIndTexWrap wrap_t, GXBool add_prev, GXBool utc_lod, GXIndTexAlphaSel alpha_sel) {
    GDWriteBPCmd(IND_CMD((ind_stage & 3), (format & 3), (bias_sel & 7), (alpha_sel & 3), (matrix_sel & 0x0F), (wrap_s & 7), (wrap_t & 7),
                         (utc_lod & 1), (add_prev & 1), (IND_CMD0_ID + (tev_stage & 0x0F))));
}

void GDSetTevDirect(GXTevStageID tev_stage) {
    GDSetTevIndirect(tev_stage, GX_INDTEXSTAGE0, GX_ITF_8, GX_ITB_NONE, GX_ITM_OFF, GX_ITW_OFF, GX_ITW_OFF, GX_FALSE, GX_FALSE, GX_ITBA_OFF);
}
