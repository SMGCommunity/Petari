#include <revolution/gx.h>
#include <revolution/gx/GXTypes.h>
#include <revolution/gx/GXRegs.h>

void GXSetScissor(u32 left, u32 top, u32 width, u32 height) {
    u32 _top, _left, bottom, right;

    _top = top + (u32)342.0f;
    _left = left + (u32)342.0f;
    bottom = _top + height - 1;
    right = _left + width - 1;

    SET_FLAG(gx->suScis0, _top, 0, 11);
    SET_FLAG(gx->suScis0, _left, 12, 11);

    SET_FLAG(gx->suScis1, bottom, 0, 11);
    SET_FLAG(gx->suScis1, right, 12, 11);

    GX_WRITE_REG(gx->suScis0);
    GX_WRITE_REG(gx->suScis1);
    gx->bpSentNot = GX_FALSE;
}
